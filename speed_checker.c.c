#include<reg51.h>
#define lcd P2

sbit IR1=P1^3;
sbit IR2=P1^4;
sbit led=P1^7;

sbit rs=P1^0; //register select
sbit rw=P1^1; //RW
sbit en=P1^2; //enable

void lcd_init();
void cmd(unsigned char);
void dat(unsigned char);
void delay(unsigned int);	//delay in milliseconds
void lcd_string(char *s);
void conv_and_disp(unsigned int a);

void main()
{   
	led=0; //output config
    lcd_init();
    lcd_string("   WELCOME   ");
	cmd(0x01);
	lcd_string("CAR SPEED");
	cmd(0xc0);
	

    while(1)
	{
	unsigned int time=0;
	unsigned int speed=0;

        if(IR1== 1)
		{
		 while(IR2==0)
		 {
          delay(100);
		  time++;
		 }
		 speed=((float)(20/time))*36;	  //considered constant distance 20m and converted to kmph
		 while(IR1==1 || IR2==1);
		 conv_and_disp(speed);
		 if(speed>72)
		 {
		 cmd(0xc0);
		 led=1;
		 lcd_string("OVER SPEED");
		 cmd(0xc0);
		 led=0;

		 }
         else
		 {
		 led=0;
		 cmd(0xc0);
		 lcd_string("NORMAL SPEED");
		 cmd(0xc0);

		 }

		}

	   else if(IR2== 1)
		{
		while(IR1==0)
		 {
          delay(100);
		  time++;
		 }
		 speed=((float)(20/time))*36;	  //considered constant distance 20m and converted to kmph
		 while(IR1==1 || IR2==1);
		 conv_and_disp(speed);
		 if(speed>72)
		 {
		 led=1;
		 cmd(0xc0);
		 lcd_string("OVER SPEED");
		 cmd(0xc0);
		 led=0;

		 }
         else
		 {
		 led=0;
		 cmd(0xc0);
		 lcd_string("NORMAL SPEED");
		 cmd(0xc0);

		 }
		 

    	}
 

	}
}

void lcd_init()
{
    cmd(0x38);
    cmd(0x0e);
    cmd(0x06);
    cmd(0x01);
    cmd(0x80);
}

void cmd(unsigned char a)
{
    lcd=a;
    rs=0;
    rw=0;
    en=1;
    delay(40);
    en=0;
}

void dat(unsigned char b)
{
    lcd=b;
    rs=1;
    rw=0;
    en=1;
    delay(40);
    en=0;
}

void lcd_string(char *s)
{
    while(*s) {
       dat(*s++);
     }
}

void delay(unsigned int j)
{
    unsigned int k;
	for(k=0;k<j;k++)
	{
	TMOD =0X01;
	TH0=0XFC;
	TL0=0X66;
	TR0=1;
	while(!TF0);
	TF0=0;
	TR0=0;

    }
}


void conv_and_disp(unsigned int a)
{

unsigned char c;
int p;
int k=0;
int num[16];

num[0]=0;
while(a>0)
{
  num[k]=a%10;
  a=a/10;
  k++;
}
if(k!=0)
{
k--;
}
for (p=k;p>=0;p--)
{
  c=num[p]+48;
  dat(c);
}
for(p=0;p<16-k;p++)
{
dat(' ');
}
return;
}
