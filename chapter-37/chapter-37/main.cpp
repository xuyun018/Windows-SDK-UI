#include <windows.h>
#include <CommCtrl.h>
#include <stdlib.h>  //_wtoi関数を使うので必要

#define ID_EDIT1   1000
#define ID_EDIT2    1010
#define ID_BUTTON1    1020
#define ID_BUTTON2    1030

HWND hEWnd1, hEWnd2, hBWnd1, hBWnd2; //标准控件的HWND
WNDPROC Org_EWnd1, Org_EWnd2;   //原始过程地址

WCHAR name[30], birth[30];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MySubProc1(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MySubProc2(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

int CalcAge(HWND); //计算并显示年龄
int focus; //编辑具有焦点的控件号
//1: hEWnd1 2: hEWnd2

WCHAR szClassName[] = L"subcl01";        //窗口类名

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
        L"猫也能理解的子类化",    //タイトルバーにこの名前が表示されます
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
        //子类化窗口
        Org_EWnd1 = (WNDPROC)GetWindowLongPtr(hEWnd1, GWLP_WNDPROC);
        SetWindowLongPtr(hEWnd1, GWLP_WNDPROC, (LONG_PTR)MySubProc1);

        hEWnd2 = CreateWindow(WC_EDIT,
            L"YYYY",
            WS_CHILD | WS_VISIBLE,
            120, 30,
            100, 20,
            hWnd,
            (HMENU)ID_EDIT2,
            hInst,
            NULL);
        // 子类化窗口
        Org_EWnd2 = (WNDPROC)GetWindowLongPtr(hEWnd2, GWLP_WNDPROC);
        SetWindowLongPtr(hEWnd2, GWLP_WNDPROC, (LONG)MySubProc2);

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
        SetFocus(hEWnd1);
        focus = 1;
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 10, 10, (LPCWSTR)L"姓名：", 3);
        TextOut(hdc, 10, 30, (LPCWSTR)L"生年（西暦）：", 7);
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case ID_BUTTON1:
            GetWindowText(hEWnd1, (LPWSTR)name, 30);
            GetWindowText(hEWnd2, (LPWSTR)birth, 30);
            CalcAge(hWnd);
            if (focus == 1)
                SetFocus(hEWnd1);
            if (focus == 2)
                SetFocus(hEWnd2);
            break;
        case ID_BUTTON2:
            SetWindowText(hEWnd1, NULL);
            SetWindowText(hEWnd2, NULL);
            if (focus == 1)
                SetFocus(hEWnd1);
            if (focus == 2)
                SetFocus(hEWnd2);
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
        //完成后取消子类化
        //这不是必需的，因为应用程序将终止。
        SetWindowLongPtr(hEWnd1, GWLP_WNDPROC, (LONG_PTR)Org_EWnd1);
        SetWindowLongPtr(hEWnd2, GWLP_WNDPROC, (LONG_PTR)Org_EWnd2);
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
    MessageBox(hWnd, str, L"年龄", MB_OK);
    return 0;
}

//子类化退出控制1（名称输入）
LRESULT CALLBACK MySubProc1(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_KEYDOWN:
        //当向下箭头或选项卡到来时，将焦点更改为年份
        if ((int)wp == VK_DOWN || (int)wp == VK_TAB) {
            SetFocus(hEWnd2);
            focus = 2;
            return 0L; //处理消息后返回0
        }
        break;
    default:
        break;
    }
    //自己不处理的事情就让原来的程序去做
    return (CallWindowProc(Org_EWnd1, hWnd, msg, wp, lp));
}

//子类化退出控制2（年份输入）
LRESULT CALLBACK MySubProc2(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_KEYDOWN:
        //当向上键或者tab键来的时候，聚焦在名字上
        if ((int)wp == VK_UP || (int)wp == VK_TAB) {
            SetFocus(hEWnd1);
            focus = 1;
            return 0L;
        }
        break;
    default:
        break;
    }
    return (CallWindowProc(Org_EWnd2, hWnd, msg, wp, lp));
}

