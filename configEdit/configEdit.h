
// configEdit.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CconfigEditApp: 
// �йش����ʵ�֣������ configEdit.cpp
//

class CconfigEditApp : public CWinApp
{
public:
	CconfigEditApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CconfigEditApp theApp;