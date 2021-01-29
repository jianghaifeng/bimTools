// PicView.cpp : 实现文件
//

#include "stdafx.h"
#include "matabee360.h"
#include "matabee360Dlg.h"
#include "PicView.h"
#include <fcntl.h>
#include <io.h>
#include <errno.h>
#include <string.h>
#include <atlconv.h>

#define TIMER_CHECK_ID   1
#define TIMER_TIMEOUT_ID 2

#define ALWAYS_SORT_DATA    //default to enable
//#define DEBUG_WITHOUT_DAV //default to disable

// CPicView

IMPLEMENT_DYNCREATE(CPicView, CScrollView)

CPicView::CPicView()
{
	m_bTerminate=false;
	m_bRunning=false;
	m_bShowPic = FALSE;
	m_hThread = NULL;

	m_issliced = false;
}

CPicView::~CPicView()
{
	release_data();
}

int CPicView::init_data()
{
	for (int i=0; i<MAX_IMG_CNT; i++) {
		m_data[i].flag = 0;
		memset(m_data[i].name, 0, STR_LONG_LEN);
		m_data[i].pImg = new CxImage(THUMB_WIDTH,THUMB_HEIGHT,3,CXIMAGE_FORMAT_BMP);
	}
	return 0;
}

int r_compare(const void* a, const void* b)
{
	return _tcscmp(((Sortdata*)a)->name, ((Sortdata*)b)->name);
}

int CPicView::sort_data()
{
	memset(m_SortedData, 0, sizeof(m_SortedData));
	int j = 0;
	for (int i=0; i<MAX_IMG_CNT; i++) {
		if (m_data[i].flag) {
			m_SortedData[j].index = i;
			_tcscpy(m_SortedData[j].name, m_data[i].name);
			j++;
		}
	}

#ifdef ALWAYS_SORT_DATA
	qsort(m_SortedData, j, sizeof(Sortdata), r_compare);
#endif

	return 0;
}

int CPicView::release_data()
{
	for (int i=0; i<MAX_IMG_CNT; i++) {
		if (m_data[i].pImg != NULL) {
			//m_data[i].pImg->Destroy();
			::delete (m_data[i].pImg);
			m_data[i].pImg = NULL;
		}
	}
	return 0;
}

int CPicView::find_data(const TCHAR* name)
{
	for (int i=0; i<MAX_IMG_CNT; i++) {
		if (m_data[i].flag == 1 && _tcsstr(m_data[i].name, name) != NULL) {
			return i;
		}
	}
	return -1;
}

int CPicView::input_data(const TCHAR* name)
{
	CxImage origin(name,CXIMAGE_FORMAT_BMP);
	RGBQUAD rgb = {0,0,0,0};

	if (find_data(name) >= 0) return -1;

	int i = 0;
	for (i=0; i<MAX_IMG_CNT; i++) {
		if (m_data[i].flag == 0) {
			origin.Thumbnail(THUMB_WIDTH,THUMB_HEIGHT,rgb,m_data[i].pImg);
			if (origin.GetWidth() == 2048 && origin.GetHeight() == 1024 ) 
			{
				m_data[i].slicesize = 1024;
			}
			else if (origin.GetWidth() == 3584 && origin.GetHeight() == 1792)
			{
				m_data[i].slicesize = 1792;
			}
			else if (origin.GetWidth() == 5376 && origin.GetHeight() == 2688)
			{
				m_data[i].slicesize = 2688;
			}
			else
			{
				return -2;
			}
			/*
			if ( !(origin.GetWidth() == 2048 && origin.GetHeight() == 1024 ) 
				&& !(origin.GetWidth() == 3584 && origin.GetHeight() == 1792)
				&& !(origin.GetWidth() == 5376 && origin.GetHeight() == 2688))
				return -2;
			*/
			m_data[i].flag = 1;
			_tcscpy(m_data[i].name, name);
			break;
		}
	}
	return i;
}

