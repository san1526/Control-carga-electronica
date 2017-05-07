/*
 * control_corriente.h
 *
 *  Created on: 9/1/2017
 *      Author: elver
 */

#ifndef SOURCE_CONTROL_CORRIENTE_H_
#define SOURCE_CONTROL_CORRIENTE_H_

#include "fsl_dac.h"

volatile float corriente_sd = 0;

void cfg_control_corriente();

void set_corriente(float corriente);

void set_potencia(float potencia);

void set_impedancia(float potencia);

#endif /* SOURCE_CONTROL_CORRIENTE_H_ */
