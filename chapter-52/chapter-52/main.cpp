//        ani01.cpp

#define STRICT
#define ID_MYTIMER    32767
#define BMP_W 79
#define BMP_H 77
#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

const WCHAR szClassName[] = L"ani01";        //ウィンドウクラス


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

//创建窗口

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"连猫都看得懂的动画",    //タイトルバーにこの名前が表示されます
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
    static int x, y, hoko, client_w;
    HDC hdc, hdc_mem;
    PAINTSTRUCT ps;
    HBITMAP hBitmap;
    HINSTANCE hInst;
    RECT rc;
    enum { right, left };

    switch (msg) {
    case WM_CREATE:
        if (SetTimer(hWnd, ID_MYTIMER, 200, NULL) == 0)
            MessageBox(hWnd, L"失敗", L"failure", MB_OK);
        break;
    case WM_TIMER:
        GetClientRect(hWnd, &rc);
        client_w = rc.right;
        if (hoko == left)
            rc.left = x - 20;
        else
            rc.left = x;
        rc.top = y;
        if (hoko == right)
            rc.right = x + BMP_W + 20;
        else
            rc.right = x + BMP_W;
        rc.bottom = y + BMP_H;
        switch (hoko) {
        case right:
            x += 20;
            if (x + BMP_W >= client_w)
                hoko = left;
            break;
        case left:
            x -= 20;
            if (x < 0)
                hoko = right;
            break;
        }
        InvalidateRect(hWnd, &rc, TRUE);
        break;
    case WM_PAINT:
        hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
        hdc = BeginPaint(hWnd, &ps);
        hBitmap = LoadBitmap(hInst, L"MYBMP");
        hdc_mem = CreateCompatibleDC(hdc);
        SelectObject(hdc_mem, hBitmap);
        BitBlt(hdc, x, y, x + BMP_W, y + BMP_H, hdc_mem, 0, 0, SRCCOPY);
        DeleteDC(hdc_mem);
        DeleteObject(hBitmap);
        EndPaint(hWnd, &ps);
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