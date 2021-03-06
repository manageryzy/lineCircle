#include "../stdafx.h"
#include <sys/stat.h>
#include <Shlobj.h>
#include "../resource.h"
#include <io.h>

bool isXMLBusy = false;

bool xmlPraseNoCache();
bool isCacheExist();
bool loadCache();
bool saveCache();

DWORD WINAPI praseXMLWorker(LPVOID lpParam);
DWORD WINAPI cacheXMLWorker(LPVOID lpParam);



bool xmlPraseNoCache()
{
	switch (SETTING_XML_MODE)
	{
	case SETTING_XML_MODE_TINYXML:
		if (!tinyXMLPrase())
		{
			MessageBox(theHWND, L"tinyXML解析失败。", L"警告", 0);
			return false;
		}
		break;
	case SETTING_XML_MODE_MINXML:
		if (!minXMLPrase())
		{
			MessageBox(theHWND, L"最小化XML解析失败。", L"警告", 0);
			return false;
		}
		break;
	case SETTING_XML_MODE_PUGIXML:
		if (!pugiXMLPrase())
		{
			MessageBox(theHWND, L"pugiXML解析失败。", L"警告", 0);
			return false;
		}
		break;
	default:
		MessageBox(theHWND, L"错误的XML解析设置", L"错误", 0);
		exit(-1);
		break;
	}
	return true;
}

//非缓存模式的工作线程
DWORD WINAPI praseXMLWorker(LPVOID lpParam)
{
	isXMLBusy = true;

	if (!xmlPraseNoCache())
	{
		MessageBox(theHWND, L"XML解析失败", L"错误", 0);
		return 0;
	}

	if (!initGra())
	{
		MessageBox(theHWND, L"裁剪区域准备失败", L"错误", 0);
		PostMessage(theHWND, WM_DESTROY, 0, 0);
	}

	isXMLBusy = false;
	isCutted = false;

	WCHAR buf[255];
	wsprintf(buf, L"XML读取结束，读入 %d 线段，%d 圆，%d 边界", lineList.size(), circleList.size(), polygonList.size());
	logMsg(buf);

	showMemPool();

	PostMessage(theHWND, WM_COMMAND, ID_ACCELERATOR_RELOAD, 0);

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
	if (_wfopen_s(&fp, fileNameBuf, L"rb"))
		return false;
	fwscanf_s(fp, L"%I64d\r\n", &mt_time);
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
	if (_waccess_s(fileNameBuf, 04))
		return false;

	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\circle.data", workingDir, SETTING_XML_TESTCASE);
	if (_waccess_s(fileNameBuf, 04))
		return false;
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\polygon.data", workingDir, SETTING_XML_TESTCASE);
	if (_waccess_s(fileNameBuf, 04))
		return false;

	return true;
}

//读入缓存
bool loadCache()
{
	FILE * fp;
	WCHAR workingDir[MAX_PATH];//工作路径，就是APPData的路径
	WCHAR fileNameBuf[MAX_PATH];//文件名缓存

	SHGetFolderPath(theHWND, CSIDL_APPDATA, NULL, 0, workingDir);

	//读取line
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\line.data", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp, fileNameBuf, L"rb");
	if (fp == NULL)
		return false;
	while (1)
	{
		Line * line = (Line *)mempool->Alloc(sizeof(Line));
		if (fread(line, sizeof(Line), 1, fp) == NULL)
		{
			mempool->Free(line);
			break;
		}
		lineList.push_back(line);
	}
	fclose(fp);

	//读取circle
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\circle.data", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp, fileNameBuf, L"rb");
	if (fp == NULL)
		return false;
	while (1)
	{
		Circle * circle = (Circle *)mempool->Alloc(sizeof(Circle));
		if (fread(circle, sizeof(Circle), 1, fp) == NULL)
		{
			mempool->Free(circle);
			break;
		}
		circleList.push_back(circle);
	}
	fclose(fp);

	//读取polygon
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\polygon.data", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp, fileNameBuf, L"rb");
	if (fp == NULL)
		return false;
	while (1)
	{
		Point * point = (Point *)mempool->Alloc(sizeof(Point));
		if (fread(point, sizeof(Point), 1, fp) == NULL)
		{
			mempool->Free(point);
			break;
		}
		polygonList.push_back(point);
	}
	fclose(fp);

	return true;
}

