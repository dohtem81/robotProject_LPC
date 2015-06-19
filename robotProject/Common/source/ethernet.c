/*
 * ethernet.c
 *
 *  Created on: 2010-07-12
 *      Author: Piit
 */

#include "../headers/lpc21xx.h"
#include "../headers/ethernet.h"
#include "../headers/ENC28J60.h"	//for ethernetBroadcast definition

//frame varaible
unsigned short txETHFrame[ETH_MAX_FLEN];
unsigned short rxETHFrame[ETH_MAX_FLEN];
struct _arpFrame ARPFrame;
struct _ETHFrameHeader ETHFrameHeader;
struct _IPFrame IPFrame;
struct _arpTable arpTable[ARP_MAX_STORED];


void wait(int ticks){
	long int i;

	for (i=0; i<ticks*250; i++);
	return;
}


//////////////////////////////////////////////////////////////////
//
//	this function claculates checksum within a range 
//		starting from specified pointer (ptr)
//		ingroing byte indicated by pointer (byteCount)
//		specified amount of bytes (including ignored) (ignore) 
//	returns calculated checksum
//////////////////////////////////////////////////////////////////
unsigned short calcCRC(unsigned short* ptr, unsigned short* ignore, unsigned int byteCount){
	unsigned long result;
	unsigned int index;
	
	result = 0;
	index = 0;
	//calculates result of all fields except indicated to ignore
	while (index++ < byteCount){
		if (ptr != ignore)		//if ptr does not point at ignore address
			result += *ptr;	//add value pointed by ptr to result
		ptr++;
	}
	
	while (result != (result & 0xffff))
		result = (result & 0xffff) + (result >> 16);
	
	return ~result;
}


//////////////////////////////////////////////////////////////////
//
//	this function initializes main ethernet frame
//	sets up all structures to point into frame body
//	sets up source mac address
//////////////////////////////////////////////////////////////////

void initializeFrame(){
	unsigned short *ix;
	
	//sets up pointers for structures - general frame
	ETHFrameHeader.destMAC		=	txETHFrame;
	ETHFrameHeader.sourceMAC	=	txETHFrame + 3;
	
	//sets up pointers for arp structure
	ix							=	txETHFrame + 6;
	ARPFrame.type				=	ix++;
	ARPFrame.arpReqSetting		=	ix++;
	ARPFrame.protocolType		=	ix++;
	ARPFrame.hdAndProSize		=	ix++;
	ARPFrame.gratuitous			=	ix++;
	ARPFrame.sourceMAC			=	ix;
	ARPFrame.sourceIP			=	(ix += 3);
	ARPFrame.destMAC			=	(ix += 2);
	ARPFrame.destIP				=	(ix += 3);
	
	//sets up pointers for ip structure (outgoing)
	ix							=	txETHFrame + 6;
	IPFrame.type						=	ix++;
	IPFrame.hvdiff						=	ix++;		//header length, version & differentiated serv fields
	IPFrame.totalLength					=	ix++;
	IPFrame.h_id						=	ix++;	
	IPFrame.flagsandoff					=	ix++;		
	IPFrame.ttlprot						=	ix++;		//ttl and protocol
	IPFrame.h_checksum					= 	ix++;		//header checksum
	//message
	IPFrame.sourceIP					=	ix;
	IPFrame.destIP						=	(ix += 2);
	ix 									+= 	2;			//adjust pointer!
	IPFrame.typecode					=	ix++;							//used for TCP
	IPFrame.d_checksumTCP				= 	ix++;		//data checksum		//used for TCP
	IPFrame.d_id						=	ix++;							//used for TCP
	IPFrame.seq_no						=	ix++;		//sequence number	//used for TCP
	IPFrame.dataTCP						=	ix;								//used for TCP
	
	//UDP datagrams - difference starts from typecode
	ix									=	IPFrame.typecode;	//setting up pointer onemore time after IP addresses
	IPFrame.srcPort						=	ix++;
	IPFrame.dstPort						=	ix++;
	IPFrame.length						=	ix++;
	IPFrame.d_checksumUDP				= 	ix++;		//data checksum
	IPFrame.dataUDP						=	ix;
	
	//sets up frame to be used
	*ETHFrameHeader.sourceMAC			=	ETH_MAC1;
	*(ETHFrameHeader.sourceMAC+1)		=	ETH_MAC2;
	*(ETHFrameHeader.sourceMAC+2)		=	ETH_MAC3;
	
	//sets up ARP table
	int i;
	for (i=0; i<ARP_MAX_STORED; i++) {
		arpTable[i].MAC_OTHER0		=	0x0000;
		arpTable[i].MAC_OTHER1		=	0x0000;
		arpTable[i].MAC_OTHER2		=	0x0000;
		arpTable[i].IP_OTHER0		=	0x0000;
		arpTable[i].IP_OTHER1		=	0x0000;
	}
}


