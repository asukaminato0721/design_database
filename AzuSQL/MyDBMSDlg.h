
// MyDBMSDlg.h: 头文件
//

#pragma once
#include <string>
#include <vector>
void ShowTable(std::vector<std::vector<std::string>> data);
void ShowInfo(std::string info);
void ShowList(std::vector<std::string> list);
// CMyDBMSDlg 对话框
class CMyDBMSDlg : public CDialogEx
{
	// 构造
public:
	CMyDBMSDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYDBMS_DIALOG };
#endif

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
	CListCtrl m_listResult;
	afx_msg void OnBnClickedButtonSubmit();
	CListBox m_tableList;
};
