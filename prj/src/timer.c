#include "platform.h"

xdata unsigned char 	ms_TH_value;
xdata unsigned char 	ms_TL_value;
xdata unsigned short 	timerCount;
xdata unsigned short	SuspendTimerCount;

void InitTimer(void)
{
	unsigned char 	x, *p;
	unsigned short 	us;

	// 1 ms
	x = 2;
	us = 0xFFFF - (2500 * x) + 1;
	p = (unsigned char *)&us;

	ms_TH_value = p[0];
	ms_TL_value = p[1];

	TH0 = ms_TH_value;
	TL0 = ms_TL_value;

	TH1 = ms_TH_value;
	TL1 = ms_TL_value;

	TH2 = ms_TH_value;
	TL2 = ms_TL_value;
	
	TMOD |= 0x11;		// Timer1 Mode 1, Timer0 Mode 1

	T1Timeout = 0;

	IE = IE | 0x0A;
}

void DelayMS(unsigned short msec)
{
	TR1 = 0;

	T1Timeout = 0;
	timerCount = msec;

	TH1 = ms_TH_value;
	TL1 = ms_TL_value;

	TR1 = 1;

	while( ! T1Timeout ){
		if( BusReset || (XBYTE[0xFC01] & 0x80 == 0)){
			return ;
		}
	}
}

void SetTimer( unsigned short msec )
{
	TR1=0;
	T1Timeout = 0;
	SuspendTimerCount = msec;
	TH1 = ms_TH_value;
	TL1 = ms_TL_value;
	TR1=1;
}

bit IsTimerExpired(void)
{
	return ( SuspendTimerCount ) ? 0 : 1;
}

void ResetTimer(void)
{
	TR1 = 0;
	SuspendTimerCount = 0;
}

// Interrupt service routine for Timer 0
void Timer0_ISR() interrupt 1 // using 3 
{
	TR0 = 0;
}

// Interrupt service routine for Timer 1
void Timer1_ISR() interrupt 3 // using 3 
{
	TR1 = 0;
	TH1 = ms_TH_value;
	TL1 = ms_TL_value;

	timerCount--;

	if( ! timerCount ) {
		T1Timeout = 1;
	}
	else {
		TR1 = 1;
	}

	if (SuspendTimerCount>0){
		SuspendTimerCount--;
		TR1 = 1;
	}
}