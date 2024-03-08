//
// Created by 86159 on 2023-11-22.
//

#include "Data_Structure.h"
#include "cmsis_os.h"
#include "retarget.h"
#include "stdlib.h"

#define FREERTOS   //NOFREERTOS

#ifdef FREERTOS
#define ListFree    vPortFree
#define ListMalloc  pvPortMalloc
#endif

#ifdef NOFREERTOS
#define ListFree    free
#define ListMalloc  malloc
#endif

UnionLStatus LinkStatus;

/**
 * 初始化单链表
 * @param L 单链表的头指针的地址（二级指针）
 * @return  LERROR：内春不足（可适当加大FreeRTOSConfig.h文件中的堆的大小） LSUCCESS：一切正常
 */
LStatus InitList(LinkList *L)
{
    //注意：这里不能用malloc，因为Freertos
    *L = (LinkList)ListMalloc(sizeof(LNode)); //生成一个头结点
    if(L == NULL)
    {
        LinkStatus.Bit.LIST_HEAP_OVERFLOW = 1; //堆溢出
        return LERROR;
    }
    (*L)->next = NULL;
    (*L)->Data = 0;  //头结点的数据域用于存储链表的长度
    LinkStatus.Bit.LIST_EMPTY = 1; //单链表为空
    return LSUCCESS;
}
/**
 * 头插法插入数据（反向插入）
 * @param L    单链表的头指针
 * @param Data 要插入的数据
 * @return     工作状态
 */
LStatus ListHeadInsert(LinkList L,ElemType Data)
{
    LinkList p = (LinkList)ListMalloc(sizeof(LNode));
    if(p == NULL)
    {
        LinkStatus.Bit.LIST_HEAP_OVERFLOW = 1; //堆溢出
        return LERROR;
    }
    p->next=NULL;
    p->Data = Data;
    if(L->next==NULL) //为空链表
    {
        L->next = p;
    }
    else//不为空链表
    {
        p->next = L->next;
        L->next = p;
    }
    L->Data++; //链表长度+1

    if(LinkStatus.Bit.LIST_EMPTY == 1) LinkStatus.Bit.LIST_EMPTY = 0;//不为空链表
    return LSUCCESS;
}
/**
 * 尾插法插入节点（正向插入）
 * @param L    链表的头指针
 * @param Data 插入的数据
 * @return     工作状态
 */
LStatus ListTailInsert(LinkList L,ElemType Data)
{
    LinkList p = (LinkList)ListMalloc(sizeof(LNode)),q; //q用于找到链表的尾结点
    if(p == NULL) return LERROR;
    p->next = NULL;
    p->Data = Data;
    q = L;
    while(q->next != NULL)
    {
        q = q->next;
    }
    q->next = p;
    L->Data += 1; //链表长度+1
    if(LinkStatus.Bit.LIST_EMPTY == 1) LinkStatus.Bit.LIST_EMPTY = 0;//不为空链表
    return LSUCCESS;
}
/**
 * 遍历并打印链表
 * @param L 链表的头指针
 * @return 工作状态
 */
LStatus PrintList(LinkList L)
{
    uint8_t i = 1;
    LinkList q = L;
    printf("The LinkList length is %d\r\n",L->Data);
    if(q->next == NULL)
    {
        printf("This is an empty LinkList!\r\n");
        return LERROR;
    }
    while(q->next != NULL)
    {
        printf("第%d个数据为：%d\r\n",i,q->next->Data);
        i++;
        q = q->next;
    }
    return LSUCCESS;
}

uint16_t ListLength(LinkList L)
{
    uint16_t i = 0;
    LinkList q = L;
    while(q->next != NULL)
    {
        i++;
        q = q->next;
    }
    return i;
}

/**
 * 按照结点的值来查找到节点并返回该节点的地址
 * @param L    单链表头指针
 * @param Elem 要找到的元素的值
 * @return     返回查找到的结点的地址，若无则为NULL
 */
LNode *GetElem(LinkList L,ElemType Elem)
{
    LNode *p;
    LinkList q = L;
    if(LinkStatus.Bit.LIST_EMPTY == 1) return NULL; //链表为空
    while(q->next != NULL)
    {
        if(q->next->Data == Elem)
        {
            return q->next;
        }
        q = q->next;
    }
    return NULL; //链表中没有该元素
}
/**
 * 按照位置顺序来查找结点
 * @param L 单链表头指针
 * @param n 要查找的结点的位置（从1开始）
 * @return  返回查找到的结点的地址，若无则为NULL
 */
LNode *LocateElem(LinkList L,uint8_t n)
{
    LinkList q = L;
    if(L->Data < n) return NULL;//超过了链表的长度
    for(uint8_t i = 0; i < n; i++)
    {
        q = q->next;
    }
    return q;
}

/**
 * 将一个数据为Data的新结点插入到第n个位置
 * @param L    单链表的头指针
 * @param n    要插入的第n个位置
 * @param Data 要插入的值
 * @return     工作状态
 */
LStatus ListInsert(LinkList L,uint8_t n, ElemType Data)
{
    uint8_t i = 0;
    LinkList q = L,r;
    LinkList p = (LinkList)ListMalloc(sizeof(LNode)); //生成一个要插入的结点
    if(p == NULL)
    {
        LinkStatus.Bit.LIST_HEAP_OVERFLOW = 1; //堆溢出
        return LERROR;
    }
    if(n > (L->Data+1)) return LERROR; //单链表太短了，不能插入到n这个结点
    p->next = NULL;
    p->Data = Data;

    for(i = 0;i < n-1;i++)
    {
        q = q->next;
    }
    r = q->next;
    q->next = p;
    p->next = r;
}
/**
 * 删除第n个结点
 * @param L 单链表头指针
 * @param n 删除掉的第n个结点
 * @return  工作状态
 */
LStatus List_Delete(LinkList L,uint8_t n)
{
    LinkList p,q = L;
    uint8_t i = 0;
    if(L->next == NULL)
    {
        LinkStatus.Bit.LIST_EMPTY = 1; //空链表
        return LERROR;
    }
    if(n>L->Data) return LERROR;
    for(i = 0;i < n-1;i++)
    {
        q = q->next;
    }
    p = q->next; //p是要删除的结点
    q->next = p->next;
    ListFree(p); //释放结点
}









