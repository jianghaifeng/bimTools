
// matabee360Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "matabee360.h"
#include "matabee360Dlg.h"
#include "afxdialogex.h"

#include "ConfigDlg.h"
#include "ReportDlg.h"
#include "ManDlg.h"
#include "PromptDlg.h"
#include <Windows.h>
#include <DbgHelp.h>  
#include <iostream>  
#include <vector>  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



Cmatabee360Dlg::Cmatabee360Dlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(Cmatabee360Dlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_bPrjCreated = false;
    memset(m_sPrjImg, 0, STR_LONG_LEN);
    memset(m_sPrjWeb, 0, STR_LONG_LEN);
    memset(m_sPrjTpl, 0, STR_LONG_LEN);
    memset(m_sPrjLog, 0, STR_LONG_LEN);
    memset(m_sPrjDt, 0, STR_DTIME_LEN);

    pView = NULL;
}

void Cmatabee360Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROGRESS_UPLOAD, m_progress);
    DDX_Control(pDX, IDC_BTN_SETTINGS, m_btnS);
    DDX_Control(pDX, IDC_BTN_ADD, m_btnA);
    DDX_Control(pDX, IDC_BTN_RMV, m_btnR);
    DDX_Control(pDX, IDC_BTN_RPT, m_btnC);
    DDX_Control(pDX, IDC_BTN_MANAGE, m_btnM);
    DDX_Control(pDX, IDC_BTN_CLEAN, m_btnD);
    DDX_Control(pDX, IDC_BTN_SLICE, m_btnE);
}

BEGIN_MESSAGE_MAP(Cmatabee360Dlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDCANCEL, &Cmatabee360Dlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_BTN_ADD, &Cmatabee360Dlg::OnBnClickedBtnAdd)
    ON_BN_CLICKED(IDC_BTN_RMV, &Cmatabee360Dlg::OnBnClickedBtnRmv)
    ON_BN_CLICKED(IDC_BTN_RPT, &Cmatabee360Dlg::OnBnClickedBtnRpt)
    ON_BN_CLICKED(IDC_BTN_SETTINGS, &Cmatabee360Dlg::OnBnClickedBtnSettings)
    ON_BN_CLICKED(IDC_BTN_MANAGE, &Cmatabee360Dlg::OnBnClickedBtnManage)
    ON_BN_CLICKED(IDC_BTN_CLEAN, &Cmatabee360Dlg::OnBnClickedBtnClean)
    ON_WM_DROPFILES()
    ON_BN_CLICKED(IDC_BTN_SLICE, &Cmatabee360Dlg::OnBnClickedBtnSlice)
END_MESSAGE_MAP()


#define BUTTON_STYLE(b,i,t) \
{ \
b.SetIcon(i); \
b.SetAlign(CButtonST::ST_ALIGN_VERT); \
b.SetFlat(TRUE); \
b.SetWindowText(t);\
}

#define BUTTON_STYLE_SETTING(b,i,t) \
{ \
b.SetIcon(i); \
b.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT); \
b.SetFlat(TRUE); \
b.SetWindowText(t);\
}

#define BUTTON_STYLE_MAN(b,i,t) \
{ \
b.SetIcon(i); \
b.SetAlign(CButtonST::ST_ALIGN_HORIZ); \
b.SetFlat(TRUE); \
b.SetWindowText(t);\
}

