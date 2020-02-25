#pragma once

// COutRange dialog

class COutRange : public CBCGPDialog
{
	DECLARE_DYNAMIC(COutRange)

public:
	COutRange(CWnd* pParent = NULL);   // standard constructor
	virtual ~COutRange();

// Dialog Data
	enum { IDD = IDD_OUTRANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL   m_bCheckOutRange;
	double m_dTop;
	double m_dLeft;
	double m_dBottom;
	double m_dRight;
	double m_dSingleArea;
	double m_dAllArea;
	double m_fScaleHor;
	double m_fScaleVer;
	afx_msg void OnBnClickedCheckOutrange();
	afx_msg void OnEnChangeEditTop();
	afx_msg void OnEnChangeEditBottom();
	afx_msg void OnEnChangeEditLeft();
	afx_msg void OnEnChangeEditright();
	afx_msg void OnEnChangeEditSinglearea();
	afx_msg void OnEnChangeEditAllarea();
	
	//BCG Control
	CBCGPButton m_cmdCheckOutRange;
	CBCGPStatic m_labTop;
	CBCGPStatic m_labLeft;
	CBCGPStatic m_labBottom;
	CBCGPStatic m_labRight;
	CBCGPStatic m_labSingleArea;
	CBCGPStatic m_labAllArea;
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinTop(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinLeft(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinBottom(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinSinglearea(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinAllarea(NMHDR *pNMHDR, LRESULT *pResult);
	CBCGPSliderCtrl m_slcTop;
	CBCGPSliderCtrl m_slcLeft;
	CBCGPSliderCtrl m_slcBottom;
	CBCGPSliderCtrl m_slcRight;
	CBCGPSliderCtrl m_slcSingleArea;
	CBCGPSliderCtrl m_slcAllArea;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//  [4/6/2017 zb] Ó¢ÎÄ½çÃæ
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
};
