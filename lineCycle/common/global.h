//全局变量头文件

extern HWND theHWND;
extern wchar_t strFile[MAX_PATH];

extern vector < line > lineList;
extern vector < circle > circleList;
extern vector < point > polygonList;


//计时部分（计时精度为55ms）
extern DWORD tickStartLoadXML;
extern DWORD tickFinishLoadXML;

extern DWORD tickStartDrawLine;
extern DWORD tickFinishDrawLine;
extern DWORD tickStartDrawCircle;
extern DWORD tickFinishDrawCircle;

extern DWORD tickStartCutLine;
extern DWORD tickFinishCutLine;
extern DWORD tickStartCutCircle;
extern DWORD tickFinishCutCircle;

extern DWORD tickStartReDrawLine;
extern DWORD tickFinishReDrawLine;
extern DWORD tickStartReDrawCircle;
extern DWORD tickFinishReDrawCircle;