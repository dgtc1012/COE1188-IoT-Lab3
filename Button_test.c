#include <stdint.h>
#include "msp432p401r.h"
#include "ClockSystem.h"

void Walk_Switch_Init(){
	P5SEL0 = 0x00;
	P5SEL1 = 0x00;
	P5REN = 0x01;
	P5DIR = 0x00;
}

uint8_t Walk_Switch_Read(){
	uint8_t p5_val = P5IN;
	uint8_t buttonState = p5_val & BIT0;
	return buttonState;
}

void Initialize_Port2(void)
{
	P2SEL0 = 0x00;
	P2SEL1 = 0x00;
	P2REN = 0x01;
	P2DIR = 0x01;
}

void Red_LED_On()
{
	P2OUT |= BIT0;
}

void Red_LED_Off()
{
	P2OUT &= ~(BIT0);
}

void main(){
	Initialize_Port2();
	Red_LED_Off();
	Walk_Switch_Init();
	while(1){
		uint8_t walk_state = Walk_Switch_Read();
		if(walk_state == 0x01){
			Red_LED_On();
		}
		else{
			Red_LED_Off();
		}
	}
}
