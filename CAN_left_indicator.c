#include <LPC21xx.H>
#include"delay.h"
#define led1 1<<19
#define led2 1<<18
#define led3 1<<17

typedef  unsigned int u32;
typedef  signed int s32;
typedef  unsigned char u8;
typedef  signed char s8;
typedef  unsigned short int u16;
typedef  signed short int s16;

typedef struct CAN2
{
	u32 id;
	u32 rtr;
	u32 dlc;
	u32 byteA;
	u32 byteB;
}CAN2_MSG;

void can2_init(void){
	PINSEL1|=0x00014000;//P0.23-->RD2 & P0.24-->TD2
	VPBDIV=1; //PCLK=60MHz
	C2MOD=0x1; //CAN2 into Reset  Mode 
	AFMR=0x2; //accept all receiving messages(data/remote)
	C2BTR=0x001C001D; //B125Kbps @ PLCK=60MHz(BRP=Pclk/bit rate*16)
	C2MOD=0x0; //CAN1 into Normal Mode
}

void can2_rx(CAN2_MSG *m1){
	while((C2GSR&0x1)==0);
	m1->id=C2RID;
	m1->dlc=(C2RFS>>16)&0xF;
	m1->rtr=(C2RFS>>30)&0x1;
	if(m1->rtr==0){ //if data frame
		m1->byteA=C2RDA;
		m1->byteB=C2RDB;
	}
	C2CMR=(1<<2);//free receiver buffer(imp)
} 
int flag=0;
CAN2_MSG m1;
void int_isr(void)__irq
{
		can2_rx(&m1);
		if(m1.id==0x01)
		{
		flag=1;
		}
		if(m1.id==0x02)
		{
		  flag=0;
		}	 
	VICVectAddr=0;
	}


int main()
{
PINSEL0 |=0;
	IODIR0 |=led1|led2|led3;	
	VICIntSelect=0;
	VICVectCntl0=0x20|27;
	VICVectAddr0=(int)int_isr;
	VICIntEnable=1<<27;
	can2_init();
	C2IER=0x01;
	 IOSET0=led1|led2|led3;
	 while(1)
	 {
		while(flag)
		{		
	    IOCLR0=led1;
		delay_ms(250);
		IOSET0=led1;
		IOCLR0=led2;
		delay_ms(250);
		IOSET0=led2;
		IOCLR0=led3;
		delay_ms(250);
		IOSET0=led3;
	}
	 	 IOSET0=led1|led2|led3;
	// delay_s(1);
}	

	  }











