/*
 * control_com.h
 *
 *  Created on: 10/1/2017
 *      Author: elver
 */

#ifndef SOURCE_CONTROL_COM_H_
#define SOURCE_CONTROL_COM_H_

#include "fsl_lpsci.h"

#define DATA_IN_HANDLER UART0_IRQHandler


volatile bool data_in_flag = false;

volatile char comando;

volatile float data_in = 0;


void init_control_com();

void cfg_control_com ();

void DATA_IN_HANDLER(void);

void send_datos(float *corriente, float *tension);

#endif /* SOURCE_CONTROL_COM_H_ */
