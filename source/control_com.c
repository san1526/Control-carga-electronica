/*
 * control_com.c
 *
 *  Created on: 10/1/2017
 *      Author: elver
 */

#include "fsl_lpsci.h"
#include "control_com.h"
#include "fsl_port.h"
#include "fsl_common.h"

#include <stdio.h>
#include <stdlib.h>

#include "fsl_debug_console.h"

volatile char buff[6] = {'\0'};

void data_parser()
{
	data_in = atof(buff);
    data_in_flag = true;
}

void DATA_IN_HANDLER(void)
{

    if ((kLPSCI_RxDataRegFullFlag)&LPSCI_GetStatusFlags(UART0) && !data_in_flag)
    {
    	LPSCI_ReadBlocking(UART0, &comando, 1);
    	LPSCI_ReadBlocking(UART0, &buff, 5);
    	data_parser();
    }
}


void init_control_com(){

    CLOCK_EnableClock(kCLOCK_PortA);
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

}

void cfg_control_com(){

	lpsci_config_t com_config={
	    	115200U,
			kLPSCI_ParityDisabled,
			kLPSCI_OneStopBit,
			true,
			true
	};


    CLOCK_SetLpsci0Clock(0x1U);

    LPSCI_Init(UART0,&com_config,CLOCK_GetFreq(kCLOCK_CoreSysClk));

    LPSCI_EnableInterrupts(UART0, kLPSCI_RxDataRegFullInterruptEnable);
    EnableIRQ(UART0_IRQn);
}

void send_datos(float *corriente, float *tension){

	char c = 'p',f = 'p';
	float potencia = (*corriente) * (*tension);

	LPSCI_WriteBlocking(UART0, &c,1);
	LPSCI_WriteBlocking(UART0, corriente, sizeof(float));
	LPSCI_WriteBlocking(UART0, tension, sizeof(float));
	LPSCI_WriteBlocking(UART0, &potencia, sizeof(float));
	LPSCI_WriteBlocking(UART0, &f, 1);
}
