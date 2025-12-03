
/********************************************************************************************************
*  [FILE NAME]   :      <main.c>                                                                        *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <Dec 2, 2024>                                                                   *
*  [Description] :      <Source file for the AVR GSM Main application driver>                           *
********************************************************************************************************/

#include "AppConfig.h"

LCD_I2C_t I2C_LCD1 =
{
	.LCD_I2C_Config.I2C_Address         = 0x27,
	.LCD_I2C_Config.I2C_Frequency       = 100000,
	.LCD_I2C_Config.I2C_InterruptStatus = I2C_InterruptEnabled,
	.LCD_I2C_Config.I2C_Mode            = I2C_Master,
	.LCD_I2C_Config.I2C_Prescaler       = I2C_Prescaler_1,
	.LCD_I2C_Mode                       = LCD_I2C_4Bit
};

USART_Config_t USART1 =
{
	.USART_BaudRate          = USART_115200bps,
	.USART_Channel           = USART_CHANNEL1,
	.USART_DataSize          = USART_8BitsDataSize,
	.USART_InterruptStatus   = USART_InterruptEnabled,
	.USART_OperationMode     = USART_AsynchronousMode,
	.USART_ParityCheck       = USART_ParityCheckDisabled,
	.USART_DoubleSpeedStatus = USART_DoubleSpeedEnabled,
	.USART_EndCharacter      = '\n'
};

USART_Config_t GSM_UART =
{
	.USART_BaudRate          = USART_115200bps,
	.USART_Channel           = USART_CHANNEL0,
	.USART_DataSize          = USART_8BitsDataSize,
	.USART_InterruptStatus   = USART_InterruptEnabled,
	.USART_OperationMode     = USART_AsynchronousMode,     
	.USART_ParityCheck       = USART_ParityCheckDisabled,
	.USART_DoubleSpeedStatus = USART_DoubleSpeedEnabled,
	.USART_EndCharacter      = '\n'
};

const uint8 SmilecustomChar[] =
{
	0x00,
	0x0A,
	0x00,
	0x04,
	0x11,
	0x0E,
	0x00,
	0x00
};

const uint8 HeartcustomChar[] =
{
	0x00,
	0x0A,
	0x1F,
	0x1F,
	0x0E,
	0x04,
	0x00,
	0x00
};

int main(void)
{
	LCD_I2C_Init(&I2C_LCD1);
	
	LCD_I2C_WriteCustomChar(&I2C_LCD1, 1, 1, HeartcustomChar, 0);
	LCD_I2C_WriteStringInPos(&I2C_LCD1, 1, 3, (const uint8*)"Welcome David");
	LCD_I2C_WriteCustomChar(&I2C_LCD1, 1, 16, SmilecustomChar, 1);
	_delay_ms(1500);
	
	/* Connect a UART module and set the termite baudrate 115200bps and end characters to Append CR-LF */
	USART_Init(&USART1);  
	
	USART_Transmit_String(&USART1, (const uint8*)"\nGSM Module \nInitializing... \n");
	_delay_ms(1500);
	
	/* Connect the GSM module */
	GSM_Init(&GSM_UART, &USART1, VODAFONE);                       /* Choose your SIM Operator */
	
	USART_StringStatus_t GSMString_Status = USART_StringUnavailable;
	USART_StringStatus_t UARTString_Status = USART_StringUnavailable;
	
	uint8 GSMString[512];
	uint8 UARTString[512];
	LCD_I2C_WriteStringInPos(&I2C_LCD1, 2, 1, (const uint8*)"GSM Module Ready");
	USART_Transmit_String(&USART1, (const uint8*)"\nGSM Module Ready\n");	

    /* Establish a GPRS connection */
	GSM_OpenGPRS(&GSM_UART);
	_delay_ms(2500);
	
	/* Establish an HTTP connection and get the response */
	GSM_GetGPRS_Response(&GSM_UART);
	_delay_ms(2500);
	
	GSM_SendSMS(&GSM_UART, (const uint8*)"+201xxxxxxxx", (const uint8*)"Message From SIM808 By David");   /* Send Message to a specific number */
	_delay_ms(2500);
	GSM_MakeCall(&GSM_UART, (const uint8*)"+201xxxxxxxxx");                                               /* make a call to a specific number */
	
	while (1)
	{
		USART_Receive_String(&GSM_UART, GSMString);                   /* Receive from GSM */
		USART_Receive_String(&USART1, UARTString);                    /* Receive from UART */
		
		USART_StringReady(&GSM_UART, &GSMString_Status);
		USART_StringReady(&USART1, &UARTString_Status);

		if (GSMString_Status == USART_StringAvailable)
		{
			USART_Transmit_String(&USART1, GSMString);
			
			if (strstr((char*)GSMString, "+CMTI: \"SM\"") != NULL)  
			{
				LCD_I2C_WriteStringInPos(&I2C_LCD1, 2, 1, (const uint8*)" New Message !  ");    /* Display new message */
				_delay_ms(1000);
				GSM_ReceiveSMS(&GSM_UART);
			}
			else if (strstr((char*)GSMString, "RING") != NULL)
			{
				LCD_I2C_WriteStringInPos(&I2C_LCD1, 2, 1, (const uint8*)" Incoming Call  ");    /* Display new call, to respond a call send ATA through the termite or ATH to reject the call*/
			}

			/* Ensure String is properly null-terminated */
			memset(GSMString, 0, sizeof(GSMString));
			GSMString_Status = USART_StringUnavailable;
		}
		
	    if (UARTString_Status == USART_StringAvailable)
		{
			USART_Transmit_String(&GSM_UART, UARTString);
			/* Ensure String is properly null-terminated */
			memset(UARTString, 0, sizeof(UARTString));
			UARTString_Status = USART_StringUnavailable;
		}
	}
	
	return 0;
}