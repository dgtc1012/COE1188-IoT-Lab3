#include <stdint.h>
#include "msp432p401r.h"
#include "ClockSystem.h"
#include "Port_Functions.h"

//Set_Output argument states are in the following order:
//Walk_Green, Walk_Red, West_Green, West_Yellow, West_Red, South_Green, South_Yellow, South_Red

typedef void (*DoStateFunctions)();

struct State{
	uint8_t out;  //8 bit output
	uint8_t time; //10 ms increments
	DoStateFunctions func;
	const struct State *Next_State_ptr[8];
};

typedef const struct State StateType;

#define GoWest &FSM[0]
#define WaitWest &FSM[1]
#define StopWest &FSM[2]
#define GoSouth &FSM[3]
#define WaitSouth &FSM[4]
#define StopSouth &FSM[5]
#define GoWalk &FSM[6]
#define WaitWalk_1 &FSM[7]
#define WaitWalk_2 &FSM[8]
#define WaitWalk_3 &FSM[9]
#define WaitWalk_4 &FSM[10]
#define WaitWalk_5 &FSM[11]
#define WaitWalk_6 &FSM[12]
#define WaitWalk_7 &FSM[13]
#define StopWalk &FSM[14]

StateType FSM[15] = {
	{0x61, 200, Reset_West_Bit, {WaitWest, WaitWest, GoWest, WaitWest, WaitWest, WaitWest, WaitWest, WaitWest}},
	{0x51, 100, No_Func, {StopWest, StopWest, StopWest, StopWest, StopWest, StopWest, StopWest, StopWest}},
	{0x49, 50, No_Func,  {GoSouth, GoSouth, GoWest, GoSouth, GoWalk, GoWalk, GoWalk, GoSouth}},
	{0x4C, 200, Reset_South_Bit, {WaitSouth, GoSouth, WaitSouth, WaitSouth, WaitSouth, WaitSouth, WaitSouth, WaitSouth}},
	{0x4A, 100, No_Func, {StopSouth, StopSouth, StopSouth, StopSouth, StopSouth, StopSouth, StopSouth, StopSouth}},
	{0x49, 50, No_Func,  {GoWalk, GoSouth, GoWest, GoWest, GoWalk, GoWalk, GoWalk, GoWalk}},
	{0x89, 200, Reset_Walk_Bit, {WaitWalk_1, WaitWalk_1, WaitWalk_1, WaitWalk_1, GoWalk, WaitWalk_1, WaitWalk_1, WaitWalk_1}},
	{0x09, 50, No_Func,  {WaitWalk_2, WaitWalk_2, WaitWalk_2, WaitWalk_2, WaitWalk_2, WaitWalk_2, WaitWalk_2, WaitWalk_2}},
	{0x49, 50, No_Func,  {WaitWalk_3, WaitWalk_3, WaitWalk_3, WaitWalk_3, WaitWalk_3, WaitWalk_3, WaitWalk_3, WaitWalk_3}},
	{0x09, 50, No_Func,  {WaitWalk_4, WaitWalk_4, WaitWalk_4, WaitWalk_4, WaitWalk_4, WaitWalk_4, WaitWalk_4, WaitWalk_4}},
	{0x49, 50, No_Func,  {WaitWalk_5, WaitWalk_5, WaitWalk_5, WaitWalk_5, WaitWalk_5, WaitWalk_5, WaitWalk_5, WaitWalk_5}},
	{0x09, 50, No_Func,  {WaitWalk_6, WaitWalk_6, WaitWalk_6, WaitWalk_6, WaitWalk_6, WaitWalk_6, WaitWalk_6, WaitWalk_6}},
	{0x49, 50, No_Func,  {WaitWalk_7, WaitWalk_7, WaitWalk_7, WaitWalk_7, WaitWalk_7, WaitWalk_7, WaitWalk_7, WaitWalk_7}},
	{0x09, 50, No_Func,  {StopWalk, StopWalk, StopWalk, StopWalk, StopWalk, StopWalk, StopWalk, StopWalk}},
	{0x49, 50, No_Func,  {GoWest, GoSouth, GoWest, GoSouth, GoWalk, GoSouth, GoWest, GoWest}}
};
void main(){
	int input;
	Initialize_Ports();
	SysTick_Init();

	StateType *Current_State_Ptr;

	Current_State_Ptr = GoWest;
	FSM_Input = 0x00;
	input = FSM_Input;
	while(1){
		//set lights to current state's output value
		Set_Output(Current_State_Ptr->out);

		Current_State_Ptr->func();

		//wait cur state's time value
		Delay(Current_State_Ptr->time);

		//get new input
		FSM_Input |= Read_Sensors();
		input = FSM_Input;
		Current_State_Ptr = Current_State_Ptr->Next_State_ptr[FSM_Input];
	}
}

void PORT3_IRQHandler(){
	//check high to low transition
	if((P3IFG&BIT6) == BIT6){
		P3IFG &= ~(BIT6);
		FSM_Input |= BIT2;
	}
}
