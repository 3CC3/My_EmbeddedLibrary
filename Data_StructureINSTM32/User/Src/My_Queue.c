//
// Created by 86159 on 2023-11-28.
//
#include "My_Queue.h"

#define FREERTOS  //NOFREERTOS
#define LINKQUEUE //SQQUEUE

#ifdef FREERTOS
#define QueueFree    vPortFree
#define QueueMalloc  pvPortMalloc
#endif

#ifdef NOFREERTOS
#define QueueFree    free
#define QueueMalloc  malloc
#endif

/************************************************链式队列**********************************************************/
#ifdef LINKQUEUE
//初始化链式队列
QStatus LinkQueue_Init(LinkQueue *LQ)
{
    LQ->front = (QueueNode*)QueueMalloc(sizeof(QueueNode));
    if(LQ->front == NULL) return QERROR;
    LQ->front->next = NULL;
    LQ->rear = LQ->front;
    return QSUCCESS;
}
//判空
QBool LinkQueue_Empty(LinkQueue LQ)
{
    if(LQ.front == LQ.rear) return QTrue;
    return QFalse;
}
//入队
QStatus LinkQueue_EnNode(LinkQueue *LQ,ElemType Data)
{
    QueueNode *p = (QueueNode*)QueueMalloc(sizeof(QueueNode));
    p->Data = Data;
    p->next = NULL;
    LQ->rear->next = p;
    LQ->rear = p;
}
//出队
QStatus LinkQueue_DeNode(LinkQueue *LQ,ElemType *Data)
{
    if(LQ->front == LQ->rear) return QERROR; //空队列
    QueueNode *p = LQ->front->next; //注意：头结点是空的
    *Data= p->Data;
    LQ->front->next = p->next;
    if(LQ->rear == p)LQ->rear = LQ->front; //当只有一个结点时
    QueueFree(p);
    p = NULL;
    return QSUCCESS;
}

//读取队头元素（注意：队首指针的数据域是空的）
ElemType LinkQueue_GetTop(LinkQueue LQ)
{
    return LQ.front->next->Data;
}

#endif

/************************************************顺序循环队列**********************************************************/
#ifdef SQQUEUE
void SqQueue_Init(SqQueue SQ)
{
    SQ.front = SQ.rear = 0;
}

QBool SqQueue_Empty(SqQueue SQ)
{
    if(SQ.front == SQ.rear)return QTrue;
    return QFalse;
}

//入队
QStatus SqQueue_EnData(SqQueue SQ,ElemType Data)
{
    if((SQ.rear+1)%MAXSIZE == SQ.front) return QERROR; //队满（注意：这里不能是SQ.rear+1 == MAXSIZE作为判断条件，因为SQ.front不一定就是0）
    SQ.Data[SQ.rear] = Data;
    SQ.rear = (SQ.rear+1)%MAXSIZE;
    return QSUCCESS;
}

//出队
QStatus SqQueue_DeData(SqQueue SQ,ElemType *Data)
{
    if(SQ.front == SQ.rear) return QERROR; //队空
    *Data = SQ.Data[SQ.front];
    SQ.front = (SQ.front+1)%MAXSIZE;
    return QSUCCESS;
}

//读取队首元素
ElemType SqQueue_GetTop(SqQueue SQ)
{
    return SQ.Data[SQ.front];
}
#endif