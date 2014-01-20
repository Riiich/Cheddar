#include "platform.h"


void usb_video_class_function();
unsigned char usbGetStatus();
unsigned char usbClearFeature();
unsigned char usbSetFeature();
unsigned char usbSetAddress();
unsigned char usbGetDescriptor();
unsigned char usbGetConfig();
unsigned char usbSetConfig();
unsigned char usbGetInterface();
unsigned char usbSetInterface();


void controlCMD()
{
	unsigned char x;
	x = ctrlFIFORead(CTRL_Buffer);
	
	if( x == TRUE ){
		if((CTRL_Buffer[0]==0x21) || (CTRL_Buffer[0]==0xA1)){
			usb_video_class_function();
		} else {
			
			switch(CTRL_Buffer[1]){
				case BR_GET_STATUS:
					x = usbGetStatus();
					break;
				case BR_CLEAR_FEATURE:
					x = usbClearFeature();
					//XBYTE[0xFC34] = 0x21;
					break;
				case BR_SET_FEATURE:
					x = usbSetFeature();
					break;
				case BR_SET_ADDRESS:
					x = usbSetAddress();
					break;
				case BR_GET_DESCRIPTOR:
					x = usbGetDescriptor();
					break;
				case BR_GET_CONFIGURATION:
					x = usbGetConfig();
					break;
				case BR_SET_CONFIGURATION:
					x = usbSetConfig();
					break;
				case BR_GET_INTERFACE:
					x = usbGetInterface();
					break;
				case BR_SET_INTERFACE:
					x = usbSetInterface();
					break;
				default:
					x = FALSE;
					break;
			}
		}
	}
}



unsigned char usbGetStatus()
{
	unsigned char x;

	// Check the Request integrity:
	// wLength must be 2 (LSB CTRL_Buffer[6], MSB CTRL_Buffer[7])
	// wValue must be 0 (LSB CTRL_Buffer[2], MSB CTRL_Buffer[3])
	if( (CTRL_Buffer[6] != 2) || (CTRL_Buffer[7] != 0) || (CTRL_Buffer[2] != 0) || (CTRL_Buffer[3] != 0) ) {
		return USB_STALL;
	}

	switch( CTRL_Buffer[0] ) {
    case 0x80:								// Device Status
			// Check wIndex (LSB CTRL_Buffer[4], MSB CTRL_Buffer[5])
        if( (CTRL_Buffer[4] == 0) && (CTRL_Buffer[5] == 0) ) {	// if wIndex == 0
						// BUG: Not assigned x value will be returned
						// Should be:  x = ((XBYTE[DEV_CTRL] >> 1) & 0x02) | DEV_CFG_SELF_PWRED; (or x = ((XBYTE[DEV_CTRL] >> 1) & 0x02) | DEV_CFG_BUS_PWRED;)
						// definitions:
						// ((XBYTE[DEV_CTRL] >> 1) & 0x02)   - return the device Remote Wake up Enabled flag in bit 1 
						// #define DEV_CFG_BUS_PWRED	(0x00)
						// #define DEV_CFG_SELF_PWRED	(0x01)
        } else {																								// if wIndex != 0
            return USB_STALL;
        }
        break;
    case 0x81:								// Interface status
        x = 0x00;	// Always return 0
        break;
    case 0x82:								// Endpoint status
				// Check wIndex (LSB CTRL_Buffer[4], MSB CTRL_Buffer[5])
        if( CTRL_Buffer[5] != 0 ) {	 	// if wIndex MSB != 0
            return USB_STALL;
        }
        switch( CTRL_Buffer[4] ) {		// LSB of wIndex specify EP
        case 0x00:	// Control EP
            x = XBYTE[EP0_CTRL];
            break;
        case 0x01:	// EP1 (OUT endpoint)
            x = XBYTE[EP1_CTRL];
            break;
        case 0x82:	// EP2 (IN Endpoint)
            x = XBYTE[EP2_CTRL];
            break;
        default:
            return USB_STALL;
        }

        if( x & 0x02 ) { 	// Check Endpoint Stall flag (bit 1)
            x = 0x01; 		// Set HALT Flag in the returned value if EP is Stalled
        } else {
            x = 0x00; 		// Do Not Set HALT Flag in the returned value if EP is Not Stalled
        }
        break;

    default:
        return USB_STALL;
    }

    XBYTE[CONTROL_FIFO] = x;		// Put the result to EP0 FIFO (LSB of the returned STATUS)
    XBYTE[CONTROL_FIFO+1] = 0;	// Put 0 byte to EP0 FIFO (MSB of the returned STATUS)
    XBYTE[EP0_CTRL] |= 0x01;		// Set Endpoint Done flag to transfer the data
		
	return USB_DONE;
}

unsigned char usbClearFeature()
{
	if( (CTRL_Buffer[3] != 0) || (CTRL_Buffer[6] != 0) || (CTRL_Buffer[7] != 0) ) {
        return USB_STALL;
	}
	
	switch( CTRL_Buffer[0] ) {
    case BMR_REC_DEVICE:																//	Device status
        if( (CTRL_Buffer[4] != 0) || (CTRL_Buffer[5] != 0) ) {
            return USB_STALL;
        }

        if( CTRL_Buffer[2] == FTR_DEVICE_REMOTE_WAKEUP ) {
            //	RemoteWakeup = 0;
        } else {
            return USB_STALL;
        }
        break;

    case BMR_REC_INTERFACE:															//	Interface status
        return USB_STALL;

    case BMR_REC_ENDPOINT:															//	Endpoint status
        if( CTRL_Buffer[2] == FTR_ENDPOINT_HALT ) {			//	Feature selector is endpoint_halt
            if( CTRL_Buffer[5] != 0 ) {
                return USB_STALL;
            }

            switch( CTRL_Buffer[4] ) {
							case 0x00:
									XBYTE[EP0_CTRL] = 0x18;
									break;
							case 0x01:
									XBYTE[EP1_CTRL] = 0x28;
									XBYTE[ENP_TOG_CLR] = 0x02;
									//	XBYTE[USB3_FIFO_FLUSH_CTRL] = 0x03;
									break;
							case 0x82:
									XBYTE[EP2_CTRL] = 0x10;
									XBYTE[ENP_TOG_CLR] = 0x04;
									break;
							default:
									return USB_STALL;
            }
        } else {
            return USB_STALL;
        }
        break;
    default:
        return USB_STALL;
    }
    return USB_DONE;
}

unsigned char usbSetFeature()
{
	if( (CTRL_Buffer[3] != 0) || (CTRL_Buffer[6] != 0) || (CTRL_Buffer[7] != 0) ) {
		return USB_STALL;
	}
	
	switch( CTRL_Buffer[0] ) {
		case BMR_REC_DEVICE:																//	Device status
        if( CTRL_Buffer[2] == FTR_DEVICE_REMOTE_WAKEUP ) {
            //  RemoteWakeup = 1;
        } else if( CTRL_Buffer[2] == FTR_DEVICE_TEST_MODE ) {
            if( (CTRL_Buffer[4] != 0) || (CTRL_Buffer[5] > 4) || (CTRL_Buffer[5] == 0) ) {
                return USB_STALL;
            }
						
            TEST_MODE = CTRL_Buffer[5] | 0x08;
        } else {
            return USB_STALL;
        }
        break;

		case BMR_REC_INTERFACE:															//	Interface status
        return USB_STALL;
				break;
		case BMR_REC_ENDPOINT:									//	Endpoint status
			if( CTRL_Buffer[2] == FTR_ENDPOINT_HALT ) {
            if( CTRL_Buffer[5] != 0 ) {
                return USB_STALL;
            }

            switch( CTRL_Buffer[4] ) {
							case 0x00:
									XBYTE[EP0_CTRL] = 0x1A;
									break;
							case 0x01:
									XBYTE[EP1_CTRL] = 0x2A;
									break;
							case 0x82:
									XBYTE[EP2_CTRL] = 0x12;
									break;
							default:
									return USB_STALL;
            }
        } else {
            return USB_STALL;
        }
        break;
    default:
        return USB_STALL;
    }

    return USB_DONE;
}

unsigned char usbSetAddress()
{
	if( (CTRL_Buffer[2] & 0x80) || (CTRL_Buffer[3] != 0) ||
            (CTRL_Buffer[4] != 0) || (CTRL_Buffer[5] != 0) ||
            (CTRL_Buffer[6] != 0) || (CTRL_Buffer[7] != 0) ) {
        return USB_STALL;
	}
	
	DEVICE0_ADDRESS = CTRL_Buffer[2];
	//Addressed = 1;	// Timer address_completion_wait
	
	XBYTE[EP0_CTRL] = 0x21;
	
	return USB_DONE;
}

unsigned char usbGetDescriptor()
{
	unsigned char code *descp;
	unsigned short rlen;
	unsigned short cmd_len = 0;
	unsigned int  offset = 0;
	
	cmd_len = (CTRL_Buffer[7]*256 ) +  CTRL_Buffer[6];

	switch( CTRL_Buffer[3] ) {

    case DS_DEVICE:											// Device Descriptor
        rlen = Ide_Dvc_Dlen;
        descp = Ide_Dvc_D;
        if( (CTRL_Buffer[6] < rlen) ) {
            rlen = CTRL_Buffer[6];
        }
        ctrlFIFOWrite(rlen,descp);
    case DS_CONFIGURATION:							// Configuration Descriptor
        descp = UVC_Std_D;
        rlen = UVC_Std_D_len;
        if( (cmd_len < rlen) ) {
            rlen = cmd_len;
        }
        ctrlFIFOWrite(rlen,descp);
        break;
    case DS_STRING:											// String Descriptor
        switch( CTRL_Buffer[2] ) {
					case 0x00:		// LangID string
							rlen = Ide_Str0_Dlen;
							descp = Ide_Str0_D;
							if( (CTRL_Buffer[6] < rlen) ) {
									rlen = CTRL_Buffer[6];
							}
							ctrlFIFOWrite(rlen,descp);
							break;
					case 0x01:
							rlen = Ide_Str1_Dlen;
							descp = Ide_Str1_D;
							if( (CTRL_Buffer[6] < rlen) ) {
									rlen = CTRL_Buffer[6];
							}
							ctrlFIFOWrite(rlen,descp);
							break;
					case 0x02:
							rlen = Ide_Str2_Dlen;
							descp = Ide_Str2_D;
							if( (CTRL_Buffer[6] < rlen) ) {
									rlen = CTRL_Buffer[6];
							}
							ctrlFIFOWrite(rlen,descp);
							// XBYTE[ENP_IRE_STORE] |= 0x01;
							break;
					case 0x03:
							rlen = Ide_Str3_Dlen;
							descp = Ide_Str3_D;
							if( (CTRL_Buffer[6] < rlen) ) {
									rlen = CTRL_Buffer[6];
							}
							ctrlFIFOWrite(rlen,descp);
							//XBYTE[ENP_IRE_STORE] |= 0x01;
							break;
					default:
							return USB_STALL;
        }
        break;
    case DS_DEVICE_QUALIFIER:						// Device_Qualifier Descriptor
        rlen = Ide_Dvc_QF_Dlen;
        descp = Ide_Dvc_QF_D;
        if( (CTRL_Buffer[6] < rlen) ) {
            rlen = CTRL_Buffer[6];
        }
        ctrlFIFOWrite(rlen,descp);
        break;
    case DS_INTERFACE_POWER:						// Interface Power Configuration
        return USB_STALL;
    default:
        return USB_STALL;
    }
		
    return USB_DONE;
}

