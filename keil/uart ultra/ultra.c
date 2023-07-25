/*
Interfacing HC-SR04 Ultrasonic Distance sensor with LPC2148 - Example Source Code for KEIL ARM.
www.tmisystems.in
*/

#include <lpc214x.h>
#include <stdio.h> 
#include "lib_funcs.h"

#define TRIG (1<<5) //P0.5
#define ECHO (1<<4) //P0.4
 //void clock_wise(void);
void intTimer (void);
//void dcmot(void);
 // unsigned long int var1,var2;
 //unsigned int i=0,j=0,k=0,l=0;
 int distance=0;

int main(void)
{
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
  
	printf("distance \n");
	delayMS(2000);//Wait for GPIO to be stable
	

	while(1)
	{
		//Output 10us HIGH on TRIG pin
		IO0SET |= TRIG;
		delayUS(10);
		IO0CLR |= TRIG;

		while(!(IO0PIN & ECHO)); //Wait for a HIGH on ECHO pin
		startTimer0(); //Start counting
		while(IO0PIN & ECHO); //Wait for a LOW on ECHO pin
		echoTime = stopTimer0(); //Stop counting and save value(us) in echoTime

		distance = (0.0343 * echoTime)/2; //Find the distance

		if(distance < 30)
		{
		printf("ALERT\n");
			delayMS(1000);
		}
		else
		{
			printf("Safe zone\n");
			delayMS(3000);
		}	
		 //wait 1 second for next update
 
 }}
 void intTimer(void){
	T0PR=11999;
	T0MR0=50000;
	T0MCR=3;
	T0TCR=2;
}

 