
/********************************************************************************************************
 *  [FILE NAME]   :      <GSM_SIM808.c>                                                                 *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <March 17, 2025>                                                               *
 *  [Description] :      <Source file for the GSM SIM808 Module driver>                                 *
 ********************************************************************************************************/

#include "../Inc/GSM_SIM808.h"

static APN_Profile_t Operator = VODAFONE;
static const USART_Config_t *USART_DEBUG;

Std_ReturnType GSM_Init(const USART_Config_t *USART, const USART_Config_t *DEBUG_UART, APN_Profile_t Profile)
{
	Std_ReturnType ret = E_OK;
	
	if(NULL == USART)
	{
		ret = E_NOT_OK;
	}
	else
	{
		Operator = Profile;
		USART_DEBUG = DEBUG_UART;
		USART_Init(USART);
		
		USART_Transmit_String(USART, (const uint8*)"AT+CFUN=1,1\r\n");         // Restart 
		GSM_WaitForResponse(USART, (const char*)"+CREG: 1", 30000);
		
		USART_Transmit_String(USART, (const uint8*)"ATE1\r\n");                // Disable echo
		GSM_WaitForResponse(USART, (const char*)"OK", 2000);
		
		USART_Transmit_String(USART, (const uint8*)"AT\r\n");                  // Check module
		GSM_WaitForResponse(USART, (const char*)"OK", 2000);
		
		USART_Transmit_String(USART, (const uint8*)"AT+CMGF=1\r\n");           // Set SMS to text mode
		GSM_WaitForResponse(USART, (const char*)"OK", 2000);
		
		USART_Transmit_String(USART, (const uint8*)"AT+CNMI=2,1,0,0,0\r\n");   // Enable new SMS notification 
		GSM_WaitForResponse(USART, (const char*)"OK", 2000);
	}
	
	return ret;
}

Std_ReturnType GSM_WaitForResponse(const USART_Config_t *USART, const char *expectedResponse, uint32 timeout)
{
	if (NULL == USART || NULL == expectedResponse) 
	{
		return E_NOT_OK;
	}

    USART_StringStatus_t GSMString_Status = USART_StringUnavailable;
	uint8 buffer[512] = {0};	
    uint32 msTimer = 0;							       // Buffer to store received data

	while (msTimer < (timeout))			               // Timeout loop
	{
		USART_Receive_String(USART, buffer);           /* Receive from GSM */    
		USART_StringReady(USART, &GSMString_Status);  
		if (GSMString_Status == USART_StringAvailable) // If data is received
		{
			USART_Transmit_String(USART_DEBUG, buffer);
			// Check if expected response is in buffer
			if (strstr((char*)buffer, expectedResponse) != NULL)
			{
				_delay_ms(1000);
				return E_OK;						  // Response found 
			}
					
            else 
			{
	            // Buffer overflow - reset buffer
	            memset(buffer, 0, sizeof(buffer));
            }
			GSMString_Status = USART_StringUnavailable;
		}
		_delay_ms(1);
		msTimer++;
	}

	return E_NOT_OK;  // Timeout occurred
}

Std_ReturnType GSM_SetAPNProfile(const USART_Config_t *USART, APN_Profile_t Profile)
{
	Std_ReturnType ret = E_OK;

	if (NULL == USART)
	{
		ret = E_NOT_OK;
	}
	else
	{
		/* Common GPRS Configuration */
		USART_Transmit_String(USART, (const uint8*)"AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");
		GSM_WaitForResponse(USART, (const char*)"OK", 5000);

		/* Set APN based on the selected profile */
		switch (Profile)
		{
			case WE:
			USART_Transmit_String(USART, (const uint8*)"AT+SAPBR=3,1,\"APN\",\"internet.te.eg\"\r\n");
			break;
			case ORANGE:
			USART_Transmit_String(USART, (const uint8*)"AT+SAPBR=3,1,\"APN\",\"mobinilweb\"\r\n");
			break;
			case ETISALAT:
			USART_Transmit_String(USART, (const uint8*)"AT+SAPBR=3,1,\"APN\",\"internet\"\r\n");
			break;
			case VODAFONE:
			USART_Transmit_String(USART, (const uint8*)"AT+SAPBR=3,1,\"APN\",\"internet.vodafone.net\"\r\n");
			break;
			default:
			ret = E_NOT_OK;  // Invalid profile
			break;
		}

		GSM_WaitForResponse(USART, (const char*)"OK", 5000);

		/* Activate GPRS */
		if (ret == E_OK)
		{
			USART_Transmit_String(USART, (const uint8*)"AT+SAPBR=1,1\r\n");
			GSM_WaitForResponse(USART, (const char*)"OK", 5000);
		}
	}

	return ret;
}

