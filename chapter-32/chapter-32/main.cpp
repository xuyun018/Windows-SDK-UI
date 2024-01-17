#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    static WCHAR str[2048];
    static WCHAR msg_str[256];
    const WCHAR* str_org = L"按下的键是'%c'。";
    const WCHAR* str_org2 = L"按下的键是'%s'。";
    int id, wh;
    HDC hdc;
    PAINTSTRUCT ps;
    static int hT;

    switch (msg) {
    case WM_CHAR:
        if ((wp >= 0x41 && wp <= 0x5A) || (wp >= 0x30 && wp <= 0x39)) {
            wsprintf((LPTSTR)str, (LPCTSTR)str_org, wp);
        }
        else {
            switch (wp) {
            case VK_TAB:
                wsprintf((LPWSTR)str, (LPCTSTR)str_org2, L"TAB");
                break;
            case VK_SHIFT:
                wsprintf((LPWSTR)str, (LPCTSTR)str_org2, L"SHIFT");
                break;
            case VK_ESCAPE:
                wsprintf((LPWSTR)str, (LPCTSTR)str_org2, L"ESC");
                break;
            default:
                wcscpy(str, L"知らないキーです");
                break;
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_SYSKEYDOWN:
        if (wp == VK_F10)
            wcscpy(str, L"Ｆ１０が押されました。");
        if (wp == VK_MENU)
            wcscpy(str, L"ALT(GRPH)キーが押されました");
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_SYSKEYUP:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        hT = DrawText(hdc,
            (LPCTSTR)str,
            wcslen(str),
            &(ps.rcPaint),
            DT_WORDBREAK);
        EndPaint(hWnd, &ps);
        wh = ps.rcPaint.bottom - ps.rcPaint.top;
        if (hT > wh - 20)
            wcscpy(str, L"");
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

BOOL InitApp(HINSTANCE hInst, LPCWSTR szClassName)
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

BOOL InitInstance(HINSTANCE hInst, LPCWSTR szClassName, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"猫でもわかるキー入力",    //タイトルバーにこの名前が表示されます
        WS_OVERLAPPEDWINDOW,    //ウィンドウの種類
        CW_USEDEFAULT,    //Ｘ座標
        CW_USEDEFAULT,    //Ｙ座標
        CW_USEDEFAULT,    //幅
        CW_USEDEFAULT,    //高さ
        NULL,    //親ウィンドウのハンドル、親を作るときはNULL
        NULL,    //メニューハンドル、クラスメニューを使うときはNULL
        hInst,    //インスタンスハンドル
        NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}


int WINAPI wWinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
    LPWSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    WCHAR szClassName[] = L"key01";        //ウィンドウクラス

    if (!hPrevInst) {
        if (!InitApp(hCurInst, szClassName))
            return FALSE;
    }
    if (!InitInstance(hCurInst, szClassName, nCmdShow)) {
        return FALSE;
    }
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}