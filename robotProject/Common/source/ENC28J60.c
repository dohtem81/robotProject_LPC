#include "../headers/lpc21xx.h"
#include "../headers/ENC28J60.h"
#include "../headers/spi.h"
#include "../headers/types.h"
#include "../headers/ethernet.h"


TXSTATUS TxStatus;
unsigned int busyFlag = 0;


////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
static void enc_resetMac(void)
{
	unsigned char bytOpcode = RESET_OP;
	enc_selMac(TRUE);              // ENC CS low
	spi_write(0, &bytOpcode, 1);     // Tx opcode and address
	enc_selMac(FALSE);
}



////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
void enc_initialise(void){
	enc_resetMac();       // resets MAC by writing 0xff to control register

                    // setup memory by defining ERXST and ERXND
	enc_bankSel(CR_BANK0);       // select bank 0
	enc_writeCtrlReg(ERXSTL,(unsigned char)( RXSTART & 0x00ff));
	enc_writeCtrlReg(ERXSTH,(unsigned char)((RXSTART & 0xff00)>> 8));
	enc_writeCtrlReg(ERXNDL,(unsigned char)( RXEND   & 0x00ff));
	enc_writeCtrlReg(ERXNDH,(unsigned char)((RXEND   & 0xff00)>>8));

                    // Make sure Rx Read ptr is at the start of Rx segment
	enc_writeCtrlReg(ERXRDPTL, (unsigned char)( RXSTART & 0x00ff));
	enc_writeCtrlReg(ERXRDPTH, (unsigned char)((RXSTART & 0xff00)>> 8));

	enc_bankSel(CR_BANK1);                             // select bank 1
	enc_writeCtrlReg(ERXFCON, 0);


                // Initialise the MAC registers
	enc_bankSel(CR_BANK2);                             // select bank 2
	enc_setBitField(MACON1, MACON1_MARXEN);     // Enable reception of frames
	enc_writeCtrlReg(MACLCON2, 63);
	enc_writeCtrlReg(MACON3, MACON3_FRMLNEN +    // Type / len field will be checked
                      MACON3_TXCRCEN +    // MAC will append valid CRC
                      MACON3_PADCFG0 +
                      MACON3_FULDPX);    // All small packets will be padded


	enc_setBitField(MACON4, MACON4_DEFER);
	enc_writeCtrlReg(MAMXFLL, (unsigned char)( ETH_MAX_FLEN & 0x00ff));     // set max frame len
	enc_writeCtrlReg(MAMXFLH, (unsigned char)((ETH_MAX_FLEN & 0xff00)>>8));
	enc_writeCtrlReg(MABBIPG, 0x15);             // back to back interpacket gap. set as per data sheet
	enc_writeCtrlReg(MAIPGL , 0x12);             // non back to back interpacket gap. set as per data sheet
	enc_writeCtrlReg(MAIPGH , 0x0C);


    //set up MAC address
	enc_bankSel(CR_BANK3);
	enc_writeCtrlReg(MAADR1, (ETH_MAC1 & 0xff00) >> 8);
	enc_writeCtrlReg(MAADR2, ETH_MAC1 & 0xff);
	enc_writeCtrlReg(MAADR3, (ETH_MAC2 & 0xff00) >> 8);
	enc_writeCtrlReg(MAADR4, ETH_MAC2 >> 8);
	enc_writeCtrlReg(MAADR5, (ETH_MAC3 & 0xff00) >> 8);
	enc_writeCtrlReg(MAADR6, ETH_MAC3 >> 8);

	//set up PHY registes
	enc_writePhyReg(PHCON1, PHCON1_PDPXMD);
	//enc_writePhyReg(PHCON2, PHCON2_HDLDIS);
	enc_writeCtrlReg(ECON1,  ECON1_RXEN);     //Enable the chip for reception of packets
}



