#include "platform.h"

bdata unsigned char	flag1;
sbit VbusInt	= flag1^0;
sbit test_flag  = flag1^1;
sbit BusReset 	= flag1^2;
sbit BusSuspend = flag1^3;
sbit BusResume 	= flag1^4;
sbit PktRcv	 	= flag1^5;
sbit BulkOutRcv = flag1^6;
sbit Abort		= flag1^7;

bdata unsigned char	flag2;
sbit busPowered			= flag2^0;
sbit RemoteWakeup		= flag2^1;
sbit Configed				= flag2^2;
sbit	Addressed			= flag2^3;
sbit	T0Timeout			= flag2^4;
sbit	T1Timeout			= flag2^5;
sbit	T2Timeout			= flag2^6;

// SETUP TOKEN Packet structure
data BYTE _bmRequestType;
data BYTE _bRequest;
data WORD _wValue;
data WORD _wIndex;
data WORD _wLength;
/*
bmRequestType	= CTRL_Buffer[0]
bRequest			= CTRL_Buffer[1]
wValue			= CTRL_Buffer[2:3]  // little endian
wIndex			= CTRL_Buffer[4:5]  // little endian  
wLength			= CTRL_Buffer[6:7]  // little endian
*/
xdata unsigned char CTRL_Buffer[8];
xdata unsigned char SHARE_Buffer[64];
xdata unsigned char GLOBAL_test;

// Value holds current configuration
BYTE	data		bConfiguration;

// Value holds the current device address
BYTE	data		bDeviceAddress;


xdata unsigned char StartCIS = 0;
xdata unsigned char StartCIS1 = 0;


