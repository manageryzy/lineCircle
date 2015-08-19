#include "../stdafx.h"

namespace drawMemoryGDI{

	bool isCuttingDCDrawed = false, isNotCuttingDCDrawed = false;

	vector<HDC> hdcList;
	vector<HBITMAP> bmpList;
	HDC cuttingDC = NULL, notCuttingDC = NULL;
	HBITMAP cuttingBmp = NULL, notCuttingBmp = NULL;
	HPEN linePen, circlePen, polygonPen;

	HANDLE * events;

	DWORD WINAPI drawMemLineWorker(LPVOID lpParam);
	DWORD WINAPI drawMemCircleWorker(LPVOID lpParam);
	DWORD WINAPI drawMemLineCutWorker(LPVOID lpParam);
	DWORD WINAPI drawMemCircleCutWorker(LPVOID lpParam);

	void drawNotCuttingDC();
	void drawCuttingDC();

	void drawNotCuttingDC()
	{
		HDC hdc = notCuttingDC;

		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 255, 255));

		events = new HANDLE[SETTING_DRAW_THREAD];
		for (int i = 0; i < SETTING_DRAW_THREAD;i++)
			events[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

		linePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		circlePen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		polygonPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));

		//初始化资源
		hdcList.clear();
		bmpList.clear();
		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
		{
			hdcList.push_back(CreateCompatibleDC(NULL));
			bmpList.push_back(CreateCompatibleBitmap(theDC, 1440, 900));
			SelectObject(hdcList.at(i), bmpList.at(i));
			
		}

		logMsg(L"开始绘制直线");
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

		logMsg(L"直线绘制结束，开始绘制圆形");
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


		logMsg(L"绘制结束，开始合并结果");
		//合并结果
		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
		{
			BitBlt(hdc, 0, 0, 1440, 900, hdcList.at(i), 0, 0, SRCPAINT);

			ReleaseDC(theHWND, hdcList.at(i));
			DeleteObject(bmpList.at(i));
		}
		hdcList.clear();
		bmpList.clear();

		logMsg(L"合并结束，开始绘制多边形");
		//单线程绘制裁剪多边形
		SelectObject(hdc, polygonPen);
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
				MoveToEx(hdc, last_x, last_y, NULL);
				LineTo(hdc, (int)(*it)->x, (int)(*it)->y);
				last_x = (int)(*it)->x;
				last_y = (int)(*it)->y;
			}
		}
		if (last_x != -1 && last_y != -1)
		{
			MoveToEx(hdc, last_x, last_y, NULL);

			LineTo(hdc, (int)(*polygonList.begin())->x, (int)(*polygonList.begin())->y);
		}

		DeleteObject(linePen);
		DeleteObject(circlePen);
		DeleteObject(polygonPen);
		logMsg(L"多边形绘制结束");

		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
			CloseHandle(events[i]);

		delete[] events;
	}

	void drawCuttingDC()
	{
		HDC hdc = cuttingDC;

		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 255, 255));

		events = new HANDLE[SETTING_DRAW_THREAD];
		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
			events[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

		linePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		circlePen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		polygonPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));

		//初始化资源
		hdcList.clear();
		bmpList.clear();
		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
		{
			hdcList.push_back(CreateCompatibleDC(NULL));
			bmpList.push_back(CreateCompatibleBitmap(theDC, 1440, 900));
			SelectObject(hdcList.at(i), bmpList.at(i));

		}

		logMsg(L"开始绘制线段");

		//多线程绘制直线
		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
		{
			HANDLE hThread;

			hThread = CreateThread(NULL, 0, drawMemLineCutWorker, (LPVOID)i, 0, NULL);
			if (hThread == NULL)
			{
				MessageBox(theHWND, L"线程创建失败", L"错误", 0);
				PostMessage(theHWND, WM_DESTROY, 0, 0);
			}

			CloseHandle(hThread);
		}
		WaitForMultipleObjects(SETTING_DRAW_THREAD, events, true, INFINITE);

		logMsg(L"直线绘制结束，开始绘制弧形");
		//多线程画弧
		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
		{
			HANDLE hThread;

			hThread = CreateThread(NULL, 0, drawMemCircleCutWorker, (LPVOID)i, 0, NULL);
			if (hThread == NULL)
			{
				MessageBox(theHWND, L"线程创建失败", L"错误", 0);
				PostMessage(theHWND, WM_DESTROY, 0, 0);
			}
			CloseHandle(hThread);
		}
		WaitForMultipleObjects(SETTING_DRAW_THREAD, events, true, INFINITE);


		logMsg(L"绘制结束，开始合并结果");
		//合并结果
		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
		{
			BitBlt(hdc, 0, 0, 1440, 900, hdcList.at(i), 0, 0, SRCPAINT);

			ReleaseDC(theHWND, hdcList.at(i));
			DeleteObject(bmpList.at(i));
		}
		hdcList.clear();
		bmpList.clear();
		logMsg(L"合并完成，开始绘制多边形");

		//单线程绘制裁剪多边形
		SelectObject(hdc, polygonPen);
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
				MoveToEx(hdc, last_x, last_y, NULL);
				LineTo(hdc, (int)(*it)->x, (int)(*it)->y);
				last_x = (int)(*it)->x;
				last_y = (int)(*it)->y;
			}
		}
		if (last_x != -1 && last_y != -1)
		{
			MoveToEx(hdc, last_x, last_y, NULL);

			LineTo(hdc, (int)(*polygonList.begin())->x, (int)(*polygonList.begin())->y);
		}

		DeleteObject(linePen);
		DeleteObject(circlePen);
		DeleteObject(polygonPen);

		logMsg(L"多边形绘制完成");

		for (int i = 0; i < SETTING_DRAW_THREAD; i++)
			CloseHandle(events[i]);

		delete[] events;
	}

	DWORD WINAPI drawMemLineWorker(LPVOID lpParam)
	{

		int workerID = (int)lpParam;
		int workerInterval = lineList.size() / SETTING_DRAW_THREAD;
		HDC hdc = hdcList.at(workerID);


		SelectObject(hdc, linePen);

		if (workerID == 0)
		{
			for (unsigned int i = 0; i < workerInterval + lineList.size() % SETTING_DRAW_THREAD; i++)
			{
				Line * l = lineList.at(i);
				MoveToEx(hdc, (int)l->x1, (int)l->y1, NULL);
				LineTo(hdc, (int)l->x2, (int)l->y2);
			}
		}
		else
		{
			for (unsigned int i = workerInterval*workerID + lineList.size() % SETTING_DRAW_THREAD; i < workerInterval*(workerID + 1) + lineList.size() % SETTING_DRAW_THREAD; i++)
			{
				Line * l = lineList.at(i);
				MoveToEx(hdc, (int)l->x1, (int)l->y1, NULL);
				LineTo(hdc, (int)l->x2, (int)l->y2);
			}
		}

		SetEvent(events[workerID]);

		return 0;
	}


	DWORD WINAPI drawMemLineCutWorker(LPVOID lpParam)
	{

		unsigned workerID = (unsigned int)lpParam;
		unsigned size = cutLineList.size();
		unsigned int workerInterval = size / SETTING_DRAW_THREAD;
		HDC hdc = hdcList.at(workerID);


		SelectObject(hdc, linePen);

		if (workerID == 0)
		{
			for (unsigned int i = 0; i < workerInterval + size % SETTING_DRAW_THREAD; i++)
			{
				Line * l = cutLineList.at(i);
				MoveToEx(hdc, (int)l->x1, (int)l->y1, NULL);
				LineTo(hdc, (int)l->x2, (int)l->y2);
			}
		}
		else
		{
			for (unsigned int i = workerInterval*workerID + size % SETTING_DRAW_THREAD; i < workerInterval*(workerID + 1) + size % SETTING_DRAW_THREAD; i++)
			{
				Line * l = cutLineList.at(i);
				MoveToEx(hdc, (int)l->x1, (int)l->y1, NULL);
				LineTo(hdc, (int)l->x2, (int)l->y2);
			}
		}

		SetEvent(events[workerID]);

		return 0;
	}

	DWORD WINAPI drawMemCircleWorker(LPVOID lpParam)
	{
		int workerID = (int)lpParam;
		int workerInterval = circleList.size() / SETTING_DRAW_THREAD;
		HDC hdc = hdcList.at(workerID);

		SelectObject(hdc, circlePen);

		if (workerID == 0)
		{
			for (unsigned int i = 0; i < workerInterval + circleList.size() % SETTING_DRAW_THREAD; i++)
			{
				Circle * l = circleList.at(i);
				Arc(hdc, (int)(l->x - l->r), (int)(l->y - l->r), (int)(l->x + l->r), (int)(l->y + l->r), 0, 0, 0, 0);
			}
		}
		else
		{
			for (unsigned int i = workerInterval*workerID + circleList.size() % SETTING_DRAW_THREAD; i < workerInterval*(workerID + 1) + circleList.size() % SETTING_DRAW_THREAD; i++)
			{
				Circle * l = circleList.at(i);
				Arc(hdc, (int)(l->x - l->r), (int)(l->y - l->r), (int)(l->x + l->r), (int)(l->y + l->r), 0, 0, 0, 0);
			}
		}

		SetEvent(events[workerID]);

		return 0;
	}

	DWORD WINAPI drawMemCircleCutWorker(LPVOID lpParam)
	{
		unsigned int workerID = (unsigned int)lpParam;
		unsigned size = cutArcList.size();
		unsigned int workerInterval = size / SETTING_DRAW_THREAD;
		HDC hdc = hdcList.at(workerID);

		SelectObject(hdc, circlePen);

		if (workerID == 0)
		{
			for (unsigned int i = 0; i < workerInterval + size % SETTING_DRAW_THREAD; i++)
			{
				CArc * l = cutArcList.at(i);
				Arc(hdc, (int)(l->x - l->r), (int)(l->y - l->r), (int)(l->x + l->r), (int)(l->y + l->r), (int)(_cosTable[((int)(l->end * 10000)) % 62852] * l->r + l->x), (int)(_sinTable[((int)(l->end * 10000)) % 62852] * l->r + l->y), (int)(_cosTable[((int)(l->begin * 10000)) % 62852] * l->r + l->x), (int)(_sinTable[((int)(l->begin * 10000)) % 62852] * l->r + l->y));
			}
		}
		else
		{
			for (unsigned int i = workerInterval*workerID + size % SETTING_DRAW_THREAD; i < workerInterval*(workerID + 1) + size % SETTING_DRAW_THREAD; i++)
			{
				CArc * l = cutArcList.at(i);
				Arc(hdc, (int)(l->x - l->r), (int)(l->y - l->r), (int)(l->x + l->r), (int)(l->y + l->r), (int)(_cosTable[((int)(l->end * 10000)) % 62852] * l->r + l->x), (int)(_sinTable[((int)(l->end * 10000)) % 62852] * l->r + l->y), (int)(_cosTable[((int)(l->begin * 10000)) % 62852] * l->r + l->x), (int)(_sinTable[((int)(l->begin * 10000)) % 62852] * l->r + l->y));
			}
		}

		SetEvent(events[workerID]);

		return 0;
	}
}

