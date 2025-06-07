
#define lcd_d 0xf<<20                                                
#define RS 1<<17
#define RW 1<<18						 
#define e 1<<19
unsigned char cgram_lut1[]={0x00,0x04,0x1e,0x1f,0x1e,0x04,0x00,0x00};
unsigned char cgram_lut2[]={0x00,0x04,0x0f,0x1f,0x0f,0x04,0x00,0x00};
void lcd_confi(void);
void lcd_com(unsigned char);
void lcd_data(unsigned char);
void lcd_string(unsigned char*);
void delay_ms(unsigned int ms);
/*void delay_ms(unsigned int ms)
{
T0PR=15000-1;
T0TCR=0x01;
while(T0TC<ms);
T0TCR=0x03;
T0TCR=0x00;
}*/				    			      
void lcd_confi(void)
{
PINSEL0=0;
IODIR1=lcd_d|RS|e|RW;
lcd_com(0x01);
lcd_com(0x02);
lcd_com(0x0c);
lcd_com(0x28);
lcd_com(0x80);
}
void lcd_com(unsigned char c)
{
IOCLR1=lcd_d;
IOSET1=(c&0xf0)<<16;
IOCLR1=RS;
IOCLR1=RW;
IOSET1=e;
delay_ms(2);
IOCLR1=e;

IOCLR1=lcd_d;
IOSET1=(c&0x0f)<<20;
IOCLR1=RS;
IOCLR1=RW;
IOSET1=e;
delay_ms(2);
IOCLR1=e;
}
void lcd_data(unsigned char d)
{
IOCLR1=lcd_d;
IOSET1=(d&0xf0)<<16;
IOSET1=RS;
IOCLR1=RW;
IOSET1=e;
delay_ms(2);
IOCLR1=e;

IOCLR1=lcd_d;
IOSET1=(d&0x0f)<<20;
IOSET1=RS;
IOCLR1=RW;
IOSET1=e;
delay_ms(2);
IOCLR1=e;
}
void lcd_string(unsigned char*s)
{
 while(*s)
 lcd_data(*s++);
}
void CGRAM_RIGHT(unsigned char nBytes)
{
unsigned char i;
lcd_com(0x40);
for(i=0;i<nBytes;i++)
lcd_data(cgram_lut1[i]);
}
void CGRAM_LEFT(unsigned char nBytes)
{
unsigned char i;
lcd_com(0x40);
for(i=0;i<nBytes;i++)
lcd_data(cgram_lut2[i]);
}

