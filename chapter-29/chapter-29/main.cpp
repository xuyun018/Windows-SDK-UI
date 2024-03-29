#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);

BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

LPCWSTR szClassName = L"icon01";    //クラス兆
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
    wc.lpfnWndProc = WndProc;    //プロシ�`ジャ兆
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;        //インスタンス
    wc.hIcon = LoadIcon(hInst, L"MYICON");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = L"MYMENU";    //メニュ�`兆
    wc.lpszClassName = szClassName;
    return (RegisterClass(&wc));
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"竪でもわかるアイコン",//タイトルバ�`にこの兆念が燕幣されます
        WS_OVERLAPPEDWINDOW,    //ウィンドウの�N��
        CW_USEDEFAULT,    //Ｘ恙��
        CW_USEDEFAULT,    //Ｙ恙��
        CW_USEDEFAULT,    //嫌
        CW_USEDEFAULT,    //互さ
        NULL,            //�Hウィンドウのハンドル、�Hを恬るときはNULL
        NULL,            //メニュ�`ハンドル、クラスメニュ�`を聞うときはNULL
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
            (LPCWSTR)L"�K阻してもよいですか",
            (LPCWSTR)L"�K阻�_�J",
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
            wcscpy(str, L"ＯＫボタンが兀されました!");
            InvalidateRect(hParent, NULL, TRUE);
            EndDialog(hWnd, IDOK);
            break;
        case IDCANCEL:
            wcscpy(str, L"キャンセルボタンが兀されました��");
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


