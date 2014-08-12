/***********************************************************
 * bsp.h
 *
 *  Created on: 29.7.2014
 *      Author: peterj
 ***********************************************************/


#ifndef BSP_H_
#define BSP_H_

#include "global.h"


#ifndef 	__BUILD_WITH_EXAMPLE__
	#define __BUILD_WITH_EXAMPLE__					1
#endif



// ----------------------------------------------- prevzate z BSP IAR examplu --------------------
//List the board that have been supported
/** Default board */
#define _DEFAULT_BOARD              (0)
/** LPC1788 OEM board connects with QVGA base board v1.2 */
#define _QVGA_BOARD                 (1)
/** LPC1788 OEM Board rev A and OEM Base Board rev A */
#define _EA_PA_BOARD                (2)
/** LPC1788 IAR Start Kit Rev.B */
#define _IAR_OLIMEX_BOARD           (3)

/** Current using board definition */
//#define _CURR_USING_BRD             _EA_PA_BOARD


//List the NandFlash that have been supported
#define _RUNNING_NANDFLASH_NONE         (0)
#define _RUNNING_NANDFLASH_K9F1G08U0C   (1)
#define _RUNNING_NANDFLASH_K9F1G08U0A   (2)

// List the LCD that have been supported
#define _RUNNING_LCD_NONE               (0)
#define _RUNNING_LCD_GFT035A320240Y     (1)
#define _RUNNING_LCD_QVGA_TFT           (2)
#define _RUNNING_LCD_EA_REV_PB1         (4)

#if (_CURR_USING_BRD == _QVGA_BOARD)
//Driver for PHY of LAN DP83848C IC
#include "phylan_dp83848c.h"


//ADC input preset on this board
#define BRD_ADC_PREPARED_CHANNEL        (ADC_CHANNEL_2)
#define BRD_ADC_PREPARED_INTR           (ADC_ADINTEN2)

#define BRD_ADC_PREPARED_CH_PORT        (0)
#define BRD_ADC_PREPARED_CH_PIN         (25)
#define BRD_ADC_PREPARED_CH_FUNC_NO     (1)


//LED indicators preset
#define BRD_LED_1_CONNECTED_PORT        (1)
#define BRD_LED_1_CONNECTED_PIN         (13)
#define BRD_LED_1_CONNECTED_MASK        (1 << BRD_LED_1_CONNECTED_PIN)

#define BRD_LED_2_CONNECTED_PORT        (0)
#define BRD_LED_2_CONNECTED_PIN         (13)
#define BRD_LED_2_CONNECTED_MASK        (1 << BRD_LED_2_CONNECTED_PIN)


//PIO interrupt preset
#define BRD_PIO_USED_INTR_PORT          (0)
#define BRD_PIO_USED_INTR_PIN           (25)
#define BRD_PIO_USED_INTR_MASK          (1 << BRD_PIO_USED_INTR_PIN)


//MCI power active levell
#define BRD_MCI_POWERED_ACTIVE_LEVEL    (0)


//Timer preset
#define BRD_TIMER_USED              (LPC_TIM0)
#define BRD_TIM_INTR_USED           (TIMER0_IRQn)

#define BRD_TIM_CAP_LINKED_PORT     (1)
#define BRD_TIM_CAP_LINKED_PIN      (26)


// NandFlash preset
#define _CUR_USING_NANDFLASH            (_RUNNING_NANDFLASH_K9F1G08U0A)

// LCD
#define _CUR_USING_LCD                  (_RUNNING_LCD_G240320LTSW)
#define LCD_CS_PORT_NUM                 (0)
#define LCD_CS_PIN_NUM                  (16)
#define LCD_DC_PORT_NUM                 (0)
#define LCD_DC_PIN_NUM                  (19)
#endif

#if (_CURR_USING_BRD == _EA_PA_BOARD)
//Driver for PHY of LAN LAN8720 IC
#include "phylan_lan8720.h"


//ADC input preset on this board
#define BRD_ADC_PREPARED_CHANNEL        (ADC_CHANNEL_2)
#define BRD_ADC_PREPARED_INTR           (ADC_ADINTEN2)

#define BRD_ADC_PREPARED_CH_PORT        (0)
#define BRD_ADC_PREPARED_CH_PIN         (25)
#define BRD_ADC_PREPARED_CH_FUNC_NO     (1)


//LED indicators preset
#define BRD_LED_1_CONNECTED_PORT        (1)
#define BRD_LED_1_CONNECTED_PIN         (18)
#define BRD_LED_1_CONNECTED_MASK        (1 << BRD_LED_1_CONNECTED_PIN)

#define BRD_LED_2_CONNECTED_PORT        (0)
#define BRD_LED_2_CONNECTED_PIN         (13)
#define BRD_LED_2_CONNECTED_MASK        (1 << BRD_LED_2_CONNECTED_PIN)

//PIO interrupt preset
#define BRD_PIO_USED_INTR_PORT          (0)
#define BRD_PIO_USED_INTR_PIN           (25)
#define BRD_PIO_USED_INTR_MASK          (1 << BRD_PIO_USED_INTR_PIN)


