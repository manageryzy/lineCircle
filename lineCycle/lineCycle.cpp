// lineCycle.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "lineCycle.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

HWND theHWND;
HDC theDC;
HGLRC ghRC;
wchar_t strFile[MAX_PATH];

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
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
			if (GetOpenFileName(&ofn)){

				//��ʼ��ȡXML

				tickStartLoadXML = GetTickCount();

				switch (SETTING_XML_MODE)
				{
				case 0:
					if (!tinyXMLPrase())
					{
						MessageBox(theHWND, L"tinyXML����ʧ�ܡ�", L"����", 0);
					}
					break;
				case 1:
					if (!minXMLPrase())
					{
						MessageBox(theHWND, L"��С��XML����ʧ�ܡ�", L"����", 0);
					}
					break;
				case 2:
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


				tickFinishLoadXML = GetTickCount();

				//��ʼ��һ���ͼ
				GetClientRect(hWnd, &rect);
				InvalidateRect(theHWND, &rect, TRUE);
				UpdateWindow(theHWND);
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

		case ID_ACCELERATOR_REFRESH:
			GetClientRect(hWnd, &rect);
			InvalidateRect(theHWND, &rect, TRUE);
			UpdateWindow(theHWND);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
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
			EndPaint(hWnd, &ps);
			break;
		case SETTING_DRAW_MODE_MEMORY:
			hdc = BeginPaint(hWnd, &ps);
			theDC = hdc;
			EndPaint(hWnd, &ps);
			break;
		case SETTING_DRAW_MODE_OPENGL:
			drawScene();
			break;
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
