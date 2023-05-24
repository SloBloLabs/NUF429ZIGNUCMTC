/**
  ******************************************************************************
  * @file    usbd_midi.c
  * @author  MCD Application Team
  * @brief   This file provides the Midi core functions.
  *
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  * @verbatim
  *
  *          ===================================================================
  *                                MIDI Class  Description
  *          ===================================================================
  *           This driver manages the Midi Class 1.0 following the "USB Device Class Definition for
  *           Midi Devices V1.0 Mar 18, 98".
  *           This driver implements the following aspects of the specification:
  *             - Device descriptor management
  *             - Configuration descriptor management
  *
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *
  *
  *  @endverbatim
  ******************************************************************************
  */

/* BSPDependencies
- "stm32xxxxx_{eval}{discovery}.c"
- "stm32xxxxx_{eval}{discovery}_io.c"
- "stm32xxxxx_{eval}{discovery}_midi.c"
EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include "usbd_midi.h"
#include "usbd_ctlreq.h"


/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_MIDI
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_MIDI_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_MIDI_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_MIDI_Private_Macros
  * @{
  */

/**
  * @}
  */


/** @defgroup USBD_MIDI_Private_FunctionPrototypes
  * @{
  */
static uint8_t USBD_MIDI_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_MIDI_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);

static uint8_t USBD_MIDI_Setup(USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req);
#ifndef USE_USBD_COMPOSITE
static uint8_t *USBD_MIDI_GetCfgDesc(uint16_t *length);
static uint8_t *USBD_MIDI_GetDeviceQualifierDesc(uint16_t *length);
#endif /* USE_USBD_COMPOSITE  */
static uint8_t USBD_MIDI_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_MIDI_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);

/**
  * @}
  */

/** @defgroup USBD_MIDI_Private_Variables
  * @{
  */

static uint8_t usb_rx_buffer[MIDI_EPOUT_SIZE] = {0};

USBD_ClassTypeDef USBD_MIDI =
{
  USBD_MIDI_Init,
  USBD_MIDI_DeInit,
  USBD_MIDI_Setup,
  NULL, //USBD_MIDI_EP0_TxReady,
  NULL, //USBD_MIDI_EP0_RxReady,
  USBD_MIDI_DataIn,
  USBD_MIDI_DataOut,
  NULL, //USBD_MIDI_SOF,
  NULL, //USBD_MIDI_IsoINIncomplete,
  NULL, //USBD_MIDI_IsoOutIncomplete,
#ifdef USE_USBD_COMPOSITE
  NULL,
  NULL,
  NULL,
  NULL,
#else
  USBD_MIDI_GetCfgDesc,
  USBD_MIDI_GetCfgDesc,
  USBD_MIDI_GetCfgDesc,
  USBD_MIDI_GetDeviceQualifierDesc,
#endif /* USE_USBD_COMPOSITE  */
};

#ifndef USE_USBD_COMPOSITE
/* USB MIDI device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_MIDI_CfgDesc[USB_MIDI_CONFIG_DESC_SIZE] __ALIGN_END =
{
  /* Configuration 1 */
  0x09,                                 /* bLength */
  USB_DESC_TYPE_CONFIGURATION,          /* bDescriptorType */
  LOBYTE(USB_MIDI_CONFIG_DESC_SIZE),    /* wTotalLength */
  HIBYTE(USB_MIDI_CONFIG_DESC_SIZE),
  0x01,                                 /* bNumInterfaces */
  0x01,                                 /* bConfigurationValue */
  0x00,                                 /* iConfiguration */
#if (USBD_SELF_POWERED == 1U)
  0xC0,                                 /* bmAttributes: Self Powered according to user configuration */
#else
  0x80,                                 /* bmAttributes: Bus Powered according to user configuration */
