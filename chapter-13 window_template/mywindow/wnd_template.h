#ifndef WND_TEMPLATE_H
#define WND_TEMPLATE_H
//---------------------------------------------------------------------------
#include <Windows.h>

#include <stdint.h>
//---------------------------------------------------------------------------
struct window_template
{
	uint32_t result;
};
//---------------------------------------------------------------------------
HWND wndtemplate_create(struct window_template* pwnd, HWND hparent, HINSTANCE hinstance, HICON hicon,
	int x, int y, int width, int height,
	const WCHAR* classname, const WCHAR* title);
//---------------------------------------------------------------------------
#endif