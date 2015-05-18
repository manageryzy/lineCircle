//��ѧ��װ

#include <math.h>

extern const float _sinTable[];
extern const float _cosTable[];


float Sin(float x);
float Cos(float x);
float Tan(float x);
float ArcSin(float x);
float ArcCos(float x);
float ArcTan(float x);


__inline float _tSin(float x);
__inline float _tCos(float x);
__inline float _tTan(float x);
__inline float _tArcSin(float x);
__inline float _tArcCos(float x);
__inline float _tArcTan(float x);
