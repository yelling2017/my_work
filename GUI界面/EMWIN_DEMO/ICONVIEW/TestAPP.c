#include "TestAPP.h"
#include "DIALOG.h"
#include "FRAMEWIN.h"
#include "rtc.h"

//BagAPP内部小控件ID
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x02)
#define ID_LISTVIEW_0 (GUI_ID_USER + 0x03)
#define ID_BUTTON_0 (GUI_ID_USER + 0x04)

//BookAPP内部小控件ID
#define ID_FRAMEWIN_1   (GUI_ID_USER + 0x05)
#define ID_TEXT_1       (GUI_ID_USER + 0x06)

//BookshAPP内部小控件ID
#define ID_FRAMEWIN_2   (GUI_ID_USER + 0x07)
#define ID_TEXT_2       (GUI_ID_USER + 0x08)

//BrowerAPP内部小控件ID
#define ID_FRAMEWIN_3   (GUI_ID_USER + 0x09)
#define ID_TEXT_3       (GUI_ID_USER + 0x10)

WM_HWIN BagAPPWin = NULL;       //BagAPP窗口
WM_HWIN BookAPPWin = NULL;      //BookAPP窗口
WM_HWIN BookshAPPWin = NULL;    //BookshAPP窗口
WM_HWIN BrowerAPPWin = NULL;    //BrowerAPP窗口

Medicine_info medicine;//药品
int listviewitem = -1;  //LISTVIEW选中的项目编号
//LISTVIEW列表
static const char *ListViewTable[][4] = {
    {"NAOH",	"20", "g", "A"},
    {"HON3",	"20", "g", "B"},
    {"HCL",	"20", "g", "C"},
    {"H2SO4",	"20", "g", "B"},
    {"H2SO3",	"20", "g", "E"},
    {"HGCL2",	"20", "g", "A"},
    {"CA(OH)2",	"20", "g", "B"},
    {"AL2O3",	"20", "g", "D"},
};

//BagAPP资源表
static const GUI_WIDGET_CREATE_INFO BagAPPDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 900, 480, FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
    { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 45, 15, 800, 380, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 700, 400, 100, 40, 0, 0x0, 0 },
};

//BookAPP资源表
static const GUI_WIDGET_CREATE_INFO BookAPPDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_1, 100, 0, 320, 240, FRAMEWIN_CF_MOVEABLE, 0x64},
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 70, 85, 200, 20, 0, 0x64, 0 },
};

//BookshAPP资源表
static const GUI_WIDGET_CREATE_INFO BookshAPPDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_2, 0, 0, 200, 100, FRAMEWIN_CF_TITLEVIS, 0x64},
    { TEXT_CreateIndirect, "DATE:",     ID_TEXT_1, 0, 0, 69, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "TIME:",     ID_TEXT_2, 0, 35, 65, 20, 0, 0x64, 0 },
};

//BrowerAPP资源表
static const GUI_WIDGET_CREATE_INFO BrowerAPPDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_3, 80, 0, 640, 480, 0, 0x64},
    { TEXT_CreateIndirect, "Text", ID_TEXT_3, 200, 230, 200, 20, 0, 0x64, 0 },
};

//BagAPP的回调函数
static void _cbBagAPPDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    WM_MESSAGE Msg;
    u8 i = 0;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        //初始化FRAMWIN
        hItem = pMsg->hWin;
        FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        FRAMEWIN_SetFont(hItem, GUI_FONT_20B_ASCII);
        FRAMEWIN_SetTextColor(hItem, GUI_YELLOW);
        FRAMEWIN_SetText(hItem, "medicine list");
        FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2); //添加关闭按钮
        //FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
        FRAMEWIN_AddMinButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2); //添加最小化按钮

        //初始化LISTVIEW
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
        LISTVIEW_SetGridVis(hItem, 1);              //网格线可见
        LISTVIEW_SetAutoScrollH(hItem, 1);          //启用自动水平滚动条
        LISTVIEW_SetAutoScrollV(hItem, 1);          //启用自动垂直滚动条
        LISTVIEW_SetHeaderHeight(hItem, 20);        //设置标题的高度
        LISTVIEW_SetRowHeight(hItem, 50);           //设置LISTVIEW的行高
        LISTVIEW_SetBkColor(hItem, 0, GUI_WHITE);  //设置背景颜色
        LISTVIEW_SetFont(hItem, GUI_FONT_32B_ASCII);
        //向LISTVIEW添加列
        LISTVIEW_AddColumn(hItem, 200, "Name", GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(hItem, 320, "Num", GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(hItem, 160, "Uint", GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(hItem, 140, "Level", GUI_TA_HCENTER | GUI_TA_VCENTER);
        //向LISTVIEW添加行
        for(i = 0; i < GUI_COUNTOF(ListViewTable); i++)
        {
            LISTVIEW_AddRow(hItem, ListViewTable[i]);
        }

        //初始化BUTTON
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
        BUTTON_SetFont(hItem, GUI_FONT_COMIC18B_ASCII);
        BUTTON_SetText(hItem, "OK");
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id)
        {
        case ID_LISTVIEW_0:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
                listviewitem = LISTVIEW_GetSel(hItem); //获取选中的项目编号
                //返回指定单元格的文本
                LISTVIEW_GetItemText(hItem, 0, listviewitem, (char*)medicine.name, 20); //获取名字
                LISTVIEW_GetItemText(hItem, 1, listviewitem, (char*)medicine.num, 20); //获取数量
                LISTVIEW_GetItemText(hItem, 2, listviewitem, (char*)(medicine.unit), 4); //获取单位
                LISTVIEW_GetItemText(hItem, 3, listviewitem, (char*) & (medicine.level), 4); //获取等级
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                break;

            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

//BookAPP的回调函数
static void _cbBookAPPDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        GUI_SetBkColor(GUI_RED);
        GUI_Clear();
        break;
    case WM_INIT_DIALOG:
        //初始化FRAMWIN
        hItem = pMsg->hWin;
        FRAMEWIN_SetText(hItem, "Book APP");
        FRAMEWIN_SetFont(hItem, GUI_FONT_20_ASCII);
        FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2); //添加关闭按钮
        //FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
        FRAMEWIN_AddMinButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2); //添加最小化按钮

        //初始化TEXT
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
        TEXT_SetText(hItem, "This is Book APP");
        TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        if(NCode == WM_NOTIFICATION_CHILD_DELETED) BookAPPWin = 0; //子窗口删除了
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}

