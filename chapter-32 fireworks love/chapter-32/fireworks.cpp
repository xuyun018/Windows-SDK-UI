#include "fireworks.h"
//---------------------------------------------------------------------------
void spark_initialize(struct spark* p)
{
	p->x = NULL;
	p->y = NULL;
}
void spark_initialize(struct spark* p, uint8_t n, int16_t x, int16_t y)
{
	unsigned int size;
	uint8_t m;
	uint8_t i;
	uint8_t j;
	uint8_t s;

	s = 3;
	s += rand() & 1;
	p->v = n << s;

	m = rand() & 7;
	m += 7;
	p->m = m;
	p->n = 360 / m;
	p->n++;
	p->t = n >> (s == 4);
	p->i = 0;
	size = p->n * sizeof(int16_t *);
	p->x = (int16_t**)MALLOC(size);
	p->y = (int16_t**)MALLOC(size);

	for (i = 0; i < p->n; i++)
	{
		p->x[i] = (int16_t*)MALLOC(p->t * sizeof(int16_t));
		p->y[i] = (int16_t*)MALLOC(p->t * sizeof(int16_t));
		for (j = 0; j < p->t; j++)
		{
			p->x[i][j] = x;
			p->y[i][j] = y;
		}
	}
}
void spark_uninitialize(struct spark* p)
{
	uint8_t i;

	if (p->x)
	{
		for (i = 0; i < p->n; i++)
		{
			FREE(p->x[i]);
		}
		FREE(p->x);
	}
	if (p->y)
	{
		for (i = 0; i < p->n; i++)
		{
			FREE(p->y[i]);
		}
		FREE(p->y);
	}
}

int spark_run(struct spark* p, int ox, int oy, uint8_t red, uint8_t green, uint8_t blue,
	HDC hdc)
{
	int x, y;
	unsigned int k;
	int v0, v1;
	int result = 0;
	uint8_t t;
	uint8_t i;
	uint8_t j;

	if (p->i < p->t)
	{
		p->i++;

		t = p->i;

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

		v0 = p->v * t;
		v1 = t * t;
		v1 >>= 1;

		float _g;
		float _a;

		_a = PI / 180.0;
		_g = v1 * G;

		i = 0;
		for (k = 0; k < 360; k += p->m)
		{
			float a = k * _a;

			x = v0 * cos(a);
			y = v0 * sin(a) - _g;

			x >>= 9;
			y >>= 9;

			x += ox;
			y = oy - y;

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

			for (j = 1; j < t; j++)
			{
				r = red;
				g = green;
				b = blue;
				if (r > j)
				{
					r -= j;
				}
				else
				{
					r = 0;
				}
				if (g > j)
				{
					g -= j;
				}
				else
				{
					g = 0;
				}
				if (b > j)
				{
					b -= j;
				}
				else
				{
					b = 0;
				}
				SetPixel(hdc, p->x[i][j], p->y[i][j], RGB(r, g, b));
			}

			p->x[i][0] = x;
			p->y[i][0] = y;
			for (j = t; j > 1; j--)
			{
				p->x[i][j - 1] = p->x[i][j - 2];
				p->y[i][j - 1] = p->y[i][j - 2];
			}

			i++;
		}

		SelectObject(hdc, hobj0);
		SelectObject(hdc, hobj1);
		DeleteObject(hpen);
		DeleteObject(hbrush);
	}
	else
	{
		if (t = p->t)
		{
			p->t--;

			HGDIOBJ hobj0;
			HGDIOBJ hobj1;
			HPEN hpen;
			HBRUSH hbrush;
			uint8_t r, g, b;

			r = red;
			g = green;
			b = blue;
			r /= p->i - t + 1;
			g /= p->i - t + 1;
			b /= p->i - t + 1;
			hpen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
			hbrush = CreateHatchBrush(0, RGB(r, g, b));
			hobj0 = SelectObject(hdc, hpen);
			hobj1 = SelectObject(hdc, hbrush);

			for (i = 0; i < p->n; i++)
			{
				Rectangle(hdc, p->x[i][0] - 1, p->y[i][0] - 1, p->x[i][0] + 1, p->y[i][0] + 1);

				for (j = 1; j < t; j++)
				{
					r = red;
					g = green;
					b = blue;
					r = (j + 1) >> 1;
					g = (j + 1) >> 1;
					b = (j + 1) >> 1;
					SetPixel(hdc, p->x[i][j], p->y[i][j], RGB(r, g, b));
				}
			}

			SelectObject(hdc, hobj0);
			SelectObject(hdc, hobj1);
			DeleteObject(hpen);
			DeleteObject(hbrush);
		}
		else
		{
			result = 1;
		}
	}
	return(result);
}

void firework_initialize(struct firework* pfw)
{
	spark_initialize(pfw->p);
}
void firework_initialize(struct firework* pfw, int width, int height, int top, int bottom)
{
	unsigned int i;
	unsigned int size;
	int x, y;
	int y0, y1;
	uint8_t n;

	spark_initialize(pfw->p);

	pfw->delay = rand() & 0xff;

	pfw->t = rand() & 0xff;

	pfw->red = rand() & 0xff;
	pfw->green = rand() & 0xff;
	pfw->blue = rand() & 0xff;

	pfw->d = 0;

	x = rand() % width;
	if (x & 1)
	{
		pfw->d |= 1;
	}

	n = rand() % 20;
	n += 60;

	pfw->n = n;
	pfw->i = n;

	y = rand() % (bottom - top);
	y += top;

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

	for (i = 0; i < sizeof(pfw->x) / sizeof(pfw->x[0]); i++)
	{
		pfw->x[i] = pfw->ox;
		pfw->y[i] = pfw->oy + y1;
	}
}
void firework_uninitialize(struct firework* pfw)
{
	spark_uninitialize(pfw->p);
}

int firework_run(struct firework* pfw, HDC hdc)
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

		for (i = 0; i < sizeof(pfw->x) / sizeof(pfw->x[0]); i++)
		{
			red = pfw->red >> ((i + 1) << 1);
			green = pfw->red >> ((i + 1) << 1);
			blue = pfw->red >> ((i + 1) << 1);
			hpen = CreatePen(PS_SOLID, 1, RGB(red, green, blue));
			hbrush = CreateHatchBrush(0, RGB(red, green, blue));
			hobj0 = SelectObject(hdc, hpen);
			hobj1 = SelectObject(hdc, hbrush);
			Ellipse(hdc, pfw->x[i] - 2, pfw->y[i] - 2, pfw->x[i] + 2, pfw->y[i] + 2);
			SelectObject(hdc, hobj0);
			SelectObject(hdc, hobj1);
			DeleteObject(hpen);
			DeleteObject(hbrush);
		}

		pfw->x[0] = x;
		pfw->y[0] = y;
		for (i = sizeof(pfw->x) / sizeof(pfw->x[0]); i > 1; i--)
		{
			pfw->x[i - 1] = pfw->x[i - 2];
			pfw->y[i - 1] = pfw->y[i - 2];
		}

		if (pfw->i == 0)
		{
			spark_initialize(pfw->p, pfw->n, x, y);
		}
	}
	else
	{
		result = spark_run(pfw->p, pfw->x[0], pfw->y[0], pfw->red, pfw->green, pfw->blue, hdc);
	}
	return(result);
}
//---------------------------------------------------------------------------