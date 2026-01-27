/*!
    \file    gd32f30x_libopt.h
    \brief   library optional for gd32f30x

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef GD32F30X_LIBOPT_H
#define GD32F30X_LIBOPT_H

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed. 
  *         If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */    

//stm32 defines

/** 
  * @brief Universal Serial Bus Full Speed Device
  */
  
  typedef struct
  {
    __IO uint16_t EP0R;            /*!< USB Endpoint 0 register,                Address offset: 0x00 */ 
    __IO uint16_t RESERVED0;       /*!< Reserved */     
    __IO uint16_t EP1R;            /*!< USB Endpoint 1 register,                Address offset: 0x04 */
    __IO uint16_t RESERVED1;       /*!< Reserved */       
    __IO uint16_t EP2R;            /*!< USB Endpoint 2 register,                Address offset: 0x08 */
    __IO uint16_t RESERVED2;       /*!< Reserved */       
    __IO uint16_t EP3R;            /*!< USB Endpoint 3 register,                Address offset: 0x0C */ 
    __IO uint16_t RESERVED3;       /*!< Reserved */       
    __IO uint16_t EP4R;            /*!< USB Endpoint 4 register,                Address offset: 0x10 */
    __IO uint16_t RESERVED4;       /*!< Reserved */       
    __IO uint16_t EP5R;            /*!< USB Endpoint 5 register,                Address offset: 0x14 */
    __IO uint16_t RESERVED5;       /*!< Reserved */       
    __IO uint16_t EP6R;            /*!< USB Endpoint 6 register,                Address offset: 0x18 */
    __IO uint16_t RESERVED6;       /*!< Reserved */       
    __IO uint16_t EP7R;            /*!< USB Endpoint 7 register,                Address offset: 0x1C */
    __IO uint16_t RESERVED7[17];   /*!< Reserved */     
    __IO uint16_t CNTR;            /*!< Control register,                       Address offset: 0x40 */
    __IO uint16_t RESERVED8;       /*!< Reserved */       
    __IO uint16_t ISTR;            /*!< Interrupt status register,              Address offset: 0x44 */
    __IO uint16_t RESERVED9;       /*!< Reserved */       
    __IO uint16_t FNR;             /*!< Frame number register,                  Address offset: 0x48 */
    __IO uint16_t RESERVEDA;       /*!< Reserved */       
    __IO uint16_t DADDR;           /*!< Device address register,                Address offset: 0x4C */
    __IO uint16_t RESERVEDB;       /*!< Reserved */       
    __IO uint16_t BTABLE;          /*!< Buffer Table address register,          Address offset: 0x50 */
    __IO uint16_t RESERVEDC;       /*!< Reserved */
  } USB_TypeDef;

  typedef struct
{
  __IO uint32_t CTL0;       /* Port control register 0 (GPIOx_CTL0, x=A..G)          Address offset: 0x00      */
  __IO uint32_t CTL1;       /* Port control register 1 (GPIOx_CTL1, x=A..G)          Address offset: 0x04      */
  __IO uint32_t ISTAT;      /* Port input status register (GPIOx_ISTAT, x=A..G       Address offset: 0x08      */
  __IO uint32_t OCTL;       /* Port output control register (GPIOx_OCTL, x=A..G)     Address offset: 0x0C      */
  __IO uint32_t BOP;        /* Port bit operate register (GPIOx_BOP, x=A..G)         Address offset: 0x10      */
  __IO uint32_t BC;         /* Port bit clear register (GPIOx_BC, x=A..G)            Address offset: 0x14      */
  __IO uint32_t LOCK;       /* Port configuration lock register (GPIOx_LOCK, x=A..G) Address offset: 0x18      */
  __IO uint32_t RSV[8];     /* Reserved Address offset: 0x1C-0x38                                              */
  __IO uint32_t SPD;        /* Port bit speed register (GPIOx_ SPD, x=A..G)          Address offset: 0x3C      */
} GPIO_TypeDef;

