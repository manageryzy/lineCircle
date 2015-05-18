// lineCycle.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "lineCycle.h"
#include "Shellapi.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

CMemPool * mempool;

HWND theHWND;
HDC theDC;
HGLRC ghRC;
wchar_t strFile[MAX_PATH];

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
bool isBusy();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LINECYCLE, szWindowClass, MAX_LOADSTRING);

	loadIniSetting();

	mempool = new CMemPool(SETTING_MEMPOOL_NUM,SETTING_MEMPOOL_SIZE);

	//提升自身优先级到实时
	HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, GetCurrentProcessId());
	if (SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS) == 0)
	{
		MessageBeep(0);
		MessageBox(0, L"设置进程优先级错误，推荐手动通过进程管理器设置优先级为实时", L"警告", 0);
	}
	CloseHandle(hProcess);

	hProcess = GetCurrentProcess();
	if (GetPriorityClass(hProcess) != REALTIME_PRIORITY_CLASS)
	{
		MessageBeep(0);
		if (MessageBox(0, L"设置进程优先级没有完全成功，是否尝试以管理员权限开启", L"警告", MB_YESNO) == IDYES)
		{
			WCHAR currentDir[MAX_PATH];

			GetCurrentDirectory(MAX_PATH, currentDir);

			SHELLEXECUTEINFO shExInfo = { 0 };
			shExInfo.cbSize = sizeof(shExInfo);
			shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			shExInfo.hwnd = 0;
			shExInfo.lpVerb = _T("open");                // Operation to perform
			shExInfo.lpFile = L"UAC.exe";				// Application to start    
			shExInfo.lpParameters = L"";                  // Additional parameters
			shExInfo.lpDirectory = currentDir;
			shExInfo.nShow = SW_SHOW;
			shExInfo.hInstApp = 0;
			if (!ShellExecuteEx(&shExInfo))
			{
				MessageBeep(0);
				MessageBox(0, L"提权失败", L"错误", 0);
			}
		}
	}
	CloseHandle(hProcess);

	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LINECYCLE));

	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LINECYCLE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1440, 900, NULL, NULL, hInstance, NULL);

   theHWND = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;

	switch (message)
	{
	case WM_CREATE:
		theDC = GetDC(hWnd);
		GetClientRect(hWnd, &rect);

		if (SETTING_DRAW_MODE == SETTING_DRAW_MODE_OPENGL)
		{
			if (!bSetupPixelFormat(theDC))
				PostQuitMessage(0);

			ghRC = wglCreateContext(theDC);
			wglMakeCurrent(theDC, ghRC);
			
			initializeGL(rect.right, rect.bottom);
		}
		break;
	case WM_CLOSE:
		if (ghRC)
			wglDeleteContext(ghRC);
		if (theDC)
			ReleaseDC(hWnd, theDC);
		ghRC = 0;
		theDC = 0;

		DestroyWindow(hWnd);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_OPEN:
			if (isXMLBusy)
			{
				MessageBox(theHWND, L"XML解析器仍然在工作中，忙", L"错误", 0);
				break;
			}

			for (vector <Point *> ::iterator it = polygonList.begin(); it != polygonList.end(); ++it)
			{
				mempool->Free(*it);
			}
			for (vector <Line *> ::iterator it = lineList.begin(); it != lineList.end(); ++it)
			{
				mempool->Free(*it);
			}
			for (vector <Circle *> ::iterator it = circleList.begin(); it != circleList.end(); ++it)
			{
				mempool->Free(*it);
			}
			lineList.clear();
			circleList.clear();
			polygonList.clear();

			OPENFILENAME ofn;
			memset(&ofn, 0, sizeof(OPENFILENAME));
			memset(strFile, 0, sizeof(char)*MAX_PATH);
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = theHWND;

			ofn.lpstrFilter = L"Data XML File\0*.xml\0";
			ofn.lpstrFile = strFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn))
			{
				xmlPrase();
			}
			else
			{
				MessageBox(theHWND, L"没有选择文件名", L"警告", 0);
			}

			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_HELP:
			//TODO:
			break;
		case IDM_CUT:
			//TODO:
			break;
		case IDM_BEFORE_CUT:
			SETTING_DRAW_CUTTING = false;
			PostMessage(theHWND, WM_COMMAND, ID_ACCELERATOR_REFRESH, NULL);
			break;
		case IDM_AFTER_CUT:
			SETTING_DRAW_CUTTING = true;
			PostMessage(theHWND, WM_COMMAND, ID_ACCELERATOR_REFRESH, NULL);
			break;
		case ID_ACCELERATOR_REFRESH:
			GetClientRect(hWnd, &rect);
			InvalidateRect(theHWND, &rect, TRUE);
			UpdateWindow(theHWND);
			break;
		case ID_ACCELERATOR_RELOAD:
			if (SETTING_DRAW_MODE == SETTING_DRAW_MODE_MEMGDI)
			{
				clearMemGDICache();
			}
			else if (SETTING_DRAW_MODE == SETTING_DRAW_MODE_MEMORY)
			{
				clearGDICache();
			}
			GetClientRect(hWnd, &rect);
			InvalidateRect(theHWND, &rect, TRUE);
			UpdateWindow(theHWND);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		if (isBusy())
		{
			hdc = BeginPaint(hWnd, &ps);
			theDC = hdc;
			GetClientRect(hWnd, &rect);
			DrawText(hdc, L"BUSY", 4, &rect, 0);

			EndPaint(hWnd, &ps);
		}
		else
		{
			switch (SETTING_DRAW_MODE)
			{
			case SETTING_DRAW_MODE_GDI:
				hdc = BeginPaint(hWnd, &ps);
				theDC = hdc;
				onGDIDraw();
				EndPaint(hWnd, &ps);
				break;
			case SETTING_DRAW_MODE_MEMGDI:
				hdc = BeginPaint(hWnd, &ps);
				theDC = hdc;
				onMemGDIDraw();
				EndPaint(hWnd, &ps);
				break;
			case SETTING_DRAW_MODE_MEMORY:
				hdc = BeginPaint(hWnd, &ps);
				theDC = hdc;
				onMemDraw();
				EndPaint(hWnd, &ps);
				break;
			case SETTING_DRAW_MODE_OPENGL:
				drawScene();
				break;
			}
		}
		
		
		break;
	case WM_DESTROY:
		if (ghRC)
			wglDeleteContext(ghRC);
		if (theDC)
			ReleaseDC(hWnd, theDC);

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

bool isBusy()
{
	return isXMLBusy;
}