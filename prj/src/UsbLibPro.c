#include "platform.h"

/*
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
*/

void controlCMD()
{
	unsigned char x;
	x = ctrlFIFORead(8, CTRL_Buffer);
	
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
					XBYTE[EP0_CTRL] = 0x21;
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
