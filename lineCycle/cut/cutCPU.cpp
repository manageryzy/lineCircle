#include  "../stdafx.h"
#include "../resource.h"

namespace cpuCUT{

	__inline float mult(Point a, Point b, Point c);

	int gra[768][1366];

	HANDLE * events;

	const float eps = 1e-8;
	const float PI = 3.141592653;


	vector <Line *> * cutLineListList;
	vector <CArc *> * cutArcListList;


	inline float mult(Point a, Point b, Point c){
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	}

	bool cmpx(const Point* p1, const Point* p2)
	{
		return p1->x < p2->x;
	}

	bool cmpy(const Point* p1, const Point* p2)
	{
		return p1->y < p2->y;
	}


	//����Ч�ʵ��£��������ַ����ˣ�������������Ҳ��������Ϊһ�ֿ�ѡ������
	//int isPointIn(const Point &p)
	//{
	//	Point q;
	//	q.x = 1368;
	//	q.y = 768;//��pΪ���qΪ�յ�������L
	//	int counter = 0;
	//
	//	int pLen = polygonList.size();
	//	//    for (vector<Point>::iterator iter2 = (iter1 + 1); iter1 < points.end(); ++iter1, ++iter2)
	//	for (unsigned int i = 0, j = i + 1; i < pLen; i++, j++)
	//	{
	//		if (p.x == polygonList.at(i)->x && p.y == polygonList.at(i)->y)
	//			return false;
	//
	//		if (j == pLen)
	//		{
	//			j = 0;
	//			if (fabs(mult(p, q, *polygonList.at(i))) < eps)
	//			{
	//				//��*points.at(i)������pq�ϣ�ֹͣ��ѭ������ȡq
	//				q.x--;
	//				i = 0;
	//				j = 1;
	//				continue;
	//			}
	//			else if (mult(p, *polygonList.at(i), q) * mult(p, *polygonList.at(j), q) < -eps && mult(*polygonList.at(i), p, *polygonList.at(j)) * mult(*polygonList.at(i), q, *polygonList.at(j)) < -eps)
	//				counter++;
	//			break;
	//		}
	//
	//		if (fabs(mult(p, q, *polygonList.at(i))) < eps)
	//		{
	//			//��*points.at(i)������pq�ϣ�ֹͣ��ѭ������ȡq
	//			q.x--;
	//			i = -1;
	//			j = 0;
	//			continue;
	//		}
	//		else if (mult(p, *polygonList.at(i), q) * mult(p, *polygonList.at(j), q) < -eps && mult(*polygonList.at(i), p, *polygonList.at(j)) * mult(*polygonList.at(i), q, *polygonList.at(j)) < -eps)
	//			counter++;
	//	}
	//	return counter & 1;
	//}

	inline int isPointIn(const Point &p)
	{
		if (gra[(int)p.y][(int)p.x] != 0)
			return true;

		return false;
	}

	

