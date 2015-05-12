#include "../stdafx.h"

bool tinyXMLPrase()
{
	bool succeed = false;

	//转换宽字节到char
	setlocale(LC_ALL, "zh-CN");
	size_t len = wcslen(strFile) + 1;
	size_t converted = 0;
	char *CStr;
	CStr = (char*)malloc(len*sizeof(char)*2);
	wcstombs_s(&converted, CStr, len*2, strFile, len*2);

	TiXmlDocument * myDocument = new TiXmlDocument(CStr);

	myDocument->LoadFile();

	if (myDocument->Error())
	{
		MessageBox(theHWND, L"XML读取失败", L"错误", 0);
		return false;
	}
	
	try
	{
		TiXmlElement * RootElement = myDocument->RootElement();

		for (TiXmlElement * TestCase = RootElement->FirstChildElement();
				TestCase->NextSiblingElement() != NULL;
				TestCase = TestCase->NextSiblingElement())
		{
			int ID;
			TestCase->QueryIntAttribute("ID", &ID);
			if (ID != SETTING_XML_TESTCASE)
			{
				TestCase->Clear();
				continue;
			}
				

			succeed = true;

			for (TiXmlElement * DataNode = TestCase->FirstChildElement();
				DataNode->NextSiblingElement() != NULL;
				DataNode = DataNode->NextSiblingElement())
			{
				if (strcmp(DataNode->Value(), "Entity") == 0)
				{

				}
				else if (strcmp(DataNode->Value(), "Boundary"))
				{

				}
				else
				{
					
				}
			}
		}
	}
	catch (...)
	{
		myDocument->Clear();
		delete(myDocument);
		MessageBox(theHWND, L"未知错误", L"错误", 0);
		return false;
	}
	

	if (!succeed)
	{
		myDocument->Clear();
		delete(myDocument);
		MessageBox(theHWND, L"没有找到数据节点", L"错误", 0);
		return false;
	}
		
	myDocument->Clear();
	delete(myDocument);

	return true;
}