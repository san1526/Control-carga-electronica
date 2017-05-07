/*
 * control_ventilador.c
 *
 *  Created on: 9/1/2017
 *      Author: elver
 */

#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_common.h"

#include "fsl_tpm.h"
#include "control_ventilador.h"

void init_control_ventilador(){

	 CLOCK_EnableClock(kCLOCK_PortA);

	 PORT_SetPinMux(PUERTO_PWM, PIN_PWM, kPORT_MuxAlt3);				//configuración del Pin del PWM


	 const port_pin_config_t config = {.pullSelect = kPORT_PullDown,.mux = kPORT_MuxAsGpio};
	 PORT_SetPinConfig(RPM_COUNTER_BASE, RPM_COUNTER_PIN , &config);		//configuración del pin del contador de RPM

}

void cfg_control_ventilador(){

/*----------------CONFIGURACION DEL PWM	---------------------*/

	CLOCK_SetTpmClock(1U);

	tpm_config_t PWM_cfg;
	tpm_chnl_pwm_signal_param_t tpmParam;
	tpm_pwm_level_select_t pwmLevel = kTPM_HighTrue;

    tpmParam.chnlNumber = PWM_CHANNEL;
    tpmParam.level = pwmLevel;
    tpmParam.dutyCyclePercent = 0U;

    TPM_GetDefaultConfig(&PWM_cfg);
    //PWM_cfg.enableDebugMode=true;

    TPM_Init(PWM_BASE,&PWM_cfg);
    TPM_SetupPwm(PWM_BASE,&tpmParam,1U,kTPM_EdgeAlignedPwm,25000U,PWM_CLOCK);

    TPM_StartTimer(PWM_BASE, kTPM_SystemClock);


/*----------------CONFIGURACION DEL CONTADOR RPM	---------------------*/

	   const gpio_pin_config_t rpm_counter_cfg={
	   	   kGPIO_DigitalInput,
	   	   0
	      };

	   PORT_SetPinInterruptConfig(RPM_COUNTER_BASE,RPM_COUNTER_PIN,kPORT_InterruptFallingEdge);

	   EnableIRQ(RPM_COUNTER_IRQ);

	   GPIO_PinInit(RPM_COUNTER_GBASE,RPM_COUNTER_PIN,&rpm_counter_cfg);



}

void set_rpm_ventilador(uint32_t temp){

	uint8_t dutyCycle= (temp - 0) * (99 - 10) / (4095 - 0) + 10;

	TPM_UpdatePwmDutycycle(PWM_BASE, PWM_CHANNEL, kTPM_EdgeAlignedPwm,dutyCycle);

}

uint32_t get_rpm_ventilador(){

	DisableIRQ(RPM_COUNTER_IRQ);

	uint32_t rpm = rpm_counter * 30;

	rpm_counter = 0;

	EnableIRQ(RPM_COUNTER_IRQ);

	return rpm;

}

void IRQ_CONTAR_RPM(){

	GPIO_ClearPinsInterruptFlags(RPM_COUNTER_GBASE,1U<<RPM_COUNTER_PIN);
	rpm_counter++;
}
