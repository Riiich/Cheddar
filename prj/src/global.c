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






xdata unsigned char CTRL_Buffer[8];
xdata unsigned char SHARE_Buffer[64];
xdata unsigned char GLOBAL_test;

xdata unsigned char StartCIS = 0;
xdata unsigned char StartCIS1 = 0;


