/*
 * TS_Analog.h
 *
 *  Created on: 11.8.2014
 *      Author: peterj
 */

#ifndef TS_ANALOG_H_
#define TS_ANALOG_H_
// ********************* CONFIGURATION
//ADC input preset on this board - definition you can find in bsp.h
#define TSX1_ADC_PREPARED_CHANNEL        BSP_TSX1_ADC_PREPARED_CHANNEL
#define TSX1_ADC_PREPARED_INTR           BSP_TSX1_ADC_PREPARED_INTR
#define TSX2_ADC_PREPARED_CHANNEL        BSP_TSX2_ADC_PREPARED_CHANNEL
#define TSX2_ADC_PREPARED_INTR           BSP_TSX2_ADC_PREPARED_INTR
#define TSY1_ADC_PREPARED_CHANNEL        BSP_TSY1_ADC_PREPARED_CHANNEL
#define TSY1_ADC_PREPARED_INTR           BSP_TSY1_ADC_PREPARED_INTR
#define TSY2_ADC_PREPARED_CHANNEL        BSP_TSY2_ADC_PREPARED_CHANNEL
#define TSY2_ADC_PREPARED_INTR           BSP_TSY2_ADC_PREPARED_INTR

#define TSX1_ADC_PREPARED_CH_PORT        BSP_TSX1_ADC_PREPARED_CH_PORT
#define TSX1_ADC_PREPARED_CH_PIN         BSP_TSX1_ADC_PREPARED_CH_PIN
#define TSX1_ADC_PREPARED_CH_FUNC_NO     BSP_TSX1_ADC_PREPARED_CH_FUNC_NO

#define TSX2_ADC_PREPARED_CH_PORT        BSP_TSX2_ADC_PREPARED_CH_PORT
#define TSX2_ADC_PREPARED_CH_PIN         BSP_TSX2_ADC_PREPARED_CH_PIN
#define TSX2_ADC_PREPARED_CH_FUNC_NO     BSP_TSX2_ADC_PREPARED_CH_FUNC_NO

#define TSY1_ADC_PREPARED_CH_PORT        BSP_TSY1_ADC_PREPARED_CH_PORT
#define TSY1_ADC_PREPARED_CH_PIN         BSP_TSY1_ADC_PREPARED_CH_PIN
#define TSY1_ADC_PREPARED_CH_FUNC_NO     BSP_TSY1_ADC_PREPARED_CH_FUNC_NO

#define TSY2_ADC_PREPARED_CH_PORT        BSP_TSY2_ADC_PREPARED_CH_PORT
#define TSY2_ADC_PREPARED_CH_PIN         BSP_TSY2_ADC_PREPARED_CH_PIN
#define TSY2_ADC_PREPARED_CH_FUNC_NO     BSP_TSY2_ADC_PREPARED_CH_FUNC_NO


// ***********************************
#define TS_CONVERSION_BITS     12

//#define TSC2046_SSP_CLOCK       (2000000ul)

/* TSC2046 control byte definitions */

#define START_BIT               (0x01<<7)

#define CHANNEL_SELECT(cmd)     ((cmd&0x07)<<4)

#define X_MEASURE               (0x05)      // X Channel

#define Y_MEASURE               (0x01)      // Y Channel

#define Z1_MEASURE              (0x03)      // Z1 Channel

#define Z2_MEASURE              (0x04)      // Z2 Channel

#define SER_MODE                (0x01<<2) // Single-Ended Reference Mode
#define DFR_MODE                (0x00<<2) // Differential Reference Mode

#define CONVERT_MODE_8_BITS     (0x01<<3)
#define CONVERT_MODE_12_BITS    (0x00<<3)

#define PD_ENABLED              (0x00)      // Power-Down Between Conversions.
#define REF_OFF_ADC_ON          (0x01)      // Reference is off and ADC is on.
#define REF_ON_ADC_OFF          (0x02)      // Reference is on and ADC is off.
#define PD_DISABLED             (0x03)      // Device is always powered

typedef struct
{
    int16_t ad_left;                        // left margin
    int16_t ad_right;                       // right margin
    int16_t ad_top;                         // top margin
    int16_t ad_bottom;                      // bottom margin
    int16_t lcd_h_size;                     // lcd horizontal size
    int16_t lcd_v_size;                     // lcd vertical size
    uint8_t swap_xy;                        // 1: swap x-y coords
} TS_Init_Type;

/* Initialize TS on analog sub-system */
void InitTS_Analog(TS_Init_Type* pConfig);
/* Get current Touch Coordinates */
void GetTouchCoord(int16_t *pX, int16_t* pY);
/**
 * @}
 */


#endif /* TS_ANALOG_H_ */
