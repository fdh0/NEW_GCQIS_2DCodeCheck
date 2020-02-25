#pragma once


// CSetTimeout dialog

class CSetTimeout : public CBCGPDialog
{
	DECLARE_DYNAMIC(CSetTimeout)

public:
	CSetTimeout(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetTimeout();

// Dialog Data
	enum { IDD = IDD_TIMEOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckTimeout();
	afx_msg void OnEnKillfocusEditSingletime();
	afx_msg void OnEnKillfocusEditAlltime();
	virtual BOOL OnInitDialog();

	BOOL m_bCheckTime;
	int  m_nSingleTime;
	int  m_nAllTime;
	
	//BCG Control
	CBCGPButton m_cmdCheckTimeout;
	CBCGPStatic m_labSingleTime;
	CBCGPStatic m_labAllTime;
	afx_msg void OnDeltaposSpinSingletime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinAlltime(NMHDR *pNMHDR, LRESULT *pResult);
	CBCGPSliderCtrl m_slcSingleTime;
	CBCGPSliderCtrl m_slcAllTime;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditSingletime();
	afx_msg void OnEnChangeEditAlltime();
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