unsigned __stdcall CPicView::SliceImage(LPVOID lpParam)
{
	CPicView * pView = (CPicView *)lpParam;

	TCHAR dir[STR_LONG_LEN],
		subdir[STR_LONG_LEN],
		sname[STR_LONG_LEN],
		dname[STR_LONG_LEN],
		slicefolder[STR_LONG_LEN];

	TCHAR slicename[8][STR_SHORT_LEN] = {_T("pano_f.jpg"),_T("pano_b.jpg"),_T("pano_d.jpg"),
		_T("pano_u.jpg"),_T("pano_l.jpg"),_T("pano_r.jpg"),_T("preview.jpg"),_T("thumb.jpg")};

	CString s1;
	FILE *fp;
	fp = _wfopen(theApp.pDlg->m_sPrjLog, _T("wt,ccs=UNICODE"));

	s1.Format(_T("URL[%d]\n"), theApp.pCfg->m_urlIndex);
	fputws(s1, fp);

	USES_CONVERSION;

	pView->m_projMgr.clear_upload_item();
	pView->m_projMgr.set_local_dir(theApp.pDlg->m_sPrjWeb);
	//pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_dir, _T(""), "/");

	_tcscpy(dir, _T("/"));
	_tcscat(dir, theApp.pCfg->m_rp_name);
	fputws(theApp.pCfg->m_rp_name, fp);
	fputws(_T("\n"), fp);
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_dir, _T(""), W2A(dir));

	_tcscat(dir, _T("/"));
	_tcscat(dir, theApp.pCfg->m_rp_name);
	_tcscat(dir, _T("_"));
	_tcscat(dir, theApp.pDlg->m_sPrjDt);
	fputws(theApp.pDlg->m_sPrjDt, fp);
	fputws(_T("\n"), fp);
	if (_tcslen(theApp.pCfg->m_rp_shop) > 0)
	{
		_tcscat(dir, _T("_"));
		_tcscat(dir, theApp.pCfg->m_rp_shop);
		fputws(theApp.pCfg->m_rp_shop, fp);
		fputws(_T("\n"), fp);
	}
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_dir, _T(""), W2A(dir));
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\index.html"), W2A(dir), "/index.html");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\superscene.swf"), W2A(dir), "/superscene.swf");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\superscene.xml"), W2A(dir), "/superscene.xml");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\superscene_editor.html"), W2A(dir), "/superscene_editor.html");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\superscene.js"), W2A(dir), "/superscene.js");

	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_dir, _T(""), W2A(dir), "/skin");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\skin\\logo.png"), W2A(dir), "/skin/logo.png");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\skin\\qtvr-cursors.png"), W2A(dir), "/skin/qtvr-cursors.png");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\skin\\superscene.png"), W2A(dir), "/skin/superscene.png");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\skin\\superscene_skin.xml"), W2A(dir), "/skin/superscene_skin.xml");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\skin\\thumbborder.png"), W2A(dir), "/skin/thumbborder.png");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\skin\\close.png"), W2A(dir), "/skin/close.png");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\skin\\mark5.png"), W2A(dir), "/skin/mark5.png");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\skin\\addmark.png"), W2A(dir), "/skin/addmark.png");

	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_dir, _T(""), W2A(dir), "/plugins");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\plugins\\dblclick.swf"), W2A(dir), "/plugins/dblclick.swf");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\plugins\\radar.swf"), W2A(dir), "/plugins/radar.swf");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\plugins\\scrollarea.swf"), W2A(dir), "/plugins/scrollarea.swf");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\plugins\\soundinterface.swf"), W2A(dir), "/plugins/soundinterface.swf");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\plugins\\textfield.swf"), W2A(dir), "/plugins/textfield.swf");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\plugins\\vtoureditor.swf"), W2A(dir), "/plugins/vtoureditor.swf");
	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, _T("\\plugins\\addMark.swf"), W2A(dir), "/plugins/addMark.swf");

	pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_dir, _T(""), W2A(dir), "/panos");
	
	int ItemCount = pView->num_data();
	int sliceIndex = 0;
	theApp.pDlg->UpdateProgress(2);
	theApp.pDlg->UpdateProgressText(_T("processing "));
	for (int i_of_data=0; i_of_data<ItemCount; i_of_data++) {

		// 0~10%
		theApp.pDlg->UpdateProgress(2+i_of_data*8/ItemCount);

		int i = pView->m_SortedData[i_of_data].index;

		CString s;
		if (i >= 0 && i < MAX_IMG_CNT && pView->m_data[i].flag == 1) {
			sliceIndex++;
			swprintf(slicefolder, _T("%s\\panos\\%02d"), theApp.pDlg->m_sPrjWeb, sliceIndex);
			CreateDirectory(slicefolder, NULL);
			//slice_routine(1024, 160, 256, pView->m_data[i].name, slicefolder, 85);
			slice_routine(pView->m_data[i].slicesize, 160, 256, pView->m_data[i].name, slicefolder, 85);
			swprintf(subdir, _T("%s/panos/%02d"), dir, sliceIndex);
			pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_dir, _T(""), W2A(subdir));

			if (theApp.pCfg->m_commit_slice) {
				for (int j=0; j<8; j++) {
					swprintf(sname, _T("\\panos\\%02d\\%s"), sliceIndex, slicename[j]);
					swprintf(dname, _T("%s/%s"), subdir, slicename[j]);
					pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, sname, W2A(dname));
				}
			}

			if (theApp.pCfg->m_commit_img) {
				pView->m_projMgr.set_local_dir(theApp.pDlg->m_sPrjImg);
				get_full_name(pView->m_data[i].name, s);
				TCHAR fstmp[64];
				swprintf(fstmp, _T("\\%s"), s);
				swprintf(dname, _T("%s/%s"), dir, s);
				pView->m_projMgr.add_to_upload_list(CProjectMgr::e_upload_file, fstmp, W2A(dname));
				pView->m_projMgr.set_local_dir(theApp.pDlg->m_sPrjWeb);
			}
		}
	}
	//theApp.pDlg->UnLockButtons();
	theApp.pDlg->UpdateProgress(10);
	theApp.pDlg->UpdateProgressText(_T("uploading "));

	pView->m_issliced = true;
	fclose(fp);

	CString sFile;
	get_exec_dir(sFile);
	sFile.Append( _T("file.tmp"));

	FILE *fp1 = _wfopen(sFile, _T("wt,ccs=UNICODE"));
	fputws(pView->m_projMgr.get_local_dir(), fp1);
	fputws(_T("\\superscene_editor.html\n"), fp1);
	fputws(pView->m_projMgr.get_local_dir(), fp1);
	fputws(_T("\\superscene.xml\n"), fp1);
	fputws(pView->m_projMgr.get_local_dir(), fp1);
	fputws(_T("\\tags.csv"), fp1);
	fclose(fp1);
