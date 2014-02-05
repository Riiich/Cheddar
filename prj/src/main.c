#include "platform.h"

void blinker(void);

void main(void)
{
	Initial_Sub();

#ifdef BLINKER
	blinker();
#else
	UpdateUSB();
#endif
}

void Initial_Sub()
{
	InitC51();
	
	InitCIS();
}

void InitC51(void)
{
#ifdef OV9155
	P3_7=1;		//BruceC, CIS_D5_EN = 1
	P3_6=0;		//BruceC, SWT0_EN =0

	P2_3 = 0;	//LED init
	P2_4 = 1;
	P2_5 = 0;
	P2_6 = 0;

	P2_2 =1;	//for Touch pull-high
#endif

	IE = 0x83;					//ET0, EX0(USB), EX1(SATA) interrupt enable
	IP = 0x01;					//Set EX0 to higher priority

	EIE = 0x01;					//EX2 Enable //20140116 BruceC
	EIP = 0x01;					//EX2 to higher priority //20140116 BruceC
	
	IT0 = 0;					//INT0 level trigger
	IT1 = 0;                    //INT1 level trigger //20140116 BruceC for P1.5 touch

	InitTimer();

	XBYTE[0xFF00] = 0x11;		//boot_ctrl_reg1. bit0:boot from embedded flash. bit4:ICE interface enable.
	XBYTE[0xFF02] = 0x01;		//Clk_Ctrl. CPU Clock:30MHz
	XBYTE[0xFF07] = 0x05;		//special_func_select. bit0:CIS. bit3:I2C-1.
#ifdef OV9155
	XBYTE[0xFF04] = 0x40;		//clk_ctrl. bit[6:7]:10,MCLK=12MHz.01,MCLK=24MHz.
#else
	XBYTE[0xFF04] = 0x80;		//clk_ctrl. bit[6:7]:10,MCLK=12MHz.01,MCLK=24MHz.
#endif

	XBYTE[0xFF0B]  = 0x0A;		//P2_2 interupt //20140116 BruceC
	//XBYTE[0xFF0A]  = 0xD0;		//P1_5 interupt
}

void blinker(void) {
	unsigned int cnt1 = 0;
	unsigned int cnt2 = 0;

	bit led_state=0;
	bit led_state2=0;

	while(1){
		cnt1++;
		if (cnt1 == 0xFFFF){
			cnt2++;
			cnt1=0;
		}
		if (cnt2 == 6){
			led_state = ~led_state;
#ifdef PL2551EVB64	
			//P2_3 = led_state;
			P2_4 = ~led_state;
			P2_5 = led_state;
#endif
			cnt1=0;
			cnt2=0;
		}
	}
}
