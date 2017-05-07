/*
 * control_corriente.c
 *
 *  Created on: 9/1/2017
 *      Author: elver
 */


#include "fsl_dac.h"

#include "control_ADC.h"
#include "control_corriente.h"

void cfg_control_corriente(){

	dac_config_t dacConfigStruct;
	DAC_GetDefaultConfig(&dacConfigStruct);

	DAC_Init(DAC0, &dacConfigStruct);
	DAC_Enable(DAC0, true);
	DAC_SetBufferReadPointer(DAC0, 0U);

}


void set_corriente(float corriente){

	uint16_t dacValue = corriente * 4096 / 5 ;
	DAC_SetBufferValue(DAC0, 0U, dacValue);

}

