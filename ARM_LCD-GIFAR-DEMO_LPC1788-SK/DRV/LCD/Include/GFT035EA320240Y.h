/*
 * GFT035EA320240Y.h
 *
 *  Created on: 11.8.2014
 *      Author: peterj
 */


#ifndef __GFT035EA320240Y_H
#define __GFT035EA320240Y_H




/* PWM */
#define LCD_BL_PWM_ID    			1
#define LCD_BL_PWM_MR0				0			// Match channel register - cycle		- 100%
#define LCD_BL_PWM_MR1				1			// Match channel register - duty cycle	- xx %

#define LCD_BL_PWM_CH				2
#define LCD_BL_PWM_PORT				2
#define LCD_BL_PWM_PIN				1
//#define _PWM_PIN_FUNC_NUM 1

#define LCD_BL_PWM_BASE				(CLKPWR_GetCLK(CLKPWR_CLKTYPE_PER) / _BACK_LIGHT_BASE_CLK)				// base frequency for PWM backlight













#endif /* _GFT035EA320240Y_H */
