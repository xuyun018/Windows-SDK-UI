#include <windows.h>

#include "sample02.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
WCHAR szClassNme[] = L"CatClassName";

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

    HMENU hmenu;
    HMENU hsubmenu;

    hmenu = CreateMenu();
    hsubmenu = CreatePopupMenu();

    AppendMenu(hsubmenu, MF_STRING, 0, L"新建");
    AppendMenu(hsubmenu, MF_STRING, 0, L"打开");
    AppendMenu(hsubmenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hsubmenu, MF_STRING, 0, L"退出");

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)hsubmenu, L"文件");

    hWnd = CreateWindow(szClassNme,
        L"猫也能理解的编程",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        hmenu,
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
            //
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_END:
            SendMessage(hWnd, WM_CLOSE, 0, 0L);
            break;
        case IDM_TEST:
            MessageBox(hWnd,
                (LPCWSTR)L"テストが押されました",
                (LPCWSTR)L"test",
                MB_OK);
            break;
        case IDM_ABOUT:
            MessageBox(hWnd,
                (LPCWSTR)L"ABOUTが押されました",
                (LPCWSTR)L"About",
                MB_OK);
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
        PostQuitMessage(0);
        break;
    default:
        return(DefWindowProc(hWnd, msg, wParam, lParam));
    }
    return (0L);
}