	DWORD WINAPI cutLineWorker(LPVOID lpParam)
	{
		vector < Point *> lineCuttingPointList;
		Point tmpPoint;

		CMemPool * mempoolnts = new CMemPool(SETTING_THREAD_MEMPOOL_SIZE, SETTING_THREAD_MEMPOOL_NUM);

		unsigned int _size = lineList.size();

		unsigned int workerID = (int)lpParam;
		unsigned int workerInterval = _size / SETTING_CUTTING_THREAD;

		if (workerID == 0)
		{
			for (unsigned int i = 0; i < workerInterval + _size % SETTING_CUTTING_THREAD; i++)
			{
				Line * l = lineList.at(i);
				unsigned int size;

				Point * pth = (Point *)mempoolnts->AllocNTS(sizeof(Point));
				pth->x = l->x1;
				pth->y = l->y1;
				lineCuttingPointList.push_back(pth);

				//������������
				size = polygonList.size();
				for (int j = 0; j < size; j++)
				{
					Point * pt1 = polygonList.at(j);
					Point * pt2;
					if (j == size - 1)
						pt2 = polygonList.at(0);
					else
						pt2 = polygonList.at(j + 1);

					Point * res = intersection(l->x1, l->y1, l->x2, l->y2, pt1->x, pt1->y, pt2->x, pt2->y, mempoolnts);
					if (res != NULL)
					{
						lineCuttingPointList.push_back(res);
					}
				}

				Point * pte = (Point *)mempoolnts->AllocNTS(sizeof(Point));
				pte->x = l->x2;
				pte->y = l->y2;
				lineCuttingPointList.push_back(pte);

				if (lineCuttingPointList.at(0)->x != lineCuttingPointList.at(1)->x)
					sort(lineCuttingPointList.begin(), lineCuttingPointList.end(), cmpx);
				else
					sort(lineCuttingPointList.begin(), lineCuttingPointList.end(), cmpy);

				size = lineCuttingPointList.size();

				for (int j = 0; j < size - 1; j++)
				{
					Point * pt1 = lineCuttingPointList.at(j);
					Point * pt2 = lineCuttingPointList.at(j + 1);
					tmpPoint.x = (pt1->x + pt2->x) / 2;
					tmpPoint.y = (pt1->y + pt2->y) / 2;
					if (isPointIn(tmpPoint))
					{
						Line * l = (Line *)mempool->Alloc(sizeof(Line));
						l->x1 = pt1->x;
						l->y1 = pt1->y;
						l->x2 = pt2->x;
						l->y2 = pt2->y;
						cutLineListList[workerID].push_back(l);
					}
				}

				//������ϴε�����
				size = lineCuttingPointList.size();
				for (int i = 0; i < size; i++)
				{
					mempoolnts->FreeNTS(lineCuttingPointList.at(i));
				}
				lineCuttingPointList.clear();
			}
		}
		else
		{
			for (unsigned int i = workerInterval*workerID + _size % SETTING_CUTTING_THREAD; i < workerInterval*(workerID + 1) + _size % SETTING_CUTTING_THREAD; i++)
			{
				Line * l = lineList.at(i);
				int size;

				Point * pth = (Point *)mempoolnts->AllocNTS(sizeof(Point));
				pth->x = l->x1;
				pth->y = l->y1;
				lineCuttingPointList.push_back(pth);

				//������������
				size = polygonList.size();
				for (int j = 0; j < size; j++)
				{
					Point * pt1 = polygonList.at(j);
					Point * pt2;
					if (j == size - 1)
						pt2 = polygonList.at(0);
					else
						pt2 = polygonList.at(j + 1);

					Point * res = intersection(l->x1, l->y1, l->x2, l->y2, pt1->x, pt1->y, pt2->x, pt2->y, mempoolnts);
					if (res != NULL)
					{
						lineCuttingPointList.push_back(res);
					}
				}

				Point * pte = (Point *)mempoolnts->AllocNTS(sizeof(Point));
				pte->x = l->x2;
				pte->y = l->y2;
				lineCuttingPointList.push_back(pte);

				if (lineCuttingPointList.at(0)->x != lineCuttingPointList.at(1)->x)
					sort(lineCuttingPointList.begin(), lineCuttingPointList.end(), cmpx);
				else
					sort(lineCuttingPointList.begin(), lineCuttingPointList.end(), cmpy);

				size = lineCuttingPointList.size();

				for (int j = 0; j < size - 1; j++)
				{
					Point * pt1 = lineCuttingPointList.at(j);
					Point * pt2 = lineCuttingPointList.at(j + 1);
					tmpPoint.x = (pt1->x + pt2->x) / 2;
					tmpPoint.y = (pt1->y + pt2->y) / 2;
					if (isPointIn(tmpPoint))
					{
						Line * l = (Line *)mempool->Alloc(sizeof(Line));
						l->x1 = pt1->x;
						l->y1 = pt1->y;
						l->x2 = pt2->x;
						l->y2 = pt2->y;
						cutLineListList[workerID].push_back(l);
					}
				}

				//������ϴε�����
				size = lineCuttingPointList.size();
				for (int i = 0; i < size; i++)
				{
					mempoolnts->FreeNTS(lineCuttingPointList.at(i));
				}
				lineCuttingPointList.clear();
			}
		}
		
		delete mempoolnts;

		SetEvent(events[workerID]);
		return 0;
	}

