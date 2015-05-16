#include "../stdafx.h"

namespace memDraw
{
	const int inf = 99999999;
	const double eps = 10e-6;
	int gra[768][1366];

	HDC notCuttingDC = NULL, cuttingDC = NULL;
	HBITMAP notCuttingBmp = NULL,cuttingBmp = NULL;
	bool isCuttingDCDrawed = false, isNotCuttingDCDrawed = false;

	HANDLE * events;

	void drawNotCuttingDC();
	void drawCuttingDC();
	void DrawLine(int x1, int y1, int x2, int y2, int rgb);
	DWORD WINAPI drawMemLineWorker(LPVOID lpParam);
	DWORD WINAPI drawMemCircleWorker(LPVOID lpParam);

	void DrawLine(int x1, int y1, int x2, int y2, int rgb)
	{
		double k;
		if (x2 == x1) k = inf;
		else k = (y2 - y1) * 1.0 / (x2 - x1);

		if (x1 > x2 || (x1 == x2 && y1 > y2))
		{
			int t = x1; x1 = x2; x2 = t;
			t = y1; y1 = y2; y2 = t;
		}

		int x, y;
		for (x = x1; x < x2; x++)
		{
			int yy = k * (x - x1) + y1;
			int yyy = k * (x - x1 + 1) + y1;
			if (yy > yyy)
			{
				int t = yy; yy = yyy; yyy = t;
			}

			for (y = yy; y <= yyy; y++)
				gra[y][x] = rgb;
		}
	}

	void DrawCircle(int x, int y, int r, int rgb)
	{
		int k;  //分几段

		if (r < 10) k = 16;
		else if (r < 20) k = 32;
		else if (r < 40) k = 64;
		else k = 128;

		double dlt = 2 * 3.1415926 / k;
		double alp = 0;
		int x1, y1, x2, y2;
		for (int i = 0; i < k; i++)
		{
			int dltx = r * _sinTable[(int)(alp * 10000)];
			int dlty = r * _cosTable[(int)(alp * 10000)];
			x1 = x + dltx;
			y1 = y + dlty;
			alp += dlt;
			dltx = r * _sinTable[(int)(alp * 10000)];
			dlty = r * _cosTable[(int)(alp * 10000)];
			x2 = x + dltx;
			y2 = y + dlty;

			DrawLine(x1, y1, x2, y2, rgb);
		}
	}

	void DrawArc(int x, int y, int r, int rgb, double alp1, double alp2)
	{
		if (alp1 > alp2)
		{
			double t = alp1; alp1 = alp2; alp2 = t;
		}
		int k = 0;
		double dlt1 = alp2 - alp1;
		int m = 6.30 / dlt1 + 1;

		if (r < 10) k = 4 / m + 1;
		else if (r < 200) k = 8 / m + 1;
		else k = 16 / m + 1;

		double dlt = dlt1 / k;
		double alp = alp1;
		int x1, y1, x2, y2;
		for (int i = 0; i < k; i++)
		{
			int dltx = r * _sinTable[(int)(alp * 10000)];
			int dlty = r * _cosTable[(int)(alp * 10000)];
			x1 = x + dltx;
			y1 = y + dlty;
			alp += dlt;
			dltx = r * _sinTable[(int)(alp * 10000)];
			dlty = r * _cosTable[(int)(alp * 10000)];
			x2 = x + dltx;
			y2 = y + dlty;
			DrawLine(x1, y1, x2, y2, rgb);
		}
	}

