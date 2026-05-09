/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-01-21     dev                 the first version
 */
#ifndef APPLICATIONS_PWM_CFG_H_
#define APPLICATIONS_PWM_CFG_H_

#include <rtthread.h>
#include <rtdevice.h>

#define PWM_DEV_NAME        "pwm3"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL     4       /* PWM通道 */

struct rt_device_pwm *pwm_dev;      /* PWM设备句柄 */

int pwm_init(void);

#endif /* APPLICATIONS_PWM_CFG_H_ */
