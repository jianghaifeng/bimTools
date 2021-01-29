#pragma once

enum e_rosetta {
	s_title,
	s_config,
	s_ok,
	s_cancel,
	s_manage,
	s_add,
	s_remove,
	s_clean,
	s_edit,
	s_report,
	s_confirm,
	s_reporter,
	s_default,
	s_s_upload_imgs,
	s_s_reporter_info,
	s_s_buzzsaw_settings,
	s_site,
	s_shopname,
	s_prompt,
	s_s_upload_ok,
	s_s_req_uname,
	s_s_prog_fail,
	s_s_exec_fail,
	s_s_exit_confirm,
	s_s_upload_fail,
	s_s_timeout,
	s_s_desc,
	s_s_tip1,
	s_s_tip2,
	s_load,
	s_preview,
	//cl 2015-11-22
	s_retry,
	//cl end
	s_end
};

class CLang
{
public:
	CLang(void);
	~CLang(void);

	TCHAR m_rosetta[s_end][128];

private:
	void LoadDefault();
	void LoadPrivate(const TCHAR* sFile);
	TCHAR m_title[s_end][32];
};

