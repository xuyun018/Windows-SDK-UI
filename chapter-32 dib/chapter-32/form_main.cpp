#include "form_main.h"

#include <commctrl.h>

#include "gdi_bmp.h"

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
	pwnd->first = 1;

	unsigned int i, n;

	n = wcslen(pwnd->text);
	pwnd->count = n;

	pwnd->x = (int16_t*)MALLOC(n * sizeof(int16_t));
	if (pwnd->x)
	{
		memset(pwnd->x, 0, n * sizeof(int16_t));
		pwnd->lx = pwnd->x[n - 1];
	}
	pwnd->y = (int16_t*)MALLOC(n * sizeof(int16_t));
	if (pwnd->y)
	{
		memset(pwnd->y, 0, n * sizeof(int16_t));
		pwnd->ly = pwnd->y[n - 1];
	}

	// 移动的距离
	// 开始都是正数
	pwnd->dx = 20;
	pwnd->dy = 20;

	pwnd->left = 0;
	pwnd->top = 0;
	pwnd->right = 0;
	pwnd->bottom = 0;

	pwnd->hbitmap = NULL;
	pwnd->pbits = NULL;

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

	if (pwnd->hbitmap)
	{
		DeleteObject(pwnd->hbitmap);
	}

	pwnd->hbitmap = bitmap_create(&pwnd->pbits, &pwnd->rowsize, &pwnd->imagesize,
		pwnd->pbi,
		32, width, height);

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

	if (pwnd->hbitmap)
	{
		DeleteObject(pwnd->hbitmap);
	}

	if (pwnd->x)
	{
		FREE(pwnd->x);
	}
	if (pwnd->y)
	{
		FREE(pwnd->y);
	}
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
		HGDIOBJ hobj0;
		HGDIOBJ hobj1;
		HPEN hpen;
		HBRUSH hbrush;
		unsigned int i;

		// 先擦掉之前的图形
		memset(pwnd->pbits, 0, pwnd->imagesize);

		pwnd->lx = pwnd->x[pwnd->count - 1];
		pwnd->ly = pwnd->y[pwnd->count - 1];

		int flag;

		do
		{
			flag = 0;

			pwnd->x[0] += pwnd->dx;
			if (pwnd->x[0] < 0 || pwnd->x[0] > pwnd->right)
			{
				pwnd->dx = -pwnd->dx;

				pwnd->x[0] += pwnd->dx;

				flag = 1;
			}
		} while (flag);

		do
		{
			flag = 0;

			pwnd->y[0] += pwnd->dy;
			if (pwnd->y[0] < 0 || pwnd->y[0] > pwnd->bottom)
			{
				pwnd->dy = -pwnd->dy;

				pwnd->y[0] += pwnd->dy;

				flag = 1;
			}
		} while (flag);

		HDC hdc1;

		hdc1 = CreateCompatibleDC(hdc);
		hobj1 = SelectObject(hdc1, pwnd->hbitmap);

		SetTextColor(hdc1, RGB(0, 0, 255));
		SetBkMode(hdc1, TRANSPARENT);

		HFONT hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

		hobj0 = SelectObject(hdc1, hfont);

		for (i = 0; i < pwnd->count; i++)
		{
			TextOut(hdc1, pwnd->x[i], pwnd->y[i], pwnd->text + i, 1);
		}

		BitBlt(hdc, 0, 0, pwnd->right, pwnd->bottom, hdc1, 0, 0, SRCCOPY);

		SelectObject(hdc1, hobj0);
		SelectObject(hdc1, hobj1);

		DeleteDC(hdc1);

		ReleaseDC(hwnd, hdc);

		for (i = pwnd->count; i > 1; i--)
		{
			pwnd->x[i - 1] = pwnd->x[i - 2];
			pwnd->y[i - 1] = pwnd->y[i - 2];
		}
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