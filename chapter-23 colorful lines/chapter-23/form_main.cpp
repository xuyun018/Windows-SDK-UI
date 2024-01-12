#include "form_main.h"

#include <commctrl.h>

#include "sample02.h"

#include "dlg_test.h"
//---------------------------------------------------------------------------
intptr_t form_on_create(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct form_main* pwnd;
	int width, height;
	CREATESTRUCT* pcs;
	HINSTANCE hinstance;
	HWND hbutton;

	pcs = (CREATESTRUCT*)lparam;

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(pwnd = (struct form_main*)pcs->lpCreateParams));

	hinstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

	pwnd->hinstance = hinstance;

	RECT rect;
	GetClientRect((HWND)hwnd, &rect);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	HMENU hmenu;
	HMENU hsubmenu;

	hmenu = CreateMenu();
	hsubmenu = CreatePopupMenu();

	AppendMenu(hsubmenu, MF_STRING, IDM_NEW, L"新建");
	AppendMenu(hsubmenu, MF_STRING, IDM_OPEN, L"打开");
	AppendMenu(hsubmenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hsubmenu, MF_STRING, IDM_EXIT, L"退出");

	AppendMenu(hmenu, MF_POPUP, (UINT_PTR)hsubmenu, L"文件");

	SetMenu(hwnd, hmenu);

	//hmenu = GetSystemMenu(hwnd, FALSE);
	//for (int i = 0; i < 6; i++)
	//	DeleteMenu(hmenu, 0, MF_BYPOSITION);
	//AppendMenu(hmenu, MF_STRING, IDM_NEW, L"添加项目");
	//DrawMenuBar(hwnd);

	srand(GetTickCount());

	RECT rectangle;

	GetClientRect(hwnd, &rectangle);
	width = rectangle.right - rectangle.left;
	height = rectangle.bottom - rectangle.top;

	// 当前坐标
	pwnd->x0 = rand() % width;
	pwnd->y0 = rand() % height;
	// 移动的距离
	// 开始都是正数
	pwnd->dx0 = rand() % 10;
	pwnd->dx0++;
	pwnd->dy0 = rand() % 10;
	pwnd->dy0++;

	pwnd->x1 = rand() % width;
	pwnd->y1 = rand() % height;
	// 移动的距离
	// 开始都是正数
	pwnd->dx1 = rand() % 20;
	pwnd->dx1++;
	pwnd->dy1 = rand() % 20;
	pwnd->dy1++;

	pwnd->left = 0;
	pwnd->top = 0;
	pwnd->right = 0;
	pwnd->bottom = 0;

	pwnd->idtimer = SetTimer(hwnd, 1, 10, NULL);

	return(0);
}
intptr_t form_on_resize(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct form_main* pwnd;
	int left, top;
	int right, bottom;
	int width, height;

	pwnd = (struct form_main*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	width = LOWORD(lparam);
	height = HIWORD(lparam);

	left = 0;
	top = 0;
	pwnd->right = width;
	pwnd->bottom = height;

	return(0);
}
void form_on_destroy(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct form_main* pwnd;

	pwnd = (struct form_main*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (pwnd->idtimer)
	{
		KillTimer(hwnd, pwnd->idtimer);
	}
}

int DrawGr(HWND hWnd, HDC hdc)
{
	HPEN hPen;
	HGDIOBJ hOldPen;
	RECT rt;
	GetClientRect(hWnd, &rt);

	hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	hOldPen = SelectObject(hdc, hPen);
	MoveToEx(hdc, 0, 0, NULL);
	LineTo(hdc, rt.right, rt.bottom);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);

	hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 255));
	hOldPen = SelectObject(hdc, hPen);
	MoveToEx(hdc, rt.right, 0, NULL);
	LineTo(hdc, 0, rt.bottom);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
	return 0;
}

void form_on_paint(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct form_main* pwnd;

	pwnd = (struct form_main*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hwnd, &ps);
	DrawGr(hwnd, hdc);
	EndPaint(hwnd, &ps);
}

