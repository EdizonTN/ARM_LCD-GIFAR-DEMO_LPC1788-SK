/*
 * TS_Analog.c
 *
 *  Created on: 11.8.2014
 *      Author: peterj
 */

#include "global.h"
#include "../Chip/Drivers/Include/lpc177x_8x_timer.h"
#include "../Chip/Drivers/Include/lpc177x_8x_gpio.h"
#include "../Chip/Drivers/Include/lpc177x_8x_pinsel.h"
#include "../Chip/Drivers/Include/lpc177x_8x_adc.h"


// ******************************************************************************************************
// Function prototypes
// ******************************************************************************************************
// extern unsigned long install_irq(unsigned long IntNumber, void *HandlerAddr, unsigned long Priority);
void Init_TS(void);
unsigned int touch_detect(void);

// ******************************************************************************************************
// Globals
// ******************************************************************************************************
TS_Init_Type TSC_Config;
static int16_t ConvertCoord(int16_t Coord, int16_t MinVal, int16_t MaxVal, int16_t TrueSize);
uint8_t TS_Activated_Flag;
unsigned int x_values[num_samples]; 				// array to store x_samples
unsigned int y_values[num_samples]; 				// array to store y_samples
static short TS_x_value = 1;
static short TS_y_value = -1;
// --------------------------------------------------


unsigned int touch_detect(void)
{
	unsigned int test;
	//config_pins_touch(); 							// configure pins fo r touch detection
	//PINSEL0 &= ~(X_2_mask); 						// X- is digital I/O
	//PINMODE0 &= ~(X_2_mask); 						// clear the two bits &
	//PINMODE0 |= X_2_no_pull; 						// no pullup on X-
	//IODIR0 &= ~(X_2); 							// X- is an input
	PINSEL_SetPinMode(TS_X2_PORT,TS_X2_PIN, PINSEL_BASICMODE_PLAINOUT);
	PINSEL_ConfigPin(TS_X2_PORT,TS_X2_PIN,IOCON_DIGITIAL_MODE);
	FIO_SetDir(TS_X2_PORT, 1 << TS_X2_PIN, GPIO_DIRECTION_INPUT);


	//PINSEL0 &= ~(Y_2_mask); 						// Y- is digital I/O
	//PINMODE0 &= ~(Y_2_mask);
	//PINMODE0 |= Y_2_no_pull;						// no pullup on Y-
	//IODIR0 |= Y_2; 								// Y- is an output
	//IOCLR0 |= Y_2; 								// make Y- low
	PINSEL_SetPinMode(TS_Y2_PORT,TS_Y2_PIN, PINSEL_BASICMODE_PLAINOUT);
	PINSEL_ConfigPin(TS_Y2_PORT,TS_Y2_PIN,IOCON_DIGITIAL_MODE);
	FIO_SetDir(TS_Y2_PORT, 1 << TS_Y2_PIN, GPIO_DIRECTION_OUTPUT);
	FIO_ClearValue(TS_Y2_PORT, 1 << TS_Y2_PIN);


	//PINSEL1 &= ~(Y_1_mask); 						// Y+ is digital I/O
	//PINMODE1 &= ~(Y_1_mask); 						// clear the two bits &
	//PINMODE1 |= Y_1_no_pull; 						// no pullup on Y+
	//IODIR0 &= ~(Y_1); 							// Y+ is an input
	PINSEL_SetPinMode(TS_Y1_PORT,TS_Y1_PIN, PINSEL_BASICMODE_PLAINOUT);
	PINSEL_ConfigPin(TS_Y1_PORT,TS_Y1_PIN,IOCON_DIGITIAL_MODE);
	FIO_SetDir(TS_Y1_PORT, 1 << TS_Y1_PIN, GPIO_DIRECTION_INPUT);


	//PINSEL1 &= ~(X_1_mask); 						// X+ is digital I/O
	//PINMODE1 &= ~(X_1_mask); 						// pullup on X
	//IODIR0 &= ~(X_1); 							// X+ is an input
	PINSEL_SetPinMode(TS_X1_PORT,TS_X1_PIN, PINSEL_BASICMODE_PULLUP);
	PINSEL_ConfigPin(TS_X1_PORT,TS_X1_PIN,IOCON_DIGITIAL_MODE);
	FIO_SetDir(TS_X1_PORT, 1 << TS_X1_PIN, GPIO_DIRECTION_INPUT);


	TIM_Waitms(TS_SETTLING_TIME);					// settling time for switching
	//return((IOPIN0 & X_plus)^X_plus); 			// return true if touch is detected
	//test = FIO_ReadValue;
	test = ((FIO_ReadValue( TS_X1_PORT) & (1 << TS_X1_PIN)) ^ (1 << TS_X1_PIN));
	return (test); 					// return true if touch is detected 24-bit
}


