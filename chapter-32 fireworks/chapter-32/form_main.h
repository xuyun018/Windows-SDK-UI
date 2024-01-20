#ifndef FORM_MAIN_H
#define FORM_MAIN_H
//---------------------------------------------------------------------------
#include <Windows.h>

#include <stdint.h>

#include "fireworks.h"
//---------------------------------------------------------------------------
struct form_main
{
	HINSTANCE hinstance;

	uint32_t result;

	uint32_t idtimer;

	BITMAPINFO pbi[1];
	HBITMAP hbitmap;
	LPVOID pbits;
	uint32_t rowsize;
	uint32_t imagesize;

	int left, top;
	int right, bottom;

	struct firework* fireworks;
	unsigned int count;
};
//---------------------------------------------------------------------------
HWND form_create(struct form_main* pwnd, HWND hparent, HINSTANCE hinstance, HICON hicon,
	int x, int y, int width, int height,
	const WCHAR* classname, const WCHAR* title);
//---------------------------------------------------------------------------
#endif