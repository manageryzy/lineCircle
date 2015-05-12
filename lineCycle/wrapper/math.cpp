#include "../stdafx.h"

const float _sinTable[] = { 0.0f };

__inline float Sin(float x)
{
	if (SETTING_ENABLE_FUNCTION_TABLE)
		return _tSin(x);
	else
		return sinf(x);
}

__inline float Cos(float x)
{
	if (SETTING_ENABLE_FUNCTION_TABLE)
		return _tCos(x);
	else
		return cosf(x);
}

__inline float Tan(float x)
{
	if (SETTING_ENABLE_FUNCTION_TABLE)
		return _tTan(x);
	else
		return tanf(x);
}

__inline float ArcSin(float x)
{
	if (SETTING_ENABLE_FUNCTION_TABLE)
		return _tArcSin(x);
	else
		return asinf(x);
}

__inline float ArcCos(float x)
{
	if (SETTING_ENABLE_FUNCTION_TABLE)
		return _tArcCos(x);
	else
		return acosf(x);
}

__inline float ArcTan(float x)
{
	if (SETTING_ENABLE_FUNCTION_TABLE)
		return _tArcTan(x);
	else
		return atanf(x);
}


float _tSin(float x)
{
	//TODO: 
	return 0.0f;
}
float _tCos(float x)
{
	//TODO: 
	return 0.0f;
}
float _tTan(float x)
{
	//TODO: 
	return 0.0f;
}
float _tArcSin(float x)
{
	//TODO: 
	return 0.0f;
}
float _tArcCos(float x)
{
	//TODO: 
	return 0.0f;
}
float _tArcTan(float x)
{
	//TODO: 
	return 0.0f;
}
