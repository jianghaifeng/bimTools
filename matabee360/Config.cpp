#include "StdAfx.h"
#include "Config.h"
#include "ConfigDlg.h"
#include "GlobalApi.h"
#include <string.h>
#include <io.h>

CConfig::CConfig(void)
{
	get_exec_dir(m_sFile);
	m_sFile.Append( _T("Matabee360.ini"));//3-Force360:force360.ini  Matabee360:matabee360.ini

	// default data value
	_tcscpy(m_rp_name,  _T(""));
	_tcscpy(m_rp_shop,  _T(""));
	_tcscpy(m_user,     _T("systembuz"));
	_tcscpy(m_passwd,   _T("7048Matabee"));

	// 2015-06-17 
	_tcscpy(m_scheme,   _T("http"));
	_tcscpy(m_site,     _T("webdav.buzzsaw.com"));
	_tcscpy(m_url[0],   _T("SEMS/pano"));//1-Force360:checkup-hs/pano   Matabee360:SEMS/pano
	_tcscpy(m_url[1],   _T("handybuz/pano"));//2-Force360:force-lcm/pano Matabee360:handybuz/pano
	_tcscpy(m_url[2],   _T(""));
	//_tcscpy(m_url[0],   _T("ap-webdav.buzzsaw.com/SEMS/pano"));
	//_tcscpy(m_url[1],   _T("webdav.buzzsaw.com/handybuz/pano"));

	m_urlIndex = 2;
	m_commit_img = false;
	m_commit_slice = true;
	//cl 2015-11-22
	m_report_timeout = 200;
	m_report_trycount = 5;
	m_http_port = 80;
	m_https_port = 443;
	//cl end

	if (_waccess(m_sFile, 0) != -1) {
		ReadOut();
	}
	else {
		CConfigDlg dlg(this);
		if (dlg.DoModal() == IDOK) {
			WriteBack();
		}
	}
}

void CConfig::ReadOut()
{
	// Read config from file
	GetPrivateProfileString(_T("Config"), _T("repoter"),   m_rp_name, m_rp_name, 32,  m_sFile);
	//GetPrivateProfileString(_T("Config"), _T("user"),	   m_user,	  m_user,	 16,  m_sFile);
	//GetPrivateProfileString(_T("Config"), _T("password"),  m_passwd,  m_passwd,	 16,  m_sFile);
	GetPrivateProfileString(_T("Config"), _T("default_scheme"),	  m_scheme,  m_scheme,	 16, m_sFile);
	GetPrivateProfileString(_T("Config"), _T("default_site"),     m_site,    m_site,	 64, m_sFile);
	GetPrivateProfileString(_T("Config"), _T("url_1"),	   m_url[0],  m_url[0],	 256, m_sFile);
	GetPrivateProfileString(_T("Config"), _T("url_2"),	   m_url[1],  m_url[1],	 256, m_sFile);
	//Add: 2015-06-17
	GetPrivateProfileString(_T("Config"), _T("url_3"),	   m_url[2],  m_url[2],	 256, m_sFile);

	m_urlIndex     = GetPrivateProfileInt(_T("Config"), _T("default_url"),    0, m_sFile);
	m_commit_img   = GetPrivateProfileInt(_T("Config"), _T("upload_image"),   0, m_sFile)==0 ? false : true ;
	m_commit_slice = GetPrivateProfileInt(_T("Config"), _T("upload_webfile"), 0, m_sFile)==0 ? false : true;
	//cl 2015-11-22
	m_report_timeout = GetPrivateProfileInt(_T("Config"), _T("report_timeout"), m_report_timeout, m_sFile);
	m_report_trycount = GetPrivateProfileInt(_T("Config"), _T("report_trycount"), m_report_trycount, m_sFile);
	m_http_port = GetPrivateProfileInt(_T("Config"), _T("default_http_port"), m_http_port, m_sFile);
	m_https_port = GetPrivateProfileInt(_T("Config"), _T("default_https_port"), m_https_port, m_sFile);
	//cl end
}

void CConfig::WriteBack()
{
	if (_waccess(m_sFile, 0) == -1) {
		FILE *fp;
		fp = _wfopen(m_sFile, _T("wt,ccs=UNICODE"));
		fclose(fp);
	}
	// write config back to file
	WritePrivateProfileString(_T("Config"), _T("repoter"),   m_rp_name, m_sFile);
//	WritePrivateProfileString(_T("Config"), _T("user"),	     m_user,    m_sFile);
//	WritePrivateProfileString(_T("Config"), _T("password"),  m_passwd,  m_sFile);
	//Add: 2015-06-17

	WritePrivateProfileString(_T("Config"), _T("default_scheme"),	 m_scheme,  m_sFile);
	WritePrivateProfileString(_T("Config"), _T("default_site"),      m_site,    m_sFile);
	WritePrivateProfileString(_T("Config"), _T("url_1"),	 m_url[0],  m_sFile);
	WritePrivateProfileString(_T("Config"), _T("url_2"),	 m_url[1],  m_sFile);
	
	WritePrivateProfileString(_T("Config"), _T("url_3"),	 m_url[2],  m_sFile);
	TCHAR stmp[5];
	swprintf(stmp, _T("%d"), m_urlIndex);
	WritePrivateProfileString(_T("Config"), _T("default_url"),    stmp,   m_sFile);
	WritePrivateProfileString(_T("Config"), _T("upload_image"),   m_commit_img  ? _T("1"):_T("0"),   m_sFile);
	WritePrivateProfileString(_T("Config"), _T("upload_webfile"), m_commit_slice? _T("1"):_T("0"),   m_sFile);
	//cl 2015-11-22
	swprintf(stmp, _T("%d"), m_http_port);
	WritePrivateProfileString(_T("Config"), _T("default_http_port"), stmp, m_sFile);
	swprintf(stmp, _T("%d"), m_https_port);
	WritePrivateProfileString(_T("Config"), _T("default_https_port"), stmp, m_sFile);
	swprintf(stmp, _T("%d"), m_report_timeout);
	WritePrivateProfileString(_T("Config"), _T("report_timeout"), stmp, m_sFile);
	swprintf(stmp, _T("%d"), m_report_trycount);
	WritePrivateProfileString(_T("Config"), _T("report_trycount"), stmp, m_sFile);
	//cl end
}

CConfig::~CConfig(void)
{
}