unsigned char usbGetConfig()
{
    if( (CTRL_Buffer[2] != 0) || (CTRL_Buffer[3] != 0) ||
            (CTRL_Buffer[4] != 0) || (CTRL_Buffer[5] != 0) ||
            (CTRL_Buffer[6] != 1) || (CTRL_Buffer[7] != 0) ) {
        return USB_STALL;
    }

    if( Configed ) {
        XBYTE[CONTROL_FIFO] = 0x01;
    } else {
        XBYTE[CONTROL_FIFO] = 0x00;
    }

    XBYTE[EP0_CTRL] |= 0x01;

    return USB_DONE;
}

unsigned char usbSetConfig()
{
	XBYTE[EP1_CTRL] = 0x28;
	XBYTE[EP2_CTRL] = 0x10;
	XBYTE[EP0_CTRL] = 0x21;

	return USB_DONE;
}

unsigned char usbGetInterface()
{
    if( (CTRL_Buffer[2] != 0) || (CTRL_Buffer[3] != 0) || (CTRL_Buffer[6] != 1) || (CTRL_Buffer[7] != 0) ) {
        return USB_STALL;
    }

    if( Configed ) {
        XBYTE[CONTROL_FIFO] = 0x00;
        XBYTE[EP0_CTRL] |= 0x01;

        return USB_DONE;
    } else {
        return USB_STALL;
    }
}

unsigned char usbSetInterface()
{
/*	
	if((CTRL_Buffer[0] == 0x01) &&
		(CTRL_Buffer[1] == 0x0B) &&
		(CTRL_Buffer[2] == 0x07) &&  
		(CTRL_Buffer[3] == 0x00) &&
		(CTRL_Buffer[4] == 0x01) && 
		(CTRL_Buffer[5] == 0x00) &&
		(CTRL_Buffer[6] == 0x00) &&
		(CTRL_Buffer[7] == 0x00) 
	){
			GLOBAL_test=1; 	
			//printStr("START_OUT\n");
	}
*/
	XBYTE[EP0_CTRL] |= 0x21;
	
	return USB_DONE;
}

