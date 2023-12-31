#ifndef DLG_TEST_H
#define DLG_TEST_H
//---------------------------------------------------------------------------
#include <Windows.h>

#include <stdint.h>
//---------------------------------------------------------------------------
struct dialog_test
{
	HWND hok;
	HWND hcancel;

	uint32_t result;
};
//---------------------------------------------------------------------------
HWND dlgtest_create(struct dialog_test* pdlg, HWND hparent, HINSTANCE hinstance, HICON hicon,
	int x, int y, int width, int height,
	const WCHAR* classname, const WCHAR* title);
//---------------------------------------------------------------------------
#endif