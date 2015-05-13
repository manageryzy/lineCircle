// UAC.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include "tlhelp32.h"

DWORD GetProcessIdFromName(LPCTSTR name)    //通过执行文件名获得进程ID的方法
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
		MessageBoxW(0, L"设置进程优先级错误，进程不存在", L"警告", 0);
		exit(-1);
	}

	if (SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS) == 0)
	{
		MessageBoxW(0, L"设置进程优先级错误，推荐手动通过进程管理器设置优先级为实时", L"警告", 0);
	}
	CloseHandle(hProcess);

	return 0;
}

