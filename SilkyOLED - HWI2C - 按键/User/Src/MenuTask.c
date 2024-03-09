//
// Created by 86159 on 2024-02-19.
//
#include "MenuTask.h"
#include "retarget.h"
#include "MyKey.h"
#include "stdlib.h"
#include "Delay.h"
#include "MyBMP.h"

UnionLStatus LinkStatus;


/**
 * 初始化单链表
 * @param L 单链表的头指针的地址（二级指针）
 * @return  LERROR：内春不足（可适当加大FreeRTOSConfig.h文件中的堆的大小） LSUCCESS：一切正常
 */
LStatus InitList(TaskLinkList *L)
{
    //注意：这里不能用malloc，因为Freertos
    *L = (TaskLinkList)ListMalloc(sizeof(LTask)); //生成一个表头
    if(L == NULL)
    {
        LinkStatus.Bit.LIST_HEAP_OVERFLOW = 1; //堆溢出
        return LERROR;
    }
    (*L)->Head = NULL;
    (*L)->LinkListTaskNum = 0;  //头结点的数据域用于存储链表的长度
    LinkStatus.Bit.LIST_EMPTY = 1; //单链表为空
    return LSUCCESS;
}



/**
 * 头插法插入数据（反向插入）
 * @param L    单链表的头指针
 * @param Data 要插入的数据
 * @return     工作状态
 */
LStatus ListHeadInsert(TaskLinkList L,ElemType Data)
{
    TaskNode p = (TaskNode)ListMalloc(sizeof(LNode));
    if(p == NULL)
    {
        LinkStatus.Bit.LIST_HEAP_OVERFLOW = 1; //堆溢出
        return LERROR;
    }
    p->next=NULL;
    p->prior=NULL;
    p->Task = Data;
    if(L->Head==NULL) //为空链表
    {
        L->Head = p;
    }
    else//不为空链表
    {
        p->next = L->Head->next;
        L->Head->next = p;
        p->next->prior=p;
        p->prior=L->Head;
    }
    L->LinkListTaskNum++; //链表长度+1

    if(LinkStatus.Bit.LIST_EMPTY == 1) LinkStatus.Bit.LIST_EMPTY = 0;//不为空链表
    return LSUCCESS;
}
/**
 * 尾插法插入节点（正向插入）
 * @param L    双向链表的头指针
 * @param Data 插入的数据
 * @return     工作状态
 */
TaskNode ListTailInsert(TaskLinkList *L,ElemType Data)
{
    TaskNode p = (TaskNode)ListMalloc(sizeof(LNode)),q; //q用于找到链表的尾结点
    if(p == NULL) return LERROR;
    p->next = NULL;
    p->prior = NULL;
    p->Task = Data;
    if((*L)->Head == NULL) //为双向空链表时
    {
        (*L)->Head = p;
        (*L)->Head->next = (*L)->Head;
        (*L)->Head->prior = (*L)->Head;
    }
    else
    {
        q = (*L)->Head->prior;
        (*L)->Head->prior = p;
        q->next = p;
        p->next = (*L)->Head;
        p->prior = q;

    }
    if(LinkStatus.Bit.LIST_EMPTY == 1) LinkStatus.Bit.LIST_EMPTY = 0;//不为空链表
    (*L)->LinkListTaskNum += 1; //链表长度+1
    return p;
}


/**
 * 遍历并打印链表
 * @param L 链表的头指针
 * @return 工作状态
 */
LStatus PrintList(TaskLinkList L)
{
    uint8_t i = 1;
    TaskNode q = L->Head;
    printf("The TaskLinkList length is %d\r\n",L->LinkListTaskNum);
    if(q == NULL)
    {
        printf("This is an empty TaskLinkList!\r\n");
        return LERROR;
    }
    while(q->next != NULL)
    {
        printf("第%d个任务名称为：%s\r\n",i,q->Task.Name);
        i++;
        q = q->next;
    }
    return LSUCCESS;
}

