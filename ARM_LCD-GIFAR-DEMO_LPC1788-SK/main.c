/*****************************************************************************
 *   lcd_test.c:  LCD module file for NXP LPC177x_8x Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.26  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
//#include "LPC177x_8x.h"
//#include "type.h"
//#include "ex_sdram.h"
//#include "lcd_params.h"
//#include "lcd_driver.h"
//#include "lpc_swim.h"
//#include "lcd_type.h"
//#include "timer.h"

#include "global.h"

extern void lcd_colorbars(void);
static int32_t dev_lcd;

/***********************************************************************
 *
 * Function: main
 *
 * Purpose: Function entry point from the startup code.
 *
 * Processing:
 *
 * Parameters: 	None
 *
 * Outputs: None
 *
 * Returns: None
 *
 **********************************************************************/
int main(void)
{
  SystemInit();
  /* initialize memory */
  EMCInit();

  /* Power Control for CLCDC */ 
  LPC_SC->PCONP |= 0x00000001;        

  /* Open LCD */
  if ((dev_lcd = lcd_open((int32_t)&LCD_DISPLAY)) == 0x0)
  {
    /* Error opening the device */
    return 0;
  }

  /* Set LCD display pointer */
  lcd_ioctl(dev_lcd, LCD_SET_BUFFER, (int32_t)SDRAM_BASE);

  /* Enable LCD */
  lcd_ioctl(dev_lcd, LCD_ENABLE, 1);

  /* Turn on LCD */
  lcd_ioctl(dev_lcd, LCD_PWR_ON, 1);

  /* initialize timer 0 */
  init_timer(0, SystemCoreClock / (LPC_SC->PCLKSEL&0x1F) / 60);

  /* enable timer 0 */
  enable_timer(0);
  disable_timer(0);
  enable_timer(0);
  	    
  /* Display 3 color bars on the lcd display */
  lcd_colorbars();
  	      
  while(1); 	/* loop forever, the end */
}
