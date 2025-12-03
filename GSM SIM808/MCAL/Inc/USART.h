
/********************************************************************************************************
*  [FILE NAME]   :      <USART.h>                                                                       *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <Dec 6, 2024>                                                                   *
*  [Description] :      <Header file for the AVR ATmega 128 USART driver>                               *
*********************************************************************************************************/


#ifndef USART_H_
#define USART_H_

#include "../../Includes/STD_TYPES.h"
#include "../../Includes/DEVICE_CONFIG.h"
#include "../../Includes/BIT_MACROS.h"
#include "../../Includes/STD_LIBRARIES.h"
#include "Global_Interrupt.h"

#define USART_TX_BUFFER_SIZE 512
#define USART_RX_BUFFER_SIZE 512


#define UCSR0A_REG     SFR_IO8(0x0B)
#define UCSR0B_REG     SFR_IO8(0x0A)
#define UCSR0C_REG     SFR_MEM8(0x95)

#define UCSR1A_REG     SFR_MEM8(0x9B)
#define UCSR1B_REG     SFR_MEM8(0x9A)
#define UCSR1C_REG     SFR_MEM8(0x9D)

#define UBRR0L_REG     SFR_IO8(0x09)
#define UBRR0H_REG     SFR_MEM8(0x90)

#define UBRR1L_REG     SFR_MEM8(0x99)
#define UBRR1H_REG     SFR_MEM8(0x98)

#define UDR0_REG       SFR_IO8(0x0C)
#define UDR1_REG       SFR_MEM8(0x9C)


/* UCSR0A */
#define RXC0_BIT       7
#define TXC0_BIT       6
#define UDRE0_BIT      5
#define FE0_BIT        4
#define DOR0_BIT       3
#define UPE0_BIT       2
#define U2X0_BIT       1
#define MPCM0_BIT      0

/* UCSR1A */
#define RXC1_BIT       7
#define TXC1_BIT       6
#define UDRE1_BIT      5
#define FE1_BIT        4
#define DOR1_BIT       3
#define UPE1_BIT       2
#define U2X1_BIT       1
#define MPCM1_BIT      0

/* UCSR0B */
#define RXCIE0_BIT     7
#define TXCIE0_BIT     6
#define UDRIE0_BIT     5
#define RXEN0_BIT      4
#define TXEN0_BIT      3
#define UCSZ02_BIT     2
#define RXB80_BIT      1
#define TXB80_BIT      0

/* UCSR1B */
#define RXCIE1_BIT     7
#define TXCIE1_BIT     6
#define UDRIE1_BIT     5
#define RXEN1_BIT      4
#define TXEN1_BIT      3
#define UCSZ12_BIT     2
#define RXB81_BIT      1
#define TXB81_BIT      0

/* UCSR0C */
#define UMSEL01_BIT    7
#define UMSEL00_BIT    6
#define UPM01_BIT      5
#define UPM00_BIT      4
#define USBS0_BIT      3
#define UCSZ01_BIT     2
#define UCSZ00_BIT     1
#define UCPOL0_BIT     0

/* UCSR1C */
#define UMSEL11_BIT    7
#define UMSEL10_BIT    6
#define UPM11_BIT      5
#define UPM10_BIT      4
#define USBS1_BIT      3
#define UCSZ11_BIT     2
#define UCSZ10_BIT     1
#define UCPOL1_BIT     0

#define USART_CHANNELS 2

typedef enum
{
	USART_CHANNEL0,
	USART_CHANNEL1,

}USART_Channel_t;

typedef enum
{
	USART_2400bps   = 2400,
	USART_4800bps   = 4800,
	USART_9600bps   = 9600,
	USART_14406bps  = 14406,
	USART_19200bps  = 19200,
	USART_38400bps  = 38400,
	USART_57600bps  = 57600,
	USART_115200bps = 115200,
	USART_128000bps = 128000,
	USART_256000bps = 256000
	
}USART_BuadRates_t;

typedef enum
{
	USART_ParityCheckDisabled,
	USART_ParityCheckEnabledEven = 2,
	USART_ParityCheckEnabledOdd
	
}USART_ParityCheck_t;

typedef enum
{
	USART_1BitStop,
	USART_2BitsStop
	
}USART_StopBits_t;

typedef enum
{
	USART_InterruptDisabled,
	USART_InterruptEnabled
	
}USART_Interrupt_t;

typedef enum
{
	USART_RisingTxFallingRxEdge,
	USART_RisingRxFallingTxEdge
	
}USART_ClockPolaritySelect_t;

typedef enum
{
	USART_AsynchronousMode,
	USART_SynchronousMode
	
}USART_OperationMode_t;

typedef enum
{
	USART_5BitsDataSize,
	USART_6BitsDataSize,
	USART_7BitsDataSize,
	USART_8BitsDataSize,
	USART_9BitsDataSize = 7
	
}USART_DataSize_t;

typedef enum
{
	USART_DoubleSpeedDisabled = 16,
	USART_DoubleSpeedEnabled  = 8
	
}USART_DoubleTransmissionSpeed_t;

typedef enum
{
	USART_StringUnavailable,
	USART_StringAvailable
	
}USART_StringStatus_t;

typedef struct
{
	USART_BuadRates_t				     USART_BaudRate;
	USART_DataSize_t					 USART_DataSize;
	USART_OperationMode_t				 USART_OperationMode;
	USART_ClockPolaritySelect_t		     USART_ClockPolarity;
	USART_Interrupt_t					 USART_InterruptStatus;
	USART_StopBits_t					 USART_StopBits;
	USART_ParityCheck_t				     USART_ParityCheck;
	USART_DoubleTransmissionSpeed_t      USART_DoubleSpeedStatus;
	USART_Channel_t                      USART_Channel;
	uint8                                USART_EndCharacter;
	
}USART_Config_t;

Std_ReturnType USART_Init(const USART_Config_t* USARTcfg);
Std_ReturnType USART_Transmit_Byte(const USART_Config_t* USARTcfg, uint8 data);
Std_ReturnType USART_Receive_Byte(const USART_Config_t* USARTcfg, uint8* data);
Std_ReturnType USART_Transmit_String(const USART_Config_t* USARTcfg, const uint8* data);
Std_ReturnType USART_Receive_String(const USART_Config_t* USARTcfg, uint8* data);
Std_ReturnType USART_Flush(const USART_Config_t* USARTcfg);
Std_ReturnType USART_StringReady(const USART_Config_t* USARTcfg, USART_StringStatus_t* status);


#endif /* USART_H_ */