	DWORD WINAPI cutCircleWorker(LPVOID lpParam)
	{
		vector < float> lineCuttingCirclePointList;

		unsigned int _size = circleList.size();

		unsigned int workerID = (int)lpParam;
		unsigned int workerInterval = _size / SETTING_CUTTING_THREAD;


		if (workerID == 0)
		{
			for (unsigned int i = 0; i < workerInterval + _size % SETTING_CUTTING_THREAD; i++)
			{
				Circle * c = circleList.at(i);
				unsigned int size;

				//������������
				size = polygonList.size();
				for (int j = 0; j < size; j++)
				{
					Point * pt1 = polygonList.at(j);
					Point * pt2;
					if (j == size - 1)
						pt2 = polygonList.at(0);
					else
						pt2 = polygonList.at(j + 1);

					float res1, res2;

					int num = GetPoint(c->x, c->y, c->r, pt1->x, pt1->y, pt2->x, pt2->y, &res1, &res2);
					if (num == 2)
					{
						lineCuttingCirclePointList.push_back(res1);
						lineCuttingCirclePointList.push_back(res2);
					}
					else if (num == 1)
					{
						lineCuttingCirclePointList.push_back(res1);
					}
				}

				sort(lineCuttingCirclePointList.begin(), lineCuttingCirclePointList.end());

				Point pt;
				size = lineCuttingCirclePointList.size();
				if (size == 0)
				{
					pt.x = c->x;
					pt.y = c->y;
					if (isPointIn(pt))
					{
						CArc * theArc = (CArc *)mempool->Alloc(sizeof(CArc));
						theArc->r = c->r;
						theArc->x = c->x;
						theArc->y = c->y;
						theArc->begin = 0;
						theArc->end = 2 * PI;
						cutArcListList[workerID].push_back(theArc);
					}
				}
				else for (int i = 0; i < size; i++)
				{
					float arc1, arc2, arc;
					arc1 = lineCuttingCirclePointList.at(i);
					if (i == size - 1)
					{
						arc2 = lineCuttingCirclePointList.at(0);
						arc2 += 2 * PI;
					}
					else
						arc2 = lineCuttingCirclePointList.at(i + 1);

					arc = (arc1 + arc2) / 2;
					pt.x = _cosTable[((int)(arc * 10000) % 63000)] * c->r + c->x;
					pt.y = _sinTable[((int)(arc * 10000) % 63000)] * c->r + c->y;

					if (isPointIn(pt))
					{
						CArc * theArc = (CArc *)mempool->Alloc(sizeof(CArc));
						theArc->r = c->r;
						theArc->x = c->x;
						theArc->y = c->y;
						theArc->begin = arc1;
						theArc->end = arc2;
						cutArcListList[workerID].push_back(theArc);
					}
				}

				//������ϴε�����
				lineCuttingCirclePointList.clear();
			}
		}
		else
		{
			for (unsigned int i = workerInterval*workerID + _size % SETTING_CUTTING_THREAD; i < workerInterval*(workerID + 1) + _size % SETTING_CUTTING_THREAD; i++)
			{
				Circle * c = circleList.at(i);
				int size;

				//������������
				size = polygonList.size();
				for (int j = 0; j < size; j++)
				{
					Point * pt1 = polygonList.at(j);
					Point * pt2;
					if (j == size - 1)
						pt2 = polygonList.at(0);
					else
						pt2 = polygonList.at(j + 1);

					float res1, res2;

					int num = GetPoint(c->x, c->y, c->r, pt1->x, pt1->y, pt2->x, pt2->y, &res1, &res2);
					if (num == 2)
					{
						lineCuttingCirclePointList.push_back(res1);
						lineCuttingCirclePointList.push_back(res2);
					}
					else if (num == 1)
					{
						lineCuttingCirclePointList.push_back(res1);
					}
				}

				sort(lineCuttingCirclePointList.begin(), lineCuttingCirclePointList.end());

				Point pt;
				size = lineCuttingCirclePointList.size();
				if (size == 0)
				{
					pt.x = c->x;
					pt.y = c->y;
					if (isPointIn(pt))
					{
						CArc * theArc = (CArc *)mempool->Alloc(sizeof(CArc));
						theArc->r = c->r;
						theArc->x = c->x;
						theArc->y = c->y;
						theArc->begin = 0;
						theArc->end = 2 * PI;
						cutArcListList[workerID].push_back(theArc);
					}
				}
				else for (int i = 0; i < size; i++)
				{
					float arc1, arc2, arc;
					arc1 = lineCuttingCirclePointList.at(i);
					if (i == size - 1)
					{
						arc2 = lineCuttingCirclePointList.at(0);
						arc2 += 2 * PI;
					}
					else
						arc2 = lineCuttingCirclePointList.at(i + 1);

					arc = (arc1 + arc2) / 2;
					pt.x = _cosTable[((int)(arc * 10000) % 63000)] * c->r + c->x;
					pt.y = _sinTable[((int)(arc * 10000) % 63000)] * c->r + c->y;

					if (isPointIn(pt))
					{
						CArc * theArc = (CArc *)mempool->Alloc(sizeof(CArc));
						theArc->r = c->r;
						theArc->x = c->x;
						theArc->y = c->y;
						theArc->begin = arc1;
						theArc->end = arc2;
						cutArcListList[workerID].push_back(theArc);
					}
				}

				//������ϴε�����
				lineCuttingCirclePointList.clear();
			}
		}

		SetEvent(events[workerID]);
		return 0;
	}

