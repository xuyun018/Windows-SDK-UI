#include <windows.h>
#include <CommCtrl.h>

#define ID_MYOWN   1000 //按钮ID

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

void DrawPic(HDC, HINSTANCE, int); //在按钮上绘制

WCHAR szClassName[] = L"owndr01";        //窗口类

HWND hMyButton; //所有者绘制按钮的窗口句柄

HBITMAP hBitmap0 = NULL;
HBITMAP hBitmap1 = NULL;


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
    wc.lpfnWndProc = WndProc;    //过程名称
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;        //实例
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;    //菜单名称
    wc.lpszClassName = (LPCWSTR)szClassName;
    return (RegisterClass(&wc));
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"连猫都能听懂的自画",    //这个名字会显示在标题栏中
        WS_OVERLAPPEDWINDOW,    //窗口类型
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
    HDC hdc;
    static int sw = 1; //绘制哪张BMP

    switch (msg) {
    case WM_CREATE: //创建父按钮后立即创建按钮
        hBitmap0 = LoadBitmap((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), L"MYBMP1");
        hBitmap1 = LoadBitmap((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), L"MYBMP2");
        hMyButton = CreateWindow(WC_BUTTON,
            NULL,  //表示文字列
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            30, 30, //位置
            100, 100, //幅・高さ
            hWnd, //親ウィンドウ
            (HMENU)ID_MYOWN, //ボタンID
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), //インスタンスハンドル
            NULL);
        break;
    case WM_DRAWITEM: //如果捕获到此消息，则绘制
        hdc = ((LPDRAWITEMSTRUCT)lp)->hDC;
        DrawPic(hdc, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), sw);
        break;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case ID_MYOWN:
            if (sw == 1) {
                sw = 0;
            }
            else if (sw == 0) {
                sw = 1;
            }
            break;
        default:
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
        DeleteObject(hBitmap0);
        DeleteObject(hBitmap1);
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return (DefWindowProc(hWnd, msg, wp, lp));
}

void DrawPic(HDC hdc, HINSTANCE hInst, int sw)
{
    HBITMAP hBitmap = NULL;
    HDC hmdc;
    switch (sw) {
    case 0:
        hBitmap = hBitmap0;
        break;
    case 1:
        hBitmap = hBitmap1;
        break;
    }
    hmdc = CreateCompatibleDC(hdc);
    SelectObject(hmdc, hBitmap);
    BitBlt(hdc, 0, 0, 100, 100, hmdc, 0, 0, SRCCOPY);
    DeleteDC(hmdc);
    return;
}