#include <windows.h>

#include "sample02.h"

WCHAR time_str[256];    //获取到的时间复制到哪​​里
WCHAR edit_str[256]; //存储编辑框中输入的字符串
int check1 = 0;
int check2 = 0; //检查按钮是否按下
int radio1 = 1;
int radio2 = 0; //单选按钮是否按下

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

LRESULT CALLBACK MyDlgProc(HWND hDlgWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_INITDIALOG:
        SetDlgItemText(hDlgWnd, IDC_EDIT1, (LPCTSTR)edit_str);
        SendMessage(GetDlgItem(hDlgWnd, IDC_CHECK1), BM_SETCHECK, (WPARAM)check1, 0L);
        SendMessage(GetDlgItem(hDlgWnd, IDC_CHECK2), BM_SETCHECK, (WPARAM)check2, 0L);
        SendMessage(GetDlgItem(hDlgWnd, IDC_RADIO1), BM_SETCHECK, (WPARAM)radio1, 0L);
        SendMessage(GetDlgItem(hDlgWnd, IDC_RADIO2), BM_SETCHECK, (WPARAM)radio2, 0L);

        SetWinCenter(hDlgWnd);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            GetDlgItemText(hDlgWnd,
                IDC_EDIT1,
                (LPWSTR)edit_str,
                sizeof(edit_str)/ sizeof(edit_str[0]));
            //Button_GetCheck(hCheckBox)
            if (IsDlgButtonChecked(hDlgWnd, IDC_CHECK1) == BST_CHECKED) {
                check1 = 1;
            }
            else { check1 = 0; }
            if (IsDlgButtonChecked(hDlgWnd, IDC_CHECK2) == BST_CHECKED) {
                check2 = 1;
            }
            else { check2 = 0; }

            if (IsDlgButtonChecked(hDlgWnd, IDC_RADIO1) == BST_CHECKED) {
                radio1 = 1;
            }
            else { radio1 = 0; }
            if (IsDlgButtonChecked(hDlgWnd, IDC_RADIO2) == BST_CHECKED) {
                radio2 = 1;
            }
            else { radio2 = 0; }
            EndDialog(hDlgWnd, IDOK);
            break;
        case IDCANCEL:
            EndDialog(hDlgWnd, IDCANCEL);
            break;
        default:
            return FALSE;
        }
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
        myProg.lpszMenuName = NULL;
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
            HMENU hmenu;
            HMENU hsubmenu;

            hmenu = CreateMenu();
            hsubmenu = CreatePopupMenu();

            AppendMenu(hsubmenu, MF_STRING, IDM_NEW, L"新建");
            AppendMenu(hsubmenu, MF_STRING, IDM_OPEN, L"打开");
            AppendMenu(hsubmenu, MF_SEPARATOR, 0, NULL);
            AppendMenu(hsubmenu, MF_STRING, IDM_EXIT, L"退出");

            AppendMenu(hmenu, MF_POPUP, (UINT_PTR)hsubmenu, L"文件");

            SetMenu(hWnd, hmenu);

            hmenu = GetSystemMenu(hWnd, FALSE);
            for (int i = 0; i < 6; i++)
                DeleteMenu(hmenu, 0, MF_BYPOSITION);
            AppendMenu(hmenu, MF_STRING, IDM_NEW, L"添加项目");
            DrawMenuBar(hWnd);

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
        case IDM_OPEN:
            //MessageBox(hWnd, L"打开", L"", MB_OK);
            hinstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
            id = DialogBox(hinstance, L"OPTDLG", hWnd, (DLGPROC)MyDlgProc);
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
