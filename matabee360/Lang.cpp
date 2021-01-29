#include "StdAfx.h"
#include "Lang.h"
#include "GlobalApi.h"
#include <io.h>

void CLang::LoadDefault()
{
	_tcscpy(m_rosetta[s_title],				_T("Matabee360"));
	_tcscpy(m_rosetta[s_config],			_T("config"));
	_tcscpy(m_rosetta[s_ok],				_T("ok"));
	_tcscpy(m_rosetta[s_cancel],			_T("cancel"));
	_tcscpy(m_rosetta[s_manage],			_T("manage"));
	_tcscpy(m_rosetta[s_add],				_T("add"));
	_tcscpy(m_rosetta[s_remove],			_T("remove"));
	_tcscpy(m_rosetta[s_clean],				_T("new"));
	_tcscpy(m_rosetta[s_edit],				_T("edit"));
	_tcscpy(m_rosetta[s_report],			_T("report"));
	_tcscpy(m_rosetta[s_confirm],			_T("confirm"));
	_tcscpy(m_rosetta[s_reporter],			_T("reporter"));
	_tcscpy(m_rosetta[s_default],			_T("default"));
	_tcscpy(m_rosetta[s_s_upload_imgs],		_T("upload_images"));
	_tcscpy(m_rosetta[s_s_reporter_info],	_T("reporter information"));
	_tcscpy(m_rosetta[s_s_buzzsaw_settings],_T("upload settings"));
	_tcscpy(m_rosetta[s_site],				_T("report settings"));
	_tcscpy(m_rosetta[s_shopname],			_T("shop"));
	_tcscpy(m_rosetta[s_prompt],			_T("prompt"));
	_tcscpy(m_rosetta[s_s_upload_ok],		_T("upload success"));
	_tcscpy(m_rosetta[s_s_req_uname],		_T("user name must be set"));
	_tcscpy(m_rosetta[s_s_prog_fail],		_T("network init fail"));
	_tcscpy(m_rosetta[s_s_exec_fail],		_T("program error"));
	_tcscpy(m_rosetta[s_s_exit_confirm],	_T("Are you sure to exit?"));
	_tcscpy(m_rosetta[s_s_upload_fail],		_T("upload failed!"));
	_tcscpy(m_rosetta[s_s_timeout],		    _T("time out, please wait"));
	_tcscpy(m_rosetta[s_s_desc],		    _T("***please check if you want to upload the image file, this will not effect user browsing"));
	_tcscpy(m_rosetta[s_s_tip1],		    _T("Add pictures"));
	_tcscpy(m_rosetta[s_s_tip2],		    _T("press the Add button or drag jpg files here!"));
	_tcscpy(m_rosetta[s_load],		        _T("Load"));
	_tcscpy(m_rosetta[s_preview],			_T("Preview"));
	//cl 2015-11-22
	_tcscpy(m_rosetta[s_retry],				_T("Retry"));
	//cl end
}

//D:\m\matabee360-2014-09-11\matabee360(UNICODE)\Debug\lang.ini
//D:\m\matabee360-2014-09-11\matabee360(UNICODE)\Debug

#define GETSTR(a) GetPrivateProfileString(_T("DICT"), m_title[a], m_rosetta[a], m_rosetta[a], 128, sFile);
void CLang::LoadPrivate(const TCHAR* sFile)
{
	GETSTR(s_title);				
	GETSTR(s_config);				
	GETSTR(s_ok);					
	GETSTR(s_cancel);				
	GETSTR(s_manage);				
	GETSTR(s_add);				
	GETSTR(s_remove);
	GETSTR(s_edit);
	GETSTR(s_clean);
	GETSTR(s_report);				
	GETSTR(s_confirm);			
	GETSTR(s_reporter);							
	GETSTR(s_default);			
	GETSTR(s_s_upload_imgs);			
	GETSTR(s_s_reporter_info);	
	GETSTR(s_s_buzzsaw_settings); 
	GETSTR(s_site);				
	GETSTR(s_shopname);
	GETSTR(s_prompt);	
	GETSTR(s_s_upload_ok);
	GETSTR(s_s_req_uname);
	GETSTR(s_s_prog_fail);
	GETSTR(s_s_exec_fail);
	GETSTR(s_s_exit_confirm);
	GETSTR(s_s_upload_fail);
	GETSTR(s_s_timeout);
	GETSTR(s_s_desc);
	GETSTR(s_s_tip1);
	GETSTR(s_s_tip2);
	GETSTR(s_load);
	GETSTR(s_preview);
	GETSTR(s_retry);
}

#define TITLE(a) _tcscpy(m_title[a], _T(#a))
CLang::CLang(void)
{
	//default to English
	TITLE(s_title);		
	TITLE(s_config);			
	TITLE(s_ok);				
	TITLE(s_cancel);			
	TITLE(s_manage);			
	TITLE(s_add);				
	TITLE(s_remove);
	TITLE(s_clean);
	TITLE(s_edit);
	TITLE(s_report);			
	TITLE(s_confirm);			
	TITLE(s_reporter);						
	TITLE(s_default);			
	TITLE(s_s_upload_imgs);		
	TITLE(s_s_reporter_info);	
	TITLE(s_s_buzzsaw_settings);
	TITLE(s_site);				
	TITLE(s_shopname);
	TITLE(s_prompt);	
	TITLE(s_s_upload_ok);
	TITLE(s_s_req_uname);
	TITLE(s_s_prog_fail);
	TITLE(s_s_exec_fail);
	TITLE(s_s_exit_confirm);
	TITLE(s_s_upload_fail);
	TITLE(s_s_timeout);
	TITLE(s_s_desc);
	TITLE(s_s_tip1);
	TITLE(s_s_tip2);
	TITLE(s_load);
	TITLE(s_preview);
	TITLE(s_retry);
	
	CString sFile;
	get_exec_dir(sFile);
	sFile+=_T("lang.ini");

	LoadDefault();

	if (_waccess(sFile, 0) != -1) {
		LoadPrivate(sFile);
	}
}


CLang::~CLang(void)
{
}