//////////////////////////////////////////////////////////////////
//
//	this function sends ARP frame
//	
//////////////////////////////////////////////////////////////////

unsigned int sendARP(void){
	unsigned short tAdd1, tAdd2, tAdd3, *tptr, *structPointer;
	unsigned int idx, handler;
	
	*ETHFrameHeader.destMAC			=	0xffff;
	*(ETHFrameHeader.destMAC+1)		=	0xffff;
	*(ETHFrameHeader.destMAC+2)		=	0xffff;
	
	//sorce is already written during initialization
	
	*ARPFrame.type 					= 	TYPE_ARP;
	*ARPFrame.arpReqSetting			=	ARP_ETH;
	*ARPFrame.protocolType			=	TYPE_IP;
	*ARPFrame.hdAndProSize			=	0x0604;
	*ARPFrame.gratuitous			=	ARPOP_REQ;
	*ARPFrame.sourceMAC				=	ETH_MAC1;
	*(ARPFrame.sourceMAC+1)			=	ETH_MAC2;
	*(ARPFrame.sourceMAC+2)			=	ETH_MAC3;
	*ARPFrame.sourceIP				=	(IP1 << 8) | IP2;
	*(ARPFrame.sourceIP+1)			=	(IP3 << 8) | IP4;
	*ARPFrame.destMAC				=	0xffff;
	*(ARPFrame.destMAC+1)			=	0xffff;
	*(ARPFrame.destMAC+2)			=	0xffff;
	*ARPFrame.destIP				=	(SERVER_IP1 << 8) | SERVER_IP2;	//TODO set address based on what
	*(ARPFrame.destIP+1)			=	(SERVER_IP3 << 8) | SERVER_IP4;	//		your are looking for

	return ethernetBroadcast(txETHFrame, 21);
}

//////////////////////////////////////////////////////////////////
//
//	this function responds to ARP frame
//	
//////////////////////////////////////////////////////////////////

unsigned int respondToARP(unsigned short *dMAC, unsigned short *dIP){
	
	*ETHFrameHeader.destMAC			=	*dMAC;
	*(ETHFrameHeader.destMAC+1)		=	*(dMAC+1);
	*(ETHFrameHeader.destMAC+2)		=	*(dMAC+2);
	
	//sorce is already written during initialization
	
	*ARPFrame.type 					= 	TYPE_ARP;
	*ARPFrame.arpReqSetting			=	ARP_ETH;
	*ARPFrame.protocolType			=	TYPE_IP;
	*ARPFrame.hdAndProSize			=	0x0604;
	*ARPFrame.gratuitous			=	ARPOP_RPY;
	*ARPFrame.sourceMAC				=	ETH_MAC1;
	*(ARPFrame.sourceMAC+1)			=	ETH_MAC2;
	*(ARPFrame.sourceMAC+2)			=	ETH_MAC3;
	*ARPFrame.sourceIP				=	(IP1 << 8) | IP2;
	*(ARPFrame.sourceIP+1)			=	(IP3 << 8) | IP4;
	*ARPFrame.destMAC				=	*dMAC;
	*(ARPFrame.destMAC+1)			=	*(dMAC+1);
	*(ARPFrame.destMAC+2)			=	*(dMAC+2);
	*ARPFrame.destIP				=	*dIP;
	*(ARPFrame.destIP+1)			=	*(dIP+1);

	return ethernetBroadcast(txETHFrame, 21);
}