// --------------------------------------------------
// Detected IRQ event from X-Plus line, Falling edge.
void GPIO_IRQHandler(void) //_ _ irq
{
	if(GPIO_GetIntStatus(TS_X1_PORT, TS_X1_PIN, 1))
	{
		GPIO_ClearInt(TS_X1_PORT, 1 << TS_X1_PIN);
		TIM_Waitms (debounce); 								// debounce the touch
		if(touch_detect())
		{
			TS_Activated_Flag = SET;

		}
		NVIC_EnableIRQ(GPIO_IRQn);
	}
}



void Init_TS(void)
{
	TIM_TIMERCFG_Type TIM_ConfigStruct;

	TSC_Config.ad_left = TOUCH_AD_LEFT;
	TSC_Config.ad_right = TOUCH_AD_RIGHT;
	TSC_Config.ad_top = TOUCH_AD_TOP;
	TSC_Config.ad_bottom = TOUCH_AD_BOTTOM;
	TSC_Config.lcd_h_size = LCD_H_SIZE;
	TSC_Config.lcd_v_size = LCD_V_SIZE;
	TSC_Config.Priority = 5;
	TSC_Config.swap_xy = 0;

	// init timer
    TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
    TIM_ConfigStruct.PrescaleValue  = 1;
    // Set configuration for Tim_config and Tim_MatchConfig
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);

	GPIO_Init();
	GPIO_IntCmd(TS_X1_PORT, 1 << TS_X1_PIN, 1);	// - 1: Falling edge
	ADC_Init(LPC_ADC, 400000);

	NVIC_SetPriority(GPIO_IRQn, TSC_Config.Priority);
	NVIC_EnableIRQ(GPIO_IRQn);
	touch_detect();
	// ADC_IntConfig (LPC_ADC_TypeDef *ADCx, ADC_TYPE_INT_OPT IntType, FunctionalState NewState);
}

void DeInit_TS(void)
{
	//GPIO_Deinit();
	// TIM_DeInit(LPC_TIM0);
}





// --------------------------------------------------
//
static int16_t ConvertCoord(int16_t Coord, int16_t MinVal, int16_t MaxVal, int16_t TrueSize)
{
	int16_t tmp;
	int16_t ret;
	uint8_t convert = 0;

	if (MinVal > MaxVal)        // Swap value
	{
		tmp = MaxVal;
		MaxVal = MinVal;
		MinVal = tmp;
		convert = 1;
	}

	ret = (Coord - MinVal) * TrueSize / (MaxVal - MinVal);
	if (convert)
		ret = TrueSize - ret;

	return ret;
}

