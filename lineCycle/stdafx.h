// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//本地化头文件
#include <locale.h>

//文件打开对话框头文件
#include <Commdlg.h>

//tinyxml
#include "../tinyxml/tinyxml.h"

//STL
#include <vector>
using std::vector;

//项目头文件
#include "setting.h"
#include "math.h"
#include "graph.h"
#include "./common/global.h"

//XML部分头文件
#include "./xml/minXml.h"
#include "./xml/tinyXMLPrase.h"

//绘图部分头文件
#include "./draw/drawGDI.h"
#include "./draw/drawMemory.h"
#include "./draw/drawMemoryGDI.h"
#include "./draw/drawOpenGL.h"