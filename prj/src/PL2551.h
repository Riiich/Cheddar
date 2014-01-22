
//General
#define	TRUE	1
#define	FALSE	0

//Registers Operation
#define XBYTE	((unsigned char volatile xdata *) 0)



//--------------PL2551 Register Table-------------------//

#define CONTROL_FIFO						0xF000

// 0xFF02 Clock Control Register
#define	CLOCK_CTRL							XBYTE[0xFF02]
#define	CLK_60M									0x0000
#define	CLK_30M									0x0001
#define	CLK_24M									0x0010
#define	CLK_12M									0x0011
#define	CLK_6M									0x0100
#define	CLK_3M									0x0101
#define	CLK_1500K								0x0110
#define	CLK_750K								0x0111
#define	CLK_375K								0x1000

// USB2.0 Device interface (0xFC00-0xFCB3)
#define	DEV_CTRL  							XBYTE[0xFC00]
#define	DC_ATTACH_DEVICE				(0x01)
#define	DC_SUSPEND_ENABLE				(0x02)
#define	DC_REMOTE_WAKE_ENABLE		(0x04)
#define	DC_ISR_MASTER_ENABLE		(0x08)
#define	DC_SOFT_RESET						(0x10)
#define	DC_INT0_INT_CLEAR				(0x20)
#define	DC_FORCE_SUSPEND				(0x40)

#define	DEV_STATUS							XBYTE[0xFC01]
#define	DS_USB_VBUS							(0x80)
#define	DS_USB_VBUS_DETACH			((DEV_STATUS & 0x80) == 0)

// Endpoint Enable Register Low
#define ENP_ENABLE_L						XBYTE[0xFC04]
#define ENP_ENABLE_SET					(0x01)				// This bit is always set to enable default control endpoint.
#define ENP1_ENABLE							(0x02)
#define ENP2_ENABLE							(0x04)
#define ENP3_ENABLE							(0x08)
#define ENP4_ENABLE							(0x10)
#define ENP5_ENABLE							(0x20)
#define ENP6_ENABLE							(0x40)
#define ENP7_ENABLE							(0x80)

// Endpoint Enable Register High
#define ENP_ENABLE_H						XBYTE[0xFC05]
#define ENP8_ENABLE							(0x01)
#define ENP9_ENABLE							(0x02)
#define ENP10_ENABLE						(0x04)
#define ENP11_ENABLE						(0x08)
#define ENP12_ENABLE						(0x10)
#define ENP13_ENABLE						(0x20)
#define ENP14_ENABLE						(0x40)
#define ENP15_ENABLE						(0x80)

// Master Interrupt Enable Register
#define MASTER_INT_ENABLE				XBYTE[0xFC06]
#define	INT0_EN_USB_VBUS				(0x01)
#define	INT0_EN_USB_BUS_RESET   (0x02)
#define	INT0_EN_USB_RESUME      (0x04)
#define	INT0_EN_USB_SUSPEND     (0x08)
#define	INT0_EN_USB_SOF         (0x10)
#define	INT0_EN_USB_uSOF				(0x20)
//#define	INT0_EN_PDMA            (0x40)
//#define	INT0_EN_SERIAL          (0x80)

// Endpoint Interrupt Mask Register Low
#define ENP_IMASK_L  						XBYTE[0xFC08]
#define ENP0_INT_ENABLE					(0x01)
#define ENP1_INT_ENABLE					(0x02)
#define ENP2_INT_ENABLE					(0x04)
#define ENP3_INT_ENABLE					(0x08)
#define ENP4_INT_ENABLE					(0x10)
#define ENP5_INT_ENABLE					(0x20)
#define ENP6_INT_ENABLE					(0x40)
#define ENP7_INT_ENABLE					(0x80)

// Endpoint Interrupt Mask Register High
#define ENP_IMASK_H							XBYTE[0xFC09]