#endif /* USBD_SELF_POWERED */
  USBD_MAX_POWER,                       /* MaxPower (mA) */
  /* 09 byte*/

  /************** MIDI Adapter Standard MS Interface Descriptor ****************/
  0x09,                         /*bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,      /*bDescriptorType: Interface descriptor type*/
  0x00,                         /*bInterfaceNumber: Index of this interface.*/
  0x00,                         /*bAlternateSetting: Alternate setting*/
  0x02,                         /*bNumEndpoints*/
  USB_DEVICE_CLASS_AUDIO,       /*bInterfaceClass: AUDIO*/
  AUDIO_SUBCLASS_MIDISTREAMING, /*bInterfaceSubClass : MIDISTREAMING*/
  0x00,                         /*nInterfaceProtocol : Unused*/
  0x00,                         /*iInterface: 0*/

  /******************** MIDI Adapter Class-specific MS Interface Descriptor ********************/
  /* USB_MIDI_CLASS_DESC_SHIFT */
  0x07,                 /*bLength: Descriptor size*/
  0x24,                 /*bDescriptorType: CS_INTERFACE descriptor*/
  0x01,                 /*bDescriptorSubtype: MS_HEADER subtype*/
  0x00,
  0x01,                 /*BcdADC: Revision of this class specification*/
  USB_MIDI_REPORT_DESC_SIZE,
  0x00,                  /*wTotalLength: Total size of class-specific descriptors*/

#if MIDI_IN_PORTS_NUM >= 1
  /******************** MIDI Adapter MIDI IN Jack Descriptor (External) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_1,            /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_2,            /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_1,            /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 2
  /******************** MIDI Adapter MIDI IN Jack Descriptor (External) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_3,            /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_4,            /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_3,            /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 3
  /******************** MIDI Adapter MIDI IN Jack Descriptor (External) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_5,            /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_6,            /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_5,            /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 4
  /******************** MIDI Adapter MIDI IN Jack Descriptor (External) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_7,            /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_8,            /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_7,            /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 5 
  /******************** MIDI Adapter MIDI IN Jack Descriptor (External) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_9,            /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_10,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_9,            /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 6 
  /******************** MIDI Adapter MIDI IN Jack Descriptor (External) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_11,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_12,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_11,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 7 
  /******************** MIDI Adapter MIDI IN Jack Descriptor (External) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_13,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_14,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_13,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 8
  /******************** MIDI Adapter MIDI IN Jack Descriptor (External) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_15,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_16,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_15,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 1
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_17,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_18,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_17,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 2
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_19,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_20,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_19,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 3
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_21,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_22,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_21,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 4
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_23,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_24,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_23,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 5
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_25,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_26,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_25,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 6
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_27,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_28,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_27,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 7
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_29,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_30,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_29,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 8
  /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded) ********************/
  0x06,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x02,                   /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
  0x01,                   /*bJackType: EMBEDDED*/
  MIDI_JACK_31,           /*bJackID: ID of this Jack.*/
  0x00,                   /*iJack: Unused.*/

  /******************** MIDI Adapter MIDI OUT Jack Descriptor (External) ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  0x24,                   /*bDescriptorType: CS_INTERFACE descriptor.*/
  0x03,                   /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
  0x02,                   /*bJackType: EXTERNAL.*/
  MIDI_JACK_32,           /*bJackID: ID of this Jack.*/
  0x01,                   /*bNrInputPins: Number of Input Pins of this Jack.*/
  MIDI_JACK_31,           /*BaSourceID(1): ID of the Entity to which this Pin is connected.*/
  0x01,                   /*BaSourcePin(1): Output Pin number of the Entity to which this Input Pin is connected.*/
  0x00,                   /*iJack: Unused.*/
#endif

  /******************** MIDI Adapter Standard Bulk OUT Endpoint Descriptor ********************/
  0x09,                   /*bLength: Size of this descriptor, in bytes*/
  USB_DESC_TYPE_ENDPOINT, /*bDescriptorType: ENDPOINT descriptor.*/
  MIDI_OUT_EP,            /*bEndpointAddress: OUT Endpoint 1.*/
  0x02,                   /*bmAttributes: Bulk, not shared.*/
  MIDI_EPOUT_SIZE, 
  0x00,                   /*wMaxPacketSize*/
  0x00,                   /*bInterval: Ignored for Bulk. Set to zero.*/
  0x00,                   /*bRefresh: Unused.*/
  0x00,                   /*bSynchAddress: Unused.*/

  /******************** MIDI Adapter Class-specific Bulk OUT Endpoint Descriptor ********************/
  (4 + MIDI_OUT_PORTS_NUM), /*bLength: Size of this descriptor, in bytes*/
  0x25,                     /*bDescriptorType: CS_ENDPOINT descriptor*/
  0x01,                     /*bDescriptorSubtype: MS_GENERAL subtype.*/
  MIDI_OUT_PORTS_NUM,       /*bNumEmbMIDIJack: Number of embedded MIDI IN Jacks.*/
