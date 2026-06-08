/*

  usb_serial.c - USB serial port implementation for STM32F3xx ARM processors

  Part of grblHAL

  Copyright (c) 2019-2024 Terje Io

  grblHAL is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  grblHAL is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with grblHAL. If not, see <http://www.gnu.org/licenses/>.

*/

#include "driver.h"
#include "usb_serial.h"
#include "grbl/hal.h"
#include "grbl/protocol.h"
#include "main.h"

#if USB_SERIAL_CDC

#include "cdc_acm_core.h"
#include "usbd_lld_regs.h"
#include "usbd_lld_int.h"
#include "usbd_lld_core.h"
#include "usbd_transc.h"
#include "usbd_core.h"
usb_dev usbd_cdc;

static stream_rx_buffer_t rxbuf = {0};
static stream_block_tx_buffer2_t txbuf = {0};
static enqueue_realtime_command_ptr enqueue_realtime_command = protocol_enqueue_realtime_command;
volatile usb_linestate_t usb_linestate = {0};

static uint8_t usbserial_send(usb_dev *udev, uint8_t* pBuf, uint16_t Len);

static bool is_connected (void)
{
#ifdef CDC_TYPE_STM32
    return usb_linestate.pin.dtr && hal.get_elapsed_ticks() - usb_linestate.timestamp >= 15;
#else
    return usb_linestate.pin.dtr && hal.get_elapsed_ticks() - usb_linestate.timestamp >= 15;
#endif
}

//
// Returns number of free characters in the input buffer
//
static uint16_t usbRxFree (void)
{
    uint16_t tail = rxbuf.tail, head = rxbuf.head;
    return RX_BUFFER_SIZE - BUFCOUNT(head, tail, RX_BUFFER_SIZE);
}

//
// Flushes the input buffer
//
static void usbRxFlush (void)
{
     rxbuf.tail = rxbuf.head;
}

//
// Flushes and adds a CAN character to the input buffer
//
static void usbRxCancel (void)
{
    rxbuf.data[rxbuf.head] = ASCII_CAN;
    rxbuf.tail = rxbuf.head;
    rxbuf.head = BUFNEXT(rxbuf.head, rxbuf);
}

//
// Writes current buffer to the USB output stream, swaps buffers
//
static inline bool usb_write (void)
{
    txbuf.s = txbuf.use_tx2data ? txbuf.data2 : txbuf.data;

    while(usbserial_send(&usbd_cdc, (uint8_t *)txbuf.s, txbuf.length) == USBD_BUSY) {
        if(!hal.stream_blocking_callback())
            return false;
    }

    txbuf.use_tx2data = !txbuf.use_tx2data;
    txbuf.s = txbuf.use_tx2data ? txbuf.data2 : txbuf.data;
    txbuf.length = 0;
    return true;
}

//
// Writes a single character to the USB output stream, blocks if buffer full
//
static bool usbPutC (const uint8_t c)
{
    static uint8_t buf[1];
    *buf = c;
        while(usbserial_send(&usbd_cdc, buf, 1) == USBD_BUSY) {
            if(!hal.stream_blocking_callback())
                return false;
        }
    return true;
}

//
// Writes a null terminated string to the USB output stream, blocks if buffer full
// Buffers string up to EOL (LF) before transmitting
//
static void usbWriteS (const char *s)
{
    size_t length = strlen(s);
    if(length == 0)
        return;
    if(txbuf.length && (txbuf.length + length) > txbuf.max_length) {
        if(!usb_write())
            return;
    }
    while(length > txbuf.max_length) {
        txbuf.length = txbuf.max_length;
        memcpy(txbuf.s, s, txbuf.length);
        if(!usb_write())
            return;
        length -= txbuf.max_length;
        s += txbuf.max_length;
    }
    if(length) {
        memcpy(txbuf.s, s, length);
        txbuf.length += length;
        txbuf.s += length;
        if(s[length - 1] == ASCII_LF)
            usb_write();
    }
}

//
// Writes a number of characters from string to the USB output stream, blocks if buffer full
//
static void usbWrite (const uint8_t *s, uint16_t length)
{
    if(length == 0)
        return;

    if(txbuf.length && (txbuf.length + length) > txbuf.max_length) {
        if(!usb_write())
            return;
    }
    while(length > txbuf.max_length) {
        txbuf.length = txbuf.max_length;
        memcpy(txbuf.s, s, txbuf.length);
        if(!usb_write())
            return;
        length -= txbuf.max_length;
        s += txbuf.max_length;
    }
    if(length) {
        memcpy(txbuf.s, s, length);
        txbuf.length += length;
        txbuf.s += length;
        usb_write();
    }
}

//
// usbGetC - returns -1 if no data available
//
static int32_t usbGetC (void)
{
    uint_fast16_t tail = rxbuf.tail;    // Get buffer pointer
    if(tail == rxbuf.head)
        return -1; // no data available
    char data = rxbuf.data[tail];       // Get next character
    rxbuf.tail = BUFNEXT(tail, rxbuf);  // and update pointer
    return (int16_t)data;
}

