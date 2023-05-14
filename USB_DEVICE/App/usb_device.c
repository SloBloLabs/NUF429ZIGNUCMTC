/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usb_device.c
  * @version        : v1.0_Cube
  * @brief          : This file implements the USB Device
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

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
//#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
//#include "usbd_dfu.h"
#include "usbd_dfu_if.h"
//#include "usbd_audio.h"
#include "usbd_audio_if.h"

/* USER CODE BEGIN Includes */
#include "usbd_composite_builder.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t AUDIO_EpAdd = AUDIO_OUT_EP;    /* AUDIO Endpoint Adress */
uint8_t CDC_EpAdd_Inst1[3] = {CDC_IN_EP, CDC_OUT_EP, CDC_CMD_EP}; /* CDC Endpoint Adress First Instance */
uint8_t DFU_EpAdd[2] = {DFU_IN_EP, DFU_OUT_EP}; /* DFU Endpoint Adress */
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Device Core handle declaration. */
USBD_HandleTypeDef hUsbDeviceFS;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_DEVICE_Init(void)
{
  /* USER CODE BEGIN USB_DEVICE_Init_PreTreatment */

  /* USER CODE END USB_DEVICE_Init_PreTreatment */

  /* Init Device Library, add supported class and start the library. */

  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
  {
    Error_Handler();
  }
//  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CMPSIT) != USBD_OK)
//  {
//    Error_Handler();
//  }
#if USBD_CMPSIT_ACTIVATE_AUDIO == 1
  if (USBD_RegisterClassComposite(&hUsbDeviceFS, &USBD_AUDIO, CLASS_TYPE_AUDIO, &AUDIO_EpAdd) != USBD_OK)
  {
    Error_Handler();
  }

  /* Add Interface callbacks for AUDIO Class */
  if (USBD_CMPSIT_SetClassID(&hUsbDeviceFS, CLASS_TYPE_AUDIO, 0) != 0xFF)
  {
    if (USBD_AUDIO_RegisterInterface(&hUsbDeviceFS, &USBD_AUDIO_fops_FS) != USBD_OK)
    {
      Error_Handler();
    }
  }
#endif
#if USBD_CMPSIT_ACTIVATE_CDC == 1
  if (USBD_RegisterClassComposite(&hUsbDeviceFS, &USBD_CDC, CLASS_TYPE_CDC, CDC_EpAdd_Inst1) != USBD_OK)
  {
    Error_Handler();
  }
  
//  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK)
//  {
//    Error_Handler();
//  }
  
  /* Add CDC Interface Class First Instance */
  if (USBD_CMPSIT_SetClassID(&hUsbDeviceFS, CLASS_TYPE_CDC, 0) != 0xFF)
  {
    if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_CDC_Interface_fops_FS) != USBD_OK)
    {
      Error_Handler();
    }
  }
#endif
#if USBD_CMPSIT_ACTIVATE_DFU == 1
  if (USBD_RegisterClassComposite(&hUsbDeviceFS, &USBD_DFU, CLASS_TYPE_DFU, DFU_EpAdd) != USBD_OK)
  {
    Error_Handler();
  }
//  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_DFU) != USBD_OK)
//  {
//    Error_Handler();
//  }

  /* Add DFU Interface Class First Instance */
  if (USBD_CMPSIT_SetClassID(&hUsbDeviceFS, CLASS_TYPE_DFU, 0) != 0xFF)
  {
    if (USBD_DFU_RegisterMedia(&hUsbDeviceFS, &USBD_DFU_fops_FS) != USBD_OK)
    {
      Error_Handler();
    }
  }
#endif
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN USB_DEVICE_Init_PostTreatment */

  /* USER CODE END USB_DEVICE_Init_PostTreatment */
}

/**
  * @}
  */

/**
  * @}
  */