///////////////////////////////////////////////////////////////////////
//
//	this function swaps bytes
//
//	ptr - pointer to beginning of the table
//	length - how many words to be swapped
//////////////////////////////////////////////////////////////////

void swapBytes(unsigned short *ptr, int length){
	int ix, idx;
	
	for (ix=length; ix>0; ix++)
		*ptr = ((*ptr & 0x00ff) << 8) + ((*ptr++ & 0xff00) >> 8);

}


///////////////////////////////////////////////////////////////////////
//
//	this function swaps one byte and returns value
//
//	ptr - pointer to byte to be swapped
//////////////////////////////////////////////////////////////////

unsigned short swapByte(unsigned short *ptr){
	return ((*ptr & 0x00ff) << 8) + ((*ptr++ & 0xff00) >> 8);

}

///////////////////////////////////////////////////////////////////////
//
//	gets source MAC from ETH frame
//	index - points rx frame to look at
//	returns pointer to beginning of MAC address
//
//////////////////////////////////////////////////////////////////////
unsigned short* getETHSourceMAC(void){
	return (rxETHFrame + 3);
}



///////////////////////////////////////////////////////////////////////
//
//	gets destination MAC from ETH frame
//	index - points rx frame to look at
//	returns pointer to beginning of MAC address
//
//////////////////////////////////////////////////////////////////////
unsigned short* getETHDestMAC(void){
	return (rxETHFrame);
}



///////////////////////////////////////////////////////////////////////
//
//	gets source MAC from ARP frame
//	index - points rx frame to look at
//	returns pointer to beginning of MAC address
//
//////////////////////////////////////////////////////////////////////
unsigned short* getARPSourceMAC(void){
	return (rxETHFrame + 11);
}



///////////////////////////////////////////////////////////////////////
//
//	gets source IP from ARP frame
//	index - points rx frame to look at
//	returns pointer to beginning of IP address
//
//////////////////////////////////////////////////////////////////////
unsigned short* getARPSourceIP(void){
	return (rxETHFrame + 14);
}



///////////////////////////////////////////////////////////////////////
//
//	gets source IP from IP frame
//	index - points rx frame to look at
//	returns pointer to beginning of IP address
//
//////////////////////////////////////////////////////////////////////
unsigned short * getIPSourceIP(void) {
	return (rxETHFrame + 13);
}



///////////////////////////////////////////////////////////////////////
//
//	returns 0 if frame in not an ARP frame
//	returns 1 if frame is ARP request
//	returns 2 if frame is ARP response
//
//////////////////////////////////////////////////////////////////////
unsigned short isARPFrame(unsigned short* ptr){
	if (*(ptr + 6) == TYPE_ARP) return (*(ptr + 10));
	else return 0;
}


///////////////////////////////////////////////////////////////////////
//
//	returns 0 if frame in not an IP frame
//	returns 1 if frame is IP frame
//
//////////////////////////////////////////////////////////////////////
unsigned short* isIPFrame(void){
	if (*(rxETHFrame + 6) == TYPE_IP) return (rxETHFrame + 7);
	else return 0;
}