//MCI power active levell
#define BRD_MCI_POWERED_ACTIVE_LEVEL    (0)


//Timer preset
#define BRD_TIMER_USED              (LPC_TIM0)
#define BRD_TIM_INTR_USED           (TIMER0_IRQn)

#define BRD_TIM_CAP_LINKED_PORT     (1)
#define BRD_TIM_CAP_LINKED_PIN      (26)


// NandFlash preset
#define _CUR_USING_NANDFLASH            (_RUNNING_NANDFLASH_K9F1G08U0C)

// LCD
//#define _CUR_USING_LCD                  (_RUNNING_LCD_QVGA_TFT)
#define _CUR_USING_LCD                  (_RUNNING_LCD_EA_REV_PB1)
#define LCD_CS_PORT_NUM                 (0)
#define LCD_CS_PIN_NUM                  (20)
#define LCD_DC_PORT_NUM                 (0)
#define LCD_DC_PIN_NUM                  (19)

#endif

#if (_CURR_USING_BRD == _IAR_OLIMEX_BOARD)
//Driver for PHY of LAN KS8721B IC
/*#include "phylan_ks8721b.h"
//ADC input preset on this board
#define BRD_ADC_PREPARED_CHANNEL        (ADC_CHANNEL_7)
#define BRD_ADC_PREPARED_INTR           (ADC_ADINTEN7)

#define BRD_ADC_PREPARED_CH_PORT        (0)
#define BRD_ADC_PREPARED_CH_PIN         (13)
#define BRD_ADC_PREPARED_CH_FUNC_NO     (3)
*/

//LED indicators preset
#define BRD_LED_1_CONNECTED_PORT        (1)
#define BRD_LED_1_CONNECTED_PIN         (13)
#define BRD_LED_1_CONNECTED_MASK        (1 << BRD_LED_1_CONNECTED_PIN)

#define BRD_LED_2_CONNECTED_PORT        (1)
#define BRD_LED_2_CONNECTED_PIN         (18)
#define BRD_LED_2_CONNECTED_MASK        (1 << BRD_LED_2_CONNECTED_PIN)

//PIO interrupt preset
#define BRD_PIO_USED_INTR_PORT          (0)
#define BRD_PIO_USED_INTR_PIN           (13)
#define BRD_PIO_USED_INTR_MASK          (1 << BRD_PIO_USED_INTR_PIN)


//MCI power active levell
#define BRD_MCI_POWERED_ACTIVE_LEVEL    (1)


//Timer preset
#define BRD_TIMER_USED              (LPC_TIM2)
#define BRD_TIM_INTR_USED           (TIMER2_IRQn)

#define BRD_TIM_CAP_LINKED_PORT     (0)
#define BRD_TIM_CAP_LINKED_PIN      (4)

// LCD
#define _CUR_USING_LCD                  (_RUNNING_LCD_GFT035A320240Y)
#endif

#ifndef _CUR_USING_NANDFLASH
#define _CUR_USING_NANDFLASH            (_RUNNING_NANDFLASH_NONE)
#endif

#ifndef _CUR_USING_LCD
#define _CUR_USING_LCD                  (_RUNNING_LCD_NONE)
#endif





















// ******************************************************************************************************
// External SDRAM memory:
// ******************************************************************************************************
#include "../DRV/ExRAM/Include/sdram_k4s561632j.h"  			// include a driver

// ******************************************************************************************************
// LCD TFT Display - GiFar GFT035EA320240Y:
// ******************************************************************************************************
#include "../DRV/LCD/Include/GFT035EA320240Y.h"  				// include a driver

// ******************************************************************************************************
// ADC Input - Potentiometer - hardware connection:
// ******************************************************************************************************
#define BSP_RPOT_ADC_PREPARED_CHANNEL        (ADC_CHANNEL_7)
#define BSP_RPOT_ADC_PREPARED_INTR           (ADC_ADINTEN7)
#define BSP_RPOT_ADC_PREPARED_CH_PORT        (0)
#define BSP_RPOT_ADC_PREPARED_CH_PIN         (13)
#define BSP_RPOT_ADC_PREPARED_CH_FUNC_NO     (3)
#include "../DRV/RPOT/Include/RPOT.h"							// include a driver

// ******************************************************************************************************
// Touch screen reader - analog - hardware connection:
// ******************************************************************************************************
#define BSP_TS_X1_PORT        				0
#define BSP_TS_X1_PIN        				24
#define BSP_TS_X2_PORT        				0
#define BSP_TS_X2_PIN        				19
#define BSP_TS_Y1_PORT        				0
#define BSP_TS_Y1_PIN        				23
#define BSP_TS_Y2_PORT        				0
#define BSP_TS_Y2_PIN        				21
#define BSP_TS_X_ADC_CH       				1
#define BSP_TS_Y_ADC_CH       				0

#include "../DRV/TouchScreen/Include/TS_Analog.h"				// include a driver


#endif /* BSP_H_ */
