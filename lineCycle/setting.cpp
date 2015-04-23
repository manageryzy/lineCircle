//设置文件

#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////
//
//							XML 解析设定
//
///////////////////////////////////////////////////////////////////////////


int SETTING_XML_MIN = true;//启用最小化XML解析，不能兼容全部XML

int SETTING_XML_TESTCASE = 1;//测试用例


///////////////////////////////////////////////////////////////////////////
//
//								画图设定
//
//////////////////////////////////////////////////////////////////////////

//默认绘图模式为内存绘图
int SETTING_DRAW_MODE = 2;

//绘图默认线程数量
int SETTING_DRAW_THREAD = 4;

//////////////////////////////////////////////////////////////////////////
//
//								裁剪设定
//
/////////////////////////////////////////////////////////////////////////

//默认裁剪模式为像素过滤
int SETTING_CUTTING_MODE = 2;

//裁剪的时候线程数量
int SETTING_CUTTING_THREAD = 4;

//NVIDIA® 的 CUDA™ 显卡加速计算
int SETTING_ENABLE_CUDA = false;

//计算三角函数等的时候是否启动打表
int SETTING_ENABLE_FUNCTION_TABLE = true;


//////////////////////////////////////////////////////////////////////////
//
//								设定读取
//
/////////////////////////////////////////////////////////////////////////

int loadIniSetting()
{
	SETTING_XML_MIN = GetPrivateProfileInt(L"xmlSetting", L"SETTING_XML_MIN", SETTING_XML_MIN, L"./setting.ini");
	SETTING_XML_TESTCASE = GetPrivateProfileInt(L"xmlSetting", L"SETTING_XML_TESTCASE", SETTING_XML_TESTCASE, L"./setting.ini");

	SETTING_DRAW_MODE = GetPrivateProfileInt(L"drawSetting", L"SETTING_DRAW_MODE", SETTING_DRAW_MODE, L"./setting.ini");
	SETTING_DRAW_THREAD = GetPrivateProfileInt(L"drawSetting", L"SETTING_DRAW_THREAD", SETTING_DRAW_THREAD, L"./setting.ini");

	SETTING_CUTTING_MODE = GetPrivateProfileInt(L"cuttingSetting", L"SETTING_CUTTING_MODE", SETTING_CUTTING_MODE, L"./setting.ini");
	SETTING_CUTTING_THREAD = GetPrivateProfileInt(L"cuttingSetting", L"SETTING_CUTTING_THREAD", SETTING_CUTTING_THREAD, L"./setting.ini");
	SETTING_ENABLE_CUDA = GetPrivateProfileInt(L"cuttingSetting", L"SETTING_ENABLE_CUDA", SETTING_ENABLE_CUDA, L"./setting.ini");
	SETTING_ENABLE_FUNCTION_TABLE = GetPrivateProfileInt(L"cuttingSetting", L"SETTING_ENABLE_FUNCTION_TABLE", SETTING_ENABLE_FUNCTION_TABLE, L"./setting.ini");

	return 0;
}