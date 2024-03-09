//
// Created by 86159 on 2024-01-23.
//

#ifndef MY_FLYBACK_ESP32_MQTT_H
#define MY_FLYBACK_ESP32_MQTT_H

#include "main.h"
#include "core_json.h"

/*******************************下面的宏定义是一些用户配置******************************************************************************/
#define WiFiSSID                "\"Xiaomi_CFED\""  //"\"realme\""     //WIFI名称
#define WiFiPassWord            "\"13794500728ebr\""  //"\"2543956551\"" //WIFI密码
                                //下面三元素在阿里云平台获取
#define DeviceName              "LEDRib"    //"ESP32_STM32"
#define ProdectKey              "k0rmgB3ff3p"  //"k0rmg8pzV7Q"
#define MQTTClientId              "\"LEDRib|securemode=2\\,signmethod=hmacsha1\\,timestamp=1707442812312|\""
//#define MQTTClientId            "\"ESP32_STM32|securemode=2\\,signmethod=hmacsha1\\,timestamp=1706012315795|\""

                                //下面三元素在MQTT_Password网站计算后得到
#define UserName                "\"LEDRib&k0rmgB3ff3p\""
//#define UserName                "\"ESP32_STM32&k0rmg8pzV7Q\""
#define MQTTPassWord            "\"86B4F09F934B1ED7EFED9F67A6AB775014F3917B\""
//#define MQTTPassWord            "\"C15EEDC481848B4A672DB219372FF43682BDCE0A\""
#define MQTTBrokerHost          "\""ProdectKey".iot-as-mqtt.cn-shanghai.aliyuncs.com\""

#define MQTTPort                "1883"    //端口号（默认为1883）
#define MQTTPublishTopic        "/sys/"ProdectKey"/"DeviceName"/thing/event/property/post" //发布的Topic
#define MQTTSubscribeTopic      "/sys/"ProdectKey"/"DeviceName"/thing/service/property/set" //订阅的Topic
#define JSONFormat              "{\\\"params\\\":{\\\"CO2\\\":%f\\,\\\"sw\\\":%d\\,\\\"PWMCCR\\\":%d\\}\\,\\\"version\\\":\\\"1.0.0\\\"}" //要从STM32下发数据到阿里云时的JSON数据格式
#define JSONPWMFormat           "{\\\"params\\\":{\\\"PWMCCR\\\":%d\\}\\,\\\"version\\\":\\\"1.0.0\\\"}"
#define JSONSWFormat            "{\\\"params\\\":{\\\"sw\\\":%d\\}\\,\\\"version\\\":\\\"1.0.0\\\"}"
/*---------------------------------------------------------------------------------------------------------------------------------*/

/***************************************---基础AT指令---*****************************************************************************/
#define TestATStart             "AT\r\n"     //测试AT启动
#define CloseEcho               "ATE0\r\n"   //关闭回显
#define ESPGMR                  "AT+GMR\r\n" //查看版本信息
#define ESPRST                  "AT+RST\r\n" //重启ESP
/*---------------------------------------------------------------------------------------------------------------------------------*/

/***************************************---WIFI AT指令---****************************************************************************/
#define SetWiFiMode             "AT+CWMODE=1,1\r\n" //设置Esp32的WIFI模式
#define ConnectAP               "AT+CWJAP="WiFiSSID","WiFiPassWord"\r\n"  //连接WIFI
#define AUTOConnectAP           "AT+CWAUTOCONN=0\r\n"  //设置上电要不要自动连接AP（WiFi）
/*---------------------------------------------------------------------------------------------------------------------------------*/

/***************************************---MQTT AT指令---*****************************************************************************/
#define MQTTUserCFG             "AT+MQTTUSERCFG=0,1,"MQTTClientId","UserName","MQTTPassWord",0,0,\"\"\r\n"  //MQTT用户属性配置
#define MQTTConnectBroker       "AT+MQTTCONN=0,"MQTTBrokerHost","MQTTPort",0\r\n"     //连接Broker(阿里云平台)
#define MQTTSubscribe           "AT+MQTTSUB=0,\""MQTTSubscribeTopic"\",0\r\n"   //订阅
/*---------------------------------------------------------------------------------------------------------------------------------*/

typedef enum
{
    ESPnotREADY = 0, //ESP没准备好，还不能发AT指令
    ESPisREADY,  //ESP准备好可以发AT指令了
    ESPATError,  //AT指令出错
    ESPSuccess,  //一切正常
    JSONFormatError, //JSON格式错误
    JSONReceiveError, //未接收到特定的JSON数据
    ESPInitError,
}ESPMQTT_Status;

ESPMQTT_Status ESPMQTT_Start(void);
ESPMQTT_Status ESP32WaitString(UartRx_Package Uart_PackageHandle,const char *WaitString,uint8_t Second);
ESPMQTT_Status ESP32Publish(UART_HandleTypeDef *huart,double CO2Concentration,bool sw,int PWM);
ESPMQTT_Status ESP32Subscribe(UartRx_Package Uart_PackageHandle);
ESPMQTT_Status ESP32ReceiveMsgJSONParse(UartRx_Package *Uart_PackageHandle,char **OutJSONData,int *OutJSONLength);
int ConvertString2Num(const char *string);
ESPMQTT_Status ESP_SendATCMD(UartRx_Package Uart_PackageHandle,uint8_t *ATCMDString,const char *WaitString,uint8_t Second);
ESPMQTT_Status JSONLEDTask(const char *Data,int DataLength);
ESPMQTT_Status JSONBreathLEDTask(const char *Data,int DataLength);
ESPMQTT_Status JSONALLTask(const char *Data,int DataLength);



#endif //MY_FLYBACK_ESP32_MQTT_H
