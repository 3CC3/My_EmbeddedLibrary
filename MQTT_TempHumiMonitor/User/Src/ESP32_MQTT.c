//
// Created by 86159 on 2024-01-23.
//
#include "ESP32_MQTT.h"
#include "usart.h"
#include "string.h"
#include "Uart_RxPackage.h"
#include "retarget.h"
#include "OLED.h"
#include "tim.h"
#include "string.h"
/**
 * 判断串口接受的数据包中有没有特定字符串“string”
 * @param string 要查找的字符串
 * @return ESPSuccess：有该字符串存在
 *         ESPATError：没查找到
 */
ESPMQTT_Status ReceiveString(const char* string)
{
    uint16_t count = 0;
    uint8_t ret = ESPATError;
    printf("RxPackage:%s\r\n",Uart2_Package.Rx_Package);
    while(Uart2_Package.RxEndFalg == 0)
    {
        count++;
        HAL_Delay(1);
        if(count>2000)
        {
            return ESPATError;
        }
    }
    if(Uart_ReceiveEsp(&Uart2_Package,string) == SUCCESS)
    {
        printf("Receive Correct!\r\n");
        return ESPSuccess;
    }
    else printf("Receive Error!\r\n");
    return ret;
}

/**
 * 给ESP发送AT指令
 * @param Uart_PackageHandle 串口接收数据包的句柄
 * @param ATCMDString        要发送的AT指令
 * @param WaitString         需要等待的字符串（如："OK"）
 * @param Second             发送完AT指令后需要停顿的时间（注意：并不是精准地指执行该命令的时间）
 * @return 状态
 */
ESPMQTT_Status ESP_SendATCMD(UartRx_Package Uart_PackageHandle,uint8_t *ATCMDString,const char *WaitString,uint8_t Second)
{
    HAL_UART_Transmit(Uart_PackageHandle.huart,(uint8_t*)ATCMDString,strlen((char*)ATCMDString),999);
    HAL_Delay(Second*1000);
    if(ESP32WaitString(Uart_PackageHandle,WaitString,1) == ESPSuccess) return ESPSuccess;
    else return ESPInitError;
}

/**
 * 等待某个字符串在串口接收的数据包中出现
 * @param Uart_PackageHandle 串口数据包的句柄
 * @param WaitString 需要等待出现的字符串
 * @return ESPisREADY：等待的字符串已经出现
 *         ESPnotREADY：等待的字符串没出现
 */
ESPMQTT_Status ESP32WaitString(UartRx_Package Uart_PackageHandle,const char *WaitString,uint8_t Second)
{
    uint16_t count = 0;
    printf("RxPackage:%s\r\n",Uart_PackageHandle.Rx_Package);
    while(strstr((char*)Uart_PackageHandle.Rx_Package,WaitString) == NULL)
    {
        //OLED_ShowString(1,1,"Wait OK         ");
        HAL_Delay(1);
        count++;
        if(count>Second*1000)
        {
            return ESPnotREADY;
        }
    }
    return ESPSuccess;
}

/**
 * 通过STM32发布AT指令给ESP32来控制ESP32发布JSON文档到阿里云
 * @param JSONString
 * @return
 */
ESPMQTT_Status ESP32Publish(UART_HandleTypeDef *huart,double CO2Concentration,bool sw,int PWM)
{
    char temp[256];
    sprintf(temp,"AT+MQTTPUB=0,\""MQTTPublishTopic"\",\""JSONFormat"\",0,0\r\n",CO2Concentration,sw,PWM);
    HAL_UART_Transmit(huart,(uint8_t*)temp,strlen(temp),999);
    return ESPSuccess;
}

/**
 * ESP订阅阿里云平台函数
 * @param huart 发布AT命令的串口
 * @return 状态
 */
ESPMQTT_Status ESP32Subscribe(UartRx_Package Uart_PackageHandle)
{
    OLED_ShowString(1,1,"MQTTSubscribing ");
    while(ESP_SendATCMD(Uart_PackageHandle,(uint8_t*)MQTTSubscribe,"OK",1) != ESPSuccess)
    {
        HAL_Delay(1000);
    }
    Uart_DoTask(&Uart2_Package);
    return ESPSuccess;
}

/**
 * 把数字字符串转化为整形的数字
 * @param string 数字字符串（如：“364”）
 * @return 转化后的整形数字
 */
int ConvertString2Num(const char *string)
{
    uint16_t i = 0;
    int num = 0,temp = 10;
    while(*(string+i) != '\0')
    {
        num = (*(string+i) - '0')+num*temp;
        i++;
    }
    return num;
}