///////////////////////////////////////////////////////////////////////
//
//	sends ping response
//
//////////////////////////////////////////////////////////////////////
unsigned int sendICMPResponse(unsigned short * dMAC, unsigned short *ptr){
	unsigned short ixData;
	
	*ETHFrameHeader.destMAC			=	*dMAC;
	*(ETHFrameHeader.destMAC+1)		=	*(dMAC+1);
	*(ETHFrameHeader.destMAC+2)		=	*(dMAC+2);
	
	//sorce is already written during initialization
	
	*IPFrame.type					=	TYPE_IP;
	*IPFrame.hvdiff					=	0x4500;		//header length, version & differentiated serv fields
	*IPFrame.totalLength			=	*(ptr + 1);
	*IPFrame.h_id					=	0x557b;	
	*IPFrame.flagsandoff			=	0x0000;		
	*IPFrame.ttlprot				=	(IPHEAD_TTL << 8) | IPPROT_ICMP;	//ttl and protocol
	*IPFrame.h_checksum				=	0x0000; //header checksum, updated when all written
	*IPFrame.sourceIP				=	(IP1 << 8) | IP2;
	*(IPFrame.sourceIP+1)			=	(IP3 << 8) | IP4;
	*IPFrame.destIP					=	*(ptr + 6);	//checks where it's coming from
	*(IPFrame.destIP+1)				=	*(ptr + 7);
	
	//calculate checksum for header
	*IPFrame.h_checksum				=	calcCRC(IPFrame.hvdiff, IPFrame.hvdiff+5, 10);	//takes all fields under consideration
	
	//message
	*IPFrame.typecode				=	(IP_PINGRPY << 8) | 0x00;
	*IPFrame.d_checksumTCP				=	0x0000; 		//data checksum
	*IPFrame.d_id					=	*(ptr + 12);
	*IPFrame.seq_no					=	*(ptr + 13);		//sequence number
	
	ixData = 0;
	while (ixData < (*IPFrame.totalLength/2) - 10)
		*(IPFrame.dataTCP + ixData) = *(ptr + 14 + ixData++);
	
	
	//calculates checksum for data
	*IPFrame.d_checksumTCP			=	calcCRC(IPFrame.typecode, IPFrame.typecode+1, (*IPFrame.totalLength/2) - 10);
	
	return ethernetBroadcast(txETHFrame, (*IPFrame.totalLength/2)+7);
}


///////////////////////////////////////////////////////////////////////
//
//	checks if destination IP in the IP frame matches
//	station IP
//
//////////////////////////////////////////////////////////////////////
unsigned short checkDestIP(unsigned short *ptr){
	return ((*(ptr + 8) == ((IP1 << 8) | IP2)) && (*(ptr + 9) == ((IP3 << 8) | IP4)));
}


///////////////////////////////////////////////////////////////////////
//
//	checks if destination MAC in the ETH frame matches
//	station MAC or broadcast MAC
//
//////////////////////////////////////////////////////////////////////
unsigned short checkDestMAC(unsigned short *ptr){
	unsigned int binMAC1, binMAC2, binMAC3;
	unsigned short isBroadcast, isDirect;
	
	isBroadcast = (((*ptr == 0xffff) && (*(ptr + 1) == 0xffff) && (*(ptr + 2) == 0xffff)) |
					(((*ptr == 0x0000) && (*(ptr + 1) == 0x0000) && (*(ptr + 2) == 0x0000))));
	isDirect = ((*ptr == ETH_MAC1) && 
				(*(ptr + 1) == ETH_MAC2) && 
				(*(ptr + 2) == ETH_MAC3));
	
	return (isBroadcast || isDirect);
}


