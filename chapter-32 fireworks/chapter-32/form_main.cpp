#include "form_main.h"

#include <commctrl.h>

#include "gdi_bmp.h"

#include "sample02.h"
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

	pwnd->left = 0;
	pwnd->top = 0;
	pwnd->right = 0;
	pwnd->bottom = 0;

	pwnd->hbitmap = NULL;
	pwnd->pbits = NULL;

	srand(GetTickCount());

	unsigned int i;
	unsigned int count;

	count = rand() & 31;
	count += 65;
	pwnd->count = count;
	pwnd->fireworks = (struct firework*)MALLOC(count * sizeof(struct firework));
	if (pwnd->fireworks)
	{
		for (i = 0; i < count; i++)
		{
			firework_initialize(&pwnd->fireworks[i]);
		}
	}

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

	if (pwnd->fireworks)
	{
		unsigned int i, count;

		count = pwnd->count;
		for (i = 0; i < count; i++)
		{
			firework_uninitialize(&pwnd->fireworks[i]);
			firework_initialize(&pwnd->fireworks[i], width, height, -100, (height + height + height) >> 2);
		}
	}

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

	if (pwnd->fireworks)
	{
		unsigned int i, count;

		count = pwnd->count;
		for (i = 0; i < count; i++)
		{
			firework_uninitialize(&pwnd->fireworks[i]);
		}
		FREE(pwnd->fireworks);
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

		HDC hdc1;
		hdc1 = CreateCompatibleDC(hdc);
		if (pwnd->hbitmap)
		{
			memset(pwnd->pbits, 0, pwnd->imagesize);

			hobj0 = SelectObject(hdc1, pwnd->hbitmap);
		}

		if (pwnd->fireworks)
		{
			unsigned int i, count;

			count = pwnd->count;
			for (i = 0; i < count; i++)
			{
				if (firework_run(&pwnd->fireworks[i], hdc1))
				{
					firework_uninitialize(&pwnd->fireworks[i]);
					firework_initialize(&pwnd->fireworks[i], pwnd->right, pwnd->bottom, -100, (pwnd->bottom + pwnd->bottom + pwnd->bottom) >> 2);
				}
			}
		}

		BitBlt(hdc, 0, 0, pwnd->right, pwnd->bottom, hdc1, 0, 0, SRCCOPY);

		DeleteDC(hdc1);

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