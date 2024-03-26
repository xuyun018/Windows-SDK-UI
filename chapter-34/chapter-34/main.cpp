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

int CalcAge(HWND);	//年齢を計算して表示

WCHAR szClassName[] = L"edit01";        //ウィンドウクラス


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
        L"猫でもわかるプログラミング",    //タイトルバーにこの名前が表示されます
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
    PAINTSTRUCT ps;
    HDC hdc;
    HINSTANCE hInst;

    switch (msg) {
    case WM_CREATE:
        hInst = ((LPCREATESTRUCT)lp)->hInstance;
        hEWnd1 = CreateWindow(WC_EDIT,
            L"在此输入",
            WS_CHILD | WS_VISIBLE,
            60, 10,
            100, 20,
            hWnd,
            (HMENU)ID_EDIT1,
            hInst,
            NULL);
        hEWnd2 = CreateWindow(WC_EDIT,
            L"YYYY",
            WS_CHILD | WS_VISIBLE,
            150, 30,
            100, 20,
            hWnd,
            (HMENU)ID_EDIT2,
            hInst,
            NULL);
        hBWnd1 = CreateWindow(WC_BUTTON,
            L"开始计算",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 50,
            100, 30,
            hWnd,
            (HMENU)ID_BUTTON1,
            hInst,
            NULL);
        hBWnd2 = CreateWindow(WC_BUTTON,
            L"清除",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            130, 50,
            100, 30,
            hWnd,
            (HMENU)ID_BUTTON2,
            hInst,
            NULL);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 10, 10, (LPCWSTR)L"姓名：", 3);
        TextOut(hdc, 10, 30, (LPCWSTR)L"出生年份（西历）：", 9);
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case ID_BUTTON1:
            GetWindowText(hEWnd1, (LPWSTR)name, 30);
            GetWindowText(hEWnd2, (LPWSTR)birth, 30);
            CalcAge(hWnd);
            break;
        case ID_BUTTON2:
            SetWindowText(hEWnd1, NULL);
            SetWindowText(hEWnd2, NULL);
            break;
        default:
            return(DefWindowProc(hWnd, msg, wp, lp));
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
        return (DefWindowProc(hWnd, msg, wp, lp));
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

