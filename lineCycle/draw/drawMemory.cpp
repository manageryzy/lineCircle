#include "../stdafx.h"

namespace memDraw
{
	const int inf = 99999999;
	const double eps = 10e-6;
	int gra[768][1366];

	HDC notCuttingDC = NULL, cuttingDC = NULL;
	HBITMAP notCuttingBmp = NULL,cuttingBmp = NULL;
	bool isCuttingDCDrawed = false, isNotCuttingDCDrawed = false;

	void drawNotCuttingDC();
	void drawCuttingDC();
	void DrawLine(float x1, float y1, float x2, float y2, int rgb);


	void DrawLine(int x1, int y1, int x2, int y2, int rgb)
	{
		int k, b;
		if ((x2 - x1) < eps) k = inf;
		else k = (y2 - y1) / (x2 - x1);

		b = y1 - k * x1;

		int x, y;
		for (x = (int)x1; x <= (int)x2 + 1; x++)
		{
			int yy = k * (x - x1) + y1;
			if (yy < y1) yy = y1;

			int yyy = k * (x - x1 + 1) + y1;
			if (yyy > y2) yyy = y2;

			for (y = (int)yy; y <= (int)yyy; y++)
				gra[y][x] = rgb;
		}
	}

	void drawNotCuttingDC()
	{
		DrawLine(0, 0, 10, 50, RGB(0, 0, 255));

		SetBitmapBits(notCuttingBmp, sizeof(gra), gra);
	}

	void drawCuttingDC()
	{

		SetBitmapBits(cuttingBmp, sizeof(gra), gra);
	}
}


void onMemDraw()
{
	using namespace memDraw;

	if (cuttingDC == NULL)
	{
		cuttingDC = CreateCompatibleDC(NULL);
		cuttingBmp = CreateCompatibleBitmap(theDC, 1366, 768);
		SelectObject(cuttingDC, cuttingBmp);
	}
	if (notCuttingDC == NULL)
	{
		notCuttingDC = CreateCompatibleDC(theDC);
		notCuttingBmp = CreateCompatibleBitmap(theDC, 1366, 768);
		SelectObject(notCuttingDC, notCuttingBmp);
	}
	
	if (SETTING_DRAW_CUTTING)
	{
		if (!isCuttingDCDrawed)
		{
			isCuttingDCDrawed = true;
			drawCuttingDC();
		}

		BitBlt(theDC, 0, 0, 1366, 768, cuttingDC, 0, 0, SRCCOPY);
	}
	else
	{
		if (!isNotCuttingDCDrawed)
		{
			isNotCuttingDCDrawed = true;
			drawNotCuttingDC();
		}

		BitBlt(theDC, 0, 0, 1366, 768, notCuttingDC, 0, 0, SRCCOPY);
	}
}


void clearGDICache()
{
	using namespace memDraw;

	ReleaseDC(theHWND, cuttingDC);
	ReleaseDC(theHWND, notCuttingDC);
	DeleteObject(cuttingBmp);
	DeleteObject(notCuttingBmp);

	cuttingDC = NULL;
	notCuttingDC = NULL;
	isCuttingDCDrawed = false;
	isNotCuttingDCDrawed = false;
}