// --------------------------------------------------
//
void TS_Read(void)
{
	uint32_t i;

	//read_ch_x(); 									// read and collect the x values
	//config_pins_x(); 								// configure pins for read x-dir
	//PINSEL0 &= ~(X_2_mask); 						// X- is digital I/O
	//PINMODE0 &= ~(X_2_mask);
	//PINMODE0 |= X_2_no_pull; 						// no pullup on X-
	//IODIR0 |= X_2; 								// X- is an output
	//IOCLR0 |= X_2; 								// make X- low
	PINSEL_SetPinMode(TS_X2_PORT,TS_X2_PIN, PINSEL_BASICMODE_PLAINOUT);
	PINSEL_ConfigPin(TS_X2_PORT,TS_X2_PIN,IOCON_DIGITIAL_MODE);
	FIO_SetDir(TS_X2_PORT, 1 << TS_X2_PIN, GPIO_DIRECTION_OUTPUT);
	FIO_ClearValue(TS_X2_PORT, 1 << TS_X2_PIN);

	//PINSEL0 &= ~(Y_2_mask); 						// Y- is digital I/O
	//PINMODE0 &= ~(Y_2_mask);
	//PINMODE0 |= Y_2_no_pull; 						// no pullup on Y-
	//IODIR0 &= ~(Y_2); 								// Y- is an input
	PINSEL_SetPinMode(TS_Y2_PORT,TS_Y2_PIN, PINSEL_BASICMODE_PLAINOUT);
	PINSEL_ConfigPin(TS_Y2_PORT,TS_Y2_PIN,IOCON_DIGITIAL_MODE);
	FIO_SetDir(TS_Y2_PORT, 1 << TS_Y2_PIN, GPIO_DIRECTION_INPUT);

	//PINSEL1 &= ~(X_1_mask); 						// X+ is digital I/O
	//PINMODE1 &= ~(X_2_mask);
	//PINMODE1 |= X_1_no_pull; 						// no pullup on X+
	//IODIR0 |= X_1; 								// X+ is an output
	//IOSET0 |= X_1; 								// make X+ high
	PINSEL_SetPinMode(TS_X1_PORT,TS_X1_PIN, PINSEL_BASICMODE_PLAINOUT);
	PINSEL_ConfigPin(TS_X1_PORT,TS_X1_PIN,IOCON_DIGITIAL_MODE);
	FIO_SetDir(TS_X1_PORT, 1 << TS_X1_PIN, GPIO_DIRECTION_OUTPUT);
	FIO_SetValue(TS_X1_PORT, 1 << TS_X1_PIN);


	//PINSEL1 &= ~(Y_1_mask);
	//PINSEL1 |= ADC_on_Y; 							// Y+ is an ADC pin
	PINSEL_ConfigPin(TS_Y1_PORT,TS_Y1_PIN,IOCON_ANALOG_MODE);

	TIM_Waitms(TS_SETTLING_TIME);					// settling time for switching

	//AD0CR = 0x00200304; 							// Power up, PCLK/4, sel AD0.2
	ADC_Init(LPC_ADC, 400000);
	for (i = 0; i < num_samples; i++)
	{
		// AD0CR |= 0x01000000; 						// Start A/D conversion

		ADC_ChannelCmd(LPC_ADC, TS_Y_ADC_CH, ENABLE);
		ADC_StartCmd(LPC_ADC, ADC_START_NOW);;
		// while (AD0DR2 & 0x80000000); 				// wait conversion completed
		while (!(ADC_ChannelGetStatus(LPC_ADC, TS_Y_ADC_CH, ADC_DATA_DONE)));
		ADC_ChannelCmd(LPC_ADC, TS_Y_ADC_CH, DISABLE);
		//x_values[i] = ((AD0DR2 >> 6) & 0x3FF); 		// store result
		x_values[i] = (ADC_ChannelGetData(LPC_ADC,TS_Y_ADC_CH) >>6) & 0x3ff;
	}
	ADC_DeInit(LPC_ADC);

	//read_ch_y();									// read and collect the y values
	//config_pins_y(); 								// configure pins for read y-dir
	//PINSEL0 &= ~(X_2_mask); 						// X- is digital I/O
	//PINMODE0 &= ~(X_2_mask);
	//PINMODE0 |= X_2_no_pull; 						// no pullup on X-
	//IODIR0 &= ~(X_2); 								// X- is an input
	PINSEL_SetPinMode(TS_X2_PORT,TS_X2_PIN, PINSEL_BASICMODE_PLAINOUT);
	PINSEL_ConfigPin(TS_X2_PORT,TS_X2_PIN,IOCON_DIGITIAL_MODE);
	FIO_SetDir(TS_X2_PORT, 1 << TS_X2_PIN, GPIO_DIRECTION_INPUT);

	//PINSEL0 &= ~(Y_2_mask); 						// Y- is digital I/O
	//PINMODE0 &= ~(Y_2_mask);
	//PINMODE0 |= Y_2_no_pull; 						// no pullup on Y-
	//IODIR0 |= Y_2; 									// Y- is an output
	//IOCLR0 |= Y_2; 									// make Y- low
	PINSEL_SetPinMode(TS_Y2_PORT,TS_Y2_PIN, PINSEL_BASICMODE_PLAINOUT);
	PINSEL_ConfigPin(TS_Y2_PORT,TS_Y2_PIN,IOCON_DIGITIAL_MODE);
	FIO_SetDir(TS_Y2_PORT, 1 << TS_Y2_PIN, GPIO_DIRECTION_OUTPUT);
	FIO_ClearValue(TS_Y2_PORT, 1 << TS_Y2_PIN);

	//PINSEL1 &= ~(X_1_mask);
	//PINSEL1 |= ADC_on_X; 							// X+ is an ADC pin
	PINSEL_ConfigPin(TS_X1_PORT,TS_X1_PIN,IOCON_ANALOG_MODE);

	//PINSEL1 &= ~(Y_1_mask); 						// Y+ is digital I/O
	//PINMODE1 &= ~(Y_1_mask); 						// clear the two bits &
	//PINMODE1 |= Y_1_no_pull; 						// no pullup on Y+
	//IODIR0 |= Y_1; 									// Y+ is an output
	//IOSET0 |= Y_1; 									// make Y+ high
	PINSEL_SetPinMode(TS_Y1_PORT,TS_Y1_PIN, PINSEL_BASICMODE_PLAINOUT);
	PINSEL_ConfigPin(TS_Y1_PORT,TS_Y1_PIN,IOCON_DIGITIAL_MODE);
	FIO_SetDir(TS_Y1_PORT, 1 << TS_Y1_PIN, GPIO_DIRECTION_OUTPUT);
	FIO_SetValue(TS_Y1_PORT, 1 << TS_Y1_PIN);

	TIM_Waitms(TS_SETTLING_TIME); 					// settling time for switching

	//AD0CR = 0x00200302; 							// Power up, PCLK/4, sel AD0.1
	ADC_Init(LPC_ADC, 400000);
	for (i = 0; i < num_samples; i++)
	{
		// AD0CR |= 0x01000000; 						// Start A/D conversion
		ADC_ChannelCmd(LPC_ADC, TS_X_ADC_CH, ENABLE);
		ADC_StartCmd(LPC_ADC, ADC_START_NOW);;
		// while (AD0DR1 & 0x80000000); 				// wait until completed
		while (!(ADC_ChannelGetStatus(LPC_ADC, TS_X_ADC_CH, ADC_DATA_DONE)));
		ADC_ChannelCmd(LPC_ADC, TS_X_ADC_CH, DISABLE);
		//x_values[i] = ((AD0DR2 >> 6) & 0x3FF); 		// store result
		x_values[i] = (ADC_ChannelGetData(LPC_ADC,TS_X_ADC_CH) >>6) & 0x3ff;
	}
	ADC_DeInit(LPC_ADC);





	TS_x_value = 0;								// initial value
	for (i=0; i < num_samples; i++)
	{
		TS_x_value += x_values[i]; 				// add up the conversion results
	}
	TS_x_value = TS_x_value /num_samples; 			// get average
	TS_y_value = 0;								// initial value
	for (i=0; i < num_samples; i++)
	{
		TS_y_value += y_values[i]; 				// add up conversion results
	}
	TS_y_value = TS_y_value /num_samples; 			// get average

}



