
// MyDBMSDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MyDBMS.h"
#include "MyDBMSDlg.h"
#include "afxdialogex.h"
#include "DataStructureModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DB database;

// CMyDBMSDlg 对话框



CMyDBMSDlg::CMyDBMSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYDBMS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDBMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
	DDX_Control(pDX, IDC_LIST_TABLES, m_tableList);
}

BEGIN_MESSAGE_MAP(CMyDBMSDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SUBMIT, &CMyDBMSDlg::OnBnClickedButtonSubmit)
	ON_LBN_DBLCLK(IDC_LIST_TABLES, &CMyDBMSDlg::OnLbnDblclkListTables)
END_MESSAGE_MAP()


// CMyDBMSDlg 消息处理程序

BOOL CMyDBMSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码

	m_listResult.ModifyStyle(0, LVS_REPORT);
	m_listResult.SetExtendedStyle(m_listResult.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void ShowTable(std::vector<std::vector<std::string>> data) {

	auto dlg = (CMyDBMSDlg*)CMyDBMSDlg::FromHandle(AfxGetMainWnd()->m_hWnd);
	dlg->m_listResult.DeleteAllItems();
	while (dlg->m_listResult.DeleteColumn(0));
	CRect rect;
	dlg->m_listResult.GetClientRect(rect);
	for (size_t i = 0; i < data[0].size(); i++)
	{
		dlg->m_listResult.InsertColumn(i, CA2T(data[0][i].c_str()));
		dlg->m_listResult.SetColumnWidth(i, rect.Width() / data[0].size());
	}
	for (size_t i = 1; i < data.size(); i++)
	{
		dlg->m_listResult.InsertItem(0, CA2T(data[i][0].c_str()));
		for (size_t j = 1; j < data[0].size(); j++)
		{
			dlg->m_listResult.SetItemText(0, j, CA2T(data[i][j].c_str()));
		}
	}

}

void ShowInfo(std::string info) {
	CString wInfo;
	wInfo = CA2T(info.c_str());
	SetDlgItemText(AfxGetMainWnd()->m_hWnd, IDC_EDIT_OUTPUT, wInfo);
}

void ShowList(std::vector<std::string> tables) {
	auto dlg = (CMyDBMSDlg*)CMyDBMSDlg::FromHandle(AfxGetMainWnd()->m_hWnd);
	dlg->m_tableList.ResetContent();
	for (size_t i = 0; i < tables.size(); i++)
	{
		dlg->m_tableList.AddString(CA2T(tables[i].c_str()));
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyDBMSDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyDBMSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyDBMSDlg::OnBnClickedButtonSubmit()
{
	SetDlgItemText(IDC_EDIT_OUTPUT, L"");
	CString input;
	GetDlgItemText(IDC_EDIT_SQL, input);
	std::string sql;
	sql = CT2A(input);
	auto sqlSingle = split(sql, "\r\n");
	for (size_t i = 0; i < sqlSingle.size(); i++)
	{
		SQL(database, sqlSingle[i]);
	}
}


void CMyDBMSDlg::OnLbnDblclkListTables()
{

	CString strText;
	int nIndex = m_tableList.GetCurSel();
	if (nIndex == -1) {
		return;
	}
	m_tableList.GetText(nIndex, strText);
	std::string tablename;
	tablename = CT2A(strText);

	SQL(database, "SELECT * FROM " + tablename + ";");
}