BOOL Cmatabee360Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    CRect rect;
    GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&rect);
    pView = new CPicView;
    ((CView*)pView)->Create(NULL, NULL, WS_EX_CLIENTEDGE, rect, GetDlgItem(IDC_STATIC_PIC), NULL, NULL);
    pView->ShowWindow(SW_SHOW);
    pView->OnInitialUpdate();

    m_progress.SetShowPercent(TRUE);
    m_progress.SetRange(0, 100);
    m_progress.SetPos(0);

    theApp.pLang = new CLang();
    theApp.pCfg = new CConfig();

    BUTTON_STYLE(m_btnA, IDI_ICON_A, theApp.pLang->m_rosetta[s_add]);
    BUTTON_STYLE(m_btnR, IDI_ICON_R, theApp.pLang->m_rosetta[s_remove]);
    BUTTON_STYLE(m_btnC, IDI_ICON_C, theApp.pLang->m_rosetta[s_report]);
    BUTTON_STYLE(m_btnD, IDI_ICON_D, theApp.pLang->m_rosetta[s_clean]);
    BUTTON_STYLE(m_btnE, IDI_ICON_E, theApp.pLang->m_rosetta[s_edit]);

    BUTTON_STYLE_SETTING(m_btnS, IDI_ICON_S, theApp.pLang->m_rosetta[s_config]);
    BUTTON_STYLE_MAN(m_btnM, IDI_ICON_M, theApp.pLang->m_rosetta[s_manage]);

    UnLockButtons();

    this->SetWindowText(theApp.pLang->m_rosetta[s_title]);

    return TRUE; 
}

void Cmatabee360Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void Cmatabee360Dlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); 

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR Cmatabee360Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void Cmatabee360Dlg::OnBnClickedCancel()
{
    if (IDYES != MessageBox(theApp.pLang->m_rosetta[s_s_exit_confirm], theApp.pLang->m_rosetta[s_confirm], MB_YESNO))
        return;

    if (theApp.pCfg != NULL)
        delete theApp.pCfg;
    if (theApp.pLang != NULL)
        delete theApp.pLang;
    if (pView != NULL)
        delete pView;

    CDialogEx::OnCancel();
}

void Cmatabee360Dlg::CreateNewProject()
{
    if (!m_bPrjCreated || pView->m_issliced) {
        CString str;
        get_time_str(str);        // will be used in uploading instance.
        _tcscpy(m_sPrjDt, str.GetBuffer(str.GetLength()));
        TCHAR PrjDir[STR_LONG_LEN];

        get_root_dir(str);
        _tcscpy(PrjDir, str.GetBuffer(str.GetLength()));

        swprintf(m_sPrjTpl, _T("%s%s"), PrjDir, _T("Template")); // will be used in uploading instance.

        _tcscat(PrjDir, _T("Project\\"));
        CreateDirectory(PrjDir, NULL);  // should be unnessesary, create again whatever...

        _tcscat(PrjDir, theApp.pCfg->m_rp_name);
        _tcscat(PrjDir, _T("_"));
        _tcscat(PrjDir, m_sPrjDt);
        CreateDirectory(PrjDir, NULL);  // create step by step.

        swprintf(m_sPrjLog, _T("%s\\%s"), PrjDir, _T("prj.log")); // set log file

        swprintf(m_sPrjImg, _T("%s\\%s"), PrjDir, _T("images"));
        CreateDirectory(m_sPrjImg, NULL);  // create images dir

        swprintf(m_sPrjWeb, _T("%s\\%s"), PrjDir, _T("web"));
        CreateDirectory(m_sPrjWeb, NULL);  // create web dir

        pView->m_issliced = false;
        UpdateProgress(0);
        m_bPrjCreated = true;
    }
}

BOOL Cmatabee360Dlg::isOK()
{
    FILE* fp;
    fp = _wfopen(m_sPrjLog, _T("rt,ccs=UNICODE"));
    TCHAR s[STR_LONG_LEN];

    if (fp) {
        for (int i = 0; i < 4; i++)
            fgetws(s, STR_LONG_LEN, fp);

        fgetws(s, STR_LONG_LEN, fp);

        fclose(fp);
        if (_tcsstr(s, _T("OK"))) {
            return TRUE;
        }
    }

    return FALSE;
}

