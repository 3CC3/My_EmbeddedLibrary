//
// Created by 86159 on 2024-01-30.
//

#ifndef MY_FLYBACK_LED_RIBBON_H
#define MY_FLYBACK_LED_RIBBON_H

#include "main.h"

#define ZEROCODE                0b111000000  //3个1相当于高电平0.33us
#define ONECODE                 0b111111000  //6个1相当于高电平0.66us(虽然稍微超出了点误差范围，但是影响不大)

#define NUMBER_OF_ALL_LIGHT        60          //灯带中总共有NUMBEROFALLLIGHT个LED灯
#define NUMBER_OF_WATERFALL_LIGHT  26          //流水灯中，同一时间共点亮NUMBEROFWATERFALLLIGHT个灯

typedef uint8_t LedSize;
typedef struct
{
    uint16_t  SwitchPeriod; //换灯时间戳（单位:ms）
    uint8_t   RandomBuff[NUMBER_OF_ALL_LIGHT]; //随机亮灯所需要用到的数组
    uint8_t   StartFlag; //可以开始换灯标志位
    void (* RandomLightFun)(void); //函数指针，指向执行随机LED的函数
}RandomLEDMode;

typedef struct
{
    SPI_HandleTypeDef *hspi;    //使用到的SPI
    LedSize NumOfAllLight;      //所有灯的数量
    LedSize NumOfWaterFallLight;//流水灯中，同一时间共点亮灯的个数
    RandomLEDMode RandomLED;    //随机亮灯句柄
}LEDRibHandle;


typedef enum
{
    BackToStart = 0,  //流水灯到尽头时回到起点重新开始
    Rebound     = 1,  //流水灯到尽头时反弹回去
}WaterFallLight_Mode;

extern LEDRibHandle hLedRib;
void LED_RibReset(void);
void LED_RibSetSingleLEDColor(uint8_t Red, uint8_t Green, uint8_t Blue);
void LED_RibALLLight(uint8_t Red, uint8_t Green, uint8_t Blue);
void Water_FallLight(WaterFallLight_Mode Mode);
void LED_RibRandomLight(void);
void LED_RibInit(void);


#endif //MY_FLYBACK_LED_RIBBON_H
