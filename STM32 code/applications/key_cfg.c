/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-01-20     dev                  the first version
 */
#include "key_cfg.h"
#include "uart_cfg.h"
void add_face(void *args)
{
    rt_kprintf("add_face cmd send successful!\n");
    const rt_uint8_t add_face_cmd[4] = {0xA1, 0x02, 0x00, 0x1A};
    rt_device_write(serial2, 0, add_face_cmd, 4);
}

void del_face(void *args)
{
    rt_kprintf("del_face cmd send successful!\n");
    const rt_uint8_t add_del_cmd[4] = {0xA1, 0x01, 0x02, 0x1A};
    rt_device_write(serial2, 0, add_del_cmd, 4);
}

void io_init(void)
{
    /* 蜂鸣器引脚为输出模式 */
    rt_pin_mode(BEEP_PIN_NUM, PIN_MODE_OUTPUT);
    /* 默认低电平 */
    rt_pin_write(BEEP_PIN_NUM, PIN_LOW);

    /* 按键0引脚为输入模式 */
    rt_pin_mode(KEY0_PIN_NUM, PIN_MODE_INPUT_PULLUP);
    /* 绑定中断，下降沿模式，回调函数名为beep_on */
    rt_pin_attach_irq(KEY0_PIN_NUM, PIN_IRQ_MODE_FALLING, add_face, RT_NULL);
    /* 使能中断 */
    rt_pin_irq_enable(KEY0_PIN_NUM, PIN_IRQ_ENABLE);

    /* 按键1引脚为输入模式 */
    rt_pin_mode(KEY1_PIN_NUM, PIN_MODE_INPUT_PULLUP);
    /* 绑定中断，下降沿模式，回调函数名为beep_off */
    rt_pin_attach_irq(KEY1_PIN_NUM, PIN_IRQ_MODE_FALLING, del_face, RT_NULL);
    /* 使能中断 */
    rt_pin_irq_enable(KEY1_PIN_NUM, PIN_IRQ_ENABLE);
}
