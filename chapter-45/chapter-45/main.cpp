// sdk45.cpp
// 由Y.Kumei 编程 1997/06/13
//        Programmed by Y.Kumei    1997/06/13
#define STRICT
#include <windows.h>
#include "resource.h"

#define IDM_END    99
#define IDM_DOG    100
#define IDM_CAT    200
#define IDM_RAT    300

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

HFONT SetMyFont(LPCWSTR, int);
void DrawPic(HWND, HDC, int);

const WCHAR szClassName[] = L"sdk45";        //ウィンドウクラス

HMENU hMenu;
LPMEASUREITEMSTRUCT lpMI;
LPDRAWITEMSTRUCT lpDI;

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
    while (GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

BOOL InitApp(HINSTANCE hInst)
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;    // 过程名称
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;        // 实例
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
        L"Cat-Friendly Menu", //此名称将显示在标题栏中
        WS_OVERLAPPEDWINDOW, //窗口类型
        CW_USEDEFAULT, //X坐标
        CW_USEDEFAULT, //Y坐标
        CW_USEDEFAULT, //宽度
        CW_USEDEFAULT, //高度
        NULL, //父窗口句柄，创建父窗口时为NULL
        NULL, //菜单句柄，使用类菜单时为NULL
        hInst, //实例句柄
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
    static int sw = 0;    //切换画什么 1: 狗 2: 猫 3: 老鼠
    PAINTSTRUCT ps;
    RECT rc;
    HDC hdc;
    POINT pt;
    SIZE sz;
    HFONT hFont, hFontOld;
    enum { MENU_END, MENU_DOG, MENU_CAT, MENU_RAT };
    LPCWSTR str[] = { L"结束", L"狗　", L"猫　", L"老鼠" };
    switch (msg) {
    case WM_CREATE:
        hMenu = CreatePopupMenu();
        AppendMenu(hMenu, MF_OWNERDRAW, IDM_END, (LPCWSTR)MENU_END);
        AppendMenu(hMenu, MF_OWNERDRAW, IDM_DOG, (LPCWSTR)MENU_DOG);
        AppendMenu(hMenu, MF_OWNERDRAW, IDM_CAT, (LPCWSTR)MENU_CAT);
        AppendMenu(hMenu, MF_OWNERDRAW, IDM_RAT, (LPCWSTR)MENU_RAT);
        break;
    case WM_MEASUREITEM:
        lpMI = (LPMEASUREITEMSTRUCT)lp;
        hdc = GetDC(hWnd);
        hFont = SetMyFont(L"ＭＳ 明朝", 30);
        hFontOld = (HFONT)SelectObject(hdc, hFont);
        GetTextExtentPoint32(hdc,
            str[lpMI->itemData],
            lstrlen(str[lpMI->itemData]) - 1,
            &sz);
        lpMI->itemWidth = sz.cx + sz.cx;
        lpMI->itemHeight = sz.cy;
        SelectObject(hdc, hFontOld);
        DeleteObject(hFont);
        ReleaseDC(hWnd, hdc);
        return TRUE;
    case WM_DRAWITEM:
        lpDI = (LPDRAWITEMSTRUCT)lp;
        rc = lpDI->rcItem;
        hdc = lpDI->hDC;
        hFont = SetMyFont(L"ＭＳ 明朝", 30);
        hFontOld = (HFONT)SelectObject(hdc, hFont);
        switch (lpDI->itemID) {
        case IDM_END:
            if (lpDI->itemState == ODS_SELECTED) {
                SetBkColor(hdc, RGB(0, 0, 255));
                SetTextColor(hdc, RGB(255, 0, 0));
            }
            TextOut(hdc, rc.left, rc.top, str[MENU_END], lstrlen(str[MENU_END]));
            break;
        case IDM_DOG:
            if (lpDI->itemState == ODS_SELECTED) {
                SetBkColor(hdc, RGB(0, 0, 255));
                SetTextColor(hdc, RGB(255, 0, 0));
            }
            TextOut(hdc, rc.left, rc.top, str[MENU_DOG], lstrlen(str[MENU_DOG]));
            break;
        case IDM_CAT:
            if (lpDI->itemState == ODS_SELECTED) {
                SetBkColor(hdc, RGB(0, 0, 255));
                SetTextColor(hdc, RGB(255, 0, 0));
            }
            TextOut(hdc, rc.left, rc.top, str[MENU_CAT], lstrlen(str[MENU_CAT]));
            return TRUE;
        case IDM_RAT:
            if (lpDI->itemState == ODS_SELECTED) {
                SetBkColor(hdc, RGB(0, 0, 255));
                SetTextColor(hdc, RGB(255, 0, 0));
            }
            TextOut(hdc, rc.left, rc.top, str[MENU_RAT], lstrlen(str[MENU_RAT]));
            break;
        }
        SelectObject(hdc, hFontOld);
        DeleteObject(hFont);
        return TRUE;
    case WM_RBUTTONDOWN:
        pt.x = LOWORD(lp);
        pt.y = HIWORD(lp);
        ClientToScreen(hWnd, &pt);
        TrackPopupMenu(hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
        break;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDM_END:
            SendMessage(hWnd, WM_CLOSE, 0L, 0L);
            break;
        case IDM_DOG:
            sw = 1;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_CAT:
            sw = 2;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_RAT:
            sw = 3;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        default:
            break;
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        DrawPic(hWnd, hdc, sw);
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
        DestroyMenu(hMenu);
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return (DefWindowProc(hWnd, msg, wp, lp));
}

HFONT SetMyFont(LPCTSTR face, int h)
{
    HFONT hFont;
    hFont = CreateFont(h, //字体高度
        0, //字符宽度
        0, // 文字角度
        0, //基线与x轴的夹角
        FW_REGULAR, //字体粗细（粗细）
        FALSE, //斜体
        FALSE, //下划线
        FALSE, //删除线
        SHIFTJIS_CHARSET, //字符集
        OUT_DEFAULT_PRECIS, //输出精度
        CLIP_DEFAULT_PRECIS,//裁剪精度
        PROOF_QUALITY, //输出质量
        FIXED_PITCH | FF_MODERN,//音高和族
        face); //字体名称
    return hFont;
}

void DrawPic(HWND hWnd, HDC hdc, int sw)
{
    HDC hmdc;
    HBITMAP hBitmap = NULL;
    BITMAP bmp;
    HINSTANCE hInst;
    int BMP_W, BMP_H;
    hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
    switch (sw) {
    case 1:
        hBitmap = LoadBitmap(hInst, L"MYDOG");
        break;
    case 2:
        hBitmap = LoadBitmap(hInst, L"MYCAT");
        break;
    case 3:
        hBitmap = LoadBitmap(hInst, L"MYRAT");
        break;
    }
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    BMP_W = (int)bmp.bmWidth;
    BMP_H = (int)bmp.bmHeight;
    hmdc = CreateCompatibleDC(hdc);
    SelectObject(hmdc, hBitmap);
    BitBlt(hdc, 20, 20, BMP_W, BMP_H, hmdc, 0, 0, SRCCOPY);
    DeleteDC(hmdc);
    DeleteObject(hBitmap);
    return;
}
