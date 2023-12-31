#include "dlg_template.h"

#include <commctrl.h>
//---------------------------------------------------------------------------
intptr_t dlgtemplate_on_create(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct dialog_template* pdlg;
	int width, height;
	CREATESTRUCT* pcs;
	HINSTANCE hinstance;
	HWND hbutton;

	pcs = (CREATESTRUCT*)lparam;

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(pdlg = (struct dialog_template*)pcs->lpCreateParams));

	hinstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

	RECT rect;
	GetClientRect((HWND)hwnd, &rect);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	pdlg->hok =
		hbutton = CreateWindowEx(0, WC_BUTTON, L"确定",
			WS_CHILD | WS_VISIBLE,
			width - 48 - 8 - 48 - 8, height - 32, 48, 24, hwnd, NULL, (HINSTANCE)hinstance, NULL);

	pdlg->hcancel =
		hbutton = CreateWindowEx(0, WC_BUTTON, L"取消",
			WS_CHILD | WS_VISIBLE,
			width - 48 - 8, height - 32, 48, 24, hwnd, NULL, (HINSTANCE)hinstance, NULL);

	return(0);
}
intptr_t dlgtemplate_on_resize(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct dialog_template* pdlg;
	int left, top;
	int right, bottom;
	int width, height;

	pdlg = (struct dialog_template*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	width = LOWORD(lparam);
	height = HIWORD(lparam);

	left = 0;
	top = 0;
	right = width;
	bottom = height;

	return(0);
}
void dlgtemplate_on_destroy(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct dialog_template* pdlg;

	pdlg = (struct dialog_template*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
}

intptr_t dlgtemplate_on_command(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct dialog_template* pdlg;

	pdlg = (struct dialog_template*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch (HIWORD(wparam))
	{
	case BN_CLICKED:
		if (lparam == (uintptr_t)pdlg->hok)
		{
			pdlg->result = IDOK;

			DestroyWindow(hwnd);
		}
		else if (lparam == (uintptr_t)pdlg->hcancel)
		{
			DestroyWindow(hwnd);
		}
		break;
	default:
		break;
	}

	return(0);
}

LRESULT CALLBACK dlgtemplate_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	intptr_t result = 0;

	switch (msg)
	{
	case WM_CREATE:
		dlgtemplate_on_create(hWnd, wParam, lParam);
		break;
	case WM_SIZE:
		dlgtemplate_on_resize(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		dlgtemplate_on_command(hWnd, wParam, lParam);
		break;
	case WM_DESTROY:
		dlgtemplate_on_destroy(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		break;

	default:
		break;
	}

	return(DefWindowProc(hWnd, msg, wParam, lParam));
}
//---------------------------------------------------------------------------
HWND dlgtemplate_create(struct dialog_template* pdlg, HWND hparent, HINSTANCE hinstance, HICON hicon,
	int x, int y, int width, int height, 
	const WCHAR *classname, const WCHAR *title)
{
	HWND hwnd;

	pdlg->result = IDCANCEL;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS | CS_OWNDC;
	wcex.lpfnWndProc = (WNDPROC)dlgtemplate_proc;
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

	// WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU 是标准窗口的 style

	hwnd = CreateWindowEx(0,
		wcex.lpszClassName,
		(LPCWSTR)title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		x, y,
		width, height,
		(HWND)hparent,
		NULL,
		wcex.hInstance,
		(LPVOID)pdlg);

	return(hwnd);
}
//---------------------------------------------------------------------------