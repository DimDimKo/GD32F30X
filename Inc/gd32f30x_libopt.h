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

#define UNUSED(X)                               (void)X                     /* To avoid gcc/g++ warnings */

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
#endif /* GD32F30X_LIBOPT_H */
