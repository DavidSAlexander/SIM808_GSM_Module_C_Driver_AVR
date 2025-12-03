
/********************************************************************************************************
 *  [FILE NAME]   :      <LCD_I2C.c>                                                                    *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Nov 14, 2024>                                                                 *
 *  [Description] :      <Source file for the Characters LCD I2C driver>                                *
 ********************************************************************************************************/


#include "../Inc/LCD_I2C.h"


static Std_ReturnType LCD_I2C_EnableControl(const LCD_I2C_t *LCD, uint8 Data);
static Std_ReturnType LCD_I2C_Send(const LCD_I2C_t *LCD, uint8 Data, uint8 Mode);


/*
 *
 * @param LCD
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_I2C_Init(LCD_I2C_t *LCD)
{
    Std_ReturnType ret = E_OK;
	
    if(NULL == LCD)
    {
        ret = E_NOT_OK;
    }
    else
    {	  
		_delay_ms(50); 
		I2C_Init(&(LCD->LCD_I2C_Config));
		LCD_I2C_WriteCommand(LCD, 0x03);
		_delay_ms(5);
		LCD_I2C_WriteCommand(LCD, 0x03);
		_delay_ms(5);
		LCD_I2C_WriteCommand(LCD, 0x03);
		_delay_ms(5);
		LCD_I2C_WriteCommand(LCD, _LCD_RETURN_HOME);
		_delay_ms(5);
		
		LCD_I2C_WriteCommand(LCD, _LCD_4BIT_MODE_2_LINE);
		LCD_I2C_WriteCommand(LCD, _LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
		LCD_I2C_WriteCommand(LCD, _LCD_ENTRY_MODE_INC_SHIFT_OFF);
		LCD_I2C_WriteCommand(LCD, _LCD_CLEAR);
        _delay_ms(5);
    }
    return ret;
}

Std_ReturnType LCD_I2C_WriteCommand(const LCD_I2C_t *LCD, uint8 Command)
{
	Std_ReturnType ret = E_OK;
	
	if(NULL == LCD)
	{
		ret = E_NOT_OK;
	}
	else
	{
	  LCD_I2C_Send(LCD, Command, 0);
	}
	
	return ret;
}


/**
 *
 * @param LCD Pointer to the LCD configurations
 * @param data The data that we need to send to the LCD
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_I2C_WriteChar(const LCD_I2C_t *LCD, uint8 Data)
{
    Std_ReturnType ret = E_OK;
    if(NULL == LCD)
    {
        ret = E_NOT_OK;
    }
    else
    {
        LCD_I2C_Send(LCD, Data, LCD_I2C_RS);
    }
    return ret;
}

/**
 *
 * @param LCD
 * @param Row
 * @param Column
 * @param Data
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_I2C_WriteCharInPos(const LCD_I2C_t *LCD,uint8 Row, uint8 Column, uint8 Data)
{
    Std_ReturnType ret = E_OK;
    if(NULL == LCD)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = LCD_I2C_SetCursor(LCD, Row, Column);
        ret = LCD_I2C_WriteChar(LCD, Data);
    }
    return ret;
}


/**
 *
 * @param LCD
 * @param Str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_I2C_WriteString(const LCD_I2C_t *LCD, const uint8 *Str)
{
    Std_ReturnType ret = E_OK;
    if(NULL == LCD)
    {
        ret = E_NOT_OK;
    }
    else
    {
        while(*Str)
        {
            ret = LCD_I2C_WriteChar(LCD, *Str++);
        }
    }
    return ret;
}

/**
 *
 * @param LCD
 * @param Row
 * @param Column
 * @param Str
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_I2C_WriteStringInPos(const LCD_I2C_t *LCD, uint8 Row, uint8 Column, const uint8 *Str)
{
    Std_ReturnType ret = E_OK;
    if(NULL == LCD)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = LCD_I2C_SetCursor(LCD, Row, Column);
        while(*Str)
        {
            ret = LCD_I2C_WriteChar(LCD, *Str++);
        }
    }
    return ret;
}

/**
 *
 * @param LCD
 * @param Row
 * @param Column
 * @param ArrChar
 * @param MemPos
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_I2C_WriteCustomChar(const LCD_I2C_t *LCD, uint8 Row, uint8 Column, const uint8 ArrChar[], uint8 MemPos)
{
    Std_ReturnType ret = E_OK;
    uint8 LCD_counter = ZERO_INIT;
    if(NULL == LCD)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ret = LCD_I2C_WriteCommand(LCD, (_LCD_CGRAM_START+(MemPos*8)));
        for(LCD_counter=0; LCD_counter<=7; ++LCD_counter)
        {
            ret = LCD_I2C_WriteChar(LCD, ArrChar[LCD_counter]);
        }
        ret = LCD_I2C_WriteCharInPos(LCD, Row, Column, MemPos);
    }
    return ret;
}

/**
 *
 * @param LCD
 * @param Num
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_I2C_WriteInteger(const LCD_I2C_t *LCD, sint32 Num)
{
    Std_ReturnType ret = E_OK;
    if(NULL == LCD)
    {
        ret = E_NOT_OK;
    }
    else
    {
        sint8 str[16];
        boolean isNegative = FALSE;
        if ( Num < 0 )
        {
            Num = -Num;
            isNegative = TRUE;
        }
        int i = 0;
        do
        {
            str[i++] = '0' + Num % 10;
            Num /= 10;
        }
        while (Num > 0);
        if (isNegative) str[i++] = '-';
        str[i] = '\0';

        // reverse the string
        for (int j = 0; j < i / 2; j++)
        {
            char temp = str[j];
            str[j] = str[i - j - 1];
            str[i - j - 1] = temp;
        }
        LCD_I2C_WriteString(LCD,(uint8*)str); /* Display the string */
    }
    return ret;
}

