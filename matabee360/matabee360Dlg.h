
// matabee360Dlg.h : ͷ�ļ�
//

#pragma once
#include "PicView.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "BtnST.h"
#include "GlobalApi.h"
#include "TextProgressCtrl.h"

/* prj.log:
 *URL[ ]
 *name
 *datetime
 *shopname
 *OK/ERR
 */

// Cmatabee360Dlg �Ի���
class Cmatabee360Dlg : public CDialogEx
{
// ����
public:
	Cmatabee360Dlg(CWnd* pParent = NULL);	// ��׼���캯��

	void PrintLog(const TCHAR* brief, const TCHAR* spec = NULL, int lv = 0);
	void UpdateProgress(int percent);
	void UpdateProgressText(const TCHAR* description);
	bool  m_bPrjCreated;			  /* project created flag */
	TCHAR m_sPrjImg[STR_LONG_LEN];    /* ..\Project\(repoter)\(date_time)\images */
	TCHAR m_sPrjWeb[STR_LONG_LEN];    /* ..\Project\(repoter)\(date_time)\web */
	TCHAR m_sPrjTpl[STR_LONG_LEN];    /* ..\Template */
	TCHAR m_sPrjDt[STR_DTIME_LEN];    /* (date_time) */
	TCHAR m_sPrjLog[STR_LONG_LEN];    /* ..\Project\(repoter)\(date_time)\prj.log*/
	
	BOOL isOK();
	//void OpenProject(TCHAR *dt, TCHAR* user);
	void CreateNewProject();
	void LockButtons();
	void UnLockButtons();

// �Ի�������
	enum { IDD = IDD_MATABEE360_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	CPicView *pView;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnRmv();
	afx_msg void OnBnClickedBtnRpt();
	afx_msg void OnBnClickedBtnSettings();
	CTextProgressCtrl m_progress;
	//CProgressCtrl m_progress;
	CButtonST  m_btnS;
	CButtonST  m_btnA;
	CButtonST  m_btnR;
	CButtonST  m_btnC;
	CButtonST  m_btnM;
	CButtonST  m_btnD;
	CButtonST  m_btnE;
	afx_msg void OnBnClickedBtnManage();
//	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	
	afx_msg void OnBnClickedBtnClean();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedBtnSlice();
	
};
