#include "../stdafx.h"

//极小化的基于状态机的xml解析
//

namespace minXMLPrase
{
	FILE * fp;
	WCHAR charBuf;
	bool praseXML(WCHAR *);
	inline void onCharRead();

	inline void onCharRead()
	{

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
	using namespace minXMLPrase;

	isXMLBusy = true;
	bool res = praseXML(strFile);
	isXMLBusy = false;

	return res;
}