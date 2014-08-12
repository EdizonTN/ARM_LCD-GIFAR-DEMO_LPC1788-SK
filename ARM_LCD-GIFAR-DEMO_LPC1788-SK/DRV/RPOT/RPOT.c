/*
 * RPOT.c
 *
 *  Created on: 11.8.2014
 *      Author: peterj
 */

#include "global.h"

// *********************
//   Initialize ADC

void Init_RPOT(void)
{
	PINSEL_ConfigPin (RPOT_ADC_PREPARED_CH_PORT, RPOT_ADC_PREPARED_CH_PIN, RPOT_ADC_PREPARED_CH_FUNC_NO);
	PINSEL_SetAnalogPinMode(RPOT_ADC_PREPARED_CH_PORT, RPOT_ADC_PREPARED_CH_PIN,ENABLE);

	ADC_Init(LPC_ADC, 400000);
	ADC_IntConfig(LPC_ADC, RPOT_ADC_PREPARED_INTR, DISABLE);
	ADC_ChannelCmd(LPC_ADC, RPOT_ADC_PREPARED_CHANNEL, ENABLE);
}


void DeInit_RPOT(void)
{

}

unsigned int GetValue_RPOT(void)
{
	  ADC_StartCmd(LPC_ADC, ADC_START_NOW);
	  while (!(ADC_ChannelGetStatus(LPC_ADC, RPOT_ADC_PREPARED_CHANNEL, ADC_DATA_DONE)));
	  return(ADC_ChannelGetData(LPC_ADC, RPOT_ADC_PREPARED_CHANNEL));
}
