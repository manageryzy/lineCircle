// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//���ػ�ͷ�ļ�
#include <locale.h>

//�ļ��򿪶Ի���ͷ�ļ�
#include <Commdlg.h>

//tinyxml
#include "../tinyxml/tinyxml.h"

//STL
#include <vector>
using std::vector;

//��Ŀͷ�ļ�
#include "setting.h"
#include "math.h"
#include "graph.h"
#include "./common/global.h"

//XML����ͷ�ļ�
#include "./xml/minXml.h"
#include "./xml/tinyXMLPrase.h"

//��ͼ����ͷ�ļ�
#include "./draw/drawGDI.h"
#include "./draw/drawMemory.h"
#include "./draw/drawMemoryGDI.h"
#include "./draw/drawOpenGL.h"