#if 0 //jhf: go with no-tag
	STARTUPINFO StartInfo;
	PROCESS_INFORMATION pinfo;

	memset(&StartInfo,0,sizeof(STARTUPINFO));
	StartInfo.cb = sizeof(STARTUPINFO);
	get_exec_dir(sFile);
	sFile.Append(_T("PanoViewer.exe"));
	BOOL ret = CreateProcess(
		NULL,
		sFile.GetBuffer(),
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&StartInfo,
		&pinfo);
#endif
	return 0;
}

unsigned __stdcall CPicView::UploadImage(LPVOID lpParam)
{
	CPicView * pView = (CPicView *)lpParam;
	FILE *fp;
	CString s;

	SliceImage(lpParam);

	USES_CONVERSION;
	fp = _wfopen(theApp.pDlg->m_sPrjLog, _T("at,ccs=UNICODE"));

	if (fp == NULL) {
		pView->m_repSig = 2;
		_endthreadex( 0 );
		return 1;
	}

	int site_num = 0;
	for (int i=0; i<3; i++)
	{
		if ( theApp.pCfg->m_urlIndex & (1 << i) )
			site_num++;
	}

	int site_index=0;
	for (int i=0; i<3; i++)
	{
		if ( (theApp.pCfg->m_urlIndex & (1 << i))  == 0)
			continue;

		site_index++;

		TCHAR sscheme[16], surl[64], dir[64];
		parse_url(theApp.pCfg->m_url[i], sscheme, surl, dir);

		for (int index=0; index < (pView->m_projMgr.upload_item_num()); index++)
		{
			theApp.pDlg->UpdateProgress(10+site_index*index*90/(site_num*(pView->m_projMgr.upload_item_num())));
			CProjectMgr::upload_item ui = pView->m_projMgr.get_upload_item(index);
			theApp.pDlg->PrintLog(A2W(pView->m_projMgr.get_upload_item_remotepath(ui).c_str()), NULL, 1);
		}
	}

	fputws(_T("OK"), fp);
	fclose(fp);
	theApp.pDlg->UpdateProgressText(_T(""));
	theApp.pDlg->UpdateProgress(100);
	pView->KillTimer(TIMER_CHECK_ID);
	theApp.pDlg->UnLockButtons();
	theApp.pDlg->PrintLog(_T("Upload finished!!!"),_T("Upload finished, kill timer, unlockbuttons"));
	CPromptDlg dlg(theApp.pLang->m_rosetta[s_s_upload_ok], theApp.pLang->m_rosetta[s_prompt],theApp.pDlg);
	dlg.DoModal();
	theApp.pDlg->UpdateProgress(0);
	_endthreadex( 0 );
	
	return 0;
}

