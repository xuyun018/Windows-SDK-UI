#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);

BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

LPCWSTR szClassName = L"icon01";    //クラス名
WCHAR str[256];

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
    wc.hIcon = LoadIcon(hInst, L"MYICON");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = L"MYMENU";    //メニュー名
    wc.lpszClassName = szClassName;
    return (RegisterClass(&wc));
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"猫でもわかるアイコン",//タイトルバーにこの名前が表示されます
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
    HDC hdc;
    PAINTSTRUCT ps;
    HINSTANCE hInst;
    hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDM_END:
            SendMessage(hWnd, WM_CLOSE, 0, 0L);
            break;
        case IDM_DLG:
            DialogBox(hInst, L"MYDLG", hWnd, (DLGPROC)MyDlgProc);
            break;
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 10, 10, (LPCTSTR)str, lstrlen(str));
        EndPaint(hWnd, &ps);
        break;
    case WM_CLOSE:
        id = MessageBox(hWnd,
            (LPCWSTR)L"終了してもよいですか",
            (LPCWSTR)L"終了確認",
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


LRESULT CALLBACK MyDlgProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    HWND hParent;
    hParent = GetParent(hWnd);

    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDOK:
            wcscpy(str, L"ＯＫボタンが押されました!");
            InvalidateRect(hParent, NULL, TRUE);
            EndDialog(hWnd, IDOK);
            break;
        case IDCANCEL:
            wcscpy(str, L"キャンセルボタンが押されました！");
            InvalidateRect(hParent, NULL, TRUE);
            EndDialog(hWnd, IDCANCEL);
            break;
        default:
            return FALSE;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}


