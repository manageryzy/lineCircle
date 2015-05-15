#include "../stdafx.h"

namespace memDraw
{
	const int inf = 99999999;
	const double eps = 10e-6;
	int gra[768][1366];

	HDC notCuttingDC = NULL, cuttingDC = NULL;
	HBITMAP notCuttingBmp = NULL,cuttingBmp = NULL;
	bool isCuttingDCDrawed = false, isNotCuttingDCDrawed = false;

	HANDLE events;
	int workerCount = 0, workerFinished = 0;

	void drawNotCuttingDC();
	void drawCuttingDC();
	void DrawLine(int x1, int y1, int x2, int y2, int rgb);
	DWORD WINAPI drawMemLineWorker(LPVOID lpParam);
	DWORD WINAPI drawMemCircleWorker(LPVOID lpParam);

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

	DWORD WINAPI drawMemLineWorker(LPVOID lpParam)
	{
		++workerCount;

		int workerID = (int)lpParam;
		int workerInterval = lineList.size() / SETTING_DRAW_THREAD;

		if (workerID == 0)
		{
			for (unsigned int i = 0; i < workerInterval + lineList.size() % SETTING_DRAW_THREAD; i++)
			{
				Line l = lineList.at(i);
				DrawLine(l.x1, l.y1, l.x2, l.y2, RGB(255, 0, 0));
			}
		}
		else
		{
			for (unsigned int i = workerInterval*workerID + lineList.size() % SETTING_DRAW_THREAD; i < workerInterval*(workerID + 1) + lineList.size() % SETTING_DRAW_THREAD; i++)
			{
				Line l = lineList.at(i);
				DrawLine(l.x1, l.y1, l.x2, l.y2, RGB(255, 0, 0));
			}
		}

		++workerFinished;

		if (workerFinished >= SETTING_DRAW_THREAD)
		{
			SetEvent(events);
		}

		--workerCount;
		return 0;
	}

	DWORD WINAPI drawMemCircleWorker(LPVOID lpParam)
	{
		++workerCount;
		int workerID = (int)lpParam;
		int workerInterval = circleList.size() / SETTING_DRAW_THREAD;
		/*HDC hdc = hdcList.at(workerID);

		SelectObject(hdc, circlePen);

		if (workerID == 0)
		{
			for (unsigned int i = 0; i < workerInterval + circleList.size() % SETTING_DRAW_THREAD; i++)
			{
				Circle l = circleList.at(i);
				Arc(hdc, (int)(l.x - l.r), (int)(l.y - l.r), (int)(l.x + l.r), (int)(l.y + l.r), 0, 0, 0, 0);
			}
		}
		else
		{
			for (unsigned int i = workerInterval*workerID + circleList.size() % SETTING_DRAW_THREAD; i < workerInterval*(workerID + 1) + circleList.size() % SETTING_DRAW_THREAD; i++)
			{
				Circle l = circleList.at(i);
				Arc(hdc, (int)(l.x - l.r), (int)(l.y - l.r), (int)(l.x + l.r), (int)(l.y + l.r), 0, 0, 0, 0);
			}
		}*/

		++workerFinished;

		if (workerFinished >= SETTING_DRAW_THREAD)
		{
			SetEvent(events);
		}

		--workerCount;
		return 0;
	}

	void drawNotCuttingDC()
	{
		events = CreateEvent(NULL, FALSE, FALSE, NULL);

		//等待所有的工作任务都退出了。虽然主线程应该是线程安全的，谁知道呢，呵呵
		while (1)
		{
			if (workerCount < 1)
				break;

			Sleep(1);
		}
		workerFinished = 0;

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

		WaitForSingleObject(events, INFINITE);
		while (1)
		{
			if (workerCount < 1)
				break;

			Sleep(1);
		}
		workerFinished = 0;

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

		WaitForSingleObject(events, INFINITE);
		while (1)
		{
			if (workerCount < 1)
				break;

			Sleep(1);
		}
		workerFinished = 0;

		//设置内存BMP
		SetBitmapBits(notCuttingBmp, sizeof(gra), gra);

		//单线程绘制裁剪多边形
		HPEN polygonPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		SelectObject(notCuttingDC, polygonPen);
		int last_x = -1, last_y = -1;
		for (vector <Point> ::iterator it = polygonList.begin(); it != polygonList.end(); ++it)
		{
			if (last_x == -1 && last_y == -1)
			{
				last_x = (int)it->x;
				last_y = (int)it->y;
				continue;
			}
			else
			{
				MoveToEx(notCuttingDC, last_x, last_y, NULL);
				LineTo(notCuttingDC, (int)it->x, (int)it->y);
				last_x = (int)it->x;
				last_y = (int)it->y;
			}
		}
		if (last_x != -1 && last_y != -1)
		{
			MoveToEx(notCuttingDC, last_x, last_y, NULL);

			LineTo(notCuttingDC, (int)polygonList.begin()->x, (int)polygonList.begin()->y);
		}

		DeleteObject(polygonPen);
		CloseHandle(events);
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