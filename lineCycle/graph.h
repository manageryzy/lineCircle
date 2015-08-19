class Point{
public :
	short x, y;
	bool operator < (const Point &m)const
	{
		return x + y < m.x + m.y;
	}
};

class PointF {
public:
	float x, y;
	bool operator < (const PointF &m)const
	{
		return x + y < m.x + m.y;
	}
};

class Line{
public :
	short x1, x2, y1, y2;
};

class Circle{
public:
	short x, y, r;
};

class CArc{
public:
	short x, y, r;
	float  begin, end;
};

PointF * intersection(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy,CMemPool *);
int GetPoint(float cx, float cy, float r, float stx, float sty, float edx, float edy, float* res1, float * res2);