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

static int8_t MIDI_Init_FS(uint32_t MidiFreq, uint32_t Volume, uint32_t options);
static int8_t MIDI_DeInit_FS(uint32_t options);
static int8_t MIDI_Receive_FS(uint8_t* pbuf, uint32_t *Len);
static int8_t MIDI_TransmitCplt_FS(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);

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
  MIDI_TransmitCplt_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the MIDI media low layer over USB FS IP
  * @param  MidiFreq: Midi frequency used to play the midi stream.
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  options: Reserved for future use
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MIDI_Init_FS(uint32_t MidiFreq, uint32_t Volume, uint32_t options)
{
  /* USER CODE BEGIN 0 */
  UNUSED(MidiFreq);
  UNUSED(Volume);
  UNUSED(options);
  printf("Midi Init\n");
  return (USBD_OK);
  /* USER CODE END 0 */
}

/**
  * @brief  De-Initializes the MIDI media low layer
  * @param  options: Reserved for future use
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MIDI_DeInit_FS(uint32_t options)
{
  /* USER CODE BEGIN 1 */
  UNUSED(options);
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
static int8_t MIDI_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
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
  * @brief  MIDI_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t MIDI_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  USBD_MIDI_HandleTypeDef *hcdc = (USBD_MIDI_HandleTypeDef*)hUsbDeviceFS.pClassData;
  if (hcdc->state == MIDI_BUSY){
    return USBD_BUSY;
  }
  //USBD_MIDI_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_MIDI_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/**
  * @brief  MIDI_TransmitCplt_FS
  *         Data transmitted callback
  *
  *         @note
  *         This function is IN transfer complete callback used to inform user that
  *         the submitted Data is successfully sent over USB.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MIDI_TransmitCplt_FS(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 13 */
  UNUSED(Buf);
  UNUSED(Len);
  UNUSED(epnum);
  /* USER CODE END 13 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */
