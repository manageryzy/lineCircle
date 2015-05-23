#include "../stdafx.h"

#include "../../js/sin.h"
#include "../../js/cos.h"
#include "../../js/tan.h"
#include "../../js/asin.h"
#include "../../js/acos.h"
#include "../../js/atan.h"

float Sin(float x)
{
	if (SETTING_ENABLE_FUNCTION_TABLE)
		return _tSin(x);
	else
		return sinf(x);
}

float Cos(float x)
{
	if (SETTING_ENABLE_FUNCTION_TABLE)
		return _tCos(x);
	else
		return cosf(x);
}

float Tan(float x)
{
	if (SETTING_ENABLE_FUNCTION_TABLE)
		return _tTan(x);
	else
		return tanf(x);
}

float ArcSin(float x)
{
	if (SETTING_ENABLE_FUNCTION_TABLE)
		return _tArcSin(x);
	else
		return asinf(x);
}

float ArcCos(float x)
{
	if (SETTING_ENABLE_FUNCTION_TABLE)
		return _tArcCos(x);
	else
		return acosf(x);
}

float ArcTan(float x)
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


