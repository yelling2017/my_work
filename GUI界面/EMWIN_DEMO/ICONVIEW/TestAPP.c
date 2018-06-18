#include "TestAPP.h"
#include "DIALOG.h"
#include "FRAMEWIN.h"
#include "rtc.h"

//BagAPP�ڲ�С�ؼ�ID
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x02)
#define ID_LISTVIEW_0 (GUI_ID_USER + 0x03)
#define ID_BUTTON_0 (GUI_ID_USER + 0x04)

//BookAPP�ڲ�С�ؼ�ID
#define ID_FRAMEWIN_1   (GUI_ID_USER + 0x05)
#define ID_TEXT_1       (GUI_ID_USER + 0x06)

//BookshAPP�ڲ�С�ؼ�ID
#define ID_FRAMEWIN_2   (GUI_ID_USER + 0x07)
#define ID_TEXT_2       (GUI_ID_USER + 0x08)

//BrowerAPP�ڲ�С�ؼ�ID
#define ID_FRAMEWIN_3   (GUI_ID_USER + 0x09)
#define ID_TEXT_3       (GUI_ID_USER + 0x10)

WM_HWIN BagAPPWin = NULL;       //BagAPP����
WM_HWIN BookAPPWin = NULL;      //BookAPP����
WM_HWIN BookshAPPWin = NULL;    //BookshAPP����
WM_HWIN BrowerAPPWin = NULL;    //BrowerAPP����

Medicine_info medicine;//ҩƷ
int listviewitem = -1;  //LISTVIEWѡ�е���Ŀ���
//LISTVIEW�б�
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

//BagAPP��Դ��
static const GUI_WIDGET_CREATE_INFO BagAPPDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 900, 480, FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
    { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 45, 15, 800, 380, 0, 0x0, 0 },
    { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 700, 400, 100, 40, 0, 0x0, 0 },
};

//BookAPP��Դ��
static const GUI_WIDGET_CREATE_INFO BookAPPDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_1, 100, 0, 320, 240, FRAMEWIN_CF_MOVEABLE, 0x64},
    { TEXT_CreateIndirect, "Text", ID_TEXT_1, 70, 85, 200, 20, 0, 0x64, 0 },
};

//BookshAPP��Դ��
static const GUI_WIDGET_CREATE_INFO BookshAPPDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_2, 0, 0, 200, 100, FRAMEWIN_CF_TITLEVIS, 0x64},
    { TEXT_CreateIndirect, "DATE:",     ID_TEXT_1, 0, 0, 69, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect, "TIME:",     ID_TEXT_2, 0, 35, 65, 20, 0, 0x64, 0 },
};

//BrowerAPP��Դ��
static const GUI_WIDGET_CREATE_INFO BrowerAPPDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_3, 80, 0, 640, 480, 0, 0x64},
    { TEXT_CreateIndirect, "Text", ID_TEXT_3, 200, 230, 200, 20, 0, 0x64, 0 },
};

//BagAPP�Ļص�����
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
        //��ʼ��FRAMWIN
        hItem = pMsg->hWin;
        FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        FRAMEWIN_SetFont(hItem, GUI_FONT_20B_ASCII);
        FRAMEWIN_SetTextColor(hItem, GUI_YELLOW);
        FRAMEWIN_SetText(hItem, "medicine list");
        FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2); //��ӹرհ�ť
        //FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����󻯰�ť
        FRAMEWIN_AddMinButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2); //�����С����ť

        //��ʼ��LISTVIEW
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
        LISTVIEW_SetGridVis(hItem, 1);              //�����߿ɼ�
        LISTVIEW_SetAutoScrollH(hItem, 1);          //�����Զ�ˮƽ������
        LISTVIEW_SetAutoScrollV(hItem, 1);          //�����Զ���ֱ������
        LISTVIEW_SetHeaderHeight(hItem, 20);        //���ñ���ĸ߶�
        LISTVIEW_SetRowHeight(hItem, 50);           //����LISTVIEW���и�
        LISTVIEW_SetBkColor(hItem, 0, GUI_WHITE);  //���ñ�����ɫ
        LISTVIEW_SetFont(hItem, GUI_FONT_32B_ASCII);
        //��LISTVIEW�����
        LISTVIEW_AddColumn(hItem, 200, "Name", GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(hItem, 320, "Num", GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(hItem, 160, "Uint", GUI_TA_HCENTER | GUI_TA_VCENTER);
        LISTVIEW_AddColumn(hItem, 140, "Level", GUI_TA_HCENTER | GUI_TA_VCENTER);
        //��LISTVIEW�����
        for(i = 0; i < GUI_COUNTOF(ListViewTable); i++)
        {
            LISTVIEW_AddRow(hItem, ListViewTable[i]);
        }

        //��ʼ��BUTTON
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
                listviewitem = LISTVIEW_GetSel(hItem); //��ȡѡ�е���Ŀ���
                //����ָ����Ԫ����ı�
                LISTVIEW_GetItemText(hItem, 0, listviewitem, (char*)medicine.name, 20); //��ȡ����
                LISTVIEW_GetItemText(hItem, 1, listviewitem, (char*)medicine.num, 20); //��ȡ����
                LISTVIEW_GetItemText(hItem, 2, listviewitem, (char*)(medicine.unit), 4); //��ȡ��λ
                LISTVIEW_GetItemText(hItem, 3, listviewitem, (char*) & (medicine.level), 4); //��ȡ�ȼ�
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

//BookAPP�Ļص�����
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
        //��ʼ��FRAMWIN
        hItem = pMsg->hWin;
        FRAMEWIN_SetText(hItem, "Book APP");
        FRAMEWIN_SetFont(hItem, GUI_FONT_20_ASCII);
        FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2); //��ӹرհ�ť
        //FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����󻯰�ť
        FRAMEWIN_AddMinButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2); //�����С����ť

        //��ʼ��TEXT
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
        TEXT_SetText(hItem, "This is Book APP");
        TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        if(NCode == WM_NOTIFICATION_CHILD_DELETED) BookAPPWin = 0; //�Ӵ���ɾ����
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}