#define INT_VECTOR  						XBYTE[0xFC0A]
#define CHIP_VER  							XBYTE[0xFC0B]
#define FRAME_CTRL							XBYTE[0xFC0C]
#define FRAME_NO_L							XBYTE[0xFC0E]
#define FRAME_NO_H							XBYTE[0xFC0F]


#define TEST_MODE 							XBYTE[0xFC10]
//#define TEST_MODE_CTRL				0xFC10
#define SHORT_PKG_CTRL					0xFC12
#define PHY_VENDER_STATUS				0xFC13

#define DEVICE0_ADDRESS					XBYTE[0xFC14]

/*
#define DEV0_ADDR 				0xFC14
#define	DEV0_OFFSET				0xFC15
#define DEV1_ADDR 				0xFC16
#define	DEV1_OFFSET				0xFC17
#define DEV2_ADDR 				0xFC18
#define	DEV2_OFFSET				0xFC19
#define DEV3_ADDR 				0xFC1A
#define	DEV3_OFFSET				0xFC1B
#define DEV4_ADDR 				0xFC1C
#define	DEV4_OFFSET				0xFC1D
*/

/*--------------------------------------------*/
/*		EndPoint registers											*/
/*--------------------------------------------*/
/*
#define EPx_MLB(x)  		XBYTE[0xFC30 + ((x)<<3)]
#define EPx_MHB(x)  		XBYTE[0xFC31 + ((x)<<3)] 

// Endpoint Configuration register
#define EPx_CFG(x)  		XBYTE[0xFC32 + ((x)<<3)] 

// Endpoint Configuration register bitfields
	#define EP_CFG_SIZE			0x07
	#define EP_CFG_SIZE_8		0x00
	#define EP_CFG_SIZE_16		0x01
	#define EP_CFG_SIZE_32		0x02
	#define EP_CFG_SIZE_64		0x03
	#define	EP_CFG_SIZE_512		0x04
	#define EP_CFG_SIZE_1023	0x05

	#define EP_CFG_TYPE			0x38
	#define EP_CFG_TYPE_CTRL	0x00
	#define EP_CFG_TYPE_RES		0x08
	#define EP_CFG_TYPE_BULKIN	0x10
	#define EP_CFG_TYPE_BULKOUT	0x18
	#define EP_CFG_TYPE_ISOIN	0x20
	#define EP_CFG_TYPE_ISOOUT	0x28
	#define EP_CFG_TYPE_INTIN	0x30
	#define EP_CFG_TYPE_INTOUT	0x38

	#define	EP_CFG_RESERVED		0xC0

// Endpoint Control register
#define EPx_CTRL(x)  		XBYTE[0xFC34 + ((x)<<3)] 

// Endpoint Control register bitfields
	#define EP_CTRL_DONE	0x01
	#define EP_CTRL_STALL	0x02
	#define EP_CTRL_FLUSH	0x04
	#define EP_CTRL_CMDREAD	0x08

	#define EP_CTRL_ABORT	0x10
	#define EP_CTRL_NAK		0x10
	#define EP_CTRL_OVR		0x10

	#define EP_CTRL_RCV		0x20
	#define EP_CTRL_SENT	0x40
	#define EP_CTRL_FIFO	0x80

	#define EP_CTRL_AINT	(EP_CTRL_RCV | EP_CTRL_SENT | EP_CTRL_ABORT) // All Endpoint Interrupts...

#define EPx_DCL(x)  		XBYTE[0xFC36 + ((x)<<3)] 
#define EPx_DCH(x)  		XBYTE[0xFC37 + ((x)<<3)] 

// Endpoint Status register
#define EPx_STATUS(x)		XBYTE[0xFC37 + ((x)<<3)] 

// Endpoint Status register bitfields
	#define EP_STATUS_DCH		0x0F
	#define EP_STATUS_STALL		0x10
	#define EP_STATUS_EMPTY		0x20
	#define EP_STATUS_FULL		0x40
	#define EP_STATUS_RESERVED	0x80
	*/
