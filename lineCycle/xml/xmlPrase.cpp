#include "../stdafx.h"
#include <sys/stat.h>
#include <Shlobj.h>

bool isXMLBusy = false;

void xmlPraseNoCache();
bool isCacheExist();
void loadCache();
void saveCache();

DWORD WINAPI praseXMLWorker(LPVOID lpParam);
DWORD WINAPI cacheXMLWorker(LPVOID lpParam);



void xmlPraseNoCache()
{
	switch (SETTING_XML_MODE)
	{
	case SETTING_XML_MODE_TINYXML:
		if (!tinyXMLPrase())
		{
			MessageBox(theHWND, L"tinyXML解析失败。", L"警告", 0);
		}
		break;
	case SETTING_XML_MODE_MINXML:
		if (!minXMLPrase())
		{
			MessageBox(theHWND, L"最小化XML解析失败。", L"警告", 0);
		}
		break;
	case SETTING_XML_MODE_PUGIXML:
		if (!pugiXMLPrase())
		{
			MessageBox(theHWND, L"pugiXML解析失败。", L"警告", 0);
		}
		break;
	default:
		MessageBox(theHWND, L"错误的XML解析设置", L"错误", 0);
		exit(-1);
		break;
	}
}

//非缓存模式的工作线程
DWORD WINAPI praseXMLWorker(LPVOID lpParam)
{
	isXMLBusy = true;

	xmlPraseNoCache();

	isXMLBusy = false;

	RECT rect;
	GetClientRect(theHWND, &rect);
	InvalidateRect(theHWND, &rect, TRUE);
	UpdateWindow(theHWND);

	return 0;
}

//判断缓存是否可用
bool isCacheExist()
{
	struct stat fileStat;
	FILE * fp;
	WCHAR workingDir[MAX_PATH];//工作路径，就是APPData的路径
	WCHAR fileNameBuf[MAX_PATH];//文件名缓存

	WCHAR cacheFileNameBuf[MAX_PATH];//文件名
	time_t mt_time;//修改时间

	SHGetFolderPath(theHWND, CSIDL_APPDATA , NULL, 0,workingDir);
	
	//读取lable文件
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\lable.txt",workingDir,SETTING_XML_TESTCASE);
	fp = _wfopen(fileNameBuf, L"r");
	if (fp == NULL)
		return false;
	fwscanf(fp, L"%I64d\n",&mt_time);
	fgetws(cacheFileNameBuf, MAX_PATH, fp);
	fclose(fp);

	//判断文件名是否一致
	if (lstrcmpW(cacheFileNameBuf, strFile) != 0)
		return false;

	//转换宽字节到char
	setlocale(LC_ALL, "zh-CN");
	size_t len = wcslen(strFile) + 1;
	size_t converted = 0;
	char *CStr;
	CStr = (char*)malloc(len*sizeof(char) * 2);
	wcstombs_s(&converted, CStr, len * 2, strFile, len * 2);

	//获得日期
	if (stat(CStr, &fileStat) == -1)
		return false;

	//判断日期是否一致
	if (mt_time != fileStat.st_mtime)
		return false;

	//判断文件完整性
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\line.data", workingDir, SETTING_XML_TESTCASE);
	fp = _wfopen(fileNameBuf, L"r");
	if (fp == NULL)
		return false;
	fclose(fp);
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\circle.data", workingDir, SETTING_XML_TESTCASE);
	fp = _wfopen(fileNameBuf, L"r");
	if (fp == NULL)
		return false;
	fclose(fp);
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\polygon.data", workingDir, SETTING_XML_TESTCASE);
	fp = _wfopen(fileNameBuf, L"r");
	if (fp == NULL)
		return false;
	fclose(fp);

	return true;
}

//读入缓存
void loadCache()
{
	//TODO:
}

//写入缓存
void saveCache()
{
	//TODO:
}

//缓存模式的工作线程
DWORD WINAPI cacheXMLWorker(LPVOID lpParam)
{
	isXMLBusy = true;

	if (isCacheExist())
	{
		loadCache();
	}
	else
	{
		xmlPraseNoCache();
		saveCache();
	}	

	isXMLBusy = false;

	RECT rect;
	GetClientRect(theHWND, &rect);
	InvalidateRect(theHWND, &rect, TRUE);
	UpdateWindow(theHWND);

	return 0;
}

void xmlPrase()
{
	HANDLE hThread;

	if (SETTING_XML_CACHE)
	{
		hThread = CreateThread(NULL, 0, cacheXMLWorker, 0, 0, NULL);
		if (hThread == NULL)
		{
			MessageBox(theHWND, L"线程创建失败", L"错误", 0);
			PostMessage(theHWND, WM_DESTROY, 0, 0);
		}
		CloseHandle(hThread);
	}
	else
	{
		hThread = CreateThread(NULL, 0, praseXMLWorker, 0, 0, NULL);
		if (hThread == NULL)
		{
			MessageBox(theHWND, L"线程创建失败", L"错误", 0);
			PostMessage(theHWND, WM_DESTROY, 0, 0);
		}
		CloseHandle(hThread);
	}

}