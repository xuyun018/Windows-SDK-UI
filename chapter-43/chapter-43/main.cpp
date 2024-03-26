#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

void DrawCat(HWND, HDC);

const WCHAR szClassName[] = L"sdk43";        //窗口类
HWND hParent; //父窗口句柄
HACCEL hAccel; //加速器表句柄
int paintsw = 0;

int WINAPI wWinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
    LPWSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;

    if (!hPrevInst) {
        if (!InitApp(hCurInst))
            return FALSE;
    }
    if (!InitInstance(hCurInst, nCmdShow)) {
        return FALSE;
    }
    hAccel = LoadAccelerators(hCurInst, L"MYACCEL");
    if (hAccel == NULL)
        MessageBox(NULL, L"失败", L"", MB_OK);
    if (hParent == NULL)
        MessageBox(NULL, L"无句柄", L"", MB_OK);
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(hParent, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
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
    wc.lpszMenuName = L"MYMENU";    //菜单名称
    wc.lpszClassName = (LPCWSTR)szClassName;
    return (RegisterClass(&wc));
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"Cat-Friendly Menu", //此名称将显示在标题栏中
        WS_OVERLAPPEDWINDOW, //窗口类型
        CW_USEDEFAULT, //X坐标
        CW_USEDEFAULT, //Y坐标
        300, // 宽度
        200, //高度
        NULL, //父窗口句柄，创建父窗口时为NULL
        NULL, //菜单句柄，使用类菜单时为NULL
        hInst, //实例句柄
        NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    hParent = hWnd;
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
    HDC hdc;
    PAINTSTRUCT ps;
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDM_END:
            SendMessage(hWnd, WM_CLOSE, 0L, 0L);
            break;
        case IDM_CAT:
            if (paintsw == 1) {
                MessageBox(hWnd,
                    L"图片已经显示",
                    L"注意！",
                    MB_ICONEXCLAMATION | MB_OK);
            }
            else
            {
                paintsw = 1;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        case IDM_CLR:
            if (paintsw == 0) {
                MessageBox(hWnd,
                    L"没有可删除的图片",
                    L"注意！",
                    MB_ICONEXCLAMATION | MB_OK);
            }
            else
            {
                paintsw = 0;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        default:
            break;
        }
        break;
    case WM_PAINT:
        if (paintsw == 1) {
            hdc = BeginPaint(hWnd, &ps);
            DrawCat(hWnd, hdc);
            EndPaint(hWnd, &ps);
        }
        else {
            //
        }
        break;
    case WM_CLOSE:
        id = MessageBox(hWnd,
            (LPCWSTR)L"您确定要退出吗？",
            (LPCWSTR)L"完成确认",
            MB_YESNO | MB_ICONQUESTION);
        if (id == IDYES) {
            DestroyWindow(hWnd);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return (DefWindowProc(hWnd, msg, wp, lp));
}

void DrawCat(HWND hWnd, HDC hdc)
{
    HDC hmdc;
    HBITMAP hBitmap;
    BITMAP bmp;
    HINSTANCE hInst;
    int BMP_W, BMP_H;

    hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
    hBitmap = LoadBitmap(hInst, L"MYBMP");
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    BMP_W = (int)bmp.bmWidth;
    BMP_H = (int)bmp.bmHeight;
    hmdc = CreateCompatibleDC(hdc);
    SelectObject(hmdc, hBitmap);
    BitBlt(hdc, 0, 0, BMP_W, BMP_H, hmdc, 0, 0, SRCCOPY);
    DeleteDC(hmdc);
    DeleteObject(hBitmap);
    return;
}