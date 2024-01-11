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

	AppendMenu(hsubmenu, MF_STRING, IDM_NEW, L"�½�");
	AppendMenu(hsubmenu, MF_STRING, IDM_OPEN, L"��");
	AppendMenu(hsubmenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hsubmenu, MF_STRING, IDM_EXIT, L"�˳�");

	AppendMenu(hmenu, MF_POPUP, (UINT_PTR)hsubmenu, L"�ļ�");

	SetMenu(hwnd, hmenu);

	//hmenu = GetSystemMenu(hwnd, FALSE);
	//for (int i = 0; i < 6; i++)
	//	DeleteMenu(hmenu, 0, MF_BYPOSITION);
	//AppendMenu(hmenu, MF_STRING, IDM_NEW, L"�����Ŀ");
	//DrawMenuBar(hwnd);
	pwnd->first = 1;

	pwnd->r = 10;
	// ��ǰ����
	pwnd->x = 0;
	pwnd->y = 0;
	// �ƶ��ľ���
	// ��ʼ��������
	pwnd->dx = 10;
	pwnd->dy = 10;

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

void form_on_timer(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct form_main* pwnd;

	pwnd = (struct form_main*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (pwnd->idtimer == wparam)
	{
		//const WCHAR* str_org = L"������%2d��%4d��%2d��%2dСʱ%2d��%2d��";

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

		if (!pwnd->first)
		{
			// �Ȳ���֮ǰ��Բ��
			hpen = (HPEN)GetStockObject(WHITE_PEN);
			hbrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
			hobj0 = SelectObject(hdc, hpen);
			hobj1 = SelectObject(hdc, hbrush);
			Ellipse(hdc, pwnd->x - pwnd->r, pwnd->y - pwnd->r, pwnd->x + pwnd->r, pwnd->y + pwnd->r);
			SelectObject(hdc, hobj0);
			SelectObject(hdc, hobj1);
		}
		pwnd->first = 0;

		pwnd->x += pwnd->dx;
		if (pwnd->x - pwnd->r < 0 || pwnd->x + pwnd->r > pwnd->right)
		{
			pwnd->dx = -pwnd->dx;
		}
		pwnd->y += pwnd->dy;
		if (pwnd->y - pwnd->r < 0 || pwnd->y + pwnd->r > pwnd->bottom)
		{
			pwnd->dy = -pwnd->dy;
		}

		hpen = (HPEN)GetStockObject(BLACK_PEN);
		hbrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		hobj0 = SelectObject(hdc, hpen);
		hobj1 = SelectObject(hdc, hbrush);

		Ellipse(hdc, pwnd->x - pwnd->r, pwnd->y - pwnd->r, pwnd->x + pwnd->r, pwnd->y + pwnd->r);

		SelectObject(hdc, hobj0);
		SelectObject(hdc, hobj1);

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
					L"MyDialogClass", L"���ԶԻ���");
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

	// WS_OVERLAPPEDWINDOW �Ǳ�׼���ڵ� style

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