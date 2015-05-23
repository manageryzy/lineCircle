class Point{
public :
	float x, y;
	bool operator < (const Point &m)const
	{
		return x + y < m.x + m.y;
	}
};

class Line{
public :
	float x1, x2, y1, y2;
};

class Circle{
public:
	float x, y, r;
};

class CArc{
public:
	float x, y, r, begin, end;
};

Point * intersection(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy);