/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_midi_if.h
  * @version        : v1.0_Cube
  * @brief          : Header for usbd_midi_if.c file.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_MIDI_IF_H__
#define __USBD_MIDI_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_midi.h"
#include "swvPrint.h"
#include <stdbool.h>

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief For Usb device.
  * @{
  */

/** @defgroup USBD_MIDI_IF USBD_MIDI_IF
  * @brief Usb midi interface device module.
  * @{
  */

/** @defgroup USBD_MIDI_IF_Exported_Defines USBD_MIDI_IF_Exported_Defines
  * @brief Defines.
  * @{
  */

/* USER CODE BEGIN EXPORTED_DEFINES */
#define MIDI_CABLES_NUMBER 3

#define MIDI_BUFFER_LENGTH           256
#define MIDI_MAX_CHANNELS_NUM        16
#define MIDI_MAX_CABLES_NUM          16
#define MIDI_MAX_ADDITIONAL_VELOCITY 127
#define MIDI_MAX_NOTE_SHIFT          24
#define MIDI_MIN_NOTE_SHIFT         -24

#define MIDI_MESSAGE_NOTE_OFF          0x08
#define MIDI_MESSAGE_NOTE_ON           0x09
#define MIDI_MESSAGE_KEY_PRESSURE      0x0A
#define MIDI_MESSAGE_CONTROL_CHANGE    0x0B
#define MIDI_MESSAGE_PROGRAM_CHANGE    0x0C
#define MIDI_MESSAGE_CHANNEL_PRESSURE  0x0D
#define MIDI_MESSAGE_PITCH_BAND_CHANGE 0x0E

#define MIDI_MESSAGE_TIMING_CLOCK   0xF8
#define MIDI_MESSAGE_START          0xFA
#define MIDI_MESSAGE_CONTINUE       0xFB
#define MIDI_MESSAGE_STOP           0xFC
#define MIDI_MESSAGE_ACTIVE_SENSING 0xFE
#define MIDI_MESSAGE_SYSTEM_RESET   0xFF

#define MIDI_MESSAGE_TIME_CODE_QTR_FRAME 0xF1
#define MIDI_MESSAGE_SONG_POSITION       0xF2
#define MIDI_MESSAGE_SONG_SELECT         0xF3

#define MIDI_MASK_STATUS_BYTE       0x80
#define MIDI_MASK_REAL_TIME_MESSAGE 0xF8

#define MIDI_MESSAGE_CONTROL_ALL_SOUNDS_OFF        120
#define MIDI_MESSAGE_CONTROL_RESET_ALL_CONTROLLERS 121
#define MIDI_MESSAGE_CONTROL_ALL_NOTES_OFF         123

#define MIDI_MESSAGE_PITCH_BAND_MIDDLE 8192
#define MIDI_MESSAGE_PITCH_BAND_MAX    16383
#define MIDI_MESSAGE_PITCH_BAND_MIN    0

#define MIDI_MESSAGE_NOTE_MAX             127
#define MIDI_MESSAGE_NOTE_VOLUME_MAX      127
#define MIDI_MESSAGE_CONTROL_VALUE_MAX    127
#define MIDI_MESSAGE_CONTROL_VALUE_MIN    0
#define MIDI_MESSAGE_CONTROL_VALUE_MIDDLE 64
#define MIDI_MESSAGE_DEFAULT_VOLUME       100
/* USER CODE END EXPORTED_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_IF_Exported_Types USBD_MIDI_IF_Exported_Types
  * @brief Types.
  * @{
  */

/* USER CODE BEGIN EXPORTED_TYPES */

/* USER CODE END EXPORTED_TYPES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_IF_Exported_Macros USBD_MIDI_IF_Exported_Macros
  * @brief Aliases.
  * @{
  */

/* USER CODE BEGIN EXPORTED_MACRO */

/* USER CODE END EXPORTED_MACRO */

/**
  * @}
  */

/** @defgroup USBD_MIDI_IF_Exported_Variables USBD_MIDI_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

/** MIDI_IF Interface callback. */
extern USBD_MIDI_ItfTypeDef USBD_MIDI_fops_FS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_MIDI_IF_Exported_FunctionsPrototype USBD_MIDI_IF_Exported_FunctionsPrototype
  * @brief Public functions declaration.
  * @{
  */

/* USER CODE BEGIN EXPORTED_FUNCTIONS */

void MIDI_ProcessUSBData(void);
void MIDI_addToUSBReport(uint8_t cable, uint8_t message, uint8_t param1, uint8_t param2);

/* USER CODE END EXPORTED_FUNCTIONS */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_MIDI_IF_H__ */
