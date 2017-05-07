/*
 * ADC_controlador.h
 *
 *  Created on: 9/1/2017
 *      Author: elver
 */

#ifndef SOURCE_CONTROL_ADC_H_
#define SOURCE_CONTROL_ADC_H_

#include "fsl_common.h"

#define ADC_PIN_BASE PORTB
#define ADC_TENSION_ENTRADA 0U
#define ADC_TEMPERATURA 1U

#define BASE_ADC ADC0
#define CANAL_TENSION_ENTRADA 8U
#define CANAL_TEMPERATURA 9U

volatile float tension_entrada = 0;
volatile uint16_t temperatura = 0;

volatile bool lectura_en_curso = false;
volatile bool lectura_temp_completada = false;
volatile bool lectura_tens_completada = false;


void init_control_ADC();

void cfg_control_ADC();

void leer_tension();
void leer_temperatura();

void ADC0_IRQHandler();

#endif /* SOURCE_CONTROL_ADC_H_ */
