/*************************************************************************
 * main.c
 *
 *  Created on: 29.7.2014
 *      Author: peterj
 ***********************************************************/

#include "global.h"

//example of picture on background
#include "../Chip/Drivers/Include/lpc177x_8x_lcd.h"
#include "backgnd.c"	// load picture data
Bmp_t LogoPic =
  {
    320,
    240,
    24,			// read from comment in backgnd.c !
    3,
    NULL,
    (uint8_t *)&image_data_Backgnd,
    ( uint8_t *)"Logos picture"
  };
  uint32_t start_pix_x, start_pix_y, pix_ofs;
// ---------------------- end of background example

void DelayMS(uint32_t dly)
{
    volatile uint32_t i = 0;

    for ( ; dly > 0; dly--)
        for (i = 0; i < 16000; i++);
}



  void main(void)
{
  int32_t backlight;
  uint32_t xs, ys;
  int draw_cursor_x, draw_cursor_y; //
  
  //Init_RPOT();
  Init_SDRAM();
  Init_LCD();
  Init_LCD_Cursor();
  Init_LCD_BackLight();
  Init_TS();

  Enable_LCD();
  Enable_LCD_Cursor();									  // Draw cursor
  Enable_TS();

  draw_cursor_x = (LCD_H_SIZE/2) - LCD_CURSOR_OFF_X;
  draw_cursor_y = (LCD_V_SIZE/2) - LCD_CURSOR_OFF_Y;
  Set_LCD_Cursor(draw_cursor_x, draw_cursor_y);			// nastav kurzor na stred LCD
  Enable_LCD_Cursor();									// zapni kurzor
  Init_LCD_BackLight();									// inicializuj touch screen
  Set_LCD_BackLight(20);								// set backlight to 20%


  //example of picture on background - Background picture show:
  LCD_SetImage(LCD_PANEL_UPPER, NULL);
  LCD_SetImage(LCD_PANEL_LOWER, NULL);

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
  // ---------------------- end of background example


  while(1)
  {
	int16_t tmp_x = -1, tmp_y = -1;
	if (TS_Activated_Flag)		// screen pressed ??
	{
		GetTouchCoord((int16_t*)&tmp_x, (int16_t*)&tmp_y);
		TS_Activated_Flag = RESET;
		if((tmp_x >= 0) && (tmp_y >0))		// touch screen pressed? Changed coords?
		{
		  draw_cursor_x = tmp_x - LCD_CURSOR_OFF_X;
		  draw_cursor_y = tmp_y - LCD_CURSOR_OFF_Y;
		  if((LCD_H_SIZE - LCD_CURSOR_OFF_X) < draw_cursor_x) draw_cursor_x = LCD_H_SIZE - LCD_CURSOR_OFF_X;
		  if(-(LCD_CURSOR_OFF_X) > draw_cursor_x) draw_cursor_x = -(LCD_CURSOR_OFF_X);
		  if((LCD_V_SIZE - LCD_CURSOR_OFF_Y) < draw_cursor_y) draw_cursor_y = (LCD_V_SIZE - LCD_CURSOR_OFF_Y);
		  if(-(LCD_CURSOR_OFF_Y) > draw_cursor_y) draw_cursor_y = -(LCD_CURSOR_OFF_Y);
		  Set_LCD_Cursor(draw_cursor_x, draw_cursor_y);			// nastav kurzor na poziciu touch screen suradnic
		}
	}
  }
}

