
// configEditDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CconfigEditDlg �Ի���
class CconfigEditDlg : public CDialog
{
// ����
public:
	CconfigEditDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CONFIGEDIT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
