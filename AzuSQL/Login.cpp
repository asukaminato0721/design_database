// Login.cpp: 实现文件
//

#include "pch.h"
#include "MyDBMS.h"
#include "Login.h"
#include "afxdialogex.h"
#include "DataStructureModule.h"

// Login 对话框

IMPLEMENT_DYNAMIC(Login, CDialogEx)

Login::Login(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

Login::~Login()
{
}

void Login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Login, CDialogEx)
	ON_BN_CLICKED(IDOK, &Login::OnBnClickedOk)
END_MESSAGE_MAP()


// Login 消息处理程序


void Login::OnBnClickedOk()
{
	CString name;
	GetDlgItemText(IDC_EDIT_USERNAME, name);
	std::string nameStr = CT2A(name);

	CString passwd;
	GetDlgItemText(IDC_EDIT_USERNAME, passwd);
	std::string passwdStr = CT2A(name);

	auto res = UserCompare(nameStr, passwdStr);

}
