#pragma once

#include "matabee360.h"
#include "afxwin.h"

// CReportDlg 对话框

class CReportDlg : public CDialog
{
	DECLARE_DYNAMIC(CReportDlg)

public:
	CReportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReportDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_UP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CListBox m_urlList;
};
