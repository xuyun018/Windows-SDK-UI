#include <windows.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

HFONT SetMyFont(HDC, LPCWSTR, int, int);

LPCWSTR szClassName = L"font01";    //类名

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

//注册窗口类

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
    wc.lpszClassName = szClassName;
    return (RegisterClass(&wc));
}

//创建窗口

BOOL InitInstance(HINSTANCE hInst, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(szClassName,
        L"Cat-Friendly font", //此名称将显示在标题栏中
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
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    return TRUE;
}

//窗口过程

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int id;
    HDC hdc;
    PAINTSTRUCT ps;
    HFONT hFont, hFontOld;

    switch (msg) {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hdc, TRANSPARENT);

        hFont = SetMyFont(hdc, (LPCWSTR)L"ＭＳ ゴシック", 40, 450);
        hFontOld = (HFONT)SelectObject(hdc, hFont);
        TextOut(hdc, 10, 300, (LPCWSTR)L"猫でもわかるフォント", 10);
        SelectObject(hdc, hFontOld);
        DeleteObject(hFont);

        hFont = SetMyFont(hdc, (LPCWSTR)L"ＭＳ 明朝", 40, -450);
        hFontOld = (HFONT)SelectObject(hdc, hFont);
        SetTextColor(hdc, RGB(255, 0, 0));
        TextOut(hdc, 10, 10, (LPCWSTR)L"粂井康孝　制作・著作", 10);
        SelectObject(hdc, hFontOld);
        DeleteObject(hFont);

        hFont = SetMyFont(hdc, (LPCWSTR)L"ＭＳ 明朝", 40, 0);
        hFontOld = (HFONT)SelectObject(hdc, hFont);
        SetTextColor(hdc, RGB(0, 255, 0));
        TextOut(hdc, 10, 150, (LPCWSTR)L"猫でもわかるフォント", 10);
        SelectObject(hdc, hFontOld);
        DeleteObject(hFont);

        EndPaint(hWnd, &ps);
        break;
    case WM_CLOSE:
        id = MessageBox(hWnd,
            (LPCWSTR)L"終了してもよいですか",
            (LPCWSTR)L"終了確認",
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

HFONT SetMyFont(HDC hdc, LPCTSTR face, int h, int angle)
{
    HFONT hFont;
    hFont = CreateFont(h,    // 字体高度
        0,                    //字符宽度
        angle,                    //文字的角度
        0,                    //基线与x轴的夹角
        FW_REGULAR,            //字体粗细（粗细）
        FALSE,                //斜体
        FALSE,                //下划线
        FALSE,                //删除线
        SHIFTJIS_CHARSET,    //字符集
        OUT_DEFAULT_PRECIS,    //输出精度
        CLIP_DEFAULT_PRECIS,//裁剪精度
        PROOF_QUALITY,        //输出质量
        FIXED_PITCH | FF_MODERN,//音高和族
        face);    //字体名称
    return hFont;
}