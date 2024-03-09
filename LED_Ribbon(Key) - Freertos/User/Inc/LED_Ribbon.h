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

typedef enum
{
    BackToStart = 0,  //流水灯到尽头时回到起点重新开始
    Rebound     = 1,  //流水灯到尽头时反弹回去
}WaterFallLight_Mode;

typedef enum
{
    DefaultMode = 0,
    RandomLightMode = 1,
    WaterFallBack2Start = 2,
    WaterFallRebound = 3,
}LEDRib_Mode;
/*随机亮灯句柄*/
typedef struct
{
    uint16_t  SwitchPeriod; //换灯时间戳（单位:ms）
    volatile uint8_t RandomBuff[NUMBER_OF_ALL_LIGHT]; //随机亮灯所需要用到的数组
    volatile uint8_t StartFlag; //可以开始换灯标志位
    void (* RandomLightFun)(void); //函数指针，指向执行随机LED的函数
}__attribute__((packed))RandomLEDMode;

/*流水灯句柄*/
typedef struct
{
    void (* WaterFallLightFun)(WaterFallLight_Mode Mode);
}__attribute__((packed))WaterFallLedMode;



typedef struct
{
    SPI_HandleTypeDef *hspi;        //使用到的SPI
    LedSize NumOfAllLight;          //所有灯的数量
    LedSize NumOfWaterFallLight;    //流水灯中，同一时间共点亮灯的个数
    void(*LightAllLEDFun)(uint8_t Red, uint8_t Green, uint8_t Blue);
    RandomLEDMode RandomLED;        //随机亮灯句柄
    WaterFallLedMode WaterFallLED;  //流水灯句柄
    LEDRib_Mode RuningMode;             //要执行的模式(0:无 1:随机亮灯 2:回头流水灯 3:反弹流水灯)
}__attribute__((packed))LEDRibHandle;

extern LEDRibHandle hLedRib;


void LED_RibReset(void);
void LED_RibSetSingleLEDColor(uint8_t Red, uint8_t Green, uint8_t Blue);
void LED_RibALLLight(uint8_t Red, uint8_t Green, uint8_t Blue);
void Water_FallLight(WaterFallLight_Mode Mode);
void LED_RibRandomLight(void);
void LED_RibInit(void);


#endif //MY_FLYBACK_LED_RIBBON_H
