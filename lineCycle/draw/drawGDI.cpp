#include "../stdafx.h"

namespace drawGDI
{
	HDC hdc;
	HPEN linePen, circlePen, polygonPen;

	HANDLE * events;

	//直线绘制工作线程（弃用：多线程绘制直线的时候会有问题）
	DWORD WINAPI drawLineWorker(LPVOID lpParam);
	//园绘制工作线程
	DWORD WINAPI drawCircleWorker(LPVOID lpParam);


	DWORD WINAPI drawLineWorker(LPVOID lpParam)
	{
		int workerID = (int)lpParam;
		int workerInterval = lineList.size() / SETTING_DRAW_THREAD;

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

	DWORD WINAPI drawCircleWorker(LPVOID lpParam)
	{
		int workerID = (int)lpParam;
		int workerInterval = circleList.size() / SETTING_DRAW_THREAD;


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

}



void onGDIDraw()
{
	using namespace drawGDI;

	logMsg(L"GDI重绘开始");

	hdc = theDC;
	events = new HANDLE[SETTING_DRAW_THREAD];
	for (int i = 0; i < SETTING_DRAW_THREAD; i++)
		events[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

	linePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	circlePen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	polygonPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));

	logMsg(L"开始绘制直线");
	//单线程绘制直线
	SelectObject(hdc, linePen);
	for (vector <Line *> ::iterator it = lineList.begin(); it != lineList.end(); ++it)
	{
		MoveToEx(hdc, (int)(*it)->x1, (int)(*it)->y1, NULL);
		LineTo(hdc, (int)(*it)->x2, (int)(*it)->y2);
	}

	logMsg(L"直线绘制结束，开始绘制圆");
	//多线程画圆
	SelectObject(hdc, circlePen);
	for (int i = 0; i < SETTING_DRAW_THREAD; i++)
	{
		HANDLE hThread;

		hThread = CreateThread(NULL, 0, drawCircleWorker, (LPVOID)i, 0, NULL);
		if (hThread == NULL)
		{
			MessageBox(theHWND, L"线程创建失败", L"错误", 0);
			PostMessage(theHWND, WM_DESTROY, 0, 0);
		}
		CloseHandle(hThread);
	}
	WaitForMultipleObjects(SETTING_DRAW_THREAD, events, true, INFINITE);

	logMsg(L"圆绘制结束，开始绘制边界多边形");
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
	for (int i = 0; i < SETTING_DRAW_THREAD; i++)
		CloseHandle(events[i]);
	logMsg(L"绘制结束");

	delete[] events;
}
