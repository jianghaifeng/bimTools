#pragma once
#include "stdafx.h"
#include "matabee360.h"
#include "Config.h"
#include "afxwin.h"
// CConfigDlg �Ի���

class CConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CConfig* pCfg, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfigDlg();

	CConfig* m_pCfg;

// �Ի�������
	enum { IDD = IDD_DIALOG_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
