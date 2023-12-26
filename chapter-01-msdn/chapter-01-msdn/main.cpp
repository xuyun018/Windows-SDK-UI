#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
WCHAR szClassNme[] = L"ウィンドウクラス・ネーム";

void ShowMyText(HWND hWnd)
{
    HDC hdc;
    PAINTSTRUCT paint;
    const WCHAR * str = L"猫也能理解的编程";

    hdc = BeginPaint(hWnd, &paint);
    SetTextColor(hdc, RGB(255, 0, 0));
    TextOut(hdc, 10, 10, (LPCWSTR)str, wcslen(str));
    EndPaint(hWnd, &paint);
    return;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInst,
    LPWSTR lpszCmdLine, int nCmdShow)
{
    HWND hWnd;
    MSG msg;
    WNDCLASS myProg;

    if (1 || !hPreInst) {
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
        
        RegisterClass(&myProg);
    }
    hWnd = CreateWindow(myProg.lpszClassName,
        L"猫でもわかるプログラミング",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);
    //ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE:
        break;
    case WM_PAINT:
        ShowMyText(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return(DefWindowProc(hWnd, msg, wParam, lParam));
    }
    return (0L);
}
