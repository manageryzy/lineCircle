#include "../stdafx.h"

//极小化的基于状态机的xml解析
//

namespace minXML
{

	bool praseXML(WCHAR * fileName)
	{
		char charBuf;
		char str[255];
		bool praseXML(WCHAR *);
		inline void onCharRead();
		int state = 0;
		int num = 0, i = 0, temp;
		short numBuf1, numBuf2;
		bool isNumRead = false;
		bool succeed = false;

		char * buffer;
		DWORD readsize;

		HANDLE fileHandle = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

		if (fileHandle == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		DWORD filesize = GetFileSize(fileHandle, NULL);

		try
		{
			buffer = new char[filesize + 1];
		}
		catch (...)
		{
			return false;
		}

		if (ReadFile(fileHandle, buffer, filesize, &readsize, NULL) == 0)
		{
			return false;
		}


		for (char * ch = buffer; ch <= buffer + filesize; ch++)
		{
			charBuf = *ch;
			if (charBuf == EOF)
				break;

			switch (state)
			{
			case 0:if (charBuf == 'I')state = 1;
				   else;  break;
			case 1:if (charBuf == 'D')state = 2;
				   else if (charBuf == 'I');
				   else state = 0;  break;
			case 2:if ('0' <= charBuf && charBuf <= '9'){ str[i++] = charBuf; state = 3; }
				   else; break;
			case 3:if ('0' <= charBuf && charBuf <= '9'){ str[i++] = charBuf; state = 3; }
				   else
				   {
					   num = 0;
					   for (int j = 0; i--; j++)
					   {
						   temp = str[i] - '0';
						   num *= 10;
						   num += temp;
					   }
					   i = 0;
					   state = 4;
				   }	break;
			case 4:if (num == SETTING_XML_TESTCASE) state = 5;		//匹配成功
				   else state = 0;  break;
			case 5:succeed = true;
				   if (charBuf == 'E') state = 6;
				   else if (charBuf == 'B') state = 37;
				   else; break;
			case 6:if (charBuf == 'L') state = 7;
				   else if (charBuf == 'C') state = 26;
				   else; break;
			case 7:if (charBuf == 'S') state = 8;
				   else if (charBuf == 'E') state = 21;
				   else; break;
			case 8:if (charBuf == '>'){ state = 9; }
				   else; break;
			case 9:
				if (charBuf == '<')
				{
					str[i] = 0;
					if (isNumRead)
					{
						isNumRead = false;
						Line * line = (Line *)mempool->Alloc(sizeof(Line));
						sscanf_s(str, "%hd,%hd", &line->x1, &line->y1);
						line->x2 = numBuf1;
						line->y2 = numBuf2;
						lineList.push_back(line);
					}
					else
					{
						isNumRead = true;
						sscanf_s(str, "%hd,%hd", &numBuf1, &numBuf2);
					}
					i = 0; state = 10;
				}
				else str[i++] = charBuf;
				break;
			case 10:if (charBuf == 'E')state = 11;
					else; break;
			case 11:if (charBuf == '>')state = 12;
					else; break;
			case 12:if (charBuf == '<')
			{
				str[i] = 0;
				if (isNumRead)
				{
					isNumRead = false;
					Line * line = (Line *)mempool->Alloc(sizeof(Line));
					sscanf_s(str, "%hd,%hd", &line->x2, &line->y2);
					line->x1 = numBuf1;
					line->y1 = numBuf2;
					lineList.push_back(line);
				}
				else
				{
					isNumRead = true;
					sscanf_s(str, "%hd,%hd", &numBuf1, &numBuf2);
				}

				i = 0; state = 13;
			}
					else str[i++] = charBuf;
					break;
			case 13:if (charBuf == '<')state = 14;
					else; break;
			case 14:if (charBuf == '<')state = 15;
					else; break;
			case 15:if (charBuf == 'E') state = 6;		//检测下一个Entity或Boundary 
					else if (charBuf == 'B') state = 16;
					else if (charBuf == 'T') state = 00; //exit
					else; break;
			case 16:if (charBuf == 'V')state = 17;
					else; break;
			case 17:if (charBuf == '>')state = 18;
					else; break;
			case 18:if (charBuf == '<')
			{
				str[i] = 0;
				Point * point = (Point *)mempool->Alloc(sizeof(Point));
				sscanf_s(str, "%hd,%hd", &point->x, &point->y);
				polygonList.push_back(point);
				i = 0; state = 19;
			}
					else str[i++] = charBuf;
					break;
			case 19:if (charBuf == '<') state = 20;
					else; break;
			case 20: if (charBuf == 'V')state = 17;
					 else if (charBuf == 'B')state = 14;
					 else; break;

			case 21:if (charBuf == '>')state = 22;		  //EndPoint在前
					else; break;
			case 22:if (charBuf == '<')
			{
				str[i] = 0;
				if (isNumRead)
				{
					isNumRead = false;
					Line * line = (Line *)mempool->Alloc(sizeof(Line));
					sscanf_s(str, "%hd,%hd", &line->x2, &line->y2);
					line->x1 = numBuf1;
					line->y1 = numBuf2;
					lineList.push_back(line);
				}
				else
				{
					isNumRead = true;
					sscanf_s(str, "%hd,%hd", &numBuf1, &numBuf2);
				}

				i = 0; state = 23;
			}
					else str[i++] = charBuf;
					break;
			case 23:if (charBuf == 'S')state = 24;
					else; break;
			case 24:if (charBuf == '>')state = 25;
					else; break;
			case 25:if (charBuf == '<')
			{
				str[i] = 0;
				if (isNumRead)
				{
					isNumRead = false;
					Line * line = (Line *)mempool->Alloc(sizeof(Line));
					sscanf_s(str, "%hd,%hd", &line->x1, &line->y1);
					line->x2 = numBuf1;
					line->y2 = numBuf2;
					lineList.push_back(line);
				}
				else
				{
					isNumRead = true;
					sscanf_s(str, "%hd,%hd", &numBuf1, &numBuf2);
				}
				i = 0; state = 13;
			}
					else str[i++] = charBuf;
					break;

			case 26:if (charBuf == 'C') state = 27;			//检测到Circle
					else if (charBuf == 'R') state = 32;
					else; break;
			case 27:if (charBuf == '>')state = 28;
					else; break;
			case 28:if (charBuf == '<')
			{
				str[i] = 0;
				if (isNumRead)
				{
					isNumRead = false;
					Circle * circle = (Circle *)mempool->Alloc(sizeof(Circle));
					sscanf_s(str, "%hd,%hd", &circle->x, &circle->y);
					circle->r = numBuf1;
					circleList.push_back(circle);
				}
				else
				{
					isNumRead = true;
					sscanf_s(str, "%hd,%hd", &numBuf1, &numBuf2);
				}
				i = 0; state = 29;
			}
					else str[i++] = charBuf;
					break;
			case 29:if (charBuf == 'R')state = 30;
					else; break;
			case 30:if (charBuf == '>')state = 31;
					else; break;
			case 31:if (charBuf == '<')
			{
				str[i] = 0;
				if (isNumRead)
				{
					isNumRead = false;
					Circle * circle = (Circle *)mempool->Alloc(sizeof(Circle));
					sscanf_s(str, "%hd", &circle->r);
					circle->x = numBuf1;
					circle->y = numBuf2;
					circleList.push_back(circle);
				}
				else
				{
					isNumRead = true;
					sscanf_s(str, "%hd", &numBuf1);
				}
				i = 0; state = 13;
			}
					else str[i++] = charBuf;
					break;

			case 32:if (charBuf == '>')state = 33;		//Radius在前
					else; break;
			case 33:if (charBuf == '<')
			{
				str[i] = 0;
				if (isNumRead)
				{
					isNumRead = false;
					Circle * circle = (Circle *)mempool->Alloc(sizeof(Circle));
					sscanf_s(str, "%hd", &circle->r);
					circle->x = numBuf1;
					circle->y = numBuf2;
					circleList.push_back(circle);
				}
				else
				{
					isNumRead = true;
					sscanf_s(str, "%hd", &numBuf1);
				}
				i = 0; state = 34;
			}
					else str[i++] = charBuf;
					break;
			case 34:if (charBuf == 'C')state = 35;
					else; break;
			case 35:if (charBuf == '>')state = 36;
					else; break;
			case 36:if (charBuf == '<')
			{
				str[i] = 0;
				if (isNumRead)
				{
					isNumRead = false;
					Circle * circle = (Circle *)mempool->Alloc(sizeof(Circle));
					sscanf_s(str, "%hd,%hd", &circle->x, &circle->y);
					circle->r = numBuf1;
					circleList.push_back(circle);
				}
				else
				{
					isNumRead = true;
					sscanf_s(str, "%hd,%hd", &numBuf1, &numBuf2);
				}
				i = 0; state = 13;
			}
					else str[i++] = charBuf;
					break;

			case 37:if (charBuf == 'V')state = 38;		//Boundary在前
					else; break;
			case 38:if (charBuf == '>')state = 39;
					else; break;
			case 39:if (charBuf == '<')
			{
				str[i] = 0;
				Point * point = (Point *)mempool->Alloc(sizeof(Point));
				sscanf_s(str, "%hd,%hd", &point->x, &point->y);
				polygonList.push_back(point);
				i = 0; state = 40;
			}
					else str[i++] = charBuf;
					break;
			case 40:if (charBuf == '<')state = 41;
					else; break;
			case 41: if (charBuf == 'V')state = 38;
					 else if (charBuf == 'B')state = 14;
					 else; break;

			}
		}

		delete[]buffer;
		CloseHandle(fileHandle);

		if (!succeed)
		{
			MessageBox(theHWND, L"没有找到数据节点", L"警告", 0);
		}

		return succeed;
	}
}


bool minXMLPrase()
{
	using namespace minXML;

	isXMLBusy = true;
	bool res = praseXML(strFile);
	isXMLBusy = false;

	return res;
}
