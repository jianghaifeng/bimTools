// Global APIs header file for Matabee360 program
// 2014-09-05 
// created by Jianghaifeng

#define STR_LONG_LEN  512  // for directory and file path
#define STR_MID_LEN   256  // for address .etc
#define STR_SHORT_LEN 32   // for username, password .etc
#define STR_DTIME_LEN 16   // YYYYMMDD_hhmmss

void ANSItoUTF8_URL(char* strAnsi);
void ANSItoUTF8(char* strAnsi);
void create_web_files(CString src, CString dst, CString name, int num);
void get_file_name(TCHAR* path, TCHAR *s);
void get_full_name(const TCHAR* path, CString& s);
void get_root_dir(CString& s);
void get_exec_dir(CString& s);
void get_time_str(CString& s);
void parse_url(TCHAR* s, TCHAR* schema, TCHAR* url, TCHAR* dir);

bool slice_routine(int lens, int thumb_lens,int preview_lens, TCHAR* source_file, TCHAR* dest_path,int quality);

void LogOutput(LPCWSTR info);