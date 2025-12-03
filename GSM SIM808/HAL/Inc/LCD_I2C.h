
/********************************************************************************************************
 *  [FILE NAME]   :      <LCD_I2C.h>                                                                    *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Nov 17, 2024>                                                                 *
 *  [Description] :      <Header file for the Characters LCD I2C driver>                                *
 ********************************************************************************************************/


#ifndef LCD_I2C_H_
#define LCD_I2C_H_

/*******************************************************************************
 *                                 Includes                                    *
 *******************************************************************************/

#include "../../MCAL/Inc/I2C.h"

/*******************************************************************************
 *                             Macro Declarations                              *
 *******************************************************************************/

#define _LCD_CLEAR                                  0X01
#define _LCD_RETURN_HOME                            0x02
#define _LCD_ENTRY_MODE_DEC_SHIFT_OFF               0x04
#define _LCD_ENTRY_MODE_DEC_SHIFT_ON                0x05
#define _LCD_ENTRY_MODE_INC_SHIFT_OFF               0x06
#define _LCD_ENTRY_MODE_INC_SHIFT_ON                0x07
#define _LCD_CURSOR_MOVE_SHIFT_LEFT                 0x10
#define _LCD_CURSOR_MOVE_SHIFT_RIGHT                0x14
#define _LCD_DISPLAY_SHIFT_LEFT                     0x18
#define _LCD_DISPLAY_SHIFT_RIGHT                    0x1C
#define _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF    0x0C
#define _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_ON     0x0D
#define _LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_OFF     0x0E
#define _LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_ON      0x0F
#define _LCD_DISPLAY_OFF_CURSOR_OFF                 0x08
#define _LCD_8BIT_MODE_2_LINE                       0x38
#define _LCD_4BIT_MODE_2_LINE                       0x28
#define _LCD_4BIT_MODE_2_LINE_INIT1                 0x33
#define _LCD_8BIT_MODE_2_LINE_INIT1                 0x32

#define _LCD_CGRAM_START                            0x40
#define _LCD_DDRAM_START                            0x80

#define ROW1                                        0x01
#define ROW2                                        0x02
#define ROW3                                        0x03
#define ROW4                                        0x04

#define LCD_I2C_BACKLIGHT                           0x08         /* LCD backlight control */
#define LCD_I2C_ENABLE                              0x04         /* LCD enable bit */
#define LCD_I2C_RW                                  0x02         /* LCD read/write control bit */
#define LCD_I2C_RS                                  0x01         /* LCD register select bit */


/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/

typedef enum
{
	LCD_I2C_4Bit,
	LCD_I2C_8Bit
}LCD_I2C_Mode_t;

typedef struct
{
	I2C_Config_t     LCD_I2C_Config;
	LCD_I2C_Mode_t   LCD_I2C_Mode;
}LCD_I2C_t;


/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

Std_ReturnType LCD_I2C_Init(LCD_I2C_t *LCD);
Std_ReturnType LCD_I2C_WriteCommand(const LCD_I2C_t *LCD, uint8 Command);
Std_ReturnType LCD_I2C_WriteChar(const LCD_I2C_t *LCD, uint8 Data);
Std_ReturnType LCD_I2C_WriteCharInPos(const LCD_I2C_t *LCD,uint8 Row, uint8 Column, uint8 Data);
Std_ReturnType LCD_I2C_WriteString(const LCD_I2C_t *LCD, const uint8 *Str);
Std_ReturnType LCD_I2C_WriteStringInPos(const LCD_I2C_t *LCD, uint8 Row, uint8 Column, const uint8 *Str);
Std_ReturnType LCD_I2C_WriteCustomChar(const LCD_I2C_t *LCD, uint8 Row, uint8 Column, const uint8 ArrChar[], uint8 MemPos);
Std_ReturnType LCD_I2C_WriteInteger(const LCD_I2C_t *LCD, sint32 Num);
Std_ReturnType LCD_I2C_WriteIntegerInPos(const LCD_I2C_t *LCD, uint8 Row, uint8 Column, sint32 Num);
Std_ReturnType LCD_I2C_SetCursor(const LCD_I2C_t *LCD, uint8 Row, uint8 Column);
Std_ReturnType LCD_I2C_Clear(const LCD_I2C_t *LCD);
#endif /* LCD_I2C_H_ */