//        chd01.cpp
#define STRICT
#include <windows.h>
#define ID_MYCHILD 100

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

BOOL InitApp(HINSTANCE, WNDPROC, LPCTSTR);
BOOL InitInstance(HINSTANCE, int, LPCTSTR);

int WINAPI wWinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
    LPWSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    LPCWSTR szClassName = L"chd01";        //��������

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

BOOL InitInstance(HINSTANCE hInst, int nCmdShow, LPCTSTR szClassName)
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
    int id;
    static HWND hChdWnd;
    HINSTANCE hInst;

    switch (msg) {
    case WM_CREATE:
        hInst = ((LPCREATESTRUCT)lp)->hInstance;
        InitApp(hInst,
            ChildProc,
            L"child");
        hChdWnd = CreateWindow(L"child",
            L"�ӹ�������ɥ�",	//�����ƽ���ʾ�ڱ�������
            WS_CHILD | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION |
            WS_MINIMIZEBOX | WS_MAXIMIZEBOX,    //������ɥ��ηN�
            CW_USEDEFAULT,    //������
            CW_USEDEFAULT,    //������
            200,    //��
            100,    //�ߤ�
            hWnd,            //�����ھ��������������ʱΪNULL
            (HMENU)ID_MYCHILD, //�˵��������ID
            hInst,            //ʵ�����
            NULL);
        ShowWindow(hChdWnd, SW_SHOW);
        UpdateWindow(hChdWnd);
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

LRESULT CALLBACK ChildProc(HWND hChdWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
    switch (msg) {
    case WM_CLOSE:
        id = MessageBox(hChdWnd,
            L"�ҿ��Թر��Ӵ�����",
            L"ȷ�����", MB_YESNO | MB_ICONQUESTION);
        if (id == IDYES)
            DestroyWindow(hChdWnd);
        break;
    default:
        break;
    }
    return (DefWindowProc(hChdWnd, msg, wp, lp));
}