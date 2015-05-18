//数学包装

#include <math.h>

extern const float _sinTable[];
extern const float _cosTable[];

__inline float Sin(float x);
__inline float Cos(float x);
__inline float Tan(float x);
__inline float ArcSin(float x);
__inline float ArcCos(float x);
__inline float ArcTan(float x);


__inline float _tSin(float x);
__inline float _tCos(float x);
__inline float _tTan(float x);
__inline float _tArcSin(float x);
__inline float _tArcCos(float x);
__inline float _tArcTan(float x);