#define HAL_MODULE_ENABLED
#define HAL_PCD_MODULE_ENABLED
#define USE_HAL_PCD_REGISTER_CALLBACKS          0U                          /* PCD register callback disabled       */
#define USB                                     ((USB_TypeDef *) USBD_BASE)
#define UID_BASE                                0x1FFFF7E8UL                /*!< Unique device ID register base address */
#define UNUSED(X)                               (void)X                     /* To avoid gcc/g++ warnings */

/******************************************************************************/
/*                                                                            */
/*                         USB Device General registers                       */
/*                                                                            */
/******************************************************************************/
#define USB_CNTR                             (USBD_BASE + 0x40U)             /*!< Control register */
#define USB_ISTR                             (USBD_BASE + 0x44U)             /*!< Interrupt status register */
#define USB_FNR                              (USBD_BASE + 0x48U)             /*!< Frame number register */
#define USB_DADDR                            (USBD_BASE + 0x4CU)             /*!< Device address register */
#define USB_BTABLE                           (USBD_BASE + 0x50U)             /*!< Buffer Table address register */

/****************************  ISTR interrupt events  *************************/
#define USB_ISTR_CTR                         ((uint16_t)0x8000U)               /*!< Correct TRansfer (clear-only bit) */
#define USB_ISTR_PMAOVR                      ((uint16_t)0x4000U)               /*!< DMA OVeR/underrun (clear-only bit) */
#define USB_ISTR_ERR                         ((uint16_t)0x2000U)               /*!< ERRor (clear-only bit) */
#define USB_ISTR_WKUP                        ((uint16_t)0x1000U)               /*!< WaKe UP (clear-only bit) */
#define USB_ISTR_SUSP                        ((uint16_t)0x0800U)               /*!< SUSPend (clear-only bit) */
#define USB_ISTR_RESET                       ((uint16_t)0x0400U)               /*!< RESET (clear-only bit) */
#define USB_ISTR_SOF                         ((uint16_t)0x0200U)               /*!< Start Of Frame (clear-only bit) */
#define USB_ISTR_ESOF                        ((uint16_t)0x0100U)               /*!< Expected Start Of Frame (clear-only bit) */
#define USB_ISTR_DIR                         ((uint16_t)0x0010U)               /*!< DIRection of transaction (read-only bit)  */
#define USB_ISTR_EP_ID                       ((uint16_t)0x000FU)               /*!< EndPoint IDentifier (read-only bit)  */

/* Legacy defines */
#define USB_ISTR_PMAOVRM USB_ISTR_PMAOVR

#define USB_CLR_CTR                          (~USB_ISTR_CTR)             /*!< clear Correct TRansfer bit */
#define USB_CLR_PMAOVR                       (~USB_ISTR_PMAOVR)          /*!< clear DMA OVeR/underrun bit*/
#define USB_CLR_ERR                          (~USB_ISTR_ERR)             /*!< clear ERRor bit */
#define USB_CLR_WKUP                         (~USB_ISTR_WKUP)            /*!< clear WaKe UP bit */
#define USB_CLR_SUSP                         (~USB_ISTR_SUSP)            /*!< clear SUSPend bit */
#define USB_CLR_RESET                        (~USB_ISTR_RESET)           /*!< clear RESET bit */
#define USB_CLR_SOF                          (~USB_ISTR_SOF)             /*!< clear Start Of Frame bit */
#define USB_CLR_ESOF                         (~USB_ISTR_ESOF)            /*!< clear Expected Start Of Frame bit */

/* Legacy defines */
#define USB_CLR_PMAOVRM USB_CLR_PMAOVR

