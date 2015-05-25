
// configEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "configEdit.h"
#include "configEditDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CconfigEditDlg �Ի���



CconfigEditDlg::CconfigEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CconfigEditDlg::IDD, pParent)
	, m_testcase(0)
	, m_drawthread(0)
	, m_memool_num(0)
	, m_mempool_size(0)
	, m_mempool_t_num(0)
	, m_cutting_thread(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CconfigEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTING_XML_MODE, m_xml_mode);
	DDX_Control(pDX, IDC_SETTING_OPCACHE, m_opcache);
	DDX_Text(pDX, IDC_SETTING_TESTCASE, m_testcase);
	DDX_Control(pDX, IDC_SETTING_DRAWMODE, m_drawmode);
	DDX_Text(pDX, IDC_SETTING_DRAW_THREAD, m_drawthread);
	DDV_MinMaxInt(pDX, m_drawthread, 0, 64);
	DDX_Text(pDX, IDC_SETTING_MEMPOOL_NUM, m_memool_num);
	DDX_Text(pDX, IDC_SETTING_MEMPOOL_SIZE, m_mempool_size);
	DDX_Text(pDX, IDC_SETTING_THREAD_MEMPOOL_SIZE, m_mempool_t_size);
	DDX_Text(pDX, IDC_SETTING_THREAD_MEMPOOL_NUM, m_mempool_t_num);
	DDX_Text(pDX, IDC_SETTING_CUTTING_THREAD, m_cutting_thread);
	DDV_MinMaxInt(pDX, m_cutting_thread, 0, 64);
}

BEGIN_MESSAGE_MAP(CconfigEditDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CconfigEditDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CconfigEditDlg ��Ϣ�������

BOOL CconfigEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	UpdateData(true);

	loadIniSetting();

	m_testcase = SETTING_XML_TESTCASE;

	m_drawthread = SETTING_DRAW_THREAD;
	m_cutting_thread = SETTING_CUTTING_THREAD;

	m_memool_num = SETTING_MEMPOOL_NUM;
	m_mempool_size = SETTING_MEMPOOL_SIZE;
	m_mempool_t_num = SETTING_THREAD_MEMPOOL_NUM;
	m_mempool_t_size = SETTING_THREAD_MEMPOOL_SIZE;

	m_xml_mode.AddString(L"0.tinyxml");
	m_xml_mode.AddString(L"1.״̬��");
	m_xml_mode.AddString(L"2.pugixml");
	m_xml_mode.SetCurSel(SETTING_XML_MODE);

	UpdateData(false);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CconfigEditDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CconfigEditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CconfigEditDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);

	SETTING_XML_TESTCASE = m_testcase;

	SETTING_DRAW_THREAD = m_drawthread;
	SETTING_CUTTING_THREAD = m_cutting_thread;

	SETTING_MEMPOOL_NUM = m_memool_num;
	SETTING_MEMPOOL_SIZE = m_mempool_size;
	SETTING_THREAD_MEMPOOL_NUM = m_mempool_t_num;
	SETTING_THREAD_MEMPOOL_SIZE = m_mempool_t_size;

	saveIniSetting();

	CDialog::OnOK();
}
