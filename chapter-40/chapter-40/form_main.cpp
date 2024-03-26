#include "form_main.h"

#include <commctrl.h>

#include "sample02.h"

#include "dlg_test.h"
//---------------------------------------------------------------------------
LRESULT CALLBACK MyHook(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_INITDIALOG:
		SetWindowText(hWnd, L"��è�����ö��Ĺ���");
		SetWindowText(GetDlgItem(hWnd, IDOK), L"����");
		SetWindowText(GetDlgItem(hWnd, IDCANCEL), L"���˳�");
		SetWindowText(GetDlgItem(hWnd, grp2), L"����һ��ʾ��");
		SetWindowText(GetDlgItem(hWnd, grp1), L"����һ��װ��(&K)");
		SetWindowText(GetDlgItem(hWnd, stc1), L"Font(&P)");
		SetWindowText(GetDlgItem(hWnd, stc2), L"Style(&S)");
		SetWindowText(GetDlgItem(hWnd, stc3), L"Size(&Z)");
		//SetWindwoText �� SetDlgItemText �����﹤����ͬ
		SetDlgItemText(hWnd, stc4, L"Color(&I)");
		//return TRUE;
		break;
	case WM_COMMAND:
		if (LOWORD(wp) == IDCANCEL) {
			if (MessageBox(hWnd,
				(LPCWSTR)L"��ȷ��Ҫ�˳���",
				(LPCWSTR)L"ȡ��",
				MB_OKCANCEL) == IDOK) {
				return FALSE;//����FALSE����ԭ����
			}
			else {
				return TRUE;//�������TRUE����ԭ������������
			}
		}
		break;
	default:
		return FALSE;
	}
	return FALSE;
}

int setcf(CHOOSEFONT* cf, LOGFONT *lf, HWND hwnd)
{
	cf->lStructSize = sizeof(CHOOSEFONT);
	cf->lpLogFont = lf;
	cf->hwndOwner = hwnd;
	cf->Flags = CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_ENABLEHOOK;
	cf->rgbColors = RGB(0, 0, 0);
	cf->nFontType = SCREEN_FONTTYPE;
	cf->lpfnHook = (LPCFHOOKPROC)MyHook;//ָ�����Ӻ���
	return 0;
}

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

	pwnd->draw_on = 0;

	pwnd->idtimer = SetTimer(hwnd, 1, 1000, NULL);

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
	right = width;
	bottom = height;

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
		const WCHAR* str_org = L"������%2d��%4d��%2d��%2dСʱ%2d��%2d��";

		SYSTEMTIME st;

		WCHAR text[256];
		GetLocalTime(&st);
		wsprintf(text, str_org, st.wMonth, st.wYear, st.wDay, st.wHour, st.wMinute, st.wSecond);
		SetWindowText(hwnd, text);
	}
}

void form_on_paint(HWND hwnd, uintptr_t wparam, uintptr_t lparam)
{
	struct form_main* pwnd;

	pwnd = (struct form_main*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (pwnd->draw_on)
	{
		PAINTSTRUCT ps;
		HFONT hfont;
		HGDIOBJ hobj;

		HDC hdc = BeginPaint(hwnd, &ps);
		hfont = CreateFontIndirect(pwnd->cf.lpLogFont);
		hobj = SelectObject(hdc, hfont);
		//crOldc = 
		SetTextColor(hdc, pwnd->cf.rgbColors);
		TextOut(hdc, 10, 10, (LPCWSTR)L"èҲ������", 5);
		SelectObject(hdc, hobj);
		//SetTextColor(hdc, crOldc);
		DeleteObject(hfont);
		EndPaint(hwnd, &ps);
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
				setcf(&pwnd->cf, &pwnd->lf, hwnd);

				if (ChooseFont(&pwnd->cf)) {
					pwnd->draw_on = 1;
					InvalidateRect(hwnd, NULL, TRUE);
				}
			}
			if (0)
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

	case WM_PAINT:
		form_on_paint(hWnd, wParam, lParam);
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