//BookshAPP�Ļص�����
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
        //��ʾ����
        HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN); //��ȡ����
        sprintf((char*)tbuf, "20%02d-%02d-%02d", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Date);
        GUI_DispStringAt((const char*)tbuf, 70, 0); //��ʾ����

        //��ʾʱ��
        HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN); //��ȡʱ��
        sprintf((char*)tbuf, "%02d:%02d:%02d", RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
        GUI_DispStringAt((const char*)tbuf, 70, 35); //��ʾ����
        break;
    case WM_INIT_DIALOG:
        //FRAMWIN��ʼ��
        hItem = pMsg->hWin;
        FRAMEWIN_SetFont(hItem, GUI_FONT_20B_ASCII);
        FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        FRAMEWIN_SetText(hItem, "TIME");
        FRAMEWIN_SetTextColor(hItem, 0x0000FFFF);

        //���ڳ�ʼ��
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
        TEXT_SetText(hItem, "DATE:");

        //ʱ���ʼ��
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
        TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
        TEXT_SetText(hItem, "TIME:");
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

//BrowerAPP�Ļص�����
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
        //��ʼ��FRAMWIN
        hItem = pMsg->hWin;
        FRAMEWIN_SetText(hItem, "Brower APP");
        FRAMEWIN_SetFont(hItem, GUI_FONT_20_ASCII);
        FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2); //��ӹرհ�ť
        //FRAMEWIN_AddMaxButton(hItem,FRAMEWIN_BUTTON_RIGHT,2);   //�����󻯰�ť
        FRAMEWIN_AddMinButton(hItem, FRAMEWIN_BUTTON_RIGHT, 2); //�����С����ť

        //��ʼ��TEXT
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
        TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
        TEXT_SetText(hItem, "This is Brower APP");
        TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        if(NCode == WM_NOTIFICATION_CHILD_DELETED) BrowerAPPWin = 0; //�Ӵ���ɾ����
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}

//����BagAPP
void CreateBagAPP(void)
{
    BagAPPWin = GUI_CreateDialogBox(BagAPPDialogCreate, GUI_COUNTOF(BagAPPDialogCreate), _cbBagAPPDialog, WM_HBKWIN, 100, 120);
}

//����BookAPP
void CreateBookAPP(void)
{
    BookAPPWin = GUI_CreateDialogBox(BookAPPDialogCreate, GUI_COUNTOF(BookAPPDialogCreate), _cbBookAPPDialog, WM_HBKWIN, 0, 120);
}

//����BookshAPP
void CreateBookshAPP(void)
{
    BookshAPPWin = GUI_CreateDialogBox(BookshAPPDialogCreate, GUI_COUNTOF(BookshAPPDialogCreate), _cbBookshAPPDialog, WM_HBKWIN, 820, 0);
    while(1)
    {
        GUI_Delay(100);
        WM_InvalidateWindow(BookshAPPWin); //�Ի��򴰿���Ч
    }
}

//����BrowerAPP
void CreateBrowerAPP(void)
{
    BrowerAPPWin = GUI_CreateDialogBox(BrowerAPPDialogCreate, GUI_COUNTOF(BrowerAPPDialogCreate), _cbBrowerAPPDialog, WM_HBKWIN, 0, 120);
}
