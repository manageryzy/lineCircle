#include "../stdafx.h"

bool pugiXMLPrase()
{
	pugi::xml_document doc;
	bool succeed = false;

	if (!doc.load_file(strFile))
	{
		MessageBox(theHWND, L"pugixml文件读取失败", L"错误", 0);
		return false;
	}

	for (pugi::xml_node testCase = doc.child("TestRoot").child("TestCase"); testCase != NULL;testCase = testCase.next_sibling())
	{
		if (testCase.attribute("ID").as_int() == SETTING_XML_TESTCASE)
		{
			succeed = true;
			for (pugi::xml_node DataNode = testCase.first_child();
				DataNode != NULL;
				DataNode = DataNode.next_sibling())
			{
				if (strcmp(DataNode.name(), "Entity") == 0)
				{
					if (strcmp(DataNode.attribute("Type").as_string(), "Line") == 0)
					{
						Line * line = new Line;
						sscanf_s(DataNode.child("StartPoint").text().as_string(), "%f,%f", &line->x1, &line->y1);
						sscanf_s(DataNode.child("EndPoint").text().as_string(), "%f,%f", &line->x2, &line->y2);
						lineList.push_back(line);
					}
					else if (strcmp(DataNode.attribute("Type").as_string(), "Circle") == 0)
					{
						Circle * circle = new Circle;
						sscanf_s(DataNode.child("CenterPoint").text().as_string(), "%f,%f", &circle->x, &circle->y);
						sscanf_s(DataNode.child("Radius").text().as_string(), "%f", &circle->r);
						circleList.push_back(circle);
					}
					else
					{
						//异常，无视就好
					}
				}
				else if (strcmp(DataNode.name(), "Boundary") == 0)
				{
					for (pugi::xml_node pointNode = DataNode.first_child();
						pointNode != NULL;
						pointNode = pointNode.next_sibling())
					{
						if (strcmp(pointNode.name(), "Vertex") != 0)
							continue;

						Point * point = new Point;
						sscanf_s(pointNode.text().as_string(), "%f,%f", &point->x, &point->y);

						polygonList.push_back(point);
					}
				}
				else
				{
					//异常，直接忽略好了
				}
			}
		}
	}

	if (!succeed)
	{
		MessageBox(theHWND, L"pugixml无法找到数据节点", L"错误", 0);
		return false;
	}

	return true;
}