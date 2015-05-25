
// configEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "configEdit.h"
#include "configEditDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CconfigEditDlg 对话框



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


// CconfigEditDlg 消息处理程序

BOOL CconfigEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	loadIniSetting();

	m_testcase = SETTING_XML_TESTCASE;

	m_drawthread = SETTING_DRAW_THREAD;
	m_cutting_thread = SETTING_CUTTING_THREAD;

	m_memool_num = SETTING_MEMPOOL_NUM;
	m_mempool_size = SETTING_MEMPOOL_SIZE;
	m_mempool_t_num = SETTING_THREAD_MEMPOOL_NUM;
	m_mempool_t_size = SETTING_THREAD_MEMPOOL_SIZE;

	{
		COMBOBOXEXITEM  Item[3];
		Item[0].mask = CBEIF_TEXT;
		Item[0].pszText = _T("0.tinyxml");
		Item[0].iItem = 0;
		Item[1].mask = CBEIF_TEXT;
		Item[1].pszText = _T("1.状态机");
		Item[1].iItem = 1;
		Item[2].mask = CBEIF_TEXT;
		Item[2].pszText = _T("2.pugixml");
		Item[2].iItem = 2;
		m_xml_mode.InsertItem(&Item[0]);
		m_xml_mode.InsertItem(&Item[1]);
		m_xml_mode.InsertItem(&Item[2]);
	}
	m_xml_mode.SetCurSel(SETTING_XML_MODE);


	{
		COMBOBOXEXITEM  Item[2];
		Item[0].mask = CBEIF_TEXT;
		Item[0].pszText = _T("0.关闭");
		Item[0].iItem = 0;
		Item[1].mask = CBEIF_TEXT;
		Item[1].pszText = _T("1.启用");
		Item[1].iItem = 1;
		m_opcache.InsertItem(&Item[0]);
		m_opcache.InsertItem(&Item[1]);
	}
	m_opcache.SetCurSel(SETTING_XML_CACHE);

	{
		COMBOBOXEXITEM  Item[3];
		Item[0].mask = CBEIF_TEXT;
		Item[0].pszText = _T("1.GDI");
		Item[0].iItem = 0;
		Item[1].mask = CBEIF_TEXT;
		Item[1].pszText = _T("2.内存绘图");
		Item[1].iItem = 1;
		Item[2].mask = CBEIF_TEXT;
		Item[2].pszText = _T("3.内存GDI");
		Item[2].iItem = 2;
		m_drawmode.InsertItem(&Item[0]);
		m_drawmode.InsertItem(&Item[1]);
		m_drawmode.InsertItem(&Item[2]);
	}
	m_drawmode.SetCurSel(SETTING_DRAW_MODE-1);

	UpdateData(false);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CconfigEditDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CconfigEditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CconfigEditDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	SETTING_XML_TESTCASE = m_testcase;

	SETTING_DRAW_THREAD = m_drawthread;
	SETTING_CUTTING_THREAD = m_cutting_thread;

	SETTING_MEMPOOL_NUM = m_memool_num;
	SETTING_MEMPOOL_SIZE = m_mempool_size;
	SETTING_THREAD_MEMPOOL_NUM = m_mempool_t_num;
	SETTING_THREAD_MEMPOOL_SIZE = m_mempool_t_size;

	SETTING_XML_MODE = m_xml_mode.GetCurSel();
	SETTING_XML_CACHE = m_opcache.GetCurSel();
	SETTING_DRAW_MODE = m_drawmode.GetCurSel()+1;

	saveIniSetting();

	MessageBox(L"有些更改需要在重新启动程序才能生效", L"警告");

	CDialog::OnOK();
}
