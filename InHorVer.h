#pragma once


// CInHorVer dialog

class CInHorVer : public CDialog
{
	DECLARE_DYNAMIC(CInHorVer)

public:
	CInHorVer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInHorVer();

// Dialog Data
	enum { IDD = IDD_IN_HOEVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckHorVer;
	afx_msg void OnBnClickedCheckHorver();
	CBCGPStatic m_LabDirect;
	CBCGPStatic m_LabArea;
	CComboBox m_ComDirect;
	int m_nArea;
	int m_nDirect;

	virtual BOOL OnInitDialog();
};
