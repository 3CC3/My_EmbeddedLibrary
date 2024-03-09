//
// Created by 86159 on 2024-01-30.
//

#include "Delay.h"

/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  */
void Delay_us(uint32_t xus)
{
    uint16_t Load = SysTick->LOAD;
    uint16_t Val = SysTick->VAL;
    uint16_t CTRL = SysTick->CTRL;

    SysTick->LOAD = 72 * xus;				//设置定时器重装值
    SysTick->VAL = 0x00;					//清空当前计数值
    SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
    while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
    SysTick->CTRL = 0x00000007;

    SysTick->LOAD = Load;
    SysTick->VAL = Val;
    SysTick->CTRL = CTRL;
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
    while(xms--)
    {
        Delay_us(1000);
    }
}

/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
    while(xs--)
    {
        Delay_ms(1000);
    }
}
