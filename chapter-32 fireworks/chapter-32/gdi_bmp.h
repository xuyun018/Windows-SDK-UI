#ifndef GDI_BMP_H
#define GDI_BMP_H
//---------------------------------------------------------------------------
#include <Windows.h>

#include <stdint.h>
//---------------------------------------------------------------------------
HBITMAP bitmap_create(LPVOID* pbits, uint32_t* rowsize, uint32_t* imagesize,
	BITMAPINFO* pbi,
	uint8_t bitcount, unsigned int width, unsigned int height);
//---------------------------------------------------------------------------
#endif