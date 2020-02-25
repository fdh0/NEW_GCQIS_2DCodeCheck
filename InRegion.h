#pragma once
#include "d:\CodeFile\inc\bcgcbpro2210\bcgpbutton.h"
#include "d:\CodeFile\inc\bcgcbpro2210\bcgpstatic.h"
#include "afxcmn.h"

// CInRegion dialog

class CInRegion : public CBCGPDialog
{
	DECLARE_DYNAMIC(CInRegion)

public:
	CInRegion(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInRegion();

// Dialog Data
	enum { IDD = IDD_IN_REGION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nDeviation;
	int m_nMaxGray;
	BOOL m_bCheckInRegin;
	afx_msg void OnBnClickedCheckInrgion();
	afx_msg void OnEnChangeEditDeviation();
	afx_msg void OnEnChangeEditMaxgray();
	virtual BOOL OnInitDialog();
	CBCGPButton m_cmdCheckInRegion;
	CBCGPStatic m_labDeviation;
	CBCGPStatic m_labMaxGray;
	afx_msg void OnDeltaposSpinDev(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinMaxgray(NMHDR *pNMHDR, LRESULT *pResult);
	CBCGPSliderCtrl m_slcDev;
	CBCGPSliderCtrl m_slcMaxGray;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	CBCGPButton m_cmdCheckHorver2;
	CBCGPStatic m_LabDirect;
	CBCGPStatic m_LabArea;
	CBCGPStatic m_LabContrast;
	CComboBox m_ComDirect;
	CBCGPSliderCtrl m_slcArea;
	CBCGPSliderCtrl m_slcContrast;
	double m_dArea;
	int m_nDirect;
	int m_nContrast;
	BOOL m_bCheckHorVer;
	BOOL m_bShowHorVer;
	void UpdateState();
	afx_msg void OnDeltaposSpinArea(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboDirect();
	afx_msg void OnEnChangeEditArea2();
	afx_msg void OnBnClickedCheckHorver2();
	afx_msg void OnEnChangeEditContrast();
	afx_msg void OnDeltaposSpinContrast(NMHDR *pNMHDR, LRESULT *pResult);
	
	CBCGPStatic m_labPencent;
	afx_msg void OnEnChangeEditPercent();
	afx_msg void OnDeltaposSpinPercent(NMHDR *pNMHDR, LRESULT *pResult);
	int m_nPercent;
	CBCGPSliderCtrl m_slcPercent;
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
