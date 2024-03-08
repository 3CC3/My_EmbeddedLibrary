//
// Created by 86159 on 2023-09-30.
//

#ifndef MY_FLYBACK_UART_RXPACKAGE_H
#define MY_FLYBACK_UART_RXPACKAGE_H

#include "main.h"
#include "stdlib.h"
typedef enum
{
    My_Error = 0,
    My_Success,
}Satus;
void Uart_RxPackage_Init(UartRx_Package *Uart_PackageHandle,uint16_t MAX_RxLength,UART_HandleTypeDef *huart);
Satus Uart_UpPack(char* InputString,uint16_t StringLength,char *OutputString);
#endif //MY_FLYBACK_UART_RXPACKAGE_H
