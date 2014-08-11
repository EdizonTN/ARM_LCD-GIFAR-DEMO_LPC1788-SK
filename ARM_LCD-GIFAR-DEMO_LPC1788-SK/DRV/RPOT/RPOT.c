/*
 * RPOT.c
 *
 *  Created on: 11.8.2014
 *      Author: peterj
 */

#include "global.h"
#include "../Chip/Drivers/Include/lpc177x_8x_adc.h"

// *********************
//   Initialize ADC

void Initialize_ADC(void)
{
	PINSEL_ConfigPin (RPOT_ADC_PREPARED_CH_PORT, RPOT_ADC_PREPARED_CH_PIN, RPOT_ADC_PREPARED_CH_FUNC_NO);
	PINSEL_SetAnalogPinMode(RPOT_ADC_PREPARED_CH_PORT, RPOT_ADC_PREPARED_CH_PIN,ENABLE);

	ADC_Init(LPC_ADC, 400000);
	ADC_IntConfig(LPC_ADC, RPOT_ADC_PREPARED_INTR, DISABLE);
	ADC_ChannelCmd(LPC_ADC, RPOT_ADC_PREPARED_CHANNEL, ENABLE);
}
