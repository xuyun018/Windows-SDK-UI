#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

void DrawCat(HWND, HDC);

const WCHAR szClassName[] = L"sdk43";        //������
HWND hParent; //�����ھ��
HACCEL hAccel; //����������
int paintsw = 0;

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
    if (hAccel == NULL)
        MessageBox(NULL, L"ʧ��", L"", MB_OK);
    if (hParent == NULL)
        MessageBox(NULL, L"�޾��", L"", MB_OK);
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
    wc.hInstance = hInst;        //ʵ��
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = L"MYMENU";    //�˵�����
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
        300, // ���
        200, //�߶�
        NULL, //�����ھ��������������ʱΪNULL
        NULL, //�˵������ʹ����˵�ʱΪNULL
        hInst, //ʵ�����
        NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    hParent = hWnd;
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
    HDC hdc;
    PAINTSTRUCT ps;
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDM_END:
            SendMessage(hWnd, WM_CLOSE, 0L, 0L);
            break;
        case IDM_CAT:
            if (paintsw == 1) {
                MessageBox(hWnd,
                    L"ͼƬ�Ѿ���ʾ",
                    L"ע�⣡",
                    MB_ICONEXCLAMATION | MB_OK);
            }
            else
            {
                paintsw = 1;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        case IDM_CLR:
            if (paintsw == 0) {
                MessageBox(hWnd,
                    L"û�п�ɾ����ͼƬ",
                    L"ע�⣡",
                    MB_ICONEXCLAMATION | MB_OK);
            }
            else
            {
                paintsw = 0;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        default:
            break;
        }
        break;
    case WM_PAINT:
        if (paintsw == 1) {
            hdc = BeginPaint(hWnd, &ps);
            DrawCat(hWnd, hdc);
            EndPaint(hWnd, &ps);
        }
        else {
            //
        }
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

void DrawCat(HWND hWnd, HDC hdc)
{
    HDC hmdc;
    HBITMAP hBitmap;
    BITMAP bmp;
    HINSTANCE hInst;
    int BMP_W, BMP_H;

    hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
    hBitmap = LoadBitmap(hInst, L"MYBMP");
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    BMP_W = (int)bmp.bmWidth;
    BMP_H = (int)bmp.bmHeight;
    hmdc = CreateCompatibleDC(hdc);
    SelectObject(hmdc, hBitmap);
    BitBlt(hdc, 0, 0, BMP_W, BMP_H, hmdc, 0, 0, SRCCOPY);
    DeleteDC(hmdc);
    DeleteObject(hBitmap);
    return;
}