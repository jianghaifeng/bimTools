#include "StdAfx.h"
#include "ProjectMgr.h"
#include "GlobalApi.h"

CProjectMgr::CProjectMgr(void)
{
}

CProjectMgr::~CProjectMgr(void)
{
}

void CProjectMgr::add_to_upload_list(upload_type ut, TCHAR * src, char * dst)
{
	upload_item ui = upload_item(ut, src, dst);
	make_upload_item_localpath(ui);
	m_upload_list.push_back(ui);
}

void CProjectMgr::add_to_upload_list(upload_type ut, TCHAR * src, char *dstdir, char * dst)
{
	char tmp[STR_LONG_LEN];
	strcpy(tmp, dstdir);
	strcat(tmp, dst);
	upload_item ui = upload_item(ut, src, tmp);
	make_upload_item_localpath(ui);
	m_upload_list.push_back(ui);
}

CProjectMgr::upload_item& CProjectMgr::get_upload_item(int index)
{
	return m_upload_list[index];
}

int CProjectMgr::upload_item_num()
{
	return m_upload_list.size();
}

void CProjectMgr::clear_upload_item()
{
	m_upload_list.clear();
}

void CProjectMgr::set_local_dir(TCHAR* dir)
{
	m_local_dir = wstring(dir);
}

void CProjectMgr::set_remote_dir(char* dir)
{
	m_remote_dir = string(dir);
}

const char* CProjectMgr::get_remote_dir()
{
	return m_remote_dir.c_str();
}

const TCHAR* CProjectMgr::get_local_dir()
{
	return m_local_dir.c_str();
}

void CProjectMgr::make_upload_item_localpath(upload_item & ui)
{
	wstring tmp = m_local_dir;
	tmp.append(ui.localpath);
	ui.localpath = tmp;
}

wstring CProjectMgr::get_upload_item_localpath(upload_item & ui)
{
	return ui.localpath;
}

string CProjectMgr::get_upload_item_remotepath(upload_item & ui)
{
	string tmp = m_remote_dir;
	return tmp.append(ui.remotepath);
}