//写入缓存
bool saveCache()
{
	struct stat fileStat;
	FILE * fp;
	WCHAR workingDir[MAX_PATH];//工作路径，就是APPData的路径
	WCHAR fileNameBuf[MAX_PATH];//文件名缓存

	SHGetFolderPath(theHWND, CSIDL_APPDATA, NULL, 0, workingDir);

	//没有数据，直接跳过
	if (polygonList.size() == 0)
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

	//创建文件夹
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d", workingDir, SETTING_XML_TESTCASE);
	_wmkdir(fileNameBuf);

	//写入lable文件
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\lable.txt", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp, fileNameBuf, L"wb");
	if (fp == NULL)
		return false;
	fwprintf(fp, L"%I64d\r\n", fileStat.st_mtime);
	fputws(strFile, fp);
	fclose(fp);

	//写入直线
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\line.data", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp, fileNameBuf, L"wb");
	if (fp == NULL)
		return false;
	for (unsigned int i=0; i < lineList.size(); ++i)
	{
		Line * line = lineList.at(i);

		fwrite(line, sizeof(Line), 1, fp);
	}
	fclose(fp);

	//写入圆
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\circle.data", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp, fileNameBuf, L"wb");
	if (fp == NULL)
		return false;
	for (unsigned int i = 0; i < circleList.size(); ++i)
	{
		Circle * circle = circleList.at(i);

		fwrite(circle, sizeof(Circle), 1, fp);
	}
	fclose(fp);

	//写入边界
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\polygon.data", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp,fileNameBuf, L"wb");
	if (fp == NULL)
		return false;
	for (unsigned int i = 0; i < polygonList.size(); ++i)
	{
		Point * point = polygonList.at(i);

		fwrite(point, sizeof(Point), 1, fp);
	}
	fclose(fp);

	return true;
}

//缓存模式的工作线程
DWORD WINAPI cacheXMLWorker(LPVOID lpParam)
{
	isXMLBusy = true;

	if (isCacheExist())
	{
		logMsg(L"缓存有效，开始读缓存");
		loadCache();
		logMsg(L"读取结束");
	}
	else
	{
		logMsg(L"缓存无效！！！");
		if (!xmlPraseNoCache())
		{
			isXMLBusy = false;
			isCutted = false;

			PostMessage(theHWND, WM_COMMAND, ID_ACCELERATOR_RELOAD, 0);
			return 0;
		}
		logMsg(L"开始写入缓存！");
		saveCache();
		logMsg(L"缓存写入结束");
	}	

	if (!initGra())
	{
		MessageBox(theHWND, L"裁剪区域准备失败", L"错误", 0);
		PostMessage(theHWND, WM_DESTROY, 0, 0);
	}

	isXMLBusy = false;
	isCutted = false;

	WCHAR buf[255];
	wsprintf(buf, L"XML读取结束，读入 %d 线段，%d 圆，%d 边界", lineList.size(), circleList.size(), polygonList.size());
	logMsg(buf);

	showMemPool();

	PostMessage(theHWND, WM_COMMAND, ID_ACCELERATOR_RELOAD, 0);

	return 0;
}

void xmlPrase()
{
	HANDLE hThread;

	if (SETTING_XML_CACHE)
	{
		logMsg(L"开始缓存模式XML解析！");
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
		logMsg(L"开始直接XML解析！");
		hThread = CreateThread(NULL, 0, praseXMLWorker, 0, 0, NULL);
		if (hThread == NULL)
		{
			MessageBox(theHWND, L"线程创建失败", L"错误", 0);
			PostMessage(theHWND, WM_DESTROY, 0, 0);
		}
		CloseHandle(hThread);
	}

}