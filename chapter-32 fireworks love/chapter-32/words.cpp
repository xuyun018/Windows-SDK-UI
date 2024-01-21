#include "words.h"
//---------------------------------------------------------------------------
unsigned int scan_text(void* pbits, int width, int height, unsigned int rowsize,
	int16_t* x, int16_t* y)
{
	unsigned char* _p;
	unsigned char* p;
	int i, j;
	unsigned int result = 0;

	_p = (unsigned char*)pbits;
	if (x == NULL || y == NULL)
	{
		for (j = 0; j < height; j++)
		{
			p = _p;
			for (i = 0; i < width; i++)
			{
				if (p[0] || p[1] || p[2])
				{
					result++;
				}

				p += 4;
			}

			_p += rowsize;
		}
	}
	else
	{
		for (j = 0; j < height; j++)
		{
			p = _p;
			for (i = 0; i < width; i++)
			{
				if (p[0] || p[1] || p[2])
				{
					x[result] = i;
					y[result] = height - j - 1;

					result++;
				}

				p += 4;
			}

			_p += rowsize;
		}
	}

	return(result);
}
void symbol_initialize(struct firework_symbol* p)
{
	p->x = NULL;
	p->y = NULL;
}
void symbol_initialize(struct firework_symbol* p, int16_t ox, int16_t oy,
	void *pbits, int width, int height, unsigned int rowsize)
{
	unsigned int count;
	unsigned int i;
	int x = 0;
	int y = 0;

	count = scan_text(pbits, width, height, rowsize,
		NULL, NULL);
	p->x = (int16_t*)MALLOC(count * sizeof(int16_t));
	p->y = (int16_t*)MALLOC(count * sizeof(int16_t));
	p->count = scan_text(pbits, width, height, rowsize,
		p->x, p->y);

	for (i = 0; i < count; i++)
	{
		x += p->x[i];
		y += p->y[i];
	}
	x /= i;
	y /= i;
	for (i = 0; i < count; i++)
	{
		p->x[i] -= x;
		p->x[i] <<= 1;
		p->x[i] += ox;
		p->y[i] -= y;
		p->y[i] <<= 1;
		p->y[i] += oy;
	}
}
void symbol_uninitialize(struct firework_symbol* p)
{
	if (p->x)
	{
		FREE(p->x);
	}
	if (p->y)
	{
		FREE(p->y);
	}
}

int symbol_run(struct firework_symbol* p, int ox, int oy, uint8_t red, uint8_t green, uint8_t blue,
	HDC hdc)
{
	int x, y;
	unsigned int k;
	int result = 0;
	uint8_t t;
	uint8_t i;
	uint8_t j;

	{
		HGDIOBJ hobj0;
		HGDIOBJ hobj1;
		HPEN hpen;
		HBRUSH hbrush;
		uint16_t rv;
		uint8_t rs;
		uint8_t r, g, b;

		rv = rand();
		rs = 0;

		r = red;
		g = green;
		b = blue;
		hpen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
		hbrush = CreateHatchBrush(0, RGB(r, g, b));
		hobj0 = SelectObject(hdc, hpen);
		hobj1 = SelectObject(hdc, hbrush);

		float _g;
		float _a;

		i = 0;
		for (k = 0; k < p->count; k++)
		{
			x = p->x[k];
			y = p->y[k];

			if ((rv >> rs) & 1)
			{
				if ((rv >> rs) & 2)
				{
					x++;
					//x++;
				}
				else
				{
					x--;
					//x--;
				}
			}
			rs += 2;
			if (rs == 16)
			{
				rs = 0;
			}
			if ((rv >> rs) & 1)
			{
				if ((rv >> rs) & 2)
				{
					y++;
					//y++;
				}
				else
				{
					y--;
					//y--;
				}
			}
			rs += 2;
			if (rs == 16)
			{
				rs = 0;
			}

			Rectangle(hdc, x - 1, y - 1, x + 1, y + 1);
		}

		SelectObject(hdc, hobj0);
		SelectObject(hdc, hobj1);
		DeleteObject(hpen);
		DeleteObject(hbrush);
	}
	return(result);
}

void word_initialize(struct firework_word* pfw)
{
	symbol_initialize(pfw->p);
}
void word_initialize(struct firework_word* pfw, int width, int height, int x, int y,
	void *pbits, unsigned int rowsize, 
	uint8_t red, uint8_t green, uint8_t blue)
{
	unsigned int i;
	unsigned int size;
	int y0, y1;
	uint8_t n;

	symbol_initialize(pfw->p);

	pfw->delay = rand() & 0xff;

	pfw->t = rand() & 0xff;

	pfw->red = red;
	pfw->green = green;
	pfw->blue = blue;

	pfw->d = 0;

	if (x & 1)
	{
		pfw->d |= 1;
	}

	n = rand() % 20;
	n += 60;

	pfw->n = n;
	pfw->i = n;

	float a, b, c;
	a = ((rand() & 0x0f) + 19) / 100.0;
	b = 1.0 + (rand() & 0x0f) / 100.0;
	c = 1.0 + (rand() & 0x0f) / 100.0;
	y1 = (n * n) * a + n * b + c;
	y0 = c;

	pfw->a = a;
	pfw->b = b;
	pfw->c = c;

	pfw->ox = x;
	if (pfw->d & 1)
	{
		pfw->ox -= n;
	}
	else
	{
		pfw->ox += n;
	}
	pfw->oy = y - y0;

	pfw->x = pfw->ox;
	pfw->y = pfw->oy + y1;

	symbol_initialize(pfw->p, x, y,
		pbits, width, height, rowsize);
}
void word_uninitialize(struct firework_word* pfw)
{
	symbol_uninitialize(pfw->p);
}

int word_run(struct firework_word* pfw, HDC hdc)
{
	int x, y;
	int result = 0;
	uint8_t i;

	if (pfw->i)
	{
		pfw->i--;

		i = pfw->i;

		y = (i * i) * pfw->a + i * pfw->b + pfw->c;

		if (pfw->d & 1)
		{
			x = pfw->ox + i;
		}
		else
		{
			x = pfw->ox - i;
		}
		y += pfw->oy;

		HGDIOBJ hobj0;
		HGDIOBJ hobj1;
		HPEN hpen;
		HBRUSH hbrush;
		uint8_t red, green, blue;

		red = pfw->red;
		green = pfw->green;
		blue = pfw->blue;
		hpen = CreatePen(PS_SOLID, 1, RGB(red, green, blue));
		hbrush = CreateHatchBrush(0, RGB(red, green, blue));
		hobj0 = SelectObject(hdc, hpen);
		hobj1 = SelectObject(hdc, hbrush);
		Ellipse(hdc, x - 2, y - 2, x + 2, y + 2);
		SelectObject(hdc, hobj0);
		SelectObject(hdc, hobj1);
		DeleteObject(hpen);
		DeleteObject(hbrush);

		pfw->x = x;
		pfw->y = y;
	}
	else
	{
		result = symbol_run(pfw->p, pfw->x, pfw->y, pfw->red, pfw->green, pfw->blue, hdc);
	}
	return(result);
}
//---------------------------------------------------------------------------