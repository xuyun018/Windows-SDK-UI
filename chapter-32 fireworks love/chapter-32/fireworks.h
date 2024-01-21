#ifndef FIREWORKS_H
#define FIREWORKS_H
//---------------------------------------------------------------------------
#include <Windows.h>

#include <math.h>

#include <stdint.h>
//---------------------------------------------------------------------------
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
//---------------------------------------------------------------------------
#define PI	3.1415926
#define G	9.8
//---------------------------------------------------------------------------
struct spark
{
	int16_t** x;
	int16_t** y;

	float v;

	uint8_t m;
	uint8_t n;
	uint8_t t;
	uint8_t i;
};

struct firework
{
	struct spark p[1];

	float a, b, c;

	int16_t x[3];
	int16_t y[3];

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
void firework_initialize(struct firework* pfw);
void firework_initialize(struct firework* pfw, int width, int height, int top, int bottom);
void firework_uninitialize(struct firework* pfw);

int firework_run(struct firework* pfw, HDC hdc);
//---------------------------------------------------------------------------
#endif