/*************************  CNTR control register bits definitions  ***********/
#define USB_CNTR_CTRM                        ((uint16_t)0x8000U)               /*!< Correct TRansfer Mask */
#define USB_CNTR_PMAOVR                      ((uint16_t)0x4000U)               /*!< DMA OVeR/underrun Mask */
#define USB_CNTR_ERRM                        ((uint16_t)0x2000U)               /*!< ERRor Mask */
#define USB_CNTR_WKUPM                       ((uint16_t)0x1000U)               /*!< WaKe UP Mask */
#define USB_CNTR_SUSPM                       ((uint16_t)0x0800U)               /*!< SUSPend Mask */
#define USB_CNTR_RESETM                      ((uint16_t)0x0400U)               /*!< RESET Mask   */
#define USB_CNTR_SOFM                        ((uint16_t)0x0200U)               /*!< Start Of Frame Mask */
#define USB_CNTR_ESOFM                       ((uint16_t)0x0100U)               /*!< Expected Start Of Frame Mask */
#define USB_CNTR_RESUME                      ((uint16_t)0x0010U)               /*!< RESUME request */
#define USB_CNTR_FSUSP                       ((uint16_t)0x0008U)               /*!< Force SUSPend */
#define USB_CNTR_LPMODE                      ((uint16_t)0x0004U)               /*!< Low-power MODE */
#define USB_CNTR_PDWN                        ((uint16_t)0x0002U)               /*!< Power DoWN */
#define USB_CNTR_FRES                        ((uint16_t)0x0001U)               /*!< Force USB RESet */

/* Legacy defines */
#define USB_CNTR_PMAOVRM USB_CNTR_PMAOVR
#define USB_CNTR_LP_MODE USB_CNTR_LPMODE

/********************  FNR Frame Number Register bit definitions   ************/
#define USB_FNR_RXDP                         ((uint16_t)0x8000U)               /*!< status of D+ data line */
#define USB_FNR_RXDM                         ((uint16_t)0x4000U)               /*!< status of D- data line */
#define USB_FNR_LCK                          ((uint16_t)0x2000U)               /*!< LoCKed */
#define USB_FNR_LSOF                         ((uint16_t)0x1800U)               /*!< Lost SOF */
#define USB_FNR_FN                           ((uint16_t)0x07FFU)               /*!< Frame Number */

/********************  DADDR Device ADDRess bit definitions    ****************/
#define USB_DADDR_EF                         ((uint8_t)0x80U)                  /*!< USB device address Enable Function */
#define USB_DADDR_ADD                        ((uint8_t)0x7FU)                  /*!< USB device address */

/******************************  Endpoint register    *************************/
#define USB_EP0R                             USBD_BASE                    /*!< endpoint 0 register address */
#define USB_EP1R                             (USBD_BASE + 0x04U)           /*!< endpoint 1 register address */
#define USB_EP2R                             (USBD_BASE + 0x08U)           /*!< endpoint 2 register address */
#define USB_EP3R                             (USBD_BASE + 0x0CU)           /*!< endpoint 3 register address */
#define USB_EP4R                             (USBD_BASE + 0x10U)           /*!< endpoint 4 register address */
#define USB_EP5R                             (USBD_BASE + 0x14U)           /*!< endpoint 5 register address */
#define USB_EP6R                             (USBD_BASE + 0x18U)           /*!< endpoint 6 register address */
#define USB_EP7R                             (USBD_BASE + 0x1CU)           /*!< endpoint 7 register address */
/* bit positions */ 
#define USB_EP_CTR_RX                        ((uint16_t)0x8000U)               /*!<  EndPoint Correct TRansfer RX */
#define USB_EP_DTOG_RX                       ((uint16_t)0x4000U)               /*!<  EndPoint Data TOGGLE RX */
#define USB_EPRX_STAT                        ((uint16_t)0x3000U)               /*!<  EndPoint RX STATus bit field */
#define USB_EP_SETUP                         ((uint16_t)0x0800U)               /*!<  EndPoint SETUP */
#define USB_EP_T_FIELD                       ((uint16_t)0x0600U)               /*!<  EndPoint TYPE */
#define USB_EP_KIND                          ((uint16_t)0x0100U)               /*!<  EndPoint KIND */
#define USB_EP_CTR_TX                        ((uint16_t)0x0080U)               /*!<  EndPoint Correct TRansfer TX */
#define USB_EP_DTOG_TX                       ((uint16_t)0x0040U)               /*!<  EndPoint Data TOGGLE TX */
#define USB_EPTX_STAT                        ((uint16_t)0x0030U)               /*!<  EndPoint TX STATus bit field */
#define USB_EPADDR_FIELD                     ((uint16_t)0x000FU)               /*!<  EndPoint ADDRess FIELD */