void form_on_timer(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct form_main* pwnd;

	pwnd = (struct form_main*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (pwnd->idtimer == wparam)
	{
		//const WCHAR* str_org = L"现在是%2d月%4d年%2d日%2d小时%2d分%2d秒";

		//SYSTEMTIME st;

		//WCHAR text[256];
		//GetLocalTime(&st);
		//wsprintf(text, str_org, st.wMonth, st.wYear, st.wDay, st.wHour, st.wMinute, st.wSecond);
		//SetWindowText(hwnd, text);

		HDC hdc = GetDC(hwnd);
		HGDIOBJ hobj;
		HPEN hpen;

		pwnd->x0 += pwnd->dx0;
		if (pwnd->x0 < 0 || pwnd->x0 > pwnd->right)
		{
			pwnd->dx0 = -pwnd->dx0;
		}
		pwnd->y0 += pwnd->dy0;
		if (pwnd->y0 < 0 || pwnd->y0 > pwnd->bottom)
		{
			pwnd->dy0 = -pwnd->dy0;
		}
		pwnd->x1 += pwnd->dx1;
		if (pwnd->x1 < 0 || pwnd->x1 > pwnd->right)
		{
			pwnd->dx1 = -pwnd->dx1;
		}
		pwnd->y1 += pwnd->dy1;
		if (pwnd->y1 < 0 || pwnd->y1 > pwnd->bottom)
		{
			pwnd->dy1 = -pwnd->dy1;
		}

		hpen = (HPEN)CreatePen(PS_SOLID, 5, RGB(rand() & 0xff, rand() & 0xff, rand() & 0xff));
		hobj = SelectObject(hdc, hpen);

		MoveToEx(hdc, pwnd->x0, pwnd->y0, NULL);
		LineTo(hdc, pwnd->x1, pwnd->y1);

		SelectObject(hdc, hobj);

		DeleteObject(hpen);

		ReleaseDC(hwnd, hdc);
	}
}

intptr_t form_on_command(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct form_main* pwnd;

	pwnd = (struct form_main*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch (HIWORD(wparam))
	{
	case BN_CLICKED:
		switch (LOWORD(wparam))
		{
		case IDM_NEW:
			if (1)
			{
				struct dialog_test *pdlg = pwnd->pdlg;
				int width = 400;
				int height = 300;

				dlgtest_create(pdlg, hwnd, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL,
					(GetSystemMetrics(SM_CXSCREEN) - width) >> 1, (GetSystemMetrics(SM_CYSCREEN) - height) >> 1, width, height,
					L"MyDialogClass", L"测试对话框");
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return(0);
}

LRESULT CALLBACK form_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	intptr_t result = 0;

	switch (msg)
	{
	case WM_CREATE:
		form_on_create(hWnd, wParam, lParam);
		break;
	case WM_SIZE:
		form_on_resize(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		form_on_command(hWnd, wParam, lParam);
		break;
	case WM_DESTROY:
		form_on_destroy(hWnd, wParam, lParam);

		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		break;

	case WM_PAINT:
		//form_on_paint(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		form_on_timer(hWnd, wParam, lParam);
		break;

	default:
		break;
	}

	return(DefWindowProc(hWnd, msg, wParam, lParam));
}
//---------------------------------------------------------------------------
HWND form_create(struct form_main* pwnd, HWND hparent, HINSTANCE hinstance, HICON hicon,
	int x, int y, int width, int height, 
	const WCHAR *classname, const WCHAR *title)
{
	HWND hwnd;

	pwnd->result = IDCANCEL;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS | CS_OWNDC;
	wcex.lpfnWndProc = (WNDPROC)form_proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = (HINSTANCE)hinstance;
	wcex.hIcon = hicon;
	wcex.hIconSm = hicon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = (LPCWSTR)classname;

	RegisterClassEx(&wcex);

	// WS_OVERLAPPEDWINDOW 是标准窗口的 style

	hwnd = CreateWindowEx(0,
		wcex.lpszClassName,
		(LPCWSTR)title,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		x, y,
		width, height,
		(HWND)hparent,
		NULL,
		wcex.hInstance,
		(LPVOID)pwnd);

	return(hwnd);
}
//---------------------------------------------------------------------------