#pragma once
#include "stdafx.h"
#include "matabee360.h"
#include "Config.h"
#include "afxwin.h"
// CConfigDlg 对话框

class CConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CConfig* pCfg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfigDlg();

	CConfig* m_pCfg;

// 对话框数据
	enum { IDD = IDD_DIALOG_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	BOOL m_checkImg;
	int m_urlmask;
	BOOL m_urlcheck1;
	BOOL m_urlcheck2;
	BOOL m_urlcheck3;
};
