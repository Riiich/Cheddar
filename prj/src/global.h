
void InitC51();
void Initial_Sub();


#define LED_ON	0
#define LED_OFF	1

#ifndef _BYTE_DEF_
#define _BYTE_DEF_
typedef unsigned char BYTE;
#endif /* _BYTE_DEF_ */

#ifndef _WORD_DEF_
#define _WORD_DEF_
//typedef union {unsigned int i; unsigned char c[2];} WORD;
typedef unsigned int	WORD;
#endif /* _WORD_DEF_ */

#define LOW_BYTE(x)  ( (BYTE) (x & 0xFF) )
#define HIGH_BYTE(x) ( (BYTE) ((x) >> 8) )
#define TOWORD(h, l) ( (((WORD) (h)) << 8) | (l) )


#ifndef _CIS_DEF_
#define _CIS_DEF_
#ifdef OV9155
#define SENSOR_POWER_PIN	P3_0
#else
#define SENSOR_POWER_PIN	P3_2
#endif
#define SENSOR_PIN_ON		0
#define SENSOR_PIN_OFF		1
#endif /* _CIS_DEF_ */

extern bdata unsigned char flag1;
extern bit	VbusInt;
extern bit	test_flag;
extern bit	BusReset;
extern bit	BusSuspend;
extern bit	BusResume;
extern bit	PktRcv;
extern bit	BulkOutRcv;
extern bit	Abort;

extern bdata unsigned char	flag2;
extern bit busPowered;
extern bit RemoteWakeup;
extern bit Configed;
extern bit	Addressed;
extern bit	T0Timeout;
extern bit	T1Timeout;
extern bit	T2Timeout;

// USB Setup token
extern data BYTE _bmRequestType;
extern data BYTE _bRequest;
extern data WORD _wValue;
extern data WORD _wIndex;
extern data WORD _wLength;

extern BYTE	data		bConfiguration;
extern BYTE	data		bDeviceAddress;

extern xdata unsigned char CTRL_Buffer[];
extern xdata unsigned char SHARE_Buffer[];
extern xdata unsigned char GLOBAL_test;
extern xdata unsigned char StartCIS;
extern xdata unsigned char StartCIS1;


