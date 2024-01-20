#ifndef FORM_MAIN_H
#define FORM_MAIN_H
//---------------------------------------------------------------------------
#include <Windows.h>

#include <stdint.h>

#include "dlg_test.h"
//---------------------------------------------------------------------------
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
//---------------------------------------------------------------------------
struct form_main
{
	struct dialog_test pdlg[1];

	HINSTANCE hinstance;

	uint32_t result;

	uint32_t idtimer;

	BITMAPINFO pbi[1];
	HBITMAP hbitmap;
	LPVOID pbits;
	uint32_t rowsize;
	uint32_t imagesize;

	const WCHAR* text;
	unsigned int count;

	int16_t* x;
	int16_t* y;

	int16_t lx;
	int16_t ly;

	int first;
	// ÒÆ¶¯µÄ¾àÀë
	int dx, dy;

	int left, top;
	int right, bottom;
};
//---------------------------------------------------------------------------
HWND form_create(struct form_main* pwnd, HWND hparent, HINSTANCE hinstance, HICON hicon,
	int x, int y, int width, int height,
	const WCHAR* classname, const WCHAR* title);
//---------------------------------------------------------------------------
#endif