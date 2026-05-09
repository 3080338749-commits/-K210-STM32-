/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-11-21     wf          the first version
 */
#include "oled_cfg.h"
#include "stdio.h"
#include "string.h"
static u8g2_t u8g2;

void DisplayCfg_Init()
{
    // Initialization
    u8g2_Setup_ssd1306_i2c_128x64_noname_f( &u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay_rtthread);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_CLOCK, OLED_I2C_PIN_SCL);
    u8x8_SetPin(u8g2_GetU8x8(&u8g2), U8X8_PIN_I2C_DATA, OLED_I2C_PIN_SDA);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
}

void Display_normal(float temp)
{
    rt_uint8_t str_buf[32] = {0};
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
    u8g2_DrawStr(&u8g2, 8, 14, "  TEMP TEST  ");
    sprintf((char *)str_buf, "%0.2f", temp);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_te);
    u8g2_DrawStr(&u8g2, 26, 48, (const char *)str_buf);
    u8g2_SendBuffer(&u8g2);
}
//INIT_APP_EXPORT(u8g2_ssd1306_12864_sw_i2c_example);