int CPicView::Slice()
{
	TerminateThread();
	m_hThread = (HANDLE) _beginthreadex(NULL,0,SliceImage,(LPVOID)this,0,&m_dwThreadID);
	m_bRunning = TRUE;
	theApp.pDlg->LockButtons();

	return 0;
}

int CPicView::CommitImage()
{
	TerminateThread();
	m_hThread = (HANDLE) _beginthreadex(NULL,0,UploadImage,(LPVOID)this,0,&m_dwThreadID);
	m_bRunning = TRUE;
	theApp.pDlg->LockButtons();
	SetTimer(TIMER_TIMEOUT_ID, 150000, NULL);
	SetTimer(TIMER_CHECK_ID, 5000, NULL);

	return 0;
}

vector<wstring> CPicView::find_all_data()
{
	vector<wstring> v;
	for (int i=0; i<MAX_IMG_CNT; i++) {
		if (m_data[i].flag) {
			v.push_back(m_data[i].name);
		}
	} 

	return v;
}

int CPicView::num_data()
{
	int cnt = 0;
	for (int i=0; i<MAX_IMG_CNT; i++) {
		if (m_data[i].flag) {
			cnt++;
		}
	}
	return cnt;
}

int CPicView::remove_data(TCHAR* name, BOOL delfile)
{
	int i = find_data(name);

	if (i >= 0) {
		m_data[i].flag = 0;
		if (delfile)
			DeleteFile(m_data[i].name);
		memset(m_data[i].name, 0, STR_LONG_LEN);
	}

	return i;
}

BEGIN_MESSAGE_MAP(CPicView, CScrollView)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CPicView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
	CRect rect;
	this->GetClientRect(&rect);

	m_listCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_ICON,
		              rect,this,IDR_LISTCTRL);
	m_ImageList.Create(THUMB_WIDTH,THUMB_HEIGHT,ILC_COLOR24,0,1);
	m_listCtrl.SetImageList(&m_ImageList,LVSIL_NORMAL);

	m_listCtrl.ShowWindow(SW_HIDE);

	if (init_data() != 0){
		CPromptDlg dlg(theApp.pLang->m_rosetta[s_s_exec_fail], theApp.pLang->m_rosetta[s_prompt],this);
		dlg.DoModal();
	}
}

