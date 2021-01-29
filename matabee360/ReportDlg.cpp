// ReportDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "matabee360.h"
#include "ReportDlg.h"
#include "afxdialogex.h"


// CReportDlg �Ի���

IMPLEMENT_DYNAMIC(CReportDlg, CDialog)

CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReportDlg::IDD, pParent)
{

}

CReportDlg::~CReportDlg()
{
}

void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Control(pDX, IDC_LIST_URLS, m_urlList);
}


BEGIN_MESSAGE_MAP(CReportDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CReportDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CReportDlg ��Ϣ�������


BOOL CReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	int index = 0;
	TCHAR stmp[256];
	TCHAR sscheme[16], surl[64], dir[64];

	for (int i=0; i<3; i++)
	{
		if (theApp.pCfg->m_urlIndex & (1<<i)) 
		{
			_tcscpy(surl, theApp.pCfg->m_site);
			parse_url(theApp.pCfg->m_url[i], sscheme, surl, dir);
			swprintf(stmp, _T("%s%s"), surl, dir);
			m_urlList.InsertString(index++, stmp);
		}
	}

	SetWindowText(theApp.pLang->m_rosetta[s_report]);

	SetDlgItemText(IDC_STATIC_1, theApp.pLang->m_rosetta[s_site]);
	SetDlgItemText(IDC_STATIC_2, theApp.pLang->m_rosetta[s_shopname]);

	SetDlgItemText(IDOK, theApp.pLang->m_rosetta[s_ok]);
	SetDlgItemText(IDCANCEL, theApp.pLang->m_rosetta[s_cancel]);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CReportDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(FALSE);
	GetDlgItemText(IDC_EDIT_SHOP, theApp.pCfg->m_rp_shop, 64);
	CDialog::OnOK();
}
