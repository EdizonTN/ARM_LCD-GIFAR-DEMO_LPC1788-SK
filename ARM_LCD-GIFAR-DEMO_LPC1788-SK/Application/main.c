/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : 4, August 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR LPC-1788 board. It shows basic use of the I/O,
 * the timer and the interrupt controllers.
 *  It starts by blinking USB Link LED.
 *
 * Jumpers:
 *  EXT/JLINK  - depending of power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 *  BDS_E      - unfilled
 *  C/SC       - SC
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 *    $Revision: 24636 $
 **************************************************************************/
#include "global.h"
#include "lcd_config.h"
#include "lpc_types.h"
#include "system_LPC177x_8x.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_lcd.h"
#include "lpc177x_8x_pwm.h"
#include "lpc177x_8x_adc.h"
#include "bsp.h"

uint8_t Smb380Id, Smb380Ver;

#include "backgnd.c"
#define LOGO_BPP			24
#define BMP_BYTES_PP		3

Bmp_t LogoPic =
{
  320,
  240,
  LOGO_BPP,
  BMP_BYTES_PP,
  NULL,
  (uint8_t *)&image_data_Backgnd,
  ( uint8_t *)"background picture"
};


void DelayMS(uint32_t dly)
{
    volatile uint32_t i = 0;

    for ( ; dly > 0; dly--)
        for (i = 0; i < 16000; i++);
}


/*************************************************************************
 * Function Name: GetBacklightVal
 * Parameters: none
 *
 * Return: none
 *
 * Description: Get backlight value from user
 *
 *************************************************************************/
