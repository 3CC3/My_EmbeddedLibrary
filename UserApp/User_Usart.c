//
// Created by 86159 on 2023-07-19.
//


/************************************************************************************************
 * 该代码可以实现打印字符串，但最大只能是100字节，且非DMA打印而是普通的串口打印
 * 2023.7.19
 *
 *************************************************************************************************/
#include "User_Usart.h"

void Uart_creat(Uart_print *Uart_print,char *name)
{
    Uart_print->Rx_cnt = 0;
    Uart_print->Rx_finishFlag = 0;
    Uart_print->Rx_statrtFlag = 0;
    Uart_print->Rx_temp[0] = 0;
    Uart_print->name = name;
    Uart_print->tick_new = 0;
    Uart_print->tick_old = 0;
}
void Uart2print(UART_HandleTypeDef *huart,Uart_print *Uart_Print)
{
    HAL_UART_Transmit(huart,Uart_Print->Rx_package,Uart_Print->Rx_cnt,9999); //输出接收到的数据
    //HAL_UART_Transmit_DMA(huart,Uart_Print->Rx_package,Uart_Print->Rx_cnt);
    //printf("接收到的数据为:%s\r\n",Uart_Print->Rx_package);
    for(uint8_t i = 0;i<Uart_Print->Rx_cnt;i++)
    {
        Uart_Print->Rx_package[i] = 0;
    }
    Uart_Print->Rx_statrtFlag = 0;
    Uart_Print->Rx_cnt = 0;
    Uart_Print->Rx_temp[0] = 0;
    Uart_Print->tick_old = 0;
    Uart_Print->tick_new = 0;
}