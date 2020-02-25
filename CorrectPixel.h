#pragma once


// CCorrectPixel dialog

class CCorrectPixel : public CDialog
{
	DECLARE_DYNAMIC(CCorrectPixel)

public:
	CCorrectPixel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCorrectPixel();
	BOOL m_bAlignCorrected;//是否进行错位校正
	int  m_nCorrectPixels;



// Dialog Data
	enum { IDD = IDD_DIALOG_CORR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckAligncorrect();
	afx_msg void OnEnChangeEditCorrpixel();
	virtual BOOL OnInitDialog();
};
