#include <windows.h>
#include <windowsx.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE, LPCWSTR);
BOOL InitInstance(HINSTANCE, LPCWSTR, int);

int DrawGr(HDC);

int WINAPI wWinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
    LPWSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    WCHAR szClassName[] = L"grph02";

    if (!hPrevInst) {
        if (!InitApp(hCurInst, szClassName))
            return FALSE;
    }
    if (!InitInstance(hCurInst, szClassName, nCmdShow)) {
        return FALSE;
    }
    while (GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

BOOL InitApp(HINSTANCE hInst, LPCWSTR szClassName)
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = L"GRPH";
    wc.lpszClassName = (LPCWSTR)szClassName;
    return (RegisterClass(&wc));
}

BOOL InitInstance(HINSTANCE hInst, LPCWSTR szClassName, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"猫でもわかるGRPH",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInst,
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
    HDC hdc;
    PAINTSTRUCT ps;

    switch (msg) {
    case WM_COMMAND:
        switch (GET_WM_COMMAND_ID(wp, lp)) {
        case IDM_END:
            SendMessage(hWnd, WM_CLOSE, 0, 0L);
            break;
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        DrawGr(hdc);
        EndPaint(hWnd, &ps);
        break;
    case WM_CLOSE:
        id = MessageBox(hWnd,
            (LPCWSTR)L"終了しますか",
            (LPCWSTR)L"終了確認",
            MB_OKCANCEL | MB_ICONQUESTION);
        if (id == IDOK) {
            DestroyWindow(hWnd);
        }
        if (id == IDCANCEL) {
            return FALSE;
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

// 图形绘制
int DrawGr(HDC hdc)
{
    HPEN hPen;
    HGDIOBJ hOldPen;
    HBRUSH hBrush;
    HGDIOBJ hOldBrush;

    hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    hOldPen = SelectObject(hdc, hPen);
    hBrush = CreateHatchBrush(HS_CROSS, RGB(0, 255, 0));
    hOldBrush = SelectObject(hdc, hBrush);
    Rectangle(hdc, 10, 10, 200, 100);
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);

    hPen = CreatePen(PS_DASH, 1, RGB(255, 100, 10));
    hOldPen = SelectObject(hdc, hPen);
    hBrush = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 0, 255));
    hOldBrush = SelectObject(hdc, hBrush);
    Rectangle(hdc, 40, 40, 240, 140);
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);

    //hPen = CreatePen(PS_DOT, 1, RGB(100, 100, 100));
    //hOldPen = SelectObject(hdc, hPen);
    //SelectObject(hdc, GetStockObject(NULL_BRUSH));
    //Rectangle(hdc, 70, 70, 270, 270);
    //SelectObject(hdc, hOldPen);
    //DeleteObject(hPen);

    hPen = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 255, 0));
    hOldPen = SelectObject(hdc, hPen);
    hBrush = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 0, 255));
    hOldBrush = SelectObject(hdc, hBrush);
    Rectangle(hdc, 100, 100, 200, 160);
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
    return 0;
}