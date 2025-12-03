
/********************************************************************************************************
 *  [FILE NAME]   :      <GSM_SIM808.h>                                                                 *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <March 17, 2025>                                                               *
 *  [Description] :      <Header file for the GSM SIM808 Module driver>                                 *
 ********************************************************************************************************/


#ifndef GSM_SIM808_H_
#define GSM_SIM808_H_

/*******************************************************************************
 *                                 Includes                                    *
 *******************************************************************************/

#include "../../MCAL/Inc/USART.h"
#include <string.h>
#include "stdio.h"

/*******************************************************************************
 *                             Macro Declarations                              *
 *******************************************************************************/


/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/

typedef enum 
{
	WE,
	ORANGE,
	ETISALAT,
	VODAFONE,
	PROFILE_INVALID
	
} APN_Profile_t;

/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

Std_ReturnType GSM_Init(const USART_Config_t *USART, const USART_Config_t *DEBUG_UART, APN_Profile_t Profile);
Std_ReturnType GSM_WaitForResponse(const USART_Config_t *USART, const char *expectedResponse, uint32 timeout);
Std_ReturnType GSM_SetAPNProfile(const USART_Config_t *USART, APN_Profile_t Profile);
Std_ReturnType GSM_MakeCall(const USART_Config_t *USART, const uint8* number);
Std_ReturnType GSM_SendSMS(const USART_Config_t *USART, const uint8* number, const uint8* message);
Std_ReturnType GSM_CheckNewSMS(const USART_Config_t *USART, const uint8 *buffer, uint8* State);
Std_ReturnType GSM_ReceiveSMS(const USART_Config_t *USART);
Std_ReturnType GSM_OpenGPRS(const USART_Config_t *USART);
Std_ReturnType GSM_GetGPRS_Response(const USART_Config_t *USART);

#endif /* GSM_SIM808_H_ */