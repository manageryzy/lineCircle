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
			MessageBox(theHWND, L"tinyXML����ʧ�ܡ�", L"����", 0);
		}
		break;
	case SETTING_XML_MODE_MINXML:
		if (!minXMLPrase())
		{
			MessageBox(theHWND, L"��С��XML����ʧ�ܡ�", L"����", 0);
		}
		break;
	case SETTING_XML_MODE_PUGIXML:
		if (!pugiXMLPrase())
		{
			MessageBox(theHWND, L"pugiXML����ʧ�ܡ�", L"����", 0);
		}
		break;
	default:
		MessageBox(theHWND, L"�����XML��������", L"����", 0);
		exit(-1);
		break;
	}
}

//�ǻ���ģʽ�Ĺ����߳�
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

//�жϻ����Ƿ����
bool isCacheExist()
{
	struct stat fileStat;
	FILE * fp;
	WCHAR workingDir[MAX_PATH];//����·��������APPData��·��
	WCHAR fileNameBuf[MAX_PATH];//�ļ�������

	WCHAR cacheFileNameBuf[MAX_PATH];//�ļ���
	time_t mt_time;//�޸�ʱ��

	SHGetFolderPath(theHWND, CSIDL_APPDATA , NULL, 0,workingDir);
	
	//��ȡlable�ļ�
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\lable.txt",workingDir,SETTING_XML_TESTCASE);
	fp = _wfopen(fileNameBuf, L"r");
	if (fp == NULL)
		return false;
	fwscanf(fp, L"%I64d\n",&mt_time);
	fgetws(cacheFileNameBuf, MAX_PATH, fp);
	fclose(fp);

	//�ж��ļ����Ƿ�һ��
	if (lstrcmpW(cacheFileNameBuf, strFile) != 0)
		return false;

	//ת�����ֽڵ�char
	setlocale(LC_ALL, "zh-CN");
	size_t len = wcslen(strFile) + 1;
	size_t converted = 0;
	char *CStr;
	CStr = (char*)malloc(len*sizeof(char) * 2);
	wcstombs_s(&converted, CStr, len * 2, strFile, len * 2);

	//�������
	if (stat(CStr, &fileStat) == -1)
		return false;

	//�ж������Ƿ�һ��
	if (mt_time != fileStat.st_mtime)
		return false;

	//�ж��ļ�������
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

//���뻺��
void loadCache()
{
	//TODO:
}

//д�뻺��
void saveCache()
{
	//TODO:
}

//����ģʽ�Ĺ����߳�
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
			MessageBox(theHWND, L"�̴߳���ʧ��", L"����", 0);
			PostMessage(theHWND, WM_DESTROY, 0, 0);
		}
		CloseHandle(hThread);
	}
	else
	{
		hThread = CreateThread(NULL, 0, praseXMLWorker, 0, 0, NULL);
		if (hThread == NULL)
		{
			MessageBox(theHWND, L"�̴߳���ʧ��", L"����", 0);
			PostMessage(theHWND, WM_DESTROY, 0, 0);
		}
		CloseHandle(hThread);
	}

}