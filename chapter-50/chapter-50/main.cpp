//        chd01.cpp
#define STRICT
#include <windows.h>
#define ID_MYCHILD 100

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

BOOL InitApp(HINSTANCE, WNDPROC, LPCTSTR);
BOOL InitInstance(HINSTANCE, int, LPCTSTR);

int WINAPI wWinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
    LPWSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    LPCWSTR szClassName = L"chd01";        //窗口类名

    if (!hPrevInst) {
        if (!InitApp(hCurInst, WndProc, szClassName))
            return FALSE;
    }
    if (!InitInstance(hCurInst, nCmdShow, szClassName)) {
        return FALSE;
    }
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

BOOL InitApp(HINSTANCE hInst, WNDPROC WndProc, LPCWSTR szClassName)
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

BOOL InitInstance(HINSTANCE hInst, int nCmdShow, LPCTSTR szClassName)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"父窗口",    //タイトルバーにこの名前が表示されます
        WS_OVERLAPPEDWINDOW,    //ウィンドウの種類
        CW_USEDEFAULT,    //Ｘ座標
        CW_USEDEFAULT,    //Ｙ座標
        CW_USEDEFAULT,    //幅
        CW_USEDEFAULT,    //高さ
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
    static HWND hChdWnd;
    HINSTANCE hInst;

    switch (msg) {
    case WM_CREATE:
        hInst = ((LPCREATESTRUCT)lp)->hInstance;
        InitApp(hInst,
            ChildProc,
            L"child");
        hChdWnd = CreateWindow(L"child",
            L"子供ウィンドウ",	//此名称将显示在标题栏中
            WS_CHILD | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION |
            WS_MINIMIZEBOX | WS_MAXIMIZEBOX,    //ウィンドウの種類
            CW_USEDEFAULT,    //Ｘ座標
            CW_USEDEFAULT,    //Ｙ座標
            200,    //幅
            100,    //高さ
            hWnd,            //父窗口句柄，创建父窗口时为NULL
            (HMENU)ID_MYCHILD, //菜单句柄，子ID
            hInst,            //实例句柄
            NULL);
        ShowWindow(hChdWnd, SW_SHOW);
        UpdateWindow(hChdWnd);
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
        break;
    }
    return (DefWindowProc(hWnd, msg, wp, lp));
}

LRESULT CALLBACK ChildProc(HWND hChdWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
    switch (msg) {
    case WM_CLOSE:
        id = MessageBox(hChdWnd,
            L"我可以关闭子窗口吗？",
            L"确认完成", MB_YESNO | MB_ICONQUESTION);
        if (id == IDYES)
            DestroyWindow(hChdWnd);
        break;
    default:
        break;
    }
    return (DefWindowProc(hChdWnd, msg, wp, lp));
}