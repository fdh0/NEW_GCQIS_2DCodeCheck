#pragma once
#include "HalconCpp.h"
#include "afxcmn.h"
#include "afxwin.h"
using namespace HalconCpp;
// CZouBanPro dialog

class CParaDlg;
class CPropDlg;

class CZouBanPro : public CBCGPDialog
{
	DECLARE_DYNAMIC(CZouBanPro)

public:
	CZouBanPro(CWnd* pParent = NULL);   // standard constructor
	virtual ~CZouBanPro();

// Dialog Data
	enum { IDD = IDD_ZOUBAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckZouban();
	afx_msg void OnEnKillfocusEditSpsous();
	afx_msg void OnBnClickedCheckInloc();
	afx_msg void OnEnKillfocusEditCzsous();
	virtual BOOL OnInitDialog();

	BOOL    m_bCheckZouban;
	BOOL    m_bInLoc;
	double  m_dSearchX;
	double  m_dSearchY;

	double  m_fScaleHor;
	double  m_fScaleVer;
	BOOL m_bChangeZouban;
	
	//BCG Control
	CBCGPButton m_cmdCheckZouban;
	CBCGPButton m_cmdCheckInLoc;
	CBCGPButton m_cmdCheckWhite;
	CBCGPButton m_cmdZBL;
	CBCGPButton m_cmdZBR;
	CBCGPButton m_cmdZBU;
	CBCGPButton m_cmdZBD;

	CBCGPStatic m_labSpsous;
	CBCGPStatic m_labCzsous;
	afx_msg void OnDeltaposSpinSpsous(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCzsous(NMHDR *pNMHDR, LRESULT *pResult);
	CBCGPSliderCtrl m_slcSpsous;
	CBCGPSliderCtrl m_slcCzsous;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditSpsous();
	afx_msg void OnEnChangeEditCzsous();

	double m_fZouBanStdVer;

	double m_fZouBanStdHor;

	double m_fZouBanRange;


	//  [11/4/2015 zl]走版白框边界位置
	int m_nleftPos;
	int m_nRightPos;
	int m_nUpPos;
	int m_nDownPos;
	HObject m_hImgSrc;
	HObject m_hoLReg;
	HObject m_hoRReg;
	HObject m_hoUReg;
	HObject m_hoDReg;

	afx_msg void OnEnChangeEditZoubanstdUp();
	afx_msg void OnEnChangeEditZoubanstdLeft();
	afx_msg void OnEnChangeEditZoubandisUp();
	afx_msg void OnEnChangeEditZoubandisRight();
	//  [11/4/2015 zl]
	int nDlgSwitch;
	CParaDlg  *m_pParaDlg;
	CPropDlg  *m_pPropDlg;
	CSliderCtrl m_sliderZBThresh;
	int m_nZouBanSegThresh;
	BOOL m_bIsWhiteZouBan;
	HTuple m_hImgW;
	HTuple m_hImgH;
	HObject m_hRealZoubanRegionL;
	HObject m_hRealZoubanRegionR;
	HObject m_hRealZoubanRegionU;
	HObject m_hRealZoubanRegionD;

	afx_msg void OnBnClickedCheckIsWhiteZouBan();
	afx_msg void OnBnClickedRadioLeft();
	afx_msg void OnBnClickedRadioRight();
	afx_msg void OnBnClickedRadioUp();
	afx_msg void OnBnClickedRadioDown();
	BOOL m_bIsCheckZouBanUp;
	BOOL m_bIsCheckZouBanDown;
	BOOL m_bIsCheckZouBanLeft;
	BOOL m_bIsCheckZouBanRight;
	CBCGPSliderCtrl m_sliderVerDis;
	CBCGPSliderCtrl m_sliderHorDis;
	CBCGPSliderCtrl m_sliderZBRange;
	afx_msg void OnEnChangeEditZoubanRange();
	afx_msg void OnEnChangeEditZoubanstdver();
	afx_msg void OnEnChangeEditZoubanstdhor();
	afx_msg void OnEnChangeEditZoubanrange();
	afx_msg void OnDeltaposSpinSpsrange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinZbver(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinZbhor(NMHDR *pNMHDR, LRESULT *pResult);
	void DispZBRange();
	CBCGPStatic m_labDisVer;
	CBCGPStatic m_labDisHor;
	CBCGPStatic m_labRange;
	//  [4/6/2017 zb] 英文界面
	int      m_nLanguage;
	CString  m_strDefaultParaPath;
	CString  m_strLanguagePath;
	CString  m_strControlMapPath;
	CString  m_strMessage;
	char     m_cMessage[256];
	typedef  struct _CONTROL_SET
	{
		int  nControlID;
		CString  strControl;
	}s_ControlSet;
	char m_strMessageText[512][256];
};