Std_ReturnType GSM_MakeCall(const USART_Config_t *USART, const uint8* number)
{
	Std_ReturnType ret = E_OK;
	
	if(NULL == USART)
	{
		ret = E_NOT_OK;
	}
	else
	{
		USART_Transmit_String(USART, (const uint8*)"ATD");     
		USART_Transmit_String(USART, number);
		USART_Transmit_String(USART, (const uint8*)";\r\n");
		GSM_WaitForResponse(USART, (const char*)"OK", 5000);
	}
	
	return ret;
}

Std_ReturnType GSM_SendSMS(const USART_Config_t *USART, const uint8* number, const uint8* message)
{
	Std_ReturnType ret = E_OK;
	
	if(NULL == USART)
	{
		ret = E_NOT_OK;
	}
	else
	{
		USART_Transmit_String(USART, (const uint8*)"AT+CMGS=\"");
		USART_Transmit_String(USART, number);
		USART_Transmit_String(USART, (const uint8*)"\"\r\n");
		USART_Transmit_String(USART, message);
		_delay_ms(100);
		USART_Transmit_Byte(USART, 0x1A);          // End message with Ctrl+Z
		USART_Transmit_Byte(USART, '\n'); 
		GSM_WaitForResponse(USART, (const char*)"OK", 5000);
	}
	
	return ret;
}

Std_ReturnType GSM_CheckNewSMS(const USART_Config_t *USART, const uint8 *buffer, uint8* State)
{
	Std_ReturnType ret = E_OK;
	
	if(NULL == USART)
	{
		ret = E_NOT_OK;
	}
	else
	{
		if (strstr((char*)buffer, "+CMTI:") != NULL)
		{
			*State = 1;  // New SMS detected
		}
		else
		{
			*State = 0;
		}
	}
	
	return ret;  // No new SMS
}

Std_ReturnType GSM_ReceiveSMS(const USART_Config_t *USART)
{
	Std_ReturnType ret = E_OK;

	if(NULL == USART)
	{
		ret = E_NOT_OK;
	}
	else
	{
		/* Send AT command to list unread SMS */
		USART_Transmit_String(USART, (const uint8*)"AT+CMGL=\"REC UNREAD\"\r\n");
		GSM_WaitForResponse(USART, (const char*)"+CMGL: 1", 3000);
	}
	
	return ret;
}



Std_ReturnType GSM_OpenGPRS(const USART_Config_t *USART)
{
	Std_ReturnType ret = E_OK;
	
	if(NULL == USART)
	{
		ret = E_NOT_OK;
	}
	else
	{
		GSM_SetAPNProfile(USART, Operator);
	}
	
	return ret;
}

Std_ReturnType GSM_GetGPRS_Response(const USART_Config_t *USART)
{
	Std_ReturnType ret = E_OK;

	if (NULL == USART)
	{
		ret = E_NOT_OK;
	}
	else
	{	
		USART_Transmit_String(USART, (const uint8*)"AT+HTTPINIT\r\n");  // Initialize HTTP service
		GSM_WaitForResponse(USART, (const char*)"OK", 5000);
		
		USART_Transmit_String(USART, (const uint8*)"AT+HTTPPARA=\"CID\",1\r\n");  // Use bearer profile 1
		GSM_WaitForResponse(USART, (const char*)"OK", 5000);
		
		// Set URL to fetch date
		USART_Transmit_String(USART, (const uint8*)"AT+HTTPPARA=\"URL\",\"http://api.quotable.io/random?tags=wisdom\"\r\n");
		GSM_WaitForResponse(USART, (const char*)"OK", 5000);
		
		USART_Transmit_String(USART, (const uint8*)"AT+HTTPACTION=0\r\n");  // Perform GET request
		GSM_WaitForResponse(USART, (const char*)"+HTTPACTION: 0,200", 5000);
		
		USART_Transmit_String(USART, (const uint8*)"AT+HTTPREAD\r\n");  // Read response
		GSM_WaitForResponse(USART, (const char*)"{\"", 5000);

		// Close HTTP connection
		USART_Transmit_String(USART, (const uint8*)"AT+HTTPTERM\r\n");  // Terminate HTTP service
		GSM_WaitForResponse(USART, (const char*)"OK", 5000);
		
		USART_Transmit_String(USART, (const uint8*)"AT+SAPBR=0,1\r\n");  // Terminate GPRS service
		GSM_WaitForResponse(USART, (const char*)"OK", 5000);
	}
	
	return ret;
}
