/*
 * control_ventilador.h
 *
 *  Created on: 9/1/2017
 *      Author: elver
 */

#ifndef SOURCE_CONTROL_VENTILADOR_H_
#define SOURCE_CONTROL_VENTILADOR_H_

#include "fsl_common.h"
#include "fsl_tpm.h"

#define PUERTO_PWM PORTA		//Si se quiere modificar el puerto o pin del PWM asegurarse de cambiar el CLOCK_ENABLE
#define PIN_PWM 5U				//y el MUX en la funcion init_control_ventilador para que funcione

#define PWM_BASE TPM0
#define PWM_CHANNEL kTPM_Chnl_2
#define PWM_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)


#define RPM_COUNTER_BASE PORTA
#define RPM_COUNTER_GBASE GPIOA
#define RPM_COUNTER_PIN 13U
#define RPM_COUNTER_IRQ PORTA_IRQn
#define IRQ_CONTAR_RPM PORTA_IRQHandler

volatile uint8_t rpm_counter = 0 ;


void IRQ_CONTAR_RPM();

void init_control_ventilador();

void cfg_control_ventilador();

void set_rpm_ventilador(uint32_t temp);

uint32_t get_rpm_ventilador();

#endif /* SOURCE_CONTROL_VENTILADOR_H_ */
