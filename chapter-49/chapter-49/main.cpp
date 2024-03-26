//        meta01.cpp

#include <windows.h>
#define IDO 50

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
void MakeMyMeta(void);
HMETAFILE MakeMyEllipse(int, int);

const WCHAR szClassName[] = L"meta01";        //窗口类名

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
    wc.lpfnWndProc = WndProc;    //过程名称
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;        //实例
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
        L"猫でもわかるメタファイル",    //这个名称将显示在标题栏中
        WS_OVERLAPPED | WS_SYSMENU,    //窗口类型
        CW_USEDEFAULT,    //Ｘ座標
        CW_USEDEFAULT,    //Ｙ座標
        258,//宽度
        75,    //高度
        NULL,            //父窗口句柄，创建父窗口时为NULL
        NULL,            //菜单句柄，使用类菜单时为NULL
        hInst,            //实例句柄
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
        //绘制背景
        GetClientRect(hWnd, &rc);
        hdc = BeginPaint(hWnd, &ps);
        MoveToEx(hdc, rc.left, rc.bottom / 2, NULL);
        LineTo(hdc, rc.right, rc.bottom / 2);
        MoveToEx(hdc, rc.left, rc.top, NULL);
        LineTo(hdc, rc.right, rc.bottom);
        MoveToEx(hdc, rc.left, rc.bottom, NULL);
        LineTo(hdc, rc.right, rc.top);
        //播放图元文件
        PlayMetaFile(hdc, hmf[play_no - 1]);
        EndPaint(hWnd, &ps);
        break;
    case WM_CLOSE:
        id = MessageBox(hWnd,
            (LPCWSTR)L"您确定要退出吗？",
            (LPCWSTR)L"終了確認",
            MB_YESNO | MB_ICONQUESTION);
        if (id == IDYES) {
            DestroyWindow(hWnd);
        }
        break;
    case WM_DESTROY:
        //释放图元文件句柄
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

HMETAFILE MakeMyEllipse(int x1, int x2) // 创建图元文件
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