#include "platform.h"

void IntVecDecode();

//External Interrupt 0 ISR entry
void int0_isr( void ) interrupt 0
{
	IntVecDecode();
}

void IntVecDecode()
{
	switch(XBYTE[0xFC0A]){
		case 0x00:
			VbusInt = 1;
			break;
		case 0x01:
			BusReset = 1;
			break;
		case 0x02:
			BusResume = 1;
			break;
		case 0x03:
			BusSuspend = 1;
			break;
		case 0x04:
			break;
		case 0x05:
			break;
		case 0x06:
			break;
		case 0x07:	
			break;
		// device 0, endpoint 0, Control endpoint
		case 0x08:
			Abort = 1;
			break;
		case 0x09:
			PktRcv = 1;
			break;
		case 0x0A:
			break;
		case 0x0B:
			break;
		// device 0, endpoint 1, Bulk Out endpoint
		case 0x0C:
			break;
		case 0x0D:
			BulkOutRcv = 1;
			break;
		case 0x0E:
			break;
		case 0x0F:
			break;
		default:
			break;
	}

}

