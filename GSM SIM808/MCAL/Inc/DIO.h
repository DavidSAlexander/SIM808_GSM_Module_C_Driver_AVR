
/********************************************************************************************************
 *  [FILE NAME]   :      <DIO.h>                                                                        *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 6, 2024>                                                                  *
 *  [Description] :      <Header file for the AVR ATmega 128 DIO driver>                                *
 ********************************************************************************************************/
 
#ifndef DIO_H
#define	DIO_H

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "../../Includes/STD_TYPES.h"
#include "../../Includes/DEVICE_CONFIG.h"
#include "../../Includes/BIT_MACROS.h"
#include "../../Includes/STD_LIBRARIES.h"

/*******************************************************************************
 *                             Macro Declarations                              *
 *******************************************************************************/


/* Port G */
#define DIO_PING    SFR_MEM8(0x63)
#define DIO_DDRG    SFR_MEM8(0x64)
#define DIO_PORTG   SFR_MEM8(0x65)

/* Port F */
#define DIO_PINF    SFR_IO8(0x00)
#define DIO_DDRF    SFR_MEM8(0x61)
#define DIO_PORTF   SFR_MEM8(0x62)

/* Port E */
#define DIO_PINE    SFR_IO8(0x01)
#define DIO_DDRE    SFR_IO8(0x02)
#define DIO_PORTE   SFR_IO8(0x03)

/* Port D */
#define DIO_PIND    SFR_IO8(0x10)  
#define DIO_DDRD    SFR_IO8(0x11)
#define DIO_PORTD   SFR_IO8(0x12)

/* Port C */
#define DIO_PINC    SFR_IO8(0x13)
#define DIO_DDRC    SFR_IO8(0x14)
#define DIO_PORTC   SFR_IO8(0x15)

/* Port B */
#define DIO_PINB    SFR_IO8(0X16)
#define DIO_DDRB    SFR_IO8(0x17)
#define DIO_PORTB   SFR_IO8(0x18)

/* Port A */
#define DIO_PINA    SFR_IO8(0X19)
#define DIO_DDRA    SFR_IO8(0X1A)
#define DIO_PORTA   SFR_IO8(0X1B)

/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/

typedef enum
{
	LOW,
	HIGH
} DIO_PinLogic;

typedef enum
{
	DIO_Pulldown,
	DIO_InternalPullup
} DIO_PinInputRes;

typedef enum
{
    PIN_INPUT,
    PIN_OUTPUT
} DIO_PinDirection;

typedef enum
{
    PORT_INPUT  = 0,
    PORT_OUTPUT = 0xFF
} DIO_PortDirection;

typedef enum
{
    PIN0_INDEX,
    PIN1_INDEX,
    PIN2_INDEX,
    PIN3_INDEX,
    PIN4_INDEX,
    PIN5_INDEX,
    PIN6_INDEX,
    PIN7_INDEX
} DIO_PinIndex;

typedef enum
{
    PORTA_INDEX,
    PORTB_INDEX,
    PORTC_INDEX,
    PORTD_INDEX,
	PORTE_INDEX,
	PORTF_INDEX,
	PORTG_INDEX,
	
} DIO_PortIndex;

typedef struct
{
    uint8 PORTx  : 3;
    uint8 PINx   : 3;
    uint8 DIRx   : 1;
    uint8 STATEx : 1;

} DIO_PinConfig;

		
/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

Std_ReturnType DIO_PinDirSetup(const DIO_PinConfig *Pin_Config);
Std_ReturnType DIO_PinDirStatus(const DIO_PinConfig *Pin_Config, DIO_PinDirection *Dir_Status);
Std_ReturnType DIO_PinWrite(const DIO_PinConfig *Pin_Config, DIO_PinLogic Logic);
Std_ReturnType DIO_PinRead(const DIO_PinConfig *Pin_Config, DIO_PinLogic *Logic);
Std_ReturnType DIO_PinToggle(const DIO_PinConfig *Pin_Config);

Std_ReturnType DIO_PortDirSetup(DIO_PortIndex Port_Config, uint8 direction);
Std_ReturnType DIO_PortDirStatus(DIO_PortIndex Port_Status, uint8 *Status);
Std_ReturnType DIO_PortWrite(const DIO_PortIndex Port_Index, uint8 Logic);
Std_ReturnType DIO_PortRead(const DIO_PortIndex Port_Index, uint8 *Logic);
Std_ReturnType DIO_PortToggle(const DIO_PortIndex Port_Index);
Std_ReturnType DIO_PortROL(DIO_PortIndex Port, uint8 NUM);
Std_ReturnType DIO_PortROR(DIO_PortIndex Port, uint8 NUM);

#endif	/* DIO_H */
