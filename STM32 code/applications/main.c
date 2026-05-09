/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-01-20     RT-Thread    first version
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "key_cfg.h"
#include "oled_cfg.h"
#include "GY906.h"
#include "pwm_cfg.h"
#include "uart_cfg.h"

float temp;
int main(void)
{
    static rt_tick_t open_door_tick = 0;
    DisplayCfg_Init();
    GY906_Init();
    io_init();
    uart3_init();
    uart2_init();
    pwm_init();
    while (1)
    {
        temp = SMBus_ReadTemp();
        Display_normal(temp);
        if(temp>37)
        {
            rt_pin_write(BEEP_PIN_NUM, PIN_HIGH);
        }else {
            rt_pin_write(BEEP_PIN_NUM, PIN_LOW);
        }

        if(face_check_flag && temp<37)
        {
            rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, 500000*2*20, 500000*5);
            face_check_flag=0;
            open_door_tick = rt_tick_get();
        }

        if((rt_tick_get()-open_door_tick>7*1000) && (open_door_tick != 0))
        {
            rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, 500000*2*20, 500000);
            open_door_tick=0;
        }

        rt_thread_mdelay(100);
    }

    return RT_EOK;
}
