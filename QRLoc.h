#pragma once


// CQRLoc dialog

class CQRLoc : public CBCGPDialog
{
	DECLARE_DYNAMIC(CQRLoc)

public:
	CQRLoc(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQRLoc();

// Dialog Data
	enum { IDD = IDD_DINGWEI_QR };
	CBCGPButton m_cmdCheckQrLoc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckQRloc;
	afx_msg void OnBnClickedCheckQrloc();
	virtual BOOL OnInitDialog();
	int      m_nLanguage;
	CString  m_strDefaultParaPath;
	CString  m_strLanguagePath;
	CString  m_strControlMapPath;
	typedef  struct _CONTROL_SET
	{
		int  nControlID;
		CString  strControl;
	}s_ControlSet;
};
