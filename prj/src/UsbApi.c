#include "platform.h"

void InitUsb2()
{
	XBYTE[0xFC00] |= 0x10 ;		 // USB Soft Reset
	_nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();

	//end-point 0 setting
    XBYTE[0xFC32] = 0x03;		// EP0 control endpoint,  packet size 64 bytes 
    XBYTE[0xFC34] = 0x20;		// Control register setting, interrupt happens when packet of data is received.  

	//end-point 6 setting
    XBYTE[0xFC61] |= 0x10;		// enable maximum packet size
    XBYTE[0xFC62] = 0xA6;		// ISO In, packet size = 1024 bytes
    XBYTE[0xFC64] = 0x10;

	//------------FIFO SIZE SETTING--------------------
	XBYTE[EP1_FIFO_SIZE_REG]= 0x00; 
	XBYTE[EP2_FIFO_SIZE_REG]= 0x00;
	XBYTE[EP3_FIFO_SIZE_REG]= 0x00;
	XBYTE[EP4_FIFO_SIZE_REG]= 0x00;
	XBYTE[EP5_FIFO_SIZE_REG]= 0x00;
	XBYTE[EP6_FIFO_SIZE_REG]= 0x80;
	XBYTE[EP7_FIFO_SIZE_REG]= 0x00;
	XBYTE[EP8_FIFO_SIZE_REG]= 0x00;
	XBYTE[EP9_FIFO_SIZE_REG]= 0x00;
	XBYTE[EP10_FIFO_SIZE_REG]= 0x00;
	XBYTE[EP11_FIFO_SIZE_REG]= 0x00;
	XBYTE[EP12_FIFO_SIZE_REG]= 0x00;
	XBYTE[EP13_FIFO_SIZE_REG]= 0x00;
	XBYTE[EP14_FIFO_SIZE_REG]= 0x00;
	XBYTE[EP15_FIFO_SIZE_REG]= 0x00;


	//Sync FIFO
	XBYTE[0xFD8B] |=0x06;		// Choose the FIFO of endpoint 6 to be the input data buffer of CIS

	//---Initial global variables---
	VbusInt = 0;
	BusReset = 0;
	BusSuspend = 0;
	BusResume = 0;
	PktRcv = 0;
	BulkOutRcv = 0;
	Abort = 0;

	XBYTE[0xFF10] |= 0x02; 		// enable resume pin
	XBYTE[0xFC04] = 0xFF;		// enable ALL endpoint buffer
	XBYTE[0xFC08] = 0xFF;		// enable ALL endpoint interrupts
	XBYTE[0xFC06] = 0x0F;		// enable BusSuspend, BusResume, BusReset and VbusInt interrupts
	
}

void UpdateUSB()
{
	InitUsb2();
START:
	
	while( (XBYTE[0xFC01] & 0x80) != 0x80 );	// check if any external USB VBUS pin input
	XBYTE[0xFC00] =  0x01 | 0x08;				// Attach device to USB bus and enable the MASTER control of USB interrupt 
	
	while (0xFF) {
		
		if(_testbit_(BusReset)){
			//InitUsb2();
			P2_3=0;
		}
		if (BusSuspend) {
			BusSuspend=0;
			P2_4=0;
		}
		if(_testbit_(BusResume)) {
			P2_5=0;
		}
		if(_testbit_(PktRcv)){
//			controlCMD();
		}
/*
		if(DS_USB_VBUS_DETACH){
			goto START;
		}
/*
		if(GLOBAL_test) {
			GLOBAL_test = 0;
			ISO_TEST_1();
		}
		*/
	}

}


// Endpoint 0 FIFO Read 
unsigned char ctrlFIFORead(/*unsigned int len, */unsigned char *buf )
{
	unsigned int  j;

    // read out the 8-byte control data from endpoint 0 FIFO to user defined buffer
    for( j=0; j<8; j++ ){
      buf[j] = XBYTE[0xF000+j];

    }
   
    // If there are not any data following this control transfer, restore EP0 interrupt here.
	if( (!((buf[0] == 0x21) && (buf[1] == 0x20) && (buf[6] == 0x07))) && (!((buf[0] == 0x21) &&  (buf[1] == 0x01))) 
				&& (!( (buf[0] == 0x21) && (buf[1] == 0xFF) && (buf[2] == 0x00) && (buf[6] != 0) ) ) )
	{
		XBYTE[0xFCB2] |= 0x01;	 //restore EP0
	}       

	XBYTE[0xFC34] = 0x2C; //bit3:EP read   bit2:flush

    // During the FIFO flush operation, if bus reset or detach occur, return immediately.
	while( XBYTE[0xFC34] & 0x04 ){
		if( BusReset || (XBYTE[0xFC01] & 0x80 == 0)){
			return 0;
		}
	}

    return( TRUE );
}


// Endpoint 0 FIFO Write
unsigned char ctrlFIFOWrite(  unsigned int len, unsigned char *buf )
{
	unsigned int i,j = 0;
	
	while( XBYTE[0xFC34] & 0x04 ){
		if( BusReset || (XBYTE[0xFC01] & 0x80 == 0)){
			return 0;
		}
	}	
	
	while ( len > 64 ){
		for ( i = 0; i < 64; i++ ){ 
			XBYTE[0xF000 + i] = buf[j++]; 
		}
			
		len -= 64;

		// check whether the FIFO is empty or not
		while( (XBYTE[0xFC37] & 0x20) == 0 ){
			if( BusReset || (XBYTE[0xFC01] & 0x80 == 0)){
				return 0;
			}			
		}
	}
	
	for ( i = 0; i < len; i++ ) { 
		XBYTE[0xF000 + i] = buf[j++]; 
	}

	XBYTE[0xFC34] = 0x21;

  return( TRUE );
}