//BookshAPP的回调函数
static void _cbBookshAPPDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;

    static u16 adcvalue = 0;
    u8 tbuf[20];
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        GUI_SetColor(GUI_RED);
        GUI_SetFont(GUI_FONT_24B_ASCII);
        //显示日期
        HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN); //获取日期
        sprintf((char*)tbuf, "20%02d-%02d-%02d", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Date);
        GUI_DispStringAt((const char*)tbuf, 70, 0); //显示日期

        //显示时间
        HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN); //获取时间
        sprintf((char*)tbuf, "%02d:%02d:%02d", RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
        GUI_DispStringAt((const char*)tbuf, 70, 35); //显示日期
        break;
    case WM_INIT_DIALOG:
        //FRAMWIN初始化
        hItem = pMsg->hWin;
        FRAMEWIN_SetFont(hItem, GUI_FONT_20B_ASCII);
        FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        FRAMEWIN_SetText(hItem, "TIME");
        FRAMEWIN_SetTextColor(hItem, 0x0000FFFF);

        //日期初始化
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
        TEXT_SetText(hItem, "DATE:");

        //时间初始化
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
        TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
        TEXT_SetText(hItem, "TIME:");
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

//BrowerAPP的回调函数
static void _cbBrowerAPPDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        GUI_SetBkColor(GUI_YELLOW);
        GUI_Clear();
        break;
    case WM_INIT_DIALOG:
        //初始化FRAMWIN
        hItem = pMsg->hWin;
        FRAMEWIN_SetText(hItem, "Brower APP");
        FRAMEWIN_SetFont(hItem, GUI_FONT_20_ASCII);
        FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2); //添加关闭按钮
        //FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //添加最大化按钮
        FRAMEWIN_AddMinButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2); //添加最小化按钮

        //初始化TEXT
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
        TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
        TEXT_SetText(hItem, "This is Brower APP");
        TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        if(NCode == WM_NOTIFICATION_CHILD_DELETED) BrowerAPPWin = 0; //子窗口删除了
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}

//创建BagAPP
void CreateBagAPP(void)
{
    BagAPPWin = GUI_CreateDialogBox(BagAPPDialogCreate, GUI_COUNTOF(BagAPPDialogCreate), _cbBagAPPDialog, WM_HBKWIN, 100, 120);
}

//创建BookAPP
void CreateBookAPP(void)
{
    BookAPPWin = GUI_CreateDialogBox(BookAPPDialogCreate, GUI_COUNTOF(BookAPPDialogCreate), _cbBookAPPDialog, WM_HBKWIN, 0, 120);
}

//创建BookshAPP
void CreateBookshAPP(void)
{
    BookshAPPWin = GUI_CreateDialogBox(BookshAPPDialogCreate, GUI_COUNTOF(BookshAPPDialogCreate), _cbBookshAPPDialog, WM_HBKWIN, 820, 0);
    while(1)
    {
        GUI_Delay(100);
        WM_InvalidateWindow(BookshAPPWin); //对话框窗口无效
    }
}

//创建BrowerAPP
void CreateBrowerAPP(void)
{
    BrowerAPPWin = GUI_CreateDialogBox(BrowerAPPDialogCreate, GUI_COUNTOF(BrowerAPPDialogCreate), _cbBrowerAPPDialog, WM_HBKWIN, 0, 120);
}
