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
				TestCase != NULL;
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
				DataNode != NULL;
				DataNode = DataNode->NextSiblingElement())
			{
				if (strcmp(DataNode->Value(), "Entity") == 0)
				{
					if (strcmp(DataNode->Attribute("Type"), "Line") == 0)
					{
						Line line;

						for (TiXmlElement * pointNode = DataNode->FirstChildElement();
							pointNode != NULL;
							pointNode = pointNode->NextSiblingElement())
						{
							if (strcmp(pointNode->Value(), "StartPoint") == 0)
							{
								sscanf_s(pointNode->GetText(), "%f,%f", &line.x1, &line.y1);
							}
							else if (strcmp(pointNode->Value(), "EndPoint") == 0)
							{
								sscanf_s(pointNode->GetText(), "%f,%f", &line.x2, &line.y2);
							}
						}

						lineList.push_back(line);
					}
					else if (strcmp(DataNode->Attribute("Type"), "Circle") == 0)
					{
						Circle circle;

						for (TiXmlElement * pointNode = DataNode->FirstChildElement();
							pointNode != NULL;
							pointNode = pointNode->NextSiblingElement())
						{
							if (strcmp(pointNode->Value(), "CenterPoint") == 0)
							{
								sscanf_s(pointNode->GetText(), "%f,%f", &circle.x, &circle.y);
							}
							else if (strcmp(pointNode->Value(), "Radius") == 0)
							{
								sscanf_s(pointNode->GetText(), "%f", &circle.r);
							}
						}

						circleList.push_back(circle);
					}
				}
				else if (strcmp(DataNode->Value(), "Boundary") == 0)
				{
					for (TiXmlElement * pointNode = DataNode->FirstChildElement();
						pointNode != NULL;
						pointNode = pointNode->NextSiblingElement())
					{
						if (strcmp(pointNode->Value(), "Vertex") != 0)
							continue;

						Point point;
						sscanf_s(pointNode->GetText(), "%f,%f", &point.x, &point.y);
						
						polygonList.push_back(point);
					}
				}
				else
				{
					//异常，还是忽略了吧
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