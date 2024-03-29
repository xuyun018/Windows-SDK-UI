#include "dlg_test.h"

#include <windowsx.h>
#include <commctrl.h>

#include "gui_common.h"
//---------------------------------------------------------------------------
intptr_t dlgtest_on_create(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct dialog_test* pdlg;
	int width, height;
	CREATESTRUCT* pcs;
	HINSTANCE hinstance;
	union
	{
		HWND hbutton;
		HWND hlistbox;
		HWND hcombobox;
		HWND hscrollbar;
	};

	pcs = (CREATESTRUCT*)lparam;

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(pdlg = (struct dialog_test*)pcs->lpCreateParams));

	hinstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

	RECT rect;
	GetClientRect((HWND)hwnd, &rect);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	pdlg->hok =
		hbutton = CreateWindowEx(0, WC_BUTTON, L"确定",
			WS_CHILD | WS_VISIBLE,
			width - 48 - 8 - 48 - 8, height - 32, 48, 24, hwnd, NULL, (HINSTANCE)hinstance, NULL);

	set_font(hbutton);

	pdlg->hcancel =
		hbutton = CreateWindowEx(0, WC_BUTTON, L"取消",
			WS_CHILD | WS_VISIBLE,
			width - 48 - 8, height - 32, 48, 24, hwnd, NULL, (HINSTANCE)hinstance, NULL);

	set_font(hbutton);

	pdlg->hlistbox = hlistbox = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, NULL,
		WS_CHILD | WS_VSCROLL | WS_TABSTOP | WS_VISIBLE | LBS_NOINTEGRALHEIGHT,
		8, 8, width - 16, 190, hwnd, NULL, (HINSTANCE)hinstance, NULL);

	set_font(hlistbox);

	pdlg->scroll_pos = 0;

	LPCWSTR liststr[] = { L"广州", L"湛江", L"北京", L"济南"};
	unsigned int i;

	for (i = 0; i < sizeof(liststr) / sizeof(liststr[0]); i++)
	{
		ListBox_AddString(hlistbox, liststr[i]);
	}

	//pdlg->hcombobox = hcombobox = CreateWindowEx(WS_EX_CLIENTEDGE, WC_COMBOBOX, NULL,
	//	WS_CHILD | WS_VSCROLL | WS_TABSTOP | WS_VISIBLE | CBS_DROPDOWN,
	//	8, 206, width - 16, 200, hwnd, NULL, (HINSTANCE)hinstance, NULL);

	pdlg->hscrollbar = hscrollbar = CreateWindowEx(WS_EX_CLIENTEDGE, WC_SCROLLBAR, NULL,
		WS_CHILD | WS_VISIBLE,
		8, 206, width - 16, 22, hwnd, NULL, (HINSTANCE)hinstance, NULL);

	SetScrollRange(hscrollbar, SB_CTL, 0, 100, TRUE);

	return(0);
}
intptr_t dlgtest_on_resize(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct dialog_test* pdlg;
	int left, top;
	int right, bottom;
	int width, height;

	pdlg = (struct dialog_test*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	width = LOWORD(lparam);
	height = HIWORD(lparam);

	left = 0;
	top = 0;
	right = width;
	bottom = height;

	return(0);
}
void dlgtest_on_destroy(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct dialog_test* pdlg;

	pdlg = (struct dialog_test*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
}

intptr_t dlgtest_on_command(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct dialog_test* pdlg;

	pdlg = (struct dialog_test*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

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

intptr_t dlgtest_on_hscroll(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct dialog_test* pdlg;

	pdlg = (struct dialog_test*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	WCHAR debugtext[256];
	wsprintf(debugtext, L"%p, %p\r\n", pdlg->hscrollbar, (void*)lparam);
	OutputDebugString(debugtext);

	switch (LOWORD(wparam)) {
	case SB_LINELEFT:
		pdlg->scroll_pos--;
		break;
	case SB_LINERIGHT:
		pdlg->scroll_pos++;
		break;
	case SB_PAGELEFT:
		pdlg->scroll_pos -= 10;
		break;
	case SB_PAGERIGHT:
		pdlg->scroll_pos += 10;
		break;
	case SB_THUMBTRACK:
		pdlg->scroll_pos = (int)(HIWORD(wparam));
		break;
	default:
		break;
	}
	if (pdlg->scroll_pos < 0)
	{
		pdlg->scroll_pos = 0;
	}
	if (pdlg->scroll_pos > 100)
	{
		pdlg->scroll_pos = 100;
	}
	SetScrollPos(pdlg->hscrollbar, SB_CTL, pdlg->scroll_pos, TRUE);

	wsprintf(debugtext, L"%3d", pdlg->scroll_pos);
	SetWindowText(hwnd, (LPCTSTR)debugtext);
	return(0);
}

LRESULT CALLBACK dlgtest_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	intptr_t result = 0;

	switch (msg)
	{
	case WM_CREATE:
		dlgtest_on_create(hWnd, wParam, lParam);
		break;
	case WM_SIZE:
		dlgtest_on_resize(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		dlgtest_on_command(hWnd, wParam, lParam);
		break;
	case WM_DESTROY:
		dlgtest_on_destroy(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		break;
	case WM_HSCROLL:
		dlgtest_on_hscroll(hWnd, wParam, lParam);
		break;
	default:
		break;
	}

	return(DefWindowProc(hWnd, msg, wParam, lParam));
}
//---------------------------------------------------------------------------
HWND dlgtest_create(struct dialog_test* pdlg, HWND hparent, HINSTANCE hinstance, HICON hicon,
	int x, int y, int width, int height, 
	const WCHAR *classname, const WCHAR *title)
{
	HWND hwnd;

	pdlg->result = IDCANCEL;

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_DBLCLKS | CS_OWNDC;
	wcex.lpfnWndProc = (WNDPROC)dlgtest_proc;
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