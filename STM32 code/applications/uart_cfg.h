/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-01-20     dev                  the first version
 */
#ifndef APPLICATIONS_UART_CFG_H_
#define APPLICATIONS_UART_CFG_H_

#include <rtthread.h>

extern uint8_t face_check_flag;

typedef struct {
    rt_uint8_t rx_buf[20];
    rt_uint8_t rx_cnt;
}UART_RX_typedef;

UART_RX_typedef uart2_rx_info;
#define SAMPLE_UART2_NAME       "uart2"
/* 用于接收消息的信号量 */
struct rt_semaphore uart2_rx_sem;
rt_device_t serial2;

#define SAMPLE_UART3_NAME       "uart3"
rt_device_t serial3;

int uart2_init(void);
int uart3_init(void);

#endif /* APPLICATIONS_UART_CFG_H_ */
