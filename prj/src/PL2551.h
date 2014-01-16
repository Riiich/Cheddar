
// General
#define	TRUE	1
#define	FALSE	0

//Registers
#define XBYTE	((unsigned char volatile xdata *) 0)
#define XWORD	((unsigned int	volatile xdata	*) 0)
#define CBYTE 	((unsigned char volatile code *) 0)
#define DBYTE	((unsigned char volatile idata	*) 0)



//--------------PL2551 Register Table-------------------//

//End-Point FIFO size register
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


