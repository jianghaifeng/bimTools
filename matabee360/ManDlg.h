#pragma once
#include "stdafx.h"
#include "matabee360.h"
#include "afxwin.h"
#include "GlobalApi.h"
#include <vector>
using namespace std;

// CManDlg 对话框

class CManDlg : public CDialog
{
	DECLARE_DYNAMIC(CManDlg)

public:
	CManDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CManDlg();

	CString m_curDir;
	vector<CString> m_listDir;

// 对话框数据
	enum { IDD = IDD_DIALOG_MAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_prjList;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
//	afx_msg void OnLbnSelchangeListPrj();
//	afx_msg void OnBnClickedButtonPrev();
//	afx_msg void OnLbnSelchangeListPrj();
	afx_msg void OnLbnDblclkListPrj();
	afx_msg void OnBnClickedButton2();
};
