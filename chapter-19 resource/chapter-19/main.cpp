#include <windows.h>

#include "timer.h"
#include "sample02.h"

#define ScrMax 100    //滚动旋钮的最大值
#define ScrMin 0    //最小值

//リストボックス
LPCWSTR liststr[] = { L"粂井康孝", L"粂井ひとみ", L"粂井櫻都", L"粂井志麻", L"マイケル" };
//コンボボックス
LPCWSTR combostr[] = { L"東京", L"北海道", L"大阪", L"名古屋" };

int initDlg(HWND);
int getDlg(HWND);
int ShowScroll(HWND);
int ShowStatic(HWND);
int ShowText(HWND);
int SetWinCenter(HWND);

WCHAR time_str[256];    //获取到的时间复制到哪​​里
WCHAR edit_str[256]; //存储编辑框中输入的字符串
int check1 = 0;
int check2 = 0; //检查按钮是否按下
int radio1 = 1;
int radio2 = 0; //单选按钮是否按下

int ScrPos = 0, ListNo = 0, ComboNo = 0;

int SetWinCenter(HWND hWnd)
{
    HWND hDeskWnd;
    RECT deskrc, rc;
    int x, y;

    hDeskWnd = GetDesktopWindow();
    GetWindowRect(hDeskWnd, (LPRECT)&deskrc);
    GetWindowRect(hWnd, (LPRECT)&rc);
    x = (deskrc.right - (rc.right - rc.left)) / 2;
    y = (deskrc.bottom - (rc.bottom - rc.top)) / 2;
    SetWindowPos(hWnd, HWND_TOP, x, y, (rc.right - rc.left), (rc.bottom - rc.top), SWP_SHOWWINDOW);
    return 0;
}

int getDlg(HWND hDlgWnd)
{
    GetWindowText(GetDlgItem(hDlgWnd, IDC_EDIT1), edit_str, sizeof(edit_str));
    //GetDlgItemText(hDlgWnd, IDC_EDIT1, edit_str, sizeof(edit_str));
    ScrPos = GetScrollPos(GetDlgItem(hDlgWnd, IDC_SCROLL1), SB_CTL);
    ListNo = (int)(DWORD)SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), LB_GETCURSEL, 0L, 0L);
    ComboNo = (int)(DWORD)SendMessage(GetDlgItem(hDlgWnd, IDC_COMBO1), CB_GETCURSEL, 0L, 0L);
    return 0;
}

int ShowStatic(HWND hWnd)
{
    WCHAR str[8];
    wsprintf(str, L"%3d", ScrPos);
    SetWindowText(GetDlgItem(hWnd, IDC_STATIC1), (LPCWSTR)str);
    return 0;
}

int ShowScroll(HWND hWnd)
{
    if (ScrPos < ScrMin)
        ScrPos = ScrMin;
    if (ScrPos > ScrMax)
        ScrPos = ScrMax;
    SetScrollPos(GetDlgItem(hWnd, IDC_SCROLL1), SB_CTL, ScrPos, TRUE);
    ShowStatic(hWnd);
    return 0;
}

int initDlg(HWND hDlgWnd)
{
    int i;
    SetWindowText(GetDlgItem(hDlgWnd, IDC_EDIT1), (LPCTSTR)edit_str);
    SetScrollRange(GetDlgItem(hDlgWnd, IDC_SCROLL1), SB_CTL, ScrMin, ScrMax, TRUE);
    ShowScroll(hDlgWnd);
    for (i = 0; i <= 4; i++)
        SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), LB_INSERTSTRING, (WPARAM)i, (LPARAM)liststr[i]);
    SendMessage(GetDlgItem(hDlgWnd, IDC_LIST1), LB_SETCURSEL, (WPARAM)ListNo, 0L);
    for (i = 0; i <= 3; i++)
        SendMessage(GetDlgItem(hDlgWnd, IDC_COMBO1), CB_INSERTSTRING, (WPARAM)i, (LPARAM)combostr[i]);
    SendMessage(GetDlgItem(hDlgWnd, IDC_COMBO1), CB_SETCURSEL, (WPARAM)ComboNo, 0L);
    return 0;
}

int ShowText(HWND hWnd)
{
    HDC hdc;
    RECT rt;
    PAINTSTRUCT ps;
    WCHAR str[256];
    const WCHAR * str_org = L"编辑框＝%s\n"
        L"列表框＝%s\n"
        L"组合框＝%s\n"
        L"滚动条＝%3d";
    wsprintf(str, str_org, edit_str,
        liststr[ListNo],
        combostr[ComboNo],
        ScrPos);
    GetClientRect(hWnd, &rt);
    hdc = BeginPaint(hWnd, &ps);
    DrawText(hdc, str, -1, &rt, DT_WORDBREAK);
    EndPaint(hWnd, &ps);
    return 0;
}

