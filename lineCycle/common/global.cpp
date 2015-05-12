#include "../stdafx.h"

vector < line > lineList;
vector < circle > circleList;
vector < point > polygonList;


//计时部分
DWORD tickStartLoadXML = 0;
DWORD tickFinishLoadXML = 0;

DWORD tickStartDrawLine = 0;
DWORD tickFinishDrawLine = 0;
DWORD tickStartDrawCircle = 0;
DWORD tickFinishDrawCircle = 0;

DWORD tickStartCutLine = 0;
DWORD tickFinishCutLine = 0;
DWORD tickStartCutCircle = 0;
DWORD tickFinishCutCircle = 0;

DWORD tickStartReDrawLine = 0;
DWORD tickFinishReDrawLine = 0;
DWORD tickStartReDrawCircle = 0;
DWORD tickFinishReDrawCircle = 0;