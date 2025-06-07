#include <LPC21xx.H>
#include"delay.h"
#include"4bitcode.h"
#define sw1 14
#define sw2 15
#define sw3 16
int i;

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
void left_indicator();
void right_indicator();
void can2_init(void){
	PINSEL1|=0x00014000;//P0.23-->RD2 & P0.24-->TD2
	VPBDIV=1; //PCLK=60MHz
	C2MOD=0x1; //CAN2 into Reset  Mode 
	AFMR=0x2; //accept all receiving messages(data/remote)
	C2BTR=0x001C001D; //B125Kbps @ PLCK=60MHz(BRP=Pclk/bit rate*16)
	C2MOD=0x0; //CAN1 into Normal Mode
}

void can2_tx(CAN2_MSG m1)
{
  C2TID1=m1.id;
C2TFI1=(m1.dlc<<16);	
	if(m1.rtr==0) //if data frame
	{
		C2TFI1&=~(1<<30);//RTR=0
		C2TDA1=m1.byteA; //lower 4bytes of data
		C2TDB1=m1.byteB; //upper 4bytes of data
	}
	else{
		C2TFI1|=(1<<30); //RTR=1
	}
	C2CMR=(1<<0)|(1<<5);//Start Xmission & select Tx Buf1
	//C2CMR=(3<<0)|(1<<5);//Start Xmission & select Tx Buf1
	while((C2GSR&(1<<3))==0);//wait for data Xmission.
}


int main()
{
	int count=0;
	CAN2_MSG m1;
	lcd_confi();
	can2_init();	
	m1.id=0x000001AF;
	m1.rtr=0;
	m1.dlc=4;
	m1.byteA=0;
	m1.byteB=0;
    
	lcd_string("BODYCONTROL MODULE");
	delay_ms(500);
	lcd_com(0X01);
	
	while(1)
		{
		if(((IOPIN0>>sw1)&1)==0)
		{
		delay_ms(200);
		count++;
		if(count==1)
		{
	     m1.id=0x01;		
	     can2_tx(m1);
		  CGRAM_LEFT(8);
		  lcd_com(0x80);
          lcd_data(0);
		  delay_ms(500);
		 }
	    if(count==2)
		 {
		 lcd_com(0x01);
		 m1.id=0x02;
		 can2_tx(m1);
		 delay_ms(500);
		  count=0;
		}
		}
  else if(((IOPIN0>>sw2)&1)==0)
		{
		delay_ms(200);
		count++;
		if(count==1)
		{
	     m1.id=0x03;		
	     can2_tx(m1);
		  CGRAM_RIGHT(8);
		  lcd_com(0x80);
          lcd_data(0);	
		  delay_ms(500);
		 }
	    if(count==2)
		 {
		 lcd_com(0x01);
		 m1.id=0x04;
		 can2_tx(m1);
		 delay_ms(500);
		  count=0;
		}
		}
		else if(((IOPIN0>>sw3)&1)==0)
		{
			delay_ms(200);
			count++;
			if(count==1)
			{
			    lcd_com(0x01);
				lcd_string("wiper slow");
		        m1.id=0x05;		
		        can2_tx(m1);
			//	delay_ms(500);
			}		
				if(count==2)
			{
				lcd_com(0x01);
				lcd_string("wiper medium");
		        m1.id=0x06;		
		        can2_tx(m1);
			//	delay_ms(500);
		  }
			if(count==3)
			{
				lcd_com(0x01);
				lcd_string("wiper high");
		        m1.id=0x07;		
		        can2_tx(m1);
			//	delay_ms(500);
				count=0;
		  }
		}
		else 
		{
		m1.id=0x00;
        can2_tx(m1);
		}

		delay_s(1);
		}
		}