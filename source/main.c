/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include <stdio.h>

//#include "fsl_debug_console.h"		//SOLO PARA DEBUG

#include "control_ventilador.h"
#include "control_corriente.h"
#include "control_ADC.h"
#include "control_com.h"
#include "timer.h"

int main(void)
{
	bool modo_potencia_constante = false, modo_impedancia_constante = false, leer_tension_NOT_temperatura = true;
	float potencia = 0, impedancia = 0;
	int rpm = 1;

	init_control_ventilador();
	init_control_com();
	init_control_ADC();

	BOARD_InitPins();
	BOARD_BootClockRUN();
	//BOARD_InitDebugConsole();

	cfg_control_ADC();
	cfg_control_corriente();
	cfg_control_ventilador();
	cfg_control_com();
	timer_init ();


	while(1){

		if(data_in_flag){

			data_in_flag = false;

			switch (comando){

				case 'c':	//modo corriente constante
					corriente_sd = data_in;
					set_corriente(corriente_sd);
					modo_impedancia_constante = false;
					modo_potencia_constante = false;
					break;

				case 'p':	//modo potencia constante
					potencia = data_in;
					modo_potencia_constante = true;
					modo_impedancia_constante = false;
					break;

				case 'z':	//modo impedancia constante
					impedancia = data_in;
					modo_impedancia_constante = true;
					modo_potencia_constante = false;
					break;

				case 'o':
					corriente_sd = 0;
					set_corriente(0);
					modo_impedancia_constante = false;
					modo_potencia_constante = false;
					break;

				case 's':	//send data
					send_datos(&corriente_sd,&tension_entrada);
					break;


			}

			comando = '\0';

		}


		if(modo_potencia_constante && tension_entrada != 0){
			corriente_sd = potencia / tension_entrada;
			set_corriente(corriente_sd);
		}

		if(modo_impedancia_constante && tension_entrada != 0){
			corriente_sd = tension_entrada / impedancia;
			set_corriente(data_in);
		}

		if(leer_ADC_flag){
			if (!lectura_en_curso && leer_tension_NOT_temperatura){
				leer_tension();
				leer_tension_NOT_temperatura = false;
			}else if(!lectura_en_curso && !leer_tension_NOT_temperatura){
				leer_temperatura();
				leer_tension_NOT_temperatura = true;
				leer_ADC_flag = false;
			}
		}

		if (rpmCountFlag){
			rpmCountFlag = false;
			rpm = get_rpm_ventilador();
		}

		if(lectura_temp_completada){
			set_rpm_ventilador(temperatura);
		}

		/*if(rpm > 10 || temperatura > 3410){
			set_corriente(0);
		}*/
	}
}
