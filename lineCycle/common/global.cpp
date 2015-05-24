#include "../stdafx.h"

vector < Line *> lineList;
vector < Circle *> circleList;
vector < Point * > polygonList;

vector < Line *> cutLineList;
vector < CArc *> cutArcList;

bool isCutBusy = false;
bool isCutted = false;

DWORD timeBase;

void logMsg(WCHAR * msg)
{
	wprintf_s(L"%d ms \t %s\n\r", GetTickCount() - timeBase , msg, 256);
}