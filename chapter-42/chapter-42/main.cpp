#define STRICT
#include <windows.h>

//实际上，请创建一个头文件并定义它
#define IDM_CAT1  1000 
#define IDM_CAT2  1010
#define IDM_CAT3  1020
#define IDM_CAT4  1030

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

const WCHAR szClassName[] = L"bmpmenu"; //窗口类

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
        L"猫でもわかるメニュー",    //タイトルバーにこの名前が表示されます
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
    HMENU hMyMenu, hMyMenu2;
    static HBITMAP hBMP1, hBMP2, hBMP3;
    HINSTANCE hInst;

    switch (msg) {
    case WM_CREATE: //親ウィンドウが作られたらすぐにメニューを作る
        hInst = ((LPCREATESTRUCT)lp)->hInstance;
        hBMP1 = LoadBitmap(hInst, L"MYBMP1");
        hBMP2 = LoadBitmap(hInst, L"MYBMP2");
        hBMP3 = LoadBitmap(hInst, L"MYBMP3");

        hMyMenu = CreateMenu();
        hMyMenu2 = CreatePopupMenu();
        AppendMenu(hMyMenu2, MF_ENABLED | MF_STRING, IDM_CAT1, L"名称");
        AppendMenu(hMyMenu2, MF_ENABLED | MF_STRING, IDM_CAT2, L"年齢");

        AppendMenu(hMyMenu, MF_ENABLED | MF_BITMAP | MF_POPUP, (UINT_PTR)hMyMenu2, MAKEINTRESOURCE(hBMP1));
        AppendMenu(hMyMenu, MF_ENABLED | MF_BITMAP, IDM_CAT3, MAKEINTRESOURCE(hBMP2));
        AppendMenu(hMyMenu, MF_ENABLED | MF_BITMAP, IDM_CAT4, MAKEINTRESOURCE(hBMP3));

        SetMenu(hWnd, hMyMenu);
        DrawMenuBar(hWnd);
        break;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDM_CAT1:
            MessageBox(hWnd, L"猫的名字是迈克尔", L"名称", MB_OK);
            break;
        case IDM_CAT2:
            MessageBox(hWnd, L"猫的年龄是2岁", L"年齢", MB_OK);
            break;
        case IDM_CAT3:
            MessageBox(hWnd, L"这只猫是迈克尔", L"迈克尔", MB_OK);
            break;
        case IDM_CAT4:
            MessageBox(hWnd, L"这只猫毕竟是迈克尔", L"迈克尔", MB_OK);
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
        DeleteObject(hBMP1);
        DeleteObject(hBMP2);
        DeleteObject(hBMP3);
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return (DefWindowProc(hWnd, msg, wp, lp));
}