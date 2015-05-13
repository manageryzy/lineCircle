#include "../stdafx.h"

void onMemGDIDraw()
{
	HDC hdc = theDC;

	for (vector <Line> ::iterator it = lineList.begin(); it != lineList.end(); ++it)
	{
		MoveToEx(hdc, it->x1, it->y1, NULL);
		LineTo(hdc, it->x2, it->y2);
	}

	for (vector <Circle> ::iterator it = circleList.begin(); it != circleList.end(); ++it)
	{
		Ellipse(hdc, it->x - it->r, it->y - it->r, it->x + it->r, it->y + it->r);
	}

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

}