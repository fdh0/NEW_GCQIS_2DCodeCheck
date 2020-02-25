/************************************************************************
/* Copyright (c) 2008, �������ͼ���Ӿ����޹�˾�Ӿ��㷨��    
/* All rights reserved.             
/*                  
/* �ļ����ƣ�  PropDlg.h
/* ժҪ�� ���ԶԻ���
/*
/* ��ǰ�汾�� 1.0
/* �����ߣ� �˺���
/* ������ڣ� 2010��8��8��
/* ˵���� ���ԶԻ����У��޸Ĳ��������������������������ʵ����
************************************************************************/

#if !defined(AFX_PROPDLG_H__8D2BD3EF_2765_4084_BF87_9E390245E887__INCLUDED_)
#define AFX_PROPDLG_H__8D2BD3EF_2765_4084_BF87_9E390245E887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropDlg.h : header file
//
#include "Check.h"
#include "afxwin.h"
#include "SpacialParaSetup.h"

/////////////////////////////////////////////////////////////////////////////
class CPropDlg : public CBCGPDialog
{
// Construction
public:
	CPropDlg(CWnd* pParent = NULL);   // standard constructor

	s_AlgImageInfo m_sAlgImageInfo;// ͼ����Ϣ
	s_RegionProperty m_sDstRegionProperty;// ������Ϣ
	
	CCheck *m_pChecker;// �ⲿָ��	
	CInnerCheck *m_pTempChecker;// �ڲ�ָ�룬��ʱ������
	s_AlgInitParam m_sAlgInitParam;// ȫ��·��
	CString m_strTaskName;//������
 	
// Dialog Data
	//{{AFX_DATA(CPropDlg)
	enum { IDD = IDD_DIALOG_PROP };
	//CScrollBar	m_scrollVer;
	//CScrollBar	m_scrollHor;
	//CString	m_strCheckResult; 
	float	m_fScaleX;// ��ʾ����x
	float	m_fScaleY;// ��ʾ����y
	//}}AFX_DATA

	BOOL bOcrDeal;
	CSpacialParaSetup *m_dlgSpacialSet;
	BOOL m_bFlagInRegion;	
	BOOL m_bFlagQrloc;
	BOOL m_bFlagOutRange;
	BOOL m_bFlagHorVer;
	s_CheckPrintParam m_sCheckPrintParam;

	// ��ʾ��ʼ�Ի��� [9/14/2015 TIAN]
	CRect m_rectDlg;
	CRect m_rectInitDlg;
	BOOL  m_bClickAdv;
	// ��ʾ [9/17/2013 Denghl]
	int m_nDlgWidth, m_nDlgHeight;
	//HTuple m_lImgW, m_lImgH;
	HTuple m_HWindowID;
	BOOL m_bCheckResult;

	//  [1/27/2015 baiyy]
	BOOL m_bIsTurnedResult;
	BOOL m_bCheckPartCode;//�Ƿ�ʶ�𲿷ֺ�����
	int  m_CodeCharL;
	int  m_CodeCharH;
	HObject m_hZoubanReg;
	HObject m_hZoubanRegL;
	HObject m_hZoubanRegR;
	HObject m_hZoubanRegU;
	HObject m_hZoubanRegD;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	//  [4/6/2017 zb] Ӣ�Ľ���
	int      m_nLanguage;
	CString  m_strLanguagePath;
	CString  m_strControlMapPath;
	CString  m_strMessage;
	char     m_cMessage[256];
	typedef  struct _CONTROL_SET
	{
		int  nControlID;
		CString  strControl;
	}s_ControlSet;

	CString m_strMessagePath;        // ��Ӣ����־����Ϣ����·��
	char m_strMessageText[512][256];
// Implementation
protected:

	// ͼ����ʾ [8/13/2010 dhl]
	int m_nShowImgType;// ͼ����ʾ��ʽ
	int m_nRangV;// ��������ֱ��Χ
	int m_nRangH;// ������ˮƽ��Χ
	int m_nCurPosx;// ��ʼ��
	int m_nCurPosy;// ��ֹ��
	int m_nImgWid;// ͼ����
	int m_nImgHei;// ͼ��߶�
	int m_nImageBit;// ͼ��λ��
	CRect m_rectImg;// �ͻ�����
	long m_lWindowHeight;// ���ڸ߶�
	long m_lWindowWidth;// ���ڿ��
	PBYTE m_pImageSrc;// ͼ��ͷָ��
	HObject m_hImgSrc;

	// ��ͼ����
	void DrawImage();

	// ��ʾ
	int ShowImage(CDC * pDC,unsigned char *lpDest, int nDispx, int nDispy, int nDispWidth, int nDispHeight,
	                	int nImagex, int nImagey, int nImageWidth, int nImageHeight, 
						int nWidth, int nHeight, int BitsPerPixel);

	// ת��Ϊ��ʾͼ��
	BOOL TransAlgImageToBuff(s_AlgImageInfo sAlgImageInfo,char *pDestImg,CRect rcCut);

	// ��ÿؼ���С
	void GetDlgItemRect(int nID,CWnd *wndParent,LPRECT Client);
	bool GetHWindowID(int nControlID, HTuple& lHWindowID,BOOL bFlag = TRUE);