	//�и������߳�
	DWORD WINAPI cuttingThread(LPVOID lpParam)
	{
		if (polygonList.size() < 3)
		{
			MessageBox(theHWND, L"�߽����ݹ���", L"����", 0);
			return 0;
		}

		if (polygonList.size() > MAX_POLYGON_SIZE)
		{
			MessageBox(theHWND, L"�߽����ݹ��࣬���޸Ĳ������±���", L"����", 0);
			return 0;
		}

		isCutBusy = true;

		events = new HANDLE[SETTING_CUTTING_THREAD];
		for (int i = 0; i < SETTING_CUTTING_THREAD; i++)
		{
			events[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		}

		cutLineListList = new vector < Line * >[SETTING_CUTTING_THREAD];
		cutArcListList = new vector < CArc * >[SETTING_CUTTING_THREAD];
			
		logMsg(L"��ʼ�ü�ֱ��");
		//���߲ü���ֱ��
		for (int i = 0; i < SETTING_CUTTING_THREAD; i++)
		{
			HANDLE hThread = CreateThread(NULL, 0, cutLineWorker, (LPVOID)i, 0, NULL);
			if (hThread == NULL)
			{
				MessageBox(theHWND, L"�̴߳���ʧ��", L"����", 0);
				PostMessage(theHWND, WM_DESTROY, 0, 0);
			}
			CloseHandle(hThread);
		}
		WaitForMultipleObjects(SETTING_CUTTING_THREAD, events, true, INFINITE);
		logMsg(L"ֱ�߲ü�����");

		for (int i = 0; i < SETTING_CUTTING_THREAD; i++)
		{
			cutLineList.insert(cutLineList.end(), cutLineListList[i].begin(), cutLineListList[i].end());
			cutLineListList[i].clear();
		}
		logMsg(L"����ϲ����");

		logMsg(L"��ʼ�ü�԰");
		//���̲߳ü�԰
		for (int i = 0; i < SETTING_CUTTING_THREAD; i++)
		{
			HANDLE hThread = CreateThread(NULL, 0, cutCircleWorker, (LPVOID)i, 0, NULL);
			if (hThread == NULL)
			{
				MessageBox(theHWND, L"�̴߳���ʧ��", L"����", 0);
				PostMessage(theHWND, WM_DESTROY, 0, 0);
			}
			CloseHandle(hThread);
		}
		WaitForMultipleObjects(SETTING_CUTTING_THREAD, events, true, INFINITE);
		logMsg(L"Բ�ü�����");


		for (int i = 0; i < SETTING_CUTTING_THREAD; i++)
		{
			cutArcList.insert(cutArcList.end(), cutArcListList[i].begin(), cutArcListList[i].end());
			cutArcListList[i].clear();
		}
		logMsg(L"����ϲ����");

		isCutBusy = false;

		PostMessage(theHWND, WM_COMMAND, IDM_AFTER_CUT, 0);

		for (int i = 0; i < SETTING_CUTTING_THREAD; i++)
			CloseHandle(events[i]);

		delete[] events;
		delete[] cutLineListList;
		delete[] cutArcListList;

		return 0;
	}
}

//���������߳��и�ͼ��
void doCPUCut()
{
	using namespace cpuCUT;

	if (isCutted)
	{
		MessageBox(theHWND, L"�Ѿ��ü���", L"����", 0);
		return;
	}
	isCutted = true;

	HANDLE hThread;

	hThread = CreateThread(NULL, 0, cuttingThread, 0, 0, NULL);
	if (hThread == NULL)
	{
		MessageBox(theHWND, L"�̴߳���ʧ��", L"����", 0);
		PostMessage(theHWND, WM_DESTROY, 0, 0);
	}

	PostMessage(theHWND, WM_COMMAND, ID_REFRESH,0);

	CloseHandle(hThread);
}

bool initGra()
{
	using namespace cpuCUT;

	logMsg(L"��ʼ��ʼ���߽绺��");

	HDC drawDC = GetDC(theHWND);
	HDC maskDC = CreateCompatibleDC(drawDC);
	HBITMAP maskBMP = CreateCompatibleBitmap(drawDC, 1366, 768);

	if (drawDC == NULL || maskDC == NULL || maskBMP == NULL)
		return false;

	SelectObject(maskDC, maskBMP);

	unsigned int size = polygonList.size();
	if (size > MAX_POLYGON_SIZE)
		return false;//��������Դû�л��գ����ù��ˣ��������в��˻���ʲôû�л���

	POINT polygonArray[MAX_POLYGON_SIZE];
	int indexArray = size;

	for (int i = 0; i < size; i++)
	{
		Point * pt = polygonList.at(i);
		polygonArray[i].x = pt->x;
		polygonArray[i].y = pt->y;
	}

	HRGN region = CreatePolyPolygonRgn(polygonArray, &indexArray, 1, WINDING);
	HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));

	if (region == NULL || brush == NULL)
		return false;

	if (FillRgn(maskDC, region, brush) == 0)
		return false;

	HPEN polygonPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	SelectObject(maskDC, polygonPen);
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
			MoveToEx(maskDC, last_x, last_y, NULL);
			LineTo(maskDC, (int)(*it)->x, (int)(*it)->y);
			last_x = (int)(*it)->x;
			last_y = (int)(*it)->y;
		}
	}
	if (last_x != -1 && last_y != -1)
	{
		MoveToEx(maskDC, last_x, last_y, NULL);
		LineTo(maskDC, (int)(*polygonList.begin())->x, (int)(*polygonList.begin())->y);
	}

	if (GetBitmapBits(maskBMP, sizeof(gra), gra) == 0)
		return false;

	DeleteDC(drawDC);
	DeleteDC(maskDC);
	DeleteObject(maskBMP);
	DeleteObject(brush);
	DeleteObject(polygonPen);

	logMsg(L"�߽绺���ʼ�����");

	return true;
}