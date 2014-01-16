#include "platform.h"

void InitC51();
void Initial_Sub();

void main(void)
{
	Initial_Sub();

	UpdateUSB();
}

void Initial_Sub()
{
	InitC51();

}

void InitC51(void)
{
#ifdef OV7675
	P3_2 = 0;					// Pin 2 of port 3, used to control the power down pin of OV7675. 1 => power down;  0 => normal operation
#else ifdef OV9155
	P3_0 = 0;
#endif

	IE = 0x83;                  // ET0, EX0(USB), EX1(SATA) interrupt enable
	IP = 0x01;                  // Set EX0 to higher priority
	IT0 = 0;                    // INT0 level trigger

	//InitTimer();

	XBYTE[0xFF00] = 0x11;
	XBYTE[0xFF02] = 0x01;		// CPU Clock:30MHz
	XBYTE[0xFF07] = 0x05; 	  	// select CIS function

	XBYTE[0xFF04] = 0x80;
	XBYTE[0xFCC3] = 0x40; 		//CPU force master clock enable
}


