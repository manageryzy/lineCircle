//ȫ�ֱ���ͷ�ļ�

extern HWND theHWND;
extern wchar_t strFile[MAX_PATH];

extern vector < line > lineList;
extern vector < circle > circleList;
extern vector < point > polygonList;


//��ʱ���֣���ʱ����Ϊ55ms��
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