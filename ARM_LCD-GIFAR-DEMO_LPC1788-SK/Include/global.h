/***********************************************************
 * global.h
 *
 *  Created on: 25.7.2014
 *      Author: peterj
 ***********************************************************/


#ifndef GLOBAL_H_
#define GLOBAL_H_

// ************ SYSTEM HEADERS ****************************************************
#include "stdint.h"


// ************ APPLICATION CONFIG ************************************************
#include "Configuration.h"





// ************ CHIP HEADERS ******************************************************
//#include "lpc177x_8x_libcfg_default.h"
//#include "../Chip/Include/arm_common_tables.h"
//#include "../Chip/Include/arm_const_structs.h"
//#include "../Chip/Include/arm_math.h"
//#include "../Chip/Include/core_cm0.h"
//#include "../Chip/Include/core_cm0plus.h"
//#include "../Chip/Include/core_cm3.h"
//#include "../Chip/Include/core_cm4.h"
//#include "../Chip/Include/core_cm4_simd.h"
//#include "../Chip/Include/core_cmFunc.h"
//#include "../Chip/Include/core_cmInstr.h"
//#include "../Chip/Include/core_sc000.h"
//#include "../Chip/Include/core_sc300.h"

//#include "../Chip/Drivers/Include/lpc177x_8x_pinsel.h"
//#include "../Chip/Drivers/Include/lpc17xx.h"
//#include "../Chip/Drivers/Include/lpc17xx_8x_libcfg.h" // load library configuration
//#include "../Chip/Drivers/Include/lpc177x_8x_gpio.h"
#include "../Chip/Drivers/Include/lpc177x_8x_clkpwr.h"
#include "../Chip/Drivers/Include/lpc177x_8x_pwm.h"
//#include "../Chip/Drivers/Include/lpc177x_8x_uart.h"

//#include "../Chip/Drivers/Include/lpc177x_8x_systick.h"

// ************ BOARD SUPPORT PACKAGE HEADERS *************************************
#include "../BSP/bsp.h"
 // #pragma GCC system_header

// ************ FRAMEWORK HEADERS *************************************************
//#include "bsp.h"
//#pragma GCC system_header

// SWIM Graphics User Interface Library
//#include "LPC177x_8x.h"
//#include "type.h"
//#include "ex_sdram.h"
//#include "lcd_params.h"
//#include "lcd_driver.h"
//#include "lcd_type.h"
#include "../LIB/GUI-SWIM/Include/lpc_swim_font.h"
#include "../LIB/GUI-SWIM/Include/lpc_swim.h"
#include "../LIB/GUI-SWIM/Include/lpc_rom8x16.h"
#include "../LIB/GUI-SWIM/Include/lpc_winfreesystem14x16.h"
#include "../LIB/GUI-SWIM/Include/lpc_x6x13.h"
//#include "LPC177x_8x.h"
//#include "type.h"
//#include "timer.h"


// IAR example driuvers an application:
#include "../Application/lcd_config.h"
#include "../Application/logo.h"
#include "../Application/cursor.h"



#endif /* GLOBAL_H_ */
