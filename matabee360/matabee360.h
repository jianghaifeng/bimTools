
// matabee360.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "Config.h"
#include "matabee360Dlg.h"
#include "Lang.h"
#include "PromptDlg.h"
// Cmatabee360App:
// 有关此类的实现，请参阅 matabee360.cpp
//

class Cmatabee360App : public CWinApp
{
public:
	Cmatabee360App();
	CConfig *pCfg;
	Cmatabee360Dlg *pDlg;
	CLang* pLang;

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern Cmatabee360App theApp;