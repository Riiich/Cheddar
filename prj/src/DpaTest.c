#include "platform.h"

/*********************************************************************************************************
** Function name:       PWM_ON
** Descriptions:        TurnOn PWM function, setup PWM0~3
** input parameters:    void
** Returned value:      void
** Date:				20140116 BruceC
*********************************************************************************************************/
void PWM_ON()
{
	XBYTE[0xFF07]|=0x02; //Open PWM func
	
  //Banner PWM1
	XBYTE[0xFCF3]=0x80; //Control Prescalar:0.468Hz~120Hz
  	XBYTE[0xFCF5]=0xFF; //Divider 0.468Hz=120Hz/(0xFF+1)
	XBYTE[0xFCF4]=0x80; //Duty 0x80/256= 50%
	
	//Banner PWM2
 	 XBYTE[0xFCF6]=0x80; //Control Prescalar:0.468Hz~120Hz
  	XBYTE[0xFCF8]=0x80; //divider 0.93Hz=120Hz/(0x80+1)
	XBYTE[0xFCF7]=0x80; //Duty 0x80/256= 50%
	
	//Banner PWM3
	XBYTE[0xFCF9]=0x80; //Control Prescalar:0.468Hz~120Hz
  	XBYTE[0xFCFB]=0x40; //divider 1.846Hz=120Hz/(0x40+1)
	XBYTE[0xFCFA]=0x80; //Duty 0x80/256= 50%
	
	//Banner PWM0
	XBYTE[0xFCF0]=0x80; //Control Prescalar:0.468Hz~120Hz
  	XBYTE[0xFCF2]=0x10; //divider 7.058Hz=120Hz/(0x10+1)
	XBYTE[0xFCF1]=0x80; //Duty 0x80/256= 50%
	
}

/*********************************************************************************************************
** Function name:       PWM_OFF
** Descriptions:        TurnOff PWM function
** input parameters:    void
** Returned value:      void
** Date:				20140116 BruceC
*********************************************************************************************************/
void PWM_OFF()
{
	XBYTE[0xFF07]&=0xFD; //Close PWM func
	P2_3 = 0;
	P2_4 = 0;
	P2_5 = 0;
	P2_6 = 0;
}

/*********************************************************************************************************
** Function name:       PWM_ON
** Descriptions:        TurnOn PWM function, setup PWM0~3
** input parameters:    void
** Returned value:      void
** Date:				20140116 BruceC
*********************************************************************************************************/
void LED_MODE1()
{
	PWM_OFF();
	P2_3 = 1;	
}

/*********************************************************************************************************
** Function name:       PWM_ON
** Descriptions:        TurnOn PWM function, setup PWM0~3
** input parameters:    void
** Returned value:      void
** Date:				20140116 BruceC
*********************************************************************************************************/
void LED_MODE2()
{
	PWM_OFF();
	P2_5 = 1;	
}

/*********************************************************************************************************
** Function name:       PWM_ON
** Descriptions:        TurnOn PWM function, setup PWM0~3
** input parameters:    void
** Returned value:      void
** Date:				20140116 BruceC
*********************************************************************************************************/
void LED_MODE3()
{
	PWM_OFF();
	P2_6 = 1;	
}

/*********************************************************************************************************
** Function name:       PWM_ON
** Descriptions:        TurnOn PWM function, setup PWM0~3
** input parameters:    void
** Returned value:      void
** Date:				20140116 BruceC
*********************************************************************************************************/
void LED_MODE4()
{
	PWM_OFF();
	P2_4 = 1;	
}

/*********************************************************************************************************
** Function name:       GPIO_ISR_1
** Descriptions:        Interrupt function for GPIO1_5 Touch
** input parameters:    void
** Returned value:      void
*********************************************************************************************************/
void GPIO_ISR_1() interrupt 2 using 3 
{
  Touchflag=~Touchflag;
	
	if(Touchflag)
	{
		PWM_ON();
		//LED_MODE3();
	}
	else
	{
		PWM_OFF();
	}
	XBYTE[0xFF0E] &= (~0x40); //Interrupt GPIO1
}


/*********************************************************************************************************
** Function name:       GPIO_ISR_7
** Descriptions:        Interrupt function for GPIO2_2 Touch
** input parameters:    void
** Returned value:      void
*********************************************************************************************************/
void GPIO_ISR_7() interrupt 7 using 3 
{
  Touchflag=~Touchflag;
	
	if(Touchflag)
	PWM_ON();
	//LED_MODE4();
	else
	PWM_OFF();
	
	XBYTE[0xFF0F] &= (~0x20); //Interrupt GPIO2
}

#ifdef DEBUG
void DEMO_SUSPEND()
{
	
	EIE = 0x00;//Close P2.2 Touch interrupt
	EIP = 0x00;//Close P2.2 Touch interrupt
	
	IE |= 0x04;//Enable P1.5 Touch Interrupt
	IP |= 0x04;
	
	P3_7=0; //BruceC,CIS_D5_EN = 0
	P3_6=1; //BruceC,SWT0_EN =1
	
	P1_5 =1;
	XBYTE[0xFF0B]  = 0x00;  //P2_2 interupt close
	XBYTE[0xFF0A]  = 0xD0;  //P1_5 interupt
	P2_2=0;
	P3_0=1; //OV9155 PWDN
	
	LED_MODE1(); //
	
}

void DEMO_RESUME()
{

	//sensor_init(); //I2C need set again??

	IE &= 0xFB; //Close P1.5 Touch Interrupt
	IP &= 0xFB;
	EIE = 0x01;//Enable P2.2 Touch Interrupt
	EIP = 0x00;//Close P2.2 Touch interrupt
	
	P3_7=1; //BruceC,CIS_D5_EN = 0
	P3_6=0; //BruceC,SWT0_EN =1
	
	P2_2=1;
	XBYTE[0xFF0A]  = 0x00;  //P1_5 interupt close
	XBYTE[0xFF0B]  = 0x0A;  //P2_2 interupt 
	P1_5 =0;
	P3_0=0; //OV9155 On
	initialCIS_I2C();
	
	LED_MODE2();
}
#endif
