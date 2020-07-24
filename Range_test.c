#include <stdint.h>
#include "msp432p401r.h"
#include "ClockSystem.h"
#include "Port_Functions.h"

void Init_Port5(){
	P5SEL0 |= 0x00;
	P5SEL1 |= 0x00;
	P5REN |= BIT0;
	P5DIR |= 0x00;
}

void Init_Port6(){
	P6SEL0 |= 0x00;
	P6SEL1 |= 0x00;
	P6REN |= BIT5;
	P6DIR |= BIT5;

	P6OUT |= BIT5; //init high bc MOSFET inverts signal

}

/*void SysTick_Init(){
	SYSTICK_STCSR = 0; // 1) disable SysTick during setup
	SYSTICK_STRVR = 0x00ffffff; // 2) delay by correct number of clock cycles
	SYSTICK_STCVR = 0; // 3) any write to current clears it
	SYSTICK_STCSR = 0x00000005; // 4) enable SysTick with core clock
}*/

//delay for a specified number of seconds
void Delay_10us(uint8_t time){
	int cycles = time * 3; //3MHz clock
	SYSTICK_STRVR = cycles - 1;
	SYSTICK_STCVR = 0;
	while((SYSTICK_STCSR & 0x00010000) == 0){
		//wait for count flag to be asserted
	}
}

int GetPulseLen(){
	int cycles = 6 * 3000000 / 100; //3MHz clock
	SYSTICK_STRVR = cycles - 1;
	SYSTICK_STCVR = 0;
	while(((SYSTICK_STCSR & 0x00010000) == 0) && ((P5IN & BIT0) == 0x00)){
		//wait for count flag to be asserted
	}
	int count = 0;
	if(P5IN&BIT0 == BIT0){

		//double range = (count/148.0)/2; //# of us*velocity in m/s divided by 2 divided by 1000000 us/s
		int waitTime = 6*2*148;
		cycles = waitTime*3;
		SYSTICK_STRVR = cycles - 1;
		SYSTICK_STCVR = 0;
		while(((SYSTICK_STCSR & 0x00010000) == 0)){
			//wait for count flag to be asserted
		}
		uint8_t bitval = P5IN & BIT0;
		if((P5IN & BIT0) == 0x00){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}

uint8_t Check_RF(){
	P6OUT &= ~(BIT5);
	Delay_10us(1);
	P6OUT |= BIT5;

	int len = GetPulseLen();
	if(len == 1){
		return 0x01;
	}
	else{
		return 0x00;
	}
}

void main(){
	Init_Port5();
	Init_Port6();
	SysTick_Init();

	int testVal = Check_RF();

	Delay(800);
}
