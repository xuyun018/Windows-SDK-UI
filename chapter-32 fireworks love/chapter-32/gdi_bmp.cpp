#include "gdi_bmp.h"
//---------------------------------------------------------------------------
HBITMAP bitmap_create(LPVOID *pbits, uint32_t *rowsize, uint32_t *imagesize, 
	BITMAPINFO *pbi, 
	uint8_t bitcount, unsigned int width, unsigned int height)
{
	HBITMAP hbitmap;
	BITMAPINFOHEADER *pbih;

	*rowsize = ((width * bitcount + 31) & ~31) >> 3;
	*imagesize = *rowsize * height;

	pbih = &pbi->bmiHeader;

	pbih->biSize = sizeof(BITMAPINFOHEADER);
	pbih->biWidth = width;
	pbih->biHeight = height;
	pbih->biPlanes = 1;
	pbih->biBitCount = bitcount;
	pbih->biCompression = BI_RGB;
	pbih->biXPelsPerMeter = 0;
	pbih->biYPelsPerMeter = 0;
	pbih->biClrUsed = 0;
	pbih->biClrImportant = 0;

	HDC hdc = GetDC(NULL);

	hbitmap = CreateDIBSection(hdc, pbi, DIB_RGB_COLORS, pbits, NULL, 0);

	ReleaseDC(NULL, hdc);

	return(hbitmap);
}
//---------------------------------------------------------------------------