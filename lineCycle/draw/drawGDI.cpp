#include "../stdafx.h"

namespace drawGDI
{
	HDC hdc;
	HPEN linePen, circlePen, polygonPen;


	DWORD WINAPI drawNotCutThread(LPVOID lpParam)
	{
		isDrawBusy = true;
		
		logMsg(L"GDI重绘开始");

		hdc = GetDC(theHWND);

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
		unsigned int size = circleList.size();
		for (unsigned int i =0; i < size; i++)
		{
			Circle * l = circleList.at(i);
			Arc(hdc, (int)(l->x - l->r), (int)(l->y - l->r), (int)(l->x + l->r), (int)(l->y + l->r), 0, 0, 0, 0);
		}

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

		ReleaseDC(theHWND, hdc);

		logMsg(L"绘制结束");

		isDrawBusy = false;

		return 0;
	}

	DWORD WINAPI drawCutThread(LPVOID lpParam)
	{
		if (!isCutted)
		{
			MessageBox(theHWND, L"裁剪数据没有就绪", L"警告", 0);
			return 0;
		}

		isDrawBusy = true;

		logMsg(L"GDI重绘开始");

		hdc = GetDC(theHWND);

		linePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		circlePen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		polygonPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));

		logMsg(L"开始绘制直线");
		//单线程绘制直线
		SelectObject(hdc, linePen);
		unsigned size = cutLineList.size();
		for (unsigned int i = 0; i < size; i++)
		{
			Line * l = cutLineList.at(i);
			MoveToEx(hdc, (int)l->x1, (int)l->y1, NULL);
			LineTo(hdc, (int)l->x2, (int)l->y2);
		}

		logMsg(L"直线绘制结束，开始绘制弧形");
		//单线程画弧
		SelectObject(hdc, circlePen);
		size = cutArcList.size();
		for (unsigned int i = 0; i < size; i++)
		{
			CArc * l = cutArcList.at(i);
			Arc(hdc, (int)(l->x - l->r), (int)(l->y - l->r), (int)(l->x + l->r), (int)(l->y + l->r), _cosTable[((int)(l->end * 10000)) % 62852] * l->r + l->x, _sinTable[((int)(l->end * 10000)) % 62852] * l->r + l->y, _cosTable[((int)(l->begin * 10000)) % 62852] * l->r + l->x, _sinTable[((int)(l->begin * 10000)) % 62852] * l->r + l->y);
		}

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

		ReleaseDC(theHWND, hdc);

		logMsg(L"绘制结束");


		isDrawBusy = false;

		return 0;
	}

}



void onGDIDraw()
{
	using namespace drawGDI;

	if (isDrawBusy)
		return;

	HANDLE hThread;

	if (SETTING_DRAW_CUTTING)
	{
		hThread = CreateThread(NULL, 0, drawCutThread, 0, 0, NULL);
	}
	else
	{
		hThread = CreateThread(NULL, 0, drawNotCutThread, 0, 0, NULL);
	}
	
	if (hThread == NULL)
	{
		MessageBox(theHWND, L"线程创建失败", L"错误", 0);
		PostMessage(theHWND, WM_DESTROY, 0, 0);
	}
	CloseHandle(hThread);
	
}
