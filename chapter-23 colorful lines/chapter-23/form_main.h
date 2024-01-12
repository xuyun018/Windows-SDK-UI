#ifndef FORM_MAIN_H
#define FORM_MAIN_H
//---------------------------------------------------------------------------
#include <Windows.h>

#include <stdint.h>

#include "dlg_test.h"
//---------------------------------------------------------------------------
struct form_main
{
	struct dialog_test pdlg[1];

	HINSTANCE hinstance;

	uint32_t result;

	uint32_t idtimer;

	// ��ǰ����
	int x0, y0;
	// �ƶ��ľ���
	int dx0, dy0;

	int x1, y1;
	// �ƶ��ľ���
	int dx1, dy1;

	int left, top;
	int right, bottom;
};
//---------------------------------------------------------------------------
HWND form_create(struct form_main* pwnd, HWND hparent, HINSTANCE hinstance, HICON hicon,
	int x, int y, int width, int height,
	const WCHAR* classname, const WCHAR* title);
//---------------------------------------------------------------------------
#endif