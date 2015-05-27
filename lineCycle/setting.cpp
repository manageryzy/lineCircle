//设置文件
#include "stdafx.h"
#include <Shlobj.h>
extern HWND theHWND;

////////////////////////////////////////////////////////////////////////////
//
//							XML 解析设定
//
///////////////////////////////////////////////////////////////////////////


int SETTING_XML_MODE = 1;//XML解析模式。0-tinyxml 1-极小化解析 2-pugixml

int SETTING_XML_CACHE = true;

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

int SETTING_DRAW_CUTTING = false;

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

int SETTING_MEMPOOL_SIZE = 20;
int SETTING_MEMPOOL_NUM = 2050000;

int SETTING_THREAD_MEMPOOL_SIZE = 20;
int SETTING_THREAD_MEMPOOL_NUM = 1024;
//////////////////////////////////////////////////////////////////////////
//
//								设定读取
//
/////////////////////////////////////////////////////////////////////////

int loadIniSetting()
{
	WCHAR workingDir[MAX_PATH];//工作路径，就是APPData的路径
	WCHAR fileNameBuf[MAX_PATH];//文件名缓存

	SHGetFolderPath(theHWND, CSIDL_APPDATA, NULL, 0, workingDir);
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\conf.ini", workingDir, SETTING_XML_TESTCASE);

	SETTING_XML_MODE = GetPrivateProfileInt(L"xmlSetting", L"SETTING_XML_MODE", SETTING_XML_MODE,fileNameBuf);
	SETTING_XML_CACHE = GetPrivateProfileInt(L"xmlSetting", L"SETTING_XML_CACHE", SETTING_XML_CACHE, fileNameBuf);
	SETTING_XML_TESTCASE = GetPrivateProfileInt(L"xmlSetting", L"SETTING_XML_TESTCASE", SETTING_XML_TESTCASE,fileNameBuf);

	SETTING_DRAW_MODE = GetPrivateProfileInt(L"drawSetting", L"SETTING_DRAW_MODE", SETTING_DRAW_MODE,fileNameBuf);
	SETTING_DRAW_THREAD = GetPrivateProfileInt(L"drawSetting", L"SETTING_DRAW_THREAD", SETTING_DRAW_THREAD,fileNameBuf);

	SETTING_CUTTING_MODE = GetPrivateProfileInt(L"cuttingSetting", L"SETTING_CUTTING_MODE", SETTING_CUTTING_MODE,fileNameBuf);
	SETTING_CUTTING_THREAD = GetPrivateProfileInt(L"cuttingSetting", L"SETTING_CUTTING_THREAD", SETTING_CUTTING_THREAD,fileNameBuf);
	SETTING_ENABLE_CUDA = GetPrivateProfileInt(L"cuttingSetting", L"SETTING_ENABLE_CUDA", SETTING_ENABLE_CUDA,fileNameBuf);
	SETTING_ENABLE_FUNCTION_TABLE = GetPrivateProfileInt(L"cuttingSetting", L"SETTING_ENABLE_FUNCTION_TABLE", SETTING_ENABLE_FUNCTION_TABLE,fileNameBuf);

	SETTING_MEMPOOL_SIZE = GetPrivateProfileInt(L"memorySetting", L"SETTING_MEMPOOL_SIZE", SETTING_MEMPOOL_SIZE, fileNameBuf);
	SETTING_MEMPOOL_NUM = GetPrivateProfileInt(L"memorySetting", L"SETTING_MEMPOOL_NUM", SETTING_MEMPOOL_NUM, fileNameBuf);
	SETTING_THREAD_MEMPOOL_SIZE = GetPrivateProfileInt(L"memorySetting", L"SETTING_THREAD_MEMPOOL_SIZE", SETTING_THREAD_MEMPOOL_SIZE, fileNameBuf);
	SETTING_THREAD_MEMPOOL_NUM = GetPrivateProfileInt(L"memorySetting", L"SETTING_THREAD_MEMPOOL_NUM", SETTING_THREAD_MEMPOOL_NUM, fileNameBuf);

	saveIniSetting();

	return 0;
}

int saveIniSetting()
{
	wchar_t buf[MAX_PATH];

	WCHAR workingDir[MAX_PATH];//工作路径，就是APPData的路径
	WCHAR fileNameBuf[MAX_PATH];//文件名缓存

	SHGetFolderPath(theHWND, CSIDL_APPDATA, NULL, 0, workingDir);
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\conf.ini", workingDir, SETTING_XML_TESTCASE);

	wsprintfW(buf, L"%d", SETTING_XML_MODE);
	WritePrivateProfileString(L"xmlSetting", L"SETTING_XML_MODE", buf, fileNameBuf);
	wsprintfW(buf, L"%d", SETTING_XML_CACHE);
	WritePrivateProfileString(L"xmlSetting", L"SETTING_XML_CACHE", buf, fileNameBuf);
	wsprintfW(buf, L"%d", SETTING_XML_TESTCASE);
	WritePrivateProfileString(L"xmlSetting", L"SETTING_XML_TESTCASE", buf, fileNameBuf);

	wsprintfW(buf, L"%d", SETTING_DRAW_MODE);
	WritePrivateProfileString(L"drawSetting", L"SETTING_DRAW_MODE", buf, fileNameBuf);
	wsprintfW(buf, L"%d", SETTING_DRAW_THREAD);
	WritePrivateProfileString(L"drawSetting", L"SETTING_DRAW_THREAD", buf, fileNameBuf);

	wsprintfW(buf, L"%d", SETTING_CUTTING_MODE);
	WritePrivateProfileString(L"cuttingSetting", L"SETTING_CUTTING_MODE", buf, fileNameBuf);
	wsprintfW(buf, L"%d", SETTING_CUTTING_THREAD);
	WritePrivateProfileString(L"cuttingSetting", L"SETTING_CUTTING_THREAD", buf, fileNameBuf);
	wsprintfW(buf, L"%d", SETTING_ENABLE_CUDA);
	WritePrivateProfileString(L"cuttingSetting", L"SETTING_ENABLE_CUDA", buf, fileNameBuf);
	wsprintfW(buf, L"%d", SETTING_ENABLE_FUNCTION_TABLE);
	WritePrivateProfileString(L"cuttingSetting", L"SETTING_ENABLE_FUNCTION_TABLE", buf, fileNameBuf);

	wsprintfW(buf, L"%d", SETTING_MEMPOOL_SIZE);
	WritePrivateProfileString(L"memorySetting", L"SETTING_MEMPOOL_SIZE", buf, fileNameBuf);
	wsprintfW(buf, L"%d", SETTING_MEMPOOL_NUM);
	WritePrivateProfileString(L"memorySetting", L"SETTING_MEMPOOL_NUM", buf, fileNameBuf);
	wsprintfW(buf, L"%d", SETTING_THREAD_MEMPOOL_SIZE);
	WritePrivateProfileString(L"memorySetting", L"SETTING_THREAD_MEMPOOL_SIZE", buf, fileNameBuf);
	wsprintfW(buf, L"%d", SETTING_THREAD_MEMPOOL_NUM);
	WritePrivateProfileString(L"memorySetting", L"SETTING_THREAD_MEMPOOL_NUM", buf, fileNameBuf);

	return 0;
}