// --------------------------------------------------
//
void GetTouchCoord(int16_t *pX, int16_t* pY)
{
	uint16_t i, tmp;
	int16_t coord, x = -1, y = -1, z1 = -1, z2 = -1, z;

	TS_Read();

	if ((TS_x_value >=0) | (TS_y_value >=0 ))
	{
		coord = 0;
		x = TS_x_value;
		y = TS_y_value;

	// Get X-Coordinate
	/*for (i = 0; i < COORD_GET_NUM; i++)
	{
		ReadWriteTSC2046(X_MEASURE, &tmp);
		X_Points[i] = tmp;
	}
	coord = EvalCoord(TS_x_value, TS_X_COORD_MAX,
			TS_DELTA_X_VARIANCE);
	if (coord > 0)
		x = coord;
	else
		return;
    */
	// Get Y-Coordinate
	/*for (i = 0; i < COORD_GET_NUM; i++)
	{
		ReadWriteTSC2046(Y_MEASURE, &tmp);
		Y_Points[i] = tmp;
	}
	coord = EvalCoord(TS_y_value, TS_Y_COORD_MAX,
			TS_DELTA_Y_VARIANCE);
	if (coord > 0)
		y = coord;
	else
		return;
    */
	// Get Z1-Coordinate
	/*for (i = 0; i < COORD_GET_NUM; i++)
	{
		ReadWriteTSC2046(Z1_MEASURE, &tmp);
		Z1_Points[i] = tmp;
	}
	coord = EvalCoord(Z1_Points, COORD_GET_NUM, TSC2046_Z1_COORD_MAX,
			TSC2046_DELTA_Z1_VARIANCE);
	if (coord > 0)
		z1 = coord;
	else
		return;
   */
	/*
	// Get Z2-Coordinate
	for (i = 0; i < COORD_GET_NUM; i++)
	{
		ReadWriteTSC2046(Z2_MEASURE, &tmp);
		Z2_Points[i] = tmp;
	}
	coord = EvalCoord(Z2_Points, COORD_GET_NUM, TSC2046_Z2_COORD_MAX,
			TSC2046_DELTA_Z2_VARIANCE);
	if (coord > 0)
		z2 = coord;
	else
		return;

	z = CalPressureCoef(x, y, z1, z2);
	if ((z < 0) || (z > 11000))
		return;
   */

	// Swap, adjust to truly size of LCD
	if ((x >= 0) && (y >= 0))
	{
		if (TSC_Config.swap_xy)
		{
			*pY = ConvertCoord(x, TSC_Config.ad_top, TSC_Config.ad_bottom, TSC_Config.lcd_v_size);
			*pX = ConvertCoord(y, TSC_Config.ad_left, TSC_Config.ad_right, TSC_Config.lcd_h_size);
		}
		else
		{
			*pX = ConvertCoord(x, TSC_Config.ad_top, TSC_Config.ad_bottom, TSC_Config.lcd_v_size);
			*pY = ConvertCoord(y, TSC_Config.ad_left, TSC_Config.ad_right, TSC_Config.lcd_h_size);
		}
	}
	TS_x_value = TS_x_value = -1;
	}

	// EnableTS();
}


