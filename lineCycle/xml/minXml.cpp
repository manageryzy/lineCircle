#include "../stdafx.h"

//极小化的基于状态机的xml解析
//
#define s0 0
#define s1 1
#define s2 2
namespace minXMLPrase_
{
	FILE * fp;
	WCHAR charBuf;
	bool praseXML(WCHAR *);
	inline void onCharRead();
	int state;
	inline void onCharRead()
	{
		switch (state)
		{
		case s0:if (charBuf == 'I')state = s1;
				else state = s0;  break;
		case s1:if (charBuf == 'D')state = s2;
				else if(charBuf == 'I')state = s1;
				else state = s0;  break;
		case s2:if ()

		}
	}

	bool praseXML(WCHAR * fileName)
	{
		_wfopen_s(&fp, fileName, L"rb");
		if (fp == NULL)
		{
			return false;
		}

		while(1)
		{
			charBuf = fgetwc(fp);
			if (charBuf == EOF)
				break;

			onCharRead();
		}
		
		fclose(fp);
		
		return true;
	}
}


bool minXMLPrase()
{
	using namespace minXMLPrase_;

	isXMLBusy = true;
	bool res = praseXML(strFile);
	isXMLBusy = false;

	return res;
}