/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_midi_if.c
  * @version        : v1.0_Cube
  * @brief          : Generic media access layer.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
 /* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_midi_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_MIDI_IF
  * @{
  */

/** @defgroup USBD_MIDI_IF_Private_TypesDefinitions USBD_MIDI_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_IF_Private_Defines USBD_MIDI_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_IF_Private_Macros USBD_MIDI_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_MIDI_IF_Private_Variables USBD_MIDI_IF_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_IF_Exported_Variables USBD_MIDI_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_IF_Private_FunctionPrototypes USBD_MIDI_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t MIDI_Init_FS(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static int8_t MIDI_DeInit_FS(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static int8_t MIDI_Receive_FS(uint8_t* pbuf, uint32_t length);
static int8_t MIDI_Send_FS(uint8_t* pbuf, uint32_t length);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_MIDI_ItfTypeDef USBD_MIDI_fops_FS =
{
  MIDI_Init_FS,
  MIDI_DeInit_FS,
  MIDI_Receive_FS,
  MIDI_Send_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the MIDI media low layer over USB FS IP
  * @param  MidiFreq: Midi frequency used to play the midi stream.
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  options: Reserved for future use
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MIDI_Init_FS(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  /* USER CODE BEGIN 0 */
  printf("Midi Init\n");
  return (USBD_OK);
  /* USER CODE END 0 */
}

/**
  * @brief  De-Initializes the MIDI media low layer
  * @param  options: Reserved for future use
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MIDI_DeInit_FS(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  /* USER CODE BEGIN 1 */
  printf("Midi DeInit\n");
  return (USBD_OK);
  /* USER CODE END 1 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MIDI_Receive_FS(uint8_t* Buf, uint32_t Len)
{
  /* USER CODE BEGIN 6 */
  uint8_t chan = Buf[1] & 0xf;
  uint8_t msgtype = Buf[1] & 0xf0;
  uint8_t b1 =  Buf[2];
  uint8_t b2 =  Buf[3];
  uint16_t b = ((b2 & 0x7f) << 7) | (b1 & 0x7f);

  UNUSED(b);

  printf("MIDI_Receive_FS: chan = 0x%02x, msgtype = 0x%02x, b1 = 0x%02x, b2 = 0x%02x\n", chan, msgtype, b1, b2);
  
  switch (msgtype) {
  case 0xF0:
    if(chan == 0x0F) {
        NVIC_SystemReset(); // Reset into DFU mode
    }
  	break;
  default:
  	break;
  }

  // TODO: promote event to application ring buffer
  
  //USBD_MIDI_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
  //USBD_MIDI_ReceivePacket(&hUsbDeviceFS);

  //ring_buffer_queue_arr(&ring_buffer, (char*)Buf, *Len);

  //for(uint32_t i = 0; i < *Len; ++i) {
  //  ring_buffer_queue(&ring_buffer, Buf[i]);
  //}
  
  // *Buf and *UserRxBufferFS are identical
  //for(uint32_t i = 0; i < *Len; ++i) {
  //  printf("%c", UserRxBufferFS[i]);
  //}
  //printf("\n");
  //for(uint32_t i = 0; i < *Len; ++i) {
  //  printf("%c", Buf[i]);
  //}
  //printf("\n");
  //numChars += *Len;

  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  MIDI_Send
  *
  * @param  buffer: bufferfer of data to be received
  * @param  length: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MIDI_Send_FS(uint8_t* buffer, uint32_t length)
{
  uint8_t ret = USBD_OK;

  USBD_MIDI_SetTxBuffer(&hUsbDeviceFS, buffer, length);

  ret = USBD_MIDI_TransmitPacket(&hUsbDeviceFS);

  return (ret);
}

void MIDI_sendMessage(uint8_t* msg, uint8_t length) {
  MIDI_Send_FS(msg, length);
}

void MIDI_note_on(uint8_t note, uint8_t velocity) {
    uint8_t b[4];
    b[0] = 0x0B;
    b[1] = 0x90;
    b[2] = note;
    b[3] = velocity;

    MIDI_Send_FS(b, 4);

}

void MIDI_note_off(uint8_t note, uint8_t velocity) {
    uint8_t b[4];
    b[0] = 0x0B;
    b[1] = 0x80;
    b[2] = note;
    b[3] = velocity;

    MIDI_Send_FS(b, 4);

}

void MIDI_cc_update(uint8_t channel , uint8_t controler_number, uint8_t controller_value) {
    uint8_t b[4];
    b[0] = 0x0B;
    b[1] = 0xB0 | channel;
    b[2] = controler_number;
    b[3] = controller_value;

    MIDI_Send_FS(b, 4);

}

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */
