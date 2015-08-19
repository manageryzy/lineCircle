// 360.cpp : 定义控制台应用程序的入口点。
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
		MessageBox(0, L"由于360恶意认定旧版本的程序有毒，我们做出一个非常沉痛的决定：不兼容运行360杀毒软件的计算机，请关闭360运行", L"悲报", 0);
		return -1;
	}

	return 0;
}