uint32_t GetBacklightVal (void) {
  int32_t val;
  uint32_t backlight_off, pclk;

  //ADC_StartCmd(LPC_ADC, ADC_START_NOW);

  //while (!(ADC_ChannelGetStatus(LPC_ADC, BRD_ADC_PREPARED_CHANNEL, ADC_DATA_DONE)));

  //val = ADC_ChannelGetData(LPC_ADC, BRD_ADC_PREPARED_CHANNEL);
  val = 1023;//GetValue_RPOT();

  if( val == -1) return(val);
#if ((_CUR_USING_LCD == _RUNNING_LCD_QVGA_TFT)||(_CUR_USING_LCD == _RUNNING_LCD_GFT035A320240Y))
  val = (val >> 7) & 0x3F;
  pclk = CLKPWR_GetCLK(CLKPWR_CLKTYPE_PER);
  backlight_off = pclk/(_BACK_LIGHT_BASE_CLK*20);
  val =  val* (pclk*9/(_BACK_LIGHT_BASE_CLK*20))/0x3F;
#else
  backlight_off = 0;
#endif
  return backlight_off + val;
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: entry
 *
 *************************************************************************/
  void main(void)
{
  volatile uint32_t i;

  uint32_t xs, ys;

  int32_t backlight;
  LCD_Cursor_Config_Type cursor_config;
  LCD_Config_Type lcd_config;
  int draw_cursor_x, draw_cursor_y;

  uint32_t start_pix_x, start_pix_y, pix_ofs;
  

  Init_RPOT();

  /***************/
  /** Initialize LCD */
  /***************/
  LCD_Enable (FALSE);
    
  // SDRAM Init = check right board to avoid linking error
  SDRAMInit();
  
  lcd_config.big_endian_byte = 0;
  lcd_config.big_endian_pixel = 0;
  lcd_config.hConfig.hbp = LCD_H_BACK_PORCH;
  lcd_config.hConfig.hfp = LCD_H_FRONT_PORCH;
  lcd_config.hConfig.hsw = LCD_H_PULSE;
  lcd_config.hConfig.ppl = LCD_H_SIZE;
  lcd_config.vConfig.lpp = LCD_V_SIZE;
  lcd_config.vConfig.vbp = LCD_V_BACK_PORCH;
  lcd_config.vConfig.vfp = LCD_V_FRONT_PORCH;
  lcd_config.vConfig.vsw = LCD_V_PULSE;
  lcd_config.panel_clk   = LCD_PIX_CLK;
  lcd_config.polarity.active_high = 1;
  lcd_config.polarity.cpl = LCD_H_SIZE;
  lcd_config.polarity.invert_hsync = 1;
  lcd_config.polarity.invert_vsync = 1;
  lcd_config.polarity.invert_panel_clock = 1;

  lcd_config.lcd_panel_upper =  LCD_VRAM_BASE_ADDR_UPPER;
  lcd_config.lcd_panel_lower =  LCD_VRAM_BASE_ADDR_LOWER;

  lcd_config.lcd_bpp = LCD_BPP_24;
  lcd_config.lcd_type = LCD_TFT;
  lcd_config.lcd_palette = LogoPic.pPalette;
  lcd_config.lcd_bgr = FALSE;
  
  LCD_Init (&lcd_config);


  TSC_Config.ad_left = TOUCH_AD_LEFT;
  TSC_Config.ad_right = TOUCH_AD_RIGHT;
  TSC_Config.ad_top = TOUCH_AD_TOP;
  TSC_Config.ad_bottom = TOUCH_AD_BOTTOM;
  TSC_Config.lcd_h_size = LCD_H_SIZE;
  TSC_Config.lcd_v_size = LCD_V_SIZE;
  TSC_Config.Priority = 5;
  TSC_Config.swap_xy = 0;
  Init_TS (&TSC_Config);	  // init touch screen with priority 1


  LCD_SetImage(LCD_PANEL_UPPER, NULL);
  LCD_SetImage(LCD_PANEL_LOWER, NULL);

  /***************/
  /* Initialize PWM */
  /***************/
  /*
  PWMCfgDat.PrescaleOption = PWM_TIMER_PRESCALE_TICKVAL;
  PWMCfgDat.PrescaleValue = 1;
  PWM_Init(_PWM_NO_USED, PWM_MODE_TIMER, (void *) &PWMCfgDat);

  PINSEL_ConfigPin (_PWM_PORT_NUM, _PWM_PIN_NUM, _PWM_PIN_FUNC_NUM);
  PWM_ChannelConfig(_PWM_NO_USED, _PWM_CHANNEL_NO, PWM_CHANNEL_SINGLE_EDGE);

  // Set MR0
  pclk = CLKPWR_GetCLK(CLKPWR_CLKTYPE_PER);
  PWM_MatchUpdate(_PWM_NO_USED, 0,pclk/_BACK_LIGHT_BASE_CLK, PWM_MATCH_UPDATE_NOW);
  PWMMatchCfgDat.IntOnMatch = DISABLE;
  PWMMatchCfgDat.MatchChannel = 0;
  PWMMatchCfgDat.ResetOnMatch = ENABLE;
  PWMMatchCfgDat.StopOnMatch = DISABLE;
  PWM_ConfigMatch(_PWM_NO_USED, &PWMMatchCfgDat);
*/


  // Enable LCD
  LCD_Enable (TRUE);
  
  if(LogoPic.H_Size > LCD_H_SIZE)
  {
    start_pix_x = (LogoPic.H_Size - LCD_H_SIZE)/2; 
    xs = 0;
  }
  else
  {
    start_pix_x = 0;
    xs = (LCD_H_SIZE - LogoPic.H_Size)/2;
  }
  
  if(LogoPic.V_Size > LCD_V_SIZE)
  {
     start_pix_y = (LogoPic.V_Size - LCD_V_SIZE)/2;
     ys = 0;
  }
  else 
  {
     ys = (LCD_V_SIZE - LogoPic.V_Size)/2;
     start_pix_y = 0;
  }
  
  pix_ofs = (start_pix_y * LogoPic.H_Size + start_pix_x)*LogoPic.BitsPP/8;
  LogoPic.pPicStream += pix_ofs;
  LCD_LoadPic(LCD_PANEL_UPPER,xs,ys,&LogoPic,0x00);
  
  //DelayMS(2000);


  // Draw color bars
  // lcd_colorbars();
  
  // Draw cursor
  LCD_Cursor_Enable(DISABLE, 0);

  cursor_config.baseaddress = LCD_CURSOR_BASE_ADDR;
  cursor_config.framesync = 1;
#if (CURSOR_SIZE == 64)
  cursor_config.size32 = 0;
#else
  cursor_config.size32 = 1;
#endif
  cursor_config.palette[0].Red = 0x00;
  cursor_config.palette[0].Green = 0x00;
  cursor_config.palette[0].Blue = 0x00;
  cursor_config.palette[1].Red = 0xFF;
  cursor_config.palette[1].Green = 0xFF;
  cursor_config.palette[1].Blue = 0xFF;
  LCD_Cursor_Cfg(&cursor_config);
  LCD_Cursor_SetImage((uint32_t *)Cursor, 0, sizeof(Cursor)/sizeof(uint32_t)) ;

  draw_cursor_x = (LCD_H_SIZE/2) - CURSOR_OFF_X;
  draw_cursor_y = (LCD_V_SIZE/2) - CURSOR_OFF_Y;
  LCD_Move_Cursor(draw_cursor_x, draw_cursor_y);

  LCD_Cursor_Enable(ENABLE, 0);
  Init_LCD_BackLight();

  while(1)
  {
   	//SetBackLight(backlight);

	int16_t tmp_x = -1, tmp_y = -1;
	// update value
	if (backlight++ < 100) Set_LCD_BackLight(backlight);
	else backlight = 0;

	if (TS_Activated_Flag)		// screen pressed ??
	{
		GetTouchCoord((int16_t*)&tmp_x, (int16_t*)&tmp_y);
		TS_Activated_Flag = RESET;
		if((tmp_x >= 0) && (tmp_y >0))		// touch screen pressed? Changed coords?
		{
		  draw_cursor_x = tmp_x - CURSOR_OFF_X;
		  draw_cursor_y = tmp_y - CURSOR_OFF_Y;
		  if((LCD_H_SIZE - CURSOR_OFF_X) < draw_cursor_x) draw_cursor_x = LCD_H_SIZE - CURSOR_OFF_X;
		  if(-(CURSOR_OFF_X) > draw_cursor_x) draw_cursor_x = -(CURSOR_OFF_X);
		  if((LCD_V_SIZE - CURSOR_OFF_Y) < draw_cursor_y) draw_cursor_y = (LCD_V_SIZE - CURSOR_OFF_Y);
		  if(-(CURSOR_OFF_Y) > draw_cursor_y) draw_cursor_y = -(CURSOR_OFF_Y);
		  LCD_Move_Cursor(draw_cursor_x, draw_cursor_y);
		}
	}
  }
}