/**
 * 开启ESP32，连上WIFI并且连上阿里云
 * @return
 */
ESPMQTT_Status ESPMQTT_Start(void)
{

    /*0.关闭回显*/
    OLED_ShowString(1,1,"0.Close Echo    ");
    while(ESP_SendATCMD(Uart2_Package,(uint8_t*)CloseEcho,"OK",1) != ESPSuccess)
    {
        Uart_DoTask(&Uart2_Package);
        HAL_Delay(1000);
    }
    Uart_DoTask(&Uart2_Package);

    /*1.设置WiFi模式*/
    OLED_ShowString(1,1,"1.Set WiFi Mode ");
    while(ESP_SendATCMD(Uart2_Package,(uint8_t*)SetWiFiMode,"OK",1)!=ESPSuccess)
    {
        Uart_DoTask(&Uart2_Package);
        HAL_Delay(1000);
    }
    Uart_DoTask(&Uart2_Package);

    /*2.上电不自动连接AP(WiFi)*/
    OLED_ShowString(1,1,"2.AUTOConnectCFG");
    while(ESP_SendATCMD(Uart2_Package,(uint8_t*)AUTOConnectAP,"OK",1)!=ESPSuccess)
    {
        Uart_DoTask(&Uart2_Package);
        HAL_Delay(1000);
    }
    Uart_DoTask(&Uart2_Package);

    /*3.重启ESP32*/
    OLED_ShowString(1,1,"3.Rest ESP      ");
    while(ESP_SendATCMD(Uart2_Package,(uint8_t*)ESPRST,"OK",3)!=ESPSuccess)
    {
        Uart_DoTask(&Uart2_Package);
        HAL_Delay(1000);
    }
    Uart_DoTask(&Uart2_Package);

    /*4.连接WiFi*/
    OLED_ShowString(1,1,"4.Connect WiFi  ");
    while(ESP_SendATCMD(Uart2_Package,(uint8_t*)ConnectAP,"OK",1)!=ESPSuccess)
    {
        Uart_DoTask(&Uart2_Package);
        HAL_Delay(1000);
    }
    Uart_DoTask(&Uart2_Package);

    /*5.配置MQTT用户属性*/
    OLED_ShowString(1,1,"5.MQTT UserCFG  ");
    while(ESP_SendATCMD(Uart2_Package,(uint8_t*)MQTTUserCFG,"OK",3)!=ESPSuccess)
    {
        Uart_DoTask(&Uart2_Package);
        HAL_Delay(1000);
    }
    Uart_DoTask(&Uart2_Package);

    /*6.连接MQTT Broker*/
    OLED_ShowString(1,1,"6.MQTTContBroker");
    while(ESP_SendATCMD(Uart2_Package,(uint8_t*)MQTTConnectBroker,"OK",2)!=ESPSuccess)
    {
        Uart_DoTask(&Uart2_Package);
        HAL_Delay(1000);
    }
    Uart_DoTask(&Uart2_Package);

    OLED_ShowString(1,1,"ESPMQTTInitSucs!");
}

/**
 * 处理接收到的JSON格式字符串时执行的LED任务
 * @param Data JSON格式的字符串
 * @param DataLength 字符串的长度
 * @return 状态
 */
ESPMQTT_Status JSONLEDTask(const char *Data,int DataLength)
{
    uint8_t LEDStatus = 0;
    JSONStatus_t Status;
    char query[] = "params.sw";
    size_t QueryLength = strlen(query);
    size_t OutValueLength;
    char *OutValue = NULL;
    Status = JSON_Validate(Data,DataLength);
    if(Status == JSONSuccess)
    {
        Status = JSON_Search((char*)Data,DataLength,query,QueryLength,&OutValue,&OutValueLength);
        if(Status == JSONSuccess)
        {
            uint8_t tempchar = OutValue[OutValueLength];
            OutValue[OutValueLength] = '\0';
            printf("Query:%s\tOutValue:%s\r\n",query,OutValue);
            LEDStatus = OutValue[0]-'0';
            printf("LEDStatus:%d\r\n",LEDStatus);
            HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,LEDStatus);
            OutValue[OutValueLength] = tempchar;

            //发布回消息到阿里云平台(否则IoT Studio可能出现数据下发超时，请检查设备)
            char temp[256];
            sprintf(temp,"AT+MQTTPUB=0,\""MQTTPublishTopic"\",\""JSONSWFormat"\",0,0\r\n",LEDStatus);
            Uart_DoTask(&Uart2_Package); //清除接收到的数据
            HAL_UART_Transmit(Uart2_Package.huart,(uint8_t*)temp,strlen(temp),999);
            HAL_Delay(50); //等待阿里云平台发送数据回来
            if(ESP32WaitString(Uart2_Package,"\"sw\":",2) != ESPSuccess) printf("IoT Studio Return Package Error!\r\n");
            Uart_DoTask(&Uart2_Package); //清除接收到的数据

            return ESPSuccess;
        }
        else printf("Not Fonud key:sw!\r\n");
    }
    else printf("JSONFormat is invalid!\r\n");
    return JSONFormatError;
}

