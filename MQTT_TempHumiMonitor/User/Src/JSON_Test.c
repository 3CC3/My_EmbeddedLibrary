//
// Created by 86159 on 2024-01-21.
//
#include "JSON_Test.h"
#include "retarget.h"
#include "string.h"
void My_JSONFun(void)
{
    JSONStatus_t result;
    char buf[] = "{\"param\":{\"temp\":38,\"humi\":94}}";
    char buf2[] = "{\"results\":[{\"location\":{\"id\":\"WT3Q0FW9ZJ3Q\",\"name\":\"武汉\",\"country\":\"CN\",\"path\":\"武汉,武汉,湖北,中国\",\"timezone\":\"Asia/Shanghai\",\"timezone_offset\":\"+08:00\"},\"now\":{\"text\":\"小雨\",\"code\":\"13\",\"temperature\":\"4\"},\"last_update\":\"2022-01-22T17:47:52+08:00\"}]}";
    size_t BufLength = sizeof(buf2) - 1; //这里要‘-1’是因为字符串的末尾会多出来个‘\0’
    char query[] = "results.[0].location.path";
    size_t QueryLength = sizeof(query) - 1;
    char *OutValue = NULL;
    size_t OutLength = 0;
    //检验JSON格式是否有效
    result = JSON_Validate(buf2,BufLength);
    if(result == JSONSuccess)
    {
        printf("JSON is valid!\r\n");
        //搜索JSON格式的buf中param的temp键对应的值
        result = JSON_Search(buf2,BufLength,query,QueryLength,&OutValue,&OutLength);
        if(result == JSONSuccess)
        {
            OutValue[OutLength] = '\0';
            printf("Found:%s -> %s\r\n",query,OutValue);
        }
        else
        {
            printf("JSON search ERROR! result = %d\r\n",result);
        }
    }
    else
    {
        printf("JSON is invalid! result = %d\r\n",result);
    }
}

const char *(JsonType[])=
        {
        "Invalid",
        "String",
        "Number",
        "True",
        "False",
        "NULL",
        "Object",
        "Array"
        };

void My_Show(const char *Json)
{
    JSONStatus_t result;
    size_t JsonLength = strlen(Json);
    size_t Start = 0,Next = 0;
    JSONPair_t outpair;
    printf("JsonLength:%d\r\n",JsonLength);
    //进行JSON格式的有效验证
    result = JSON_Validate(Json,JsonLength);

    if(result == JSONSuccess)
    {
        result = JSON_Iterate(Json,JsonLength,&Start,&Next,&outpair);
        while(result == JSONSuccess)
        {
            printf("Start:%d\tNext:%d\r\n",Start,Next);
            printf("Key:%.*s\t",outpair.keyLength,outpair.key);
            printf("Value(%s):%.*s\r\n",JsonType[outpair.jsonType],outpair.valueLength,outpair.value);
            result = JSON_Iterate(Json,JsonLength,&Start,&Next,&outpair);
        }
    }
    else
    {
        printf("JSON Invalid!\r\n");
    }
 }
