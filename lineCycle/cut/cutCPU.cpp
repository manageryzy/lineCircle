#include  "../stdafx.h"


namespace cpuCUT{

	__inline float mult(Point a, Point b, Point c);

	int gra[768][1366];

	HANDLE * events;

	const double eps = 1e-8;


	inline float mult(Point a, Point b, Point c){
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	}

	bool cmpx(const Point* p1, const Point* p2)
	{
		return p1->x > p2->x;
	}

	bool cmpy(const Point* p1, const Point* p2)
	{
		return p1->y > p2->y;
	}


	//由于效率底下，放弃这种方法了，由于懒，所以也不打算作为一种可选方案了
	//int isPointIn(const Point &p)
	//{
	//	Point q;
	//	q.x = 1368;
	//	q.y = 768;//以p为起点q为终点做射线L
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
	//				//点*points.at(i)在射线pq上，停止本循环，另取q
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
	//			//点*points.at(i)在射线pq上，停止本循环，另取q
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

	bool GetPoint(float cx, float cy, float r, float stx, float sty, float edx, float edy,float* res1,float * res2){
		//(x - cx )^2 + (y - cy)^2 = r^2
		//y = kx +b
		//求得直线方程
		float k = ((float)(edy - sty)) / (edx - stx);
		float b = edy - k * edx;
		//列方程
		//(1 + k^2)*x^2 - x*(2*cx -2*k*(b -cy) ) + cx*cx + ( b - cy)*(b - cy) - r*r = 0
		float x1, y1, x2, y2;
		float c = cx * cx + (b - cy) * (b - cy) - r * r;
		float a = (1 + k * k);
		float b1 = (2 * cx - 2 * k * (b - cy));
		//得到下面的简化方程
		// a*x^2 - b1*x + c = 0;
		float dlt = b1 * b1 - 4 * a * c;
		if (dlt < 0) return false; //直线和圆不相交
		if (dlt == 0) {  //圆与直线只有一个交点
			x1 = b1 / 2 / a, y1 = k * x1 + b;
			float dlty = y1 - cy, dltx = x1 - cx;  //计算弧度
			float alp = ArcTan(dlty / dltx);
			if (y1 < cy || (y1 == cy && x1 < cx))
				alp += 3.1415926;
			*res1 = alp;
			return true;
		}   //圆与直线有两个交点
		float tmp = sqrt(dlt);
		x1 = (b1 + tmp) / (2 * a);
		y1 = k * x1 + b;
		x2 = (b1 - tmp) / (2 * a);
		y2 = k * x2 + b;

		float dlty = y1 - cy, dltx = x1 - cx;
		float alp = ArcTan(dlty / dltx);
		if (y1 < cy || (y1 == cy && x1 < cx))
			alp += 3.1415926;
		*res1 = alp;    //第一个点

		dlty = y2 - cy, dltx = x2 - cx;
		alp = ArcTan(dlty / dltx);
		if (y2 < cy || (y2 == cy && x2 < cx))
			alp += 3.1415926;
		*res2 = alp;    //第二个点
		return true;
	}

	DWORD WINAPI cutLineWorker(LPVOID lpParam)
	{
		vector < Point *> lineCuttingPointList;


		for (unsigned int i = 0; i < lineList.size() ; i++)
		{
			Line * l = lineList.at(i);
			int size;

			//清除掉上次的垃圾
			size = lineCuttingPointList.size();
			for (int i = 0; i < size; i++)
			{
				mempool->Free(lineCuttingPointList.at(i));
			}
			lineCuttingPointList.clear();

			Point * pth = (Point *)mempool->Alloc(sizeof(Point));
			pth->x = l->x1;
			pth->y = l->y1;
			lineCuttingPointList.push_back(pth);

			//建立交点数组
			size = polygonList.size();
			for (int j = 0; j < size; j++)
			{
				Point * pt1 = polygonList.at(j);
				Point * pt2;
				if (j == size - 1)
					pt2 = polygonList.at(0);
				else
					pt2 = polygonList.at(j + 1);

				Point * res = intersection(l->x1, l->y1, l->x2, l->y2, pt1->x, pt1->y, pt2->x, pt2->y);
				if (res != NULL)
				{
					lineCuttingPointList.push_back(res);
				}
			}

			Point * pte = (Point *)mempool->Alloc(sizeof(Point));
			pte->x = l->x2;
			pte->y = l->y2;
			lineCuttingPointList.push_back(pte);

			if (lineCuttingPointList.at(0)->x != lineCuttingPointList.at(1)->x)
				sort(lineCuttingPointList.begin(), lineCuttingPointList.end(), cmpx);
			else
				sort(lineCuttingPointList.begin(), lineCuttingPointList.end(), cmpy);

			size = lineCuttingPointList.size();
			Point tmpPoint;
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
					cutLineList.push_back(l);
				}
			}
		}
		

		SetEvent(events[0]);
		return 0;
	}

	DWORD WINAPI cutCircleWorker(LPVOID lpParam)
	{
		SetEvent(events[1]);
		return 0;
	}


	//切割启动线程
	DWORD WINAPI cuttingThread(LPVOID lpParam)
	{
		if (polygonList.size() < 3)
		{
			MessageBox(theHWND, L"边界内容过少", L"错误", 0);
			return 0;
		}

		if (polygonList.size() > MAX_POLYGON_SIZE)
		{
			MessageBox(theHWND, L"边界内容过多，请修改参数重新编译", L"错误", 0);
			return 0;
		}

		isCutBusy = true;

		events = new HANDLE[2];
		for (int i = 0; i < 2; i++)
			events[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

		//多线裁剪制直线
		HANDLE hThread;
		hThread = CreateThread(NULL, 0, cutLineWorker, 0, 0, NULL);
		if (hThread == NULL)
		{
			MessageBox(theHWND, L"线程创建失败", L"错误", 0);
			PostMessage(theHWND, WM_DESTROY, 0, 0);
		}
		CloseHandle(hThread);

		//多线程裁剪园
		hThread = CreateThread(NULL, 0, cutCircleWorker, 0, 0, NULL);
		if (hThread == NULL)
		{
			MessageBox(theHWND, L"线程创建失败", L"错误", 0);
			PostMessage(theHWND, WM_DESTROY, 0, 0);
		}
		CloseHandle(hThread);

		WaitForMultipleObjects(SETTING_DRAW_THREAD, events, true, INFINITE);

		isCutBusy = false;

		for (int i = 0; i < 2; i++)
			CloseHandle(events[i]);

		delete[] events;

		return 0;
	}
}

//非阻塞多线程切割图形
void doCPUCut()
{
	using namespace cpuCUT;

	HANDLE hThread;

	hThread = CreateThread(NULL, 0, cuttingThread, 0, 0, NULL);
	if (hThread == NULL)
	{
		MessageBox(theHWND, L"线程创建失败", L"错误", 0);
		PostMessage(theHWND, WM_DESTROY, 0, 0);
	}

	CloseHandle(hThread);
}

bool initGra()
{
	using namespace cpuCUT;

	HDC drawDC = GetDC(theHWND);
	HDC maskDC = CreateCompatibleDC(drawDC);
	HBITMAP maskBMP = CreateCompatibleBitmap(drawDC, 1366, 768);

	if (drawDC == NULL || maskDC == NULL || maskBMP == NULL)
		return false;

	SelectObject(maskDC, maskBMP);

	unsigned int size = polygonList.size();
	if (size > MAX_POLYGON_SIZE)
		return false;//这里有资源没有回收，懒得管了，程序都运行不了还管什么没有回收

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

	return true;
}