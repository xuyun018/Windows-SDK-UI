//        chd02.cpp
#define STRICT
#define ID_MYTIMER    32767
#define ID_MYCHILD    100
#define CHD_W 79
#define CHD_H 77
#include <windows.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

BOOL InitApp(HINSTANCE, WNDPROC, LPCTSTR);
BOOL InitInstance(HINSTANCE, int, LPCTSTR);

int WINAPI wWinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
    LPWSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    LPCWSTR szClassName = L"chd02";        // ��������

    if (!hPrevInst) {
        if (!InitApp(hCurInst, WndProc, szClassName))
            return FALSE;
    }
    if (!InitInstance(hCurInst, nCmdShow, szClassName)) {
        return FALSE;
    }
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

BOOL InitApp(HINSTANCE hInst, WNDPROC WndProc, LPCWSTR szClassName)
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
    wc.lpszClassName = (LPCWSTR)szClassName;
    return (RegisterClass(&wc));
}

BOOL InitInstance(HINSTANCE hInst, int nCmdShow, LPCWSTR szClassName)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"������",    //�����ȥ�Щ`�ˤ�����ǰ����ʾ����ޤ�
        WS_OVERLAPPEDWINDOW,    //������ɥ��ηN�
        CW_USEDEFAULT,    //������
        CW_USEDEFAULT,    //������
        CW_USEDEFAULT,    //��
        CW_USEDEFAULT,    //�ߤ�
        NULL,            //�H������ɥ��Υϥ�ɥ롢�H������Ȥ���NULL
        NULL,            //��˥�`�ϥ�ɥ롢���饹��˥�`��ʹ���Ȥ���NULL
        hInst,            //���󥹥��󥹥ϥ�ɥ�
        NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    RECT rc;
    static int x, y;
    static int direction;
    int id;
    static HWND hChdWnd;
    HINSTANCE hInst;
    enum { right, left };

    switch (msg) {
    case WM_CREATE:
        hInst = ((LPCREATESTRUCT)lp)->hInstance;
        InitApp(hInst,
            ChildProc,
            L"child");
        hChdWnd = CreateWindow(L"child",
            NULL,//�����ȥ�Щ`�ˤ�����ǰ����ʾ����ޤ�
            WS_CHILD,    //������ɥ��ηN�
            0,    //������
            0,    //������
            CHD_W,    //��
            CHD_H,    //�ߤ�
            hWnd,            //�H������ɥ��Υϥ�ɥ롢�H������Ȥ���NULL
            (HMENU)ID_MYCHILD,//��˥�`�ϥ�ɥ롢�ӹ���ID
            hInst,            //���󥹥��󥹥ϥ�ɥ�
            NULL);
        ShowWindow(hChdWnd, SW_SHOW);
        UpdateWindow(hChdWnd);
        SetTimer(hWnd, ID_MYTIMER, 100, NULL);
        break;
    case WM_TIMER:
        GetClientRect(hWnd, &rc);
        MoveWindow(hChdWnd, x, y, CHD_W, CHD_H, TRUE);
        switch (direction) {
        case right:
            x += 10;
            if (x >= rc.right - CHD_W)
                direction = left;
            break;
        case left:
            x -= 10;
            if (x < 0)
                direction = right;
            break;
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
        KillTimer(hWnd, ID_MYTIMER);
        PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0L;
}

LRESULT CALLBACK ChildProc(HWND hChdWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    HBITMAP hBitmap;
    HINSTANCE hInst;
    PAINTSTRUCT ps;
    HDC hdc, hdc_mem;
    switch (msg) {
    case WM_PAINT:
        hInst = (HINSTANCE)GetWindowLongPtr(hChdWnd, GWLP_HINSTANCE);
        hdc = BeginPaint(hChdWnd, &ps);
        hBitmap = LoadBitmap(hInst, L"MYBMP");
        hdc_mem = CreateCompatibleDC(hdc);
        SelectObject(hdc_mem, hBitmap);
        BitBlt(hdc, 0, 0, CHD_W, CHD_H, hdc_mem, 0, 0, SRCCOPY);
        DeleteDC(hdc_mem);
        DeleteObject(hBitmap);
        EndPaint(hChdWnd, &ps);
        break;
    default:
        return (DefWindowProc(hChdWnd, msg, wp, lp));
    }
    return 0L;
}
