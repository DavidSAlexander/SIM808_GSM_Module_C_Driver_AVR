# SIM808 GSM Driver for AVR Microcontrollers

This repository provides a fully modular GSM driver for the **SIM808 GSM/GPRS module**, implemented for AVR microcontrollers. It includes APIs for initializing the module, managing GPRS connections, sending/receiving SMS messages, making phone calls, and interacting with HTTP/GPRS services.

The driver uses **USART** for communication with the SIM808 module and provides optional debug output using a secondary UART channel.

---

## Features

* GSM module initialization with configurable APN profiles
* Send and receive SMS messages
* Make outgoing calls
* Detect incoming messages and calls
* GPRS activation and HTTP response retrieval
* Response waiting with timeout
* Fully interrupt‑based USART communication

---

## Project Structure

```
GSM SIM808/
├── Application/
│   └── main.c
├── HAL/
│   ├── Inc/
│   │   └── GSM_Driver.h
│   └── Src/
│       └── GSM_Driver.c
```

## Getting Started
### 1. **Include the Required Headers**
```c
#include "GSM_Driver.h"
#include "USART_Driver.h"
````

### 2. **Configure USART Channels**

You must configure:

* One UART for SIM808 communication
* One UART for debugging (optional)

```c
USART_Config_t GSM_UART = {
    .USART_BaudRate          = USART_115200bps,
    .USART_Channel           = USART_CHANNEL0,
    .USART_DataSize          = USART_8BitsDataSize,
    .USART_InterruptStatus   = USART_InterruptEnabled,
    .USART_OperationMode     = USART_AsynchronousMode,
    .USART_ParityCheck       = USART_ParityCheckDisabled,
    .USART_DoubleSpeedStatus = USART_DoubleSpeedEnabled,
    .USART_EndCharacter      = '\n'
};
```

---

## GSM Driver API

Below is the list of all implemented APIs with descriptions.

### ### `Std_ReturnType GSM_Init(const USART_Config_t *USART, const USART_Config_t *DEBUG_UART, APN_Profile_t Profile);`

Initializes the SIM808 module, configures APN, enables text mode, and prepares the module for operation.

### `Std_ReturnType GSM_WaitForResponse(const USART_Config_t *USART, const char *expectedResponse, uint32 timeout);`

Waits for a specific response string from the SIM808 module within a timeout.

### `Std_ReturnType GSM_SetAPNProfile(const USART_Config_t *USART, APN_Profile_t Profile);`

Sets APN configuration depending on SIM operator (Vodafone, Orange, Etisalat, WE).

### `Std_ReturnType GSM_MakeCall(const USART_Config_t *USART, const uint8* number);`

Initiates a phone call to a specified phone number.

### `Std_ReturnType GSM_SendSMS(const USART_Config_t *USART, const uint8* number, const uint8* message);`

Sends an SMS message in text mode.

### `Std_ReturnType GSM_CheckNewSMS(const USART_Config_t *USART, const uint8 *buffer, uint8* State);`

Checks for new unread SMS messages.

### `Std_ReturnType GSM_ReceiveSMS(const USART_Config_t *USART);`

Reads the content of the most recently received SMS message.

### `Std_ReturnType GSM_OpenGPRS(const USART_Config_t *USART);`

Activates GPRS packet service and prepares for HTTP communication.

### `Std_ReturnType GSM_GetGPRS_Response(const USART_Config_t *USART);`

Retrieves HTTP/GPRS response after opening a connection.

---

## Example Usage

Below is a minimal example from `main.c`.

```c
/* Initialize LCD */
LCD_I2C_Init(&I2C_LCD1);

/* Initialize UART for debug and GSM */
USART_Init(&USART1);
USART_Init(&GSM_UART);

/* Initialize GSM module */
GSM_Init(&GSM_UART, &USART1, VODAFONE);

/* Send a message */
GSM_SendSMS(&GSM_UART, (const uint8*)"+201xxxxxxxx", (const uint8*)"Message From SIM808");

/* Make a call */
GSM_MakeCall(&GSM_UART, (const uint8*)"+201xxxxxxxxx");

/* Enable GPRS */
GSM_OpenGPRS(&GSM_UART);
GSM_GetGPRS_Response(&GSM_UART);

/* Listen for responses */
while (1)
{
    USART_Receive_String(&GSM_UART, GSMString);
    USART_StringReady(&GSM_UART, &GSMString_Status);

    if (GSMString_Status == USART_StringAvailable)
    {
        if (strstr((char*)GSMString, "+CMTI: \"SM\"") != NULL)
        {
            GSM_ReceiveSMS(&GSM_UART);
        }
        else if (strstr((char*)GSMString, "RING") != NULL)
        {
            // Incoming call detected
        }

        memset(GSMString, 0, sizeof(GSMString));
        GSMString_Status = USART_StringUnavailable;
    }
}
```

---

## Supported SIM Operators (APN Profiles - You can add your own)

| Provider | APN                     |
| -------- | ----------------------- |
| Vodafone | `internet.vodafone.net` |
| Orange   | `internet`              |
| Etisalat | `etisalat`              |
| WE       | `internet`              |

---

## Requirements

* AVR Microcontroller (ATmega128A/ATmega2560/etc.)
* SIM808 GSM/GPRS Module (or any)
* USART drivers (included)
* Optional: LCD I2C driver for debugging/output

---

## Notes

* Ensure proper baud rate: **115200 bps** recommended.
* SIM card must have:

  * Sufficient balance
  * SMS/Call capability
  * Data enabled

---

## Author

**David S. Alexander**

Feel free to contribute, open issues, or request enhancements!