#if MIDI_OUT_PORTS_NUM >= 1
  MIDI_JACK_17,             /*BaAssocJackID(1): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 2
  MIDI_JACK_19,             /*BaAssocJackID(2): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 3
  MIDI_JACK_21,             /*BaAssocJackID(3): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 4
  MIDI_JACK_23,             /*BaAssocJackID(4): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 5
  MIDI_JACK_25,             /*BaAssocJackID(5): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 6
  MIDI_JACK_27,             /*BaAssocJackID(6): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 7
  MIDI_JACK_29,             /*BaAssocJackID(7): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 8
  MIDI_JACK_31,             /*BaAssocJackID(8): ID of the Embedded MIDI IN Jack.*/
#endif

  /******************** MIDI Adapter Standard Bulk IN Endpoint Descriptor ********************/
  0x09,                    /*bLength: Size of this descriptor, in bytes*/
  USB_DESC_TYPE_ENDPOINT,  /*bDescriptorType: ENDPOINT descriptor.*/
  MIDI_IN_EP,          /*bEndpointAddress: IN Endpoint 1.*/
  0x02,                    /*bmAttributes: Bulk, not shared.*/
  MIDI_EPIN_SIZE, 
  0x00,                    /*wMaxPacketSize*/
  0x00,                    /*bInterval: Ignored for Bulk. Set to zero.*/
  0x00,                    /*bRefresh: Unused.*/
  0x00,                    /*bSynchAddress: Unused.*/

  /******************** MIDI Adapter Class-specific Bulk IN Endpoint Descriptor ********************/
  (4 + MIDI_IN_PORTS_NUM), /*bLength: Size of this descriptor, in bytes*/
  0x25,                    /*bDescriptorType: CS_ENDPOINT descriptor*/
  0x01,                    /*bDescriptorSubtype: MS_GENERAL subtype.*/
  MIDI_IN_PORTS_NUM,       /*bNumEmbMIDIJack: Number of embedded MIDI OUT Jacks.*/
#if MIDI_IN_PORTS_NUM >= 1
  MIDI_JACK_2,             /*BaAssocJackID(1): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 2
  MIDI_JACK_4,             /*BaAssocJackID(2): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 3
  MIDI_JACK_6,             /*BaAssocJackID(3): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 4
  MIDI_JACK_8,             /*BaAssocJackID(4): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 5
  MIDI_JACK_10,            /*BaAssocJackID(5): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 6
  MIDI_JACK_12,            /*BaAssocJackID(6): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 7
  MIDI_JACK_14,            /*BaAssocJackID(7): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 8
  MIDI_JACK_16,            /*BaAssocJackID(8): ID of the Embedded MIDI OUT Jack.*/
#endif

} ;

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_MIDI_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};
#endif /* USE_USBD_COMPOSITE  */

static uint8_t MIDIInEpAdd  = MIDI_IN_EP;
static uint8_t MIDIOutEpAdd = MIDI_OUT_EP;

/**
  * @}
  */

/** @defgroup USBD_MIDI_Private_Functions
  * @{
  */

/**
  * @brief  USBD_MIDI_Init
  *         Initialize the MIDI interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_MIDI_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);
  USBD_MIDI_HandleTypeDef *hmidi;

  /* Allocate Midi structure */
  hmidi = (USBD_MIDI_HandleTypeDef *)USBD_malloc(sizeof(USBD_MIDI_HandleTypeDef));

  if (hmidi == NULL)
  {
    pdev->pClassDataCmsit[pdev->classId] = NULL;
    return (uint8_t)USBD_EMEM;
  }

  pdev->pClassDataCmsit[pdev->classId] = (void *)hmidi;
  pdev->pClassData = pdev->pClassDataCmsit[pdev->classId];

