#include "platform.h"

void main(void)
{
	Initial_Sub();

	UpdateUSB();
}

void Initial_Sub()
{
	InitC51();
	
#ifndef OV9155	// !define
	InitCIS();
#endif
}

void InitC51(void)
{
	IE = 0x83;					//ET0, EX0(USB), EX1(SATA) interrupt enable
	IP = 0x01;					//Set EX0 to higher priority
	IT0 = 0;					//INT0 level trigger

	InitTimer();

	XBYTE[0xFF00] = 0x11;		//boot_ctrl_reg1. bit0:boot from embedded flash. bit4:ICE interface enable.
	XBYTE[0xFF02] = 0x01;		//Clk_Ctrl. CPU Clock:30MHz
	
#ifdef OV9155
	XBYTE[0xFF04] = 0x40;		//clk_ctrl. bit[6:7]:10,MCLK=12MHz.01,MCLK=24MHz.
#else
	XBYTE[0xFF04] = 0x80;		//clk_ctrl. bit[6:7]:10,MCLK=12MHz.01,MCLK=24MHz.
#endif

	XBYTE[0xFF07] = 0x05;		//special_func_select. bit0:CIS. bit3:I2C-1.
}


