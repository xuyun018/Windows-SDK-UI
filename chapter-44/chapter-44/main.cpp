#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

void DrawPic(HWND, HDC, int);

const WCHAR szClassName[] = L"sdk44";        //������
HWND hParent;  //���游���ھ��
HACCEL hAccel; //����������

enum { clr, cat, dog, rat };

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

    hAccel = LoadAccelerators(hCurInst, L"MYACCEL");

    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(hParent, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}

BOOL InitApp(HINSTANCE hInst)
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;    //��������
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;        //ʵ�����
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;    //�˵�����
    wc.lpszClassName = (LPCWSTR)szClassName;
    return (RegisterClass(&wc));
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

        hWnd = CreateWindow(szClassName,
            L"Cat-Friendly Menu", //�����ƽ���ʾ�ڱ�������
            WS_OVERLAPPEDWINDOW, //��������
            CW_USEDEFAULT, //X����
            CW_USEDEFAULT, //Y����
            200, // ���
            100, // �߶�
            NULL, //�����ھ��������������ʱΪNULL
            NULL, //�˵������ʹ����˵�ʱΪNULL
            hInst, //ʵ�����
            NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    hParent = hWnd; //���洰�ھ������Ҫ���ǣ�
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
    static int sw = 0;
    POINT pt;
    HMENU hmenu, hSubmenu;
    HDC hdc;
    PAINTSTRUCT ps;

    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDM_END:
            SendMessage(hWnd, WM_CLOSE, 0L, 0L);
            break;
        case IDM_CAT:
            sw = cat;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_DOG:
            sw = dog;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_RAT:
            sw = rat;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case IDM_CLR:
            sw = clr;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        default:
            break;
        }
        break;
    case WM_RBUTTONDOWN:
        pt.x = LOWORD(lp);
        pt.y = HIWORD(lp);
        hmenu = LoadMenu((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), L"MYPOPUP");
        hSubmenu = GetSubMenu(hmenu, 0);
        ClientToScreen(hWnd, &pt);
        TrackPopupMenu(hSubmenu, TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
        DestroyMenu(hmenu);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        DrawPic(hWnd, hdc, sw);
        EndPaint(hWnd, &ps);
        break;
    case WM_CLOSE:
        id = MessageBox(hWnd,
            (LPCWSTR)L"��ȷ��Ҫ�˳���",
            (LPCWSTR)L"���ȷ��",
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

void DrawPic(HWND hWnd, HDC hdc, int sw)
{
    HDC hmdc;
    HBITMAP hBitmap;
    BITMAP bmp;
    HINSTANCE hInst;
    int BMP_W, BMP_H;

    hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
    switch (sw) {
    case clr:
        return;
    case cat:
        hBitmap = LoadBitmap(hInst, L"MYCAT");
        break;
    case dog:
        hBitmap = LoadBitmap(hInst, L"MYDOG");
        break;
    case rat:
        hBitmap = LoadBitmap(hInst, L"MYRAT");
        break;
    default:
        hBitmap = NULL;
        break;
    }
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    BMP_W = (int)bmp.bmWidth;
    BMP_H = (int)bmp.bmHeight;
    hmdc = CreateCompatibleDC(hdc);
    SelectObject(hmdc, hBitmap);
    BitBlt(hdc, 0, 0, BMP_W, BMP_H, hmdc, 0, 0, SRCCOPY);
    DeleteDC(hmdc);
    return;
}