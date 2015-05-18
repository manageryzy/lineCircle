#include  "../stdafx.h"

namespace cpuCUT{
	__inline float mult(Point a, Point b, Point c);
	bool isPointOut(Point point);

	float mult(Point a, Point b, Point c){
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	}

	bool isPointOut(Point point){   //n�Ƕ���ζ������, p[]�Ƕ���εĶ��㣬�жϵ�point�Ƿ��ڶ������
		int n = polygonList.size();
		float eps = 1e-4;
		float pre = mult(*polygonList.at(0), *polygonList.at(1), point);
		for (int i = 1; i < n; i++){
			float now = mult(*polygonList.at(i), *polygonList.at((i+1)% n), point);
			if (now * pre < -eps) return true;   //û�ڶ������
		}
		return false;   //�ڶ������
	}

	//��ֱ�� l1�� a1*x + b1*y + c1 = 0 ��ֱ�� l2�� a2*x + b2*y + c2 = 0 �Ľ���
	//a��bΪֱ�� l1 �ϵ����㣬 c , dΪֱ�� l2 �ϵ�����
	bool intersection(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy,Point * res)
	{
		float a1 = by - ay;
		float b1 = ax - bx;
		float c1 = bx * ay - by * ax;
		float a2 = dy - cy;
		float b2 = cx - dx;
		float c2 = dx * cy - dy * cx;
		float dlt = a1 * b2 - a2 * b1;
		if (dlt == 0) return false;    //�߶�ƽ��
		float x = (c2 * b1 - c1 * b2) / dlt;
		if (x < ax || x > bx || x < cx || x > dx) return false; //�߶�û�н���
		float y = (c1 * a2 - c2 * a1) / dlt; //����������
		res->x = x, res->y = y;
		return true;
	}

	bool GetPoint(float cx, float cy, float r, float stx, float sty, float edx, float edy,float* res1,float * res2){
		//(x - cx )^2 + (y - cy)^2 = r^2
		//y = kx +b
		//���ֱ�߷���
		float k = ((float)(edy - sty)) / (edx - stx);
		float b = edy - k * edx;
		//�з���
		//(1 + k^2)*x^2 - x*(2*cx -2*k*(b -cy) ) + cx*cx + ( b - cy)*(b - cy) - r*r = 0
		float x1, y1, x2, y2;
		float c = cx * cx + (b - cy) * (b - cy) - r * r;
		float a = (1 + k * k);
		float b1 = (2 * cx - 2 * k * (b - cy));
		//�õ�����ļ򻯷���
		// a*x^2 - b1*x + c = 0;
		float dlt = b1 * b1 - 4 * a * c;
		if (dlt < 0) return false; //ֱ�ߺ�Բ���ཻ
		if (dlt == 0) {  //Բ��ֱ��ֻ��һ������
			x1 = b1 / 2 / a, y1 = k * x1 + b;
			float dlty = y1 - cy, dltx = x1 - cx;  //���㻡��
			float alp = atan(dlty / dltx);
			if (y1 < cy || (y1 == cy && x1 < cx))
				alp += 3.1415926;
			*res1 = alp;
			return true;
		}   //Բ��ֱ������������
		float tmp = sqrt(dlt);
		x1 = (b1 + tmp) / (2 * a);
		y1 = k * x1 + b;
		x2 = (b1 - tmp) / (2 * a);
		y2 = k * x2 + b;

		float dlty = y1 - cy, dltx = x1 - cx;
		float alp = atan(dlty / dltx);
		if (y1 < cy || (y1 == cy && x1 < cx))
			alp += 3.1415926;
		*res1 = alp;    //��һ����

		dlty = y2 - cy, dltx = x2 - cx;
		alp = atan(dlty / dltx);
		if (y2 < cy || (y2 == cy && x2 < cx))
			alp += 3.1415926;
		*res2 = alp;    //�ڶ�����
		return true;
	}
}

//���������߳��и�ͼ��
void doCPUCut()
{
	using namespace cpuCUT;

	if (polygonList.size() < 3)
	{
		MessageBox(theHWND, L"�߽����ݹ���", L"����", 0);
		return;
	}


}