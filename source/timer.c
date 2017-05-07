/*
 * timer.c
 *
 *  Created on: 9/1/2017
 *      Author: elver
 */

#include "timer.h"
#include "fsl_pit.h"
#include "fsl_debug_console.h"

void timer_init (){

	const pit_config_t timer_cfg={true};

	PIT_Init(PIT,&timer_cfg);

	PIT_EnableInterrupts(PIT, RPM_COUNT, kPIT_TimerInterruptEnable);
	PIT_EnableInterrupts(PIT, ADC_TIMER, kPIT_TimerInterruptEnable);

	PIT_SetTimerPeriod(PIT, RPM_COUNT, MSEC_TO_COUNT(1000U, PIT_CLOCK));
	PIT_SetTimerPeriod(PIT, ADC_TIMER, MSEC_TO_COUNT(10U, PIT_CLOCK));

	PIT_StartTimer(PIT, RPM_COUNT);
	PIT_StartTimer(PIT, ADC_TIMER);

	EnableIRQ(PIT_IRQn);

}


void PIT_IRQHandler(){

	if(PIT_GetStatusFlags(PIT, RPM_COUNT)){

		rpmCountFlag=true;
		PIT_ClearStatusFlags(PIT, RPM_COUNT, kPIT_TimerFlag);
	}

	if(PIT_GetStatusFlags(PIT, ADC_TIMER)){

		leer_ADC_flag = true;
		PIT_ClearStatusFlags(PIT, ADC_TIMER, kPIT_TimerFlag);
	}
}
