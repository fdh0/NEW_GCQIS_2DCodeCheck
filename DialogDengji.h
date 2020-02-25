#if !defined(AFX_DIALOGDENGJI_H__2D97BA22_0FC0_4B68_98D6_4D4836AFA0A9__INCLUDED_)
#define AFX_DIALOGDENGJI_H__2D97BA22_0FC0_4B68_98D6_4D4836AFA0A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogDengji.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogDengji dialog

class CDialogDengji : public CBCGPDialog
{
// Construction
public:
	CDialogDengji(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogDengji)
	enum { IDD = IDD_DIALOG_DENGJI };
	BOOL	m_bDJ1;
	BOOL	m_bDJ2;
	BOOL	m_bDJ3;
	BOOL	m_bDJ4;
	BOOL	m_bDJ5;
	BOOL	m_bDJ6;
	BOOL	m_bDJ7;

//	BOOL	m_bAddR;
	BOOL	m_bCheckAll;
	//BOOL	m_bDecodeAbility;
	//BOOL	m_bDefect;
	//BOOL	m_bMinEC;
	//BOOL	m_bMinR;
	//BOOL	m_bModule;
	//BOOL	m_bSymCon;
	BOOL	m_bNoAll;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDengji)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogDengji)
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	afx_msg void OnCheck5();
	afx_msg void OnCheck6();
	afx_msg void OnCheck7();
	afx_msg void OnCheckAll();
	afx_msg void OnCheckNoall();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CBCGPStatic m_labStd;
	CBCGPStatic m_labStdValue;
	CBCGPButton m_cmdCheckAll;
	CBCGPButton m_cmdCheckNoAll;
	CBCGPButton m_cmdCheck1;
	CBCGPButton m_cmdCheck2;
	CBCGPButton m_cmdCheck3;
	CBCGPButton m_cmdCheck4;
	CBCGPButton m_cmdCheck5;
	CBCGPButton m_cmdCheck6;
	CBCGPButton m_cmdCheck7;
	CBCGPButton m_cmdOnOK;
	CBCGPButton m_cmdCancel;
	//  [4/7/2017 zb]
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
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCancel1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGDENGJI_H__2D97BA22_0FC0_4B68_98D6_4D4836AFA0A9__INCLUDED_)
