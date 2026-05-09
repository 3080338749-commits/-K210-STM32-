/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-01-20     dev                  the first version
 */
#include "uart_cfg.h"

uint8_t face_check_flag=0;

/* 接收数据回调函数 */
rt_err_t uart2_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&uart2_rx_sem);

    return RT_EOK;
}

void serial2_thread_entry(void *parameter)
{
    char ch;
    static rt_tick_t k210_last_tick=0;
    while (1)
    {
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(serial2, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&uart2_rx_sem, RT_WAITING_FOREVER);
        }
        /* 读取到的数据通过串口错位输出 */
        uart2_rx_info.rx_buf[uart2_rx_info.rx_cnt] = ch;
        if(uart2_rx_info.rx_buf[0] == 0xAF)
        {
            uart2_rx_info.rx_cnt++;
        }else {
            uart2_rx_info.rx_cnt=0;
        }

        if(uart2_rx_info.rx_cnt>3)
        {
            if(uart2_rx_info.rx_buf[1] == 0x05 && uart2_rx_info.rx_buf[2] == 0x0 && uart2_rx_info.rx_buf[3] == 0xFA)
            {
                const uint8_t voice_add_face_succ[5] = {0xAA ,0x55 ,0x01 ,0x55 ,0xAA };
                rt_device_write(serial3, 0, voice_add_face_succ, 5);
                k210_last_tick = rt_tick_get();
            }
            if(uart2_rx_info.rx_buf[1] == 0x02 && uart2_rx_info.rx_buf[3] == 0xFA)
            {
                const uint8_t voice_check_face_succ[5] = {0xAA ,0x55 ,0x02 ,0x55 ,0xAA };
                if(rt_tick_get() - k210_last_tick > 5*1000)
                {
                    rt_device_write(serial3, 0, voice_check_face_succ, 5);
                    k210_last_tick = rt_tick_get();
                    face_check_flag=1;
                }

            }
            if(uart2_rx_info.rx_buf[1] == 0x01 && uart2_rx_info.rx_buf[2] == 0x0 && uart2_rx_info.rx_buf[3] == 0xFA)
            {
                const uint8_t voice_del_face_succ[5] = {0xAA ,0x55 ,0x03 ,0x55 ,0xAA };
                rt_device_write(serial3, 0, voice_del_face_succ, 5);
            }
            uart2_rx_info.rx_cnt=0;
        }
    }
}

int uart2_init(void)
{
    rt_err_t ret = RT_EOK;
    char str[] = "uart2 init successful!\r\n";
    uart2_rx_info.rx_cnt = 0;

    /* 查找系统中的串口设备 */
    serial2 = rt_device_find(SAMPLE_UART2_NAME);
    if (!serial2)
    {
        rt_kprintf("find %s failed!\n", SAMPLE_UART2_NAME);
        return RT_ERROR;
    }

    /* 初始化信号量 */
    rt_sem_init(&uart2_rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial2, RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial2, uart2_input);
    /* 发送字符串 */
    //rt_device_write(serial2, 0, str, (sizeof(str) - 1));

    /* 创建 serial 线程 */
    rt_thread_t uart2_thread = rt_thread_create("serial2", serial2_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (uart2_thread != RT_NULL)
    {
        rt_thread_startup(uart2_thread);
    }
    else
    {
        ret = RT_ERROR;
    }
    rt_kprintf(" %s\n", str);
    return ret;
}

int uart3_init(void)
{
    rt_err_t ret = RT_EOK;
    char str[] = "uart3 init successful!\r\n";

    /* 查找系统中的串口设备 */
    serial3 = rt_device_find(SAMPLE_UART3_NAME);
    if (!serial3)
    {
        rt_kprintf("find %s failed!\n", SAMPLE_UART3_NAME);
        return RT_ERROR;
    }
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial3, RT_DEVICE_FLAG_INT_RX);



    rt_kprintf(" %s\n", str);
    return ret;
}