LRESULT CALLBACK MyDlgProc(HWND hDlgWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_INITDIALOG:
        SetWinCenter(hDlgWnd);
        initDlg(hDlgWnd);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            getDlg(hDlgWnd);
            EndDialog(hDlgWnd, IDOK);
            break;
        case IDCANCEL:
            EndDialog(hDlgWnd, IDCANCEL);
            break;
        default:
            return FALSE;
        }
        break;
    case WM_HSCROLL:
        switch (LOWORD(wParam)) {
        case SB_LINELEFT:
            ScrPos--;
            break;
        case SB_LINERIGHT:
            ScrPos++;
            break;
        case SB_PAGELEFT:
            ScrPos -= 10;
            break;
        case SB_PAGERIGHT:
            ScrPos += 10;
            break;
        case SB_THUMBTRACK:
            ScrPos = (int)(HIWORD(wParam));
            break;
        }
        ShowScroll(hDlgWnd);
        break;
    default:
        break;
    }
    //return(DefWindowProc(hDlgWnd, msg, wParam, lParam));
    return(0);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
WCHAR szClassNme[] = L"CatClassName";

UINT idTimer;

int GetTimeStr(HWND hWnd)
{
    PAINTSTRUCT ps;
    const WCHAR* str_org = L"现在是%2d月%4d年%2d日%2d小时%2d分%2d秒";

    SYSTEMTIME st;

    WCHAR text[256];
    GetLocalTime(&st);
    wsprintf(text, str_org, st.wMonth, st.wYear, st.wDay, st.wHour, st.wMinute, st.wSecond);
    SetWindowText(hWnd, text);
    //HDC hdc = BeginPaint(hWnd, &ps);
    //TextOut(hdc, 10, 10, (LPCWSTR)text, wcslen(text));
    //EndPaint(hWnd, &ps);
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInst,
    LPWSTR lpszCmdLine, int nCmdShow)
{
    HWND hWnd;
    MSG msg;
    WNDCLASS myProg;
    if (!hPreInst) {
        myProg.style = CS_HREDRAW | CS_VREDRAW;
        myProg.lpfnWndProc = WndProc;
        myProg.cbClsExtra = 0;
        myProg.cbWndExtra = 0;
        myProg.hInstance = hInstance;
        myProg.hIcon = NULL;
        myProg.hCursor = LoadCursor(NULL, IDC_ARROW);
        myProg.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        myProg.lpszMenuName = L"MYMENU";
        myProg.lpszClassName = szClassNme;
        if (!RegisterClass(&myProg))
            return FALSE;
    }

    int width = 800;
    int height = 600;

    hWnd = CreateWindow(szClassNme,
        L"猫也能理解的编程",
        WS_OVERLAPPEDWINDOW,
        (GetSystemMetrics(SM_CXSCREEN) - width) >> 1,
        (GetSystemMetrics(SM_CYSCREEN) - height) >> 1,
        width,
        height,
        NULL,
        NULL,
        hInstance,
        NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hinstance;
    int id;
    WCHAR str[256];
    const WCHAR* str_org = L"编辑框＝%s\n"
        L"单选按钮 1 = %d，按钮 2 = %d\n"
        L"复选框 1 = %d，复选框 2 = %d";

    switch (msg) {
    case WM_CREATE:
        if (1)
        {
            //SetWinCenter(hWnd);

            idTimer = SetTimer(hWnd, 1, 1000, NULL);
        }
        break;
    case WM_TIMER:
        if (idTimer == wParam)
        {
            KillTimer(hWnd, idTimer);
            //InvalidateRect(hWnd, NULL, FALSE);
            GetTimeStr(hWnd);
            idTimer = SetTimer(hWnd, 1, 1000, NULL);
        }
        break;
    case WM_PAINT:
        break;
    case WM_SYSCOMMAND:
        if (1)
        {
            int uCmdType = wParam;
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);

            switch (uCmdType) {
            case IDM_NEW:
                MessageBox(hWnd,
                    (LPCWSTR)L"已选择测试项目。",
                    (LPCWSTR)L"测试",
                    MB_OK);
                break;
            default:
                break;
            }
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_NEW:
            MessageBox(hWnd, L"新建", L"", MB_OK);
            break;
        case IDM_DLG:
            //MessageBox(hWnd, L"打开", L"", MB_OK);
            hinstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
            id = DialogBox(hinstance, L"MYDIALOG", hWnd, (DLGPROC)MyDlgProc);
            if (id == IDOK) {
                wsprintf((LPTSTR)str,
                    (LPCTSTR)str_org,
                    edit_str, radio1, radio2, check1, check2);
                MessageBox(hWnd, (LPCWSTR)str, (LPCWSTR)L"显示", MB_OK);
            }
            break;
        case IDM_EXIT:
            SendMessage(hWnd, WM_CLOSE, 0, 0L);
            break;
        default:
            break;
        }
        break;
    case WM_CLOSE:
        id = MessageBox(hWnd,
            (LPCWSTR)L"你想退出吗？",
            (LPCWSTR)L"完成确认",
            MB_OKCANCEL | MB_ICONQUESTION);
        if (id == IDOK)
            DestroyWindow(hWnd);
        return (0L);
    case WM_DESTROY:
        if (idTimer)
        {
            KillTimer(hWnd, idTimer);
        }

        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return(DefWindowProc(hWnd, msg, wParam, lParam));
}