#ifdef USE_USBD_COMPOSITE
  /* Get the Endpoints addresses allocated for this class instance */
  MIDIInEpAdd  = USBD_CoreGetEPAdd(pdev, USBD_EP_IN, USBD_EP_TYPE_BULK);
  MIDIOutEpAdd  = USBD_CoreGetEPAdd(pdev, USBD_EP_OUT, USBD_EP_TYPE_BULK);
#endif /* USE_USBD_COMPOSITE */

  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    pdev->ep_in[MIDIInEpAdd & 0xFU].bInterval = MIDI_HS_BINTERVAL;
    pdev->ep_out[MIDIOutEpAdd & 0xFU].bInterval = MIDI_HS_BINTERVAL;
  }
  else   /* LOW and FULL-speed endpoints */
  {
    pdev->ep_in[MIDIInEpAdd & 0xFU].bInterval = MIDI_FS_BINTERVAL;
    pdev->ep_out[MIDIOutEpAdd & 0xFU].bInterval = MIDI_FS_BINTERVAL;
  }

  // Open EP IN
  (void)USBD_LL_OpenEP(pdev, MIDIInEpAdd, USBD_EP_TYPE_INTR, MIDI_EPIN_SIZE);
  pdev->ep_in[MIDIInEpAdd & 0xFU].is_used = 1U;

  /* Open EP OUT */
  (void)USBD_LL_OpenEP(pdev, MIDIOutEpAdd, USBD_EP_TYPE_INTR, MIDI_EPOUT_SIZE);
  pdev->ep_out[MIDIOutEpAdd & 0xFU].is_used = 1U;

  //hmidi->alt_setting = 0U;
  //hmidi->offset = MIDI_OFFSET_UNKNOWN;
  //hmidi->wr_ptr = 0U;
  //hmidi->rd_ptr = 0U;
  //hmidi->rd_enable = 0U;

  /* Initialize the Midi output Hardware layer */
  if (((USBD_MIDI_ItfTypeDef *)pdev->pUserData[pdev->classId])->Init())
  {
    return (uint8_t)USBD_FAIL;
  }

  /* Prepare Out endpoint to receive 1st packet */
  //(void)USBD_LL_PrepareReceive(pdev, MIDIOutEpAdd, hmidi->buffer,
  //                             MIDI_OUT_PACKET_SIZE);
  (void)USBD_LL_PrepareReceive(pdev, MIDIOutEpAdd, usb_rx_buffer, MIDI_EPOUT_SIZE);

  hmidi->state = MIDI_IDLE;

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MIDI_Init
  *         DeInitialize the MIDI layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_MIDI_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

#ifdef USE_USBD_COMPOSITE
  /* Get the Endpoints addresses allocated for this class instance */
  MIDIInEpAdd  = USBD_CoreGetEPAdd(pdev, USBD_EP_IN, USBD_EP_TYPE_BULK);
  MIDIOutEpAdd = USBD_CoreGetEPAdd(pdev, USBD_EP_OUT, USBD_EP_TYPE_BULK);
