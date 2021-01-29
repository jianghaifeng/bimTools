#pragma once

#include <vector>
#include <string>

using namespace std;

class CProjectMgr
{
public:
	enum upload_type {
		e_upload_file = 0,
		e_upload_dir,
		e_upload_none
	};

	class upload_item {
	public:
		upload_type ut;
		wstring localpath;
		string remotepath;

	public:
		upload_item() :
		  ut(e_upload_dir),
		  localpath(_T("\\")),
		  remotepath("/") {}

		upload_item(upload_type a_ut, TCHAR* a_localpath, char* a_remotepath) :
		  ut(a_ut),
		  localpath(a_localpath),
		  remotepath(a_remotepath) {}
	};

public:
	CProjectMgr(void);
	~CProjectMgr(void);

	void add_to_upload_list(upload_type ut, TCHAR * src, char * dst);
	void add_to_upload_list(upload_type ut, TCHAR * src, char *dstdir, char * dst);
	upload_item & get_upload_item(int index);
	void make_upload_item_localpath(upload_item & ui);
	wstring get_upload_item_localpath(upload_item & ui);
	string get_upload_item_remotepath(upload_item & ui);
	int upload_item_num();
	void clear_upload_item();
	void set_local_dir(TCHAR* dir);
	void set_remote_dir(char* dir);
	const char* get_remote_dir();
	const TCHAR* get_local_dir();

private:
	wstring m_local_dir;
	string m_remote_dir;
	vector<upload_item> m_upload_list;
};

