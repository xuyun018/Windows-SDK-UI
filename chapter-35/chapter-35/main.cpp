#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <stdlib.h>  //_wtoi関数を使うので必要

#define ID_EDIT1   1000
#define ID_EDIT2    1010
#define ID_BUTTON1    1020
#define ID_BUTTON2    1030

HWND hEWnd1, hEWnd2, hBWnd1, hBWnd2;
WCHAR name[30], birth[30];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

BOOL My_OnCreate(HWND, LPCREATESTRUCT);
void My_OnPaint(HWND);
void My_OnCommand(HWND, int, HWND, UINT);
void My_OnClose(HWND);
void My_OnDestroy(HWND);

int CalcAge(HWND); //年齢を計算して表示

WCHAR szClassName[] = L"edit02";        //ウィンドウクラス

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
    wc.lpszMenuName = NULL;    //メニュー名
    wc.lpszClassName = (LPCWSTR)szClassName;
    return (RegisterClass(&wc));
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"猫也能理解的消息破坏者",    //タイトルバーにこの名前が表示されます
        WS_OVERLAPPED | WS_SYSMENU,    //ウィンドウの種類
        CW_USEDEFAULT,    //Ｘ座標
        CW_USEDEFAULT,    //Ｙ座標
        250,    //幅
        120,    //高さ
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        HANDLE_MSG(hwnd, WM_CREATE, My_OnCreate);
        HANDLE_MSG(hwnd, WM_PAINT, My_OnPaint);
        HANDLE_MSG(hwnd, WM_COMMAND, My_OnCommand);
        HANDLE_MSG(hwnd, WM_CLOSE, My_OnClose);
        HANDLE_MSG(hwnd, WM_DESTROY, My_OnDestroy);
    default:
        return (DefWindowProc(hwnd, msg, wParam, lParam));
    }
    return 0L;
}

int CalcAge(HWND hWnd)
{
    SYSTEMTIME st;
    int age;
    WCHAR str[256];
    const WCHAR* str_org = L"%s %d 岁";

    GetLocalTime(&st);
    age = st.wYear - _wtoi(birth);
    wsprintf(str, str_org, name, age);
    MessageBox(hWnd, str, L"年齢", MB_OK);
    return 0;
}

BOOL My_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    HINSTANCE hInst;
    hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
    hEWnd1 = CreateWindow(WC_EDIT,
        L"在此输入",
        WS_CHILD | WS_VISIBLE,
        60, 10,
        100, 20,
        hwnd,
        (HMENU)ID_EDIT1,
        hInst,
        NULL);
    hEWnd2 = CreateWindow(WC_EDIT,
        L"YYYY",
        WS_CHILD | WS_VISIBLE,
        120, 30,
        100, 20,
        hwnd,
        (HMENU)ID_EDIT2,
        hInst,
        NULL);
    hBWnd1 = CreateWindow(WC_BUTTON,
        L"計算開始",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 50,
        100, 30,
        hwnd,
        (HMENU)ID_BUTTON1,
        hInst,
        NULL);
    hBWnd2 = CreateWindow(WC_BUTTON,
        L"清除",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        130, 50,
        100, 30,
        hwnd,
        (HMENU)ID_BUTTON2,
        hInst,
        NULL);
    return 1L;
}

void My_OnPaint(HWND hwnd)
{
    HDC hdc;
    PAINTSTRUCT ps;

    hdc = BeginPaint(hwnd, &ps);
    TextOut(hdc, 10, 10, (LPCWSTR)L"名前：", 3);
    TextOut(hdc, 10, 30, (LPCWSTR)L"生年（西暦）：", 7);
    EndPaint(hwnd, &ps);
    return;
}

void My_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {
    case ID_BUTTON1:
        GetWindowText(hEWnd1, (LPWSTR)name, 30);
        GetWindowText(hEWnd2, (LPWSTR)birth, 30);
        CalcAge(hwnd);
        break;
    case ID_BUTTON2:
        SetWindowText(hEWnd1, NULL);
        SetWindowText(hEWnd2, NULL);
    }
    return;
}

void My_OnClose(HWND hwnd)
{
    int id;
    id = MessageBox(hwnd,
        (LPCWSTR)L"您确定要退出吗？",
        (LPCWSTR)L"完成确认",
        MB_YESNO | MB_ICONQUESTION);
    if (id == IDYES)
        DestroyWindow(hwnd);
    return;
}

void My_OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
    return;
}