void CPicView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	CString str;
	str.Format(_T("%s\r\n%s"),theApp.pLang->m_rosetta[s_s_tip1],
		theApp.pLang->m_rosetta[s_s_tip2]);
	
	CRect rect;
	CRect rectTmp;
	this->GetClientRect(&rect);
	int height = pDC->DrawText( str, &rectTmp, DT_CALCRECT|DT_CENTER|DT_EDITCONTROL|DT_WORDBREAK );
 
	//垂直、水平居中显示
	rect.top += (rect.Height() - height)/2 - 10;
	pDC->DrawText( str, &rect, DT_CENTER|DT_EDITCONTROL|DT_WORDBREAK );
}

#ifdef _DEBUG
void CPicView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CPicView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

// CPicView 消息处理程序

void CPicView::LoadTumbImage()
{
	TerminateThread();
	m_hThread = (HANDLE) _beginthreadex(NULL,0,LoadThumbNail,(LPVOID)this,0,&m_dwThreadID);
	m_bRunning = TRUE;
}

BOOL CPicView::TerminateThread()
{
	if ( !m_bRunning )
		return TRUE;
	
	m_bTerminate=true;
	
	for( ; ; )
	{
		if ( ::WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0 )
			break;
		
		MSG msg;
		while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) 
		{ 
			if (!AfxGetApp()->PumpMessage()) 
				break; 
		} 
	}
	
	::CloseHandle(m_hThread);
	return TRUE;
}

int CPicView::DelAllImage()
{
	int cnt = m_listCtrl.GetItemCount();
	for (int i=0; i<cnt; i++) {
		CString str = m_listCtrl.GetItemText(i,0);
		remove_data(str.GetBuffer(str.GetLength()), FALSE);
	}
	LoadTumbImage();

	return num_data();
}

int CPicView::DelImage()
{
	int cnt = m_listCtrl.GetSelectedCount();
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		LogOutput(_T("No items were selected!\n"));
	}
	else
	{
		while (pos)
		{
			int nItem = m_listCtrl.GetNextSelectedItem(pos);
			CString str = m_listCtrl.GetItemText(nItem, 0);
			remove_data(str.GetBuffer(str.GetLength()),TRUE);
		}
		LoadTumbImage();
	}

	return num_data();
}