#if 0
void Cmatabee360Dlg::OpenProject(TCHAR* dt, TCHAR* user)
{
    CString PrjDir;

    get_root_dir(PrjDir);
    swprintf(m_sPrjTpl, _T("%s%s"), PrjDir, "Template"); // will be used by in uploading instance.

    _tcscpy(theApp.pCfg->m_rp_name, user);
    PrjDir.Append(_T("Project\\"));
    _tcscpy(m_sPrjDt, dt);
    PrjDir.Append(user);
    PrjDir.Append(_T("_"));
    PrjDir.Append(dt);

    swprintf(m_sPrjLog, _T("%s\\%s"), PrjDir, _T("prj.log")); // set log file

    swprintf(m_sPrjImg, _T("%s\\%s"), PrjDir, _T("images"));
    swprintf(m_sPrjWeb, _T("%s\\%s"), PrjDir, _T("web"));

    m_bPrjCreated = true;
}
#endif
/* Jiang haifeng comment 2014-09-10:
 *	local dir:
 *   image directory: /project/[reporter]/YYYYMMDD_hhmmss/images/(xxx.jpg xxx.jpg ...)
 *   webfile        : /project/[reporter]/YYYYMMDD_hhmmss/web/(index.html ...)
 *
 *	webdav:
 *		/reporter_YYYYMMDD_hhmmss[_SHOPNAME]/images
 *		/reporter_YYYYMMDD_hhmmss[_SHOPNAME]/web
 */

void Cmatabee360Dlg::OnBnClickedBtnAdd()
{
    TCHAR szLargeBuf[51200];
    memset(szLargeBuf, 0, 51200);

    CFileDialog dlg(TRUE,
        _T("JPG Files (*JPG)"),
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT | OFN_ENABLESIZING,
        _T("Layer Files (*.TIFF;*.BMP;*.JPG)|*.TIFF;*.BMP;*.JPG;)||"),
        NULL,
        0,
        FALSE);
    dlg.m_ofn.lpstrFile = szLargeBuf;
    dlg.m_ofn.nMaxFile = 50000;

    if (IDOK == dlg.DoModal()) {
        POSITION pos = dlg.GetStartPosition();
        if (pos != NULL) {
            CreateNewProject();
            CString strinfo;

            strinfo.Format(_T("Path    :[%s\\%s]\r\n")
                _T("Reporter:[%s]\r\n")
                _T("Buzzaw  :[%d]\r\n")
                _T("Upload Image/webfiles: [%s/%s]"),
                theApp.pCfg->m_rp_name, m_sPrjDt,
                theApp.pCfg->m_user,
                theApp.pCfg->m_urlIndex,
                theApp.pCfg->m_commit_img ? _T("yes") : _T("no"),
                theApp.pCfg->m_commit_slice ? _T("yes") : _T("no"));

            PrintLog(_T("project created!!!"), strinfo);
        }
        CString fullname, filepath;
        int res;

        // recover data
        if (pView->num_data() != 0) {
            vector<wstring> v = pView->find_all_data();

            for (vector<wstring>::iterator it = v.begin(); it != v.end(); ++it) {
                CString str(it->c_str());
                get_full_name(str, fullname);
                filepath.Format(_T("%s\\%s"), m_sPrjImg, fullname);
                CopyFile(str, filepath, false);
            }
        }

        while (pos != NULL) {
            CString str = dlg.GetNextPathName(pos);

            get_full_name(str, fullname);
            filepath.Format(_T("%s\\%s"), m_sPrjImg, fullname);

            if (pView->find_data(filepath) < 0) {
                CopyFile(str, filepath, false);
                res = pView->input_data(filepath);
                if (MAX_IMG_CNT == res)
                {
                    PrintLog(_T("file exceed max number"));
                    return;
                }
                else if (-1 == res)
                {
                    PrintLog(_T("duplicate file"));
                }
                else if (-2 == res)
                {
                    PrintLog(_T("image size not correct"));
                }
            }
        }
        pView->LoadTumbImage();
    }
}

void Cmatabee360Dlg::OnBnClickedBtnRmv()
{
    if (m_bPrjCreated == true && 0 == pView->DelImage()) {
        //m_bPrjCreated = false;
        SetDlgItemText(IDC_EDIT_INFO, _T(""));
    }
}

