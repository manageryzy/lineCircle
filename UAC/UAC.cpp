// UAC.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include "tlhelp32.h"

DWORD GetProcessIdFromName(LPCTSTR name)    //ͨ��ִ���ļ�����ý���ID�ķ���
{
	PROCESSENTRY32 pe;
	DWORD id = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return 0;

	do
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (strcmp(pe.szExeFile, name) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}

	} while (1);

	CloseHandle(hSnapshot);

	return id;
}



int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, GetProcessIdFromName("lineCycle.exe"));
	
	if (hProcess == NULL)
	{
		MessageBoxW(0, L"���ý������ȼ����󣬽��̲�����", L"����", 0);
		exit(-1);
	}

	if (SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS) == 0)
	{
		MessageBoxW(0, L"���ý������ȼ������Ƽ��ֶ�ͨ�����̹������������ȼ�Ϊʵʱ", L"����", 0);
	}
	CloseHandle(hProcess);

	return 0;
}