static bool usbSuspendInput (bool suspend)
{
    return stream_rx_suspend(&rxbuf, suspend);
}

static bool usbEnqueueRtCommand (uint8_t c)
{
    return enqueue_realtime_command(c);
}

static enqueue_realtime_command_ptr usbSetRtHandler (enqueue_realtime_command_ptr handler)
{
    enqueue_realtime_command_ptr prev = enqueue_realtime_command;
    if(handler)
        enqueue_realtime_command = handler;
    return prev;
}

static void gd32_cdc_init(void)
{
    /* Force USB re-enumeration */
    NVIC_DisableIRQ(USBD_LP_CAN0_RX0_IRQn);
    rcu_periph_clock_disable(RCU_USBD);
    uint32_t treg = GPIO_CTL1(GPIOA);
    treg &= ~(GPIO_MODE_MASK(USBD_DP_PIN&7U) | GPIO_MODE_MASK(USBD_DM_PIN&7U));
    treg |= 0x34000; // Set A12 Pin mode to OUT_PP
    GPIO_CTL1(GPIOA) = treg;
    GPIO_BC(GPIOA)    = GPIO_BC_CR12;
    for (int i=0; i<0xFFFF; i++) {
        __NOP();
    }
    treg &= ~GPIO_MODE_MASK(USBD_DP_PIN&7U);
    GPIO_CTL1(GPIOA) = treg | GPIO_MODE_SET(USBD_DP_PIN&7, GPIO_MODE_IN_FLOATING);
    /* RCU configure */
    uint32_t system_clock = rcu_clock_freq_get(CK_SYS);
    if(48000000U == system_clock) {
        rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV1);
    } else if(72000000U == system_clock) {
        rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV1_5);
    } else if(96000000U == system_clock) {
        rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV2);
    } else if(120000000U == system_clock) {
        rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV2_5);
    }
    // rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV2_5); //Hardcoded for CK_SYS=120M
    rcu_periph_clock_enable(RCU_USBD);
    /* USB device configuration */
    usbd_init(&usbd_cdc, &cdc_desc, &cdc_class);
    /* 2 bits for preemption priority, 2 bits for subpriority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    /* enable the USB low priority interrupt */
    nvic_irq_enable((uint8_t)USBD_LP_CAN0_RX0_IRQn, 2U, 0U);
    usbd_connect(&usbd_cdc);
}

// NOTE: USB interrupt priority should be set lower than stepper/step timer to avoid jitter
// It is set in HAL_PCD_MspInit() in usbd_conf.c
const io_stream_t *usbInit (uint32_t baud_rate)
{
    static const io_stream_t stream = {
        .type = StreamType_Serial,
        .state.is_usb = On,
        .state.linestate_event = On,
        .is_connected = is_connected,
        .read = usbGetC,
        .write = usbWriteS,
        .write_char = usbPutC,
        .write_n = usbWrite,
        .enqueue_rt_command = usbEnqueueRtCommand,
        .get_rx_buffer_free = usbRxFree,
        .reset_read_buffer = usbRxFlush,
        .cancel_read_buffer = usbRxCancel,
        .suspend_read = usbSuspendInput,
        .set_enqueue_rt_handler = usbSetRtHandler
    };

    gd32_cdc_init();

    txbuf.s = txbuf.data;
    txbuf.max_length = BLOCK_TX_BUFFER_SIZE;
    return &stream;
}

static uint8_t usbserial_send(usb_dev *udev, uint8_t* pBuf, uint16_t Len)
{
    usb_cdc_handler *cdc = (usb_cdc_handler *)udev->class_data[CDC_COM_INTERFACE];
    if((Len != 0U) && (cdc->packet_sent == 1)) {
        if (is_connected()) {
            cdc->packet_sent = 0U;
            usbd_ep_send(&usbd_cdc, CDC_IN_EP, pBuf, Len);
            cdc->receive_length = 0U;
        }
        return USBD_OK;
    } else {
        return USBD_BUSY;
    }
}

static void WriteC2rxbuf(uint8_t c)
{
    if(!enqueue_realtime_command(c)) {                  // Check and strip realtime commands,
        rxbuf.data[rxbuf.head] = c;
        rxbuf.head = BUFNEXT(rxbuf.head, rxbuf);
    }
}

