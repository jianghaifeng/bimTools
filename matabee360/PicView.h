#pragma once

#include "ximage.h"

#include "ProjectMgr.h"
#include "ProjectEntity.h"
#include "GlobalApi.h"

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
    CPicView();
    virtual ~CPicView();

    BOOL m_bShowPic;
    int  m_repSig;			// report signal
    bool m_bRunning;
    bool m_bTerminate;
    HANDLE m_hThread;
    unsigned int m_dwThreadID;

    CListCtrl  m_listCtrl;
    CImageList m_ImageList;

    typedef struct _data {
        int flag;
        int slicesize;
        TCHAR name[STR_LONG_LEN];
        CxImage* pImg = NULL;
    } Pic;
    
    Pic m_data[MAX_IMG_CNT];

    Sortdata m_SortedData[MAX_IMG_CNT];

    int sort_data();
    int init_data();
    int release_data();
    int find_data(const TCHAR* name);
    int input_data(const TCHAR* name);
    int num_data();
    vector<wstring> find_all_data();
    int remove_data(TCHAR* name, BOOL delfile);

    int DelAllImage();
    int DelImage();
    BOOL TerminateThread();
    void LoadTumbImage();
    bool m_issliced;
    int Slice();
    int CommitImage();

    TCHAR errstr[256];
    CProjectMgr m_projMgr;
    ProjectEntity* m_projectEntity;

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
    virtual void OnDraw(CDC* pDC);
    virtual void OnInitialUpdate();

    DECLARE_MESSAGE_MAP()
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};


