//        meta01.cpp

#include <windows.h>
#define IDO 50

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
void MakeMyMeta(void);
HMETAFILE MakeMyEllipse(int, int);

const WCHAR szClassName[] = L"meta01";        //��������

HMETAFILE hmf[5];
int play_no;

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
    wc.lpfnWndProc = WndProc;    //��������
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;        //ʵ��
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
        L"è�Ǥ�狼��᥿�ե�����",    //������ƽ���ʾ�ڱ�������
        WS_OVERLAPPED | WS_SYSMENU,    //��������
        CW_USEDEFAULT,    //������
        CW_USEDEFAULT,    //������
        258,//���
        75,    //�߶�
        NULL,            //�����ھ��������������ʱΪNULL
        NULL,            //�˵������ʹ����˵�ʱΪNULL
        hInst,            //ʵ�����
        NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id, i;
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rc;

    switch (msg) {
    case WM_CREATE:
        MakeMyMeta();
        SetTimer(hWnd, 1, 200, NULL);
        break;
    case WM_TIMER:
        play_no++;
        if (play_no > 5)
            play_no = 1;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
        //���Ʊ���
        GetClientRect(hWnd, &rc);
        hdc = BeginPaint(hWnd, &ps);
        MoveToEx(hdc, rc.left, rc.bottom / 2, NULL);
        LineTo(hdc, rc.right, rc.bottom / 2);
        MoveToEx(hdc, rc.left, rc.top, NULL);
        LineTo(hdc, rc.right, rc.bottom);
        MoveToEx(hdc, rc.left, rc.bottom, NULL);
        LineTo(hdc, rc.right, rc.top);
        //����ͼԪ�ļ�
        PlayMetaFile(hdc, hmf[play_no - 1]);
        EndPaint(hWnd, &ps);
        break;
    case WM_CLOSE:
        id = MessageBox(hWnd,
            (LPCWSTR)L"��ȷ��Ҫ�˳���",
            (LPCWSTR)L"�K�˴_�J",
            MB_YESNO | MB_ICONQUESTION);
        if (id == IDYES) {
            DestroyWindow(hWnd);
        }
        break;
    case WM_DESTROY:
        //�ͷ�ͼԪ�ļ����
        for (i = 0; i <= 4; i++) {
            DeleteMetaFile(hmf[i]);
        }
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return (DefWindowProc(hWnd, msg, wp, lp));
}

void MakeMyMeta(void)
{
    static int x1 = 0, x2 = 50;
    int i;
    for (i = 0; i <= 4; i++) {
        hmf[i] = MakeMyEllipse(x1, x2);
        x1 += IDO;
        x2 += IDO;
    }
    return;
}

HMETAFILE MakeMyEllipse(int x1, int x2) // ����ͼԪ�ļ�
{
    HDC hdc_meta;
    HBRUSH hBrush;
    HPEN hPen;
    hdc_meta = CreateMetaFile(NULL);
    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    SelectObject(hdc_meta, hPen);
    SelectObject(hdc_meta, hBrush);
    Ellipse(hdc_meta, x1, 0, x2, 50);
    DeleteObject(hPen);
    return (CloseMetaFile(hdc_meta));
}