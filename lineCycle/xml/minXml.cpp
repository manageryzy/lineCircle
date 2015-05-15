#include "../stdafx.h"

//极小化的基于状态机的xml解析
//
#define s0 0
#define s1 1
#define s2 2
#define s3 3
#define s4 4
#define s5 5
#define s6 6
#define s7 7
namespace minXMLPrase_
{
	FILE * fp;
	WCHAR charBuf;
	WCHAR str[255];
	bool praseXML(WCHAR *);
	inline void onCharRead();
	int state;
	int num=0,i=0,temp;
	inline void onCharRead()
	{
		switch (state)
		{
		case s0:if (charBuf == 'I')state = s1;
				else ;  break;
		case s1:if (charBuf == 'D')state = s2;
				else if(charBuf == 'I');
				else state = s0;  break;
		case s2:if (30 <= charBuf <= 39){ str[i++] = charBuf; state = s3; }
				else ; break;
		case s3:if (30 <= charBuf <= 39){ str[i++] = charBuf; state = s3; }
				else
				{
					for (int j = 0; i--; j++)
					{
						temp = str[i - 1] - '0';
						while (j--)
							temp *= 10;
						num += temp;
					}
					state = s4;
				}	break;
		case s4:if (num == SETTING_XML_TESTCASE) state = s5;
				else state = s0;  break;
		case s5:if (charBuf == 'E') state = s6;
				else if (charBuf == 'B') state = s;
				else state = s5; break; 
		case s6:if (charBuf == 'L') state = s7;
				else if (charBuf == 'C') state = s;
				else state = s6; break;
		case s7:if (30 <= charBuf <= 39) state = s8;
				else state = s7;
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