/*
void EnableTS(void)
{
	uint8_t cmd;
	//SSP_DATA_SETUP_Type sspCfg;

/*
#if (TS_CONVERSION_BITS == 8)
	cmd = START_BIT | CHANNEL_SELECT(Y_MEASURE)|CONVERT_MODE_8_BITS|DFR_MODE|PD_ENABLED;
#else
	cmd = START_BIT
			| CHANNEL_SELECT(
					Y_MEASURE)|CONVERT_MODE_12_BITS|DFR_MODE|PD_ENABLED;
#endif
	sspCfg.tx_data = &cmd;
	sspCfg.rx_data = NULL;
	sspCfg.length = 1;
	CS_ON;
	SSP_ReadWrite(TSC2046_SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);
	CS_OFF;
}
*/

/*
void InitTS(TSC2046_Init_Type *pConfig)
{
	PINSEL_ConfigPin(RPOT_ADC_PREPARED_CH_PORT, RPOT_ADC_PREPARED_CH_PIN,
			RPOT_ADC_PREPARED_CH_FUNC_NO);
	PINSEL_SetAnalogPinMode(RPOT_ADC_PREPARED_CH_PORT, RPOT_ADC_PREPARED_CH_PIN,
			ENABLE);

	ADC_Init(LPC_ADC, 400000);
	ADC_IntConfig(LPC_ADC, RPOT_ADC_PREPARED_INTR, DISABLE);
	ADC_ChannelCmd(LPC_ADC, RPOT_ADC_PREPARED_CHANNEL, ENABLE);

	SSP_CFG_Type SSP_ConfigStruct;

	// PIN config SSP
	PINSEL_ConfigPin(0, 15, 2);
	//PINSEL_ConfigPin(0, 16, 2);   // Use another pin for CS
	PINSEL_ConfigPin(0, 17, 2);
	PINSEL_ConfigPin(0, 18, 2);
	PINSEL_ConfigPin(TSC2046_CS_PORT_NUM, TSC2046_CS_PIN_NUM, 0);
	GPIO_SetDir(TSC2046_CS_PORT_NUM, (1 << TSC2046_CS_PIN_NUM), 1);

	// initialize SSP configuration structure to default
	SSP_ConfigStructInit(&SSP_ConfigStruct);

	// set clock rate
	SSP_ConfigStruct.ClockRate = TSC2046_SSP_CLOCK;
	SSP_ConfigStruct.CPHA = SSP_CPHA_SECOND;
	SSP_ConfigStruct.CPOL = SSP_CPOL_LO;

	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(TSC2046_SSP_PORT, &SSP_ConfigStruct);

	// Enable SSP peripheral
	SSP_Cmd(TSC2046_SSP_PORT, ENABLE);

	// Enable Touch Screen Controller
	EnableTS();

	TSC_Config = *pConfig;

}

static void ReadWriteTS(uint8_t channel, uint16_t* data)
{
	uint8_t cmd;
	//volatile uint32_t tmp;
	SSP_DATA_SETUP_Type sspCfg;
	uint8_t rx[2];

	CS_ON;

	// Send command
#if (TSC2046_CONVERSION_BITS == 8)
	cmd = START_BIT | CHANNEL_SELECT(channel)|CONVERT_MODE_8_BITS|DFR_MODE|REF_OFF_ADC_ON;
#else
	cmd = START_BIT
			| CHANNEL_SELECT(
					channel)|CONVERT_MODE_12_BITS|DFR_MODE|REF_OFF_ADC_ON;
#endif
	sspCfg.tx_data = &cmd;
	sspCfg.rx_data = NULL;
	sspCfg.length = 1;
	SSP_ReadWrite(TSC2046_SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);

	//for(tmp = 0x100; tmp;tmp--);

	// Read the response
	sspCfg.tx_data = NULL;
	sspCfg.rx_data = rx;
	sspCfg.length = 2;
	SSP_ReadWrite(TSC2046_SSP_PORT, &sspCfg, SSP_TRANSFER_POLLING);

#if (TSC2046_CONVERSION_BITS == 8)
	*data = (((rx[0]&0x7F) <<8) | (rx[1]>>0)) >> 7;
#else
	*data = (((rx[0] & 0x7F) << 8) | (rx[1] >> 0)) >> 3;
#endif

	CS_OFF;

	//for(tmp = 0x10; tmp;tmp--);
}
*/

