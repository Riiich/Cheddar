#include "platform.h"

void InitUsb2()
{
	XBYTE[0xFC00] |= 0x10 ;		 // USB Soft Reset
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();

	//end-point 0 setting
	XBYTE[0xFC32] = 0x03;		// EP0 control endpoint,  packet size 64 bytes 
	XBYTE[0xFC34] = 0x20;		// Control register setting, interrupt happens when packet of data is received.

	//end-point 6 setting
	XBYTE[0xFC61] = 0x10;		// enable maximum packet size
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


//-----------Set Descriptors--------------
// initial the pointer to the USB descriptor 
	 //Con_Std_Descp = Ide_Con_Std_D;
	 //InterfaceEP_Descp =  UVC_Std_D_H;
	 //InterfaceEP_Oth_Descp = UVC_Std_D_F;
//----------------------------------------


	//Sync FIFO
	XBYTE[0xFD8B] |=0x06;		// Choose the FIFO of endpoint 6 to be the input data buffer of CIS

	XBYTE[0xFF10] |= 0x02; 		// enable resume pin
	
	XBYTE[0xFC04] = 0x41;		// enable ALL endpoint buffer
	XBYTE[0xFC06] = 0x0F;		// enable BusSuspend, BusResume, BusReset and VbusInt interrupts
	XBYTE[0xFC08] = 0x01;		// enable ALL endpoint interrupts

	//---Initial global variables---
	VbusInt = 0;
	BusReset = 0;
	BusSuspend = 0;
	BusResume = 0;
	PktRcv = 0;
	BulkOutRcv = 0;
	Abort = 0;
	bConfiguration = 0;
	bDeviceAddress = 0;
}

void ISO_TEST_1()
{
	// Request frame/line count is 0x1F = 31
 	XBYTE[0xFCC4] = 0x1F;	//0xFCC4,Request_Frame/Line_Count_L
 	XBYTE[0xFCC5] = 0x00;	//0xFCC5,Request_Frame/Line_Count_H
	
	XBYTE[0xFCC0] = 0x73;

	// Line count per frame is 0x1E0 = 480
	XBYTE[0xFCC1] = 0xE0;	//CIS_Config_L. [7:0] Line count(7:0) per frame.
 	XBYTE[0xFCC2] = 0x09;	//CIS_Config_H. [2:0] Line count(10:8) per frame. [3] if set, the "Frame Number" will show in PTS field of stream header.
 	
 	XBYTE[0xFCC3] = 0x20;	//CIS_Ctrl. 0x20:Data in enable, it will be in Idel state automatically when transfer finished. (Non-stop)
}

void UpdateUSB()
{
	InitUsb2();
	
START:
	
	while( (XBYTE[0xFC01] & 0x80) != 0x80 );	// check if any external USB VBUS pin input
	XBYTE[0xFC00] =  0x01 | 0x08;				// Attach device to USB bus and enable the MASTER control of USB interrupt 
	
	while (0xFF) {
		
		if(_testbit_(BusReset)){
			InitUsb2();
		}
		if (BusSuspend) {
			BusSuspend=0;
		}
		if(_testbit_(BusResume)) {
		}
		if(_testbit_(PktRcv)){
			controlCMD();
		}

		if(DS_USB_VBUS_DETACH){
			goto START;
		}
		
		if(GLOBAL_test) {
			GLOBAL_test=0;
			//GPIO Pin. To control the POWER_DOWN pin of CIS sensor. 1: power down, 0: normal operation
			//SENSOR_POWER_PIN = SENSOR_PIN_ON;
			ISO_TEST_1();
		}
	}

}


unsigned char ctrlFIFORead(unsigned int len, unsigned char *buf )
{
    unsigned int  i;

    XBYTE[EP0_CTRL] = 0x10;
    XBYTE[ENP_IRE_STORE] = 0x03;

    i = XBYTE[EP0_CTRL+3] & 0x0F;			//ep_ctrl+3 = ep_len_H
    i = (i << 8) | XBYTE[EP0_CTRL+2];	//ep_ctrl+2 = ep_len_L
    if( i == len )
    {
        for( i=0; i<len; i++ )
        {
            buf[i] = XBYTE[CONTROL_FIFO+i];
        }
        XBYTE[EP0_CTRL] |= 0x04; //flush
        XBYTE[EP0_CTRL] = 0x18;
        return USB_DONE;
    }
    else
    {
        XBYTE[EP0_CTRL] |= 0x04; //flush
        XBYTE[EP0_CTRL] = 0x18;
        return( FALSE );
    }
}

unsigned char ctrlFIFOWrite(unsigned int len, unsigned char *buf )
{
    unsigned int i,j;
    unsigned char k;
	
    if(len > 64) {
        j=0;
        while(j<len) {
            if( (j+64) >len){ 	
                k = len - j;
                for(i=0; i<k; i++) {
                    XBYTE[CONTROL_FIFO+i]=buf[j+i];
                }
                j+=k;
            }
            else {
                for(i=0; i<64; i++) {
                    XBYTE[CONTROL_FIFO+i]=buf[j+i];
                }
								j+=64;
            }       
						DelayMS(4);	
		
          	if(j!=len){
							while(!(XBYTE[EP0_STATUS_H] & 0x20)){
								if(DS_USB_VBUS_DETACH )
									return(0);
								if(BusReset)
									return(0);
							}
							/*while(!(XBYTE[EP0_STATUS_H] & 0x20)){
								CHeck_Crash;
							}*/
						}
        }

        XBYTE[EP0_CTRL] |= 0x21;
    }
    else {
			for(i=0; i<len; i++) {
        XBYTE[CONTROL_FIFO+i]=buf[i];
			}
			XBYTE[EP0_CTRL] |= 0x21;
    }
		
		return USB_DONE;
}

/*
// Endpoint 0 FIFO Read 
unsigned char ctrlFIFORead(unsigned char *buf )
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
*/