////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
unsigned short enc_write(unsigned char * ptrBuffer, unsigned short ui_Len){
	volatile unsigned short i;
	volatile unsigned short address = TXSTART;
	unsigned char  bytControl;

	bytControl = 0x00;


	enc_bankSel(CR_BANK0);                                          // select bank 0
	enc_writeCtrlReg(ETXSTL,(unsigned char)( TXSTART & 0x00ff));        // write ptr to start of Tx packet
	enc_writeCtrlReg(ETXSTH,(unsigned char)((TXSTART & 0xff00)>>8));

	enc_writeCtrlReg(EWRPTL,(unsigned char)( TXSTART & 0x00ff));        // Set write buffer to point to start of Tx Buffer
	enc_writeCtrlReg(EWRPTH,(unsigned char)((TXSTART & 0xff00)>>8));

	enc_writeMacBuffer(&bytControl,1);                       // write per packet control byte
  	address++;

  	address += enc_writeMacBuffer(ptrBuffer, ui_Len);          // write packet. Assume correct formating src, dst, type  + data

  	enc_writeCtrlReg(ETXNDL, (unsigned char)( address & 0x00ff));       // Tell MAC when the end of the packet is
  	enc_writeCtrlReg(ETXNDH, (unsigned char)((address & 0xff00)>>8));


  	enc_clrBitField(EIR,EIR_TXIF);
  	//enc_setBitField(EIE, EIE_TXIE |EIE_INTIE);

  	enc_setBitField(ECON1, ECON1_TXRST);
  	enc_clrBitField(ECON1, ECON1_TXRST);
  	enc_clrBitField(EIR, EIR_TXERIF | EIR_TXIF);
  	enc_setBitField(ECON1, ECON1_TXRTS);                     // begin transmitting;

  	//while (!(enc_readEthReg(EIR) & (EIR_TXIF)));             // kill some time. Note: Nice place to block?

  	enc_clrBitField(ECON1, ECON1_TXRTS);

  	enc_bankSel(CR_BANK0);                       //read tx status bytes
  	address++;                                          // increment ptr to address to start of status struc
  	enc_writeCtrlReg(ERDPTL, (unsigned char)( address & 0x00ff));      // Setup the buffer read ptr to read status struc
  	enc_writeCtrlReg(ERDPTH, (unsigned char)((address & 0xff00)>>8));
  	enc_readMacBuffer(&TxStatus.v[0],7);

  	if (enc_readEthReg(ESTAT) & ESTAT_TXABRT){                // did transmission get interrupted?
  		if (TxStatus.bits.LateCollision){
  			enc_clrBitField(ECON1, ECON1_TXRTS);
  			enc_setBitField(ECON1, ECON1_TXRTS);

  			enc_clrBitField(ESTAT,ESTAT_TXABRT | ESTAT_LATECOL);
  		}
  		enc_clrBitField(EIR, EIR_TXERIF | EIR_TXIF);
  		enc_clrBitField(ESTAT,ESTAT_TXABRT);

  		return FALSE;                                          // packet transmit failed. Inform calling function
  	} else                                                         // calling function may inquire why packet failed by calling [TO DO] function
  		return TRUE;                                           // all fan dabby dozy
}



////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
unsigned char enc_read(unsigned char * ptrBuffer) {
//	unsigned short pckLen,test;
//	unsigned short rdptr, wrtptr;
	static unsigned short nextpckptr = RXSTART;
	RXSTATUS ptrRxStatus;
	unsigned char bytPacket;

	enc_bankSel(CR_BANK1);

	bytPacket = enc_readEthReg(EPKTCNT);          // How many packets have been received

	if(bytPacket == 0)
		return 0;                       // No full packets received

	enc_bankSel(CR_BANK0);

	enc_writeCtrlReg(ERDPTL,(unsigned char)(nextpckptr & 0x00ff));   //write this value to read buffer ptr
	enc_writeCtrlReg(ERDPTH,(unsigned char)((nextpckptr & 0xff00)>>8));

	enc_readMacBuffer((unsigned char*)&ptrRxStatus.v[0],6);             // read next packet ptr + 4 status bytes
	nextpckptr = ptrRxStatus.bits.NextPacket;

	enc_readMacBuffer(ptrBuffer,ptrRxStatus.bits.ByteCount);   // read packet into buffer
                                        // ptrBuffer should now contain a MAC packet

	//now byte swap should be performed
	unsigned short buffer;
	unsigned short rem_len;
	rem_len = ptrRxStatus.bits.ByteCount;
	while (rem_len > 1){
		rem_len -= 2;
		buffer = swapByte((unsigned short *)ptrBuffer);
		*ptrBuffer++ = (unsigned char)(buffer & 0x00ff);
		*ptrBuffer++ = (unsigned char)((buffer & 0xff00) >> 8);
	}

	enc_bankSel(CR_BANK0);
	enc_writeCtrlReg(ERXRDPTL,ptrRxStatus.v[0]);  // free up ENC memory my adjustng the Rx Read ptr
	enc_writeCtrlReg(ERXRDPTH,ptrRxStatus.v[1]);

	// decrement packet counter
	enc_setBitField(ECON2, ECON2_PKTDEC);


	return bytPacket;
}



////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
unsigned char enc_readEthReg(unsigned char bytAddress) {
	unsigned char bytData;

	if (bytAddress > 0x1F)
		return FALSE;                 // address invalid, [TO DO]

	enc_selMac(TRUE);                 // ENC CS low
	spi_write(0, &bytAddress, 1);       // write opcode
	spi_read(0, &bytData, 1);          // read value
	enc_selMac(FALSE);

	return bytData;

}



////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
static unsigned char enc_readMacReg(unsigned char bytAddress)
{
	unsigned char bytData;

	if (bytAddress > 0x1F)
		return FALSE;                 // address invalid, [TO DO]

	enc_selMac(TRUE);                 // ENC CS low
	spi_write(0, &bytAddress,1);    // write opcode
	spi_read(0, &bytData, 1);       // read dummy byte
	spi_read(0, &bytData, 1);       // read value
	enc_selMac(FALSE);

	return bytData;
}



////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
static unsigned char enc_writePhyReg(unsigned char address, unsigned short data)
{
	if (address > 0x14)
		return FALSE;

	enc_bankSel(CR_BANK2);

	enc_writeCtrlReg(MIREGADR,address);        // Write address of Phy reg
	enc_writeCtrlReg(MIWRL,(unsigned char)data);        // lower phy data
	enc_writeCtrlReg(MIWRH,((unsigned char)(data >>8)));    // Upper phydata

	return TRUE;
}



