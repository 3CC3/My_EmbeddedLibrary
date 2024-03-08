//
// Created by 86159 on 2023-11-27.
//

#ifndef MY_FREERTOS_TEMPLATE_MY_STACK_H
#define MY_FREERTOS_TEMPLATE_MY_STACK_H

#include "main.h"
#include "cmsis_os.h"

typedef enum
{
    SFalse = 0,
    STrue,
}SBool;

typedef enum
{
    SERROR = 0,
    SSUCCESS,
}SStatus;

#define ElemType uint8_t

/************************************************顺序栈**********************************************************/
#define MAXSIZE 100
typedef struct
{
    ElemType Data[MAXSIZE];
    int16_t top;  //栈顶指针存储当前存储的数据的数目
}SqStack;

void Stack_Init(SqStack* Stack);
SBool Stack_Empty(SqStack Stack);
SStatus Stack_Push(SqStack *Stack,ElemType x);
SStatus Stack_Pop(SqStack *Stack,ElemType *x);
ElemType Stack_GetTop(SqStack Stack);
void Stack_Print(SqStack Stack);
void Stack_Destroy(SqStack *Stack);


/************************************************链栈**********************************************************/
//链栈的结点
typedef struct StackNode
{
    ElemType Data;
    struct StackNode *next;
}StackNode;

typedef StackNode* LinkStack; //栈顶指针，指向头结点

void LinkStack_Init(LinkStack *LS);
SStatus LinkStack_Push(LinkStack *LS,ElemType Data);
SStatus LinkStack_Pop(LinkStack *LS,ElemType *Data);
SBool LinkStack_Empty(LinkStack LS);
SStatus LinkStack_GetTop(LinkStack LS,ElemType *Data);
SStatus LinkStack_Print(LinkStack LS);
uint8_t LinkStack_GetLength(LinkStack LS);
void LinkStack_Clear(LinkStack *LS,ElemType num);
void LinkStack_Destroy(LinkStack *LS);




#endif //MY_FREERTOS_TEMPLATE_MY_STACK_H
