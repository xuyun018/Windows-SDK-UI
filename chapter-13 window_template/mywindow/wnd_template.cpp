#include "wnd_template.h"

#include <commctrl.h>
//---------------------------------------------------------------------------
intptr_t wndtemplate_on_create(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct window_template* pwnd;
	int width, height;
	CREATESTRUCT* pcs;
	HINSTANCE hinstance;
	HWND hbutton;

	pcs = (CREATESTRUCT*)lparam;

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(pwnd = (struct window_template*)pcs->lpCreateParams));

	hinstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

	RECT rect;
	GetClientRect((HWND)hwnd, &rect);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	return(0);
}
intptr_t wndtemplate_on_resize(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct window_template* pwnd;
	int left, top;
	int right, bottom;
	int width, height;

	pwnd = (struct window_template*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	width = LOWORD(lparam);
	height = HIWORD(lparam);

	left = 0;
	top = 0;
	right = width;
	bottom = height;

	return(0);
}
void wndtemplate_on_destroy(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct window_template* pwnd;

	pwnd = (struct window_template*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
}

intptr_t wndtemplate_on_command(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct window_template* pwnd;

	pwnd = (struct window_template*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch (HIWORD(wparam))
	{
	case BN_CLICKED:
		break;
	default:
		break;
	}

	return(0);
}

LRESULT CALLBACK wndtemplate_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	intptr_t result = 0;

	switch (msg)
	{
	case WM_CREATE:
		wndtemplate_on_create(hWnd, wParam, lParam);
		break;
	case WM_SIZE:
		wndtemplate_on_resize(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		wndtemplate_on_command(hWnd, wParam, lParam);
		break;
	case WM_DESTROY:
		wndtemplate_on_destroy(hWnd, wParam, lParam);

		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		break;

	default:
		break;
	}

	return(DefWindowProc(hWnd, msg, wParam, lParam));
}
//---------------------------------------------------------------------------
HWND wndtemplate_create(struct window_template* pwnd, HWND hparent, HINSTANCE hinstance, HICON hicon,
	int x, int y, int width, int height, 
	const WCHAR *classname, const WCHAR *title)
{
	HWND hwnd;

	pwnd->result = IDCANCEL;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS | CS_OWNDC;
	wcex.lpfnWndProc = (WNDPROC)wndtemplate_proc;
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