// ConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "matabee360.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"

// CConfigDlg 对话框

IMPLEMENT_DYNAMIC(CConfigDlg, CDialog)

CConfigDlg::CConfigDlg(CConfig* pCfg, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
	, m_checkImg(FALSE)
	, m_urlmask(0)
	, m_urlcheck1(false)
	, m_urlcheck2(false)
	, m_urlcheck3(false)
{
	m_pCfg = pCfg;
}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_IMG, m_checkImg);
	DDX_Check(pDX, IDC_CHECK_URL1, m_urlcheck1);
	DDX_Check(pDX, IDC_CHECK_URL2, m_urlcheck2);
	DDX_Check(pDX, IDC_CHECK_URL3, m_urlcheck3);

	// 2015-06-17
	//DDX_Radio(pDX, IDC_RADIO1, m_rdurl);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigDlg 消息处理程序


void CConfigDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pCfg != NULL) {
		this->UpdateData(TRUE);

		GetDlgItemText(IDC_EDIT_REPT_NAME, m_pCfg->m_rp_name, 32);

		if (_tcslen(m_pCfg->m_rp_name) == 0)
		{
			CPromptDlg dlg(theApp.pLang->m_rosetta[s_s_req_uname], theApp.pLang->m_rosetta[s_prompt], this);
			dlg.DoModal();
			return;
		}

		GetDlgItemText(IDC_EDIT_URL1, m_pCfg->m_url[0], 256);
		GetDlgItemText(IDC_EDIT_URL2, m_pCfg->m_url[1], 256);

		//Add: 2015-06-17
		GetDlgItemText(IDC_EDIT_URL3, m_pCfg->m_url[2], 256);

		m_pCfg->m_commit_img = m_checkImg;
		m_pCfg->m_commit_slice = TRUE;
		m_urlmask = (m_urlcheck1 | (m_urlcheck2 << 1) | (m_urlcheck3 << 2));
		m_pCfg->m_urlIndex = m_urlmask;
	}
	
	CDialog::OnOK();
}

BOOL CConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_pCfg == NULL) return FALSE;

	SetDlgItemText(IDC_EDIT_REPT_NAME, m_pCfg->m_rp_name);

	SetDlgItemText(IDC_EDIT_URL1, m_pCfg->m_url[0]);
	SetDlgItemText(IDC_EDIT_URL2, m_pCfg->m_url[1]);

	//Add: 2015-06-17
	SetDlgItemText(IDC_EDIT_URL3, m_pCfg->m_url[2]);

	m_checkImg = m_pCfg->m_commit_img;
	m_urlmask = m_pCfg->m_urlIndex;
	
	m_urlcheck1 = ((m_urlmask & 1) != 0);
	m_urlcheck2 = ((m_urlmask & 2) != 0);
	m_urlcheck3 = ((m_urlmask & 4) != 0);

	this->SetWindowText(theApp.pLang->m_rosetta[s_config]);
	this->UpdateData(FALSE);

	SetDlgItemText(IDC_STATIC_1, theApp.pLang->m_rosetta[s_s_reporter_info]);
	SetDlgItemText(IDC_STATIC_2, theApp.pLang->m_rosetta[s_s_buzzsaw_settings]);
	SetDlgItemText(IDC_STATIC_3, theApp.pLang->m_rosetta[s_site]);
	SetDlgItemText(IDC_STATIC_4, theApp.pLang->m_rosetta[s_reporter]);

	SetDlgItemText(IDC_CHECK_IMG, theApp.pLang->m_rosetta[s_s_upload_imgs]);
	// 2015-06-17
	//SetDlgItemText(IDC_RADIO1, theApp.pLang->m_rosetta[s_default]);
	//SetDlgItemText(IDC_RADIO2, theApp.pLang->m_rosetta[s_default]);
	SetDlgItemText(IDC_CHECK_URL1, theApp.pLang->m_rosetta[s_default]);
	SetDlgItemText(IDC_CHECK_URL2, theApp.pLang->m_rosetta[s_default]);
	SetDlgItemText(IDC_CHECK_URL3, theApp.pLang->m_rosetta[s_default]);

	SetDlgItemText(IDOK, theApp.pLang->m_rosetta[s_ok]);
	SetDlgItemText(IDCANCEL, theApp.pLang->m_rosetta[s_cancel]);
	SetDlgItemText(IDC_STATIC_DESC, theApp.pLang->m_rosetta[s_s_desc]);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
