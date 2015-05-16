#include "../stdafx.h"
#include <sys/stat.h>
#include <Shlobj.h>
#include "../resource.h"

bool isXMLBusy = false;

void xmlPraseNoCache();
bool isCacheExist();
bool loadCache();
bool saveCache();

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

	PostMessage(theHWND, WM_COMMAND, ID_ACCELERATOR_RELOAD, 0);

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
	fp = _wfopen(fileNameBuf, L"rb");
	if (fp == NULL)
		return false;
	fwscanf_s(fp, L"%I64d\r\n", &mt_time, sizeof(time_t));
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
bool loadCache()
{
	FILE * fp;
	WCHAR workingDir[MAX_PATH];//����·��������APPData��·��
	WCHAR fileNameBuf[MAX_PATH];//�ļ�������

	SHGetFolderPath(theHWND, CSIDL_APPDATA, NULL, 0, workingDir);

	//��ȡline
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\line.data", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp, fileNameBuf, L"rb");
	if (fp == NULL)
		return false;
	while (1)
	{
		Line line;
		if (fread(&line, sizeof(line), 1, fp) == NULL)
			break;
		lineList.push_back(line);
	}
	fclose(fp);

	//��ȡcircle
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\circle.data", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp, fileNameBuf, L"rb");
	if (fp == NULL)
		return false;
	while (1)
	{
		Circle circle;
		if (fread(&circle, sizeof(circle), 1, fp) == NULL)
			break;
		circleList.push_back(circle);
	}
	fclose(fp);

	//��ȡpolygon
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\polygon.data", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp, fileNameBuf, L"rb");
	if (fp == NULL)
		return false;
	while (1)
	{
		Point point;
		if (fread(&point, sizeof(point), 1, fp) == NULL)
			break;
		polygonList.push_back(point);
	}
	fclose(fp);

	return true;
}

//д�뻺��
bool saveCache()
{
	struct stat fileStat;
	FILE * fp;
	WCHAR workingDir[MAX_PATH];//����·��������APPData��·��
	WCHAR fileNameBuf[MAX_PATH];//�ļ�������

	SHGetFolderPath(theHWND, CSIDL_APPDATA, NULL, 0, workingDir);

	//û�����ݣ�ֱ������
	if (polygonList.size() == 0)
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

	//�����ļ���
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d", workingDir, SETTING_XML_TESTCASE);
	_wmkdir(fileNameBuf);

	//д��lable�ļ�
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\lable.txt", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp, fileNameBuf, L"wb");
	if (fp == NULL)
		return false;
	fwprintf(fp, L"%I64d\r\n", fileStat.st_mtime);
	fputws(strFile, fp);
	fclose(fp);

	//д��ֱ��
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\line.data", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp, fileNameBuf, L"wb");
	if (fp == NULL)
		return false;
	for (unsigned int i=0; i < lineList.size(); ++i)
	{
		Line line = lineList.at(i);

		fwrite(&line, sizeof(line), 1, fp);
	}
	fclose(fp);

	//д��Բ
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\circle.data", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp, fileNameBuf, L"wb");
	if (fp == NULL)
		return false;
	for (unsigned int i = 0; i < circleList.size(); ++i)
	{
		Circle circle = circleList.at(i);

		fwrite(&circle, sizeof(circle), 1, fp);
	}
	fclose(fp);

	//д��߽�
	wsprintf(fileNameBuf, L"%s\\manageryzy\\lineCircle\\cache\\%d\\polygon.data", workingDir, SETTING_XML_TESTCASE);
	_wfopen_s(&fp,fileNameBuf, L"wb");
	if (fp == NULL)
		return false;
	for (unsigned int i = 0; i < polygonList.size(); ++i)
	{
		Point point = polygonList.at(i);

		fwrite(&point, sizeof(point), 1, fp);
	}
	fclose(fp);

	return true;
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

	if (SETTING_DRAW_MODE == SETTING_DRAW_MODE_MEMGDI)
	{
		clearMemGDICache();
	}

	PostMessage(theHWND, WM_COMMAND, ID_ACCELERATOR_RELOAD,0);

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