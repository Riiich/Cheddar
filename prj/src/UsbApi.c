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
#ifdef OV9155
	// Request frame/line count is 0xFFFF = 65535
 	XBYTE[0xFCC4] = 0xFF;	//0xFCC4,Request_Frame/Line_Count_L
 	XBYTE[0xFCC5] = 0xFF;	//0xFCC5,Request_Frame/Line_Count_H
 	
	XBYTE[0xFCC0] = 0x72;	//CIS_Config_byte0. [7:5] Length of stream header: 12 Byte (20140128 BruceC for OV9155 SXGA)

	// Line count per frame is 0x400 = 1024
	XBYTE[0xFCC1] = 0x00;	//CIS_Config_byte1. [7:0] Line count(7:0) per frame.
 	XBYTE[0xFCC2] = 0x0C;	//CIS_Config_byte2. [2:0] Line count(10:8) per frame. [3] if set, the "Frame Number" will show in PTS field of stream header.
#else
	// Request frame/line count is 0x1F = 31
	XBYTE[0xFCC4] = 0x1F;	//0xFCC4,Request_Frame/Line_Count_L
	XBYTE[0xFCC5] = 0x00;	//0xFCC5,Request_Frame/Line_Count_H

	XBYTE[0xFCC0] = 0x73;	//CIS_Config_byte0. [7:5] Length of stream header.

	// Line count per frame is 0x1E0 = 480
	XBYTE[0xFCC1] = 0xE0;	//CIS_Config_byte1. [7:0] Line count(7:0) per frame.
 	XBYTE[0xFCC2] = 0x09;	//CIS_Config_byte2. [2:0] Line count(10:8) per frame. [3] if set, the "Frame Number" will show in PTS field of stream header.
#endif
 
 	XBYTE[0xFCC3] = 0x20;	//CIS_Ctrl. 0x20:Data in enable, it will be in Idel state automatically when transfer finished. (Non-stop)
}

void UpdateUSB()
{
START:
	
	InitUsb2();
	XBYTE[0xFF05] = 0x00;						// disable CIS and FIFO soft reset
	
	while( (XBYTE[0xFC01] & 0x80) != 0x80 );	// check if any external USB VBUS pin input
	DelayMS( 20 );
	while( (XBYTE[0xFC01] & 0x80) != 0x80 );	// check if any external USB VBUS pin input
	XBYTE[0xFC00] =  0x01 | 0x08;				// Attach device to USB bus and enable the MASTER control of USB interrupt
	XBYTE[0xFC00] |= 0x04;						// Remote wakeup enable
	XBYTE[0xFC06] = 0x0F;						// enable BusSuspend, BusResume, BusReset and VbusInt interrupts
	
	while (0xFF) {
		
		if(_testbit_(BusReset)){
			XBYTE[0xFF05] = 0x06;				// CIS and FIFO soft reset
			DelayMS(1);
			XBYTE[0xFF05] = 0x00;				// disable CIS and FIFO soft reset
			InitUsb2();
		}
		if (BusSuspend) {
			BusSuspend=0;
#ifdef OV9155
			EIE = 0x00;							//Close P2.2 Touch interrupt
			EIP = 0x00;							//Close P2.2 Touch interrupt

			IE |= 0x04;							//Enable P1.5 Touch Interrupt
			IP |= 0x04;

			P3_7=0; 							//BruceC,CIS_D5_EN = 0
			P3_6=1; 							//BruceC,SWT0_EN =1

			P1_5 =1;
			XBYTE[0xFF0B]  = 0x00;  			//P2_2 interupt close
			XBYTE[0xFF0A]  = 0xD0;  			//P1_5 interupt
			P2_2=0;
			
			SENSOR_POWER_PIN = SENSOR_PIN_OFF;	//Power OFF sensor

			EventHdl_BusSuspend();
#endif

		}
		if(_testbit_(BusResume)) {
#ifdef OV9155
			IE &= 0xFB;							//Close P1.5 Touch Interrupt
			IP &= 0xFB;
			EIE = 0x01;							//Enable P2.2 Touch Interrupt
			EIP = 0x00;							//Close P2.2 Touch interrupt

			P3_7=1;								//BruceC,CIS_D5_EN = 0
			P3_6=0;								//BruceC,SWT0_EN =1

			P2_2=1;
			XBYTE[0xFF0A]  = 0x00;				//P1_5 interupt close
			XBYTE[0xFF0B]  = 0x0A;				//P2_2 interupt 
			P1_5 =0;

			InitCIS();
#endif
		}
		if(_testbit_(PktRcv)){
			controlCMD();
		}

		if(DS_USB_VBUS_DETACH){
			goto START;
		}
		
		if(StartCIS) {
			StartCIS=0;
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

// Subroutine for handling the USB BUS Suspend condition
void EventHdl_BusSuspend(void) {

	///////////////////////////////////////////////////////////////////////////////
	// check if USB cable is unplugged
	///////////////////////////////////////////////////////////////////////////////
	SetTimer ( 20 );      // set 20ms and start counting
	///////////////////////////////////////////////////////////////////////////////
	while ( !IsTimerExpired() ){
		if ( BusReset || BusResume ){
			ResetTimer();
			BusResume = 0;
			return;
		} // if
	} // while
	///////////////////////////////////////////////////////////////////////////////

	ResetTimer();

	///////////////////////////////////////////////////////////////////////////////
	if ( ( XBYTE[DEV_STATUS] & 0x80 ) == 0 ){
		return; 
	} // USB unplugged

	///////////////////////////////////////////////////////////////////////////////
	// start suspending
	///////////////////////////////////////////////////////////////////////////////

	XBYTE[0xFC06] = 0x07;			// enable BusResume, BusReset and VbusInt interrupt

	// disable Suspend interrupt 
	if(RemoteWakeup){
		XBYTE[0xFC00] = 0x0F;		//Richard(Prolific) say force set
	}
	else{
		XBYTE[0xFC00] = 0x0B;
	}

	// CPU clock will be stopped some clock cycles after suspend enable bit in register 0xFC00 is set
	// 
	///////////////////////////////////////////////////////////////////////////////
	// 48 NOP
	///////////////////////////////////////////////////////////////////////////////
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	_nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	///////////////////////////////////////////////////////////////////////////////
	
	do { XBYTE[0xFC06] = 0x0F;}
	while ( XBYTE[0xFC06] != 0x0F );
	
}
