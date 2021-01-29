// ManDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "matabee360.h"
#include "ManDlg.h"
#include "afxdialogex.h"


// CManDlg 对话框

IMPLEMENT_DYNAMIC(CManDlg, CDialog)

CManDlg::CManDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManDlg::IDD, pParent)
{

}

CManDlg::~CManDlg()
{
}

void CManDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRJ, m_prjList);
}


BEGIN_MESSAGE_MAP(CManDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CManDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CManDlg::OnBnClickedCancel)
//	ON_LBN_SELCHANGE(IDC_LIST_PRJ, &CManDlg::OnLbnSelchangeListPrj)
//  ON_BN_CLICKED(IDC_BUTTON_PREV, &CManDlg::OnBnClickedButtonPrev)
//  ON_LBN_SELCHANGE(IDC_LIST_PRJ, &CManDlg::OnLbnSelchangeListPrj)
    ON_LBN_DBLCLK(IDC_LIST_PRJ, &CManDlg::OnLbnDblclkListPrj)
	ON_BN_CLICKED(IDC_BUTTON2, &CManDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CManDlg 消息处理程序


BOOL CManDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString PrjDir;

	get_root_dir(PrjDir);
	PrjDir += _T("Project\\*.*");

    CFileFind tempFind;
    BOOL bFound=tempFind.FindFile(PrjDir);
    CString strTmp,strShow;
	int index = 0;
    while(bFound)
    {
        bFound=tempFind.FindNextFile();
        if(tempFind.IsDots()) continue;
        if(tempFind.IsDirectory())
        {
			strTmp=tempFind.GetFilePath();
			
			if (_tcslen(theApp.pDlg->m_sPrjDt)>0 && strTmp.Find(theApp.pDlg->m_sPrjDt)>=0)
				continue;
			
			strTmp.Append(_T("\\prj.log"));
			FILE *fp;
			fp = _wfopen(strTmp, _T("rt,ccs=UNICODE"));
			TCHAR c[STR_LONG_LEN];
			//fread(c, 128, 1, fp);
			if (!fp) {
				//swprintf(c, _T("URL[ ] %s"), tempFind.GetFileName());
				//m_prjList.InsertString(0,c);
				continue;
			}
			else {
				fgetws(c, STR_LONG_LEN, fp);//url
				strShow.Format(_T("%s "),c);

				fgetws(c, STR_LONG_LEN, fp);//name
				strShow.Append(c);
				strShow.Append(_T("_"));

				fgetws(c, STR_LONG_LEN, fp);//datetime
				strShow.Append(c);

				//shop
				if (fgetws(c, STR_LONG_LEN, fp) && _tcslen(c) > 1) {
					strShow.Append(_T("_"));
					strShow.Append(c);
				}

				//OK/ERR
				if (fgetws(c, STR_LONG_LEN, fp) && _tcslen(c) > 1) {
					strShow.Append(_T(" "));
					strShow.Append(c);
				}
				
				m_prjList.InsertString(index,strShow);
				m_listDir.push_back(tempFind.GetFilePath());
				index++;
				fclose(fp);
			}
        }
    }
    tempFind.Close();

	SetWindowText(theApp.pLang->m_rosetta[s_manage]);

	SetDlgItemText(IDOK, theApp.pLang->m_rosetta[s_load]);
	SetDlgItemText(IDC_BUTTON2, theApp.pLang->m_rosetta[s_preview]);
	SetDlgItemText(IDCANCEL, theApp.pLang->m_rosetta[s_cancel]);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CManDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString dir;
	int n = m_prjList.GetCurSel();

	if (n < 0 || n >= m_listDir.size())
		return;

	m_curDir = m_listDir[n];
	CDialog::OnOK();
}


void CManDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

void CManDlg::OnLbnDblclkListPrj()
{
	CString dir;
	int n = m_prjList.GetCurSel();

	if (n < 0 || n >= m_listDir.size())
		return;

	dir = m_listDir[n];
	dir.Append(_T("\\web\\index.html"));

	if (_waccess(dir, 0) != -1) {
		ShellExecute(::GetDesktopWindow(),_T("open"),dir,NULL,NULL,SW_MAXIMIZE);
	}
}

void CManDlg::OnBnClickedButton2()
{
	OnLbnDblclkListPrj();
}