	DWORD WINAPI drawMemLineWorker(LPVOID lpParam)
	{
		int workerID = (int)lpParam;
		int workerInterval = lineList.size() / SETTING_DRAW_THREAD;

		if (workerID == 0)
		{
			for (unsigned int i = 0; i < workerInterval + lineList.size() % SETTING_DRAW_THREAD; i++)
			{
				Line * l = lineList.at(i);
				DrawLine(l->x1, l->y1, l->x2, l->y2, RGB(255, 0, 0));
			}
		}
		else
		{
			for (unsigned int i = workerInterval*workerID + lineList.size() % SETTING_DRAW_THREAD; i < workerInterval*(workerID + 1) + lineList.size() % SETTING_DRAW_THREAD; i++)
			{
				Line * l = lineList.at(i);
				DrawLine(l->x1, l->y1, l->x2, l->y2, RGB(255, 0, 0));
			}
		}

		SetEvent(events[workerID]);
		return 0;
	}

	DWORD WINAPI drawMemCircleWorker(LPVOID lpParam)
	{
		int workerID = (int)lpParam;
		int workerInterval = circleList.size() / SETTING_DRAW_THREAD;


		if (workerID == 0)
		{
			for (unsigned int i = 0; i < workerInterval + circleList.size() % SETTING_DRAW_THREAD; i++)
			{
				Circle * l = circleList.at(i);
				DrawCircle((int)l->x, (int)l->y, (int)l->r, RGB(0, 255, 0));
			}
		}
		else
		{
			for (unsigned int i = workerInterval*workerID + circleList.size() % SETTING_DRAW_THREAD; i < workerInterval*(workerID + 1) + circleList.size() % SETTING_DRAW_THREAD; i++)
			{
				Circle * l = circleList.at(i);
				DrawCircle((int)l->x, (int)l->y, (int)l->r, RGB(0, 255, 0));
			}
		}

		SetEvent(events[workerID]);
		return 0;
	}

	void drawNotCuttingDC()
	{
		events = new HANDLE[SETTING_DRAW_THREAD];
		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
			events[i] = CreateEvent(NULL, FALSE, FALSE, NULL);


		//多线程绘制直线
		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
		{
			HANDLE hThread;

			hThread = CreateThread(NULL, 0, drawMemLineWorker, (LPVOID)i, 0, NULL);
			if (hThread == NULL)
			{
				MessageBox(theHWND, L"线程创建失败", L"错误", 0);
				PostMessage(theHWND, WM_DESTROY, 0, 0);
			}

			CloseHandle(hThread);
		}

		WaitForMultipleObjects(SETTING_DRAW_THREAD, events, true, INFINITE);

		//多线程画圆
		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
		{
			HANDLE hThread;

			hThread = CreateThread(NULL, 0, drawMemCircleWorker, (LPVOID)i, 0, NULL);
			if (hThread == NULL)
			{
				MessageBox(theHWND, L"线程创建失败", L"错误", 0);
				PostMessage(theHWND, WM_DESTROY, 0, 0);
			}
			CloseHandle(hThread);
		}

		WaitForMultipleObjects(SETTING_DRAW_THREAD, events, true, INFINITE);

		//设置内存BMP
		SetBitmapBits(notCuttingBmp, sizeof(gra), gra);

		//单线程绘制裁剪多边形
		HPEN polygonPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		SelectObject(notCuttingDC, polygonPen);
		int last_x = -1, last_y = -1;
		for (vector <Point *> ::iterator it = polygonList.begin(); it != polygonList.end(); ++it)
		{
			if (last_x == -1 && last_y == -1)
			{
				last_x = (int)(*it)->x;
				last_y = (int)(*it)->y;
				continue;
			}
			else
			{
				MoveToEx(notCuttingDC, last_x, last_y, NULL);
				LineTo(notCuttingDC, (int)(*it)->x, (int)(*it)->y);
				last_x = (int)(*it)->x;
				last_y = (int)(*it)->y;
			}
		}
		if (last_x != -1 && last_y != -1)
		{
			MoveToEx(notCuttingDC, last_x, last_y, NULL);

			LineTo(notCuttingDC, (int)(*polygonList.begin())->x, (int)(*polygonList.begin())->y);
		}

		DeleteObject(polygonPen);
		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
			CloseHandle(events[i]);

		delete[] events;
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