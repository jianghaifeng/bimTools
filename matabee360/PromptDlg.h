#pragma once


// CPromptDlg 对话框

class CPromptDlg : public CDialog
{
	DECLARE_DYNAMIC(CPromptDlg)

public:
	CPromptDlg(const TCHAR* text, const TCHAR* title, CWnd* pParent = NULL, BOOL isRetry = false);   // 标准构造函数
	virtual ~CPromptDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROMPT };

private:
	CWnd* pa;
	CString stext;
	CString stitle;
	BOOL isRetry;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRetry();
	afx_msg void OnBnClickedNo();
};
