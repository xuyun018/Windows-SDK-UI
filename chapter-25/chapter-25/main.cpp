#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawCircle(HDC, int, int, int, int);
void ShowText(HDC);
WCHAR szClassNme[] = L"grph03";

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
    int n;

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        for (n = 0; n <= 4; n++) {
            ShowText(hdc);
        }
        for (n = 0; n <= 200; n += 5) {
            DrawCircle(hdc, n, n, n + 100, n + 150);
        }
        EndPaint(hWnd, &ps);
        break;
    default:
        return(DefWindowProc(hWnd, msg, wParam, lParam));
    }
    return (0L);
}

void DrawCircle(HDC hdc, int x1, int y1, int x2, int y2)
{
    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;

    hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    hOldPen = (HPEN)SelectObject(hdc, hPen);
    hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Ellipse(hdc, x1, y1, x2, y2);
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);

    return;
}

void ShowText(HDC hdc)
{
    TextOut(hdc, 10, 20, L"猫でもわかるプログラミング", 13);
    TextOut(hdc, 10, 40, L"制作・著作　粂井康孝", 10);
}