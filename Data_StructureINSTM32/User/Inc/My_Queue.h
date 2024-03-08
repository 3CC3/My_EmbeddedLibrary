//
// Created by 86159 on 2023-11-28.
//

#ifndef MY_FREERTOS_TEMPLATE_MY_QUEUE_H
#define MY_FREERTOS_TEMPLATE_MY_QUEUE_H

#include "main.h"
#include "cmsis_os.h"

#define MAXSIZE 100
#define ElemType uint8_t
typedef enum
{
    QFalse = 0,
    QTrue,
}QBool;

typedef enum
{
    QERROR = 0,
    QSUCCESS,
}QStatus;
/************************************************顺序队列**********************************************************/

typedef struct SqQueue
{
    ElemType Data[MAXSIZE]; //存放队列元素
    int front,rear; //队列的头尾（指针）
}SqQueue;


/************************************************链式队列**********************************************************/
//链式队列
typedef struct QueueNode
{
    ElemType Data;
    struct QueueNode* next;
}QueueNode;

typedef struct
{
    QueueNode *front; //头指针
    QueueNode *rear;  //尾指针
}LinkQueue;

QStatus LinkQueue_Init(LinkQueue *LQ);
QBool LinkQueue_Empty(LinkQueue LQ);
QStatus LinkQueue_EnNode(LinkQueue *LQ,ElemType Data);
QStatus LinkQueue_DeNode(LinkQueue *LQ,ElemType *Data);
ElemType LinkQueue_GetTop(LinkQueue LQ);



#endif //MY_FREERTOS_TEMPLATE_MY_QUEUE_H
