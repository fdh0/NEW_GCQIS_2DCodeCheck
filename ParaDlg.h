/************************************************************************
/* Copyright (c) 2008, 北京大恒图像视觉有限公司视觉算法部    
/* All rights reserved.             
/*                  
/* 文件名称：  ParaDlg.h
/* 摘要： 参数对话框。
/*
/* 当前版本： 1.0
/* 创建者： 邓红丽
/* 完成日期： 2010年8月8日
/* 说明： 参数对话框中，修改参数、保存参数、应用所有以及参数分析和现实函数
************************************************************************/
#if !defined(AFX_PARADLG_H__1369F846_2240_419D_9B01_C9FF8BC39475__INCLUDED_)
#define AFX_PARADLG_H__1369F846_2240_419D_9B01_C9FF8BC39475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParaDlg.h : header file
//
#include "Check.h"
#include "afxwin.h"
#include "SpacialParaSetup.h"

class CSpacialParaSetup;
/////////////////////////////////////////////////////////////////////////////
// CParaDlg dialog

class CParaDlg : public CBCGPDialog
{
// Construction
public:
	CParaDlg(CWnd* pParent = NULL);   // standard constructor

	s_AlgImageInfo m_sAlgImageInfo;// 图像信息
//	s_RegionProperty m_sDstRegionProperty;// 区域信息	
	S_SingleCell_Or_RegionsInfo m_sSingleCellRegionsInfo;
	CCheck *m_pChecker;// 外部指针	
	CCheck *m_pTempChecker;// 内部指针，临时测试用
	s_AlgInitParam m_sAlgInitParam;// 全局路径

	int m_nSetRegionType;

	s_SigleCell_CheckResult m_sSingleCellCheckResult;//接收结果等

	BOOL bOcrDeal;

	BOOL m_bAuthority;//是否有权限修改参数
	BOOL m_bIsModify;

	CSpacialParaSetup *m_dlgSpacialSet;
	BOOL m_bFlagInRegion;	
	BOOL m_bFlagQrloc;
	BOOL m_bFlagOutRange;
	BOOL m_bFlagHorVer;
	s_CheckPrintParam m_sCheckPrintParam;
	
	// 显示初始对话框 [9/14/2015 TIAN]
	CRect m_rectDlg;
	CRect m_rectInitDlg;
	BOOL  m_bClickAdv;

	// 显示 [9/17/2013 Denghl]
	int   m_nDlgWidth, m_nDlgHeight;
//	HTuple m_lImgW, m_lImgH;
	HTuple m_HWindowID;
	BOOL m_bCheckResult;
	s_SysAuthority m_sSysAuthority;

// Dialog Data
	//{{AFX_DATA(CParaDlg)
	enum { IDD = IDD_DIALOG_PRAR };
	//CScrollBar	m_scrollVer;
	//CScrollBar	m_scrollHor;
	//CString	m_strCheckResult;
	float	m_fScaleX;
	float	m_fScaleY;
	//}}AFX_DATA

	//  [12/2/2014 baiyy]
	BOOL m_bCheckResultMode;// 检测结果模式，默认为0，存小开结果;1为缺陷小图结果
	//BOOL m_bAlignCorrected;//是否进行错位校正
	//int  m_nCorrectPixels;
	BOOL m_bCheckPartCode;//是否识别部分号码结果
	int  m_CodeCharL;
	int  m_CodeCharH;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	//  [4/7/2017 zb] 英文界面
	int      m_nLanguage;
	CString  m_strLanguagePath;
	CString m_strDefaultParaPath;
	CString  m_strControlMapPath;
	CString  m_strMessage;
	char     m_cMessage[256];
	typedef  struct _CONTROL_SET
	{
		int  nControlID;
		CString  strControl;
	}s_ControlSet;

	CString m_strMessagePath;        // 中英文日志或消息配置路径
	char m_strMessageText[512][256];
// Implementation
protected:
	
	// 图像显示 [8/13/2010 dhl]
	int m_nShowImgType;// 图像显示方式
	int m_nRangV;// 滚动条垂直范围
	int m_nRangH;// 滚动条水平范围
	int m_nCurPosx;// 起始点
	int m_nCurPosy;// 终止点
	int m_nImgWid;// 图像宽度
	int m_nImgHei;// 图像高度
	int m_nImageBit;// 图像位数
	CRect m_rectImg;// 客户区域
	long m_lWindowHeight;// 窗口高度
	long m_lWindowWidth;// 窗口宽度
	PBYTE m_pImageSrc;// 图像头指针
	HObject m_hImgSrc; 
	//  [11/13/2015 zl]
	HObject m_hImgOri;//原始图
	int  m_nChannel;  //图像通道
	//  [11/4/2015 zl]
	public:
	HObject m_hZoubanReg;

	HObject m_hZoubanRegL;
	HObject m_hZoubanRegR;
	HObject m_hZoubanRegU;
	HObject m_hZoubanRegD;
	
	// 画图函数
	void DrawImage();
	
	// 显示
	int ShowImage(CDC * pDC,unsigned char *lpDest, int nDispx, int nDispy, int nDispWidth, int nDispHeight,
		int nImagex, int nImagey, int nImageWidth, int nImageHeight, 
		int nWidth, int nHeight, int BitsPerPixel);
	
