//
// Created by 86159 on 2023-11-27.
//
#include "My_Stack.h"
#include "retarget.h"
#include "stdlib.h"

#define MYLINKSTACK   //MYSQSTACK
#define FREERTOS      //NOFREERTOS

#ifdef FREERTOS
#define StackFree    vPortFree
#define StackMalloc  pvPortMalloc
#endif

#ifdef NOFREERTOS
#define StackFree    free
#define StackMalloc  malloc
#endif
/************************************************顺序栈**********************************************************/
#ifdef MYSQSTACK
//初始化栈
void Stack_Init(SqStack* Stack)
{
    printf("初始化成功！\r\n");
    Stack->top = -1; //初始时设置栈顶指针为-1
}
//栈判空操作
SBool Stack_Empty(SqStack Stack)
{
    if(Stack.top == -1)return STrue;
    else return SFalse;
}

//入栈操作
SStatus Stack_Push(SqStack *Stack,ElemType x)
{
    if(Stack->top >= MAXSIZE - 1) return SERROR; //栈满了
    Stack->Data[++Stack->top] = x;
    return SSUCCESS;
}

//出栈操作
SStatus Stack_Pop(SqStack *Stack,ElemType *x)
{
    if(Stack->top == -1) return SERROR; //栈空
    *x = Stack->Data[Stack->top--];
    return SSUCCESS;
}

ElemType Stack_GetTop(SqStack Stack)
{
    if(Stack.top == -1) return -1;//栈空
    return Stack.Data[Stack.top];
}

//遍历栈
void Stack_Print(SqStack Stack)
{
    int16_t i = Stack.top;
    while(i > -1)
    {
        printf("栈的第%d个值为:%d",i,Stack.Data[i]);
        i--;
    }
}

//销毁栈
void Stack_Destroy(SqStack *Stack)
{
    Stack->top = -1;
}
#endif

/************************************************链栈**********************************************************/

#ifdef MYLINKSTACK

//初始化栈
void LinkStack_Init(LinkStack *LS)
{
    *LS = NULL;
    (*LS)->Data = 0; //头结点存储栈的长度
}

/***
 * 入栈函数
 * @param LS   栈顶指针
 * @param Data 入栈的数据
 * @return     状态
 */
SStatus LinkStack_Push(LinkStack *LS,ElemType Data)
{
    StackNode* p = (StackNode*) StackMalloc(sizeof(StackNode));
    if(p == NULL) return SERROR; //栈满

    p->Data = Data;
    if(*LS == NULL) //栈中无数据
    {
        (*LS) = p;
        p->next = NULL;
    }
    else  //栈中已有数据
    {
        p->next = (*LS);
        (*LS) = p;
    }
    (*LS)->Data++;
    return SSUCCESS;
}

/***
 * 出栈函数
 * @param LS   头指针
 * @param Data 出栈的数据的
 * @return     状态
 */
SStatus LinkStack_Pop(LinkStack *LS,ElemType *Data)
{
    StackNode *p = (*LS);
    if(*LS == NULL) return SERROR; //栈空
    *Data = (*LS)->Data;
    (*LS) = p->next;
    StackFree(p);
    p = NULL;
    (*LS)->Data--;
    return SSUCCESS;
}

//判断栈是否为空
SBool LinkStack_Empty(LinkStack LS)
{
    if(LS == NULL) return STrue;
    return SFalse;
}

//获取头结点的数据
SStatus LinkStack_GetTop(LinkStack LS,ElemType *Data)
{
    if(LS== NULL) return SERROR;
    *Data = LS->Data;
    return SSUCCESS;
}

//遍历栈
SStatus LinkStack_Print(LinkStack LS)
{
    if(LS == NULL) return SERROR;
    StackNode *p = LS;
    uint8_t i = 0;
    while(p->next != NULL)
    {
        printf("链栈的第%d个数据为%d\r\n",i++,p->Data);
        p = p->next;
    }
    return SSUCCESS;
}

//获取栈的长度
uint8_t LinkStack_GetLength(LinkStack LS)
{
    uint8_t i = 0;
    StackNode *p = LS;
    while(p->next != NULL)
    {
        i++;
        p = p->next;
    }
    return i;
}

/***
 * 清除栈中的数据
 * @param LS  栈顶指针
 * @param num 把栈的数据清除为num
 */
void LinkStack_Clear(LinkStack *LS,ElemType num)
{
    StackNode *p = (*LS);
    while(p->next != NULL)
    {
        p->Data = num;
        p = p->next;
    }
}

//销毁栈
void LinkStack_Destroy(LinkStack *LS)
{
    StackNode *p = (*LS),*q;
    while(p->next != NULL)
    {
        q = p->next;
        StackFree(p);
        p = q;
    }
}



#endif