void Cmatabee360Dlg::OnBnClickedBtnSlice()
{
    TCHAR errstr[256];
    int num = pView->num_data();
    if (num == 0)
        PrintLog(_T("no image"));
    else {
        // prepare web files
        if (_tcslen(theApp.pCfg->m_rp_name) < 1) {
            CPromptDlg dlg(theApp.pLang->m_rosetta[s_s_req_uname], theApp.pLang->m_rosetta[s_prompt], this);
            dlg.DoModal();
            return;
        }

        if (!pView->m_issliced) {
            CReportDlg dlg;
            if (dlg.DoModal() == IDOK) {
                CString strName;
                if (_tcslen(theApp.pCfg->m_rp_shop) > 0) {
                    strName.Format(_T("%s"), theApp.pCfg->m_rp_shop);
                }
                else {
                    strName.Format(_T("%s"), m_sPrjDt);
                }
                create_web_files(m_sPrjTpl, m_sPrjWeb, strName, num);
                PrintLog(_T("web files created!"));
                pView->Slice();
            }
        }
        else
        {
            pView->Slice();
        }
    }
}

void Cmatabee360Dlg::OnBnClickedBtnRpt()
{
    TCHAR errstr[256];
    int num = pView->num_data();
    if (num == 0)
        PrintLog(_T("no image"));
    else {
        // prepare web files
        if (_tcslen(theApp.pCfg->m_rp_name) < 1) {
            CPromptDlg dlg(theApp.pLang->m_rosetta[s_s_req_uname], theApp.pLang->m_rosetta[s_prompt], this);
            dlg.DoModal();
            return;
        }

        if (!pView->m_issliced) {
            CReportDlg dlg;
            if (dlg.DoModal() == IDOK) {
                CString strName;
                if (_tcslen(theApp.pCfg->m_rp_shop) > 0) {
                    strName.Format(_T("%s"), theApp.pCfg->m_rp_shop);
                }
                else {
                    strName.Format(_T("%s"), m_sPrjDt);
                }
                create_web_files(m_sPrjTpl, m_sPrjWeb, strName, num);
                PrintLog(_T("web files created!"));
                //pView->Slice();
                pView->CommitImage();
            }
        }
        else
        {
            //pView->Slice();
            pView->CommitImage();
        }
    }
    //if (pView->m_issliced)
    //	pView->CommitImage();
}

void Cmatabee360Dlg::PrintLog(const TCHAR* brief, const TCHAR* spec, int lv)
{
    static TCHAR info[3][STR_SHORT_LEN];
    TCHAR showinfo[STR_MID_LEN];

    if (brief != NULL && lv >= 0 && lv < 3) {
        // copy info string, length limited.
        _tcsncpy(info[lv], brief, STR_SHORT_LEN - 2);
        info[lv][STR_SHORT_LEN - 1] = 0;

        if (lv == 0) {
            SetDlgItemText(IDC_EDIT_INFO, info[0]);
        }
        else if (lv == 1) {
            _tcscpy(showinfo, info[0]);
            _tcscat(showinfo, _T("\r\n"));
            _tcscat(showinfo, info[1]);
            SetDlgItemText(IDC_EDIT_INFO, showinfo);
        }
        else if (lv == 2) {
            _tcscpy(showinfo, info[0]);
            _tcscat(showinfo, _T("\r\n"));
            _tcscat(showinfo, info[1]);
            _tcscat(showinfo, _T("\r\n"));
            _tcscat(showinfo, info[2]);
            SetDlgItemText(IDC_EDIT_INFO, showinfo);
        }
    }

    if (spec != NULL)
        LogOutput(spec);
}

void Cmatabee360Dlg::UpdateProgress(int percent)
{
    if (percent <= 100 && percent >= 0)
        m_progress.SetPos(percent);
}

void Cmatabee360Dlg::UpdateProgressText(const TCHAR* description)
{
    m_progress.SetWindowTextW(description);
}

void Cmatabee360Dlg::OnBnClickedBtnSettings()
{
    // TODO: 在此添加控件通知处理程序代码
    CConfigDlg dlg(theApp.pCfg);
    if (dlg.DoModal() == IDOK) {
        theApp.pCfg->WriteBack();
    }
}

void Cmatabee360Dlg::LockButtons()
{
    m_btnS.EnableWindow(FALSE);
    m_btnA.EnableWindow(FALSE);
    m_btnR.EnableWindow(FALSE);
    m_btnC.EnableWindow(FALSE);
    m_btnM.EnableWindow(FALSE);
    m_btnD.EnableWindow(FALSE);
    m_btnE.EnableWindow(FALSE);
}

