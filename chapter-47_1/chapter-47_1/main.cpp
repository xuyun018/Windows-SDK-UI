//        omake.cpp

#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

const WCHAR szClassName[] = L"omake";        //窗口类名

int d_w, d_h;


int WINAPI wWinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
    LPWSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    srand((unsigned)time(NULL));

    if (!hPrevInst) {
        if (!InitApp(hCurInst))
            return FALSE;
    }
    if (!InitInstance(hCurInst, nCmdShow)) {
        return FALSE;
    }
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

BOOL InitApp(HINSTANCE hInst)
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;    //过程名称
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;        //实例
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;    //菜单名
    wc.lpszClassName = (LPCWSTR)szClassName;
    return (RegisterClass(&wc));
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"Bonus", //此名称将显示在标题栏中
        WS_OVERLAPPEDWINDOW, //窗口类型
        300,    //Ｘ座標
        300,    //Ｙ座標
        100,    //幅
        100, // 高度
        NULL, //父窗口句柄，创建父窗口时为NULL
        NULL, //菜单句柄，使用类菜单时为NULL
        hInst, //实例句柄
        NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
    static HDC hdc;
    HPEN hPen;
    HWND hDesktop;
    RECT rc;
    int x1, x2, y1, y2;
    int c1, c2, c3;
    static int d_w, d_h;

    switch (msg) {
    case WM_CREATE:
        hdc = CreateDC(L"DISPLAY", NULL, NULL, NULL);
        SetTimer(hWnd, 100, 100, NULL);
        hDesktop = GetDesktopWindow();
        GetWindowRect(hDesktop, &rc);
        d_w = rc.right;
        d_h = rc.bottom;
        break;
    case WM_TIMER:
        x1 = rand() % d_w;
        x2 = rand() % d_w;
        y1 = rand() % d_h;
        y2 = rand() % d_h;
        c1 = rand() % 256;
        c2 = rand() % 256;
        c3 = rand() % 256;

        hPen = CreatePen(PS_SOLID, 6, RGB(c1, c2, c3));
        SelectObject(hdc, hPen);
        MoveToEx(hdc, x1, y1, NULL);
        LineTo(hdc, x2, y2);
        DeleteObject(hPen);
        break;
    case WM_CLOSE:
        id = MessageBox(hWnd,
            (LPCWSTR)L"您确定要退出吗？",
            (LPCWSTR)L"終了確認",
            MB_YESNO | MB_ICONQUESTION);
        if (id == IDYES) {
            DestroyWindow(hWnd);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 100);
        DeleteDC(hdc);
        PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0L;
}

