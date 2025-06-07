#include <LPC21xx.H>
#include"delay.h"
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
CAN2_MSG m1;
int i,value;
int main()
{	
PINSEL0 |=0x00000008;
PWMTCR=0x02;
PWMPR=0x1D;
PWMMR0=20000;
PWMMR3=650;
PWMMCR=0x0202;
PWMLER=0x09;
PWMPCR=0x800;
PWMTCR=0x09;
	
	can2_init();	
	while(1)
		{		
		can2_rx(&m1);//data-frame
		if(m1.id==0x05)
		{
			for(i=0; i<=5; i++)
		{
			value = 1000;
			PWMMR3=value;
			PWMLER=0x09;
			delay_ms(1000);
			value = 2000;
			PWMMR3=value;
			PWMLER=0x09;
			delay_ms(1000);
			}
		}
		if(m1.id==0x06)
		{
			for(i=0; i<=5; i++)
	    	{
			value = 1000;
			PWMMR3=value;
			PWMLER=0x09;
			delay_ms(500);
			value = 2000;
			PWMMR3=value;
			PWMLER=0x09;
			delay_ms(500);
			}
		}
		if(m1.id==0x07)
		{
			for(i=0; i<=5; i++)
		{
			value = 1000;
			PWMMR3=value;
			PWMLER=0x09;
			delay_ms(100);
			value = 2000;
			PWMMR3=value;
			PWMLER=0x09;
			delay_ms(100);
			}
		}
		delay_s(1);
		}
		}