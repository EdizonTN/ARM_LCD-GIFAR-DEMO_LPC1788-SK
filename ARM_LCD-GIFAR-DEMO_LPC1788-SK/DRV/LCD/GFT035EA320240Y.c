/*
 * GFT035EA320240Y.c
 *
 *  Created on: 11.8.2014
 *      Author: peterj
 */

#include "global.h"


/*************************************************************************
 * Function Name: SetBackLight
 * Parameters: level     Backlight value
 *
 * Return: none
 *
 * Description: Set LCD backlight
 *
 *************************************************************************/
void Init_LCD_BackLight(void)		// 0-100%
{
    uint32_t pclk;

	PWM_TIMERCFG_Type PWMCfgDat;
	PWM_MATCHCFG_Type PWMMatchCfgDat;

	PWMCfgDat.PrescaleOption = PWM_TIMER_PRESCALE_TICKVAL;
	PWMCfgDat.PrescaleValue = 1;
	PWM_Init(LCD_BL_PWM_ID, PWM_MODE_TIMER, (void *) &PWMCfgDat);

	PINSEL_ConfigPin(LCD_BL_PWM_PORT, LCD_BL_PWM_PIN, 1);		// funct.no.1 - PWM1[2] — Pulse Width Modulator 1, channel 2 output.

	PWM_ChannelConfig(LCD_BL_PWM_ID, LCD_BL_PWM_CH, PWM_CHANNEL_SINGLE_EDGE);
	PWM_ChannelConfig(LCD_BL_PWM_ID, LCD_BL_PWM_CH, PWM_CHANNEL_SINGLE_EDGE);

	// Set MR0
	pclk = CLKPWR_GetCLK(CLKPWR_CLKTYPE_PER);
	PWM_MatchUpdate(LCD_BL_PWM_ID, 0 , LCD_BL_PWM_BASE, PWM_MATCH_UPDATE_NOW);


	PWMMatchCfgDat.IntOnMatch = ENABLE;					// without interrupt
	PWMMatchCfgDat.MatchChannel = LCD_BL_PWM_MR0;			// Match channel register - cycle		- 100%
	PWMMatchCfgDat.ResetOnMatch = ENABLE;					// auto reload
	PWMMatchCfgDat.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LCD_BL_PWM_ID, &PWMMatchCfgDat);		// store it
	PWM_MatchUpdate(LCD_BL_PWM_ID, LCD_BL_PWM_MR0, LCD_BL_PWM_BASE, PWM_MATCH_UPDATE_NOW);

	PWMMatchCfgDat.IntOnMatch = ENABLE;					// without interrupt
	PWMMatchCfgDat.MatchChannel = LCD_BL_PWM_MR1;			// Match channel register - duty cycle	- xx %
	PWMMatchCfgDat.ResetOnMatch = ENABLE;					// auto reload
	PWMMatchCfgDat.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LCD_BL_PWM_ID, &PWMMatchCfgDat);		// store it
	PWM_MatchUpdate(LCD_BL_PWM_ID, LCD_BL_PWM_MR1, (50 * LCD_BL_PWM_BASE) / 100, PWM_MATCH_UPDATE_NOW); // 50%

	PWM_ChannelCmd(LCD_BL_PWM_ID, LCD_BL_PWM_CH, ENABLE);	// Enable PWM Channel Output
	PWM_ResetCounter(LCD_BL_PWM_ID);						// reset and start counter

	NVIC_EnableIRQ(PWM0_IRQn);
	PWM_Cmd(LCD_BL_PWM_ID, ENABLE);						// start PWM
}

/*************************************************************************
 * Function Name: SetBackLight
 * Parameters: level     Backlight value
 *
 * Return: none
 *
 * Description: Set LCD backlight
 *
 *************************************************************************/
void Set_LCD_BackLight(uint16_t level)		// 0-100%
{
    if (level > 100) level = 100;

	PWM_MatchUpdate(LCD_BL_PWM_ID, LCD_BL_PWM_MR1, (level * LCD_BL_PWM_BASE) / 100, PWM_MATCH_UPDATE_NOW);
}

void PWM0_IRQHandler(void)
{
	NVIC_EnableIRQ(PWM0_IRQn);
}
