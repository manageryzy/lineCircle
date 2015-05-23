#include "../stdafx.h"

#pragma optimize( "", off )

//��ֱ�� l1�� a1*x + b1*y + c1 = 0 ��ֱ�� l2�� a2*x + b2*y + c2 = 0 �Ľ���
//a��bΪֱ�� l1 �ϵ����㣬 c , dΪֱ�� l2 �ϵ�����
Point * intersection(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy)
{
	float a1 = by - ay;
	float b1 = ax - bx;
	float c1 = bx * ay - by * ax;
	float a2 = dy - cy;
	float b2 = cx - dx;
	float c2 = dx * cy - dy * cx;
	float dlt = a1 * b2 - a2 * b1;
	if (dlt == 0) return NULL;    //�߶�ƽ��
	float x = (c2 * b1 - c1 * b2) / dlt;
	float y = (c1 * a2 - c2 * a1) / dlt; //����������

	if (x < min(ax, bx) || x > max(ax, bx) || x < min(cx, dx) || x > max(cx, dx) || y < min(ay, by) || y > max(ay, by) || y < min(cy, dy) || y > max(cy, dy)) return NULL; //�߶�û�н���
	//if ((int)x == (int)cx && (int)y == (int)cy) return NULL;
	Point * res = (Point *)mempool->Alloc(sizeof(Point));
	res->x = x, res->y = y;
	return res;
}