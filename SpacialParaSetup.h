#pragma once
#include "afxcmn.h"
//#include "XTabCtrl.h"

#include "InRegion.h"
#include "QRLoc.h"
//#include "InHorVer.h"
#include "OutRange.h"
#include "CInnerCheck.h"
#include "ZouBanPro.h"
//#include "CorrectPixel.h"
#include "SetTimeout.h"
#include "afxwin.h"

class CParaDlg;
class CPropDlg;

// CSpacialParaSetup dialog
class CParaDlg;
class CInRegion;
class CQRLoc;
class COutRange;
//class CInHorVer;
class CZouBanPro;
//class CCorrectPixel;
class CSetTimeout;

class CSpacialParaSetup : public CBCGPDialog
{
	DECLARE_DYNAMIC(CSpacialParaSetup)

public:
	CSpacialParaSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSpacialParaSetup();

// Dialog Data
	enum { IDD = IDD_DIALOG_PARASETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	void GetTempCheckParam();
	void UpdateTempCheckParam();
	//CXTabCtrl      m_ctrlTab;
	CInRegion       *m_dlgInRegion;
	CQRLoc        *m_dlgQrloc;
    COutRange       *m_dlgOutRange;
    //CInHorVer     *m_dlgHorVer;
	//CCorrectPixel *m_dlgCorrPixel;
	CZouBanPro      *m_dlgZouBan;	

	BOOL m_bChangeZoubanWhite;
	CSetTimeout     *m_dlgTimeOut;

	double m_fScaleHor;
	double m_fScaleVer;
	
	//BOOL m_bFlagInRegion;	
	BOOL m_bFlagQrloc;
	//BOOL m_bFlagOutRange;
	BOOL m_bFlagHorVer;
	s_CheckPrintParam m_sCheckPrintParam;
	CStatic           m_labAddTab;
	CBCGPTabWnd       m_wndTab;
	//  [11/4/2015 zl]
	HObject m_hImgSrc;
	int nDlgSwitch;
	CParaDlg  *m_pParaDlg;
	CPropDlg  *m_pPropDlg;
	//  [4/6/2017 zb]
	int m_nLanguage;
	CString  m_strDefaultParaPath;
	char m_strMessageText[512][256];
};
