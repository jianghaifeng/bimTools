#pragma once

#include "ximage.h"

#include "ProjectMgr.h"
#include "GlobalApi.h"


// CPicView ÊÓÍ¼
#define THUMB_WIDTH    160  // thumbnail width
#define THUMB_HEIGHT   80   // thumbnail height

#define MAX_IMG_CNT    100  // max image count

typedef struct _sortdata {
	TCHAR name[STR_LONG_LEN];
	int index;
} Sortdata;

class CPicView : public CScrollView
{
	DECLARE_DYNCREATE(CPicView)

public:
	CPicView();           // ¶¯Ì¬´´½¨ËùÊ¹ÓÃµÄÊÜ±£»¤µÄ¹¹Ôìº¯Êý
	virtual ~CPicView();

	BOOL m_bShowPic;
	int  m_repSig;			// report signal
	bool m_bRunning;
	bool m_bTerminate;
	HANDLE m_hThread;
	unsigned int m_dwThreadID;

	CListCtrl  m_listCtrl;
	CImageList m_ImageList;

	struct _data {
		int flag;
		int slicesize;
		TCHAR name[STR_LONG_LEN];
		CxImage *pImg;
	} m_data[MAX_IMG_CNT];

	Sortdata m_SortedData[MAX_IMG_CNT];

	int sort_data();
	int init_data();
	int release_data();
	int find_data(const TCHAR* name);
	int input_data(const TCHAR* name);
	int num_data();
	vector<wstring> find_all_data();
	int remove_data(TCHAR* name, BOOL delfile);

	int InitSock();
	void ExitSock();
	int DelAllImage();
	int DelImage();
	BOOL TerminateThread();
	void LoadTumbImage();
	bool m_issliced;
	int Slice();
	int CommitImage();

	TCHAR errstr[256];
	CProjectMgr m_projMgr;

	//ne_session * m_pDav;
	int Dav_init(char* scheme, char* url);
	int Dav_mkcol(const char* p);
	int Dav_put(const TCHAR* s, const char* d, int mode);
	int CPicView::Dav_del(const char* d);
	int Dav_close();

	static unsigned __stdcall	LoadThumbNail(LPVOID lpParam);
	static unsigned __stdcall   UploadImage(LPVOID lpParam);
	static unsigned __stdcall   SliceImage(LPVOID lpParam);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	virtual void OnDraw(CDC* pDC);      // ÖØÐ´ÒÔ»æÖÆ¸ÃÊÓÍ¼
	virtual void OnInitialUpdate();     // ¹¹ÔìºóµÄµÚÒ»´Î

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


