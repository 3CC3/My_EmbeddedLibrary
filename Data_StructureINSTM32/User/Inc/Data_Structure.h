//
// Created by 86159 on 2023-11-22.
//

#ifndef MY_FREERTOS_TEMPLATE_DATA_STRUCTURE_H
#define MY_FREERTOS_TEMPLATE_DATA_STRUCTURE_H

/***如果不增大默认堆区的大小（3072）则最多只能开辟20个结点左右***/
#include "main.h"

#define ElemType uint8_t   //定义链表中的数据类型
typedef enum
{
    LERROR = 0,
    LSUCCESS,
}LStatus;
//struct  //声明并且定义一个Bit的结构体，该结构体只能有一个
//{
//    uint8_t test;
//}Bit;
typedef union //联合体（也叫共用体），里面所有的变量共用一块内存
{
    uint8_t StatusValue; //原则上该变量不赋值（会改变下面的结构体的值）
    struct
    {
        uint8_t LIST_HEAP_OVERFLOW :1; //:1表示只用一个Bit位，节省空间
        uint8_t LIST_EMPTY :1;
        uint8_t            :1;
        uint8_t            :1;
        uint8_t            :1;
        uint8_t            :1;
        uint8_t            :1;
        uint8_t            :1;
    }Bit;
}UnionLStatus;
//链表节点（注意：头结点的数据域用于存储链表的长度）
typedef struct LNode
{
    ElemType Data; //数据
    struct LNode *next; //指向下一个节点的指针
}LNode,*LinkList;

LStatus InitList(LinkList *L);
LStatus ListHeadInsert(LinkList L,ElemType Data);
LStatus ListTailInsert(LinkList L,ElemType Data);
LStatus PrintList(LinkList L);
uint16_t ListLength(LinkList L);
LNode *GetElem(LinkList L,ElemType Elem);
LNode *LocateElem(LinkList L,uint8_t n);
LStatus ListInsert(LinkList L,uint8_t n, ElemType Data);
LStatus List_Delete(LinkList L,uint8_t n);





#endif //MY_FREERTOS_TEMPLATE_DATA_STRUCTURE_H
