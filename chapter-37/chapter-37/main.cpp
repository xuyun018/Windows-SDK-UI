#include <windows.h>
#include <CommCtrl.h>
#include <stdlib.h>  //_wtoi�v����ʹ���ΤǱ�Ҫ

#define ID_EDIT1   1000
#define ID_EDIT2    1010
#define ID_BUTTON1    1020
#define ID_BUTTON2    1030

HWND hEWnd1, hEWnd2, hBWnd1, hBWnd2; //��׼�ؼ���HWND
WNDPROC Org_EWnd1, Org_EWnd2;   //ԭʼ���̵�ַ

WCHAR name[30], birth[30];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MySubProc1(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MySubProc2(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

int CalcAge(HWND); //���㲢��ʾ����
int focus; //�༭���н���Ŀؼ���
//1: hEWnd1 2: hEWnd2

WCHAR szClassName[] = L"subcl01";        //��������

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
    wc.lpfnWndProc = WndProc;    //�ץ����`������
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


BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"èҲ����������໯",    //�����ȥ�Щ`�ˤ�����ǰ����ʾ����ޤ�
        WS_OVERLAPPED | WS_SYSMENU,    //������ɥ��ηN�
        CW_USEDEFAULT,    //������
        CW_USEDEFAULT,    //������
        250,    //��
        120,    //�ߤ�
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
    PAINTSTRUCT ps;
    HDC hdc;
    HINSTANCE hInst;

    switch (msg) {
    case WM_CREATE:
        hInst = ((LPCREATESTRUCT)lp)->hInstance;
        hEWnd1 = CreateWindow(WC_EDIT,
            L"�ڴ�����",
            WS_CHILD | WS_VISIBLE,
            60, 10,
            100, 20,
            hWnd,
            (HMENU)ID_EDIT1,
            hInst,
            NULL);
        //���໯����
        Org_EWnd1 = (WNDPROC)GetWindowLongPtr(hEWnd1, GWLP_WNDPROC);
        SetWindowLongPtr(hEWnd1, GWLP_WNDPROC, (LONG_PTR)MySubProc1);

        hEWnd2 = CreateWindow(WC_EDIT,
            L"YYYY",
            WS_CHILD | WS_VISIBLE,
            120, 30,
            100, 20,
            hWnd,
            (HMENU)ID_EDIT2,
            hInst,
            NULL);
        // ���໯����
        Org_EWnd2 = (WNDPROC)GetWindowLongPtr(hEWnd2, GWLP_WNDPROC);
        SetWindowLongPtr(hEWnd2, GWLP_WNDPROC, (LONG)MySubProc2);

        hBWnd1 = CreateWindow(WC_BUTTON,
            L"��ʼ����",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 50,
            100, 30,
            hWnd,
            (HMENU)ID_BUTTON1,
            hInst,
            NULL);
        hBWnd2 = CreateWindow(WC_BUTTON,
            L"���",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            130, 50,
            100, 30,
            hWnd,
            (HMENU)ID_BUTTON2,
            hInst,
            NULL);
        SetFocus(hEWnd1);
        focus = 1;
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 10, 10, (LPCWSTR)L"������", 3);
        TextOut(hdc, 10, 30, (LPCWSTR)L"���꣨��������", 7);
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case ID_BUTTON1:
            GetWindowText(hEWnd1, (LPWSTR)name, 30);
            GetWindowText(hEWnd2, (LPWSTR)birth, 30);
            CalcAge(hWnd);
            if (focus == 1)
                SetFocus(hEWnd1);
            if (focus == 2)
                SetFocus(hEWnd2);
            break;
        case ID_BUTTON2:
            SetWindowText(hEWnd1, NULL);
            SetWindowText(hEWnd2, NULL);
            if (focus == 1)
                SetFocus(hEWnd1);
            if (focus == 2)
                SetFocus(hEWnd2);
            break;
        default:
            return(DefWindowProc(hWnd, msg, wp, lp));
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
        //��ɺ�ȡ�����໯
        //�ⲻ�Ǳ���ģ���ΪӦ�ó�����ֹ��
        SetWindowLongPtr(hEWnd1, GWLP_WNDPROC, (LONG_PTR)Org_EWnd1);
        SetWindowLongPtr(hEWnd2, GWLP_WNDPROC, (LONG_PTR)Org_EWnd2);
        PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0L;
}

int CalcAge(HWND hWnd)
{
    SYSTEMTIME st;
    int age;
    WCHAR str[256];
    const WCHAR* str_org = L"%s %d ��";

    GetLocalTime(&st);
    age = st.wYear - _wtoi(birth);
    wsprintf(str, str_org, name, age);
    MessageBox(hWnd, str, L"����", MB_OK);
    return 0;
}

//���໯�˳�����1���������룩
LRESULT CALLBACK MySubProc1(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_KEYDOWN:
        //�����¼�ͷ��ѡ�����ʱ�����������Ϊ���
        if ((int)wp == VK_DOWN || (int)wp == VK_TAB) {
            SetFocus(hEWnd2);
            focus = 2;
            return 0L; //������Ϣ�󷵻�0
        }
        break;
    default:
        break;
    }
    //�Լ����������������ԭ���ĳ���ȥ��
    return (CallWindowProc(Org_EWnd1, hWnd, msg, wp, lp));
}

//���໯�˳�����2��������룩
LRESULT CALLBACK MySubProc2(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_KEYDOWN:
        //�����ϼ�����tab������ʱ�򣬾۽���������
        if ((int)wp == VK_UP || (int)wp == VK_TAB) {
            SetFocus(hEWnd1);
            focus = 1;
            return 0L;
        }
        break;
    default:
        break;
    }
    return (CallWindowProc(Org_EWnd2, hWnd, msg, wp, lp));
}
