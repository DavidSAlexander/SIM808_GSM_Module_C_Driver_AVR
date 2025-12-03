/********************************************************************************************************
*  [FILE NAME]   :      <I2C.c>                                                                        *
*  [AUTHOR]      :      <David S. Alexander>                                                           *
*  [DATE CREATED]:      <Dec 6, 2024>                                                                  *
*  [Description] :      <Source file for the AVR I2C driver>                                           *
********************************************************************************************************/

#include "../Inc/I2C.h"
#include <math.h>

Std_ReturnType I2C_Init(const I2C_Config_t* I2Ccfg)
{
	Std_ReturnType ret = E_OK;
	if (NULL == I2Ccfg)
	{
		ret = E_NOT_OK;
	}
	else
	{
		/* Configure I2C Interrupt */
		if (I2Ccfg->I2C_InterruptStatus == I2C_InterruptDisabled || I2Ccfg->I2C_InterruptStatus == I2C_InterruptEnabled)
		{
			TWCR_REG |= I2Ccfg->I2C_InterruptStatus;
		}
		else return E_NOT_OK;
		
		/* Configure I2C Prescaler */
		if (I2Ccfg->I2C_Prescaler >= I2C_Prescaler_1 && I2Ccfg->I2C_Prescaler <= I2C_Prescaler_64 )
		{
			TWSR_REG |= I2Ccfg->I2C_Prescaler;
		}
		else return E_NOT_OK;
		
		
		/* Set I2C Address */
		TWAR_REG |= (I2Ccfg->I2C_Address & 0xFE);
		
		/* Calculate and set I2C Bit Rate Register (TWBR) */
		uint8 PrescalerVal = I2Ccfg->I2C_Prescaler == I2C_Prescaler_1   ? 1  :
                   		     I2Ccfg->I2C_Prescaler == I2C_Prescaler_4   ? 4  :
		                     I2Ccfg->I2C_Prescaler == I2C_Prescaler_16  ? 16 : 64;
		
		float32 TWBR_Val = ((F_CPU / I2Ccfg->I2C_Frequency) - 16) / (2 * PrescalerVal);			 
		TWBR_REG = (uint8) round(TWBR_Val);
		
		/* Enable TWI module */
		SET_BIT(TWCR_REG, TWEN_BIT);
	}
	
	return ret;
}
Std_ReturnType I2C_WriteByte(const I2C_Config_t* I2Ccfg, uint8 Data)
{
	Std_ReturnType ret = E_OK;
	if (NULL == I2Ccfg)
	{
		ret = E_NOT_OK;
	}
	else
	{
		/* Load data into TWDR */
		TWDR_REG = Data;
		
		/* Clear TWINT to start transmission and ensure TWEN is set */
		TWCR_REG = (1 << TWINT_BIT) | (1 << TWEN_BIT);
		
		
		/* Wait for TWINT to be set, indicating transmission is complete */
		while (BIT_IS_CLEAR(TWCR_REG, TWINT_BIT));

		/* Check the status register to confirm successful transmission */
		uint8 Status = I2C_GetStatus(); 

        /* Check for data sent or address acknowledgment */
		if (Status != I2C_SLA_WTransmitted_ACKReceived && Status != I2C_DataByteTransmitted_ACKReceived)  
		{
			/* Transmission error or no ACK received */
			ret = E_NOT_OK; 
		}
	}
	return ret;
}


Std_ReturnType I2C_ReadByteWithAck(const I2C_Config_t* I2Ccfg, uint8* Data)
{
	Std_ReturnType ret = E_OK;
	if (NULL == I2Ccfg || NULL == Data)
	{
		ret = E_NOT_OK;
	}
	else
	{
		TWCR_REG |= (1 << TWINT_BIT) | (1 << TWEN_BIT) | (1 << TWEA_BIT);
		while(BIT_IS_CLEAR(TWCR_REG,TWINT_BIT));
		*Data = TWDR_REG;
	}
	return ret;
}
Std_ReturnType I2C_ReadByteWithNack(const I2C_Config_t* I2Ccfg, uint8* Data)
{
	Std_ReturnType ret = E_OK;
	if (NULL == I2Ccfg || NULL == Data)
	{
		ret = E_NOT_OK;
	}
	else
	{
		TWCR_REG = (1 << TWINT_BIT) | (1 << TWEN_BIT);
		while(BIT_IS_CLEAR(TWCR_REG,TWINT_BIT));
		*Data = TWDR_REG;
	}
	return ret;
}
Std_ReturnType I2C_GeneralCallRecognition(I2C_GCE_t I2Cgce)
{
	Std_ReturnType ret = E_OK;
	if (I2Cgce != I2C_DisableGCE && I2Cgce != I2C_EnableGCE)
	{
		ret = E_NOT_OK;
	}
	else
	{
		TWAR_REG |= I2Cgce;
	}
	return ret;
}
uint8 I2C_GetStatus(void)
{
	uint8 Status;
	Status = TWSR_REG & I2C_NoRelevantStatusInformation;
	
	return Status;
}
Std_ReturnType I2C_Start()
{
	/* Set TWINT, TWSTA, and TWEN to start the I2C transmission */
	TWCR_REG = (1 << TWINT_BIT) | (1 << TWSTA_BIT) | (1 << TWEN_BIT);
	
	/* Wait for TWINT to be set, indicating the start condition has been transmitted */
	while (BIT_IS_CLEAR(TWCR_REG, TWINT_BIT))
	{
		/* Check for START sent acknowledgment */
		if (I2C_GetStatus() == I2C_SLA_RTransmitted_NACKReceived || I2C_GetStatus() == I2C_SLA_WTransmitted_NACKReceived)  
		{
			return E_NOT_OK;
		}
	}
	
	return E_OK;
}
Std_ReturnType I2C_Stop()
{
	/* Set TWINT, TWSTO, and TWEN to send the stop condition */
	TWCR_REG = (1 << TWINT_BIT) | (1 << TWEN_BIT) | (1 << TWSTO_BIT);
	
	return E_OK;
}