
// matabee360.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Config.h"
#include "matabee360Dlg.h"
#include "Lang.h"
#include "PromptDlg.h"
// Cmatabee360App:
// �йش����ʵ�֣������ matabee360.cpp
//

class Cmatabee360App : public CWinApp
{
public:
	Cmatabee360App();
	CConfig *pCfg;
	Cmatabee360Dlg *pDlg;
	CLang* pLang;

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cmatabee360App theApp;