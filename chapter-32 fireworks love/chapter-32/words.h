#ifndef FIREWORK_WORD_H
#define FIREWORK_WORD_H
//---------------------------------------------------------------------------
#include <Windows.h>

#include <stdint.h>
//---------------------------------------------------------------------------
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
//---------------------------------------------------------------------------
struct firework_symbol
{
	int16_t* x;
	int16_t* y;

	unsigned int count;
};

struct firework_word
{
	struct firework_symbol p[1];

	float a, b, c;

	int16_t x;
	int16_t y;

	int16_t ox, oy;

	uint8_t n;
	// 
	uint8_t i;

	uint8_t d;
	uint8_t delay;

	uint8_t t;
	uint8_t red, green, blue;
};
//---------------------------------------------------------------------------
void word_initialize(struct firework_word* pfw);
void word_initialize(struct firework_word* pfw, int width, int height, int x, int y,
	void* pbits, unsigned int rowsize,
	uint8_t red, uint8_t green, uint8_t blue);
void word_uninitialize(struct firework_word* pfw);

int word_run(struct firework_word* pfw, HDC hdc);
//---------------------------------------------------------------------------
#endif