	// 转换为显示图像
	BOOL TransAlgImageToBuff(s_AlgImageInfo sAlgImageInfo,char *pDestImg,CRect rcCut);
	
	// 获得控件大小
	void GetDlgItemRect(int nID,CWnd *wndParent,LPRECT Client);
    bool GetHWindowID(int nControlID, HTuple& lHWindowID,BOOL bFlag = TRUE);

	// Generated message map functions
	//{{AFX_MSG(CParaDlg)
	virtual void OnOK();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTest();
	afx_msg void OnButtonSave();
	afx_msg void OnDestroy();
	afx_msg void OnButtonSaveall();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRadioSetrate();
	afx_msg void OnRadioNorm();
	afx_msg void OnRadioAuto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonParaSet();
	afx_msg void OnBnClickedButtonModifyMode();
	afx_msg void OnBnClickedButtonRecord();
	CString m_strexpCode;
	CString m_strResultCode;
	afx_msg void OnEnChangeEditExpcode();
	afx_msg void OnEnKillfocusEditExpcode();
	afx_msg void OnBnClickedButtonSettight();
	CComboBox m_comRegionType;
	CComboBox m_comRegionID;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonTypeSet();
	afx_msg void OnBnClickedButtonOnlyread();
	afx_msg void OnBnClickedButtonChar();
	afx_msg void OnBnClickedButtonLoookChar();
//	CComboBox m_comProductType;
//	afx_msg void OnCbnSelchangeComboProductType();
	CComboBox m_comCodeType;
	afx_msg void OnCbnSelchangeComboCodetype();
	//CString m_strWord;
	CComboBox m_comChannel;
	afx_msg void OnCbnSelchangeComboChannel();
	CString m_strExpCode;
	afx_msg void OnEnChangeEdit2();
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
	//afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheckResultmode();
	afx_msg void OnBnClickedCheckCode();
	afx_msg void OnEnChangeEditCodel();
	afx_msg void OnEnChangeEditCodeh();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedCheckHorver();
	afx_msg void OnBnClickedCheckAligncorrect();
	afx_msg void OnEnChangeEditCorrpixel();
	afx_msg void OnDeltaposSpinMaxpixel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCodel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinCodeh(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinHor(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinVer(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedDrawLoc();
	afx_msg void OnStnClickedDrawOffset();
	afx_msg void OnBnClickedButtonAdvpara();
	
	//BCG Control
	CBCGPStatic m_labTitle;
	CBCGPStatic m_labShowMode;
	CBCGPStatic m_labExpCode;
	CBCGPStatic m_labRecCode;
	CBCGPStatic m_labType;
	CBCGPStatic m_labChannel;
	CBCGPStatic m_labMaxpixel;
	CBCGPStatic m_labRecRange;
	CBCGPStatic m_labHor;
	CBCGPStatic m_labVer;
	CBCGPStatic m_labDengji;
	CBCGPStatic m_labBiaoZhun;
	CBCGPStatic m_labTo;

	CBCGPButton m_cmdRadioAuto;
	CBCGPButton m_cmdRadioNorm;
	CBCGPButton m_cmdRadioSetrate;
	
	CBCGPButton m_cmdTest;
	CBCGPButton m_cmdSave;
	CBCGPButton m_cmdSaveAll;
	CBCGPButton m_cmdExit;
	CBCGPButton m_cmdSetDengji;

	CBCGPButton m_cmdCheckGao;
	CBCGPButton m_cmdCheckAll;
	CBCGPButton m_cmdCheckRstMode;
	CBCGPButton m_cmdCheckHorVer;
	CBCGPButton m_cmdCheckAlignCor;
	CBCGPButton m_cmdCheckCode;
	CBCGPButton m_cmdCheckDengji;
	
	BOOL m_bCheckHorVer;
	BOOL m_bCheckAlignCor;
	double m_dCorrPixel;
	
	
	
	CBCGPStatic m_labDrawLoc;
	CBCGPStatic m_labDrawOffset;
	CString m_strCheckResult;
	HFONT m_hFont1;
	
	CBCGPButton m_cmdAdvPara;


	//图像显示操作
	int   m_nZDelta;//鼠标滑动值（自定义）
	BOOL  m_bIsLButtonDown;
	BOOL  m_bIsMouseWheel;
	POINT ptOri,ptMove;
	int   m_nMoveX,m_nMoveY;
	int   m_nMoveXlast,m_nMoveYlast;

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CBCGPGroup m_grpShowImage;
	afx_msg void OnCbnSelchangeComboPolar();
	CComboBox m_comPolar;
	CString m_strPolar;
//	CString m_strPolarChinese;
	CBCGPStatic m_labPolar;
	BOOL m_bIsImageSmooth;
	afx_msg void OnBnClickedCheckImagesmooth();
	CBCGPButton m_cmdCheckImageSmooth;
	BOOL m_bIsZoomCheck;
	afx_msg void OnBnClickedCheckImagezoom();
	CBCGPButton m_cmdCheckImageZoom;
	double m_fZoomRadio;
	afx_msg void OnEnKillfocusEditSfradio();
	CBCGPStatic m_bcgBiLv;
	UINT m_nResultNum;
	afx_msg void OnEnKillfocusEditResultnum();
	CBCGPStatic m_labResultNum;
	//CBCGPEdit m_editResultNum;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARADLG_H__1369F846_2240_419D_9B01_C9FF8BC39475__INCLUDED_)
