#include "../stdafx.h"

HDC hdc;

HANDLE events;
int workerCount = 0;

DWORD WINAPI drawLineWorker(LPVOID lpParam);
DWORD WINAPI drawCircleWorker(LPVOID lpParam);

void onGDIDraw()
{
	hdc = theDC;
	events = CreateEvent(NULL, FALSE, FALSE, NULL);

	for (int i = 0; i < SETTING_DRAW_THREAD; i++)
	{
		HANDLE hThread;
		
		hThread = CreateThread(NULL, 0, drawLineWorker, (LPVOID)i, 0, NULL);
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
	ResetEvent(events);

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

	WaitForSingleObject(events, INFINITE);
	while (1)
	{
		if (workerCount == 0)
			break;

		Sleep(1);
	}
	ResetEvent(events);
	

	float last_x = -1, last_y = -1;
	for (vector <Point> ::iterator it = polygonList.begin(); it != polygonList.end(); ++it)
	{
		if (last_x == -1 && last_y == -1)
		{
			last_x = it->x;
			last_y = it->y;
			continue;
		}
		else
		{
			MoveToEx(hdc, last_x, last_y, NULL);
			LineTo(hdc, it->x, it->y);
			last_x = it->x;
			last_y = it->y;
		}
	}
	if (last_x != -1 && last_y != -1)
	{
		MoveToEx(hdc, last_x, last_y, NULL);

		LineTo(hdc, polygonList.begin()->x, polygonList.begin()->y);
	}
	
	CloseHandle(events);
}

DWORD WINAPI drawLineWorker(LPVOID lpParam)
{
	int workerID = (int)lpParam;
	int workerInterval = lineList.size() / SETTING_DRAW_THREAD;

	++workerCount;
	
	SetEvent(events);

	if (workerID == 0)
	{
		for (int i = 0; i < workerInterval + lineList.size() % SETTING_DRAW_THREAD; i++)
		{
			Line l = lineList.at(i);
			MoveToEx(hdc, l.x1, l.y1, NULL);
			LineTo(hdc, l.x2, l.y2);
		}
	}
	else
	{
		for (int i = workerInterval*workerID + lineList.size() % SETTING_DRAW_THREAD; i < workerInterval*(workerID + 1) + lineList.size() % SETTING_DRAW_THREAD; i++)
		{
			Line l = lineList.at(i);
			MoveToEx(hdc, l.x1, l.y1, NULL);
			LineTo(hdc, l.x2, l.y2);
		}
	}
	
	--workerCount;

	return 0;
}

DWORD WINAPI drawCircleWorker(LPVOID lpParam)
{
	int workerID = (int)lpParam;
	int workerInterval = circleList.size() / SETTING_DRAW_THREAD;

	++workerCount;

	SetEvent(events);

	if (workerID == 0)
	{
		for (int i = 0; i < workerInterval + circleList.size() % SETTING_DRAW_THREAD; i++)
		{
			Circle l = circleList.at(i);
			Ellipse(hdc, l.x - l.r, l.y - l.r, l.x + l.r, l.y + l.r);
		}
	}
	else
	{
		for (int i = workerInterval*workerID + circleList.size() % SETTING_DRAW_THREAD; i < workerInterval*(workerID + 1) + circleList.size() % SETTING_DRAW_THREAD; i++)
		{
			Circle l = circleList.at(i);
			Ellipse(hdc, l.x - l.r, l.y - l.r, l.x + l.r, l.y + l.r);
		}
	}

	--workerCount;

	return 0;
}