////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
static unsigned short enc_readPhyReg(unsigned char address)
{
	unsigned short uiData;
	unsigned char bytStat;

	enc_bankSel(CR_BANK2);
	enc_writeCtrlReg(MIREGADR,address);    // Write address of phy register to read
	enc_setBitField(MICMD, MICMD_MIIRD);  // Set rd bit
	do {
		bytStat = enc_readMacReg(MISTAT);
	} while(bytStat & MISTAT_BUSY);

	enc_clrBitField(MICMD,MICMD_MIIRD);   // Clear rd bit
	uiData = (unsigned short)enc_readMacReg(MIRDL);       // Read low data byte
	uiData |=((unsigned short)enc_readMacReg(MIRDH)<<8); // Read high data byte

	return uiData;
}



////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
static unsigned char enc_writeCtrlReg(unsigned char bytAddress,unsigned char bytData) {
	if (bytAddress > 0x1f)
		return FALSE;

	bytAddress |= WCR_OP;       // Set opcode
	enc_selMac(TRUE);              // ENC CS low
	spi_write(0, &bytAddress, 1);    // Tx opcode and address
	spi_write(0, &bytData, 1);       // Tx data
	enc_selMac(FALSE);

	return TRUE;
}



////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
static unsigned short enc_readMacBuffer(unsigned char * bytBuffer,unsigned char byt_length)
{
	unsigned char bytOpcode;
	unsigned short buffer;
	unsigned short len, rem_len;

	bytOpcode = RBM_OP;
	enc_selMac(TRUE);            // ENC CS low
	spi_write(0, &bytOpcode, 1);   // Tx opcode
	len = byt_length;
	spi_read(0, bytBuffer, byt_length);
	rem_len = len;
	enc_selMac(FALSE);           // release CS

	return len;

}



////////////////////////////////////////////////////////////////////////
//
//	writes data into MAC buffer
//	if length is > 1, byteswap will be performed on all bytes
//
////////////////////////////////////////////////////////////////////////
static unsigned short enc_writeMacBuffer(unsigned char * bytBuffer,unsigned short ui_len)
{
	unsigned char bytOpcode;
	unsigned short bytData;
	int ix;

	bytOpcode = WBM_OP;
	enc_selMac(TRUE);            // ENC CS low
	spi_write(0, &bytOpcode, 1);   // Tx opcode
	if (ui_len == 1)
		spi_write(0, bytBuffer, ui_len);   // read bytes into buffer
	else {
		for (ix = 0; ix < ui_len; ix += 2){
			//bytData = swapByte((unsigned short*)bytBuffer);
			bytData = (*bytBuffer << 8) | (*(bytBuffer+1));
			spi_write(0, (unsigned char*)(&bytData), 2);
			bytBuffer += 2;
		}
	}
	enc_selMac(FALSE);           // release CS

	return ui_len;

}



////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
static unsigned char enc_setBitField(unsigned char bytAddress, unsigned char bytData)
{
	if (bytAddress > 0x1f)
		return FALSE;

	bytAddress |= BFS_OP;       // Set opcode
	enc_selMac(TRUE);              // ENC CS low
	spi_write(0, &bytAddress,1);    // Tx opcode and address
	spi_write(0, &bytData,1);       // Tx data
	enc_selMac(FALSE);

	return TRUE;
}



////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
static unsigned char enc_clrBitField(unsigned char bytAddress, unsigned char bytData)
{
	if (bytAddress > 0x1f)
		return FALSE;

	bytAddress |= BFC_OP;       // Set opcode
	enc_selMac(TRUE);              // ENC CS low
	spi_write(0, &bytAddress, 1);    // Tx opcode and address
	spi_write(0, &bytData, 1);       // Tx data
	enc_selMac(FALSE);

	return TRUE;

}



////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
static void enc_bankSel(unsigned char bank)
{
	unsigned char temp;

	if (bank >3)
		return;

	temp = enc_readEthReg(ECON1);       // Read ECON1 register
	temp &= ~ECON1_BSEL;            // mask off the BSEL bits
	temp |= bank;                   // set BSEL bits
	enc_writeCtrlReg(ECON1, temp);       // write new values back to ENC28J60
}


////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
inline void enc_selMac(unsigned char bool){
	(bool == TRUE) ? (IO0CLR = CS_ENC28J60) : (IO0SET = CS_ENC28J60);
}


///////////////////////////////////////////////////////////////////////
//
//	this function broadcasts on ethernet
//	ptr - pointer where to start
//	length - how many words should be sent
//////////////////////////////////////////////////////////////////////
unsigned int ethernetBroadcast(unsigned short *ptr, int length){
	if ( busyFlag == 0 ) {
		busyFlag = 1;
		enc_write((unsigned char *)ptr, (2*length));	//conversion from words to bytes
		busyFlag = 0;
	}

	return busyFlag;
}

///////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
unsigned int enc_getStatus(){

	return busyFlag;
}