void Cmatabee360Dlg::UnLockButtons()
{
    m_btnS.EnableWindow(TRUE);
    m_btnA.EnableWindow(TRUE);
    m_btnR.EnableWindow(TRUE);
    m_btnC.EnableWindow(TRUE);
    m_btnM.EnableWindow(TRUE);
    m_btnD.EnableWindow(TRUE);
    m_btnE.EnableWindow(TRUE);
}

void Cmatabee360Dlg::OnBnClickedBtnManage()
{
    CManDlg dlg;
    if (dlg.DoModal() == IDOK)
    {
        OnBnClickedBtnClean();
        this->CreateNewProject();

        TCHAR PrjDir[STR_LONG_LEN];
        TCHAR PrjFile[STR_LONG_LEN];
        _tcscpy(PrjDir, dlg.m_curDir.LockBuffer());
        wsprintf(PrjFile, _T("%s\\prj.log"), PrjDir);
        //::DeleteFile(PrjFile);

        _tcscat(PrjDir, _T("\\images\\*.*"));

        CFileFind tempFind;
        BOOL bFound = tempFind.FindFile(PrjDir);
        CString strTmp, filepath, fullname;
        while (bFound)
        {
            bFound = tempFind.FindNextFile();
            if (tempFind.IsDots()) continue;
            if (!tempFind.IsDirectory())
            {
                strTmp = tempFind.GetFilePath();
                get_full_name(strTmp, fullname);
                filepath.Format(_T("%s\\%s"), m_sPrjImg, fullname);
                CopyFile(strTmp, filepath, false);
                pView->input_data(strTmp.GetBuffer(strTmp.GetLength()));
            }
        }
        pView->LoadTumbImage();
        tempFind.Close();
    }
}

void Cmatabee360Dlg::OnBnClickedBtnClean()
{
    // TODO: 在此添加控件通知处理程序代码
    pView->DelAllImage();
    m_bPrjCreated = false;
}
#pragma comment(lib, "dbghelp.lib")  

using namespace std;

const int MAX_ADDRESS_LENGTH = 32;
const int MAX_NAME_LENGTH = 1024;

// 崩溃信息  
//   
struct CrashInfo
{
    CHAR ErrorCode[MAX_ADDRESS_LENGTH];
    CHAR Address[MAX_ADDRESS_LENGTH];
    CHAR Flags[MAX_ADDRESS_LENGTH];
};

// CallStack信息  
//   
struct CallStackInfo
{
    CHAR ModuleName[MAX_NAME_LENGTH];
    CHAR MethodName[MAX_NAME_LENGTH];
    CHAR FileName[MAX_NAME_LENGTH];
    CHAR LineNumber[MAX_NAME_LENGTH];
};

// 安全拷贝字符串函数  
//  
void SafeStrCpy(char* szDest, size_t nMaxDestSize, const char* szSrc)
{
    if (nMaxDestSize <= 0) return;
    if (strlen(szSrc) < nMaxDestSize)
    {
        strcpy_s(szDest, nMaxDestSize, szSrc);
    }
    else
    {
        strncpy_s(szDest, nMaxDestSize, szSrc, nMaxDestSize);
        szDest[nMaxDestSize - 1] = '\0';
    }
}

//  
CrashInfo GetCrashInfo(const EXCEPTION_RECORD* pRecord)
{
    CrashInfo crashinfo;
    SafeStrCpy(crashinfo.Address, MAX_ADDRESS_LENGTH, "N/A");
    SafeStrCpy(crashinfo.ErrorCode, MAX_ADDRESS_LENGTH, "N/A");
    SafeStrCpy(crashinfo.Flags, MAX_ADDRESS_LENGTH, "N/A");

    sprintf_s(crashinfo.Address, "%08X", pRecord->ExceptionAddress);
    sprintf_s(crashinfo.ErrorCode, "%08X", pRecord->ExceptionCode);
    sprintf_s(crashinfo.Flags, "%08X", pRecord->ExceptionFlags);

    return crashinfo;
}

