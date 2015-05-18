// lineCycle.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "lineCycle.h"
#include "Shellapi.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

CMemPool * mempool;

HWND theHWND;
HDC theDC;
HGLRC ghRC;
wchar_t strFile[MAX_PATH];

// �˴���ģ���а����ĺ�����ǰ������: 
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

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LINECYCLE, szWindowClass, MAX_LOADSTRING);

	loadIniSetting();

	mempool = new CMemPool(SETTING_MEMPOOL_NUM,SETTING_MEMPOOL_SIZE);

	//�����������ȼ���ʵʱ
	HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, GetCurrentProcessId());
	if (SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS) == 0)
	{
		MessageBeep(0);
		MessageBox(0, L"���ý������ȼ������Ƽ��ֶ�ͨ�����̹������������ȼ�Ϊʵʱ", L"����", 0);
	}
	CloseHandle(hProcess);

	hProcess = GetCurrentProcess();
	if (GetPriorityClass(hProcess) != REALTIME_PRIORITY_CLASS)
	{
		MessageBeep(0);
		if (MessageBox(0, L"���ý������ȼ�û����ȫ�ɹ����Ƿ����Թ���ԱȨ�޿���", L"����", MB_YESNO) == IDYES)
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
				MessageBox(0, L"��Ȩʧ��", L"����", 0);
			}
		}
	}
	CloseHandle(hProcess);

	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LINECYCLE));

	// ����Ϣѭ��: 
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
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
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
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_OPEN:
			if (isXMLBusy)
			{
				MessageBox(theHWND, L"XML��������Ȼ�ڹ����У�æ", L"����", 0);
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
				MessageBox(theHWND, L"û��ѡ���ļ���", L"����", 0);
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

// �����ڡ������Ϣ�������
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