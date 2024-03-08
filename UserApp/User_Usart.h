//
// Created by 86159 on 2023-07-19.
//

#ifndef DUAL_BLUETOOTH_CONNECTION_USER_USART_H
#define DUAL_BLUETOOTH_CONNECTION_USER_USART_H

#include "main.h"

typedef struct Uart_print
{
    char *name; //记录串口名称
    uint16_t Rx_cnt; //共接收到的字节数
    uint8_t Rx_temp[1]; //用于接收单个串口传来的数据
    uint8_t Rx_statrtFlag : 1; //由于这是个标志位，所以仅需要一个bit就可以
    uint8_t Rx_finishFlag : 1;//由于这是个标志位，所以仅需要一个bit就可以
    uint8_t Rx_package[100];
    uint32_t tick_old;
    uint32_t tick_new;
}Uart_print;
void Uart_creat(Uart_print *Uart_print,char *name);
void Uart2print(UART_HandleTypeDef *huart,Uart_print *Uart_Print);
#endif //DUAL_BLUETOOTH_CONNECTION_USER_USART_H
