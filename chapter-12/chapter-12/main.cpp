#include <windows.h>

#include "sample02.h"

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

    hWnd = CreateWindow(szClassNme,
        L"猫也能理解的编程",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
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
    int id;

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
            MessageBox(hWnd, L"打开", L"", MB_OK);
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
