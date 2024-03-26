#include <windows.h>

#include "sample02.h"

#include "form_main.h"

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

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    MSG msg;
    struct form_main pwnd[1];
    HWND hwnd;
    int width = 800;
    int height = 600;

    hwnd = form_create(pwnd, NULL, hInstance, NULL,
        (GetSystemMetrics(SM_CXSCREEN) - width) >> 1, (GetSystemMetrics(SM_CYSCREEN) - height) >> 1, width, height,
        L"MyWindowClass", L"猫也能理解的编程");

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}
