#include <stdint.h>
#include "msp432p401r.h"
#include "ClockSystem.h"

void SysTick_Init( )
{
	SYSTICK_STCSR = 0; // 1) disable SysTick during setup
	SYSTICK_STRVR = 0x00ffffff; // 2) delay by correct number of clock cycles
	SYSTICK_STCVR = 0; // 3) any write to current clears it
	SYSTICK_STCSR = 0x00000005; // 4) enable SysTick with core clock
}

void Delay_1s(){
	SYSTICK_STRVR = 0x00Afffff-1;
	SYSTICK_STCVR = 0;
	while((SYSTICK_STCSR & 0x00010000) == 0){
		//wait for count flag to be asserted
	}
}

void Initialize_Port4(void)
{
	P4SEL0 = 0x00;
	P4SEL1 = 0x00;
	P4REN = 0xE7;
	P4DIR = 0xE7;

	P4OUT &= ~(BIT0 | BIT1 | BIT2 | BIT5 | BIT6 | BIT7);
}

void Red_West(int state)
{
	if(state == 1){
		P4OUT |= BIT2;
	}
	else{
		P4OUT &= ~(BIT2);
	}
}

void Red_South(int state)
{
	if(state == 1){
		P4OUT |= BIT7;
	}
	else{
		P4OUT &= ~(BIT7);
	}
}

void Yellow_West(int state)
{
	if(state == 1){
		P4OUT |= BIT1;
	}
	else{
		P4OUT &= ~(BIT1);
	}
}

void Yellow_South(int state)
{
	if(state == 1){
		P4OUT |= BIT6;
	}
	else{
		P4OUT &= ~(BIT6);
	}
}

void Green_West(int state)
{
	if(state == 1){
		P4OUT |= BIT0;
	}
	else{
		P4OUT &= ~(BIT0);
	}
}

void Green_South(int state)
{
	if(state == 1){
		P4OUT |= BIT5;
	}
	else{
		P4OUT &= ~(BIT5);
	}
}


void main(){
	Initialize_Port4();
	SysTick_Init();

	Green_West(1);
	Delay_1s();

	Green_West(0);
	Green_South(1);
	Delay_1s();

	Green_South(0);
	Yellow_West(1);
	Delay_1s();

	Yellow_West(0);
	Yellow_South(1);
	Delay_1s();

	Yellow_South(0);
	Red_West(1);
	Delay_1s();

	Red_West(0);
	Red_South(1);
	Delay_1s();

	Red_South(0);
}
