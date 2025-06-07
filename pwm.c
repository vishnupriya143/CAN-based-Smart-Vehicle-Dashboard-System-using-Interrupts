#include<LPC21xx.h>
int value;
void pwm_pin()
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
}
void wiper()
{
	while(1)
	{
	int i;
		for(i=0; i<5; i++)
		{
			value = 650;
			PWMMR3=value;
			PWMLER=0x09;
			delay_ms(1000);
			value = 1000;
			PWMMR3=value;
			PWMLER=0x09;
			delay_ms(1000);
			break;
		}
	}
}
