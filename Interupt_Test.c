#include <stdint.h>
#include "msp432p401r.h"
#include "ClockSystem.h"
#include "Port_Functions.h"

void Init_Port3(){
/*	P3DIR &= ~(0xC0); 	//p3.6 and p3.7 inputs
	P3REN |= 0xC0;		//enable pull-up resistor
	P3IFG &= ~(0xC0);	//Clear interrupt flags
	P3IES &= ~(0x80);	//3.6 interrupt low-to-high transition
	P3IES |= 0x40;		//3.7 interrupt high-to-low transition
	P3IE |= 0xC0;		//Enable interrupts for 3.6 and 3.7
*/
	P3DIR &= ~(BIT6); 	//p3.6 and p3.7 inputs
	P3OUT |= BIT6;
	P3REN |= BIT6;		//enable pull-up resistor
	P3IFG &= ~(BIT6);	//Clear interrupt flags
	P3IES &= ~(BIT6);		//3.7 interrupt high-to-low transition
	P3IE |= BIT6;		//Enable interrupts for 3.6 and 3.7

	__enable_interrupt();
	NVIC_ISER1 = 1 <<((INT_PORT3-16)&31);
}

Init_Port5(){
	P5DIR &= ~(BIT2); 	//p3.6 and p3.7 inputs
	P3OUT |= BIT2;
	P5REN |= BIT2;		//enable pull-up resistor
	P5IFG &= ~(BIT2);	//Clear interrupt flags
	P5IES &= ~(BIT2);	//3.6 interrupt low-to-high transition
	P5IE |= BIT2;		//Enable interrupts for 3.6 and 3.7

	__enable_interrupt();
	NVIC_ISER1 = 1 <<((INT_PORT5-16)&31);
}

void Init_Port2(){
	//Port 2 has the red and green LEDs for the walk light
	P2SEL0 |= 0x00;
	P2SEL1 |= 0x00;
	P2REN |= 0x01;
	P2DIR |= 0x01;

	P2OUT &= ~(BIT0);
}

void main(){
	//Init_Port5();
	SysTick_Init();
	Init_Port3();
	Init_Port2();

	while(1){}
}
/*
void PORT3_IRQHandler(){
	//check low to high transition
	if((P3IFG&BIT7) == BIT7){
		P3IFG &= ~(BIT7);
		//P3IES ^= BIT7;

		P2OUT |= BIT0;
	}
	else if((P3IFG&BIT6) == BIT6){
		P3IFG &= ~(BIT6);
		P2OUT &= ~(BIT0);
		//P2OUT |= BIT0;
	}
}
*/
void PORT5_IRQHandler(){
	//check low to high transition
	if((P5IFG&BIT2) == BIT2){
		P5IFG &= ~(BIT2);
		//P3IES ^= BIT7;

		P2OUT |= BIT0;
	}
}

void PORT3_IRQHandler(){
	//check high to low transition
	if((P3IFG&BIT6) == BIT6){
		P3IFG &= ~(BIT6);
		//P2OUT &= ~(BIT0);
		P2OUT |= BIT0;
	}

	Delay(100);
	P2OUT &= ~(BIT0);
}
