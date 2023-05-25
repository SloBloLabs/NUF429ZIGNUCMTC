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
static uint8_t buffUsbReport[MIDI_EPIN_SIZE] = {0};
static uint8_t buffUsbReportNextIndex = 0;

static uint8_t buffUsb[MIDI_BUFFER_LENGTH] = {0};
static volatile uint8_t buffUsbNextIndex = 0;
static uint8_t buffUsbCurrIndex = 0;
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

static int8_t MIDI_Init_FS();
static int8_t MIDI_DeInit_FS();
//static int8_t MIDI_Receive_FS(uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_MIDI_ItfTypeDef USBD_MIDI_fops_FS =
{
  MIDI_Init_FS,
  MIDI_DeInit_FS,
  //MIDI_Receive_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the MIDI media low layer over USB FS IP
  * @param  MidiFreq: Midi frequency used to play the midi stream.
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  options: Reserved for future use
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MIDI_Init_FS()
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
static int8_t MIDI_DeInit_FS()
{
  /* USER CODE BEGIN 1 */
  printf("Midi DeInit\n");
  return (USBD_OK);
  /* USER CODE END 1 */
}

///**
//  * @brief  Data received over USB OUT endpoint are sent over CDC interface
//  *         through this function.
//  *
//  *         @note
//  *         This function will issue a NAK packet on any OUT packet received on
//  *         USB endpoint until exiting this function. If you exit this function
//  *         before transfer is complete on CDC interface (ie. using DMA controller)
//  *         it will result in receiving more data while previous ones are still
//  *         not sent.
//  *
//  * @param  Buf: Buffer of data to be received
//  * @param  Len: Number of data received (in bytes)
//  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
//  */
//static int8_t MIDI_Receive_FS(uint8_t* Buf, uint32_t *Len)
//{
//  /* USER CODE BEGIN 6 */
//  //USBD_MIDI_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
//  //USBD_MIDI_ReceivePacket(&hUsbDeviceFS);
//
//  //ring_buffer_queue_arr(&ring_buffer, (char*)Buf, *Len);
//
//  //for(uint32_t i = 0; i < *Len; ++i) {
//  //  ring_buffer_queue(&ring_buffer, Buf[i]);
//  //}
//  
//  // *Buf and *UserRxBufferFS are identical
//  //for(uint32_t i = 0; i < *Len; ++i) {
//  //  printf("%c", UserRxBufferFS[i]);
//  //}
//  //printf("\n");
//  //for(uint32_t i = 0; i < *Len; ++i) {
//  //  printf("%c", Buf[i]);
//  //}
//  //printf("\n");
//  //numChars += *Len;
//  return (USBD_OK);
//  /* USER CODE END 6 */
//}

void USBD_MIDI_DataInHandler(uint8_t *usb_rx_buffer, uint8_t usb_rx_buffer_length)
{
  printf("MidiIF: Handling Data, size=%d\n", usb_rx_buffer_length);
  for(uint32_t i = 0; i < usb_rx_buffer_length; ++i) {
    //printf("%d", usb_rx_buffer[i]);
    printf("%0x", usb_rx_buffer[i] & 0xff);
  }
  printf("\n");

  while (usb_rx_buffer_length && *usb_rx_buffer != 0x00)
  {
    buffUsb[buffUsbNextIndex++] = *usb_rx_buffer++;
    buffUsb[buffUsbNextIndex++] = *usb_rx_buffer++;
    buffUsb[buffUsbNextIndex++] = *usb_rx_buffer++;
    buffUsb[buffUsbNextIndex++] = *usb_rx_buffer++;

    usb_rx_buffer_length -= 4;
  }
}
/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

bool MIDI_HasUSBData(void)
{
  return buffUsbCurrIndex != buffUsbNextIndex;
}

void MIDI_ProcessUSBData(void)
{
  static uint8_t lastMessagesBytePerCable[MIDI_CABLES_NUMBER] = {0};
  uint8_t *pLastMessageByte;
  uint8_t cable;
  uint8_t messageByte;
  uint8_t message;
  uint8_t param1;
  uint8_t param2;
  void (*pSend)(uint8_t);

  if (buffUsbCurrIndex == buffUsbNextIndex)
    return;

  cable = (buffUsb[buffUsbCurrIndex] >> 4);
  messageByte = buffUsb[buffUsbCurrIndex + 1];

  if (cable == 0)
  {
    pLastMessageByte = &lastMessagesBytePerCable[0];
    //pSend = &UART1_Send;
    printf("Send cable 1\n");
  }
  else if (cable == 1)
  {
    pLastMessageByte = &lastMessagesBytePerCable[1];
    //pSend = &UART2_Send;
    printf("Send cable 2\n");
  }
  else if (cable == 2)
  {
    pLastMessageByte = &lastMessagesBytePerCable[2];
    //pSend = &UART3_Send;
    printf("Send cable 3\n");
  }
  else
  {
    pSend = NULL;
  }

  if (pSend != NULL)
  {
    message = (messageByte >> 4);
    param1 = buffUsb[buffUsbCurrIndex + 2];
    param2 = buffUsb[buffUsbCurrIndex + 3];

    if ((messageByte & MIDI_MASK_REAL_TIME_MESSAGE) == MIDI_MASK_REAL_TIME_MESSAGE)
    {
      pSend(messageByte);
    }
    else if (message == MIDI_MESSAGE_CHANNEL_PRESSURE ||
             message == MIDI_MESSAGE_PROGRAM_CHANGE ||
             messageByte == MIDI_MESSAGE_TIME_CODE_QTR_FRAME ||
             messageByte == MIDI_MESSAGE_SONG_SELECT)
    {
      if (*pLastMessageByte != messageByte)
      {
        pSend(messageByte);
        *pLastMessageByte = messageByte;
      }
      pSend(param1);
    }
    else if (message == MIDI_MESSAGE_NOTE_ON ||
             message == MIDI_MESSAGE_NOTE_OFF ||
             message == MIDI_MESSAGE_KEY_PRESSURE ||
             message == MIDI_MESSAGE_CONTROL_CHANGE ||
             messageByte == MIDI_MESSAGE_SONG_POSITION ||
             message == MIDI_MESSAGE_PITCH_BAND_CHANGE)
    {
      if (*pLastMessageByte != messageByte)
      {
        pSend(messageByte);
        *pLastMessageByte = messageByte;
      }
      pSend(param1);
      pSend(param2);
    }
  }

  buffUsbCurrIndex += 4;
}

void MIDI_addToUSBReport(uint8_t cable, uint8_t message, uint8_t param1, uint8_t param2)
{
  buffUsbReport[buffUsbReportNextIndex++] = (cable << 4) | (message >> 4);
  buffUsbReport[buffUsbReportNextIndex++] = (message);
  buffUsbReport[buffUsbReportNextIndex++] = (param1);
  buffUsbReport[buffUsbReportNextIndex++] = (param2);

  if (buffUsbReportNextIndex == MIDI_EPIN_SIZE)
  {
    while (USBD_MIDI_GetState(&hUsbDeviceFS) != MIDI_IDLE) {};
    USBD_MIDI_SendReport(&hUsbDeviceFS, buffUsbReport, MIDI_EPIN_SIZE);
    buffUsbReportNextIndex = 0;
  }
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */
