#include "gui_common.h"
//---------------------------------------------------------------------------
void set_font(HWND hwnd, HFONT hfont)
{
	if (hfont == NULL)
	{
		hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	}
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hfont, MAKELPARAM(TRUE, 0));
}
void set_font(HWND hwnd)
{
	set_font(hwnd, NULL);
}
//---------------------------------------------------------------------------