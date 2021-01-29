// PromptDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "matabee360.h"
#include "PromptDlg.h"
#include "afxdialogex.h"


// CPromptDlg �Ի���

IMPLEMENT_DYNAMIC(CPromptDlg, CDialog)

CPromptDlg::CPromptDlg(const TCHAR* text, const TCHAR* title, CWnd* pParent, /*=NULL*/ BOOL isRetry)
	: CDialog(CPromptDlg::IDD, pParent)
{
	stext = text;
	stitle = title;

	pa = pParent;
	this->isRetry = isRetry;
}

CPromptDlg::~CPromptDlg()
{
}

void CPromptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPromptDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CPromptDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CPromptDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDNO, &CPromptDlg::OnBnClickedNo)
END_MESSAGE_MAP()


// CPromptDlg ��Ϣ�������


BOOL CPromptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rct,rct1;
	::GetWindowRect(pa->m_hWnd, rct);
	GetWindowRect(rct1);
	MoveWindow(rct.left+(rct.Width()-rct1.Width())/2, 
		rct.top+(rct.Height()-rct1.Height())/2, rct1.Width(), rct1.Height());

	SetWindowText(stitle);
	SetDlgItemText(IDC_STATIC_1, stext);
	SetDlgItemText(IDOK, theApp.pLang->m_rosetta[s_ok]);
	SetDlgItemText(IDNO, theApp.pLang->m_rosetta[s_retry]);

	if(isRetry) {
		GetDlgItem(IDNO)->ShowWindow(SW_SHOW);
	} else {
		GetDlgItem(IDNO)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPromptDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// CDialog::OnCancel();
}


void CPromptDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
	int i = 3;
}


void CPromptDlg::OnBnClickedNo() {
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}
