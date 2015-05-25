
// configEditDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CconfigEditDlg 对话框
class CconfigEditDlg : public CDialog
{
// 构造
public:
	CconfigEditDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CONFIGEDIT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBoxEx m_xml_mode;
	CComboBoxEx m_opcache;
	int m_testcase;
	CComboBoxEx m_drawmode;
	int m_drawthread;
	int m_memool_num;
	int m_mempool_size;
	int m_mempool_t_size;
	int m_mempool_t_num;
	afx_msg void OnBnClickedOk();
	int m_cutting_thread;
};