/* EndPoint REGister MASK (no toggle fields) */
#define USB_EPREG_MASK     (USB_EP_CTR_RX|USB_EP_SETUP|USB_EP_T_FIELD|USB_EP_KIND|USB_EP_CTR_TX|USB_EPADDR_FIELD)
                                                                               /*!< EP_TYPE[1:0] EndPoint TYPE */
#define USB_EP_TYPE_MASK                     ((uint16_t)0x0600U)               /*!< EndPoint TYPE Mask */
#define USB_EP_BULK                          ((uint16_t)0x0000U)               /*!< EndPoint BULK */
#define USB_EP_CONTROL                       ((uint16_t)0x0200U)               /*!< EndPoint CONTROL */
#define USB_EP_ISOCHRONOUS                   ((uint16_t)0x0400U)               /*!< EndPoint ISOCHRONOUS */
#define USB_EP_INTERRUPT                     ((uint16_t)0x0600U)               /*!< EndPoint INTERRUPT */
#define USB_EP_T_MASK                        ((uint16_t) ~USB_EP_T_FIELD & USB_EPREG_MASK)
                                                                 
#define USB_EPKIND_MASK                      ((uint16_t) ~USB_EP_KIND & USB_EPREG_MASK)            /*!< EP_KIND EndPoint KIND */
                                                                               /*!< STAT_TX[1:0] STATus for TX transfer */
#define USB_EP_TX_DIS                        ((uint16_t)0x0000U)               /*!< EndPoint TX DISabled */
#define USB_EP_TX_STALL                      ((uint16_t)0x0010U)               /*!< EndPoint TX STALLed */
#define USB_EP_TX_NAK                        ((uint16_t)0x0020U)               /*!< EndPoint TX NAKed */
#define USB_EP_TX_VALID                      ((uint16_t)0x0030U)               /*!< EndPoint TX VALID */
#define USB_EPTX_DTOG1                       ((uint16_t)0x0010U)               /*!< EndPoint TX Data TOGgle bit1 */
#define USB_EPTX_DTOG2                       ((uint16_t)0x0020U)               /*!< EndPoint TX Data TOGgle bit2 */
#define USB_EPTX_DTOGMASK  (USB_EPTX_STAT|USB_EPREG_MASK)
                                                                               /*!< STAT_RX[1:0] STATus for RX transfer */
#define USB_EP_RX_DIS                        ((uint16_t)0x0000U)               /*!< EndPoint RX DISabled */
#define USB_EP_RX_STALL                      ((uint16_t)0x1000U)               /*!< EndPoint RX STALLed */
#define USB_EP_RX_NAK                        ((uint16_t)0x2000U)               /*!< EndPoint RX NAKed */
#define USB_EP_RX_VALID                      ((uint16_t)0x3000U)               /*!< EndPoint RX VALID */
#define USB_EPRX_DTOG1                       ((uint16_t)0x1000U)               /*!< EndPoint RX Data TOGgle bit1 */
#define USB_EPRX_DTOG2                       ((uint16_t)0x2000U)               /*!< EndPoint RX Data TOGgle bit1 */
#define USB_EPRX_DTOGMASK  (USB_EPRX_STAT|USB_EPREG_MASK)

#include "gd32f30x_rcu.h"
// #include "gd32f30x_adc.h"
//#include "gd32f30x_can.h"
// #include "gd32f30x_crc.h"
// #include "gd32f30x_ctc.h"
// #include "gd32f30x_dac.h"
#include "gd32f30x_dbg.h"
#include "gd32f30x_dma.h"
#include "gd32f30x_exti.h"
#include "gd32f30x_fmc.h"
// #include "gd32f30x_fwdgt.h"
#include "gd32f30x_gpio.h"
#include "gd32f30x_i2c.h"
#include "gd32f30x_pmu.h"
// #include "gd32f30x_bkp.h"
// #include "gd32f30x_rtc.h"
#include "gd32f30x_sdio.h"
#include "gd32f30x_spi.h"
#include "gd32f30x_timer.h"
#include "gd32f30x_usart.h"
// #include "gd32f30x_wwdgt.h"
#include "gd32f30x_misc.h"
// #include "gd32f30x_enet.h"
// #include "gd32f30x_exmc.h"
#include "stm32f3xx_hal_pcd.h"
#endif /* GD32F30X_LIBOPT_H */