void ucdc_isr(void)
{
    __IO uint16_t int_status = (uint16_t)USBD_INTF;
    __IO uint16_t int_flag = (uint16_t)(USBD_INTF & USBD_INTEN);
    uint16_t ctl_reg = (uint16_t)(USBD_CTL);

    int_flag &= ctl_reg;

    usb_dev *udev = usbd_core.dev;

    if(INTF_STIF & int_flag) {
        /* wait till interrupts are not pending */
        while((int_status = (uint16_t)USBD_INTF) & (uint16_t)INTF_STIF) {
            /* get endpoint number */
            uint8_t ep_num = (uint8_t)(int_status & INTF_EPNUM);

            if(int_status & INTF_DIR) {
                /* handle the USB OUT direction transaction */
                if(USBD_EPxCS(ep_num) & EPxCS_RX_ST) {
                    /* clear successful receive interrupt flag */
                    USBD_EP_RX_ST_CLEAR(ep_num);
                    if (ep_num == 0) {
                        if(USBD_EPxCS(ep_num) & EPxCS_SETUP) {
                            _usb_setup_transc(udev, ep_num);
                        } else { // handle data ep0 OUT
                            usb_transc *transc = &udev->transc_out[ep_num];
                            uint16_t count = udev->drv_handler->ep_read(transc->xfer_buf, ep_num, (uint8_t)EP_BUF_SNG);
                            //uint16_t count = usbd_ep_data_read(transc->xfer_buf, ep_num, (uint8_t)EP_BUF_SNG);
                            transc->xfer_buf += count;
                            transc->xfer_count += count;
                            if((transc->xfer_count >= transc->xfer_len) || (count < transc->max_len)) {
                                _usb_out0_transc(udev, ep_num);
                            } else {
                                USBD_EP_RX_STAT_SET(EP_ID(ep_num), EPRX_VALID);
                            }
                        }

                    } else {
                        /* handle the USB OUT non-control ep data transaction */
                        uint16_t bytes = (uint16_t)(btable_ep[ep_num].rx_count & EPRCNT_CNT);
                        uint32_t *read_addr = (uint32_t *)(btable_ep[ep_num].rx_addr * 2U + USBD_RAM);
                        if (bytes < (RX_BUFFER_SIZE - BUFCOUNT(rxbuf.head, rxbuf.tail, RX_BUFFER_SIZE))) {
                            // Read PMA to fifo
                            uint16_t read16;
                            for(uint16_t n = 0U; n < bytes / 2U; n++) {
                                read16 = (uint16_t) * read_addr++;
                                WriteC2rxbuf(BYTE_LOW(read16));
                                WriteC2rxbuf(BYTE_HIGH(read16));
                            }
                            if (bytes % 2) {
                                read16 = (uint16_t) * read_addr++;
                                WriteC2rxbuf(BYTE_LOW(read16));
                            }
                            // cdc_acm_data_out
                            USBD_EP_RX_STAT_SET(EP_ID(ep_num), EPRX_VALID);    // Ready to receive next packet
                        } else { //buffer overflow
                            rxbuf.overflow = 1;
                            return;
                        }
                    }
                }
            } else {
                /* handle the USB IN direction transaction */
                if(USBD_EPxCS(ep_num) & EPxCS_TX_ST) {
                    /* clear successful transmit interrupt flag */
                    USBD_EP_TX_ST_CLEAR(ep_num);
                    usb_transc *transc = &udev->transc_in[ep_num];
                    if(0U == transc->xfer_len) {
                        if (udev->ep_transc[ep_num][TRANSC_IN]) {
                            udev->ep_transc[ep_num][TRANSC_IN](udev, ep_num);
                        }
                    } else {
                        usbd_ep_send(udev, ep_num, transc->xfer_buf, transc->xfer_len);
                    }
                }
            }
        }
    }

    if(INTF_WKUPIF & int_flag) {
        /* clear wakeup interrupt flag in INTF */
        CLR(WKUPIF);
        /* restore the old cur_status */
        //udev->cur_status = udev->backup_status;

        /*if(0U == udev->pm.remote_wakeup_on) {
            resume_mcu(udev);
        } */
    }

    if(INTF_SPSIF & int_flag) {
        if(!(USBD_CTL & CTL_RSREQ)) {
            // usbd_int_suspend(udev);
            /* clear of suspend interrupt flag bit must be done after setting of CTLR_SETSPS */
            CLR(SPSIF);
        }
    }

    if(INTF_SOFIF & int_flag) {
        /* clear SOF interrupt flag in INTF */
        CLR(SOFIF);

        /* if necessary, user can add code here */
        //if(NULL != usbd_int_fops) {
        //    (void)usbd_int_fops->SOF(udev);
        //}
    }

    if(INTF_ESOFIF & int_flag) {
        /* clear ESOF interrupt flag in INTF */
        CLR(ESOFIF);

        /* control resume time by ESOFs */
        if(udev->pm.esof_count > 0U) {
            if(0U == --udev->pm.esof_count) {
                if(udev->pm.remote_wakeup_on) {
                    USBD_CTL &= ~CTL_RSREQ;
                    udev->pm.remote_wakeup_on = 0U;
                } else {
                    USBD_CTL |= CTL_RSREQ;
                    udev->pm.esof_count = 3U;
                    udev->pm.remote_wakeup_on = 1U;
                }
            }
        }
    }

    if(INTF_RSTIF & int_flag) {
        /* clear reset interrupt flag in INTF */
        CLR(RSTIF);
        udev->drv_handler->ep_reset(udev);
        //usbd_ep_reset(udev);
    }
}

#endif // USB_SERIAL_CDC