#pragma once

#include "matabee360.h"
#include "afxwin.h"

// CReportDlg �Ի���

class CReportDlg : public CDialog
{
	DECLARE_DYNAMIC(CReportDlg)

public:
	CReportDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReportDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_UP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CListBox m_urlList;
};
