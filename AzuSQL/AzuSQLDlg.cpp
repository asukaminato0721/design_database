#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "framework.h"
#include "AzuSQL.h"
#include "AzuSQLDlg.h"
#include "afxdialogex.h"
#include "./DataStructureModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DB database;


CAzuSQLDlg::CAzuSQLDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AZUSQL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAzuSQLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAzuSQLDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXEC, &CAzuSQLDlg::OnBnClickedButtonExec)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CAzuSQLDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()



BOOL CAzuSQLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAzuSQLDlg::OnPaint()
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

HCURSOR CAzuSQLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAzuSQLDlg::OnBnClickedButtonExec()
{
	CString input;
	GetDlgItemText(IDC_EDIT_SQL, input);
	std::string sql = CT2A(input);

	std::string result = SQL(database, sql);
	CString r = CA2T(result.c_str());
	SetDlgItemText(IDC_EDIT_RESULT, r);
	return;
}


void CAzuSQLDlg::OnBnClickedButtonClear()
{
	SetDlgItemText(IDC_EDIT_RESULT, L"");
	return;
}
