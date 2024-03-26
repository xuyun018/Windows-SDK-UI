//        chd02.cpp
#define STRICT
#define ID_MYTIMER    32767
#define ID_MYCHILD    100
#define CHD_W 79
#define CHD_H 77
#include <windows.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

BOOL InitApp(HINSTANCE, WNDPROC, LPCTSTR);
BOOL InitInstance(HINSTANCE, int, LPCTSTR);

int WINAPI wWinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
    LPWSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    LPCWSTR szClassName = L"chd02";        // 窗口类名

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

BOOL InitInstance(HINSTANCE hInst, int nCmdShow, LPCWSTR szClassName)
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
    RECT rc;
    static int x, y;
    static int direction;
    int id;
    static HWND hChdWnd;
    HINSTANCE hInst;
    enum { right, left };

    switch (msg) {
    case WM_CREATE:
        hInst = ((LPCREATESTRUCT)lp)->hInstance;
        InitApp(hInst,
            ChildProc,
            L"child");
        hChdWnd = CreateWindow(L"child",
            NULL,//タイトルバーにこの名前が表示されます
            WS_CHILD,    //ウィンドウの種類
            0,    //Ｘ座標
            0,    //Ｙ座標
            CHD_W,    //幅
            CHD_H,    //高さ
            hWnd,            //親ウィンドウのハンドル、親を作るときはNULL
            (HMENU)ID_MYCHILD,//メニューハンドル、子供のID
            hInst,            //インスタンスハンドル
            NULL);
        ShowWindow(hChdWnd, SW_SHOW);
        UpdateWindow(hChdWnd);
        SetTimer(hWnd, ID_MYTIMER, 100, NULL);
        break;
    case WM_TIMER:
        GetClientRect(hWnd, &rc);
        MoveWindow(hChdWnd, x, y, CHD_W, CHD_H, TRUE);
        switch (direction) {
        case right:
            x += 10;
            if (x >= rc.right - CHD_W)
                direction = left;
            break;
        case left:
            x -= 10;
            if (x < 0)
                direction = right;
            break;
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
        KillTimer(hWnd, ID_MYTIMER);
        PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0L;
}

LRESULT CALLBACK ChildProc(HWND hChdWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    HBITMAP hBitmap;
    HINSTANCE hInst;
    PAINTSTRUCT ps;
    HDC hdc, hdc_mem;
    switch (msg) {
    case WM_PAINT:
        hInst = (HINSTANCE)GetWindowLongPtr(hChdWnd, GWLP_HINSTANCE);
        hdc = BeginPaint(hChdWnd, &ps);
        hBitmap = LoadBitmap(hInst, L"MYBMP");
        hdc_mem = CreateCompatibleDC(hdc);
        SelectObject(hdc_mem, hBitmap);
        BitBlt(hdc, 0, 0, CHD_W, CHD_H, hdc_mem, 0, 0, SRCCOPY);
        DeleteDC(hdc_mem);
        DeleteObject(hBitmap);
        EndPaint(hChdWnd, &ps);
        break;
    default:
        return (DefWindowProc(hChdWnd, msg, wp, lp));
    }
    return 0L;
}
