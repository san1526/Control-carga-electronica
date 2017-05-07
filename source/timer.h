/*
 * timer.h
 *
 *  Created on: 9/1/2017
 *      Author: elver
 */

#ifndef SOURCE_TIMER_H_
#define SOURCE_TIMER_H_

#include "fsl_pit.h"

#define RPM_COUNT kPIT_Chnl_0
#define ADC_TIMER kPIT_Chnl_1

#define PIT_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)

volatile bool rpmCountFlag = false;
volatile bool leer_ADC_flag = false;

void timer_init();

void PIT_IRQHandler();

#endif /* SOURCE_TIMER_H_ */
