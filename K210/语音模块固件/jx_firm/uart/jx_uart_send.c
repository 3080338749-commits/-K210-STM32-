#include "jx_uart_send.h"

// 串口通信消息头
const unsigned char g_uart_send_head[] = {
  0xaa, 0x55
};

// 串口通信消息尾
const unsigned char g_uart_send_foot[] = {
  0x55, 0xaa
};

// 串口发送函数实现
void _uart_send_impl(unsigned char* buff, int len) {
  // TODO: 调用项目实际的串口发送函数
  /*
  int i = 0;
  unsigned char c;
  for (i = 0; i < len; i++) {
    c = buff[i];
    printf("%02X ", c);
  }
  printf("\n");
  */
}

// action: LoginSucc
void _uart_LoginSucc() {
  uart_param_t param;
  int i = 0;
  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++) {
      buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_LoginSucc;
  for (i = 0; i < UART_MSG_FOOT_LEN; i++) {
      buff[i + 3] = g_uart_send_foot[i];
  }
  _uart_send_impl(buff, 5);
}

// action: FindSucc
void _uart_FindSucc() {
  uart_param_t param;
  int i = 0;
  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++) {
      buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_FindSucc;
  for (i = 0; i < UART_MSG_FOOT_LEN; i++) {
      buff[i + 3] = g_uart_send_foot[i];
  }
  _uart_send_impl(buff, 5);
}

// action: DelSucc
void _uart_DelSucc() {
  uart_param_t param;
  int i = 0;
  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++) {
      buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_DelSucc;
  for (i = 0; i < UART_MSG_FOOT_LEN; i++) {
      buff[i + 3] = g_uart_send_foot[i];
  }
  _uart_send_impl(buff, 5);
}

// action: TempErr
void _uart_TempErr() {
  uart_param_t param;
  int i = 0;
  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++) {
      buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_TempErr;
  for (i = 0; i < UART_MSG_FOOT_LEN; i++) {
      buff[i + 3] = g_uart_send_foot[i];
  }
  _uart_send_impl(buff, 5);
}

