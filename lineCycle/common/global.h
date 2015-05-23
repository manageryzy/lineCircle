//全局变量头文件

extern HWND theHWND;
extern HDC theDC;
extern HGLRC ghRC;
extern wchar_t strFile[MAX_PATH];

extern CMemPool * mempool;

extern vector < Line *> lineList;
extern vector < Circle *> circleList;
extern vector < Point *> polygonList;

extern vector < Line *> cutLineList;
extern vector < CArc *> cutArcList;

extern bool isCutBusy;