/**
 * 处理接收到的JSON格式字符串时执行的呼吸灯
 * @param Data JSON格式的字符串
 * @param DataLength 字符串的长度
 * @return 状态
 */
ESPMQTT_Status JSONBreathLEDTask(const char *Data,int DataLength)
{
    uint8_t LEDCCR = 0;
    JSONStatus_t Status;
    char query[] = "params.PWMCCR";
    size_t QueryLength = strlen(query);
    size_t OutValueLength;
    char *OutValue = NULL;
    Status = JSON_Validate(Data,DataLength);
    if(Status == JSONSuccess)
    {
        Status = JSON_Search((char*)Data,DataLength,query,QueryLength,&OutValue,&OutValueLength);
        if(Status == JSONSuccess)
        {
            uint8_t tempchar = OutValue[OutValueLength];
            OutValue[OutValueLength] = '\0';
            printf("Query:%s\tOutValue:%s\r\n",query,OutValue);

            LEDCCR = ConvertString2Num(OutValue);
            printf("LED CCR:%d\r\n",LEDCCR);
            __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,LEDCCR);

            OutValue[OutValueLength] = tempchar;

            //发布回消息到阿里云平台(否则IoT Studio可能出现数据下发超时，请检查设备)
            char temp[256];
            sprintf(temp,"AT+MQTTPUB=0,\""MQTTPublishTopic"\",\""JSONPWMFormat"\",0,0\r\n",LEDCCR);
            Uart_DoTask(&Uart2_Package); //清除接收到的数据
            HAL_UART_Transmit(Uart2_Package.huart,(uint8_t*)temp,strlen(temp),999);
            HAL_Delay(50);
            if(ESP32WaitString(Uart2_Package,"\"PWMCCR\":",2) != ESPSuccess) printf("IoT Studio Return Package Error!\r\n");
            Uart_DoTask(&Uart2_Package); //清除接收到的数据

            return ESPSuccess;
        }
        else printf("Not Fonud key:PWMCCR!\r\n");
    }
    else printf("JSONFormat is invalid! Status = %d\r\n",Status);
    return JSONFormatError;
}

/**特别注意！！！:发布回消息回到阿里云平台不能在所有任务执行完前发布。因为所有的Data的内存都同属于串口接收数据包，所有一旦串口接收的数据包发生变化，那么所有的Data都会改变,所以会发生JSON Format Invalid
 * 统管所有需要执行的任务
 * @param Data        得到的JSON格式字符串
 * @param DataLength  JSON字符串的长度
 * @return            状态
 */
