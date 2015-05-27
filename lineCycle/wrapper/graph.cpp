#include "../stdafx.h"

#pragma optimize( "", off )
const float PI = 3.141592653;

//求直线 l1； a1*x + b1*y + c1 = 0 与直线 l2： a2*x + b2*y + c2 = 0 的交点
//a，b为直线 l1 上的两点， c , d为直线 l2 上的两点
Point * intersection(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy,CMemPool * theMemPool)
{
	float a1 = by - ay;
	float b1 = ax - bx;
	float c1 = bx * ay - by * ax;
	float a2 = dy - cy;
	float b2 = cx - dx;
	float c2 = dx * cy - dy * cx;
	float dlt = a1 * b2 - a2 * b1;
	if (dlt == 0) return NULL;    //线段平行
	float x = (c2 * b1 - c1 * b2) / dlt;
	float y = (c1 * a2 - c2 * a1) / dlt; //交点纵坐标

	if (x < min(ax, bx) || x > max(ax, bx) || x < min(cx, dx) || x > max(cx, dx) || y < min(ay, by) || y > max(ay, by) || y < min(cy, dy) || y > max(cy, dy)) return NULL; //线段没有交点
	//if ((int)x == (int)cx && (int)y == (int)cy) return NULL;
	Point * res = (Point *)theMemPool->AllocNTS(sizeof(Point));
	res->x = x, res->y = y;
	return res;
}

int GetPoint(float cx, float cy, float r, float stx, float sty, float edx, float edy, float* res1, float * res2)
{
	//直线方程 ax + by + c = 0
	stx = stx - cx;
	sty = sty - cy;
	edx = edx - cx;
	edy = edy - cy;

	float a = edy - sty;
	float b = stx - edx;
	float c = edx * sty - edy * stx;

	float a2 = a*a;
	float b2 = b*b;
	float c2 = c*c;

	float r2 = r*r;

	float del = (b2 * (a2 * r2 + b2 * r2 - c2));

	int num = 2;

	if (del < 0)
	{
		//没有交点
		return 0;
	}
	if ((a2 + b2) == 0)
	{
		return 0;
	}

	del = sqrtf(del);

	float b2c = b*b*c;
	float ac = a*c;

	float x1, x2, y1, y2;

	if (b == 0)
	{
		x1 = -c / a;
		x2 = -c / a;
		y1 = sqrtf(r2 - (c*c / a2));
		y2 = -y1;
	}
	else
	{
		x1 = (-del - ac) / (a2 + b2);
		y1 = (a * del - b2c) / (b * (a2 + b2));
		x2 = (del - ac) / (a2 + b2);
		y2 = (-b2c - a*del) / (b * (a2 + b2));
	}

	if (x1 == 0)
	{
		if (y1 > 0)
			*res1 = PI / 2;
		else
			*res1 = PI * 3 / 2;
	}
	else
	{
		*res1 = atanf(y1 / x1);
		if (x1 <= 0 && y1 >= 0)
		{
			*res1 += PI;
		}
		else if (x1 <= 0 && y1 <= 0)
		{
			*res1 += PI;
		}
		else if (x1 >= 0 && y1 <= 0)
		{
			*res1 += 2 * PI;
		}
	}
		
	if (x2 == 0)
	{
		if (y2 > 0)
			*res2 = PI / 2;
		else
			*res2 = PI * 3 / 2;
	}
	else
	{
		*res2 = atanf(y2 / x2);
		if (x2 <= 0 && y2 >= 0)
		{
			*res2 += PI;
		}
		else if (x2 <= 0 && y2 <= 0)
		{
			*res2 += PI;
		}
		else if (x2 >= 0 && y2 <= 0)
		{
			*res2 += 2 * PI;
		}
	}
	
	if (x1 < min(stx, edx) || x1 > max(stx, edx) || y1 < min(sty,edy) || y1 > max(sty,edy))
	{
		num--;
		*res1 = *res2;
	}

	if (x2 < min(stx, edx) || x2 > max(stx, edx) || y2 < min(sty,edy) || y2 > max(sty,edy))
	{
		num--;
	}
	

	return num;
}