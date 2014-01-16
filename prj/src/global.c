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



