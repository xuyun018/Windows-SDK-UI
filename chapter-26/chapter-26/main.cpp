#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ShowMyBMP(HWND, HDC);
WCHAR szClassNme[] = L"grph04";

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInst,
    LPWSTR lpszCmdLine, int nCmdShow)
{
    HWND hWnd;
    MSG lpMsg;
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
        L"猫でもわかるプログラミング",
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
    while (GetMessage(&lpMsg, NULL, 0, 0)) {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }
    return (lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        ShowMyBMP(hWnd, hdc);
        TextOut(hdc, 50, 50, L"猫でもわかるプログラミング", 13);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 0, 0));
        TextOut(hdc, 50, 100, L"粂井康孝 制作・著作", 10);
        EndPaint(hWnd, &ps);
        break;
    default:
        return(DefWindowProc(hWnd, msg, wParam, lParam));
    }
    return (0L);
}

void ShowMyBMP(HWND hWnd, HDC hdc)
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
    StretchBlt(hdc, 0, BMP_H, BMP_W / 2, BMP_H / 2, hmdc, 0, 0, BMP_W, BMP_H, SRCCOPY);
    DeleteDC(hmdc);
    DeleteObject(hBitmap);
    return;
}