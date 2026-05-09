/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-11-21     sz          the first version
 */
#ifndef APPLICATIONS_DISPLAY_CFG_H_
#define APPLICATIONS_DISPLAY_CFG_H_

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <u8g2_port.h>
#include "drv_common.h"

#define OLED_I2C_PIN_SCL                   GET_PIN(B, 6)
#define OLED_I2C_PIN_SDA                   GET_PIN(B, 7)


void DisplayCfg_Init();

void Display_normal(float temp);
#endif /* APPLICATIONS_DISPLAY_CFG_H_ */
