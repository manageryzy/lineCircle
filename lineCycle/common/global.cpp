#include "../stdafx.h"

vector < Line *> lineList;
vector < Circle *> circleList;
vector < Point * > polygonList;

vector < Line *> cutLineList;
vector < CArc *> cutArcList;

bool isCutBusy = false;
bool isCutted = false;

DWORD timeBase;

wstring msgBuffer;

void logMsg(WCHAR * msg)
{
	WCHAR buf[2048];
	static int i = 0;

	wsprintf(buf, L"%d ms \t %s\r\n", GetTickCount() - timeBase, msg);

	msgBuffer += buf;
	SendMessage(theEditHWND, WM_SETTEXT, 0, (LPARAM)msgBuffer.c_str());
	SendMessage(theEditHWND, EM_SETSEL, 0, (LPARAM)msgBuffer.length());
	SendMessage(theEditHWND, EM_LINESCROLL, 0, ++i);
}