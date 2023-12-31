#ifndef DLG_TEMPLATE_H
#define DLG_TEMPLATE_H
//---------------------------------------------------------------------------
#include <Windows.h>

#include <stdint.h>
//---------------------------------------------------------------------------
struct dialog_template
{
	HWND hok;
	HWND hcancel;

	uint32_t result;
};
//---------------------------------------------------------------------------
HWND dlgtemplate_create(struct dialog_template* pdlg, HWND hparent, HINSTANCE hinstance, HICON hicon,
	int x, int y, int width, int height,
	const WCHAR* classname, const WCHAR* title);
//---------------------------------------------------------------------------
#endif