uint16_t ListLength(TaskLinkList L)
{
    uint16_t i = 0;
    TaskNode q = L->Head;
    while(q != NULL)
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
LNode *GetElem(TaskLinkList L,ElemType Elem)
{
    TaskNode p;
    TaskNode q = L->Head;
    if(LinkStatus.Bit.LIST_EMPTY == 1) return NULL; //链表为空
    while(q != NULL)
    {
        if(q->Task.Name == Elem.Name)
        {
            return q;
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
LNode *LocateElem(TaskLinkList L,uint8_t n)
{
    TaskNode q = L->Head;
    if(L->LinkListTaskNum < n) return NULL;//超过了链表的长度
    for(uint8_t i = 1; i < n; i++)
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
LStatus ListInsert(TaskLinkList L,uint8_t n, ElemType Data)
{
    uint8_t i = 0;
    TaskNode q = L->Head,r;
    TaskNode p = (TaskNode)ListMalloc(sizeof(LNode)); //生成一个要插入的结点
    if(p == NULL)
    {
        LinkStatus.Bit.LIST_HEAP_OVERFLOW = 1; //堆溢出
        return LERROR;
    }
    if(n > (L->LinkListTaskNum+1)) return LERROR; //单链表太短了，不能插入到n这个结点
    p->next = NULL;
    p->Task = Data;

    for(i = 0;i < n-1;i++)
    {
        q = q->next;
    }
    r = q->next;
    q->next = p;
    p->prior = q;
    p->next = r;
    r->prior = p;
}
/**
 * 删除第n个结点
 * @param L 单链表头指针
 * @param n 删除掉的第n个结点
 * @return  工作状态
 */
LStatus List_Delete(TaskLinkList L,uint8_t n)
{
    TaskNode p,q = L->Head;
    uint8_t i = 0;
    if(L->Head == NULL)
    {
        LinkStatus.Bit.LIST_EMPTY = 1; //空链表
        return LERROR;
    }
    if(n>L->LinkListTaskNum) return LERROR;
    for(i = 0;i < n-1;i++)
    {
        q = q->next;
    }
    p = q->next; //p是要删除的结点
    q->next = p->next;
    p->next->prior = q;
    ListFree(p); //释放结点
    return LSUCCESS;
}



/**
 * @brief 调整菜单中任务的位置
 * @param Direction 1：向下移动  0：向上移动
 * @param Position  当前List1的位置
 */
void AdjustPosition(MenuHandle *MENU,uint8_t Direction, uint8_t *Position)
{
    //向上移动
    if(!Direction)
    {
        if((*Position) <= 0)(*Position) = MENU->MenuTaskNum - 1;
        else (*Position)--;
    }
        //向下移动
    else
    {
        if((*Position) >= (MENU->MenuTaskNum - 1)) (*Position) = 0;
        else (*Position)++;
    }
}

/**
 * @brief 改变所有任务的x和y坐标的值
 * @param L             任务的双向链表
 * @param XVariation    X坐标改变值（0-127）
 * @param YVariation    Y坐标改变值（0-63）
 * @param XDecrease      1：减少    0：增加
 * @param YDecrease      1：减少    0：增加
 */
void ChangeAllTaskPosition(TaskLinkList *L,u8g2_uint_t XVariation,u8g2_uint_t YVariation,uint8_t XDecrease,uint8_t YDecrease)
{
    uint8_t i = 0;
    int16_t ChangeXNum = 0,ChangeYNum = 0;
    TaskNode p = (*L)->Head; //获取头结点
    if(XDecrease == 1)  ChangeXNum = -XVariation; //X减少
    else ChangeXNum = XVariation; //X增加
    if(YDecrease == 1) ChangeYNum = -YVariation;//Y减少
    else ChangeYNum = YVariation;//Y增加

    for(i = 0; i < (*L)->LinkListTaskNum; i++)
    {
        p->Task.YPosition += ChangeYNum;
        p->Task.XPosition += ChangeXNum;
        p = p->next;
    }
}

/**
 * @brief 菜单切换函数--V2.0
 * @param u8g2       u8g2句柄
 * @param MENU       MENU菜单句柄
 * @param L          菜单中的任务双向链表
 * @param ToNextTask 1:菜单往下移动一格
 * @param ToLastTask 1:菜单往上移动一格
 * @param EnterTask  1:进入选定的任务
 */
void ReMenuSwitch(u8g2_t *u8g2,MenuHandle *MENU,TaskLinkList *L,uint8_t ToNextTask,uint8_t ToLastTask,uint8_t EnterTask)
{
    uint16_t k = 0;
    static uint8_t StartFlag = 0;
    //按下按键三 （进入选中的任务）
    if(EnterTask == 1)
    {
        (*L)->Head->Task.TaskFun(u8g2); //执行对应的任务函数
    }
    else
    {
        //按下按键二，选中框(任务链表的头结点)向上移动
        if(ToLastTask == 1)
        {
            (*L)->Head = (*L)->Head->prior;

            //说明前一个头指针指向的任务在屏幕能显示的第一个任务之前
            if((*L)->Head->next == MENU->FirstTaskInScreen)
            {
                MENU->LastTaskInScreen = MENU->LastTaskInScreen->prior;
                MENU->FirstTaskInScreen = MENU->FirstTaskInScreen->prior;
            }
        }
            //按下按键一，选中框向下移动
        else if(ToNextTask == 1)
        {
            (*L)->Head = (*L)->Head->next;

            //说明前一个头指针指向的任务在屏幕能显示的最后一个任务之后
            if((*L)->Head->prior == MENU->LastTaskInScreen)
            {
                MENU->LastTaskInScreen = MENU->LastTaskInScreen->next;
                MENU->FirstTaskInScreen = MENU->FirstTaskInScreen->next;
            }
//        if((*L)->Head == MENU->FirstTaskInAllTask)
//        {
//            for(k = 0; k < MENU->TaskNumInScreen-1; k++) //这里需要-1是因为上面那个if也符合了，所有也执行了一次
//            {
//                MENU->LastTaskInScreen = MENU->LastTaskInScreen->next;
//                MENU->FirstTaskInScreen = MENU->FirstTaskInScreen->next;
//            }
//        }
        }

        u8g2_FirstPage(u8g2);
        TaskNode p = MENU->FirstTaskInScreen; //获取屏幕显示的第一个任务
        do
        {
            k = 0;
            u8g2_SetDrawColor(u8g2,1);
            u8g2_SetFont(u8g2,MENU->Font); //设置字体
            while(p != MENU->LastTaskInScreen)
            {
                p->Task.YPosition = MENU->FontHeight+MENU->TaskHeight*k+MENU->TaskGap/2;
                u8g2_DrawStr(u8g2,p->Task.XPosition,p->Task.YPosition,(char*)p->Task.Name);
                p = p->next;
                k++;
            }
            p->Task.YPosition = MENU->FontHeight+MENU->TaskHeight*k+MENU->TaskGap/2;
            u8g2_DrawStr(u8g2,p->Task.XPosition,p->Task.YPosition,(char*)p->Task.Name);


            u8g2_DrawRBox(u8g2,(*L)->Head->Task.XPosition-3,(*L)->Head->Task.YPosition-MENU->FontHeight-MENU->TaskGap/2,11*(*L)->Head->Task.NameLength+2*3,MENU->FontHeight+MENU->TaskGap,(MENU->FontHeight+MENU->TaskGap)/2);
            u8g2_SetDrawColor(u8g2,0);
            u8g2_DrawStr(u8g2,MENU->TaskStartX,(*L)->Head->Task.YPosition,(char*)(*L)->Head->Task.Name);
        }while (u8g2_NextPage(u8g2));
    }
}

/**
 * @brief 菜单切换函数---V1.0
 * @param u8g2 u8g2句柄
 */
/*
void MenuSwitch(u8g2_t *u8g2,MenuHandle *MENU)
{
    static uint8_t MenuPosition = 0; //List1的位置
    static uint16_t LastMaxNum = 0;
    uint8_t List1StartYPosition = 0;//List1的起始位置（y）
    uint16_t k = 0;

    AdjustPosition(MENU,hKey.KeyLooseFlag,&MenuPosition);
    uint16_t CurrentNum = MenuPosition*MENU->TaskHeight;
    //printf("MenuPosition:%d",MenuPosition);
    u8g2_SetFont(u8g2,MENU->Font); //设置字体
    //向上移动
    if(hKey2.KeyLooseFlag == 1)
    {
        if(MenuPosition == (MENU->MenuTaskNum - 1))   //List1从位置0向上移动到位置3(最后的位置)
        {
            List1StartYPosition = MENU->FontHeight;
            for(k = 0; k <= (MENU->FontHeight+MENU->SwitchSpeed); k+=MENU->SwitchSpeed)
            {
                u8g2_FirstPage(u8g2);
                do
                {
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*0 - k,(const char*)MENU->TASK[0].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*1 - k,(const char*)MENU->TASK[1].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*2 - k,(const char*)MENU->TASK[2].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*3 - k,(const char*)MENU->TASK[3].Name);

                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*0 - k + OLEDHeight,(const char*)MENU->TASK[0].Name);
                    */
/*下面的三个都超出了范围实际没显示*//*

//                    u8g2_DrawStr(&TestU8G2,MENU->ListStartX,List1StartYPosition+(FontHeight+ListGap)*1 - k + OLEDHeight,"List2");
//                    u8g2_DrawStr(&TestU8G2,MENU->ListStartX,List1StartYPosition+(FontHeight+ListGap)*2 - k + OLEDHeight,"List3");
//                    u8g2_DrawStr(&TestU8G2,MENU->ListStartX,List1StartYPosition+(FontHeight+ListGap)*3 - k + OLEDHeight,"List4");

                    u8g2_DrawRFrame(u8g2,0,15,60,18,3);
                    //Delay_ms(50);
                }while (u8g2_NextPage(u8g2));
            }
        }
        else
        {
            List1StartYPosition = MENU->FontHeight;
            for(k = LastMaxNum; k >= (CurrentNum+MENU->SwitchSpeed) ; k-=MENU->SwitchSpeed) //防止k为负数
            {
                u8g2_FirstPage(u8g2);
                do
                {
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*0 + k,(const char*)MENU->TASK[0].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*1 + k,(const char*)MENU->TASK[1].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*2 + k,(const char*)MENU->TASK[2].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*3 + k,(const char*)MENU->TASK[3].Name);

                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*0 + k - OLEDHeight,(const char*)MENU->TASK[0].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*1 + k - OLEDHeight,(const char*)MENU->TASK[1].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*2 + k - OLEDHeight,(const char*)MENU->TASK[2].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*3 + k - OLEDHeight,(const char*)MENU->TASK[3].Name);

                    u8g2_DrawRFrame(u8g2,0,15,60,18,3);
                    //Delay_ms(50);
                }while (u8g2_NextPage(u8g2));
            }
        }

    }
        //向下移动
    else
    {
        if(MenuPosition == 0)   //List1从位置3向下移动到位置0
        {
            List1StartYPosition = 0;
            for(k = 0; k <= (uint16_t)(MENU->FontHeight+MENU->SwitchSpeed); k+=MENU->SwitchSpeed)
            {
                u8g2_FirstPage(u8g2);
                do
                {
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*0 + k,(const char*)MENU->TASK[0].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*1 + k,(const char*)MENU->TASK[1].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*2 + k,(const char*)MENU->TASK[2].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*3 + k,(const char*)MENU->TASK[3].Name);

                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*0  + k + OLEDHeight,(const char*)MENU->TASK[0].Name);
                    */
/*下面的三个都超出了范围实际没显示*//*

//                    u8g2_DrawStr(&TestU8G2,MENU->ListStartX,List1StartYPosition+(FontHeight+ListGap)*1 + k + OLEDHeight,(const char*)MENU->TASK[1].Name);
//                    u8g2_DrawStr(&TestU8G2,MENU->ListStartX,List1StartYPosition+(FontHeight+ListGap)*2 + k + OLEDHeight,(const char*)MENU->TASK[2].Name);
//                    u8g2_DrawStr(&TestU8G2,MENU->ListStartX,List1StartYPosition+(FontHeight+ListGap)*3 + k + OLEDHeight,(const char*)MENU->TASK[3].Name);

                    u8g2_DrawRFrame(u8g2,0,15,60,18,3);
                    //Delay_ms(50);
                }while (u8g2_NextPage(u8g2));
            }
        }
        else
        {
            List1StartYPosition = MENU->FontHeight;
            for(k = LastMaxNum; k <= (CurrentNum+MENU->SwitchSpeed); k+=MENU->SwitchSpeed)
            {
                u8g2_FirstPage(u8g2);
                do
                {
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*0 + k,(const char*)MENU->TASK[0].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*1 + k,(const char*)MENU->TASK[1].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*2 + k,(const char*)MENU->TASK[2].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*3 + k,(const char*)MENU->TASK[3].Name);

                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*0 + k - OLEDHeight,(const char*)MENU->TASK[0].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*1 + k - OLEDHeight,(const char*)MENU->TASK[1].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*2 + k - OLEDHeight,(const char*)MENU->TASK[2].Name);
                    u8g2_DrawStr(u8g2,MENU->TaskStartX,List1StartYPosition+MENU->TaskHeight*3 + k - OLEDHeight,(const char*)MENU->TASK[3].Name);

                    u8g2_DrawRFrame(u8g2,0,15,60,18,3);
                    //Delay_ms(50);
                }while (u8g2_NextPage(u8g2));
            }
        }

    }

    LastMaxNum = CurrentNum;
}
*/

/**
 * @brief 计算字符串的长度（0-255）
 * @param String 字符串
 * @return 返回字符串的长度
 */
uint8_t My_StringLength(const uint8_t *String)
{
    uint8_t cnt = 0;
    while(*(String+cnt) != '\0')
    {
        cnt++;
    }
    return cnt;
}
void MenuTask1(u8g2_t *u8g2)
{
    /*What you want to do when click task1*/
    u8g2_FirstPage(u8g2);
    do
    {
        u8g2_SetDrawColor(u8g2,1);
        u8g2_DrawXBMP(u8g2,32,0,64,64,BMP_YoKo);
    }while (u8g2_NextPage(u8g2));


    /*END*/
}

void MenuTask2(u8g2_t *u8g2)
{
    /*What you want to do when click task2*/
    u8g2_FirstPage(u8g2);
    do
    {
        u8g2_SetDrawColor(u8g2,1);
        u8g2_DrawXBMP(u8g2,0,0,102,64,BMP_Geass);
    }while (u8g2_NextPage(u8g2));


    /*END*/
}

void MenuTask3(u8g2_t *u8g2)
{
    /*What you want to do when click task3*/
    u8g2_FirstPage(u8g2);
    do
    {
        u8g2_SetDrawColor(u8g2,1);
        u8g2_DrawXBMP(u8g2,32,0,64,64,BMP_Boy);
    }while (u8g2_NextPage(u8g2));


    /*END*/
}

void MenuTask4(u8g2_t *u8g2)
{
    /*What you want to do when click task4*/
    u8g2_FirstPage(u8g2);
    do
    {
        u8g2_SetDrawColor(u8g2,1);
        u8g2_DrawXBMP(u8g2,0,0,94,64,BMP_Dog);
    }while (u8g2_NextPage(u8g2));


    /*END*/
}

void MenuTask5(u8g2_t *u8g2)
{
    /*What you want to do when click task4*/
    u8g2_FirstPage(u8g2);
    do
    {
        u8g2_SetDrawColor(u8g2,1);
        u8g2_SetFont(u8g2, u8g2_font_fub11_tf); //字库选择
        u8g2_DrawStr(u8g2, 0, 13, "Running Task5!");
    }while (u8g2_NextPage(u8g2));


    /*END*/
}

void MenuTask6(u8g2_t *u8g2)
{
    /*What you want to do when click task4*/
    u8g2_FirstPage(u8g2);
    do
    {
        u8g2_SetDrawColor(u8g2,1);
        u8g2_SetFont(u8g2, u8g2_font_fub11_tf); //字库选择
        u8g2_DrawStr(u8g2, 0, 13, "Running Task6!");
    }while (u8g2_NextPage(u8g2));


    /*END*/
}
/**
 * @brief 任务的初始化
 * @param TASK 任务句柄
 * @param Name 任务的名称
 * @param TaskFun 在该任务中执行的函数的函数指针
 */
TaskNode TaskInit(TaskLinkList *L,uint8_t *Name, void(*TaskFun)(u8g2_t *u8g2),uint8_t XPosition,uint8_t YPosition)
{
    TaskHandle TASK;
    TASK.Name = Name;
    TASK.NameLength = My_StringLength(Name);
    TASK.XPosition = XPosition;
    TASK.YPosition = YPosition;
    TASK.TaskFun = TaskFun;
    return ListTailInsert(L,TASK);
}

/**
 * @brief 菜单初始化
 * @param u8g2 u8g2句柄
 * @param MENU MENU句柄
 * @param Font 菜单显示的字体
 */
void MenuInit(u8g2_t *u8g2,MenuHandle *MENU,TaskLinkList *L,const uint8_t *Font)
{
    MENU->TaskStartX = 3;                               //菜单栏最左边的坐标x
    MENU->FontHeight = 11;                              //字体高度
    MENU->TaskGap = 5;                                  //每一项任务的间隔宽度
    MENU->TaskHeight = MENU->FontHeight+MENU->TaskGap;  //字体加上间隙的总宽度
    MENU->TaskStartY = MENU->FontHeight;                //菜单栏最左边的坐标x
    MENU->MenuTaskNum = TASKNum;                        //菜单中总的栏数
    MENU->TaskNumInScreen = TASKNumInScreen;
    MENU->SwitchSpeed = 2;                              //按下按键时菜单滚动的速度（越接近1速度越快 取值范围：1-5）
    MENU->Font = Font;                                  //菜单的字体

    /*初始化菜单的双向链表*/
    InitList(L);

    /*菜单中使用到的所有任务的初始化*/
    MENU->FirstTaskInAllTask = MENU->FirstTaskInScreen = TaskInit(L,(uint8_t*)"Yoko",MenuTask1,MENU->TaskStartX,11+MENU->TaskGap/2);
    TaskInit(L,(uint8_t*)"Geass",MenuTask2,MENU->TaskStartX,27+MENU->TaskGap/2);
    TaskInit(L,(uint8_t*)"Boy",MenuTask3,MENU->TaskStartX,43+MENU->TaskGap/2);
    MENU->LastTaskInScreen = TaskInit(L,(uint8_t*)"Dog",MenuTask4,MENU->TaskStartX,59+MENU->TaskGap/2);
    TaskInit(L,(uint8_t*)"Task5",MenuTask5,MENU->TaskStartX,75+MENU->TaskGap/2);
    TaskInit(L,(uint8_t*)"Task6",MenuTask6,MENU->TaskStartX,75+MENU->TaskGap/2);

    u8g2_FirstPage(u8g2);
    u8g2_SetFont(u8g2,MENU->Font);

    TaskNode p = (*L)->Head; //获取头结点
    do
    {
        for(uint8_t i = 0; i < MENU->MenuTaskNum; i++)
        {
            u8g2_DrawStr(u8g2,MENU->TaskStartX,MENU->TaskStartY+MENU->TaskHeight*i,(char*)p->Task.Name);
            p = p->next;
        }
        u8g2_DrawRBox(u8g2,(*L)->Head->Task.XPosition-3,(*L)->Head->Task.YPosition-MENU->FontHeight-MENU->TaskGap/2,11*(*L)->Head->Task.NameLength+2*3,MENU->FontHeight+MENU->TaskGap,(MENU->FontHeight+MENU->TaskGap)/2);
        u8g2_SetFont(u8g2,MENU->Font); //设置字体
        u8g2_SetDrawColor(u8g2,0);
        u8g2_DrawStr(u8g2,MENU->TaskStartX,(*L)->Head->Task.YPosition,(char*)(*L)->Head->Task.Name);
        Delay_ms(50);
    }while (u8g2_NextPage(u8g2));
}
