// 360.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include"tlhelp32.h"

DWORD GetProcessidFromName(LPCTSTR name)
{
	PROCESSENTRY32 pe;
	DWORD id = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return 0;
	while (1)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (lstrcmp(pe.szExeFile, name) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}


	}
	CloseHandle(hSnapshot);
	return id;
}

int main()
{
	if (GetProcessidFromName(L"360tray.exe") != 0) {
		MessageBox(0, L"����360�����϶��ɰ汾�ĳ����ж�����������һ���ǳ���ʹ�ľ���������������360ɱ������ļ��������ر�360����", L"����", 0);
		return -1;
	}

	return 0;
}