#endif /* USE_USBD_COMPOSITE */

  /* Close EP IN */
  (void)USBD_LL_CloseEP(pdev, MIDIInEpAdd);
  pdev->ep_in[MIDIInEpAdd & 0xFU].is_used = 0U;
  pdev->ep_in[MIDIInEpAdd & 0xFU].bInterval = 0U;

  /* Close EP OUT */
  (void)USBD_LL_CloseEP(pdev, MIDIOutEpAdd);
  pdev->ep_out[MIDIOutEpAdd & 0xFU].is_used = 0U;
  pdev->ep_out[MIDIOutEpAdd & 0xFU].bInterval = 0U;

  /* DeInit physical Interface components */
  if (pdev->pClassDataCmsit[pdev->classId] != NULL)
  {
    ((USBD_MIDI_ItfTypeDef *)pdev->pUserData[pdev->classId])->DeInit();
    (void)USBD_free(pdev->pClassDataCmsit[pdev->classId]);
    pdev->pClassDataCmsit[pdev->classId] = NULL;
    pdev->pClassData = NULL;
  }

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MIDI_Setup
  *         Handle the MIDI specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t USBD_MIDI_Setup(USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req)
{
  USBD_MIDI_HandleTypeDef *hmidi;
  uint16_t len = 0;
  uint8_t *pbuf;
  //uint16_t status_info = 0U;
  USBD_StatusTypeDef ret = USBD_OK;

  printf("MIDI SETUP\n");

  hmidi = (USBD_MIDI_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

  if (hmidi == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
    case USB_REQ_TYPE_CLASS:
      switch (req->bRequest)
      {
      case MIDI_REQ_SET_PROTOCOL:
        hmidi->Protocol = (uint8_t)(req->wValue);
        break;
        
      case MIDI_REQ_GET_PROTOCOL:
        USBD_CtlSendData (pdev, 
                          (uint8_t *)&hmidi->Protocol,
                          1);    
        break;
        
      case MIDI_REQ_SET_IDLE:
        hmidi->IdleState = (uint8_t)(req->wValue >> 8);
        break;
        
      case MIDI_REQ_GET_IDLE:
        USBD_CtlSendData (pdev, 
                          (uint8_t *)&hmidi->IdleState,
                          1);        
        break;
        //case MIDI_REQ_GET_CUR:
        //  MIDI_REQ_GetCurrent(pdev, req);
        //  break;
//
        //case MIDI_REQ_SET_CUR:
        //  MIDI_REQ_SetCurrent(pdev, req);
        //  break;

        default:
          USBD_CtlError(pdev, req);
          ret = USBD_FAIL;
          break;
      }
      break;

    case USB_REQ_TYPE_STANDARD:
      switch (req->bRequest)
      {
      case USB_REQ_GET_DESCRIPTOR: 
        if( req->wValue >> 8 == MIDI_DESCRIPTOR_TYPE)
        {
          pbuf = USBD_MIDI_CfgDesc + USB_MIDI_CLASS_DESC_SHIFT;
          len = MIN(USB_MIDI_DESC_SIZE , req->wLength);
        }
        
        USBD_CtlSendData (pdev, pbuf, len);
        break;
        
      case USB_REQ_GET_INTERFACE :
        USBD_CtlSendData (pdev,
                          (uint8_t *)&hmidi->AltSetting,
                          1);
        break;
        
      case USB_REQ_SET_INTERFACE :
        hmidi->AltSetting = (uint8_t)(req->wValue);
        break;
        //case USB_REQ_GET_STATUS:
        //  if (pdev->dev_state == USBD_STATE_CONFIGURED)
        //  {
        //    (void)USBD_CtlSendData(pdev, (uint8_t *)&status_info, 2U);
        //  }
        //  else
        //  {
        //    USBD_CtlError(pdev, req);
        //    ret = USBD_FAIL;
        //  }
        //  break;
//
        //case USB_REQ_GET_DESCRIPTOR:
        //  if ((req->wValue >> 8) == MIDI_DESCRIPTOR_TYPE)
        //  {
        //    pbuf = (uint8_t *)USBD_MIDI_GetMidiHeaderDesc(pdev->pConfDesc);
        //    if (pbuf != NULL)
        //    {
        //      len = MIN(USB_MIDI_DESC_SIZ, req->wLength);
        //      (void)USBD_CtlSendData(pdev, pbuf, len);
        //    }
        //    else
        //    {
        //      USBD_CtlError(pdev, req);
        //      ret = USBD_FAIL;
        //    }
        //  }
        //  break;
//
        //case USB_REQ_GET_INTERFACE:
        //  if (pdev->dev_state == USBD_STATE_CONFIGURED)
        //  {
        //    (void)USBD_CtlSendData(pdev, (uint8_t *)&hmidi->alt_setting, 1U);
        //  }
        //  else
        //  {
        //    USBD_CtlError(pdev, req);
        //    ret = USBD_FAIL;
        //  }
        //  break;
//
        //case USB_REQ_SET_INTERFACE:
        //  if (pdev->dev_state == USBD_STATE_CONFIGURED)
        //  {
        //    if ((uint8_t)(req->wValue) <= USBD_MAX_NUM_INTERFACES)
        //    {
        //      hmidi->alt_setting = (uint8_t)(req->wValue);
        //    }
        //    else
        //    {
        //      /* Call the error management function (command will be NAKed */
        //      USBD_CtlError(pdev, req);
        //      ret = USBD_FAIL;
        //    }
        //  }
        //  else
        //  {
        //    USBD_CtlError(pdev, req);
        //    ret = USBD_FAIL;
        //  }
        //  break;
//
        //case USB_REQ_CLEAR_FEATURE:
        //  break;
//
        //default:
        //  USBD_CtlError(pdev, req);
        //  ret = USBD_FAIL;
        //  break;
      }
      break;
    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
  }

  return (uint8_t)ret;
}

/**
  * @brief  USBD_MIDI_GetState 
  *         Get MIDI State
  * @param  pdev: device instance
  * @retval usb state  (MIDI_IDLE, MIDI_BUSY)
  */
uint8_t USBD_MIDI_GetState(USBD_HandleTypeDef *pdev)
{
  return ((USBD_MIDI_HandleTypeDef *)pdev->pClassData)->state;
}

/**
  * @brief  USBD_MIDI_SendReport 
  *         Send MIDI Report
  *         called by application
  * @param  pdev: device instance
  * @param  report: pointer to report
  * @param  len: size of report
  * @retval status
  */
uint8_t USBD_MIDI_SendReport(USBD_HandleTypeDef  *pdev, 
                                 uint8_t *report,
                                 uint16_t len)
{
  //USBD_MIDI_HandleTypeDef *hmidi = pdev->pClassData;
  USBD_MIDI_HandleTypeDef *hmidi = (USBD_MIDI_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];
  
  printf("MIDI SEND REPORT\n");

#ifdef USE_USBD_COMPOSITE
  /* Get the Endpoints addresses allocated for this class instance */
  MIDIInEpAdd  = USBD_CoreGetEPAdd(pdev, USBD_EP_IN, USBD_EP_TYPE_BULK);
#endif /* USE_USBD_COMPOSITE */

  if (pdev->dev_state == USBD_STATE_CONFIGURED)
  {
    if(hmidi->state == MIDI_IDLE)
    {
      hmidi->state = MIDI_BUSY;
      USBD_LL_Transmit(pdev, MIDIInEpAdd, report, len);
    }
  }
  return USBD_OK;
}

/**
  * @brief  USBD_CDC_TransmitPacket
  *         Transmit packet on IN endpoint
  * @param  pdev: device instance
  * @retval status
  */
uint8_t USBD_MIDI_TransmitPacket(USBD_HandleTypeDef *pdev)
{
  USBD_MIDI_HandleTypeDef *hmidi = (USBD_MIDI_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];
  USBD_StatusTypeDef ret = USBD_BUSY;

  printf("MIDI TRANSMIT PACKET\n");

#ifdef USE_USBD_COMPOSITE
  /* Get the Endpoints addresses allocated for this class instance */
  MIDIInEpAdd  = USBD_CoreGetEPAdd(pdev, USBD_EP_IN, USBD_EP_TYPE_BULK);
#endif /* USE_USBD_COMPOSITE */
  if (pdev->pClassDataCmsit[pdev->classId] == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  if (hmidi->state == MIDI_IDLE)
  {
    /* Tx Transfer in progress */
    hmidi->state = MIDI_BUSY;

    /* Update the packet total length */
    //pdev->ep_in[MIDIInEpAdd & 0xFU].total_length = hmidi->TxLength;
    pdev->ep_in[MIDIInEpAdd & 0xFU].total_length = MIDI_EPOUT_SIZE;

    /* Transmit next packet */
    //(void)USBD_LL_Transmit(pdev, MIDIInEpAdd, hmidi->TxBuffer, hmidi->TxLength);
    (void)USBD_LL_Transmit(pdev, MIDIInEpAdd, usb_rx_buffer, MIDI_EPOUT_SIZE);

    ret = USBD_OK;
  }

  return (uint8_t)ret;
}


#ifndef USE_USBD_COMPOSITE
/**
  * @brief  USBD_MIDI_GetCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_MIDI_GetCfgDesc(uint16_t *length)
{
  
  printf("MIDI GET CFG DESC\n");

  *length = (uint16_t)sizeof(USBD_MIDI_CfgDesc);

  return USBD_MIDI_CfgDesc;
}
#endif /* USE_USBD_COMPOSITE  */

/**
  * @brief  USBD_MIDI_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_MIDI_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  //uint16_t PacketSize;
  USBD_MIDI_HandleTypeDef *hmidi;

  printf("MIDI DATA IN\n");
#ifdef USE_USBD_COMPOSITE
  /* Get the Endpoints addresses allocated for this class instance */
  MIDIInEpAdd  = USBD_CoreGetEPAdd(pdev, USBD_EP_IN, USBD_EP_TYPE_BULK);
#endif /* USE_USBD_COMPOSITE */

  hmidi = (USBD_MIDI_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

  if (hmidi == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  /* Ensure that the FIFO is empty before a new transfer, this condition could 
     be caused by a new transfer before the end of the previous transfer */
  if (epnum == MIDIInEpAdd)
  {
    ((USBD_MIDI_HandleTypeDef *)pdev->pClassData)->state = MIDI_IDLE;
  }

  return (uint8_t)USBD_OK;
}

void writeMidi(USBD_HandleTypeDef *pdev)
{
	const uint8_t virtualCable = 0;
	const uint8_t channel = 0;
	const uint8_t controlChannel = 12;
	const uint8_t value = 42;

	//                             cable | 0xB
	const uint8_t usbFrame = (virtualCable << 4) | 0x0B;

	//MIDI command for CC message: 4 bit | 4 bit
	//                             0xB   | midi channel
	const uint8_t midiCommand = 0xB0 | (channel > 0xF ? 0xF : channel);

	uint8_t buffer[12];

	//just random shit so i can see something on the midi receiver
	buffer[0] = usbFrame;
	buffer[1] = midiCommand;
	buffer[2] = controlChannel > 119 ? 119 : controlChannel;
	buffer[3] = value > 127 ? 127 : value;

	buffer[4] = usbFrame;
	buffer[5] = midiCommand;
	buffer[6] = controlChannel > 119 ? 119 : controlChannel;
	buffer[7] = value > 127 ? 127 : value;

	buffer[8] = usbFrame;
	buffer[9] = midiCommand;
	buffer[10] = controlChannel > 119 ? 119 : controlChannel;
	buffer[11] = value > 127 ? 127 : value;

    switch(USBD_LL_Transmit (pdev, MIDI_IN_EP,  buffer, 8))
    {
    case USBD_OK:
    	printf("ok");
		break;
    case USBD_FAIL:
    	printf("fail\n");
		break;
    case USBD_BUSY:
    	printf("busy\n");
    	break;
    default:
		printf("default\n");

    }
}

/**
  * @brief  USBD_MIDI_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_MIDI_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_MIDI_HandleTypeDef *hmidi;
  
#ifdef USE_USBD_COMPOSITE
  /* Get the Endpoints addresses allocated for this class instance */
  MIDIOutEpAdd  = USBD_CoreGetEPAdd(pdev, USBD_EP_OUT, USBD_EP_TYPE_ISOC);
#endif /* USE_USBD_COMPOSITE */

  hmidi = (USBD_MIDI_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

  if (hmidi == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  if (epnum == MIDIOutEpAdd)
  {
    USBD_MIDI_DataInHandler(usb_rx_buffer, MIDI_EPOUT_SIZE);
  
    memset(usb_rx_buffer, 0, MIDI_EPOUT_SIZE);

    ///* Get received data packet length */
    //PacketSize = (uint16_t)USBD_LL_GetRxDataSize(pdev, epnum);
//
    ///* Packet received Callback */
    //((USBD_MIDI_ItfTypeDef *)pdev->pUserData[pdev->classId])->PeriodicTC(&hmidi->buffer[hmidi->wr_ptr],
    //                                                                      PacketSize, MIDI_OUT_TC);
//
    ///* Increment the Buffer pointer or roll it back when all buffers are full */
    //hmidi->wr_ptr += PacketSize;
//
    //if (hmidi->wr_ptr == MIDI_TOTAL_BUF_SIZE)
    //{
    //  /* All buffers are full: roll back */
    //  hmidi->wr_ptr = 0U;
//
    //  if (hmidi->offset == MIDI_OFFSET_UNKNOWN)
    //  {
    //    ((USBD_MIDI_ItfTypeDef *)pdev->pUserData[pdev->classId])->MidiCmd(&hmidi->buffer[0],
    //                                                                        MIDI_TOTAL_BUF_SIZE / 2U,
    //                                                                        MIDI_CMD_START);
    //    hmidi->offset = MIDI_OFFSET_NONE;
    //  }
    //}
//
    //if (hmidi->rd_enable == 0U)
    //{
    //  if (hmidi->wr_ptr == (MIDI_TOTAL_BUF_SIZE / 2U))
    //  {
    //    hmidi->rd_enable = 1U;
    //  }
    //}

    /* Prepare Out endpoint to receive next midi packet */
    //(void)USBD_LL_PrepareReceive(pdev, MIDIOutEpAdd,
    //                             &hmidi->buffer[hmidi->wr_ptr],
    //                             MIDI_OUT_PACKET_SIZE);

    USBD_LL_PrepareReceive(pdev, MIDIOutEpAdd, usb_rx_buffer, MIDI_EPOUT_SIZE); 
  }

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MIDI_DataInHandler
  * @param  usb_rx_buffer: midi messages buffer
  * @param  usb_rx_buffer_length: midi messages buffer length
  */
__weak extern void USBD_MIDI_DataInHandler(uint8_t * usb_rx_buffer, uint8_t usb_rx_buffer_length)
{
  printf("MIDI DATA IN HANDLER\n");
  // For user implementation.
}

#ifndef USE_USBD_COMPOSITE
/**
  * @brief  DeviceQualifierDescriptor
  *         return Device Qualifier descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_MIDI_GetDeviceQualifierDesc(uint16_t *length)
{
  printf("MIDI GET DEVICE QUALIFIER\n");

  *length = (uint16_t)sizeof(USBD_MIDI_DeviceQualifierDesc);

  return USBD_MIDI_DeviceQualifierDesc;
}
#endif /* USE_USBD_COMPOSITE  */

/**
  * @brief  USBD_MIDI_RegisterInterface
  * @param  pdev: device instance
  * @param  fops: Midi interface callback
  * @retval status
  */
uint8_t USBD_MIDI_RegisterInterface(USBD_HandleTypeDef *pdev,
                                     USBD_MIDI_ItfTypeDef *fops)
{
  if (fops == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  pdev->pUserData[pdev->classId] = fops;

  return (uint8_t)USBD_OK;
}

#ifdef USE_USBD_COMPOSITE
/**
  * @brief  USBD_MIDI_GetEpPcktSze
  * @param  pdev: device instance (reserved for future use)
  * @param  If: Interface number (reserved for future use)
  * @param  Ep: Endpoint number (reserved for future use)
  * @retval status
  */
uint32_t USBD_MIDI_GetEpPcktSze(USBD_HandleTypeDef *pdev, uint8_t If, uint8_t Ep)
{
  uint32_t mps;

  UNUSED(pdev);
  UNUSED(If);
  UNUSED(Ep);

  mps = MIDI_PACKET_SZE_WORD(USBD_MIDI_FREQ);

  /* Return the wMaxPacketSize value in Bytes (Freq(Samples)*2(Stereo)*2(HalfWord)) */
  return mps;
}
#endif /* USE_USBD_COMPOSITE */

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */
