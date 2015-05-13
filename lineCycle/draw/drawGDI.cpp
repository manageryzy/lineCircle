#include "../stdafx.h"

HDC hdc;
HPEN linePen,circlePen,polygonPen;

HANDLE events;
int workerCount = 0;

//ֱ�߻��ƹ����̣߳����ã����̻߳���ֱ�ߵ�ʱ��������⣩
DWORD WINAPI drawLineWorker(LPVOID lpParam);
//԰���ƹ����߳�
DWORD WINAPI drawCircleWorker(LPVOID lpParam);

void onGDIDraw()
{
	hdc = theDC;
	events = CreateEvent(NULL, FALSE, FALSE, NULL);

	linePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	circlePen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	polygonPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));

	//�ȴ����еĹ��������˳��ˡ���Ȼ���߳�Ӧ�����̰߳�ȫ�ģ�˭֪���أ��Ǻ�
	while (1)
	{
		if (workerCount < 1)
			break;

		Sleep(1);
	}

	//���̻߳���ֱ�߱�����
	/*for (int i = 0; i < SETTING_DRAW_THREAD; i++)
	{
		HANDLE hThread;
		
		hThread = CreateThread(NULL, 0, drawLineWorker, (LPVOID)i, 0, NULL);
		if (hThread == NULL)
		{
			MessageBox(theHWND, L"�̴߳���ʧ��", L"����", 0);
			PostMessage(theHWND, WM_DESTROY, 0, 0);
		}
		CloseHandle(hThread);
	}
	WaitForSingleObject(events, INFINITE);*/

	//���̻߳���ֱ��
	SelectObject(hdc, linePen);
	for (vector <Line> ::iterator it = lineList.begin(); it != lineList.end(); ++it)
	{
		MoveToEx(hdc, it->x1, it->y1, NULL);
		LineTo(hdc, it->x2, it->y2);
	}

	//���̻߳�Բ
	SelectObject(hdc, circlePen);
	for (int i = 0; i < SETTING_DRAW_THREAD; i++)
	{
		HANDLE hThread;

		hThread = CreateThread(NULL, 0, drawCircleWorker, (LPVOID)i, 0, NULL);
		if (hThread == NULL)
		{
			MessageBox(theHWND, L"�̴߳���ʧ��", L"����", 0);
			PostMessage(theHWND, WM_DESTROY, 0, 0);
		}
		CloseHandle(hThread);
	}

	WaitForSingleObject(events, INFINITE);

	
	//���̻߳��Ʋü������
	SelectObject(hdc, polygonPen);
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

	if (workerCount < 1 )
		SetEvent(events);

	return 0;
}

DWORD WINAPI drawCircleWorker(LPVOID lpParam)
{
	int workerID = (int)lpParam;
	int workerInterval = circleList.size() / SETTING_DRAW_THREAD;

	++workerCount;

	if (workerID == 0)
	{
		for (int i = 0; i < workerInterval + circleList.size() % SETTING_DRAW_THREAD; i++)
		{
			Circle l = circleList.at(i);
			Arc(hdc, l.x - l.r, l.y - l.r, l.x + l.r, l.y + l.r, 0, 0, 0, 0);
		}
	}
	else
	{
		for (int i = workerInterval*workerID + circleList.size() % SETTING_DRAW_THREAD; i < workerInterval*(workerID + 1) + circleList.size() % SETTING_DRAW_THREAD; i++)
		{
			Circle l = circleList.at(i);
			Arc(hdc, l.x - l.r, l.y - l.r, l.x + l.r, l.y + l.r, 0, 0, 0, 0);
		}
	}

	--workerCount;

	if (workerCount < 1)
		SetEvent(events);

	return 0;
}
