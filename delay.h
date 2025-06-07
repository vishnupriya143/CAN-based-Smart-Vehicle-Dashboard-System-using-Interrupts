void delay_s(unsigned int s)
{
T0PR=600000-1;
T0TCR=0x01;
while(T0TC<s);
T0TCR=0x03;
T0TCR=0x00;
}

void delay_ms(unsigned int ms)
{
T0PR=60000-1;
T0TCR=0x01;
while(T0TC<ms);
T0TCR=0x03;
T0TCR=0x00;
}