/*
// 	***********************************************************************
// 	* @brief       Evaluate the coords received from TSC.
// 	* @param[in]   pPoints    list of coords
// 	* @param[in]   PointNum   the number of entries in above list
// 	* @param[in]   MaxVal     the maximum value of a coord
// 	* @param[in]   MaxDelta   the maximum delta between coords
// 	* @return      -1: Invalid coords, coord in case it is valid.
// 	**********************************************************************
static int16_t EvalCoord(uint16_t pPoints, uint16_t MaxVal,	uint16_t MaxDelta)
{
	uint32_t i = 0;
	int16_t diff = 0, coord = -1;
	uint8_t coord_valid = 0;

/*	for (i = 0; i < PointNum; i++)
	{
		// ignore values are not in range
		if (pPoints[i] >= MaxVal)
		{
			coord = -1;
			coord_valid = 0;
			continue;
		}

		// the first valid coord
		if (coord == -1)
		{
			coord = pPoints[i];
			coord_valid = 0;
			continue;
		}

		// evaluate coord
		diff = pPoints[i] - coord;
		if (diff < 0)
			diff = 0 - diff;
		if (diff < MaxDelta)
		{
			coord = (coord + pPoints[i]) / 2;  // get mean value
			coord_valid = 1;         // at least 2 continuous coords are valid
		}
		else
		{
			coord = pPoints[i];      // new coord
			coord_valid = 0;
		}
	}

	//if (coord_valid)
		return coord;
	//return -1;
}
// 	***********************************************************************
//	* @brief       Calculate the coefficient of pressure
// 	* @param[in]   x           X-Coordinate
// 	* @param[in]   y           Y-Coordinate
// 	* @param[in]   z1          Z1-Coordinate
// 	* @param[in]   z2          Z2-Coordinate
// 	* @return      coefficient.
// 	**********************************************************************
static int16_t CalPressureCoef(int16_t x, int16_t y, int16_t z1, int16_t z2)
{
	int16_t z = -1;

	z = x * (z2 / z1 - 1);

	return z;
}


// 	***********************************************************************
// 	* @brief       Get Touch coordinates.
// 	* @param[out]  pX     X-Coord
// 	* @param[out]  pY     Y-Coord
// 	* @return      None
// 	**********************************************************************
void GetTouchCoord(int16_t *pX, int16_t* pY)
{
	uint16_t i, tmp;
	int16_t coord, x = -1, y = -1, z1 = -1, z2 = -1, z;

	coord = 0;
	// Get X-Coordinate
	for (i = 0; i < COORD_GET_NUM; i++)
	{
		ReadWriteTSC2046(X_MEASURE, &tmp);
		X_Points[i] = tmp;
	}
	coord = EvalCoord(X_Points, COORD_GET_NUM, TSC2046_X_COORD_MAX,
			TSC2046_DELTA_X_VARIANCE);
	if (coord > 0)
		x = coord;
	else
		return;

	// Get Y-Coordinate
	for (i = 0; i < COORD_GET_NUM; i++)
	{
		ReadWriteTSC2046(Y_MEASURE, &tmp);
		Y_Points[i] = tmp;
	}
	coord = EvalCoord(Y_Points, COORD_GET_NUM, TSC2046_Y_COORD_MAX,
			TSC2046_DELTA_Y_VARIANCE);
	if (coord > 0)
		y = coord;
	else
		return;

	// Get Z1-Coordinate
	for (i = 0; i < COORD_GET_NUM; i++)
	{
		ReadWriteTSC2046(Z1_MEASURE, &tmp);
		Z1_Points[i] = tmp;
	}
	coord = EvalCoord(Z1_Points, COORD_GET_NUM, TSC2046_Z1_COORD_MAX,
			TSC2046_DELTA_Z1_VARIANCE);
	if (coord > 0)
		z1 = coord;
	else
		return;

	// Get Z2-Coordinate
	for (i = 0; i < COORD_GET_NUM; i++)
	{
		ReadWriteTSC2046(Z2_MEASURE, &tmp);
		Z2_Points[i] = tmp;
	}
	coord = EvalCoord(Z2_Points, COORD_GET_NUM, TSC2046_Z2_COORD_MAX,
			TSC2046_DELTA_Z2_VARIANCE);
	if (coord > 0)
		z2 = coord;
	else
		return;

	z = CalPressureCoef(x, y, z1, z2);
	if ((z < 0) || (z > 11000))
		return;

	// Swap, adjust to truly size of LCD
	if ((x >= 0) && (y >= 0))
	{
		if (TSC_Config.swap_xy)
		{
			*pY = ConvertCoord(x, TSC_Config.ad_top, TSC_Config.ad_bottom,
					TSC_Config.lcd_v_size);
			*pX = ConvertCoord(y, TSC_Config.ad_left, TSC_Config.ad_right,
					TSC_Config.lcd_h_size);
		}
		else
		{
			*pX = ConvertCoord(x, TSC_Config.ad_top, TSC_Config.ad_bottom,
					TSC_Config.lcd_v_size);
			*pY = ConvertCoord(y, TSC_Config.ad_left, TSC_Config.ad_right,
					TSC_Config.lcd_h_size);
		}
	}
	EnableTS();
}
*/

