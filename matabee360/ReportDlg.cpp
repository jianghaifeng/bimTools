// ReportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "matabee360.h"
#include "ReportDlg.h"
#include "afxdialogex.h"


// CReportDlg 对话框

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


// CReportDlg 消息处理程序


BOOL CReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

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
	// 异常: OCX 属性页应返回 FALSE
}


void CReportDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
	GetDlgItemText(IDC_EDIT_SHOP, theApp.pCfg->m_rp_shop, 64);
	CDialog::OnOK();
}