/**
 *
 * @param LCD
 * @param Row
 * @param Column
 * @param Num
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_I2C_WriteIntegerInPos(const LCD_I2C_t *LCD, uint8 Row, uint8 Column, sint32 Num)
{
	Std_ReturnType ret = E_OK;
	if(NULL == LCD)
	{
		ret = E_NOT_OK;
	}
	else
	{
		sint8 str[16];
		boolean isNegative = FALSE;
		if ( Num < 0 )
		{
			Num = -Num;
			isNegative = TRUE;
		}
		int i = 0;
		do
		{
			str[i++] = '0' + Num % 10;
			Num /= 10;
		}
		while (Num > 0);
		if (isNegative) str[i++] = '-';
		str[i] = '\0';

		// reverse the string
		for (int j = 0; j < i / 2; j++)
		{
			char temp = str[j];
			str[j] = str[i - j - 1];
			str[i - j - 1] = temp;
		}
		LCD_I2C_WriteStringInPos(LCD, Row, Column, (uint8*)str); /* Display the string */
		LCD_I2C_WriteChar(LCD,' ');
	}
	return ret;
}
/**
 *
 * @param LCD
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_I2C_Clear(const LCD_I2C_t *LCD)
{
	Std_ReturnType ret = E_OK;
	if(NULL == LCD)
	{
		ret = E_NOT_OK;
	}
	else
	{
		LCD_I2C_WriteCommand(LCD, _LCD_CLEAR );
		_delay_ms(50);
	}
	return ret;
}

/**
 *
 * @param LCD
 * @param Row
 * @param Coulmn
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */
Std_ReturnType LCD_I2C_SetCursor(const LCD_I2C_t *LCD, uint8 Row, uint8 Coulmn)
{
    Std_ReturnType ret = E_OK;
	if (Coulmn > 0)
	{
		 Coulmn--;
	}
    switch(Row)
    {
    case ROW1 :
        ret = LCD_I2C_WriteCommand(LCD, (0x80 + Coulmn));
        break;
    case ROW2 :
        ret = LCD_I2C_WriteCommand(LCD, (0xC0 + Coulmn));
        break;
    case ROW3 :
        ret = LCD_I2C_WriteCommand(LCD, (0x94 + Coulmn));
        break;
    case ROW4 :
        ret = LCD_I2C_WriteCommand(LCD, (0xD4 + Coulmn));
        break;
    default :
        ;
    }
    return ret;
}

static Std_ReturnType LCD_I2C_EnableControl(const LCD_I2C_t *LCD, uint8 Data)
{
	Std_ReturnType ret = E_OK;
	
	if (I2C_Start() == E_OK)                                                                           // Start I2C communication
	{
		if (I2C_WriteByte(&(LCD->LCD_I2C_Config), LCD->LCD_I2C_Config.I2C_Address << 1) == E_OK)            // Send the slave address
		{
			I2C_WriteByte(&(LCD->LCD_I2C_Config), Data | LCD_I2C_ENABLE);                              // Write data to the slave
			_delay_us(1);
			I2C_WriteByte(&(LCD->LCD_I2C_Config), Data & ~LCD_I2C_ENABLE);                             // Write data to the slave
			_delay_us(50);
		}
		I2C_Stop();                                                                                    // Stop I2C communication
	}
	
	return ret;
}

static Std_ReturnType LCD_I2C_Send(const LCD_I2C_t *LCD, uint8 Data, uint8 Mode) 
{
	Std_ReturnType ret = E_OK;
	
	uint8 HighNibble = Data & 0xF0;
	uint8 LowNibble = (Data << 4) & 0xF0;

	/* Send high and low nibbles with enable pulses */
	LCD_I2C_EnableControl(LCD, HighNibble | Mode | LCD_I2C_BACKLIGHT);
	LCD_I2C_EnableControl(LCD, LowNibble  | Mode | LCD_I2C_BACKLIGHT);
	
	return ret;
}