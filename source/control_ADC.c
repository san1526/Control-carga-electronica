/*
 * ADC_controlador.c
 *
 *  Created on: 9/1/2017
 *      Author: elver
 */

#include "control_ADC.h"
#include "fsl_adc16.h"
#include "fsl_port.h"
#include "fsl_common.h"



void init_control_ADC(){

	CLOCK_EnableClock(kCLOCK_PortB);
	PORT_SetPinMux(PORTB, ADC_TENSION_ENTRADA, kPORT_PinDisabledOrAnalog);
	PORT_SetPinMux(PORTB, ADC_TEMPERATURA, kPORT_PinDisabledOrAnalog);
}


const adc16_channel_config_t temperatura_cfg={CANAL_TEMPERATURA,true,false};
const adc16_channel_config_t tension_cfg={CANAL_TENSION_ENTRADA,true,false};

void cfg_control_ADC(){

	const adc16_config_t ADC_cfg;

	EnableIRQ(ADC0_IRQn);

	ADC16_GetDefaultConfig(&ADC_cfg);
	ADC16_Init(BASE_ADC, &ADC_cfg);
	ADC16_EnableHardwareTrigger(BASE_ADC, false);

	ADC16_DoAutoCalibration(BASE_ADC);

}

volatile bool tension_flag = false;
volatile bool temperature_flag = false;

void leer_tension(){

	ADC16_SetChannelConfig(BASE_ADC, 0U, &tension_cfg);
	tension_flag = true;
	lectura_en_curso = true;
	lectura_tens_completada = false;

}

void leer_temperatura(){

	ADC16_SetChannelConfig(BASE_ADC, 0U, &temperatura_cfg);
	temperature_flag = true;
	lectura_en_curso = true;
	lectura_temp_completada = false;
}

void ADC0_IRQHandler(){

	if(tension_flag == true){
		tension_entrada = (float)ADC16_GetChannelConversionValue(BASE_ADC, 0U) * 20 / 4095;
		tension_flag = false;
		lectura_en_curso = false;
		lectura_tens_completada = true;
	}
	if(temperature_flag){
		temperatura = ADC16_GetChannelConversionValue(BASE_ADC, 0U);
		temperature_flag = false;
		lectura_en_curso = false;
		lectura_temp_completada = true;
	}

}