//  
vector<CallStackInfo> GetCallStack(const CONTEXT* pContext)
{
    HANDLE hProcess = GetCurrentProcess();

    SymInitialize(hProcess, NULL, TRUE);

    vector<CallStackInfo> arrCallStackInfo;

    CONTEXT c = *pContext;

    STACKFRAME64 sf;
    memset(&sf, 0, sizeof(STACKFRAME64));
    DWORD dwImageType = IMAGE_FILE_MACHINE_I386;

#ifdef _M_IX86  
    sf.AddrPC.Offset = c.Eip;
    sf.AddrPC.Mode = AddrModeFlat;
    sf.AddrStack.Offset = c.Esp;
    sf.AddrStack.Mode = AddrModeFlat;
    sf.AddrFrame.Offset = c.Ebp;
    sf.AddrFrame.Mode = AddrModeFlat;
#elif _M_X64  
    dwImageType = IMAGE_FILE_MACHINE_AMD64;
    sf.AddrPC.Offset = c.Rip;
    sf.AddrPC.Mode = AddrModeFlat;
    sf.AddrFrame.Offset = c.Rsp;
    sf.AddrFrame.Mode = AddrModeFlat;
    sf.AddrStack.Offset = c.Rsp;
    sf.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64  
    dwImageType = IMAGE_FILE_MACHINE_IA64;
    sf.AddrPC.Offset = c.StIIP;
    sf.AddrPC.Mode = AddrModeFlat;
    sf.AddrFrame.Offset = c.IntSp;
    sf.AddrFrame.Mode = AddrModeFlat;
    sf.AddrBStore.Offset = c.RsBSP;
    sf.AddrBStore.Mode = AddrModeFlat;
    sf.AddrStack.Offset = c.IntSp;
    sf.AddrStack.Mode = AddrModeFlat;
#else  
#error "Platform not supported!"  
#endif  

    HANDLE hThread = GetCurrentThread();

    while (true)
    {
        if (!StackWalk64(dwImageType, hProcess, hThread, &sf, &c, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
        {
            break;
        }

        if (sf.AddrFrame.Offset == 0)
        {
            break;
        }

        CallStackInfo callstackinfo;
        SafeStrCpy(callstackinfo.MethodName, MAX_NAME_LENGTH, "N/A");
        SafeStrCpy(callstackinfo.FileName, MAX_NAME_LENGTH, "N/A");
        SafeStrCpy(callstackinfo.ModuleName, MAX_NAME_LENGTH, "N/A");
        SafeStrCpy(callstackinfo.LineNumber, MAX_NAME_LENGTH, "N/A");

        BYTE symbolBuffer[sizeof(IMAGEHLP_SYMBOL64) + MAX_NAME_LENGTH];
        IMAGEHLP_SYMBOL64* pSymbol = (IMAGEHLP_SYMBOL64*)symbolBuffer;
        memset(pSymbol, 0, sizeof(IMAGEHLP_SYMBOL64) + MAX_NAME_LENGTH);

        pSymbol->SizeOfStruct = sizeof(symbolBuffer);
        pSymbol->MaxNameLength = MAX_NAME_LENGTH;

        DWORD symDisplacement = 0;

        //  
        if (SymGetSymFromAddr64(hProcess, sf.AddrPC.Offset, NULL, pSymbol))
        {
            SafeStrCpy(callstackinfo.MethodName, MAX_NAME_LENGTH, pSymbol->Name);
        }

        IMAGEHLP_LINE64 lineInfo;
        memset(&lineInfo, 0, sizeof(IMAGEHLP_LINE64));

        lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        DWORD dwLineDisplacement;

        //  
        if (SymGetLineFromAddr64(hProcess, sf.AddrPC.Offset, &dwLineDisplacement, &lineInfo))
        {
            SafeStrCpy(callstackinfo.FileName, MAX_NAME_LENGTH, lineInfo.FileName);
            sprintf_s(callstackinfo.LineNumber, "%d", lineInfo.LineNumber);
        }

        IMAGEHLP_MODULE64 moduleInfo;
        memset(&moduleInfo, 0, sizeof(IMAGEHLP_MODULE64));

        moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);

        //  
        if (SymGetModuleInfo64(hProcess, sf.AddrPC.Offset, &moduleInfo))
        {
            SafeStrCpy(callstackinfo.ModuleName, MAX_NAME_LENGTH, moduleInfo.ModuleName);
        }

        arrCallStackInfo.push_back(callstackinfo);
}

    SymCleanup(hProcess);

    return arrCallStackInfo;
}

void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS* pException)
{
    //  
    HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    //  
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;

    //  
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

    CloseHandle(hDumpFile);
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
    CreateDumpFile(_T("C:\\errdump.dmp"), pException); 
    //  
#ifdef _M_IX86  
    if (pException->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
    {
        static char TempStack[1024 * 128];
        __asm mov eax, offset TempStack[1024 * 128];
        __asm mov esp, eax;
    }
#endif    

    CrashInfo crashinfo = GetCrashInfo(pException->ExceptionRecord);

    FILE* fp;
    fp = _wfopen(_T("C:\\errlog.txt"), _T("w"));


    USES_CONVERSION;

    fwprintf(fp, _T("ErrorCode: %s\r\n"), A2W(crashinfo.ErrorCode));
    fwprintf(fp, _T("Address: %s\r\n"), A2W(crashinfo.Address));
    fwprintf(fp, _T("Flags: %s\r\n"), A2W(crashinfo.Flags));

    vector<CallStackInfo> arrCallStackInfo = GetCallStack(pException->ContextRecord);

    fwprintf(fp, _T("CallStack:\r\n"));

    for (vector<CallStackInfo>::iterator i = arrCallStackInfo.begin(); i != arrCallStackInfo.end(); ++i)
    {
        CallStackInfo callstackinfo = (*i);

        fwprintf(fp, _T("%s() : [%s] (File : %s @Line%s)\r\n"), A2W(callstackinfo.MethodName), A2W(callstackinfo.ModuleName), A2W(callstackinfo.FileName), A2W(callstackinfo.LineNumber));
    }

    fclose(fp);

    FatalAppExit(-1, _T("An Exception happend! The information are saved in C:\\errlog.txt and C:\\errdump.dmp"));

    return EXCEPTION_EXECUTE_HANDLER;
}

void Cmatabee360Dlg::OnDropFiles(HDROP hDropInfo)
{
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);

    UINT        uNumFiles;
    TCHAR       szNextFile[STR_LONG_LEN];
    CString     filepath, fullname;
    int         res;

    uNumFiles = DragQueryFile(hDropInfo, -1, NULL, 0);

    if (uNumFiles > 0) {
        CreateNewProject();
    }

    // recover data
    if (pView->num_data() != 0) {
        vector<wstring> v = pView->find_all_data();

        for (vector<wstring>::iterator it = v.begin(); it != v.end(); ++it) {
            CString str(it->c_str());
            get_full_name(str, fullname);
            filepath.Format(_T("%s\\%s"), m_sPrjImg, fullname);
            CopyFile(str, filepath, false);
        }
    }

    for (UINT uFile = 0; uFile < uNumFiles; uFile++) {
        if (DragQueryFile(hDropInfo, uFile, szNextFile, MAX_PATH) > 0) {
            get_full_name(szNextFile, fullname);
            filepath.Format(_T("%s\\%s"), m_sPrjImg, fullname);

            if (pView && pView->find_data(filepath) < 0) {
                CopyFile(szNextFile, filepath, false);
                res = pView->input_data(filepath);
                if (MAX_IMG_CNT == res)
                {
                    PrintLog(_T("file exceed max number"));
                    return;
                }
                else if (-1 == res)
                {
                    PrintLog(_T("duplicate file"));
                }
                else if (-2 == res)
                {
                    PrintLog(_T("image size not correct"));
                }
            }
            // If the file has the hidden attribute set, its attributes in
            // the SHFILEINFO struct will include SFGAO_GHOSTED.  We'll mark
            // the item as "cut" in our list to give it the same ghosted look.

        }   // end for
    }

    pView->LoadTumbImage();

    DragFinish(hDropInfo);
    //CDialogEx::OnDropFiles(hDropInfo);
}


