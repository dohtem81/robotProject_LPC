/*
 * ethernet.h
 *
 *  Created on: 2010-07-12
 *      Author: Piit
 */

#ifndef ETHERNET_H_
#define ETHERNET_H_

#define OLD_EMAC_MODULE_ID ((0x3902 << 16) | 0x2000)
#define SWAPBYTES	1

/* MAC address definition.  The MAC address must be unique on the network. */
//#define ETH_MAC1	0x0011
//#define ETH_MAC2	0x4353
//#define ETH_MAC3	0xe48b

#define ETH_MAX_FLEN     1508
#define NUM_RX_FRAG         4           /* Num.of RX Fragments 4*1536= 6.0kB */
#define NUM_TX_FRAG         2           /* Num.of TX Fragments 2*1536= 3.0kB */

//server mac and IP address address
#define SERVER_MAC1	0x0016
#define SERVER_MAC2	0x368f
#define SERVER_MAC3	0xa30e
#define SERVER_IP1	192
#define SERVER_IP2	168
#define SERVER_IP3	100
#define SERVER_IP4	100


#define IP1			192
#define IP2			168
#define	IP3			100
#define	IP4			201


//definitions for frames
#define			TYPE_IP		0x0800
#define			TYPE_ARP	0x0806

	
#define			ARP_ETH		0x0001		//hardware type ethernet
#define			ARP_HDPS	0x0604		//hardware & protocol size

#define			ARPOP_REQ	0x0001
#define			ARPOP_RPY	0x0002

#define			IP_PINGREQ		0x08		//ICMP ping request
#define			IP_PINGRPY		0x00		//ICMP ping reply

//for header protocol definitions
#define			IPHEAD_TTL				0x40		//TTL 64

//protocol definitions
#define			IPPROT_ICMP				0x01
#define			IPPROT_TCP				0x06
#define			IPPROT_UDP				0x11



#define			ETH_IX0		0x01
#define			ETH_IX1		0x02
#define			ETH_IX2		0x04
#define			ETH_IX3		0x08

#define			ARP_MAX_STORED	20

//UDP protocol constants used
#define			UDP_PORT13	0x000d


//MAC into IP table
struct _arpTable {
	unsigned short MAC_OTHER0;
	unsigned short MAC_OTHER1;
	unsigned short MAC_OTHER2;
	unsigned short IP_OTHER0;
	unsigned short IP_OTHER1;
};

//structures
struct _ETHFrameHeader {
	//unsigned int frameSize;
	unsigned short *destMAC;
	unsigned short *sourceMAC;
};

struct _arpFrame {
	unsigned short *type;
	unsigned short *arpReqSetting;
	unsigned short *protocolType;
	unsigned short *hdAndProSize;
	unsigned short *gratuitous;
	unsigned short *sourceMAC;
	unsigned short *sourceIP;
	unsigned short *destMAC;
	unsigned short *destIP;
};

struct _IPFrame {
	//header
	unsigned short *type;		//type of frame (doubled with ARP structure)
	unsigned short *hvdiff;		//header length, version & differentiated serv fields
	unsigned short *totalLength;
	unsigned short *h_id;	
	unsigned short *flagsandoff;//flags and offset		
	unsigned short *ttlprot;	//ttl and protocol
	unsigned short *h_checksum; //header checksum
	unsigned short *sourceIP;
	unsigned short *destIP;
	//message - used for ping response
	unsigned short *typecode;
	unsigned short *d_checksumTCP; //data checksum for TCP protocol
	unsigned short *d_id;
	unsigned short *seq_no;		//sequence number
	unsigned short *dataTCP;
	//fields used for UDP datagrams
	unsigned short *srcPort;
	unsigned short *dstPort;
	unsigned short *length;
	unsigned short *d_checksumUDP; //data checksum for UDP protocol
	unsigned short *dataUDP;
	//unsigned short dataUDP[1500];
};


struct _MACAddress {
	unsigned short MACadd1;
	unsigned short MACadd2;
	unsigned short MACadd3;
};

struct _UDPMesg {
	unsigned int length;
	unsigned char mesgBody[1400];
};

// function prototypes

void 				wait(int);
unsigned int		sendARP(void);
unsigned short		calcCRC(unsigned short*, unsigned short*, unsigned int);
void				swapBytes(unsigned short*, int); 	//pointer to beginning of table to swap, 
													//number of words to swap
unsigned short  	swapByte(unsigned short*);
unsigned int		respondToARP(unsigned short *, unsigned short *);
unsigned short* 	getARPSourceMAC(void);
unsigned short* 	getARPSourceIP(void);
unsigned short 		isARPFrame(unsigned short *);
unsigned short* 	isIPFrame(void);
unsigned int		sendICMPResponse(unsigned short *, unsigned short *);
unsigned short* 	getETHSourceMAC(void);
unsigned short* 	getETHDestMAC(void);
unsigned short* 	getIPSourceIP(void);
unsigned short 		checkDestIP(unsigned short *);
unsigned short 		checkDestMAC(unsigned short *);
void 				processFrame(unsigned short, unsigned short*);
unsigned int		sendARPFrame(struct _arpTable*);
void				ackFrame(unsigned short *);
unsigned int		sendUDPFrame(struct _IPFrame*, struct _MACAddress, unsigned int, unsigned int, struct _UDPMesg*);
void				initializeFrame();

unsigned short*		returnRXFramePointer(void);

#endif /* ETHERNET_H_ */