ESPMQTT_Status JSONALLTask(const char *Data,int DataLength)
{
    JSONStatus_t Status;

    //任务一：LED呼吸灯
    size_t OutValueLength1;
    char *OutValue1 = NULL;
    uint8_t LEDCCR = 0;
    char query1[] = "params.PWMCCR";
    size_t QueryLength1 = strlen(query1);
    JSONStatus_t Task1Status;

    //任务二：LED开关
    size_t OutValueLength2;
    char *OutValue2 = NULL;
    uint8_t LEDStatus = 0;
    char query2[] = "params.sw";
    size_t QueryLength2 = strlen(query2);
    JSONStatus_t Task2Status;

    Status = JSON_Validate(Data,DataLength);
    if(Status == JSONSuccess)
    {
        Task1Status = JSON_Search((char*)Data,DataLength,query1,QueryLength1,&OutValue1,&OutValueLength1);
        Task2Status = JSON_Search((char*)Data,DataLength,query2,QueryLength2,&OutValue2,&OutValueLength2);
        /**任务一**/
        if(Task1Status == JSONSuccess)
        {
            uint8_t tempchar = OutValue1[OutValueLength1];
            OutValue1[OutValueLength1] = '\0';
            printf("Query:%s\tOutValue:%s\r\n",query1,OutValue1);

            LEDCCR = ConvertString2Num(OutValue1);
            printf("LED CCR:%d\r\n",LEDCCR);
            __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,LEDCCR);

            OutValue1[OutValueLength1] = tempchar;
        }
        else printf("Not Fonud key:sw!\r\n");

        /**任务二**/
        if(Task2Status == JSONSuccess)
        {
            uint8_t tempchar = OutValue2[OutValueLength2];
            OutValue2[OutValueLength2] = '\0';
            printf("Query:%s\tOutValue:%s\r\n",query2,OutValue2);
            LEDStatus = OutValue2[0]-'0';
            printf("LEDStatus:%d\r\n",LEDStatus);
            HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,LEDStatus);
            OutValue2[OutValueLength2] = tempchar;
        }
        else printf("Not Fonud key:PWMCCR!\r\n");

        if(Task1Status == JSONSuccess)
        {
            //发布回消息到阿里云平台(否则IoT Studio可能出现数据下发超时，请检查设备)
            char temp[256];
            sprintf(temp,"AT+MQTTPUB=0,\""MQTTPublishTopic"\",\""JSONPWMFormat"\",0,0\r\n",LEDCCR);
            Uart_DoTask(&Uart2_Package); //清除接收到的数据
            HAL_UART_Transmit(Uart2_Package.huart,(uint8_t*)temp,strlen(temp),999);
            HAL_Delay(50);
            if(ESP32WaitString(Uart2_Package,"\"PWMCCR\":",2) != ESPSuccess) printf("IoT Studio Return Package Error!\r\n");
            Uart_DoTask(&Uart2_Package); //清除接收到的数据
        }
        if(Task2Status == JSONSuccess)
        {
            //发布回消息到阿里云平台(否则IoT Studio可能出现数据下发超时，请检查设备)
            char temp[256];
            sprintf(temp,"AT+MQTTPUB=0,\""MQTTPublishTopic"\",\""JSONSWFormat"\",0,0\r\n",LEDStatus);
            Uart_DoTask(&Uart2_Package); //清除接收到的数据
            HAL_UART_Transmit(Uart2_Package.huart,(uint8_t*)temp,strlen(temp),999);
            HAL_Delay(50); //等待阿里云平台发送数据回来
            if(ESP32WaitString(Uart2_Package,"\"sw\":",2) != ESPSuccess) printf("IoT Studio Return Package Error!\r\n");
            Uart_DoTask(&Uart2_Package); //清除接收到的数据
        }
        return ESPSuccess;
    }
    else printf("JSONFormat is invalid!\r\n");
    return JSONFormatError;
}


/**
 * ESP32接收到阿里云下发的数据时干的事
 * @param  Uart_PackageHandle 串口接收数据包句柄
 * @return 状态
 */
ESPMQTT_Status ESP32ReceiveMsgJSONParse(UartRx_Package *Uart_PackageHandle,char **OutJSONData,int *OutJSONLength)
{
    int DataLength = 0;
    char *Data = NULL;
    char *NumString = NULL;
    uint8_t index = 2;
    //printf("完全数据包：%s\r\n",Uart_PackageHandle->Rx_Package);
    if(strstr((const char*)Uart_PackageHandle->Rx_Package,"+MQTTSUBRECV:")!=NULL)
    {
        /*由于sscanf占用的flash太大了所以不得不投机取巧用{"method":作为JSON数据的帧头来识别*/
        //sscanf((const char*)Uart_PackageHandle->Rx_Package,"+MQTTSUBRECV:0,\""MQTTSubscribeTopic"\",%d,%s",&DataLength,Data);
        Data = strstr((const char*)Uart_PackageHandle->Rx_Package,"{\"method\":");

        //下面这段代码用于获取JSON数据的长度
        while(Data[-index]!=',')index++;
        int numLength = index-2;
        NumString = &Data[-(index-1)];
        NumString[numLength] = '\0';
        DataLength = ConvertString2Num(NumString);

        printf("DataLength:%d\tData:%s\r\n",DataLength,Data);
        *OutJSONData = Data;
        *OutJSONLength = DataLength;
        //处理接收到的JSON格式的数据
        //JSONALLTask(Data,DataLength);
//        JSONLEDTask(Data,DataLength);
//        JSONBreathLEDTask(Data,DataLength);
        return ESPSuccess;
    }
    else
    {
        printf("JSON Receive Error\r\n");
        return JSONReceiveError;
    }
}