void clearMemGDICache()
{
	using namespace drawMemoryGDI;

	ReleaseDC(theHWND,cuttingDC);
	ReleaseDC(theHWND,notCuttingDC);
	DeleteObject(cuttingBmp);
	DeleteObject(notCuttingBmp);

	cuttingDC = NULL;
	notCuttingDC = NULL;
	isCuttingDCDrawed = false;
	isNotCuttingDCDrawed = false;
}

void onMemGDIDraw()
{
	using namespace drawMemoryGDI;

	if (cuttingDC == NULL)
	{
		cuttingDC = CreateCompatibleDC(NULL);
		cuttingBmp = CreateCompatibleBitmap(theDC, 1440, 900);
		SelectObject(cuttingDC, cuttingBmp);
	}
	if (notCuttingDC == NULL)
	{
		notCuttingDC = CreateCompatibleDC(NULL);
		notCuttingBmp = CreateCompatibleBitmap(theDC, 1440, 900);
		SelectObject(notCuttingDC, notCuttingBmp);
	}
	

	if (SETTING_DRAW_CUTTING)
	{
		if (!isCuttingDCDrawed)
		{
			isCuttingDCDrawed = true;
			drawCuttingDC();
		}
		
		BitBlt(theDC, 0, 0, 1440, 900, cuttingDC, 0, 0, SRCCOPY);
	}
	else
	{
		if (!isNotCuttingDCDrawed)
		{
			isNotCuttingDCDrawed = true;
			drawNotCuttingDC();
		}

		BitBlt(theDC, 0, 0, 1440, 900, notCuttingDC, 0, 0, SRCCOPY);
	}
}