	// Generated message map functions
	//{{AFX_MSG(CPropDlg)
	virtual void OnOK();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonTest();
	afx_msg void OnButtonSave();
	afx_msg void OnRadioAuto();
	afx_msg void OnRadioNorm();
	afx_msg void OnRadioSetrate();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnCheckRotate();
	afx_msg void OnButtonReaddefault();
	afx_msg void OnButtonWritedefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	CComboBox m_comProductType;
//	int m_nProductType;
	//int m_nProductNum;// ��֧�ֲ�Ʒ������� [4/7/2012 Denghl]
	//std::vector<CString> m_vcstrAllProType;// ���в�Ʒ����
//	afx_msg void OnCbnSelchangeComboProductType();
	afx_msg void OnBnClickedButtonSettype();
	afx_msg void OnBnClickedButtonParaSet();
	CComboBox m_comCodeType;
//	CString m_strWord;
//	afx_msg void OnEnChangeEditWord();
	afx_msg void OnCbnSelchangeComboCodetype();
	CComboBox m_comChannel;
	afx_msg void OnCbnSelchangeComboChannel();
	BOOL m_bCheckAllMod;
	afx_msg void OnBnClickedCheckAll();
	BOOL m_bCheckGao;
	afx_msg void OnBnClickedCheckGao();
	double m_fScaleHor;
	double m_fScaleVer;
	afx_msg void OnEnChangeEditHorscal();
	afx_msg void OnEnChangeEditVerscal();
	afx_msg void OnEnKillfocusEditHorscal();
	afx_msg void OnEnKillfocusEditVerscal();
	afx_msg void OnBnClickedButtonSetdeng();
	CComboBox m_comDengSet;
	afx_msg void OnCbnSelchangeComboDengji();
	BOOL m_bCheckDengji;
	afx_msg void OnBnClickedCheckDengji();
	CComboBox m_comGBType;
	afx_msg void OnCbnSelchangeComboGbtype();	
	afx_msg void OnBnClickedCheckCode();
	afx_msg void OnEnChangeEditCodel();
	afx_msg void OnEnChangeEditCodeh();
	afx_msg void OnBnClickedCheckIsturnedresult();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnDeltaposSpinMaxpixel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCodel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCodeh(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinHor(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinVer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedDrawLoc();
	afx_msg void OnStnClickedDrawOffset();
	afx_msg void OnBnClickedBtnAdvancepara();
	afx_msg void OnBnClickedCheckHorver();
	afx_msg void OnBnClickedCheckAligncorrect();
	afx_msg void OnEnChangeEditCorrpixel();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	//BCG Control
	CBCGPStatic m_labShowMode;
	CBCGPStatic m_labType;
	CBCGPStatic m_labChannel;
	CBCGPStatic m_labMaxPixel;
	CBCGPStatic m_labCheckRange;
	CBCGPStatic m_labHor;
	CBCGPStatic m_labVer;
	CBCGPStatic m_labCheckGrade;
	CBCGPStatic m_labCheckStd;
	CBCGPStatic m_labTitle;
	CBCGPStatic m_labTo;
	CBCGPStatic m_labDrawLoc;
	CBCGPStatic m_labDrawOffset;

	CBCGPButton m_cmdRadioAuto;
	CBCGPButton m_cmdRadioNorm;
	CBCGPButton m_cmdRadioSetrate;
	CBCGPButton m_cmdAdvancePara;
	CBCGPButton m_cmdTest;
	CBCGPButton m_cmdSave;
	CBCGPButton m_cmdExit;
	CBCGPButton m_cmdReadDefault;
	CBCGPButton m_cmdWriteDefault;
	CBCGPButton m_cmdCheckGao;
	CBCGPButton m_cmdCheckAll;
	CBCGPButton m_cmdCheckTurnResult;
	CBCGPButton m_cmdCheckHorVer;
	CBCGPButton m_cmdCheckAlignCor;
	CBCGPButton m_cmdCheckCode;
	CBCGPButton m_cmdCheckDengji;
	CBCGPButton m_cmdSetDengji;
	
	BOOL m_bCheckHorVer;
	BOOL m_bCheckAlignCor;
	double m_dCorrPixel;
	CString m_strCheckResult;
	HFONT m_hFont1;
	
	//ͼ����ʾ����
	int   m_nZDelta;//��껬��ֵ���Զ��壩
	BOOL  m_bIsLButtonDown;
	BOOL  m_bIsMouseWheel;
	POINT ptOri,ptMove;
	int   m_nMoveX,m_nMoveY;
	int   m_nMoveXlast,m_nMoveYlast;
	afx_msg void OnCbnSelchangeComboPolar();
	CComboBox m_comPolar;
	CString m_strPolar;
//	CString m_strPolarChinese;
	CBCGPStatic m_labPolar;
	afx_msg void OnBnClickedCheckImagesmooth();
	BOOL m_bIsImageSmooth;
	CBCGPButton m_cmdCheckImageSmooth;
	BOOL m_bIsZoomCheck;
	afx_msg void OnBnClickedCheckImagezoom();
	CBCGPButton m_cmdCheckImageZoom;
	double m_fZoomRadio;
	afx_msg void OnEnKillfocusEditSfradio();
	CBCGPStatic m_bcgBilv;
	UINT m_nResultNum;
	afx_msg void OnEnKillfocusEditResultnum();
	CBCGPStatic m_labResultNum;
	//CBCGPEdit m_editResult;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPDLG_H__8D2BD3EF_2765_4084_BF87_9E390245E887__INCLUDED_)
