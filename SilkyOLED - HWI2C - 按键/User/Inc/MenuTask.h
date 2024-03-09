//
// Created by 86159 on 2024-02-19.
//

#ifndef MY_FLYBACK_MENUTASK_H
#define MY_FLYBACK_MENUTASK_H

#include "main.h"
#define NOFREERTOS   //FREERTOS

#ifdef FREERTOS
#define ListFree    vPortFree
#define ListMalloc  pvPortMalloc
#endif

#ifdef NOFREERTOS
#define ListFree    free
#define ListMalloc  malloc
#endif

#define ElemType TaskHandle   //定义链表中的数据类型

//#define FontHeight  (11)    //菜单字体的高度
//#define ListGap     (5)     //菜单中每一项之间的上下间隔
//#define SwitchSpeed (2)     //切换菜单的速度（越接近1速度越快）
//#define MenuTaskNum (4)     //菜单中任务的项数
#define OLEDHeight    (64)    //OLED的高度
#define OLEDWidth     (128)   //OLED的宽度
#define TASKNum       (6)     //菜单中的项目数（总栏数）
#define TASKNumInScreen (4)

typedef struct
{
    uint8_t *Name;          //任务名称
    uint8_t NameLength;     //任务名称的长度（0-255）
    u8g2_uint_t XPosition;      //初始化时在OLED显示中的任务字符串的左下角X坐标
    u8g2_uint_t YPosition;      //当时屏幕中显示的Y坐标
    void(*TaskFun)(u8g2_t *u8g2);   //选定后执行任务的函数指针
}TaskHandle;


//链表节点（注意：头结点的数据域用于存储链表的长度）
typedef struct LinkNode
{
    TaskHandle Task;        //任务句柄
    struct LinkNode *next;  //指向后一个任务结点
    struct LinkNode *prior; //指向前一个任务结点
}*TaskNode,LNode;

typedef struct
{
    uint8_t LinkListTaskNum;//链表中的任务总数
    LNode *Head;         //头结点指针
}*TaskLinkList,LTask;

typedef struct
{
    uint8_t  FontHeight;         //字体高度
    uint8_t  TaskGap;            //每一项任务的间隔宽度
    uint8_t  TaskHeight;         //字体加上间隙的总宽度

    uint8_t  MenuTaskNum;        //菜单中总的栏数
    uint8_t  TaskNumInScreen;    //在屏幕中能够显示的最大任务数

    uint8_t  SwitchSpeed;        //按下按键时菜单滚动的速度（越接近1速度越快 取值范围：1-5）

    TaskNode FirstTaskInScreen;  //屏幕中的第一个任务
    TaskNode LastTaskInScreen;   //屏幕中的最后一个任务
    TaskNode FirstTaskInAllTask; //在菜单中的所有任务的第一个任务

    u8g2_uint_t  TaskStartX;         //菜单栏最左边的坐标x
    u8g2_uint_t  TaskStartY;         //菜单栏最左边的坐标y
    const uint8_t *Font;         //菜单的字体
}MenuHandle;

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

LStatus InitList(TaskLinkList *L);
LStatus ListHeadInsert(TaskLinkList L,ElemType Data);
TaskNode ListTailInsert(TaskLinkList *L,ElemType Data);
LStatus PrintList(TaskLinkList L);
uint16_t ListLength(TaskLinkList L);
LNode *GetElem(TaskLinkList L,ElemType Elem);
LNode *LocateElem(TaskLinkList L,uint8_t n);
LStatus ListInsert(TaskLinkList L,uint8_t n, ElemType Data);
LStatus List_Delete(TaskLinkList L,uint8_t n);

void MenuInit(u8g2_t *u8g2,MenuHandle *MENU,TaskLinkList *L,const uint8_t *Font);
void MenuSwitch(u8g2_t *u8g2,MenuHandle *MENU);
uint8_t My_StringLength(const uint8_t *String);
TaskNode TaskInit(TaskLinkList *L,uint8_t *Name, void(*TaskFun)(u8g2_t *u8g2),uint8_t XPosition,uint8_t YPosition);
void ReMenuSwitch(u8g2_t *u8g2,MenuHandle *MENU,TaskLinkList *L,uint8_t ToNextTask,uint8_t ToLastTask,uint8_t EnterTask);
void Re3MenuSwitch(u8g2_t *u8g2,MenuHandle *MENU,TaskLinkList *L);


void MenuTask1(u8g2_t *u8g2);
void MenuTask2(u8g2_t *u8g2);
void MenuTask3(u8g2_t *u8g2);
void MenuTask4(u8g2_t *u8g2);




#endif //MY_FLYBACK_MENUTASK_H