unsigned __stdcall CPicView::LoadThumbNail(LPVOID lpParam)
{
	CPicView * pView = (CPicView *)lpParam;
	CListCtrl &ListCtrl = pView->m_listCtrl;
	CImageList &ImageList = pView->m_ImageList;

	for (int i=0;i<ImageList.GetImageCount();i++)
	{
		ImageList.Remove(i);
	}
	ListCtrl.DeleteAllItems();
	
	int ItemCount = pView->num_data();
	ImageList.SetImageCount(ItemCount);
	pView->sort_data();

	if (ItemCount > 0)
		pView->m_listCtrl.ShowWindow(SW_SHOW);
	else 
		pView->m_listCtrl.ShowWindow(SW_HIDE);

	pView->Invalidate();
	pView->UpdateWindow();

	int k;
	for (int i=0,j=0; i<ItemCount; i++) {
		k = pView->m_SortedData[i].index;
		if (k >= 0 && k < MAX_IMG_CNT && pView->m_data[k].flag == 1) {
			TCHAR s[256];
			get_file_name(pView->m_data[k].name, s);
			ListCtrl.InsertItem(j,s,j);
			j++;
		}
	}
	/*
	for (int i=0,j=0; i<MAX_IMG_CNT; i++) {
		if (pView->m_data[i].flag) {
			TCHAR s[256];
			get_file_name(pView->m_data[i].name, s);
			ListCtrl.InsertItem(j,s,j);
			j++;
		}
	}
	*/

	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();
	
	// Create Brushes for Border and BackGround
	HBRUSH hBrushBorder=::CreateSolidBrush(RGB(0, 0, 0));
	//HBRUSH hBrushBk=::CreateSolidBrush(RGB(255, 255, 255));
	
	CRect rcBorder;
	rcBorder.left = rcBorder.top = 0;
	rcBorder.right = THUMB_WIDTH;
	rcBorder.bottom = THUMB_HEIGHT;

	for (int i=0,j=0; i<ItemCount; i++) {
		k = pView->m_SortedData[i].index;
		if (k >= 0 && k < MAX_IMG_CNT && pView->m_data[k].flag == 1) {
			CClientDC cdc(pView);
			HDC hDC = ::CreateCompatibleDC(cdc.m_hDC);
			HBITMAP bm = ::CreateCompatibleBitmap(cdc.m_hDC,THUMB_WIDTH,THUMB_HEIGHT);
			HBITMAP OldBm = (HBITMAP) SelectObject(hDC,bm);

			//::FillRect(hDC,&rcBorder,hBrushBk);
			pView->m_data[k].pImg->Stretch(hDC,0,0,THUMB_WIDTH,THUMB_HEIGHT);
			::FrameRect(hDC,&rcBorder,hBrushBorder);
		
			SelectObject(hDC,OldBm);
			CBitmap bitMap;
			bitMap.Attach(bm);
			ImageList.Replace(j,&bitMap,NULL);
		
			ListCtrl.RedrawItems(j,j);
			DeleteObject(bm);
			DeleteDC(hDC);
			j++;
		}
	}
	
	/*
	for (int i=0,j=0; i<MAX_IMG_CNT; i++) {
		if (pView->m_data[i].flag) {
			CClientDC cdc(pView);
			HDC hDC = ::CreateCompatibleDC(cdc.m_hDC);
			HBITMAP bm = ::CreateCompatibleBitmap(cdc.m_hDC,THUMB_WIDTH,THUMB_HEIGHT);
			HBITMAP OldBm = (HBITMAP) SelectObject(hDC,bm);

			//::FillRect(hDC,&rcBorder,hBrushBk);
			pView->m_data[i].pImg->Stretch(hDC,0,0,THUMB_WIDTH,THUMB_HEIGHT);
			::FrameRect(hDC,&rcBorder,hBrushBorder);
		
			SelectObject(hDC,OldBm);
			CBitmap bitMap;
			bitMap.Attach(bm);
			ImageList.Replace(j,&bitMap,NULL);
		
			ListCtrl.RedrawItems(j,j);
			DeleteObject(bm);
			DeleteDC(hDC);
			j++;
		}
	}
	*/
	//DeleteObject(hBrushBk);
	DeleteObject(hBrushBorder);
	ListCtrl.Invalidate();

	_endthreadex( 0 );
	return 0;
}

void CPicView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int hisSig = 0;

	if (nIDEvent == 1) {
		DWORD ec;
		::GetExitCodeThread(m_hThread, &ec);

		if (ec != STILL_ACTIVE && m_repSig == 2) {
			KillTimer(TIMER_CHECK_ID);
			KillTimer(TIMER_TIMEOUT_ID);
			//CPromptDlg dlg(theApp.pLang->m_rosetta[s_s_upload_fail], theApp.pLang->m_rosetta[s_prompt],this);
			//dlg.DoModal();
			theApp.pDlg->UnLockButtons();
		}
	}
	else if (nIDEvent == 0) {
		CString str;
		CString strtime;
		get_time_str(strtime);
		str.Format(_T("TIMER(%d), %s: sig=%d\r\n"), nIDEvent, strtime, m_repSig);
		LogOutput(str);

		if (m_repSig == 1) {
			if (hisSig == 1) {
				CPromptDlg dlg(theApp.pLang->m_rosetta[s_s_timeout], theApp.pLang->m_rosetta[s_prompt],this);
				dlg.DoModal();
				return;
			}
			hisSig = 1;
		}
		else if (m_repSig == 0)
		{
			m_repSig = 1;
			hisSig = 0;
		}
	}

	CScrollView::OnTimer(nIDEvent);
}
