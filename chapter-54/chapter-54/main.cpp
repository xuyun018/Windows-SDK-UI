//        gr01.cpp

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
void DrawGr(HWND, HDC);

const WCHAR szClassName[] = L"gr01";        // 窗口类
WCHAR kokugo[4], sansu[4], rika[4], shakai[4];    //每个科目的分数字符串
int point[4];//各科得分
enum { koku, san, ri, sha };
int sw; //绘制开关

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
    wc.lpszMenuName = L"MYMENU";    //メニュー名
    wc.lpszClassName = (LPCWSTR)szClassName;
    return (RegisterClass(&wc));
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"连猫都能看懂的图表",    //タイトルバーにこの名前が表示されます
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
    HINSTANCE hInst;
    HDC hdc;
    PAINTSTRUCT ps;

    switch (msg) {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        DrawGr(hWnd, hdc);
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDM_END:
            SendMessage(hWnd, WM_CLOSE, 0L, 0L);
            break;
        case IDM_DATA:
            hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
            DialogBox(hInst, L"MYDLG", hWnd, (DLGPROC)MyDlgProc);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        default:
            return (DefWindowProc(hWnd, msg, wp, lp));
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
        PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0L;
}

LRESULT CALLBACK MyDlgProc(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_INITDIALOG:
        Edit_SetText(GetDlgItem(hDlgWnd, IDC_KOKUGO), kokugo);
        Edit_SetText(GetDlgItem(hDlgWnd, IDC_SANSU), sansu);
        Edit_SetText(GetDlgItem(hDlgWnd, IDC_RIKA), rika);
        Edit_SetText(GetDlgItem(hDlgWnd, IDC_SHAKAI), shakai);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDOK:
            Edit_GetText(GetDlgItem(hDlgWnd, IDC_KOKUGO), kokugo, sizeof(kokugo));
            Edit_GetText(GetDlgItem(hDlgWnd, IDC_SANSU), sansu, sizeof(sansu));
            Edit_GetText(GetDlgItem(hDlgWnd, IDC_RIKA), rika, sizeof(rika));
            Edit_GetText(GetDlgItem(hDlgWnd, IDC_SHAKAI), shakai, sizeof(shakai));
            point[koku] = _wtoi(kokugo);
            point[san] = _wtoi(sansu);
            point[ri] = _wtoi(rika);
            point[sha] = _wtoi(shakai);
            sw = 1;
            EndDialog(hDlgWnd, IDOK);
            return TRUE;
        case IDCANCEL:
            EndDialog(hDlgWnd, IDCANCEL);
            return TRUE;
        default:
            return FALSE;
        }
    default:
        return FALSE;
    }
}

void DrawGr(HWND hWnd, HDC hdc)
{
    RECT rc;
    HBRUSH hBrush;
    int y_unit, x_unit, i, space_x, space_y, gr_haba;

    if (sw == 0)
        return;
    GetClientRect(hWnd, &rc);
    space_x = rc.right / 10;
    space_y = rc.bottom / 10;
    gr_haba = rc.right / 12;
    y_unit = (rc.bottom - (2 * space_y)) / 10;
    x_unit = (rc.right - 2 * space_x) / 5;

    //绘制轴
    MoveToEx(hdc, space_x, rc.bottom - space_y, NULL);
    LineTo(hdc, rc.right - space_x, rc.bottom - space_y);
    MoveToEx(hdc, space_x, rc.bottom - space_y, NULL);
    LineTo(hdc, space_x, space_y);
    //Y轴刻度
    for (i = 0; i <= 10; i++) {
        MoveToEx(hdc, space_x, rc.bottom - space_y - y_unit * i, NULL);
        LineTo(hdc, space_x - space_x / 8, rc.bottom - space_y - y_unit * i);
    }
    //X轴刻度
    for (i = 1; i <= 4; i++) {
        MoveToEx(hdc, space_x + x_unit * i, rc.bottom - (space_y - space_y / 8), NULL);
        LineTo(hdc, space_x + x_unit * i, rc.bottom - space_y);
    }
    //0分和100分
    TextOut(hdc, 0, rc.bottom - space_y, L"0", 1);
    TextOut(hdc, 0, rc.bottom - space_y - 11 * y_unit, L"100", 3);
    //
    TextOut(hdc, space_x + x_unit * 1 - gr_haba / 2, rc.bottom - space_y + 4, L"国語", 2);
    TextOut(hdc, space_x + x_unit * 2 - gr_haba / 2, rc.bottom - space_y + 4, L"算数", 2);
    TextOut(hdc, space_x + x_unit * 3 - gr_haba / 2, rc.bottom - space_y + 4, L"理科", 2);
    TextOut(hdc, space_x + x_unit * 4 - gr_haba / 2, rc.bottom - space_y + 4, L"社会", 2);
    //各科成绩图
    hBrush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(hdc, hBrush);
    for (i = 1; i <= 4; i++) {

        Rectangle(hdc,
            space_x + x_unit * i - gr_haba / 2,
            rc.bottom - space_y,
            space_x + x_unit * i + gr_haba / 2,
            rc.bottom - space_y - point[i - 1] * y_unit / 10);
    }
    DeleteObject(hBrush);
    return;
}