void usb_video_class_function()
{	
	// wLength represents the size of the parameters which will be passed in the data stage
	// of the control transfer
	// Potential BUG: just LSB of the wLength is used here. If the parameters length will be
	// bigger than 255 bytes we will not able to process the request correctly
	unsigned char UAC_len, i;	//wLength
	UAC_len = CTRL_Buffer[6];
	
	// Host to Device
	if((CTRL_Buffer[0] == 0x21)) {	// If SET Request addressed to Entity ID or Interface received.
		
		// Read the Request Parameters from the EP0 FIFO
		// Potential BUG: What if the parameters data are not ready yet? I think some wait timout need to be implemented here
		if (XBYTE[EP0_CTRL+2]==UAC_len) { // If the amount of data in FIFO == wLength
			// Read out the Request Parameters to the SHARE_Buffer
			for (i=0; i<UAC_len; i++) {
				SHARE_Buffer[i] = XBYTE[CONTROL_FIFO+i];
			}
		}
		
		// If we have
		// bRequest	(Control Attribute ID)	= SET_CUR (Current Setting Attribute = 0x01 ) // CTRL_Buffer[1]
		// wValue (Control Selector )				= 0x0200 (Control Selector = 0x02 ) // LSB CTRL_Buffer[2], MSB CTRL_Buffer[3] // WHAT IS CONTROL 0x02 ????
		// wIndex (Entity ID, Interface )		= 0x0001 (Interface = 0x01, Entity ID = 0x00 for interface ) // LSB CTRL_Buffer[4] - Interface, MSB CTRL_Buffer[5] - Entity ID
		// wLength ( lengths of the parameter block )	= 0x00A1 // LSB CTRL_Buffer[6], MSB CTRL_Buffer[7]
		if ((CTRL_Buffer[1] == 0x01) &&
				(CTRL_Buffer[2] == 0x00) && 
				(CTRL_Buffer[3] == 0x02) && 
				(CTRL_Buffer[4] == 0x01) &&
				(CTRL_Buffer[5] == 0x00) &&
				(CTRL_Buffer[6] == 0x1A) &&
				(CTRL_Buffer[7] == 0x00)){
					//GLOBAL_test = 1;
		}
		
		//???????????????????
		for (i=0; i<0xFE; i++) 
		{             		
		}//Delay for confirming the last data is already written to the register
				
		XBYTE[EP0_CTRL] |= 0x04;	//flush control pipe
		XBYTE[EP0_CTRL] = 0x21;		// Configure Endpoint 0: (bit5) - unmask "Packet of Data being received" interrupt, (bit0) - set Endpoint Done flag
	}
	// Device to Host
	else if((CTRL_Buffer[0] == 0xA1)) {	// If GET Request addressed to Entity ID or Interface received
		
		if(UAC_len ==0) {
			// if the parameters data length is 0 (so no data stage expected in the request transfer )
			// Finalyze request transfer
			XBYTE[EP0_CTRL] = 0x21;
			// XBYTE[EP0_CTRL] = 0x21; // Do twice???
		}
		
		
		// GET_INFO (0x86) Request Processing
		// The GET_INFO request queries the capabilities and status of the specified control. 
		// When issuing this request, the wLength field shall always be set to a value of 1 byte. 
		// The result returned is a bit mask reporting the capabilities of the control. The bits are defined as:
		// ****************************************************************************************
		//	Bit field	*	Description												*	Bit State
		// ****************************************************************************************
		//				*															*
		//	D0			*	1=Supports GET value requests							*	Capability
		// ****************************************************************************************	
		//	D1			*	1=Supports SET value requests							*	Capability
		// ****************************************************************************************	
		//	D2			*	1=Disabled due to automatic mode 						*	State
		//				*	(under device control)									*
		// ****************************************************************************************	
		//	D3			*	1= Autoupdate Control 									*	Capability
		//				*	(see section 2.4.2.2 "Status Interrupt Endpoint")		*
		// ****************************************************************************************
		//	D4			*	1= Asynchronous Control  								*	Capability
		//				*	(see sections 2.4.2.2 "Status Interrupt Endpoint"		*
		//				*	and 2.4.4, ?ontrol Transfer and Request Processing?	*
		// ****************************************************************************************	
		//	D7..D5		*	Reserved (Set to 0)										*	--
		// ****************************************************************************************
		if(CTRL_Buffer[1] == USB_UVC_GET_INFO) {
			
			if((CTRL_Buffer[3] == 0x03) || (CTRL_Buffer[3] == 0x06) || (CTRL_Buffer[3] == 0x08)||
		      ((CTRL_Buffer[3] == 0x09) && (CTRL_Buffer[5] == 0x02))|| (CTRL_Buffer[3] == 0x01) || (CTRL_Buffer[3] == 0x05) ||
			    (CTRL_Buffer[3] == 0x10) || (CTRL_Buffer[3] == 0x18) || ((CTRL_Buffer[3] == 0x07) && (CTRL_Buffer[5] == 0x02))||
			    ((CTRL_Buffer[3] == 0x04) && (CTRL_Buffer[5] == 0x02)) || ((CTRL_Buffer[3] == 0x04) && (CTRL_Buffer[5] == 0x04))) {
				SHARE_Buffer[0]=0x03;
				ctrlFIFOWrite(1, SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21;
		  }
			/*else if( (CTRL_Buffer[3] == 0x02) ){
				SHARE_Buffer[0]=0x03;
				ctrlFIFOWrite(1,SHARE_Buffer);				
			}*/
			else if(((CTRL_Buffer[3] == 0x04) && (CTRL_Buffer[5] == 0x01)) || (CTRL_Buffer[3] == 0x0A)){
				SHARE_Buffer[0]=0x0F;
				ctrlFIFOWrite(1, SHARE_Buffer); 
			  XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x07) && (CTRL_Buffer[5] == 0x04)) {
				SHARE_Buffer[0]=0x01;
				ctrlFIFOWrite(1, SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else {
				XBYTE[EP0_CTRL] = 0x23; 
			}
		}	// GET INFO
		
		// GET_MIN (0x82)
		if(CTRL_Buffer[1] == USB_UVC_GET_MIN) {
			if ((CTRL_Buffer[5] == 0x04) && (CTRL_Buffer[6] == 0x04)) {
				SHARE_Buffer[0]=0x00;
				SHARE_Buffer[1]=0x00;
				SHARE_Buffer[2]=0x00;
				SHARE_Buffer[3]=0x00;
				ctrlFIFOWrite(4, SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if(((CTRL_Buffer[3] == 0x03) || 
				(CTRL_Buffer[3] == 0x07) || 
				(CTRL_Buffer[3] == 0x08) || 
				(CTRL_Buffer[3] == 0x01)) &&
				(CTRL_Buffer[5] == 0x02)){
				SHARE_Buffer[0]=0x00;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2, SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if(((CTRL_Buffer[3] == 0x05) || (CTRL_Buffer[3] == 0x01)) && (CTRL_Buffer[6] == 0x01)) {
				SHARE_Buffer[0]=0x00;
				ctrlFIFOWrite(1,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x04)){
				if((CTRL_Buffer[5] == 0x01)){
					SHARE_Buffer[0]=0x08;
					SHARE_Buffer[1]=0x00;
					SHARE_Buffer[2]=0x00;
					SHARE_Buffer[3]=0x00;
					ctrlFIFOWrite(4,SHARE_Buffer);
					XBYTE[EP0_CTRL] = 0x21;
				}
				else{
					SHARE_Buffer[0]=0x20;
					SHARE_Buffer[1]=0x00;
					ctrlFIFOWrite(2,SHARE_Buffer);     
					XBYTE[EP0_CTRL] = 0x21;   
				}
			}
			else if((CTRL_Buffer[3] == 0x02)){
				if((CTRL_Buffer[2] == 0x00)) {
					SHARE_Buffer[0]=0xF6;
					SHARE_Buffer[1]=0xFF;
					ctrlFIFOWrite(2,SHARE_Buffer);
					XBYTE[EP0_CTRL] = 0x21;
				}
				else{
					SHARE_Buffer[0]=0x00;
					SHARE_Buffer[1]=0xE5;
					ctrlFIFOWrite(2,SHARE_Buffer);
					XBYTE[EP0_CTRL] = 0x21;
				}
			}
			else if((CTRL_Buffer[3] == 0x06)){
				SHARE_Buffer[0]=0xFB;
				SHARE_Buffer[1]=0xFF;
				ctrlFIFOWrite(2,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x09)){
				SHARE_Buffer[0]=0x64;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x0A)){
				SHARE_Buffer[0]=0xF0;
				SHARE_Buffer[1]=0x0A;
				ctrlFIFOWrite(2,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x0A)){
				SHARE_Buffer[0]=0xF0;
				SHARE_Buffer[1]=0x0A;
				ctrlFIFOWrite(2,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x01) &&
				(CTRL_Buffer[4] == 0x01) && 
				(CTRL_Buffer[5] == 0x00) && 
				(CTRL_Buffer[6] == 0x1A)){
					SHARE_Buffer[0]=0x00;
					SHARE_Buffer[1]=0x00;
					SHARE_Buffer[2]=0x01;
					SHARE_Buffer[3]=0x03;
					SHARE_Buffer[4]=0x15;
					SHARE_Buffer[5]=0x16;
					SHARE_Buffer[6]=0x05;
					SHARE_Buffer[7]=0x00;
					SHARE_Buffer[8]=0x00;
					SHARE_Buffer[9]=0x00;
					SHARE_Buffer[10]=0x00;
					SHARE_Buffer[11]=0x00;
					SHARE_Buffer[12]=0x00;
					SHARE_Buffer[13]=0x00;
					SHARE_Buffer[14]=0x00;
					SHARE_Buffer[15]=0x00;
					SHARE_Buffer[16]=0x00;
					SHARE_Buffer[17]=0x00;
					SHARE_Buffer[18]=0x00;
					SHARE_Buffer[19]=0x58;
					SHARE_Buffer[20]=0x02;
					SHARE_Buffer[21]=0x00;
					SHARE_Buffer[22]=0x00;
					SHARE_Buffer[23]=0x0C;
					SHARE_Buffer[24]=0x00;
					SHARE_Buffer[25]=0x00;
					ctrlFIFOWrite(26,SHARE_Buffer);	
			}
		}	// GET_MIN
		
		// GET_MAX (0x83)
		if(CTRL_Buffer[1] == USB_UVC_GET_MAX) {
			if ((CTRL_Buffer[5] == 0x04) && (CTRL_Buffer[6] == 0x04)) {
				SHARE_Buffer[0]=0xFF;
				SHARE_Buffer[1]=0xFF;
				SHARE_Buffer[2]=0xFF;
				SHARE_Buffer[3]=0xFF;
				ctrlFIFOWrite(4,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if(((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x02) && (CTRL_Buffer[3] == 0x07) )){
				SHARE_Buffer[0]=0x0A;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if(((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x02)) && ((CTRL_Buffer[3] == 0x02 ||
			(CTRL_Buffer[3] == 0x08)))){
				SHARE_Buffer[0]=0x0A;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x04)){
				if((CTRL_Buffer[5] == 0x01)){
					SHARE_Buffer[0]=0x00;
					SHARE_Buffer[1]=0x40;
					SHARE_Buffer[2]=0x00;
					SHARE_Buffer[3]=0x00;
					ctrlFIFOWrite(4,SHARE_Buffer); 
					XBYTE[EP0_CTRL] = 0x21;
				}
				else if((CTRL_Buffer[5] == 0x02)){
					SHARE_Buffer[0]=0x30;
					SHARE_Buffer[1]=0x00;
					ctrlFIFOWrite(2,SHARE_Buffer);
					XBYTE[EP0_CTRL] = 0x21;
				}
			}
			else if((CTRL_Buffer[3] == 0x03)){
				SHARE_Buffer[0]=0x14;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x06)){
				SHARE_Buffer[0]=0x05;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x09)){
				SHARE_Buffer[0]=0xC8;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x0A)){
				SHARE_Buffer[0]=0x64;
				SHARE_Buffer[1]=0x19;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x01)){
				if((CTRL_Buffer[5] == 0x02)){
					SHARE_Buffer[0]=0x01;
					SHARE_Buffer[1]=0x00;
					ctrlFIFOWrite(2,SHARE_Buffer);  
					XBYTE[EP0_CTRL] = 0x21;
				}
				else if( (CTRL_Buffer[4] == 0x01) && (CTRL_Buffer[5] == 0x00) && (CTRL_Buffer[6] == 0x1A)){
					SHARE_Buffer[0]=0x00;
					SHARE_Buffer[1]=0x00;
					SHARE_Buffer[2]=0x01;
					SHARE_Buffer[3]=0x03;
					SHARE_Buffer[4]=0x15;
					SHARE_Buffer[5]=0x16;
					SHARE_Buffer[6]=0x05;
					SHARE_Buffer[7]=0x00;
					SHARE_Buffer[8]=0x00;
					SHARE_Buffer[9]=0x00;
					SHARE_Buffer[10]=0x00;
					SHARE_Buffer[11]=0x00;
					SHARE_Buffer[12]=0x00;
					SHARE_Buffer[13]=0x00;
					SHARE_Buffer[14]=0x00;
					SHARE_Buffer[15]=0x00;
					SHARE_Buffer[16]=0x00;
					SHARE_Buffer[17]=0x00;
					SHARE_Buffer[18]=0x00;
					SHARE_Buffer[19]=0x58;
					SHARE_Buffer[20]=0x02;
					SHARE_Buffer[21]=0x00;
					SHARE_Buffer[22]=0x00;
					SHARE_Buffer[23]=0x0C;
					SHARE_Buffer[24]=0x00;
					SHARE_Buffer[25]=0x00;
					ctrlFIFOWrite(26,SHARE_Buffer);	
				}
				else{
					SHARE_Buffer[0]=0xFF;
					ctrlFIFOWrite(1,SHARE_Buffer);
					XBYTE[EP0_CTRL] = 0x21;
				}
			}
			else if((CTRL_Buffer[3] == 0x05) && (CTRL_Buffer[5] == 0x02)){
				SHARE_Buffer[0]=0x02;
				ctrlFIFOWrite(1,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[2] == 0x01)){
				SHARE_Buffer[0]=0x00;
				SHARE_Buffer[1]=0x15;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
		}	// GET MAX
		
		// GET_RES
		if(CTRL_Buffer[1] == USB_UVC_GET_RES) {
			if (((CTRL_Buffer[5] == 0x04) && (CTRL_Buffer[6] == 0x04)) || ((CTRL_Buffer[5] == 0x01) && (CTRL_Buffer[6] == 0x04))) {
				SHARE_Buffer[0]=0x01;
				SHARE_Buffer[1]=0x00;
				SHARE_Buffer[2]=0x00;
				SHARE_Buffer[3]=0x00;
				ctrlFIFOWrite(4,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if(((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x02)) && (CTRL_Buffer[6] == 0x02)){
				SHARE_Buffer[0]=0x01;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x01) || ((CTRL_Buffer[3] == 0x05) && (CTRL_Buffer[5] == 0x02))){
				SHARE_Buffer[0]=0x01;
				ctrlFIFOWrite(1,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21; 
			}
			else if((CTRL_Buffer[2] == 0x01)){
				SHARE_Buffer[0]=0x00;
				SHARE_Buffer[1]=0x01;
				ctrlFIFOWrite(2,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21; 
			}
			else if(((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x01)) && (CTRL_Buffer[6] == 0x02)){
				SHARE_Buffer[0]=0x01;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
		}	// GET RES
		
		// GET_DEF
		if(CTRL_Buffer[1] == USB_UVC_GET_DEF) {
			if ((CTRL_Buffer[5] == 0x04) && (CTRL_Buffer[6] == 0x04)) {
				SHARE_Buffer[0]=0x00;
				SHARE_Buffer[1]=0x00;
				SHARE_Buffer[2]=0x00;
				SHARE_Buffer[3]=0x00;
				ctrlFIFOWrite(4,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x04) && (CTRL_Buffer[5] == 0x01)){
				SHARE_Buffer[0]=0x00;
				SHARE_Buffer[1]=0x02;
				SHARE_Buffer[2]=0x00;
				SHARE_Buffer[3]=0x00;
				ctrlFIFOWrite(4,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x02)){
				SHARE_Buffer[0]=0x02;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x03)){
				SHARE_Buffer[0]=0x0A;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x06)){
				SHARE_Buffer[0]=0x00;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if(((CTRL_Buffer[3] == 0x07) && (CTRL_Buffer[5] == 0x02))){
				SHARE_Buffer[0]=0x04;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x08)){
				SHARE_Buffer[0]=0x04;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			} 
			else if((CTRL_Buffer[3] == 0x09)){
				SHARE_Buffer[0]=0x82;
				SHARE_Buffer[1]=0x00;	
				ctrlFIFOWrite(2,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21; 
			}
			else if((CTRL_Buffer[3] == 0x0A)){
				SHARE_Buffer[0]=0x64;
				SHARE_Buffer[1]=0x19;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x01) && (CTRL_Buffer[5] == 0x02)){
				SHARE_Buffer[0]=0x01;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x04) && (CTRL_Buffer[5] == 0x02)){
				SHARE_Buffer[0]=0x22;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x05) && (CTRL_Buffer[5] == 0x02)){
				SHARE_Buffer[0]=0x02;
				ctrlFIFOWrite(1,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21; 
			}
			else if((CTRL_Buffer[3] == 0x01) && (CTRL_Buffer[5] == 0x04)){
				SHARE_Buffer[0]=0x00;
				ctrlFIFOWrite(1,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
		}	// GET DEF
		
		// GET_CUR
		if((CTRL_Buffer[1] == USB_UVC_GET_CUR)) {
			if((CTRL_Buffer[3] == 0x02)) {
				if((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x00) && (CTRL_Buffer[6] == 0x01)){
					SHARE_Buffer[0]=0x06;
					ctrlFIFOWrite(1,SHARE_Buffer);
					XBYTE[EP0_CTRL] = 0x21;
				}
				else if((CTRL_Buffer[4] == 0x02) && (CTRL_Buffer[5] == 0x02)){
					SHARE_Buffer[0]=0x00;
					SHARE_Buffer[1]=0x0B;
					ctrlFIFOWrite(2,SHARE_Buffer);
					XBYTE[EP0_CTRL] = 0x21;
				}
				else if((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x02)){
					SHARE_Buffer[0]=0x02;
					SHARE_Buffer[1]=0x00;
					ctrlFIFOWrite(2,SHARE_Buffer);
					XBYTE[EP0_CTRL] = 0x21;
				}
				else if((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x01)){
					SHARE_Buffer[0]=0x08;
					ctrlFIFOWrite(1,SHARE_Buffer);
					XBYTE[EP0_CTRL] = 0x21;
				}
			}
			else if(CTRL_Buffer[3] == 0x01){
				if((CTRL_Buffer[4] == 0x02)){
					SHARE_Buffer[0]=0x00;
					ctrlFIFOWrite(1,SHARE_Buffer);
					XBYTE[EP0_CTRL] = 0x21;
				}
				else if((CTRL_Buffer[4] == 0x00)) {
					SHARE_Buffer[0]=0x01;
					SHARE_Buffer[1]=0x00;
					ctrlFIFOWrite(2,SHARE_Buffer);
					XBYTE[EP0_CTRL] = 0x21;
				}
				else if((CTRL_Buffer[4] == 0x01)) {
					if (test_flag == 0){
						for(i = 0;i < 26;i++){
							SHARE_Buffer[i]=0x00;
						}
						SHARE_Buffer[2]=0x01;
						SHARE_Buffer[3]=0x01;
						SHARE_Buffer[4]=0x15;
						SHARE_Buffer[5]=0x16;
						SHARE_Buffer[6]=0x05;
						SHARE_Buffer[19]=0x60;
						SHARE_Buffer[20]=0x09;
						SHARE_Buffer[22]=0x80;
						SHARE_Buffer[23]=0x0A;
						ctrlFIFOWrite(26,SHARE_Buffer);
						XBYTE[EP0_CTRL] = 0x21;
						test_flag = 1;
					}
					else{
						for(i = 0;i < 26;i++)
						{
							SHARE_Buffer[i]=0x00;
						}
						SHARE_Buffer[2]=0x01;
						SHARE_Buffer[3]=0x01;
						SHARE_Buffer[4]=0x15;
						SHARE_Buffer[5]=0x16;
						SHARE_Buffer[6]=0x05;
						SHARE_Buffer[19]=0x60;
						SHARE_Buffer[20]=0x09;
						SHARE_Buffer[22]=0x00;
						SHARE_Buffer[23]=0x0C;
						ctrlFIFOWrite(26,SHARE_Buffer);
						XBYTE[EP0_CTRL] = 0x21;
						//test_flag = 2;
					}
				}
			}
			else if(CTRL_Buffer[3] == 0x03){
				if(CTRL_Buffer[4] == 0x00){
					SHARE_Buffer[0]=0x0A;
					SHARE_Buffer[1]=0x00;
					ctrlFIFOWrite(2,SHARE_Buffer); 
					XBYTE[EP0_CTRL] = 0x21;
				}
				else if(CTRL_Buffer[4] == 0x01){
					if(test_flag == 1){
						SHARE_Buffer[0]=0x01;
						SHARE_Buffer[1]=0x01;
						SHARE_Buffer[2]=0x00;
						SHARE_Buffer[3]=0x00;
						SHARE_Buffer[4]=0x06;
						SHARE_Buffer[5]=0x09;
						SHARE_Buffer[6]=0x00;
						SHARE_Buffer[7]=0x80;
						SHARE_Buffer[8]=0x0A;
						SHARE_Buffer[9]=0x00;
						SHARE_Buffer[10]=0x00;
						ctrlFIFOWrite(11,SHARE_Buffer);
						XBYTE[EP0_CTRL] = 0x21;
						test_flag = 3;
					}
					else{
						SHARE_Buffer[0]=0x01;
						SHARE_Buffer[1]=0x01;
						SHARE_Buffer[2]=0x00;
						SHARE_Buffer[3]=0x00;
						SHARE_Buffer[4]=0x06;
						SHARE_Buffer[5]=0x09;
						SHARE_Buffer[6]=0x00;
						SHARE_Buffer[7]=0x00;
						SHARE_Buffer[8]=0x0C;
						SHARE_Buffer[9]=0x00;
						SHARE_Buffer[10]=0x00;
						ctrlFIFOWrite(11,SHARE_Buffer);
						XBYTE[EP0_CTRL] = 0x21;
					}
				}
			}
			else if(CTRL_Buffer[3] == 0x06){
				SHARE_Buffer[0]=0x00;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x07)){
				SHARE_Buffer[0]=0x04;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if((CTRL_Buffer[3] == 0x08)){
				SHARE_Buffer[0]=0x04;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if(CTRL_Buffer[3] == 0x09){
				SHARE_Buffer[0]=0x82;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if(CTRL_Buffer[3] == 0x0A){
				SHARE_Buffer[0]=0x64;
				SHARE_Buffer[1]=0x19;
				ctrlFIFOWrite(2,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21; 
			}
			else if(CTRL_Buffer[3] == 0x0B){
				SHARE_Buffer[0]=0x01;
				ctrlFIFOWrite(1,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if(CTRL_Buffer[3] == 0x04){
				SHARE_Buffer[0]=0x00;
				SHARE_Buffer[1]=0x02;
				SHARE_Buffer[2]=0x00;
				SHARE_Buffer[3]=0x00;
				ctrlFIFOWrite(4,SHARE_Buffer); 
				XBYTE[EP0_CTRL] = 0x21;
			}
			
		}	// GET_CUR
		
		// GET_LEN
		if(CTRL_Buffer[1] == USB_UVC_GET_LEN) {
			if ((CTRL_Buffer[3] == 0x04) ||
					(CTRL_Buffer[3] == 0x05) || 
					(CTRL_Buffer[3] == 0x07) || 
					(CTRL_Buffer[3] == 0x10) ||
					(CTRL_Buffer[3] == 0x18)) {
				SHARE_Buffer[0]=0x04;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21;
			}
			else if(CTRL_Buffer[3] == 0x01){
				SHARE_Buffer[0]=0x01;
				SHARE_Buffer[1]=0x00;
				ctrlFIFOWrite(2,SHARE_Buffer);
				XBYTE[EP0_CTRL] = 0x21; 
			}
		}// GET_LEN
		
	}
}




/*
//----------------Based on Prolific 20130805-------------------//

// GET_STATUS
unsigned char usbGetStatus(void)
{
	
unsigned char x;

    if( (CTRL_Buffer[6] != 2) || (CTRL_Buffer[7] != 0) || (CTRL_Buffer[2] != 0) || (CTRL_Buffer[3] != 0) )
    {
				XBYTE[0xFC34] = 0x23;
        return( FALSE );
    }


    switch( CTRL_Buffer[0] )
    {
            // device status
        case 0x80:
            if( (CTRL_Buffer[4] == 0) && (CTRL_Buffer[5] == 0) )
            {
                /*
                if( ! busPowered ) {
                	x = 0x01;
                }
                if( RemoteWakeup ) {
                	x = 0x00 | 0x02;
                }
								*-/
                
            }
            else
            {
		        		XBYTE[0xFC34] = 0x23;	         // endpoin stall
                return( FALSE );
            }

            break;

            // interface status
        case 0x81:
            x = 0x00;
            break;

            // endpoint status
        case 0x82:
            if( CTRL_Buffer[5] != 0 )
            {

				        XBYTE[0xFC34] = 0x23;
                return( FALSE );
            }

            switch( CTRL_Buffer[4] )
            {
                case 0x00:
                    x = XBYTE[0xFC34];
                    break;
                case 0x81:
                    x = XBYTE[0xFC3C];
                    break;

                case 0x86:
                    x = XBYTE[0xFC64];
                    break;

                default:

					          XBYTE[0xFC64] = 0x23;
                    return( FALSE );
            }
			
            if( x & 0x02 )
            {
			
                x = 0x01;
            }
            else
            {
		
                x = 0x00;
            }
            break;

        default:

			XBYTE[0xFC34] = 0x23;
            return( FALSE );
    }

    XBYTE[0xF000] = x;
    XBYTE[0xF000+1] = 0;
    XBYTE[0xFC34] |= 0x21; //done

    return( TRUE );
}

// CLEAR_FEATURE
unsigned char usbClearFeature(void)
{
    if( (CTRL_Buffer[3]) || (CTRL_Buffer[6]) || (CTRL_Buffer[7]) )
    {
		    XBYTE[0xFC34] = 0x23;	
        return( FALSE );
    }


    switch( CTRL_Buffer[0] )
    {

            // device status
        case 0x00:
            if( (CTRL_Buffer[4] != 0) || (CTRL_Buffer[5] != 0) )
            {
			        	XBYTE[0xFC34] = 0x23;
                return( FALSE );
            }

            if( CTRL_Buffer[2] == DEVICE_REMOTE_WAKEUP ){
              	//RemoteWakeup = 0;
            }
            else{
				        XBYTE[0xFC34] = 0x23;
                return( FALSE );
            }
            break;


            // interface status
        case 0x01:
			      XBYTE[0xFC34] = 0x23;
            return( FALSE );


            // endpoint status
        case 0x02:
            if( CTRL_Buffer[2] == ENDPOINT_HALT )
            {

                if( CTRL_Buffer[5] != 0 )
                {
					          XBYTE[0xFC34] = 0x23;
                    return( FALSE );
                }

                switch( CTRL_Buffer[4] )
                {
                    case 0x00:
                        XBYTE[0xFC34] = 0x18;
                        break;
                    case 0x81:
                        XBYTE[0xFC3C] = 0x00;
                        XBYTE[0xFCB0] = 0x02;//ENP_TOG_CLR_L

                    case 0x86:
                        XBYTE[0xFC64] = 0x00;
                        XBYTE[0xFCB0] = 0x40;//ENP_TOG_CLR_L
						
                        break;							               
					          default:
						            XBYTE[0xFC34] = 0x23;
                        return( FALSE );
                }
            }
            else
            {
				        XBYTE[0xFC34] = 0x23;
                return( FALSE );
            }

            break;

        default:
			      XBYTE[0xFC34] = 0x23;
            return( FALSE );
    }

	  XBYTE[0xFC34] = 0x21;
    return( TRUE );
}

// GET_CONFIGURATION
unsigned char usbGetConfig(void)
{
    if( (CTRL_Buffer[2] == 0) || (CTRL_Buffer[3] == 0) ||
            (CTRL_Buffer[4] == 0) || (CTRL_Buffer[5] == 0) ||
            (CTRL_Buffer[6] == 1) || (CTRL_Buffer[7] == 0) )
    {
	    if( Configed ){
	        XBYTE[0xF000] = 0x01;
	    }
	    else
	    {
	        XBYTE[0xF000] = 0x00;
	    }
	    XBYTE[0xFC34] |= 0x21;
    }
	  else
	  {
		  XBYTE[0xFC34] = 0x23;
        return( FALSE );		
	  }
  
    return( TRUE );
}


// GET_DESCRIPTOR
unsigned char usbGetDescriptor(void)
{
unsigned char code *descp;
unsigned short rlen,i;
unsigned short cmd_len = 0;
unsigned int  offset = 0;

    cmd_len = (CTRL_Buffer[7]*256 ) +  CTRL_Buffer[6];

    switch( CTRL_Buffer[3] )
    {

      // Device Descriptor
      case 0x01:

            rlen = Ide_Dvc_D[0];
            descp = Ide_Dvc_D;
            if( (cmd_len < rlen) )
            {
                rlen = CTRL_Buffer[6];
            }

            ctrlFIFOWrite(rlen,descp);

            return( TRUE );
      
      // Configuration Descriptor & OTHER_SPEED_CONFIGURATION      
   		case 0x02:
    	case 0x07:
		
            rlen = Ide_Con_Dlen;
            for(i=0; i<Ide_Con_Dlen; i++)
            {
               SHARE_Buffer[i] = *(Con_Std_Descp+i);
            }

            SHARE_Buffer[1] = CTRL_Buffer[3];
             
		   	    if(cmd_len == 9)
			      {
			 	      ctrlFIFOWrite(rlen,SHARE_Buffer);	
			      }
			      else
			      {	
		          // Configuration Descriptor Interface Descriptor
		
					    rlen = UVC_Std_Dlen+Ide_Con_Dlen;
		          
		          if( (CTRL_Buffer[3] == 0x02)  )
		          {
		              for(i=0; i<UVC_Std_Dlen; i++)
		              {
		
		                  SHARE_Buffer[i+Ide_Con_Dlen] = *(InterfaceEP_Descp+i);
		              }
					    }
					    else
					    {
		              for(i=0; i<UVC_Std_Dlen; i++)
		              {
		                  SHARE_Buffer[i+Ide_Con_Dlen] = *(InterfaceEP_Oth_Descp+i);
		              }	
					    }
		                
							if( (cmd_len < rlen) )
		          {
		              rlen = cmd_len;
		          }
					
		          ctrlFIFOWrite(rlen,SHARE_Buffer);
				    }
            
            return( TRUE );

            break;

            // String Descriptor
       case 0x03:	// String Descriptor
            switch( CTRL_Buffer[2] )
            {
                case 0x00:
                    rlen = Ide_Str0_Dlen;
                    descp = Ide_Str0_D;
                    if( (cmd_len < rlen) )
                    {
                        rlen = cmd_len;
                    }

                    ctrlFIFOWrite(rlen,descp);
                    return( TRUE );
                    break;
                case 0x01:
                    rlen = Ide_Str1_Dlen;
                    descp = Ide_Str1_D;
                    if( (cmd_len < rlen) )
                    {
                        rlen = cmd_len;
                    }

                    ctrlFIFOWrite(rlen,descp);
                    return( TRUE );
                    break;
                case 0x02:

                    rlen = Ide_Str2_Dlen;
                    descp = Ide_Str2_D;
                    if( (cmd_len < rlen) )
                    {
                        rlen = cmd_len;
                    }

                    ctrlFIFOWrite(rlen,descp);
                    return( TRUE );
                    break;
                case 0x03:
                    rlen = Ide_Str2_Dlen;
                    descp = Ide_Str2_D;
                    if( (cmd_len < rlen) )
                    {
                        rlen = cmd_len;
                    }

                    ctrlFIFOWrite(rlen,descp);
                    return( TRUE );
                    break;

		            default:
		                goto L_GetDescpStall;
		         }
		
		         break;
       
       // Device_Qualifier Descriptor
       case 0x06:	
            rlen = Ide_Dvc_QF_Dlen;
            descp = Ide_Dvc_QF_D;
            if( (cmd_len < rlen) )
            {
                rlen = cmd_len;
            }

            ctrlFIFOWrite(rlen,descp);
            return( TRUE );
            break;
      
      // Interface Power Configuration  
	    case 0x08: 
	        goto L_GetDescpStall;
	        
	    // BOS Descriptor    
      case 0x0F:
		     	rlen = 12;
			    descp = Ide_BOS_D;
          if( (cmd_len < rlen) )
          {
              rlen = cmd_len;
          }
          ctrlFIFOWrite(rlen,descp);
			
          return( TRUE );
          break;	
        break;
    
      default:
        goto L_GetDescpStall;
    }

    return( TRUE );

L_GetDescpStall:
		XBYTE[0xFC34] = 0x23;        // enpoint 0 stall
    return( FALSE );
}


// GET_INTERFACE

unsigned char usbGetInterface(void)
{
    if( (CTRL_Buffer[2] != 0) || (CTRL_Buffer[3] != 0) || (CTRL_Buffer[6] != 1) || (CTRL_Buffer[7] != 0) )
    {
		    XBYTE[0xFC34] = 0x23; 
        return( FALSE );
    }


    if( Configed )
    {
        XBYTE[0xF000] = SHARE_Buffer[0];

        XBYTE[0xFC34] |= 0x21;

        return( TRUE );
    }
    else
    {
        return( FALSE );
    }
}


// SET_INTERFACE
unsigned char usbSetInterface(void)
{
	if(  (CTRL_Buffer[3] != 0) ||
	     (CTRL_Buffer[5] != 0) ||
	    (CTRL_Buffer[6] != 0) || (CTRL_Buffer[7] != 0) ) 
	{
			XBYTE[0xFC34] = 0x23;    	
	    return( FALSE );
	}
	else 
	{
		if( (CTRL_Buffer[0] == 0x01) && (CTRL_Buffer[1] == 0x0B) && (CTRL_Buffer[2] == 0x07) && 
			(CTRL_Buffer[3] == 0x00) && (CTRL_Buffer[4] == 0x01) && StartCIS1 == 1)
		{
		
			StartCIS1 = 0;
			StartCIS = 1;	
		}

		SHARE_Buffer[0] = CTRL_Buffer[2];
		XBYTE[0xFC64] = 0x10;
	   
		XBYTE[0xFC34] |= 0x21;
		return( TRUE );
	}

  return( TRUE );
}


// SET_ADDRESS

unsigned char usbSetAddress(void)
{
    if( (CTRL_Buffer[2] & 0x80) || (CTRL_Buffer[3] != 0) ||
            (CTRL_Buffer[4] != 0) || (CTRL_Buffer[5] != 0) ||
            (CTRL_Buffer[6] != 0) || (CTRL_Buffer[7] != 0) )
    {

		    XBYTE[0xFC34] = 0x23;
        return( FALSE );
    }
    else
    {

        XBYTE[0xFC14] = CTRL_Buffer[2];    // Store the address to device address register
        //Addressed = 1;
        XBYTE[0xFC34] = 0x21;
        return( TRUE );

    }
}


unsigned char usbSetConfig(void)
{

    if( (CTRL_Buffer[3] != 0) || (CTRL_Buffer[4] != 0) || (CTRL_Buffer[5] != 0) || (CTRL_Buffer[6] != 0) || (CTRL_Buffer[7] != 0) )
    {
		    XBYTE[0xFC34] = 0x23;
        return( FALSE );
    }

    switch( CTRL_Buffer[2] )
    {
        case 0:
            Configed = 0;
            XBYTE[0xFC04] = 0x01;

            break;

        case 1:
            Configed = 1;
            XBYTE[0xFC04] = 0x7F;
            XBYTE[0xFC08] = 0x01;       // enable EP0 interrupt
            XBYTE[0xFCB0] = 0x41;             // EP6 and EP0 toggle clear to DATA0
            
            break;

        default:
            return( FALSE );
    }
		XBYTE[0xFC64] = 0x10;
		XBYTE[0xFC34] |= 0x21;

    return( TRUE );

}



// SET_FEATURE

unsigned char usbSetFeature(void)
{
    if( (CTRL_Buffer[3] != 0) || (CTRL_Buffer[6] != 0) || (CTRL_Buffer[7] != 0) )
    {
		    XBYTE[0xFC34] = 0x23;
        return( FALSE );
    }

    switch( CTRL_Buffer[0] )
    {

        // device status
        case 0x00:
            if( CTRL_Buffer[2] == DEVICE_REMOTE_WAKEUP )
            {		  
                  RemoteWakeup = 1;
            }
            else if( CTRL_Buffer[2] == TEST_MODE )
            {
                if( (CTRL_Buffer[4] != 0) || (CTRL_Buffer[5] > 4) || (CTRL_Buffer[5] == 0) )
                {
					          XBYTE[0xFC34] = 0x23;
                    return( FALSE );
                }

                XBYTE[0xFC10] = CTRL_Buffer[5] | 0x08;
			
            }
            else
            {
				        XBYTE[0xFC34] = 0x23;
                return( FALSE );
            }

            break;


         // interface status
        case 0x01:
				    XBYTE[0xFC34] = 0x23;
            return( FALSE );
            
        // endpoint status
        case 0x02:
            if( CTRL_Buffer[2] == ENDPOINT_HALT )
            {
                if( CTRL_Buffer[5] != 0 )
                {
				            XBYTE[0xFC34] = 0x23;
                    return( FALSE );
                }

                switch( CTRL_Buffer[4] )
                {
                    case 0x00:
                        XBYTE[0xFC34] = 0x1A;           // EP0 flush FIFO, endpoint done
                    case 0x81:
                        XBYTE[0xFC3C] = 0x12;           // EP1 stall
                        break;
                    case 0x86:
                        XBYTE[0xFC64] = 0x12;           // EP6 stall
                        break;

                    default:
		
						            XBYTE[0xFC34] = 0x23;
                        return( FALSE );
                }
            }
            else
            {
		
				        XBYTE[0xFC34] = 0x23;
                return( FALSE );
            }

            break;

        default:
	
				    XBYTE[0xFC34] = 0x23;
            return( FALSE );
    }

    XBYTE[0xFC34] |= 0x21;
    return( TRUE );
}


void usb_video_class_function(void)
{
unsigned char UAC_len,i;


  UAC_len = CTRL_Buffer[6];

  // Host to Device
  if((CTRL_Buffer[0] == 0x21)) 
	{

  	if ((CTRL_Buffer[1] == 0x01) && (CTRL_Buffer[2] == 0x00) && (CTRL_Buffer[3] == 0x02) && (CTRL_Buffer[4] == 0x01)
		     && (CTRL_Buffer[5] == 0x00) && (CTRL_Buffer[6] == 0x1A) && (CTRL_Buffer[7] == 0x00))
		{			 
			 StartCIS1 = 1;
		}
	 
	
		while(XBYTE[0xFC34+2] != UAC_len)
		{
			if( BusReset || (XBYTE[0xFC01] & 0x80 == 0))
			{
				return ;
			}			
		}


    if (XBYTE[0xFC34+2] == UAC_len) 
		{
          
      for (i=0; i<UAC_len; i++) 
			{
         SHARE_Buffer[i] = XBYTE[0xF000+i];
		
      }
      
			if( (CTRL_Buffer[3] == 0x02) && (CTRL_Buffer[5] == 0x01))
			{
				if( (SHARE_Buffer[0] != 0x01) && (SHARE_Buffer[0] != 0x08) )
				{
					XBYTE[0xFCB2] |= 0x01;
					SHARE_Buffer[0] = 0x04;								
			    XBYTE[0xFC34] |= 0x04;   //flush FIFO
				  XBYTE[0xFC34] = 0x23;		 // EP0 stall		
				}
				else
				{			
					XBYTE[0xFCB2] |= 0x01;
					XBYTE[0xFC34] |= 0x04; //flush
			 	  XBYTE[0xFC34] = 0x21;					
				}
			}
			else
			{

				XBYTE[0xFCB2] |= 0x01;	//ENP_Interrupt_Restore_L
				XBYTE[0xFC34] |= 0x04; //flush
			  XBYTE[0xFC34] = 0x21;				
			}			

    }
  } 
	
	// Device to Host
	else if((CTRL_Buffer[0] == 0xA1)) 
	{
    if(UAC_len ==0) {
        XBYTE[0xFC34] = 0x21;
	  }
	
		// GET_INFO
		if(CTRL_Buffer[1] == 0x86) 
		{
      if((CTRL_Buffer[3] == 0x03) || (CTRL_Buffer[3] == 0x06) || (CTRL_Buffer[3] == 0x08)||
		      ((CTRL_Buffer[3] == 0x09) && (CTRL_Buffer[5] == 0x02))|| (CTRL_Buffer[3] == 0x01) || (CTRL_Buffer[3] == 0x05) ||
			    (CTRL_Buffer[3] == 0x10) || (CTRL_Buffer[3] == 0x18) || ((CTRL_Buffer[3] == 0x07) && (CTRL_Buffer[5] == 0x02))||
			    ((CTRL_Buffer[3] == 0x04) && (CTRL_Buffer[5] == 0x02)) || ((CTRL_Buffer[3] == 0x04) && (CTRL_Buffer[5] == 0x04))) 
		  {
			  
			    SHARE_Buffer[0]=0x03;
		      ctrlFIFOWrite(1,SHARE_Buffer);			    
		  }
			else if( (CTRL_Buffer[3] == 0x02) )
			{

					SHARE_Buffer[0]=0x03;
					ctrlFIFOWrite(1,SHARE_Buffer);				
			}

			else if( ( (CTRL_Buffer[3] == 0x04) && (CTRL_Buffer[5] == 0x01) ) )
		  {
		
		      SHARE_Buffer[0]=0x0F;
		      ctrlFIFOWrite(1,SHARE_Buffer); 
		  }
     	else if((CTRL_Buffer[3] == 0x07) && (CTRL_Buffer[5] == 0x04))
		  {		
		      SHARE_Buffer[0]=0x01;
		      ctrlFIFOWrite(1,SHARE_Buffer); 
	  	}
     	else
		  {
	       	XBYTE[0xFC34] = 0x23; 
		  }

		 
		}   //577 if

		
		//GET_MIN
		if(CTRL_Buffer[1] == 0x82) 
		{
        if ((CTRL_Buffer[5] == 0x04) && (CTRL_Buffer[6] == 0x04)) 
		  	{
			  
			    SHARE_Buffer[0]=0x00;
			    SHARE_Buffer[1]=0x00;
			    SHARE_Buffer[2]=0x00;
			    SHARE_Buffer[3]=0x00;
		      ctrlFIFOWrite(4,SHARE_Buffer);
		  	}

			  else if(((CTRL_Buffer[3] == 0x03) || (CTRL_Buffer[3] == 0x07) || (CTRL_Buffer[3] == 0x08) || (CTRL_Buffer[3] == 0x01)) &&
		         (CTRL_Buffer[5] == 0x02))
		  	{
		
		      SHARE_Buffer[0]=0x00;
			    SHARE_Buffer[1]=0x00;
		      ctrlFIFOWrite(2,SHARE_Buffer); 
		  	}

	    	else if(((CTRL_Buffer[3] == 0x05) || (CTRL_Buffer[3] == 0x01)) && (CTRL_Buffer[6] == 0x01))
		  	{
		
		      SHARE_Buffer[0]=0x00;
		      ctrlFIFOWrite(1,SHARE_Buffer); 
		  	}

	    	else if((CTRL_Buffer[3] == 0x04))
		  	{
			  	if((CTRL_Buffer[5] == 0x01))
			  	{

				  	SHARE_Buffer[0]=0x08;
			    	SHARE_Buffer[1]=0x00;
			    	SHARE_Buffer[2]=0x00;
			    	SHARE_Buffer[3]=0x00;
		       	ctrlFIFOWrite(4,SHARE_Buffer);
			  	}
			  	else
			  	{
			  
           	SHARE_Buffer[0]=0x20;
			    	SHARE_Buffer[1]=0x00;
		       	ctrlFIFOWrite(2,SHARE_Buffer);     
			  	}		       
		  	}

	    	else if((CTRL_Buffer[3] == 0x02))
		  	{
		    	if((CTRL_Buffer[2] == 0x00))
			    {
					  if(CTRL_Buffer[6] == 0x02)
					  {			   
				     	SHARE_Buffer[0]=0xF6;
				     	SHARE_Buffer[1]=0xFF;
			        ctrlFIFOWrite(2,SHARE_Buffer);
			     	}
					  else if(CTRL_Buffer[5] == 0x01)
					  {
						  SHARE_Buffer[0]=0x07;
					 	  XBYTE[0xFC34] = 0x23;
			     	}
				  }
			    else 
				  {

				 	  SHARE_Buffer[0]=0x00;
			     	SHARE_Buffer[1]=0xE5;
		        ctrlFIFOWrite(2,SHARE_Buffer);
				  }
		
		  	}

			  else if((CTRL_Buffer[3] == 0x06))
		    {
		   	   
		   	 	SHARE_Buffer[0]=0xFB;
			 	  SHARE_Buffer[1]=0xFF;
		     	ctrlFIFOWrite(2,SHARE_Buffer);
		    }

			  else if((CTRL_Buffer[3] == 0x09))
		    {
		   	   
		   	 	SHARE_Buffer[0]=0x64;
			 	  SHARE_Buffer[1]=0x00;
		     	ctrlFIFOWrite(2,SHARE_Buffer);
		    }

			  else if((CTRL_Buffer[3] == 0x0A))
		    {
		   	   
		   	 	SHARE_Buffer[0]=0xF0;
			 	  SHARE_Buffer[1]=0x0A;
		     	ctrlFIFOWrite(2,SHARE_Buffer);
		    }

			  else if((CTRL_Buffer[3] == 0x0A))
		    {
		   	   
		   	 	SHARE_Buffer[0]=0xF0;
			 	  SHARE_Buffer[1]=0x0A;
		     	ctrlFIFOWrite(2,SHARE_Buffer);
		    }
			  else if( (CTRL_Buffer[3] == 0x01) )
			  {
				  	for(i = 0;i < 26;i++)
			   		{
			      		SHARE_Buffer[i]=0x00;
			   		}
			   		
					  if(test_flag == 1)
					  {
				   		SHARE_Buffer[2]=0x01;
				   		SHARE_Buffer[3]=0x01;
				   		SHARE_Buffer[4]=0x15;
				   		SHARE_Buffer[5]=0x16;
				   		SHARE_Buffer[6]=0x05;
				   		SHARE_Buffer[19]=0x60;
				   		SHARE_Buffer[20]=0x09;
				   		SHARE_Buffer[22]=0x00;
				   		SHARE_Buffer[23]=0x0C;						
					  }
					  else
					  {
				   		SHARE_Buffer[2]=0x01;
				   		SHARE_Buffer[3]=0x01;
				   		SHARE_Buffer[4]=0x15;
				   		SHARE_Buffer[5]=0x16;
				   		SHARE_Buffer[6]=0x05;
				   		SHARE_Buffer[19]=0x60;
				   		SHARE_Buffer[20]=0x09;
				   		SHARE_Buffer[22]=0x80;
				   		SHARE_Buffer[23]=0x0A;
					  }
		       		ctrlFIFOWrite(26,SHARE_Buffer);	
			  }

		}	 

    // GET_MAX
		if(CTRL_Buffer[1] == 0x83) 
		{
        if ((CTRL_Buffer[5] == 0x04) && (CTRL_Buffer[6] == 0x04)) 
		    {
			  
			    SHARE_Buffer[0]=0xFF;
			    SHARE_Buffer[1]=0xFF;
			    SHARE_Buffer[2]=0xFF;
			    SHARE_Buffer[3]=0xFF;
		      ctrlFIFOWrite(4,SHARE_Buffer);
		    }

			  else if(((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x02) && (CTRL_Buffer[3] == 0x07) ))
		    {
		
		      SHARE_Buffer[0]=0x0A;
			    SHARE_Buffer[1]=0x00;
		      ctrlFIFOWrite(2,SHARE_Buffer); 
		
		    }
			  else if(((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x02)) && ((CTRL_Buffer[3] == 0x02 ||
		          (CTRL_Buffer[3] == 0x08))))
		    {
		
		      SHARE_Buffer[0]=0x0A;
			    SHARE_Buffer[1]=0x00;
		      ctrlFIFOWrite(2,SHARE_Buffer); 
		
		    }

	    	else if((CTRL_Buffer[3] == 0x04))
		    {
				  if((CTRL_Buffer[5] == 0x01))
			 	  {
			 
			   		SHARE_Buffer[0]=0x00;
			   		SHARE_Buffer[1]=0x40;
			   		SHARE_Buffer[2]=0x00;
			   		SHARE_Buffer[3]=0x00;
		      	ctrlFIFOWrite(4,SHARE_Buffer); 
			 	  }
			 	  else if((CTRL_Buffer[5] == 0x02))
			 	  {
			   
			   		SHARE_Buffer[0]=0x30;
			   		SHARE_Buffer[1]=0x00;
		     		ctrlFIFOWrite(2,SHARE_Buffer);
			 
			 	  }
		      		
		 	  }

	    	else if((CTRL_Buffer[3] == 0x03))
		    {
		
		      SHARE_Buffer[0]=0x14;
			    SHARE_Buffer[1]=0x00;
		      ctrlFIFOWrite(2,SHARE_Buffer); 
		  	}

		 	  else if((CTRL_Buffer[3] == 0x06))
		  	{
		
		      SHARE_Buffer[0]=0x05;
			    SHARE_Buffer[1]=0x00;
		      ctrlFIFOWrite(2,SHARE_Buffer); 
		  	}

		  	else if((CTRL_Buffer[3] == 0x09))
		  	{
		
		      SHARE_Buffer[0]=0xC8;
			    SHARE_Buffer[1]=0x00;
		      ctrlFIFOWrite(2,SHARE_Buffer); 
		  	}

		  	else if((CTRL_Buffer[3] == 0x0A))
		  	{		
		      SHARE_Buffer[0]=0x64;
			    SHARE_Buffer[1]=0x19;
		      ctrlFIFOWrite(2,SHARE_Buffer); 
		  	}

		  	else if((CTRL_Buffer[3] == 0x01))
		  	{
		
		    	if((CTRL_Buffer[5] == 0x02))
			   	{
			   
			     	SHARE_Buffer[0]=0x01;
				   	SHARE_Buffer[1]=0x00;
		       	ctrlFIFOWrite(2,SHARE_Buffer);  
			   	}
				  else if( (CTRL_Buffer[4] == 0x01) )	 
				  {
					  for(i = 0;i < 26;i++)
			   		{
			      		SHARE_Buffer[i]=0x00;
			   		}
			   		
					  if(test_flag == 1)
					  {
				   		SHARE_Buffer[2]=0x01;
				   		SHARE_Buffer[3]=0x01;
				   		SHARE_Buffer[4]=0x15;
				   		SHARE_Buffer[5]=0x16;
				   		SHARE_Buffer[6]=0x05;
				   		SHARE_Buffer[19]=0x60;
				   		SHARE_Buffer[20]=0x09;
				   		SHARE_Buffer[22]=0x00;
				   		SHARE_Buffer[23]=0x0C;						
					  }
					  else
					  {
				   	 	SHARE_Buffer[2]=0x01;
				   		SHARE_Buffer[3]=0x01;
				   		SHARE_Buffer[4]=0x15;
				   		SHARE_Buffer[5]=0x16;
				   		SHARE_Buffer[6]=0x05;
				   		SHARE_Buffer[19]=0x60;
				   		SHARE_Buffer[20]=0x09;
				   		SHARE_Buffer[22]=0x80;
				   		SHARE_Buffer[23]=0x0A;					
					  }

		       		ctrlFIFOWrite(26,SHARE_Buffer);	
				  }
			   	else
			   	{
			   
			   	 	  SHARE_Buffer[0]=0xFF;
		         	ctrlFIFOWrite(1,SHARE_Buffer);
			   	}
		        
		
		 	  }

		  	else if((CTRL_Buffer[3] == 0x05) && (CTRL_Buffer[5] == 0x02))
		  	{
		
		       SHARE_Buffer[0]=0x02;
		       ctrlFIFOWrite(1,SHARE_Buffer); 
		  	}

		  	else if((CTRL_Buffer[2] == 0x01))
		  	{
		
		       SHARE_Buffer[0]=0x00;
			     SHARE_Buffer[1]=0x15;
		       ctrlFIFOWrite(2,SHARE_Buffer); 
		  	}
			  else if( (CTRL_Buffer[3] == 0x02) && (CTRL_Buffer[5] == 0x00) & (CTRL_Buffer[4] == 0x00))
			  {
				   SHARE_Buffer[0]=0x07;
				   XBYTE[0xFC34] = 0x23;
			  }
			  else
			  { 
				   XBYTE[0xFC34] = 0x23;	
							
			  }

		}	 // end GET_MAX


		// GET_RES
		if(CTRL_Buffer[1] == 0x84) 
		{
        if (((CTRL_Buffer[5] == 0x04) && (CTRL_Buffer[6] == 0x04)) || ((CTRL_Buffer[5] == 0x01) && (CTRL_Buffer[6] == 0x04))) 
		  	{
			  
			    SHARE_Buffer[0]=0x01;
			    SHARE_Buffer[1]=0x00;
			    SHARE_Buffer[2]=0x00;
			    SHARE_Buffer[3]=0x00;
		      ctrlFIFOWrite(4,SHARE_Buffer);
		  	}

			  else if(((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x02)) && (CTRL_Buffer[6] == 0x02))
		  	{
		
		      SHARE_Buffer[0]=0x01;
			    SHARE_Buffer[1]=0x00;
		      ctrlFIFOWrite(2,SHARE_Buffer); 
		
		  	}

	    	else if((CTRL_Buffer[3] == 0x01) || ((CTRL_Buffer[3] == 0x05) && (CTRL_Buffer[5] == 0x02)))
		  	{
		
		      SHARE_Buffer[0]=0x01;
		      ctrlFIFOWrite(1,SHARE_Buffer);
		  	}

	    	else if((CTRL_Buffer[2] == 0x01))
		  	{
		
		      SHARE_Buffer[0]=0x00;
			    SHARE_Buffer[1]=0x01;
		      ctrlFIFOWrite(2,SHARE_Buffer);
		  	}

			  else if(((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x01)) && (CTRL_Buffer[6] == 0x02))
		  	{
		
		      SHARE_Buffer[0]=0x01;
			    SHARE_Buffer[1]=0x00;
		      ctrlFIFOWrite(2,SHARE_Buffer); 
		  	}
			  else if(((CTRL_Buffer[3] == 0x02) && (CTRL_Buffer[5] == 0x01)) && ( (CTRL_Buffer[6] == 0x01) || (CTRL_Buffer[6] == 0x04) ) )
		  	{
		
		      SHARE_Buffer[0]=0x09;
		      ctrlFIFOWrite(1,SHARE_Buffer); 
		  	}
		}	//  end GET_RES


		// GET_DEF
		if(CTRL_Buffer[1] == 0x87) 
		{
      if ((CTRL_Buffer[5] == 0x04) && (CTRL_Buffer[6] == 0x04)) 
		  {
			  
			  SHARE_Buffer[0]=0x00;
			  SHARE_Buffer[1]=0x00;
			  SHARE_Buffer[2]=0x00;
			  SHARE_Buffer[3]=0x00;
		    ctrlFIFOWrite(4,SHARE_Buffer);
		  }

			else if((CTRL_Buffer[3] == 0x04) && (CTRL_Buffer[5] == 0x01))
		 	{
		
		    SHARE_Buffer[0]=0x00;
			  SHARE_Buffer[1]=0x02;
			  SHARE_Buffer[2]=0x00;
			  SHARE_Buffer[3]=0x00;
		    ctrlFIFOWrite(4,SHARE_Buffer); 
		  }

	    else if((CTRL_Buffer[3] == 0x02))
		  {
			  if(CTRL_Buffer[6] == 0x02)
			  {	
			     SHARE_Buffer[0]=0x02;
				   SHARE_Buffer[1]=0x00;
			     ctrlFIFOWrite(2,SHARE_Buffer); 
			  }
			  else if(CTRL_Buffer[6] == 0x01)
			  {
				   SHARE_Buffer[0]=0x08;
				   ctrlFIFOWrite(1,SHARE_Buffer); 
			  }
		  }

	    else if((CTRL_Buffer[3] == 0x03))
		  {
		
		    SHARE_Buffer[0]=0x0A;
			  SHARE_Buffer[1]=0x00;
		    ctrlFIFOWrite(2,SHARE_Buffer); 
		  }

	    else if((CTRL_Buffer[3] == 0x06))
		  {
		
		    SHARE_Buffer[0]=0x00;
			  SHARE_Buffer[1]=0x00;
		    ctrlFIFOWrite(2,SHARE_Buffer); 
		  }

			else if(((CTRL_Buffer[3] == 0x07) && (CTRL_Buffer[5] == 0x02)))
		  {
		
		    SHARE_Buffer[0]=0x04;
			  SHARE_Buffer[1]=0x00;
		    ctrlFIFOWrite(2,SHARE_Buffer); 
		  }

      else if((CTRL_Buffer[3] == 0x08))
		  {
		
		    SHARE_Buffer[0]=0x04;
			  SHARE_Buffer[1]=0x00;
		    ctrlFIFOWrite(2,SHARE_Buffer); 
		  } 

		 	else if((CTRL_Buffer[3] == 0x09))
		  {
		
		    SHARE_Buffer[0]=0x82;
			  SHARE_Buffer[1]=0x00;	
		    ctrlFIFOWrite(2,SHARE_Buffer);
		  }

		  else if((CTRL_Buffer[3] == 0x0A))
		  {
		
		    SHARE_Buffer[0]=0x64;
			  SHARE_Buffer[1]=0x19;
		    ctrlFIFOWrite(2,SHARE_Buffer); 
		  }

		  else if((CTRL_Buffer[3] == 0x01) && (CTRL_Buffer[5] == 0x02))
		  {
		
		    SHARE_Buffer[0]=0x01;
			  SHARE_Buffer[1]=0x00;
		    ctrlFIFOWrite(2,SHARE_Buffer); 
		  }

		  else if((CTRL_Buffer[3] == 0x04) && (CTRL_Buffer[5] == 0x02))
		  {	
		    SHARE_Buffer[0]=0x22;
			  SHARE_Buffer[1]=0x00;
		    ctrlFIFOWrite(2,SHARE_Buffer); 
		  }

		  else if((CTRL_Buffer[3] == 0x05) && (CTRL_Buffer[5] == 0x02))
		  {
		
			  SHARE_Buffer[0]=0x02;
		    ctrlFIFOWrite(1,SHARE_Buffer);
		  }

		  else if((CTRL_Buffer[3] == 0x01) && (CTRL_Buffer[5] == 0x04))
		  {
			  SHARE_Buffer[0]=0x00;
		    ctrlFIFOWrite(1,SHARE_Buffer); 
		  }
		}	// end GET_DEF

		// GET_CUR
		if((CTRL_Buffer[1] == 0x81)) 
		{
      if((CTRL_Buffer[3] == 0x02)) 
		  {
			 	if((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x00) && (CTRL_Buffer[6] == 0x01))
			  {  // Request error code control
   		
		    	ctrlFIFOWrite(1,SHARE_Buffer);
		  
			  }

			 	else if((CTRL_Buffer[4] == 0x02) && (CTRL_Buffer[5] == 0x02))
			 	{
			 
			  	SHARE_Buffer[0]=0x00;
			  	SHARE_Buffer[1]=0x0B;
		    	ctrlFIFOWrite(2,SHARE_Buffer);
			 	}

			 	else if((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x02))
			 	{
				
					SHARE_Buffer[0]=0x06;
					XBYTE[0xFC34] = 0x23;
			 	}
			  
			 	else if((CTRL_Buffer[4] == 0x00) && (CTRL_Buffer[5] == 0x01))
			 	{
					SHARE_Buffer[0]=0x06;
					XBYTE[0xFC34] = 0x23;					
			 	}
		  }

			else if(CTRL_Buffer[3] == 0x01)
		 	{

		    if((CTRL_Buffer[4] == 0x02))
			 	{
			 
			  	SHARE_Buffer[0]=0x00;
		    	ctrlFIFOWrite(1,SHARE_Buffer);
			 	}
		
		    else if( (CTRL_Buffer[4] == 0x00) && ( (CTRL_Buffer[5] == 0x01) || (CTRL_Buffer[5] == 0x02) ) ) 
			 	{
					SHARE_Buffer[0]=0x06;
			  	XBYTE[0xFC34] = 0x23;
			 	}

			 	else if((CTRL_Buffer[4] == 0x01)) 
			 	{
				   
			 		if (test_flag == 0)
					{
			   		for(i = 0;i < 26;i++)
			   		{
			      		SHARE_Buffer[i]=0x00;
			   		}
			   		SHARE_Buffer[2]=0x01;
			   		SHARE_Buffer[3]=0x01;
			   		SHARE_Buffer[4]=0x15;
			   		SHARE_Buffer[5]=0x16;
			   		SHARE_Buffer[6]=0x05;
			   		SHARE_Buffer[19]=0x60;
			   		SHARE_Buffer[20]=0x09;
			   		SHARE_Buffer[22]=0x80;
			   		SHARE_Buffer[23]=0x0A;
		      	ctrlFIFOWrite(26,SHARE_Buffer);
					  test_flag = 1;
					}				
					else
					{
			   		for(i = 0;i < 26;i++)
			   		{
			      		SHARE_Buffer[i]=0x00;
			   		}
			   		SHARE_Buffer[2]=0x01;
			   		SHARE_Buffer[3]=0x01;
			   		SHARE_Buffer[4]=0x15;
			   		SHARE_Buffer[5]=0x16;
			   		SHARE_Buffer[6]=0x05;
			   		SHARE_Buffer[19]=0x60;
			   		SHARE_Buffer[20]=0x09;
			   		SHARE_Buffer[22]=0x00;
			   		SHARE_Buffer[23]=0x0C;
		       	ctrlFIFOWrite(26,SHARE_Buffer);
					}
				 
			 	}
		
		 	}

	    else if(CTRL_Buffer[3] == 0x03)
		  {
			  	if(CTRL_Buffer[4] == 0x00)
			  	{
					  SHARE_Buffer[0]=0x06;
					  XBYTE[0xFC34] = 0x23;
			  	}

			  	else if(CTRL_Buffer[4] == 0x01)
			  	{
			  		if(test_flag == 1)
					  {
			      	SHARE_Buffer[0]=0x01;
			    	  SHARE_Buffer[1]=0x01;
			    	  SHARE_Buffer[2]=0x00;
			    	  SHARE_Buffer[3]=0x00;
			    	  SHARE_Buffer[4]=0x06;
			    	  SHARE_Buffer[5]=0x09;
			    	  SHARE_Buffer[6]=0x00;
			    	  SHARE_Buffer[7]=0x80;
			    	  SHARE_Buffer[8]=0x0A;
					    SHARE_Buffer[9]=0x00;
					    SHARE_Buffer[10]=0x00;
					    ctrlFIFOWrite(11,SHARE_Buffer);
					    test_flag = 3;
					  }

			  		else
					  {
			    	  SHARE_Buffer[0]=0x01;
			    	  SHARE_Buffer[1]=0x01;
			    	  SHARE_Buffer[2]=0x00;
			    	  SHARE_Buffer[3]=0x00;
			    	  SHARE_Buffer[4]=0x06;
			    	  SHARE_Buffer[5]=0x09;
			    	  SHARE_Buffer[6]=0x00;
			    	  SHARE_Buffer[7]=0x00;
			    	  SHARE_Buffer[8]=0x0C;
					    SHARE_Buffer[9]=0x00;
					    SHARE_Buffer[10]=0x00;
					    ctrlFIFOWrite(11,SHARE_Buffer);
					  }
			  	}
		      
		
		 	}

		  else if( (CTRL_Buffer[3] == 0x04) || (CTRL_Buffer[3] == 0x0C) || (CTRL_Buffer[3] == 0x0F) ||
					 (CTRL_Buffer[3] == 0x10) || (CTRL_Buffer[3] == 0x0D) || (CTRL_Buffer[3] == 0x0E) ||
                     (CTRL_Buffer[3] == 0x11) || (CTRL_Buffer[3] == 0x05) || (CTRL_Buffer[3] == 0x12) ||
					 (CTRL_Buffer[3] == 0x0B) || (CTRL_Buffer[3] == 0x0A) || (CTRL_Buffer[3] == 0x09) ||
					 (CTRL_Buffer[3] == 0x08) || (CTRL_Buffer[3] == 0x07) || (CTRL_Buffer[3] == 0x06) )
		  {
					SHARE_Buffer[0]=0x06;
					XBYTE[0xFC34] = 0x23;
					
		  }

		}  // end GET_CUR

		
		
		// GET_LEN
		if(CTRL_Buffer[1] == 0x85) 
		{
      if ( (CTRL_Buffer[3] == 0x05) || (CTRL_Buffer[3] == 0x07) || (CTRL_Buffer[3] == 0x10) ||
		        (CTRL_Buffer[3] == 0x18)) 
		  {
			  
			  SHARE_Buffer[0]=0x04;
			  SHARE_Buffer[1]=0x00;
		    ctrlFIFOWrite(2,SHARE_Buffer);
		  }
			else if( (CTRL_Buffer[3] == 0x04) && (CTRL_Buffer[5] == 0x01) && (CTRL_Buffer[6] == 0x04))
			{
				SHARE_Buffer[0]=0x07;
				XBYTE[0xFC34] = 0x23;				
			}

			else if(CTRL_Buffer[3] == 0x01)
		  {
		
		    SHARE_Buffer[0]=0x01;
			  SHARE_Buffer[1]=0x00;
		    ctrlFIFOWrite(2,SHARE_Buffer);
		
		  }
			else if( (CTRL_Buffer[3] == 0x02) && (CTRL_Buffer[5] == 0x01))
			{
				SHARE_Buffer[0]=0x07;
				XBYTE[0xFC34] = 0x23;
			}
			else
			{
				XBYTE[0xFC34] = 0x23;	
			}

		}  // end GET_LEN
  }
}		// end usb_video_class_function()
*/
