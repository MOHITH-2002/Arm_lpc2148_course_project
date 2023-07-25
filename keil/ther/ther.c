
#include <lpc214x.h>
#include <stdio.h> 
#include "lib_funcs.h"

#define TRIG (1<<8) //P0.8
#define ECHO (1<<9) //P0.9

//void intTimer (void);

 int distance=0;




#define RS 4
#define EN 5
#define DESIRED_COUNT	1000			// for 1sec
#define PRESCALER	11999

void lcdint(void);
void cmd(unsigned char);
void data(unsigned char);
void string(char *);
void delay(unsigned int);
char val[30];

void InitTimer1(void)
{
		 	
	T1PR=PRESCALER;
	T1MR0=DESIRED_COUNT;	//interrupt every 1 sec for interval = 1000
	T1MCR=3;		//interrupt and reset when counter=match
	T1TCR=2;		 //	reset timer
   
}


void lcdint(void)
{
IODIR0 = 0X0000f030;
delay(20);
cmd(0x02);
cmd(0x28);
cmd(0x0c);
cmd(0x06);
cmd(0x01);
cmd(0x80);
IOCLR0 = 1<<EN;
}
void cmd(unsigned char cm)
{
IOPIN0 = ((IOPIN0 & 0XFFFF00FF) | ((cm & 0XF0)<<8)); //upper nibble
IOSET0 = 1<<EN;
IOCLR0 = 1<<RS;
delay(1);
IOCLR0 = 1<<EN;
delay(1);
IOPIN0 = ((IOPIN0 & 0XFFFF00FF) | ((cm & 0X0F)<<12));
IOSET0 = 1<<EN;
IOCLR0 = 1<<RS;
delay(1);
IOCLR0 = 1<<EN;
delay(1);
}
void data(unsigned char d)
{
IOPIN0 = ((IOPIN0 & 0XFFFF00FF) | ((d & 0XF0)<<8)); //upper nibble
IOSET0 = (1<<EN) | (1<<RS);
delay(1);
IOCLR0 = 1<<EN;
delay(1);
IOPIN0 = ((IOPIN0 & 0XFFFF00FF) | ((d & 0X0F)<<12));
IOSET0 = (1<<EN) | (1<<RS);
//IOCLR0 = 1<<rs;
delay(1);
IOCLR0 = 1<<EN;
delay(1);

}
void string(char *msg)
{
unsigned char i=0;
while(msg[i]!=0)
{
data(msg[i]);
i++;
}
}
void delay(unsigned int x) //1 MILLI SECOND
{
unsigned int i,j;
for(i=0;i<x;i++)
{
for(j=0;j<6000;j++);
}
}











int main(void)
{ 
	
	
	
	
	
	
	char *sitr;
	int i;


	
	PINSEL0 = 0x00FFFFFF;		//P0.12 to P0.15 GPIo
	IO0DIR |= 0x0000F000;		//P0.12 to P0.15 output
	IO0SET= 0X00000100; 			//P0.8 should always high.
	initClocks(); //Set PCLK = CCLK = 60Mhz - used by: UART, Timer and ADC
	initUART0();  //Initialize UART0 for retargeted printf()
	initTimer0(); //Init Timer for delay functions
	int echoTime=0;
	
	IO0DIR |= TRIG;    //Set P0.2(TRIG) as output
	IO0DIR &= ~(ECHO); //Set P0.3(ECHO) as input (explicitly)
	IO0CLR |= TRIG;    //Set P0.2 LOW initially
  
	printf("Safe Zone \n");
	delayMS(2000);//Wait for GPIO to be stable
	
lcdint();
InitTimer1();  					// initialise timer0 - 
T1TCR = 0x01;
	

	while(1)
	{
		//Output 10us HIGH on TRIG pin
		sprintf(val,"TEMP:%d C",i);

string(val);
	i++;


while(!( T1IR==0x01));
T1IR=0x01;
cmd(0x01); //next reading
	
		
		
		
		
		IO0SET |= TRIG;
		delayUS(10);
		IO0CLR |= TRIG;

		while(!(IO0PIN & ECHO)); //Wait for a HIGH on ECHO pin
		startTimer0(); //Start counting
		while(IO0PIN & ECHO); //Wait for a LOW on ECHO pin
		echoTime = stopTimer0(); //Stop counting and save value(us) in echoTime

		 //Find the distance
		distance = (30400 * echoTime) / 10; // Dividing by 2 and then by 10 to get cm directly
//sprintf(sitr, "%d", distance);

        if (distance < 30)
        {
            printf("ALERT\n");
								
            delayMS(1000);
        }
        else
        {
            printf("Safe zone\n");
										//
					
          delayMS(3000);
				}
			
		 //wait 1 second for next update
 
 }
	
	
	
	
	

 
}
 void intTimer(void){
	T0PR=11999;
	T0MR0=50000;
	T0MCR=3;
	T0TCR=2;
}



 