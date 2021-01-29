#pragma once

class CConfig
{
// data members
public:
	TCHAR m_rp_name[32];
	TCHAR m_rp_shop[64];
	CString m_sFile;
	TCHAR m_user[16];
	TCHAR m_passwd[16];
	int  m_urlIndex;
	TCHAR m_scheme[16];
	TCHAR m_site[64];
	TCHAR m_url[3][256]; // two urls can be chosen by user
	bool m_commit_img;
	bool m_commit_slice;
	//cl 2015-11-22
	int m_report_timeout;
	int m_report_trycount;
	int m_http_port;
	int m_https_port;
	//cl end

	void ReadOut();
	void WriteBack();

public:
	CConfig(void);
	~CConfig(void);
};