///////////////////////////////////////////////////////////////////////
//
//	analyze and process frame
//	checks if dest address matches station address
//	if IP protocol, checks if dest IP match station IP
//	process data, sets up response
//
//////////////////////////////////////////////////////////////////////
void processFrame(unsigned short index, unsigned short *quePtr){
	unsigned short arpType, produceIx;
	unsigned short *ipPtr, *ipData, headerLength, totalLength, dataLength;
	char *dataPtr;
	unsigned short ipType;	//can be UDP or TCP
	ipType = 0;

	/*
	 * following steps to process recieved frame
	 * -----------------------------------------
	 * 0.	check if MAC is braodcast or matches station MAC; if yes
	 * 		proceed to 1; if not, return from function
	 * 
	 * 1.	check type of frame. possibilities are:
	 * 			(a) IP frame
	 * 			(b) ARP request frame
	 * 			(c)	ARP reply frame
	 * 		other frames are not supported
	 * 		
	 * 		a-1	IP frame has header length equal to 20, 
	 * 			although this number don't match when read from packet (WHY?!)
	 * 		a-2	checks validity of IP frame CRC. if correct goes to next step
	 * 			if not returns from function
	 * 		a-3 calculates data length based on total length and header length 
	 * 			this library is based on words (which is not best idea but...)
	 * 			lengths read from packet are in bytes - that's why we divide by 2
	 * 		a-4	checks CRC for data. if correct goes to next step. if not
	 * 			returns from function
	 * 		a-5	checks what type of IP frame it is. possibilities:
	 * 				(*)		ICMP (ping request)
	 * 				(**)	UDP frame
	 * 			if not listed, frame will be ignored
	 * 
	 * 				(*-1)	checks if ICMP req and if IP matches. if yes, send 
	 * 						response, if not ignore frame
	 */
	
	
	//checks if frame is broadcast or destination is this station
	if (!(checkDestMAC(rxETHFrame))) {
		//print("frame dropped");
		ackFrame(quePtr);
		return;
	}

	//is frame IP frame?
	if ((ipPtr = isIPFrame()) != 0){
		headerLength = 20; //investigate WHY can;t take it from frame!?!
		
		//chek what type of IP protocol is used
		ipType = *(ipPtr+4) & 0x00ff;
		
		//checks if IP frame CRC is valid and if IP matches
		if (*(ipPtr+5) == calcCRC(ipPtr, ipPtr+5, (headerLength / 2)) && checkDestIP(ipPtr)) {

			totalLength = *(ipPtr+1);
			dataLength = totalLength - headerLength; //this should be reviewed
			ipData = ipPtr + (headerLength/2); //moves pointer to beginning of IP msg
			
			//if ping request
			if (((*ipData >> 8) == IP_PINGREQ) && (ipType == 0x01)){
				//first check for crc
				if (*(ipData+1) == calcCRC(ipData, ipData+1, (dataLength / 2))){
					if ( sendICMPResponse(getETHSourceMAC(), ipPtr) ) 	//sends ping response
						ackFrame(quePtr);
					return;					//leaves the funtion to save time
					
				} else {  //if crc check failed send out error
					ackFrame(quePtr);
					return;
				}
			}
			
			//for UDP datagrams CRC is not checked
			//UDP daytime response
			if ((*ipData == 49913) && (ipType == 0x11)) {
				//format is following
				int length, index;

				dataPtr = (char*)(ipData+4);

				length = *(ipData+2) - 8;
				char t[length];

				for (index = 0; index < length;){

					t[index+1] = *(dataPtr+index);
					t[index] = *(dataPtr+index+1);
					index += 2;
				}
				t[index] = (char) 0;
				//readNTPTime(t);
				ackFrame(quePtr);
				return;					//leaves the funtion to save time
			}
			
			//UDP daytime response
			if (!(*ipData == UDP_PORT13) && (ipType == 0x11)) {

				ackFrame(quePtr);
				return;					//leaves the funtion to save time
			}
			
			ackFrame(quePtr);	//if none above
			return;

		} else { //if header CRC not ok
			////print ("header crc failed"); //this case to be removed later
			ackFrame(quePtr);
			return;
		}
	}
	
	//is it ARP request and if IP matches
	if (	((arpType = isARPFrame(rxETHFrame)) == ARPOP_REQ)  	&&
			(*(rxETHFrame+19) == ((IP1 << 8) | IP2)) 	&&
			(*(rxETHFrame+20) == ((IP3 << 8) | IP4))		) {

		// if respondToArp success, then ackFrame
		if ( respondToARP(getARPSourceMAC(), getARPSourceIP()) )
			ackFrame(quePtr);
		return;						//leaves the function to save time
	}
		
	if (arpType == ARPOP_RPY); //here should be stored MAC-IP dependency;

	ackFrame(quePtr);			//any other frame; need to ack non-usable frame
}


