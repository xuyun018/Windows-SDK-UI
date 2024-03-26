#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

void DrawPic(HWND, HDC, int);

const WCHAR szClassName[] = L"sdk44";        //窗口类
HWND hParent;  //保存父窗口句柄
HACCEL hAccel; //加速器表句柄

enum { clr, cat, dog, rat };

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
    wc.hInstance = hInst;        //实例句柄
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;    //菜单名称
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
            200, // 宽度
            100, // 高度
            NULL, //父窗口句柄，创建父窗口时为NULL
            NULL, //菜单句柄，使用类菜单时为NULL
            hInst, //实例句柄
            NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    hParent = hWnd; //保存窗口句柄（不要忘记）
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
    static int sw = 0;
    POINT pt;
    HMENU hmenu, hSubmenu;
    HDC hdc;
    PAINTSTRUCT ps;

    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDM_END:
            SendMessage(hWnd, WM_CLOSE, 0L, 0L);
            break;
        case IDM_CAT:
            sw = cat;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_DOG:
            sw = dog;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_RAT:
            sw = rat;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_CLR:
            sw = clr;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        default:
            break;
        }
        break;
    case WM_RBUTTONDOWN:
        pt.x = LOWORD(lp);
        pt.y = HIWORD(lp);
        hmenu = LoadMenu((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), L"MYPOPUP");
        hSubmenu = GetSubMenu(hmenu, 0);
        ClientToScreen(hWnd, &pt);
        TrackPopupMenu(hSubmenu, TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
        DestroyMenu(hmenu);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        DrawPic(hWnd, hdc, sw);
        EndPaint(hWnd, &ps);
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

void DrawPic(HWND hWnd, HDC hdc, int sw)
{
    HDC hmdc;
    HBITMAP hBitmap;
    BITMAP bmp;
    HINSTANCE hInst;
    int BMP_W, BMP_H;

    hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
    switch (sw) {
    case clr:
        return;
    case cat:
        hBitmap = LoadBitmap(hInst, L"MYCAT");
        break;
    case dog:
        hBitmap = LoadBitmap(hInst, L"MYDOG");
        break;
    case rat:
        hBitmap = LoadBitmap(hInst, L"MYRAT");
        break;
    default:
        hBitmap = NULL;
        break;
    }
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    BMP_W = (int)bmp.bmWidth;
    BMP_H = (int)bmp.bmHeight;
    hmdc = CreateCompatibleDC(hdc);
    SelectObject(hmdc, hBitmap);
    BitBlt(hdc, 0, 0, BMP_W, BMP_H, hmdc, 0, 0, SRCCOPY);
    DeleteDC(hmdc);
    return;
}