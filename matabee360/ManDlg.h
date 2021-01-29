#pragma once
#include "stdafx.h"
#include "matabee360.h"
#include "afxwin.h"
#include "GlobalApi.h"
#include <vector>
using namespace std;

// CManDlg �Ի���

class CManDlg : public CDialog
{
	DECLARE_DYNAMIC(CManDlg)

public:
	CManDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CManDlg();

	CString m_curDir;
	vector<CString> m_listDir;

// �Ի�������
	enum { IDD = IDD_DIALOG_MAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
