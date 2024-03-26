//        rgn01.cpp

#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
void DrawMyBMP(HWND, HDC, int);
HRGN MakeMyClipRgn(int, BITMAP);

const WCHAR szClassName[] = L"rgn01";        //窗口类名


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
    wc.lpfnWndProc = WndProc;    //プロシージャ名
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;        //インスタンス
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = L"MYMENU";    //メニュー名
    wc.lpszClassName = (LPCWSTR)szClassName;
    return (RegisterClass(&wc));
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"猫友好区域",    //タイトルバーにこの名前が表示されます
        WS_OVERLAPPEDWINDOW,    //ウィンドウの種類
        CW_USEDEFAULT,    //Ｘ座標
        CW_USEDEFAULT,    //Ｙ座標
        195,    //幅
        163,    //高さ
        NULL,            //親ウィンドウのハンドル、親を作るときはNULL
        NULL,            //メニューハンドル、クラスメニューを使うときはNULL
        hInst,            //インスタンスハンドル
        NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    static int sw; //裁剪区域形状
    int id;
    HDC hdc;
    PAINTSTRUCT ps;

    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDM_RCT:
            sw = 1;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_ELL:
            sw = 2;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_ELLOR:
            sw = 3;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_ELLXOR:
            sw = 4;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_END:
            SendMessage(hWnd, WM_CLOSE, 0L, 0L);
            break;
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        DrawMyBMP(hWnd, hdc, sw);
        EndPaint(hWnd, &ps);
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
        PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0L;
}

void DrawMyBMP(HWND hWnd, HDC hdc, int sw)
{
    HINSTANCE hInst;
    HBITMAP hBitmap;
    BITMAP bm;
    HDC hmdc;
    LONG bmp_w, bmp_h;
    HRGN hRgn;

    hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
    hBitmap = LoadBitmap(hInst, L"MYBMP");
    GetObject(hBitmap, sizeof(BITMAP), &bm);
    bmp_w = bm.bmWidth;
    bmp_h = bm.bmHeight;
    hRgn = MakeMyClipRgn(sw, bm);
    SelectClipRgn(hdc, hRgn);
    hmdc = CreateCompatibleDC(hdc);
    SelectObject(hmdc, hBitmap);
    BitBlt(hdc, 0, 0, (int)bmp_w, (int)bmp_h, hmdc, 0, 0, SRCCOPY);
    DeleteObject(hRgn);
    DeleteDC(hmdc);
    DeleteObject(hBitmap);
    return;
}

HRGN MakeMyClipRgn(int sw, BITMAP bitmap)
{
    int bmp_w, bmp_h;
    bmp_w = (int)bitmap.bmWidth;
    bmp_h = (int)bitmap.bmHeight;
    HRGN hRgn, hRgn1, hRgn2;

    int mode;

    switch (sw) {
    case 1:
        return CreateRectRgn(0, 0, bmp_w, bmp_h);
    case 2:
        return CreateEllipticRgn(0, 0, bmp_w, bmp_h);
    case 3:
    case 4:
        hRgn = CreateRectRgn(0, 0, 10, 10);
        hRgn1 = CreateEllipticRgn(0, 0, (bmp_w / 3) * 2, bmp_h);
        hRgn2 = CreateEllipticRgn(bmp_w / 3, 0, bmp_w, bmp_h);
        if (sw == 3)
        {
            mode = RGN_OR;
        }
        else
        {
            mode = RGN_XOR;
        }
        CombineRgn(hRgn, hRgn1, hRgn2, mode);
        DeleteObject(hRgn1);
        DeleteObject(hRgn2);
        return hRgn;
    default:
        return NULL;
    }
}