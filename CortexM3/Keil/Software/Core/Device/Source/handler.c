#include "CortexM3.h"
#include "music.h"

void NMIHandler(void) {
    ;
}

void HardFaultHandler(void) {
    ;
}

void MemManageHandler(void) {
    ;
}

void BusFaultHandler(void) {
    ;
}

void UsageFaultHandler(void) {
    ;
}

void SVCHandler(void) {
    ;
}

void DebugMonHandler(void) {
    ;
}

void PendSVHandler(void) {
    ;
}

void SysTickHandler(void) {
    Set_SysTick_CTRL(0);
    SCB->ICSR = SCB->ICSR | (1 << 25);
}

void UARTRXHandler(void) {
    ;
}

void UARTTXHandler(void) {
    ;
}

void UARTOVRHandler(void) {
    ;
}

void TIMER0Handler(void) {	
	TIMER0_INT_SET;  // 写1清中断标志
	
	timerPara.cnt++;
	if ( (timerPara.cnt==25 && starSky_beat[timerPara.index]==1) || (timerPara.cnt==50 && starSky_beat[timerPara.index]==2) )
	{
		timerPara.cnt = 0;
		timerPara.index = (timerPara.index+1)%43;
		
		Timer1->CTRL  = 0;
		Timer1->LOAD  = 25000000/starSky_key[timerPara.index];
		Timer1->VALUE = 0;
		Timer1->CTRL  = 0b1001;
	}
}

void TIMER1Handler(void) {	
	TIMER1_INT_SET;  // 写1清中断标志
	
	BEEP_FLIP;
}
