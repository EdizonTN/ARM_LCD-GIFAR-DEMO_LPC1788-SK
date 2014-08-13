/*
 * TS_Analog.h
 *
 *  Created on: 11.8.2014
 *      Author: peterj
 */

#ifndef TS_ANALOG_H_
#define TS_ANALOG_H_

// ******************************************************************************************************
// Configuration
// ******************************************************************************************************
#define			debounce 				100 // debounce delay#define	TS_SETTLING_TIME 				100 				// settling time delay
#define TS_CONVERSION_BITS     			12

//#define TSC2046_SSP_CLOCK       		(2000000ul)
// TSC2046 control byte definitions
#define START_BIT               		(0x01<<7)
#define CHANNEL_SELECT(cmd)     		((cmd&0x07)<<4)
#define X_MEASURE               		(0x05)      		// X Channel#define Y_MEASURE               		(0x01)      		// Y Channel#define Z1_MEASURE              		(0x03)      		// Z1 Channel#define Z2_MEASURE              		(0x04)      		// Z2 Channel#define SER_MODE                		(0x01<<2) 			// Single-Ended Reference Mode#define DFR_MODE                		(0x00<<2) 			// Differential Reference Mode#define CONVERT_MODE_8_BITS     		(0x01<<3)
#define CONVERT_MODE_12_BITS    		(0x00<<3)
#define PD_ENABLED              		(0x00)      		// Power-Down Between Conversions.#define REF_OFF_ADC_ON          		(0x01)      		// Reference is off and ADC is on.#define REF_ON_ADC_OFF          		(0x02)      		// Reference is on and ADC is off.#define PD_DISABLED             		(0x03)      		// Device is always powered
#define COORD_GET_NUM                 	(10)
#define TOUCH_AD_LEFT    				3686
#define TOUCH_AD_RIGHT   				205
#define TOUCH_AD_TOP     				3842
#define TOUCH_AD_BOTTOM  				267
// ******************************************************************************************************
// Hardware connection
// ******************************************************************************************************
#define TS_X1_PORT						BSP_TS_X1_PORT
#define TS_X1_PIN						BSP_TS_X1_PIN
#define TS_X1_IRQ						PIN_INT0_IRQHandler	// route wakeup event to IRQ
#define TS_X2_PORT						BSP_TS_X2_PORT
#define TS_X2_PIN						BSP_TS_X2_PIN
#define TS_Y1_PORT						BSP_TS_Y1_PORT
#define TS_Y1_PIN						BSP_TS_Y1_PIN
#define TS_Y2_PORT						BSP_TS_Y2_PORT
#define TS_Y2_PIN						BSP_TS_Y2_PIN
#define TS_X_ADC_CH						BSP_TS_X_ADC_CH
#define TS_Y_ADC_CH						BSP_TS_Y_ADC_CH



// ******************************************************************************************************
// Export
// ******************************************************************************************************
// Do NOT modify next lines !!!

typedef struct
{
	int16_t ad_left;                        // left margin
	int16_t ad_right;                       // right margin
	int16_t ad_top;                         // top margin
	int16_t ad_bottom;                      // bottom margin
	int16_t lcd_h_size;                     // lcd horizontal size
	int16_t lcd_v_size;                     // lcd vertical size
	uint8_t swap_xy;                        // 1: swap x-y coords
	uint8_t Priority;						// priority level for TS Interrupt
} TS_Init_Type;

extern TS_Init_Type TSC_Config;
extern uint8_t TS_Activated_Flag;
// Initialize TS on analog sub-system
void Init_TS(TS_Init_Type *pConfig);

// Get current Touch Coordinates
void GetTouchCoord(int16_t *pX, int16_t* pY);


#endif /* TS_ANALOG_H_ */
