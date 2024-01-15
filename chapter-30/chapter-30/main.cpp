#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <windowsx.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);

BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

int DrawMyText(HDC);
HFONT SetMyFont(LPCTSTR);
int SetMyDlg(HWND);
int GetMyDlg(HWND);
int SetDraw(HWND);

int nCF[5];        //ɫ�ȥե����

LPCWSTR szClassName = L"mdless01";    //���饹��

HWND hDlg;

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
        if (hDlg == NULL || !IsDialogMessage(hDlg, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}

//ע�ᴰ����

BOOL InitApp(HINSTANCE hInst)
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;    //�ץ��`������
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;        //���󥹥���
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;    //��˥�`��
    wc.lpszClassName = szClassName;
    return (RegisterClass(&wc));
}


//��������

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd, hDlgWnd;

    hWnd = CreateWindow(szClassName,
        L"è�Ǥ�狼���`�ɥ쥹",//�����ȥ�Щ`�ˤ�����ǰ����ʾ����ޤ�
        WS_OVERLAPPEDWINDOW,    //������ɥ��ηN�
        CW_USEDEFAULT,    //������
        CW_USEDEFAULT,    //������
        300,    //��
        150,    //�ߤ�
        NULL,            //�H������ɥ��Υϥ�ɥ롢�H������Ȥ���NULL
        NULL,            //��˥�`�ϥ�ɥ롢���饹��˥�`��ʹ���Ȥ���NULL
        hInst,            //���󥹥��󥹥ϥ�ɥ�
        NULL);
    if (!hWnd)
        return FALSE;
    hDlgWnd = CreateDialog(hInst, L"MYDLG", hWnd, (DLGPROC)MyDlgProc);
    if (hDlgWnd == NULL)
        return FALSE;
    hDlg = hDlgWnd;
    ShowWindow(hDlgWnd, SW_SHOW);
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hDlgWnd);
    UpdateWindow(hWnd);
    return TRUE;
}

//���ڹ���

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
    HDC hdc;
    PAINTSTRUCT ps;
    static HFONT hFont, hFontOld;

    switch (msg) {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        if (nCF[0])
            SetTextColor(hdc, RGB(255, 0, 0));
        if (nCF[1])
            SetTextColor(hdc, RGB(0, 255, 0));
        if (nCF[2])
            SetTextColor(hdc, RGB(0, 0, 255));
        if (nCF[3])
            hFont = SetMyFont(L"�ͣ� ����");
        hFontOld = (HFONT)SelectObject(hdc, hFont);
        if (nCF[4])
            hFont = SetMyFont(L"�ͣ� �����å�");
        hFontOld = (HFONT)SelectObject(hdc, hFont);
        DrawMyText(hdc);
        SelectObject(hdc, hFontOld);
        DeleteObject(hFont);
        EndPaint(hWnd, &ps);
        break;

    case WM_CLOSE:
        id = MessageBox(hWnd,
            (LPCWSTR)L"�K�ˤ��Ƥ�褤�Ǥ���",
            (LPCWSTR)L"�K�˴_�J",
            MB_YESNO | MB_ICONQUESTION);
        if (id == IDYES) {
            DestroyWindow(hDlg);
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
    static HWND hParent;
    hParent = GetParent(hWnd);

    switch (msg) {
    case WM_INITDIALOG:
        SetMyDlg(hWnd);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case IDOK:
            GetMyDlg(hWnd);
            InvalidateRect(hParent, NULL, TRUE);
            return TRUE;
        case IDCANCEL:
            SendMessage(hParent, WM_CLOSE, 0, 0L);
            return TRUE;
        }
    }
    return FALSE;
}

int DrawMyText(HDC hdc)
{
    const WCHAR* pstr;

    pstr = L"è�Ǥ�狼��";
    TextOut(hdc, 10, 10, pstr, wcslen(pstr));
    pstr = L"��`�ɥ쥹";
    TextOut(hdc, 15, 50, pstr, wcslen(pstr));
    return 0;
}

HFONT SetMyFont(LPCTSTR face)
{
    HFONT hFont;
    hFont = CreateFont(40,    //�ե���ȸߤ�
        0,                    //���ַ�
        0,                    //�ƥ����ȤνǶ�
        0,                    //�٩`���饤��ȣ��S�ȤνǶ�
        FW_REGULAR,            //�ե���Ȥ��ؤ���̫����
        FALSE,                //������å���
        FALSE,                //������`�饤��
        FALSE,                //���������
        SHIFTJIS_CHARSET,    //���֥��å�
        OUT_DEFAULT_PRECIS,    //��������
        CLIP_DEFAULT_PRECIS,//����åԥ󥰾���
        PROOF_QUALITY,        //����Ʒ�|
        FIXED_PITCH | FF_MODERN,//�ԥå��ȥե��ߥ�`
        face);    //������
    return hFont;
}

int SetMyDlg(HWND hWnd)
{
    Button_SetCheck(GetDlgItem(hWnd, IDC_RED), BST_CHECKED);
    Button_SetCheck(GetDlgItem(hWnd, IDC_MIN), BST_CHECKED);
    GetMyDlg(hWnd);
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
}

int GetMyDlg(HWND hWnd)
{
    int i, id[5];
    id[0] = IDC_RED;
    id[1] = IDC_GREEN;
    id[2] = IDC_BLUE;
    id[3] = IDC_MIN;
    id[4] = IDC_GO;

    for (i = 0; i <= 4; i++)
        nCF[i] = Button_GetCheck(GetDlgItem(hWnd, id[i]));
    return 0;
}