/*------------------------------------------------------------------*/



#define POWER_CTRL				0xFC20
#define EP0_CFG  				0xFC32
#define EP0_CTRL  				0xFC34
#define EP_done					0x01
#define EP_stall				0x02
#define EP_flush				0x03
#define EP0_STATUS_L  			0xFC36
#define EP0_STATUS_H  			0xFC37
#define EP1_CFG  				0xFC3A
#define EP1_CTRL  				0xFC3C
#define EP1_STATUS_L   			0xFC3E
#define EP1_STATUS_H  			0xFC3F
#define EP2_CFG     			0xFC42
#define EP2_CTRL    			0xFC44
#define EP2_STATUS_L		  	0xFC46
#define EP2_STATUS_H	  		0xFC47
#define	EP3_CFG      		    0xFC4A
#define	EP3_CTRL        		0xFC4C
#define EP3_STATUS_L  			0xFC4E
#define EP3_STATUS_H  			0xFC4F
#define	EP6_CFG      		     	0xFC62 
#define	EP6_CTRL        		 	0xFC64 
#define ENP_TOG_CLR				0xFCB0
#define ENP_IRE_STORE			0xFCB2

//	End-Point FIFO size register
#define EP1_FIFO_SIZE_REG           0xFC38
#define EP2_FIFO_SIZE_REG           0xFC40
#define EP3_FIFO_SIZE_REG           0xFC48
#define EP4_FIFO_SIZE_REG           0xFC50
#define EP5_FIFO_SIZE_REG           0xFC58
#define EP6_FIFO_SIZE_REG           0xFC60
#define EP7_FIFO_SIZE_REG           0xFC68
#define EP8_FIFO_SIZE_REG           0xFC70
#define EP9_FIFO_SIZE_REG           0xFC78
#define EP10_FIFO_SIZE_REG          0xFC80
#define EP11_FIFO_SIZE_REG          0xFC88
#define EP12_FIFO_SIZE_REG          0xFC90
#define EP13_FIFO_SIZE_REG          0xFC98
#define EP14_FIFO_SIZE_REG          0xFCA0
#define EP15_FIFO_SIZE_REG          0xFCA8


#define I2C1_CTRL_2551B               0xFDE0
#define I2C1_Status                   0xFDE1
#define I2C1_DATA_REG_2551B           0xFDE2
#define I2C1_SCL                      0xFDE3
#define I2C1_FIFO_CTRL                0xFDE7

#define CIS_CTRL                      0xFCC3
#define Peripheral_1_CLK_CTRL					0xFF03
#define InterruptSourceJudg_reg1      0xFF0E



// ----------------------------- Prolific DcD CPU -----------------------------
/*
// Timer2
sfr T2CON	= 0xC8;
sfr RLDL	= 0xCA;
sfr RCAP2L	= 0xCA;
sfr RLDH	= 0xCB;
sfr RCAP2H	= 0xCB;
sfr TL2		= 0xCC;
sfr TH2		= 0xCD;

sbit CP_RL2 = T2CON^0;
sbit C_T2   = T2CON^1;
sbit TR2    = T2CON^2;
sbit EXEN2  = T2CON^3;
sbit TCLK   = T2CON^4;
sbit RCL    = T2CON^5;
sbit EXF2   = T2CON^6;
sbit TF2    = T2CON^7;

// WatchDog Timer
sfr WDCON	= 0xD8;
sfr TA		= 0xEB;

// I2C
sfr I2C_CTRL	= 0xBC;
sfr I2C_STATUS	= 0xBD;
sfr I2C_DATA	= 0xBF;
sfr I2C_CKL	= 0xBB;
sfr I2C_CKH	= 0xBA;

sbit EI2C	= EIE^1;
sbit EINT2	= EIE^0;
sbit EINT3	= EIE^1;
sbit EINT4	= EIE^2;
sbit EINT5	= EIE^3;
sbit EINT6	= EIE^4;

*/