///////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
//	prepares and sends ARP frame
//
unsigned int sendARPFrame(struct _arpTable* data){
	unsigned short frame[21], *ptr; //42 bytes is ARP frame
	
	//sets pointer at begining fo the frame
	ptr 		=	frame;
	//first 3 words is destination MAC - broadcast	[0-2]
	*(ptr++)		=	0xffff;
	*(ptr++)		=	0xffff;
	*(ptr++)		=	0xffff;
	
	//3 wrods source MAC (this station)	[3-5]
	*(ptr++)		=	ETH_MAC1;
	*(ptr++)		=	ETH_MAC2;
	*(ptr++)		=	ETH_MAC3;
	
	//sets type as ARP	[6]
	*(ptr++)		=	TYPE_ARP;
	
	//hardware type - ethernet	[7]
	*(ptr++)		=	ARP_ETH;
	
	//protocol type IP	[8]
	*(ptr++)		=	TYPE_IP;
	
	//hardware size and protocol size (contant)		[9]
	*(ptr++)		=	ARP_HDPS;
	
	//opcode (is gratuitous)	[10]
	*(ptr++)		=	0x0001;
	
	//sorce MAC again...	[11-13]
	*(ptr++)		=	ETH_MAC1;
	*(ptr++)		=	ETH_MAC2;
	*(ptr++)		=	ETH_MAC3;
	
	//source IP address		[14-15]
	*(ptr++)		=	(IP1 << 8) | IP2;
	*(ptr++)		=	(IP3 << 8) | IP4;
	
	//destination MAC - this time 0s	[16-18]
	*(ptr++)		=	0x0000;
	*(ptr++)		=	0x0000;
	*(ptr++)		=	0x0000;
	
	//destination IP
	*(ptr++)		=	data->IP_OTHER0;
	*ptr			=	data->IP_OTHER1;
	
	//when frame ready and assembled broadcast it
	return ethernetBroadcast(ptr, 21);
}


///////////////////////////////////////////////////////////////////////
//
//	decreses que pointer for frame count
//	used when frame is proccessed or dropped
//	for any reason
//
//////////////////////////////////////////////////////////////////////
void ackFrame(unsigned short *qPtr){
	(*qPtr)--;
	
	if (*qPtr <= 0)
		*qPtr = 0;
}



///////////////////////////////////////////////////////////////////////
//
//	sends out pre-configured UDP frame
//
///////////////////////////////////////////////////////////////////////
unsigned int sendUDPFrame(	struct _IPFrame *frm,
			struct _MACAddress destMac, 
			unsigned int destPort,
			unsigned int srcPort,
			struct _UDPMesg *message)	 {

	// check status, if busy, exit
	if ( enc_getStatus == 1 )
		return 1;

	*ETHFrameHeader.destMAC			=	destMac.MACadd1;
	*(ETHFrameHeader.destMAC+1)		=	destMac.MACadd2;
	*(ETHFrameHeader.destMAC+2)		=	destMac.MACadd3;

	//sorce is already written during initialization
	
	*IPFrame.type					=	TYPE_IP;
	*IPFrame.hvdiff					=	0x4500;		//header length, version & differentiated serv fields
	*IPFrame.totalLength			=	0x14 + 0x0e + message->length + 0x08; //header (20) + IPFrame header (14) + message length + 0x08
	*IPFrame.h_id					=	0x0000;	
	*IPFrame.h_id					=	0x0000;	
	*IPFrame.flagsandoff			=	0x4000;		
	*IPFrame.ttlprot				=	0x4011;	
	*IPFrame.h_checksum				=	0x0000; //header checksum, updated when all written
	*IPFrame.sourceIP				=	(IP1 << 8) | IP2;
	*(IPFrame.sourceIP+1)			=	(IP3 << 8) | IP4;
	*IPFrame.destIP					=	0xc0a8;
	*(IPFrame.destIP+1)				=	0x6464;
	
	//calculate checksum for header
	*IPFrame.h_checksum				=	calcCRC(IPFrame.hvdiff, IPFrame.h_checksum, 10);	//takes all fields under consideration
	
	//message
	*IPFrame.srcPort				=	srcPort;
	*IPFrame.dstPort				=	destPort;
	*IPFrame.length					=	message->length + 8;
	*IPFrame.d_checksumUDP			=	0x0000;
	
	int i;

	for (i=0; i < message->length; i++){
		*(IPFrame.dataUDP+i)	=	(*(message->mesgBody+(i*2)))<<8;

		if (i*2+1 < message->length)
			*(IPFrame.dataUDP+i)	|=	(*(message->mesgBody+i*2+1));
		else
			*(IPFrame.dataUDP+i)	|=	(char) 0;
	}
	*(IPFrame.dataUDP+i)	=	((char) 0)<<8;
	
	return ethernetBroadcast(txETHFrame, *IPFrame.totalLength);
}


///////////////////////////////////////////////////////////////////////
//
//	returns pointer to RX frame[0]
//
///////////////////////////////////////////////////////////////////////
unsigned short * returnRXFramePointer(void){
	return rxETHFrame;
}

//EOF
