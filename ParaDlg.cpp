// ParaDlg.cpp : implementation file
//
/************************************************************************/
/* Copyright (c) 2008, 北京图像视觉技术有限公司视觉算法部     
/* All rights reserved.													
/*																		
/* 文件名称： ParaDlg.cpp												
/* 摘要： 检测参数对话框类函数集
/*
/* 作者： 邓红丽
/* 完成日期： 2010年08月08日
/************************************************************************/
#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "ParaDlg.h"
#include "DialogDengji.h"
#include "DialogDengji2006.h"
#include "DialogDengjiPDF417.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParaDlg dialog


CParaDlg::CParaDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CParaDlg::IDD, pParent)
	, m_strexpCode(_T(""))
	, m_strResultCode(_T(""))
	/*, m_strWord(_T(""))*/
	, m_strExpCode(_T(""))
	, m_bCheckAllMod(FALSE)
	, m_bCheckGao(FALSE)
	, m_fScaleHor(0)
	, m_fScaleVer(0)
	, m_bCheckDengji(FALSE)
	,m_bCheckResultMode(TRUE)
	, m_bCheckHorVer(FALSE)
	, m_bCheckAlignCor(FALSE)
	, m_dCorrPixel(0)
	, m_strCheckResult(_T(""))
	, m_strPolar(_T(""))
	, m_bIsImageSmooth(FALSE)
	, m_bIsZoomCheck(FALSE)
	, m_fZoomRadio(0)
	, m_nResultNum(0)
{
	//{{AFX_DATA_INIT(CParaDlg)
	//m_strCheckResult = _T("");
	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;
	//}}AFX_DATA_INIT
	m_pImageSrc = NULL;
	m_pTempChecker = NULL;
	m_nShowImgType = 0;
	bOcrDeal = FALSE;

	m_dlgSpacialSet = NULL;
	m_bFlagInRegion = TRUE;	
	m_bFlagQrloc = TRUE;
	m_bFlagOutRange = TRUE;
	m_bFlagHorVer = TRUE;
	m_dlgSpacialSet = NULL;
	m_bCheckResult = FALSE;	
	m_bAuthority = FALSE;
	m_bIsModify = FALSE;

	m_bCheckPartCode = FALSE;
	m_CodeCharL = 0;
	m_CodeCharH = 0;
	m_nZDelta   = 0;
	m_nMoveX    = 0;
	m_nMoveY    = 0;
	m_bIsLButtonDown = FALSE;
	m_bIsMouseWheel  = FALSE;
	m_nMoveXlast=0;
	m_nMoveYlast=0;
	// 初始化不显示高级参数 [9/14/2015 TIAN]
	m_bClickAdv = FALSE;

	//  [11/6/2015 TIAN]
	GenEmptyRegion(&m_hZoubanRegL);
	GenEmptyRegion(&m_hZoubanRegR);
	GenEmptyRegion(&m_hZoubanRegU);
	GenEmptyRegion(&m_hZoubanRegD);
	GenEmptyObj(&m_hImgOri);
	//  [10/10/2015 TIAN]
	m_nChannel = 0;
	//BCG Control
	//CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS (CBCGPVisualManagerVS2010));
	EnableVisualManagerStyle(TRUE,TRUE);
	//m_strPolarChinese = _T("");
	//  [4/7/2017 zb]
	m_nLanguage         = 0;
	m_strDefaultParaPath= _T("");
	m_strLanguagePath   = _T("");
	m_strControlMapPath = _T("");
	m_strMessage        = _T("");
	memset(m_cMessage,0,256);
	m_strMessagePath= _T("");
	memset(m_strMessageText,0,512*256*sizeof(char));
}

void CParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParaDlg)
	//DDX_Control(pDX, IDC_SCROLLBAR_VER, m_scrollVer);
	//DDX_Control(pDX, IDC_SCROLLBAR_HOR, m_scrollHor);
	//DDX_Text(pDX, IDC_STATIC_CHECKRESULT, m_strCheckResult);
	//DDX_Text(pDX, IDC_EDIT_SHOWRATE_X, m_fScaleX);
	//DDX_Text(pDX, IDC_EDIT_SHOWRATE_Y, m_fScaleY);
	//}}AFX_DATA_MAP
	//DDX_Text(pDX, IDC_EDIT_EXPCODE, m_strexpCode);
	//	DDV_MaxChars(pDX, m_strexpCode, 15);
	DDX_Text(pDX, IDC_EDIT_RESULT_CODE, m_strResultCode);
	//DDV_MaxChars(pDX, m_strResultCode, 15);
	//DDX_Control(pDX, IDC_COMBO_REGIONTYPE, m_comRegionType);
	//DDX_Control(pDX, IDC_COMBO_REGIONID, m_comRegionID);
	//DDX_Control(pDX, IDC_COMBO_PRODUCT_TYPE, m_comProductType);
	DDX_Control(pDX, IDC_COMBO_CODETYPE, m_comCodeType);
	//DDX_Text(pDX, IDC_EDIT_WORD, m_strWord);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_comChannel);
	DDX_Text(pDX, IDC_EDIT2, m_strexpCode);
	DDX_Check(pDX, IDC_CHECK_ALL, m_bCheckAllMod);
	DDX_Check(pDX, IDC_CHECK_GAO, m_bCheckGao);
	DDX_Text(pDX, IDC_EDIT_HORSCAL, m_fScaleHor);
	DDX_Text(pDX, IDC_EDIT_VERSCAL, m_fScaleVer);
	DDX_Control(pDX, IDC_COMBO_DENGJI, m_comDengSet);
	DDX_Check(pDX, IDC_CHECK_DENGJI, m_bCheckDengji);
	DDX_Control(pDX, IDC_COMBO_GBTYPE, m_comGBType);

	DDX_Check(pDX, IDC_CHECK_RESULTMODE, m_bCheckResultMode);
	DDX_Check(pDX, IDC_CHECK_CODE, m_bCheckPartCode);
	DDX_Text(pDX, IDC_EDIT_CODEL, m_CodeCharL);
	DDV_MinMaxInt(pDX, m_CodeCharL, 0, 1024);
	DDX_Text(pDX, IDC_EDIT_CODEH, m_CodeCharH);
	DDV_MinMaxInt(pDX, m_CodeCharH, 0, 1024);

	DDX_Control(pDX, IDC_STATIC_SHOWMODE, m_labShowMode);
	DDX_Control(pDX, IDC_RADIO_AUTO, m_cmdRadioAuto);
	DDX_Control(pDX, IDC_RADIO_NORM, m_cmdRadioNorm);
	DDX_Control(pDX, IDC_RADIO_SETRATE, m_cmdRadioSetrate);
	DDX_Control(pDX, IDC_STATIC_EXPCODE, m_labExpCode);
	DDX_Control(pDX, IDC_STATIC_RECCODE, m_labRecCode);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_cmdTest);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_cmdSave);
	DDX_Control(pDX, IDC_BUTTON_SAVEALL, m_cmdSaveAll);
	DDX_Control(pDX, IDC_BTN_EXIT, m_cmdExit);
	DDX_Control(pDX, IDC_STATIC_2DTYPE, m_labType);
	DDX_Control(pDX, IDC_STATIC_CHAN, m_labChannel);
	DDX_Control(pDX, IDC_CHECK_GAO, m_cmdCheckGao);
	DDX_Control(pDX, IDC_CHECK_ALL, m_cmdCheckAll);
	DDX_Control(pDX, IDC_CHECK_RESULTMODE, m_cmdCheckRstMode);
	//DDX_Control(pDX, IDC_CHECK_HORVER, m_cmdCheckHorVer);
	DDX_Control(pDX, IDC_CHECK_ALIGNCORRECT, m_cmdCheckAlignCor);
	DDX_Control(pDX, IDC_STATIC_MAXPIXEL, m_labMaxpixel);
	DDX_Control(pDX, IDC_CHECK_CODE, m_cmdCheckCode);
	DDX_Control(pDX, IDC_STATIC_RECRANGE, m_labRecRange);
	DDX_Control(pDX, IDC_STATIC_HOR, m_labHor);
	DDX_Control(pDX, IDC_STATIC_VER, m_labVer);
	DDX_Control(pDX, IDC_CHECK_DENGJI, m_cmdCheckDengji);
	DDX_Control(pDX, IDC_STATIC_DENGJI, m_labDengji);
	DDX_Control(pDX, IDC_STATIC_BIAOZHUN, m_labBiaoZhun);
	DDX_Control(pDX, IDC_BUTTON_SETDENG, m_cmdSetDengji);
	//DDX_Check(pDX, IDC_CHECK_HORVER, m_bCheckHorVer);
	DDX_Check(pDX, IDC_CHECK_ALIGNCORRECT, m_bCheckAlignCor);
	DDX_Text(pDX, IDC_EDIT_CORRPIXEL, m_dCorrPixel);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_labTitle);
	DDX_Control(pDX, IDC_STATIC_TO, m_labTo);
	DDX_Control(pDX, IDC_DRAW_LOC, m_labDrawLoc);
	DDX_Control(pDX, IDC_DRAW_OFFSET, m_labDrawOffset);
	DDX_Text(pDX, IDC_EDIT_CHECKRESULT, m_strCheckResult);
	DDX_Control(pDX, IDC_BUTTON_ADVPARA, m_cmdAdvPara);
	DDX_Control(pDX, IDC_STATIC_SHOWIMAGE, m_grpShowImage);
	DDX_Control(pDX, IDC_COMBO_POLAR, m_comPolar);
	//DDX_CBString(pDX, IDC_COMBO_POLAR, m_strPolar);
	DDX_Control(pDX, IDC_STATIC_POLAR, m_labPolar);
	DDX_Check(pDX, IDC_CHECK_IMAGESMOOTH, m_bIsImageSmooth);
	DDX_Control(pDX, IDC_CHECK_IMAGESMOOTH, m_cmdCheckImageSmooth);
	DDX_Check(pDX, IDC_CHECK_IMAGEZOOM, m_bIsZoomCheck);
	DDX_Control(pDX, IDC_CHECK_IMAGEZOOM, m_cmdCheckImageZoom);
	DDX_Text(pDX, IDC_EDIT_SFRADIO, m_fZoomRadio);
	DDX_Control(pDX, IDC_STATICSFBL, m_bcgBiLv);
	DDX_Text(pDX, IDC_EDIT_RESULTNUM, m_nResultNum);
	DDV_MinMaxInt(pDX, m_nResultNum, 0, 500);
	DDX_Control(pDX, IDC_STATIC_RESULTNUM, m_labResultNum);
	//DDX_Control(pDX, IDC_EDIT_RESULTNUM, m_editResultNum);
}


BEGIN_MESSAGE_MAP(CParaDlg, CBCGPDialog)
	//{{AFX_MSG_MAP(CParaDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SAVEALL, OnButtonSaveall)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_RADIO_SETRATE, OnRadioSetrate)
	ON_BN_CLICKED(IDC_RADIO_NORM, OnRadioNorm)
	ON_BN_CLICKED(IDC_RADIO_AUTO, OnRadioAuto)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PARA_SET, &CParaDlg::OnBnClickedButtonParaSet)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_MODE, &CParaDlg::OnBnClickedButtonModifyMode)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, &CParaDlg::OnBnClickedButtonRecord)
	ON_EN_CHANGE(IDC_EDIT_EXPCODE, &CParaDlg::OnEnChangeEditExpcode)
	ON_EN_KILLFOCUS(IDC_EDIT_EXPCODE, &CParaDlg::OnEnKillfocusEditExpcode)
	ON_BN_CLICKED(IDC_BUTTON_SETTIGHT, &CParaDlg::OnBnClickedButtonSettight)
	ON_BN_CLICKED(IDCANCEL, &CParaDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_TYPE_SET, &CParaDlg::OnBnClickedButtonTypeSet)
	ON_BN_CLICKED(IDC_BUTTON_ONLYREAD, &CParaDlg::OnBnClickedButtonOnlyread)
	ON_BN_CLICKED(IDC_BUTTON_CHAR, &CParaDlg::OnBnClickedButtonChar)
	ON_BN_CLICKED(IDC_BUTTON_LOOOK_CHAR, &CParaDlg::OnBnClickedButtonLoookChar)
	//ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_TYPE, &CParaDlg::OnCbnSelchangeComboProductType)
	ON_CBN_SELCHANGE(IDC_COMBO_CODETYPE, &CParaDlg::OnCbnSelchangeComboCodetype)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, &CParaDlg::OnCbnSelchangeComboChannel)
	ON_EN_CHANGE(IDC_EDIT2, &CParaDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_CHECK_ALL, &CParaDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_CHECK_GAO, &CParaDlg::OnBnClickedCheckGao)
	ON_EN_CHANGE(IDC_EDIT_HORSCAL, &CParaDlg::OnEnChangeEditHorscal)
	ON_EN_CHANGE(IDC_EDIT_VERSCAL, &CParaDlg::OnEnChangeEditVerscal)
	ON_EN_KILLFOCUS(IDC_EDIT_HORSCAL, &CParaDlg::OnEnKillfocusEditHorscal)
	ON_EN_KILLFOCUS(IDC_EDIT_VERSCAL, &CParaDlg::OnEnKillfocusEditVerscal)
	ON_BN_CLICKED(IDC_BUTTON_SETDENG, &CParaDlg::OnBnClickedButtonSetdeng)
	ON_CBN_SELCHANGE(IDC_COMBO_DENGJI, &CParaDlg::OnCbnSelchangeComboDengji)
	ON_BN_CLICKED(IDC_CHECK_DENGJI, &CParaDlg::OnBnClickedCheckDengji)
	ON_CBN_SELCHANGE(IDC_COMBO_GBTYPE, &CParaDlg::OnCbnSelchangeComboGbtype)
	//ON_BN_CLICKED(IDC_BUTTON1, &CParaDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK_RESULTMODE, &CParaDlg::OnBnClickedCheckResultmode)
	ON_BN_CLICKED(IDC_CHECK_CODE, &CParaDlg::OnBnClickedCheckCode)
	ON_EN_CHANGE(IDC_EDIT_CODEL, &CParaDlg::OnEnChangeEditCodel)
	ON_EN_CHANGE(IDC_EDIT_CODEH, &CParaDlg::OnEnChangeEditCodeh)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CParaDlg::OnBnClickedBtnExit)
	//ON_BN_CLICKED(IDC_CHECK_HORVER, &CParaDlg::OnBnClickedCheckHorver)
	ON_BN_CLICKED(IDC_CHECK_ALIGNCORRECT, &CParaDlg::OnBnClickedCheckAligncorrect)
	ON_EN_CHANGE(IDC_EDIT_CORRPIXEL, &CParaDlg::OnEnChangeEditCorrpixel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MAXPIXEL, &CParaDlg::OnDeltaposSpinMaxpixel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CODEL, &CParaDlg::OnDeltaposSpinCodel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CODEH, &CParaDlg::OnDeltaposSpinCodeh)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HOR, &CParaDlg::OnDeltaposSpinHor)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_VER, &CParaDlg::OnDeltaposSpinVer)
	ON_STN_CLICKED(IDC_DRAW_LOC, &CParaDlg::OnStnClickedDrawLoc)
	ON_STN_CLICKED(IDC_DRAW_OFFSET, &CParaDlg::OnStnClickedDrawOffset)
	ON_BN_CLICKED(IDC_BUTTON_ADVPARA, &CParaDlg::OnBnClickedButtonAdvpara)
	ON_WM_MOUSEWHEEL()
	ON_CBN_SELCHANGE(IDC_COMBO_POLAR, &CParaDlg::OnCbnSelchangeComboPolar)
	ON_BN_CLICKED(IDC_CHECK_IMAGESMOOTH, &CParaDlg::OnBnClickedCheckImagesmooth)
	ON_BN_CLICKED(IDC_CHECK_IMAGEZOOM, &CParaDlg::OnBnClickedCheckImagezoom)
	ON_EN_KILLFOCUS(IDC_EDIT_SFRADIO, &CParaDlg::OnEnKillfocusEditSfradio)
	ON_EN_KILLFOCUS(IDC_EDIT_RESULTNUM, &CParaDlg::OnEnKillfocusEditResultnum)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParaDlg message handlers

void CParaDlg::OnOK() 
{
	// TODO: Add extra validation here

	CBCGPDialog::OnOK();
}

// 画图
void CParaDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	ClearWindow(m_HWindowID);
	if (!bOcrDeal)
	{
		DrawImage();
		std::list<s_InnerCheckInfo>::iterator itInfo;
		itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
		std::list<CInnerCheck>::iterator itInnerCheck;
		itInnerCheck = itInfo->lstInnerCheck.begin();
		if (m_bFlagHorVer||m_bFlagInRegion||m_bFlagOutRange||m_bFlagQrloc)
		{
			m_dlgSpacialSet->GetTempCheckParam();
			m_sCheckPrintParam = m_dlgSpacialSet->m_sCheckPrintParam;
			itInnerCheck->m_sCheckPrintParam = m_sCheckPrintParam;	    
		}
		SetColor(m_HWindowID,"blue");
		if(itInnerCheck->m_sCheckPrintParam.sZouBanPara.bValid)
		{
			//if(itInnerCheck->m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan)
			{
				if(itInnerCheck->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft)
				{
					DispObj(m_hZoubanRegL,m_HWindowID);
					//write_region(m_hZoubanRegL,"D://m_hZoubanRegL.reg");
				}
				if(itInnerCheck->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight)
				{
					DispObj(m_hZoubanRegR,m_HWindowID);
					//write_region(m_hZoubanRegR,"D://m_hZoubanRegR.reg");
				}
				if(itInnerCheck->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp)
				{
					DispObj(m_hZoubanRegU,m_HWindowID);
					//write_region(m_hZoubanRegU,"D://m_hZoubanRegU.reg");
				}
				if(itInnerCheck->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown)
				{
					DispObj(m_hZoubanRegD,m_HWindowID);
					//write_region(m_hZoubanRegD,"D://m_hZoubanRegD.reg");
				}
			}
		}
	}
	else
	{
		std::list<s_InnerCheckInfo>::iterator itInfo;
		itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
		std::list<CInnerCheck>::iterator itInnerCheck;
		itInnerCheck = itInfo->lstInnerCheck.begin();
		// 显示实际检测旋转后的图像 [9/17/2013 Denghl]
		if (itInnerCheck->m_bCheckPrint&&m_bCheckResult)
		{
			HObject hTempImg;
			HTuple hWid,hHei;
			GetImageSize(itInnerCheck->m_ImageRotate,&hWid,&hHei);
			//SetPart(m_HWindowID, 0, 0, hHei-1,hWid-1);	
			MirrorImage(itInnerCheck->m_ImageRotate,&hTempImg,"row");
			DispObj(hTempImg,m_HWindowID);
			SetColor(m_HWindowID, "green");	
			HObject hShowReg;
			MirrorRegion(itInnerCheck->m_RegionAffineTrans,&hShowReg,"row",hHei);
			DispObj(hShowReg,m_HWindowID);
			
			// 显示 [9/17/2013 Denghl]
			if (itInnerCheck->m_sCheckPrintParam.sInHorverPara.bValid)
			{
				SetColor(m_HWindowID, "blue");
				MirrorRegion(itInnerCheck->m_InWhiteLine,&hShowReg,"row",hHei);
				DispObj(hShowReg,m_HWindowID);

				//////////////////////////////////////////////////////////////////////////
				HTuple lentemp,lenVer;
				TupleLength(itInnerCheck->hv_Indices,&lentemp);
				TupleLength(itInnerCheck->hv_IndicesVer,&lenVer);
				//write_tuple(itInnerCheck->hv_Indices,"d://hv_Indices.tup");
				int HorStart = -1,VerStart=-1;
				if (lentemp>0)
				{
					HorStart = itInnerCheck->hv_Indices[0].D();
				}
				if (lenVer>0)
				{
					VerStart = itInnerCheck->hv_IndicesVer[0].D();
				}
				if (HorStart>=0||VerStart>=0)
				{
					HTuple hRow1,hRow2,hCol1,hCol2;
					SmallestRectangle1(itInnerCheck->m_InRegion,&hRow1,&hCol1,&hRow2,&hCol2);
					HTuple len;

					HObject hunion;
					GenEmptyRegion(&hunion);
					TupleLength(itInnerCheck->hv_Indices,&len);
					if (len>0&&itInnerCheck->hv_Indices[0].D()>=0)
					{					
						for (int i=0;i<len;i++)
						{
							HObject hReg;
							GenRectangle1(&hReg,hRow1+itInnerCheck->hv_Indices[i].I(),hCol1,hRow1+itInnerCheck->hv_Indices[i].I(),hCol2);
							Union2(hunion,hReg,&hunion);
						}
					}
					TupleLength(itInnerCheck->hv_IndicesVer,&len);
					if (len>0&&itInnerCheck->hv_IndicesVer[0].D()>=0)
					{					
						for (int i=0;i<len;i++)
						{
							HObject hReg;
							GenRectangle1(&hReg,hRow1,hCol1+itInnerCheck->hv_IndicesVer[i].I(),hRow2,hCol1+itInnerCheck->hv_IndicesVer[i].I());
							Union2(hunion,hReg,&hunion);
						}
					}
					SetColor(m_HWindowID, "yellow");
					MirrorRegion(hunion,&hShowReg,"row",hHei);
					DispObj(hShowReg,m_HWindowID);
				}				

			}
			if (itInnerCheck->m_sCheckPrintParam.sOutRangPara.bValid)//显示区域外脏
			{
				SetColor(m_HWindowID, "red");
				MirrorRegion(itInnerCheck->m_RegionOutRang,&hShowReg,"row",hHei);
				DispObj(hShowReg,m_HWindowID);

				SetColor(m_HWindowID, "blue");
				MirrorRegion(itInnerCheck->m_RegionOutRect,&hShowReg,"row",hHei);
				DispObj(hShowReg,m_HWindowID);
			}
			if (itInnerCheck->m_sCheckPrintParam.sInRegionPara.bValid)
			{
				SetColor(m_HWindowID, "blue");
				MirrorRegion(itInnerCheck->m_InRegionQue,&hShowReg,"row",hHei);
				DispObj(hShowReg,m_HWindowID);
			}

			// 显示走版区域 [7/31/2014 Denghl]
			if (itInnerCheck->m_sCheckPrintParam.sZouBanPara.bValid)
			{
				SetColor(m_HWindowID, "orange red");
				//if(itInnerCheck->m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan)
				{
					if(itInnerCheck->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft)
					{
						DispObj(itInnerCheck->m_hRealZoubanRegionL,m_HWindowID);
					}
					if(itInnerCheck->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight)
					{
						DispObj(itInnerCheck->m_hRealZoubanRegionR,m_HWindowID);
					}
					if(itInnerCheck->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp)
					{
						MirrorRegion(itInnerCheck->m_hRealZoubanRegionU,&hShowReg,"row",hHei);
						DispObj(hShowReg,m_HWindowID);
					}
					if(itInnerCheck->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown)
					{
						MirrorRegion(itInnerCheck->m_hRealZoubanRegionD,&hShowReg,"row",hHei);
						DispObj(hShowReg,m_HWindowID);
					}
				}
				/*else
				{
				MirrorRegion(itInnerCheck->m_hRealZoubanRegion,&hShowReg,"row",hHei);
				DispObj(hShowReg,m_HWindowID);
				}*/
			}
		}
		else
		{
			HObject hTempImg;
			HTuple hWid,hHei;
			GetImageSize(itInnerCheck->m_hImageCheck,&hWid,&hHei);
			//SetPart(m_HWindowID, 0, 0, hHei-1,hWid-1);	
			MirrorImage(itInnerCheck->m_hImageCheck,&hTempImg,"row");
			DispObj(hTempImg,m_HWindowID);
			if (m_bCheckResult)
			{
				SetColor(m_HWindowID, "green");	
				HObject hShowReg,hRegionOrg;
				GenRegionContourXld(itInnerCheck->hSymbolXLDs, &hRegionOrg, "filled");
				if (itInnerCheck->m_bIsZoomCheck || itInnerCheck->m_bZoomCheckFlag)
				{
					ZoomRegion(hRegionOrg,&hRegionOrg,itInnerCheck->m_dHorEnlargeRate,itInnerCheck->m_dVerEnlargeRate);
				}
				MirrorRegion(hRegionOrg,&hShowReg,"row",hHei[0]);
				SetDraw(m_HWindowID,"margin");
				DispObj(hShowReg,m_HWindowID);
			}	
		}	
	}
	
}


// 对话框初始化
BOOL CParaDlg::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();

	// 显示初始状态下的对话框 [9/14/2015 TIAN]
	CRect rcClient;
	GetClientRect(&rcClient);
	GetWindowRect(&m_rectDlg);
	 
	m_rectInitDlg = m_rectDlg;
	m_rectInitDlg.right    -= 235;
	SetWindowPos(NULL, 0, 0, m_rectInitDlg.Width(), m_rectInitDlg.Height(), SWP_NOMOVE|SWP_NOZORDER);

	///////////////////////////////添加 BCG 控制///////////////////////////////
	//CBCGPVisualManager2007::SetStyle(CBCGPVisualManager2007:: VS2007_LunaBlue);
	//CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS (CBCGPVisualManager2007));

	//设置背景
	//SetBackgroundImage(IDB_BITMAP_BACK);

	//字体大小与类型
	
	m_hFont1=CreateFont(-23,0,0,0,FW_BOLD,false,false,false,
		GB2312_CHARSET,OUT_OUTLINE_PRECIS,
		CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
		FF_MODERN,"微软雅黑");

	//静态文本
	m_labTitle.m_clrText = RGB(255,255,255);
	m_labTitle.m_hFont = m_hFont1;
	m_labShowMode.m_clrText   = RGB(218,165,0);
	m_labExpCode.m_clrText    = RGB(218,165,0);
	m_labRecCode.m_clrText    = RGB(218,165,0);
	m_labType.m_clrText       = RGB(218,165,0);
	m_labChannel.m_clrText    = RGB(218,165,0);
	m_labMaxpixel.m_clrText   = RGB(218,165,0);
	m_labRecRange.m_clrText   = RGB(218,165,0);
	m_labHor.m_clrText        = RGB(218,165,0);
	m_labVer.m_clrText        = RGB(218,165,0);
	m_labDengji.m_clrText     = RGB(218,165,0);
	m_labBiaoZhun.m_clrText   = RGB(218,165,0);
	m_labTo.m_clrText         = RGB(218,165,0);
	m_labPolar.m_clrText      = RGB(218,165,0);
	m_labDrawLoc.m_clrText    = RGB(0,255,0);
	m_labDrawOffset.m_clrText = RGB(0,255,0);
	m_bcgBiLv.m_clrText = RGB(218,165,0);
	m_labResultNum.m_clrText = RGB(218,165,0);
	

	//Radio Buttom
	m_cmdRadioAuto.SetTextColor(RGB (255, 255, 255));
	m_cmdRadioAuto.SetMouseCursorHand();
	m_cmdRadioNorm.SetTextColor(RGB (255, 255, 255));
	m_cmdRadioNorm.SetMouseCursorHand();
	m_cmdRadioSetrate.SetTextColor(RGB (255, 255, 255));
	m_cmdRadioSetrate.SetMouseCursorHand();

	//Push buttom
	m_cmdAdvPara.SetFaceColor(RGB (40, 105, 151));
	m_cmdAdvPara.SetMouseCursorHand();
	m_cmdTest.SetFaceColor(RGB (40, 105, 151));
	m_cmdTest.SetMouseCursorHand();
	m_cmdSave.SetFaceColor(RGB (40, 105, 151));
	m_cmdSave.SetMouseCursorHand();
	m_cmdSaveAll.SetFaceColor(RGB (40, 105, 151));
	m_cmdSaveAll.SetMouseCursorHand();
	m_cmdExit.SetFaceColor(RGB (40, 105, 151));
	m_cmdExit.SetMouseCursorHand();
	m_cmdSetDengji.SetFaceColor(RGB (40, 105, 151));
	m_cmdSetDengji.SetMouseCursorHand();

	//Check Buttom
	m_cmdCheckGao.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckGao.SetMouseCursorHand();
	m_cmdCheckAll.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckAll.SetMouseCursorHand();
	m_cmdCheckRstMode.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckRstMode.SetMouseCursorHand();
	m_cmdCheckHorVer.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckHorVer.SetMouseCursorHand();
	m_cmdCheckAlignCor.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckAlignCor.SetMouseCursorHand();
	m_cmdCheckCode.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckCode.SetMouseCursorHand();
	m_cmdCheckDengji.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckDengji.SetMouseCursorHand();
	m_cmdCheckImageSmooth.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckImageSmooth.SetMouseCursorHand();
	m_cmdCheckImageZoom.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckImageZoom.SetMouseCursorHand();
	//////////////////////////////////////////////////////////////////////////
	// 主窗口
	CRect rect;
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_PIC);	
	pWnd->GetClientRect(&rect);
	pWnd->GetClientRect(&m_rectImg);
	pWnd->MapWindowPoints(this, &rect);
	long m_lWindowRow    = rect.top;
	long m_lWindowColumn = rect.left;
	m_lWindowHeight = rect.Height();
	m_lWindowWidth  = rect.Width(); 	
	m_nShowImgType  = 0;

	CRect rcMinRect;// 区域的最小外接矩形 [5/18/2011 Denghl]
	std::list<CPoint>::iterator itRegionPoint;
	if (m_sSingleCellRegionsInfo.sSingleCellPro.lstCell.size()<=0)
	{
		//AfxMessageBox("无原始小开坐标");
		::MessageBoxA(m_hWnd,"Region Coordinate Error!","""",MB_OK);
		return FALSE;
	}
	std::list<s_AlgRegions>::iterator itlstAlgRegs;
	itlstAlgRegs = m_sSingleCellRegionsInfo.lstAlgRegions.begin();
	std::list<s_RegionProperty>::iterator itRegionRect;
	itRegionRect = itlstAlgRegs->lstSRegPro.begin();

	itRegionPoint = itRegionRect->lstRegionOrg.begin();

	rcMinRect.top  = rcMinRect.bottom = itRegionPoint->y;
	rcMinRect.left = rcMinRect.right  = itRegionPoint->x;
	for(;itRegionPoint!=itRegionRect->lstRegionOrg.end();
		itRegionPoint++)
	{
		rcMinRect.top    = min(rcMinRect.top,itRegionPoint->y);
		rcMinRect.bottom = max(rcMinRect.bottom,itRegionPoint->y);
		rcMinRect.left   = min(rcMinRect.left,itRegionPoint->x);
		rcMinRect.right  = max(rcMinRect.right,itRegionPoint->x);
	}

	// 如果为小图，则直接用图像坐标定义开大小 [9/2/2011 Denghl]
	if (rcMinRect.right>m_sAlgImageInfo.sizeImage.cx
		||rcMinRect.bottom>m_sAlgImageInfo.sizeImage.cy)
	{
		// 新增如果缺陷图像是小开图像时，需要将区域进行对应裁剪 [2/14/2016 dhl]
		if (m_sSingleCellCheckResult.rCircumRect.left==m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.left
			&&m_sSingleCellCheckResult.rCircumRect.right==m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.right
			&&m_sSingleCellCheckResult.rCircumRect.top==m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.top
			&&m_sSingleCellCheckResult.rCircumRect.bottom==m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.bottom)
		{
			rcMinRect.top    = 0;
			rcMinRect.left   = 0;
			rcMinRect.bottom = m_sAlgImageInfo.sizeImage.cy-1;
			rcMinRect.right  = m_sAlgImageInfo.sizeImage.cx-1;
		}
		else
		{
			CRect rcMinRectTmp = rcMinRect;
			if (m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.left==0&&m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.right==0
				&&m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.top==0&&m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.bottom==0)
			{

				rcMinRect.top    = rcMinRectTmp.top-m_sSingleCellCheckResult.rCircumRect.top;
				rcMinRect.left   = rcMinRectTmp.left-m_sSingleCellCheckResult.rCircumRect.left;
				rcMinRect.bottom = rcMinRect.top+rcMinRectTmp.bottom-rcMinRectTmp.top;
				rcMinRect.right  = rcMinRect.left+rcMinRectTmp.right-rcMinRectTmp.left;
				rcMinRect.right = rcMinRect.left+((rcMinRect.right-rcMinRect.left)/4)*4-1;
			} 
			else
			{
				
				rcMinRect.top    = m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.top-m_sSingleCellCheckResult.rCircumRect.top;
				rcMinRect.left   = m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.left-m_sSingleCellCheckResult.rCircumRect.left;
				rcMinRect.bottom = rcMinRect.top+m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.bottom-m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.top;
				rcMinRect.right  = rcMinRect.left+m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.right-m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.left;
				rcMinRect.right = rcMinRect.left+((rcMinRect.right-rcMinRect.left)/4)*4-1;
			}
		}		
	}
	
	m_nImgWid   = rcMinRect.right-rcMinRect.left+1;
	m_nImgHei   = rcMinRect.bottom-rcMinRect.top;
	m_nImageBit = m_sAlgImageInfo.nBits;
	////  [12/12/2016 zb]
	//if (m_nImgWid<1||m_nImgHei<1)
	//{
	//	OnCancel();
	//	AfxMessageBox("当前图像不支持算法分析");
	//	return FALSE;
	//}
	//m_pImageSrc = new BYTE[((m_nImgWid+3)/4*4)*m_nImgHei*(m_nImageBit/8)];
	//TransAlgImageToBuff(m_sAlgImageInfo,(char*)m_pImageSrc,rcMinRect);
	//GenImage1Extern(&m_hImgSrc,"byte",WIDTHBYTES(m_nImgWid*8),m_nImgHei,(HTuple)m_pImageSrc,NULL);
	HObject hImgSrc,hImgSrc1;

	if (m_sAlgImageInfo.nBits == 8)
	{	
//		HObject hTemp;
		GenImage1Extern(&hImgSrc, "byte",
			(m_sAlgImageInfo.sizeImage.cx+3)/4*4,
			m_sAlgImageInfo.sizeImage.cy,(Hlong)m_sAlgImageInfo.pImageData, NULL);
//		crop_rectangle1(hTemp,&m_hImgSrc,rcMinRect.top,rcMinRect.left,rcMinRect.bottom,rcMinRect.right);
	}
	else if(m_sAlgImageInfo.nBits == 24)
	{
	//	HObject hTemp;
		HObject HImgR, HImgG, HImgB;
		GenImage1Extern(&HImgR, "byte", 
			(m_sAlgImageInfo.sizeImage.cx+3)/4*4, 
			m_sAlgImageInfo.sizeImage.cy, (Hlong)m_sAlgImageInfo.pImageR, NULL);
		GenImage1Extern(&HImgG, "byte", 
			(m_sAlgImageInfo.sizeImage.cx+3)/4*4, 
			m_sAlgImageInfo.sizeImage.cy, (Hlong)m_sAlgImageInfo.pImageG, NULL);
		GenImage1Extern(&HImgB, "byte", 
			(m_sAlgImageInfo.sizeImage.cx+3)/4*4, 
			m_sAlgImageInfo.sizeImage.cy, (Hlong)m_sAlgImageInfo.pImageB, NULL);
		Compose3(HImgR, HImgG, HImgB, &hImgSrc);
//		crop_rectangle1(hTemp,&m_hImgSrc,rcMinRect.top,rcMinRect.left,rcMinRect.bottom,rcMinRect.right);
	}
	// 生成最小外接矩形 [10/10/2015 TIAN]
	m_hImgOri=hImgSrc;
	HObject hRectangle;
	GenRectangle1(&hRectangle,rcMinRect.top,rcMinRect.left,rcMinRect.bottom,rcMinRect.right);
	
	ReduceDomain(hImgSrc,hRectangle,&hImgSrc1);
	CropDomain(hImgSrc1,&m_hImgSrc);
	//write_image(m_hImgSrc,"bmp",0,"d://1.bmp");
	
	//////////////////////////////////////////////////////////////////////////
	GetHWindowID(IDC_PIC, m_HWindowID);
	
	m_pTempChecker = new CCheck;
	s_ALGCheckParam sAlgCheckParam;
	m_pTempChecker->Init(m_pChecker->m_sAlgInitParam,&sAlgCheckParam,m_sSysAuthority);
	//  [4/7/2017 zb] 国际化配置，界面英文显示
	m_nLanguage = m_pChecker->m_sAlgInitParam.nLanguage;//0 中文 1英文

	size_t iPos = m_pTempChecker->m_strDefaultParaIni.Find("\\2DCodeCheckPara.ini");
	m_strDefaultParaPath = m_pTempChecker->m_strDefaultParaIni.Left(iPos); 
	m_strLanguagePath.Format("%s\\International Config\\ControlEnglish.ini", m_strDefaultParaPath);
	m_strControlMapPath.Format("%s\\International Config\\ControlMap.ini", m_strDefaultParaPath);
	if (m_nLanguage == 1)//英文
	{
		int nNum = 0;
		nNum = GetPrivateProfileInt("Check Parameters", "Total Control Number", 0, m_strLanguagePath);
		if(nNum > 0)
		{
			s_ControlSet *psControlSet = NULL;
			psControlSet = new s_ControlSet[nNum];
			for (int i=1;i<=nNum;i++)
			{
				char str[256];
				memset(str,0,256);
				CString strNo;
				strNo.Format("CControl NO %d",i);
				// 获取控件ID
				GetPrivateProfileString(strNo,"Control ID","",str,255,m_strLanguagePath);
				psControlSet[i-1].nControlID = GetPrivateProfileInt("Control Map",str,-1,m_strControlMapPath);	
				// 获取控件英文内容
				GetPrivateProfileString(strNo,"English","",psControlSet[i-1].strControl.GetBuffer(255),255,m_strLanguagePath);
				// 显示控件英文内容
				SetDlgItemText(psControlSet[i-1].nControlID,psControlSet[i-1].strControl);
			}
			delete[] psControlSet;
			psControlSet = NULL;
		}
	}
	//  [4/10/2017 zb]
	int nTextNum = 0;
	if (m_nLanguage == 0)
	{
		m_strMessagePath.Format("%s\\International Config\\MessageChinese.ini", m_strDefaultParaPath);
		nTextNum = GetPrivateProfileInt("中文文本", "文本总数", 0, m_strMessagePath);
		char chMessage[256];
		CString strIndex;
		for(int i=0; i<nTextNum; i++)
		{ 
			memset(chMessage,0,256);
			strIndex.Format("%d",i);
			GetPrivateProfileString("详细内容",strIndex,"",chMessage,255,m_strMessagePath);
			memset(m_strMessageText[i],0,256*sizeof(char));
			memcpy(m_strMessageText[i],chMessage,256*sizeof(char));
		}
	}
	else if (m_nLanguage == 1)
	{
		m_strMessagePath.Format("%s\\International Config\\MessageEnglish.ini", m_strDefaultParaPath);
		nTextNum = GetPrivateProfileInt("English Text", "Total Text Number", 0, m_strMessagePath);
		char chMessage[256];
		CString strIndex;
		for(int i=0; i<nTextNum; i++)
		{ 
			memset(chMessage,0,256);
			strIndex.Format("%d",i);
			GetPrivateProfileString("Detail Content",strIndex,"",chMessage,255,m_strMessagePath);
			memset(m_strMessageText[i],0,256*sizeof(char));
			memcpy(m_strMessageText[i],chMessage,256*sizeof(char));
		}
	}
	//  [4/7/2012 Denghl]
	s_DH_Template stmpDhTemplate;
	memcpy(&stmpDhTemplate.cTemplateName, m_pChecker->m_sDHTemplate.cTemplateName, 
		DH_MAX_NAMESIZE*sizeof(char));
	memcpy(&stmpDhTemplate.cTemplateTime, m_pChecker->m_sDHTemplate.cTemplateTime, 
		DH_MAX_NAMESIZE*sizeof(char));
	memcpy(&stmpDhTemplate.cResvered, m_pChecker->m_sDHTemplate.cResvered, 
		DH_MAX_NAMESIZE*sizeof(char));
	std::list<s_RegionProperty>::iterator itRegion;
	if (itlstAlgRegs->lstSRegPro.size()==0)
	{
		//AfxMessageBox("输入区域链表为空");
		CString str;
		str.Format("%s",m_strMessageText[30]);
		::MessageBoxA(m_hWnd,str,"""",MB_OK);
		return FALSE;
	}
	itRegion = itlstAlgRegs->lstSRegPro.begin();
	stmpDhTemplate.nCameraID   = itRegion->nCameraID;
	stmpDhTemplate.nComputerID = itRegion->nComputerID;
	m_nSetRegionType = itRegion->nRegionType;
	// 获取所有区域类型列表 [9/15/2011 Denghl]
	// 判断如果结果图像为区域图像，加载任务时，只需要加载改区域信息即可 [11/16/2011 Denghl]
	S_SingleCell_Or_RegionsInfo sTmpCellRegionInfo;
	s_AlgRegions sTmpAlgRegions;
	if (m_sAlgImageInfo.nImageType!=0||m_sSingleCellCheckResult.nImageType!=0)
	{
		if (m_sSingleCellCheckResult.vsErrorInfo.size()!=1)
		{
			//AfxMessageBox("结果输入信息不对称");
			CString str;
			str.Format("%s",m_strMessageText[31]);
			::MessageBoxA(m_hWnd,str,"""",MB_OK);
		}
		std::vector<s_sErrorInfo>::iterator itError;
		itError = m_sSingleCellCheckResult.vsErrorInfo.begin();
		int nRegionID = itError->nRegionID;
		sTmpCellRegionInfo.sSingleCellPro = m_sSingleCellRegionsInfo.sSingleCellPro;	

		std::list<s_RegionProperty>::iterator itRegionTmp;
		for (itRegionTmp=itlstAlgRegs->lstSRegPro.begin();
			itRegionTmp!=itlstAlgRegs->lstSRegPro.end();
			itRegionTmp++)
		{
			if (itRegionTmp->nRegionID==nRegionID)
			{
				sTmpAlgRegions.lstSRegPro.push_back(*itRegionTmp);
				break;
			}
		}
		sTmpCellRegionInfo.lstAlgRegions.push_back(sTmpAlgRegions);
	}
	else
	{
		sTmpCellRegionInfo = m_sSingleCellRegionsInfo;
	}

	stmpDhTemplate.lstSCellRegion.push_back(m_sSingleCellRegionsInfo);	
	m_pTempChecker->LoadTask(m_pChecker->m_strTaskName,stmpDhTemplate,m_sSysAuthority, TRUE);

	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo = m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	m_sCheckPrintParam = itInnerCheck->m_sCheckPrintParam;
	m_strexpCode = _T("");
	std::vector <char>::iterator itCode;
	if (itInnerCheck->m_sDstRegionProperty.vcExpectCode.size()>0)
	{
		for (itCode= itInnerCheck->m_sDstRegionProperty.vcExpectCode.begin();
			itCode!=itInnerCheck->m_sDstRegionProperty.vcExpectCode.end();
			itCode++)
		{
			m_strexpCode += *itCode;
		}
	}
	m_bCheckDengji = m_sCheckPrintParam.sDengjiPara.bValid;
	m_comDengSet.SetCurSel(m_sCheckPrintParam.sDengjiPara.nSetGrade);
	m_comGBType.SetCurSel(m_sCheckPrintParam.sDengjiPara.nGBID);
	
	//write_image(m_hImgSrc,"bmp",0,"D://m_hImgSrc.bmp");
	//生成走版白边区域
	if(m_sCheckPrintParam.sZouBanPara.bValid)
	{
		HTuple hWid,hHei;
		GetImageSize(m_hImgSrc,&hWid,&hHei);
		if(m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft)
		{
			GenRectangle1(&m_hZoubanRegL,0,m_sCheckPrintParam.sZouBanPara.m_nLeftPos,
				hHei-1,m_sCheckPrintParam.sZouBanPara.m_nLeftPos);
			//write_region(m_hZoubanRegL,"D://m_hZoubanRegL.reg");
		}
		if(m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight)
		{
			GenRectangle1(&m_hZoubanRegR,0,m_sCheckPrintParam.sZouBanPara.m_nRightPos,
				hHei-1,m_sCheckPrintParam.sZouBanPara.m_nRightPos);
			//write_region(m_hZoubanRegR,"D://m_hZoubanRegR.reg");
		}
		if(m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp)
		{
			GenRectangle1(&m_hZoubanRegU,m_sCheckPrintParam.sZouBanPara.m_nUpPos,0
				,m_sCheckPrintParam.sZouBanPara.m_nUpPos,hWid-1);
			MirrorRegion(m_hZoubanRegU,&m_hZoubanRegU,"row",hHei);
			//write_region(m_hZoubanRegU,"D://m_hZoubanRegU.reg");
		}
		if(m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown)
		{
			GenRectangle1(&m_hZoubanRegD,m_sCheckPrintParam.sZouBanPara.m_nDownPos,0
				,m_sCheckPrintParam.sZouBanPara.m_nDownPos,hWid-1);
			MirrorRegion(m_hZoubanRegD,&m_hZoubanRegD,"row",hHei);
			//write_region(m_hZoubanRegD,"D://m_hZoubanRegD.reg");
		}
	}
	//int nIndex = 0;
	//if (itInnerCheck->m_nProductNum>0)
	//{
	//	for (int i=0;i<itInnerCheck->m_nProductNum;i++)
	//	{
	//		CString str;
	//		str.Format("%s",itInnerCheck->m_vcstrAllProType.at(i));
	//		m_comProductType.AddString(str);
	//		if (itInnerCheck->m_strProductType==itInnerCheck->m_vcstrAllProType.at(i))
	//		{
	//			nIndex = i;
	//			
	//		}
	//	}
	//	m_comProductType.SetCurSel(nIndex);
	//}

	m_comCodeType.AddString("Data Matrix");
	m_comCodeType.AddString("PDF417");
	m_comCodeType.AddString("QR");
	m_comCodeType.AddString("Auto");
	m_comCodeType.SetCurSel(itInnerCheck->m_iCheckMethod);
	m_fScaleHor = itInnerCheck->m_fScaleHor;
	m_fScaleVer = itInnerCheck->m_fScaleVer;
	//	m_strWord = itInnerCheck->m_strWordArray;

	m_bCheckAllMod = itInnerCheck->m_bCheckAllMod;
	m_bCheckGao    = itInnerCheck->m_bCheckGao;
	//  [12/2/2014 baiyy]
	m_bCheckResultMode = itInnerCheck->m_bCheckResultMode;
	m_bCheckPartCode   = itInnerCheck->m_bCheckPartCode;
	m_CodeCharL = itInnerCheck->m_CodeCharL;
	m_CodeCharH = itInnerCheck->m_CodeCharH;

	//  [12/30/2015 TIAN]
	m_bIsImageSmooth = itInnerCheck->m_bIsImageSmooth;
	m_bIsZoomCheck   = itInnerCheck->m_bIsZoomCheck;
	m_fZoomRadio     = itInnerCheck->m_fZoomRadio;
	
	if (m_bCheckPartCode)
	{
		GetDlgItem(IDC_EDIT_CODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CODEH)->EnableWindow(TRUE);
		//GetDlgItem(IDC_STATIC_TO)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_CODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_CODEH)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CODEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CODEH)->EnableWindow(FALSE);
		//GetDlgItem(IDC_STATIC_TO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_CODEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_CODEH)->EnableWindow(FALSE);
	}

	if (m_sAlgImageInfo.nBits==24)
	{
		m_comChannel.AddString("Original");
		m_comChannel.AddString("R");
		m_comChannel.AddString("G");
		m_comChannel.AddString("B");
		m_comChannel.AddString("RGB");
		m_comChannel.SetCurSel(itInnerCheck->m_nChannel);
		m_nChannel = itInnerCheck->m_nChannel;
	}
	else
	{
		m_comChannel.AddString("Gray");
		m_comChannel.SetCurSel(0);
		m_nChannel = 0;
		GetDlgItem(IDC_COMBO_CHANNEL)->EnableWindow(FALSE);
		//GetDlgItem(IDC_STATIC_CHAN)->EnableWindow(FALSE);
	}
	//  [4/7/2017 zb]
	// 添加二维码极性 
	m_strPolar = itInnerCheck->m_strPolar;
	if (m_nLanguage == 1)//英文
	{
		m_comPolar.InsertString(0,"dark_on_light");
		m_comPolar.InsertString(1,"light_on_dark");
	
	} 
	else
	{
		//  [10/11/2016 zb]
		m_comPolar.InsertString(0,"黑码白底");
		m_comPolar.InsertString(1,"白码黑底");
	
	}
		if (m_strPolar=="light_on_dark")
		{
			//m_strPolarChinese.Format("白码黑底");
			m_comPolar.SetCurSel(1);
		} 
		else
		{
			m_comPolar.SetCurSel(0);
			//m_strPolarChinese.Format("黑码白底");
		}
	m_nResultNum = itInnerCheck->m_nResultNum;

	//屏蔽
	//横纵
	//m_bCheckHorVer   = m_sCheckPrintParam.sInHorverPara.bValid;
	//错位
	//m_bCheckAlignCor = m_sCheckPrintParam.sCorrPixelParam.bValid;
	//m_dCorrPixel     = m_sCheckPrintParam.sCorrPixelParam.nCorrPixels*m_fScaleVer;
	// 按照规则解析冠字 [6/11/2012 Denghl]
	char cProduct[255];
	memset(cProduct,0,255);
	char cExpCode[255];
	memset(cExpCode,0,255);
	int nCharNum=0;
	for (int nIndex=0;nIndex<MIN(254,itInnerCheck->m_sDstRegionProperty.vcExpectCode.size());nIndex++)
	{

		cExpCode[nIndex] = itInnerCheck->m_sDstRegionProperty.vcExpectCode.at(nIndex);
	}	

	m_strExpCode.Format("%s",cExpCode);	
	// 根据是否存在预期号显示预期号文本框与否 [10/16/2015 TIAN]
	if(itInnerCheck->m_sDstRegionProperty.bIsExistExpectCode&&
		cExpCode[0]!=0)
	{
		GetDlgItem(IDC_STATIC_EXP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_EXPCODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_RECCODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RESULT_CODE)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_EXP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_EXPCODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RECCODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RESULT_CODE)->ShowWindow(SW_HIDE);
	}
	//局部显示
	if (m_bCheckAlignCor)
	{
		GetDlgItem(IDC_EDIT_CORRPIXEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_MAXPIXEL)->EnableWindow(TRUE);

		//
	}
	else
	{
		GetDlgItem(IDC_EDIT_CORRPIXEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_MAXPIXEL)->EnableWindow(TRUE);
	}
	// 等级模块显示 [12/27/2013 Denghl]
	if (itInnerCheck->m_sCheckPrintParam.sDengjiPara.bValid)
	{
		GetDlgItem(IDC_COMBO_DENGJI)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_GBTYPE)->EnableWindow(TRUE);
		//GetDlgItem(IDC_BUTTON_SETDENG)->EnableWindow(TRUE);
		if (itInnerCheck->m_iCheckMethod==1||itInnerCheck->m_iCheckMethod==3)
		{
			m_comGBType.SetCurSel(0);
			itInnerCheck->m_sCheckPrintParam.sDengjiPara.nGBID = 0;
			GetDlgItem(IDC_COMBO_GBTYPE)->EnableWindow(FALSE);			
		}
		else
		{
			GetDlgItem(IDC_COMBO_GBTYPE)->EnableWindow(TRUE);	
		}
	}
	else
	{
		GetDlgItem(IDC_COMBO_DENGJI)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_GBTYPE)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_SETDENG)->EnableWindow(FALSE);
	}
		
	if (itInnerCheck->m_iCheckMethod==2||itInnerCheck->m_iCheckMethod==3)
	{
		m_bFlagQrloc = TRUE;
	}
	else
	{
		m_bFlagQrloc = FALSE;
	}	
	if (itInnerCheck->m_iCheckMethod==1)
	{
		m_bFlagHorVer = FALSE;
	}
	else
	{
		m_bFlagHorVer = TRUE;
	}
	if (m_bFlagInRegion||m_bFlagQrloc||m_bFlagOutRange||m_bFlagHorVer)
	{
		m_dlgSpacialSet = new CSpacialParaSetup;
		m_dlgSpacialSet->m_bFlagQrloc = m_bFlagQrloc;
		m_dlgSpacialSet->m_sCheckPrintParam = m_sCheckPrintParam;
		m_dlgSpacialSet->m_fScaleVer        = m_fScaleVer;
		m_dlgSpacialSet->m_fScaleHor        = m_fScaleHor;
		m_dlgSpacialSet->m_bFlagHorVer      = m_bFlagHorVer;
		//  [11/4/2015 zl]
		m_dlgSpacialSet->m_hImgSrc=m_hImgSrc;
		m_dlgSpacialSet->m_pParaDlg         = this;
		m_dlgSpacialSet->nDlgSwitch         = 2;
		//  [4/7/2017 zb]
		m_dlgSpacialSet->m_nLanguage	    = m_nLanguage;
		m_dlgSpacialSet->m_strDefaultParaPath = m_strDefaultParaPath;
		memcpy((void *)&m_dlgSpacialSet->m_strMessageText,(void *)&m_strMessageText,512*256*sizeof(TCHAR));
		CRect rectTab;
		GetDlgItem(IDC_SHOWTAB)->GetWindowRect(rectTab);
		ScreenToClient(rectTab);
		m_dlgSpacialSet->Create(IDD_DIALOG_PARASETUP, this);
		m_dlgSpacialSet->MoveWindow(rectTab);
		m_dlgSpacialSet->ShowWindow(SW_SHOW);
	}
	UpdateData(FALSE);
	CheckRadioButton(IDC_RADIO_AUTO, //第一个参数为该组的第一个单选按钮的ID  		
		IDC_RADIO_SETRATE, //第二个参数为该组的最后一个单选按钮的ID  		
		IDC_RADIO_AUTO);
	OnRadioAuto();
	GetDlgItem(IDC_PIC)->SetFocus();
	//  [12/5/2016 zb]
	OnButtonTest();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
bool CParaDlg::GetHWindowID(int nControlID, HTuple& lHWindowID,BOOL bFlag)
{

	CRect rect;
	CWnd *pWnd;
	HTuple lWWindowID;
	lWWindowID = (Hlong)GetDlgItem(nControlID)->m_hWnd;
	// 主窗口
	pWnd = GetDlgItem(nControlID);
	pWnd->GetClientRect(&rect);
	pWnd->MapWindowPoints(this, &rect);

	m_nDlgWidth  = rect.Width();
	m_nDlgHeight = rect.Height();

	HTuple lWindowRow = 0;// rect.top;//
	HTuple lWindowColumn = 0;//rect.left;

	SetWindowAttr("border_width",0);	
	SetCheck("~father");
	OpenWindow(lWindowRow, lWindowColumn,
		m_nDlgWidth, m_nDlgHeight,
		lWWindowID, "visible", "", &lHWindowID);
	SetWindowType("WIN32-Window");
	SetCheck("father");
	SetPart(lHWindowID, 0, 0, m_nDlgHeight- 1, m_nDlgWidth - 1);
	SetPartStyle(lHWindowID, 0);
	SetDraw(lHWindowID, "margin");  

	return true;
}


// 检测
void CParaDlg::OnButtonTest() 
{
	//s_InnerCheckInputParam sAlgCheckInput;
	//sAlgCheckInput.sAlgImageInfo = m_sAlgImageInfo;
	//sAlgCheckInput.sRegionProperty = m_sDstRegionProperty;
	//s_InnerCheckOutputParam sAlgCheckOutput;
	//m_pTempChecker->Check(sAlgCheckInput,&sAlgCheckOutput);
	m_strCheckResult = _T("");
	bOcrDeal=FALSE;
	s_AlgCheckInputParam  sAlgInputParamTemp;
	s_AlgCheckOutputParam sAlgCheckOutputTemp;
	sAlgInputParamTemp.sAlgImageInfo = m_sAlgImageInfo;
	//  [2/14/2016 dhl]
	CRect rcMinRect;// 区域的最小外接矩形 [5/18/2011 Denghl]
	std::list<CPoint>::iterator itRegionPoint;
		std::list<s_AlgRegions>::iterator itlstAlgRegs;
	itlstAlgRegs = m_sSingleCellRegionsInfo.lstAlgRegions.begin();
	std::list<s_RegionProperty>::iterator itRegionRect;
	itRegionRect = itlstAlgRegs->lstSRegPro.begin();
	itRegionPoint = itRegionRect->lstRegionOrg.begin();
	rcMinRect.top  = rcMinRect.bottom = itRegionPoint->y;
	rcMinRect.left = rcMinRect.right  = itRegionPoint->x;
	for(;itRegionPoint!=itRegionRect->lstRegionOrg.end();
		itRegionPoint++)
	{
		rcMinRect.top    = min(rcMinRect.top,itRegionPoint->y);
		rcMinRect.bottom = max(rcMinRect.bottom,itRegionPoint->y);
		rcMinRect.left   = min(rcMinRect.left,itRegionPoint->x);
		rcMinRect.right  = max(rcMinRect.right,itRegionPoint->x);
	}
//	rcMinRect.right = rcMinRect.left+((rcMinRect.right-rcMinRect.left)/4)*4-1;
		HTuple pImage;
		HTuple pImageR;
		HTuple pImageG;
		HTuple pImageB;
	if ((rcMinRect.right>m_sAlgImageInfo.sizeImage.cx
		||rcMinRect.bottom>m_sAlgImageInfo.sizeImage.cy)
		&&!(m_sSingleCellCheckResult.rCircumRect.left==m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.left
		&&m_sSingleCellCheckResult.rCircumRect.right==m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.right
		&&m_sSingleCellCheckResult.rCircumRect.top==m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.top
		&&m_sSingleCellCheckResult.rCircumRect.bottom==m_sSingleCellCheckResult.vsErrorInfo[0].RectErrorReg.bottom))// 缺陷小开分析 [2/14/2016 dhl]
	{
			HTuple hWid;
			HTuple hHei;
			char pType;
			HTuple htByte;
		if (m_sAlgImageInfo.nBits==8)
		{
			GetImagePointer1(m_hImgSrc,&pImage,&htByte,&hWid,&hHei);
			sAlgInputParamTemp.sAlgImageInfo.pImageData = (BYTE*)(Hlong)pImage;
			sAlgInputParamTemp.sAlgImageInfo.pImageR = (BYTE *)(Hlong)pImage;
			sAlgInputParamTemp.sAlgImageInfo.pImageG = (BYTE *)(Hlong)pImage;
			sAlgInputParamTemp.sAlgImageInfo.pImageB = (BYTE *)(Hlong)pImage;
		}
		else
		{

			GetImagePointer3(m_hImgSrc,&pImageR,&pImageG,&pImageB,&htByte,&hWid,&hHei);
			sAlgInputParamTemp.sAlgImageInfo.pImageData = (BYTE *)(Hlong)pImageR;
			sAlgInputParamTemp.sAlgImageInfo.pImageR = (BYTE *)(Hlong)pImageR;
			sAlgInputParamTemp.sAlgImageInfo.pImageG = (BYTE *)(Hlong)pImageG;
			sAlgInputParamTemp.sAlgImageInfo.pImageB = (BYTE *)(Hlong)pImageB;
		}
		sAlgInputParamTemp.sAlgImageInfo.sizeImage.cx = hWid[0].D();
		sAlgInputParamTemp.sAlgImageInfo.sizeImage.cy = hHei[0].D();

	}
	

	sAlgInputParamTemp.sDHTemplate   = m_pTempChecker->m_sDHTemplate;
	// 手动填写预知号码 [11/18/2011 Denghl]
	std::list<S_SingleCell_Or_RegionsInfo>::iterator itCell;
	itCell = sAlgInputParamTemp.sDHTemplate.lstSCellRegion.begin();
	std::list<s_AlgRegions>::iterator itlstRegions;
	itlstRegions = itCell->lstAlgRegions.begin();
	std::list<s_RegionProperty>::iterator itRegion;
	itRegion = itlstRegions->lstSRegPro.begin();
	if (m_strexpCode.GetLength()>0)
	{
		// 将预知号码填写到第一个区域上 [11/18/2011 Denghl]		
		//  [11/18/2011 Denghl]
		itRegion->vcExpectCode.clear();
		for (int i=0;i<m_strexpCode.GetLength();i++)
		{
			itRegion->vcExpectCode.push_back(m_strexpCode.GetAt(i));
		}
		itRegion->bIsExistExpectCode = TRUE;
	}
	else
	{
		itRegion->vcExpectCode.clear();		
	}
	// 将参数更新到对应临时检测类 [9/16/2013 Denghl]
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	
	if (m_bFlagHorVer||m_bFlagInRegion||m_bFlagOutRange||m_bFlagQrloc)
	{	
		m_dlgSpacialSet->GetTempCheckParam();
		m_sCheckPrintParam = m_dlgSpacialSet->m_sCheckPrintParam;
		itInnerCheck->m_sCheckPrintParam = m_sCheckPrintParam;	    
	}
	//  [12/2/2014 baiyy]
	itInnerCheck->m_bCheckResultMode = m_bCheckResultMode;
	m_pTempChecker->m_bCheckTimeOut  = m_sCheckPrintParam.sTimeOut.bValid;
	m_pTempChecker->m_nAllTime       = m_sCheckPrintParam.sTimeOut.nAllTime;
	itInnerCheck->FreeCheckMethod();
	itInnerCheck->MallocCheckMethod();

	//m_pTempChecker->m_sMiddleResult.vsLocGroupResult = sSingleCellCheckResult.vsLocGroupResult;
	s_Status sTatus;
	//sAlgInputParamTemp.lstAlgLearnInfo;// 设置学习输入 [9/15/2011 Denghl]
	HObject hSrcImage;
	HObject hImgTmp;
	GenEmptyObj(&hSrcImage);
	GenImage1Extern(&hSrcImage,"byte",WIDTHBYTES(sAlgInputParamTemp.sAlgImageInfo.sizeImage.cx*8),
		sAlgInputParamTemp.sAlgImageInfo.sizeImage.cy, 
		(Hlong)sAlgInputParamTemp.sAlgImageInfo.pImageR, NULL);
	if (sAlgInputParamTemp.sAlgImageInfo.nBits==24)
	{
		GenImage1Extern(&hImgTmp,"byte",WIDTHBYTES(sAlgInputParamTemp.sAlgImageInfo.sizeImage.cx*8),
			sAlgInputParamTemp.sAlgImageInfo.sizeImage.cy, 
			(Hlong)sAlgInputParamTemp.sAlgImageInfo.pImageG, NULL);
		ConcatObj(hSrcImage,hImgTmp,&hSrcImage);
		GenImage1Extern(&hImgTmp,"byte",WIDTHBYTES(sAlgInputParamTemp.sAlgImageInfo.sizeImage.cx*8),
			sAlgInputParamTemp.sAlgImageInfo.sizeImage.cy, 
			(Hlong)sAlgInputParamTemp.sAlgImageInfo.pImageB, NULL);
		ConcatObj(hSrcImage,hImgTmp,&hSrcImage);
	}
	sTatus = m_pTempChecker->Check(sAlgInputParamTemp,hSrcImage,&sAlgCheckOutputTemp);
	if (sTatus.nErrorType!=0)
	{
		return;
	}
	bOcrDeal = TRUE;

	if (itInnerCheck->m_iRealMethod>=0&&itInnerCheck->m_iRealMethod<3)
	{
		if (!m_bCheckAllMod)
		{
			itInnerCheck->FreeCheckMethod();
			itInnerCheck->m_iCheckMethod = itInnerCheck->m_iRealMethod;
			itInnerCheck->MallocCheckMethod();
			m_comCodeType.SetCurSel(itInnerCheck->m_iCheckMethod);			
			UpdateData(FALSE);			
		}
	}

	// 显示检测结果 [11/16/2011 Denghl]
	if (sAlgCheckOutputTemp.sCheckResult.iErrorLstSigCCR==0
		||sAlgCheckOutputTemp.sCheckResult.lstSigCCR.size()<=0)
	{
		CString str;
		str.Format("%s\n%s",m_strMessageText[32],sTatus.chErrorContext);
		//AfxMessageBox(str);	
		::MessageBoxA(m_hWnd,str,"""",MB_OK);
	}
	std::list<s_SigleCell_CheckResult>::iterator itResult;
	itResult = sAlgCheckOutputTemp.sCheckResult.lstSigCCR.begin();
	/*CString strTmp;	
	strTmp.Format("检测结果:\n结果个数:%d",itResult->vsErrorInfo.size());*/
	std::vector<s_sErrorInfo>::iterator itError;
	itError = itResult->vsErrorInfo.begin();

	CString str;
	CString strCode;
	CString strAllCode;
	//实际返回的识别结果（有局部号码情况） [10/16/2015 TIAN]
	if (itError->vcCheckResultCode.size()>0)
	{
		for (int j=0;j<int(itError->vcCheckResultCode.size());j++)
		{
			if (itError->vcCheckResultCode.at(j)!=0)
			{
				strCode += itError->vcCheckResultCode.at(j);
			}
			else
			{
				break;
			}
		}

	}
	//完整识别的二维码结果 [10/16/2015 TIAN]
	if (itInnerCheck->m_CheckResultCode.size()>0)
	{
		for (int j=0;j<int(itInnerCheck->m_CheckResultCode.size());j++)
		{
			if (itInnerCheck->m_CheckResultCode.at(j)!=0)
			{
				strAllCode += itInnerCheck->m_CheckResultCode.at(j);
			}
			else
			{
				break;
			}
		}

	}

	// 将非显示的字符进行显示 [8/16/2013 Denghl]
	BOOL bFlag = FALSE;
	CString strShow;
	strShow = _T("");


	for (int i=0;i<strAllCode.GetLength();i++)
	{
		if (strAllCode.GetAt(i)==13)
		{
			strShow +="\\r";
			bFlag = TRUE;
		}
		else if (strAllCode.GetAt(i)==10)
		{
			strShow += "\\n";
			bFlag = TRUE;
		}
		else
		{
			strShow+=strAllCode.GetAt(i);

		}
	}

	// 结果显示 [10/16/2015 TIAN]
	if (!bFlag)
	{
		if (m_bCheckPartCode)//  [12/19/2016 zb]
		{
			if (m_nLanguage == 1)
			{
				m_strCheckResult.Format("Region ID=%d,Error Type=%d,Error Grade=%d,Error Description=%s\r\nIdentification Number=%s\r\nLocal Number=%s\r\nAdditional Information=%s\r\n",
					itError->nRegionID,
					itError->bytErrorType,
					itError->bytErrorGrade,
					itError->cErrorDescription,
					strAllCode,
					strCode,
					itError->cResvered);
			} 
			else
			{
				m_strCheckResult.Format("区域ID=%d,错误类型=%d,错误级别=%d,错误描述=%s\r\n识别号码=%s\r\n局部号码=%s\r\n附加信息=%s\r\n",
					itError->nRegionID,
					itError->bytErrorType,
					itError->bytErrorGrade,
					itError->cErrorDescription,
					strAllCode,
					strCode,
					itError->cResvered);
			}
		}
		else
		{
			if (m_nLanguage == 1)
			{
				m_strCheckResult.Format("Region ID=%d,Error Type=%d,Error Grade=%d,Error Description=%s\r\nIdentification Number=%s\r\nAdditional Information=%s\r\n",
					itError->nRegionID,
					itError->bytErrorType,
					itError->bytErrorGrade,
					itError->cErrorDescription,
					strAllCode,
					itError->cResvered);
			} 
			else
			{
				m_strCheckResult.Format("区域ID=%d,错误类型=%d,错误级别=%d,错误描述=%s\r\n识别号码=%s\r\n附加信息=%s\r\n",
					itError->nRegionID,
					itError->bytErrorType,
					itError->bytErrorGrade,
					itError->cErrorDescription,
					strAllCode,
					itError->cResvered);
			}
		}
		
	}
	else
	{
		if (m_bCheckPartCode)//  [12/19/2016 zb]
		{
			if (m_nLanguage == 1)
			{
				m_strCheckResult.Format("Region ID=%d,Error Type=%d,Error Grade =%d,Error Description=%s\r\nIdentification Number=%s\r\nLocal Number=%s\r\nAdditional Information=%s\r\nEscape Show Identification Character\r\n",
					itError->nRegionID,
					itError->bytErrorType,
					itError->bytErrorGrade,
					itError->cErrorDescription,
					strShow,
					strCode,
					itError->cResvered);
			} 
			else
			{
				m_strCheckResult.Format("区域ID=%d,错误类型=%d,错误级别=%d,错误描述=%s\r\n识别号码=%s\r\n局部号码=%s\r\n附加信息=%s\r\n对识别字符进行了转义显示\r\n",
					itError->nRegionID,
					itError->bytErrorType,
					itError->bytErrorGrade,
					itError->cErrorDescription,
					strShow,
					strCode,
					itError->cResvered);
			}
			
		}
		else
		{
			if (m_nLanguage == 1)
			{
				m_strCheckResult.Format("Region ID=%d,Error Type=%d,Error Grade =%d,Error Description=%s\r\nIdentification Number=%s\r\nAdditional Information=%s\r\nEscape Show Identification Character\r\n",
					itError->nRegionID,
					itError->bytErrorType,
					itError->bytErrorGrade,
					itError->cErrorDescription,
					strShow,
					itError->cResvered);
			} 
			else
			{
				m_strCheckResult.Format("区域ID=%d,错误类型=%d,错误级别=%d,错误描述=%s\r\n识别号码=%s\r\n附加信息=%s\r\n对识别字符进行了转义显示\r\n",
					itError->nRegionID,
					itError->bytErrorType,
					itError->bytErrorGrade,
					itError->cErrorDescription,
					strShow,
					itError->cResvered);
			}
		}
	}
	////  [12/5/2016 zb]
	//if (m_bCheckPartCode)
	//{
	//	CString str;
	//	str.Format("\r\n局部号码显示=%s",strCode);
	//	m_strCheckResult+=str;
	//}
	//////////////////////////////////////////////////////////////////////////
	if (itError->bytErrorType!=ABNORMAL_2D
		&&itError->bytErrorType!=PRINTTRY)
	{
		m_bCheckResult = TRUE;
	}
	else
	{
		m_bCheckResult = FALSE;
	}
	if (itInnerCheck->m_sCheckPrintParam.sDengjiPara.bValid
		&&(itError->bytErrorType==GRADELOW
		||itError->bytErrorType==NORMAL_2D))
	{
		m_strCheckResult += itInnerCheck->m_strRealGrade;
	}

	if (itInnerCheck->m_sCheckPrintParam.sInRegionPara.bValid
		&&itError->bytErrorType==INREGION)
	{
		CString strInR;
		if (m_nLanguage == 1)
		{
			strInR.Format("\r\nInky Ualue=%f,Inky Difference Degree=%f",
				itInnerCheck->hv_Mean[0].D()+2*itInnerCheck->hv_Deviation[0].D(),
				itInnerCheck->hv_Deviation[0].D());
		} 
		else
		{
			strInR.Format("\r\n墨色值=%f,墨色差异度=%f",
				itInnerCheck->hv_Mean[0].D()+2*itInnerCheck->hv_Deviation[0].D(),
				itInnerCheck->hv_Deviation[0].D());
		}
		m_strCheckResult += strInR;
	}
	if (itInnerCheck->m_sCheckPrintParam.sInHorverPara.bValid
		&&itError->bytErrorType==HORVER)
	{
		CString strInR;
		if (itInnerCheck->m_hWhiteLineValue>0)
		{
			//  [Baiyy]
			//strInR.Format("\r\n白线面积值=%.1f",
			//	itInnerCheck->m_hWhiteLineValue[0].D()*m_fScaleHor*m_fScaleVer);
			//  [10/19/2015 TIAN]
			if (m_nLanguage == 1)
			{
				strInR.Format("\r\nWhite Line Area=%.1f",
					itInnerCheck->m_hWhiteLineValue[0].D()*itInnerCheck->m_fScaleHor*itInnerCheck->m_fScaleVer);
			} 
			else
			{
				strInR.Format("\r\n白线面积值=%.1f",
					itInnerCheck->m_hWhiteLineValue[0].D()*itInnerCheck->m_fScaleHor*itInnerCheck->m_fScaleVer);
			}

			m_strCheckResult += strInR;
		}
		//////////////////////////////////////////////////////////////////////////
		if (itInnerCheck->m_nHorverType == 31)
		{
			if (m_nLanguage == 1)
			{
				strInR = "“White Point”";
			} 
			else
			{
				strInR = "“白点”";
			}
			m_strCheckResult += strInR;
		}
		else if (itInnerCheck->m_nHorverType == 32)
		{
			if (m_nLanguage == 1)
			{
				strInR = "“White Line”";
			} 
			else
			{
				strInR = "“白线”";
			}
			
			m_strCheckResult += strInR;
		}
	}
	if (itInnerCheck->m_sCheckPrintParam.sOutRangPara.bValid		
		&&itError->bytErrorType==OUTRANGE)
	{

		CString strInR;
		double dArea = 0;

		dArea = itInnerCheck->hv_AreaAll[0].D()*itInnerCheck->m_fScaleHor*itInnerCheck->m_fScaleVer;
		if (m_nLanguage == 1)
		{
			strInR.Format("\nSmearing Area=%f", dArea);
		} 
		else
		{
			strInR.Format("\n蹭脏面积=%f", dArea);
		}
	
		m_strCheckResult += strInR;
	}
	//////////////////////////////////////////////////////////////////////////
	
	m_strResultCode = strCode;  //实际识别结果
	bOcrDeal = TRUE;

	//////////////////////////////////////////////////////////////////////////

	InvalidateRect(&m_rectImg);	
	UpdateData(FALSE);

	sAlgInputParamTemp.lst_vec_clear();
	sAlgCheckOutputTemp.lst_vec_clear();

}

// 保存
void CParaDlg::OnButtonSave() 
{
	// m_pChecker->m_sCheckPara = m_pTempChecker->m_sCheckPara; 参数保存，这里需要添加代码 [6/24/2010 dhl]
	// 默认弹第一个区域参数 [11/16/2011 Denghl]
	if (!m_bAuthority)
	{
		//AfxMessageBox("无权限修改和保存参数,不能保存！");	
		CString str;
		str.Format("%s",m_strMessageText[33]);
		::MessageBoxA(m_hWnd,str,"""",MB_OK);
		return;
	}
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();

	if (m_bFlagHorVer||m_bFlagInRegion||m_bFlagOutRange||m_bFlagQrloc)
	{
		m_dlgSpacialSet->GetTempCheckParam();
		m_sCheckPrintParam = m_dlgSpacialSet->m_sCheckPrintParam;
		itInnerCheck->m_sCheckPrintParam = m_sCheckPrintParam;	 
	}
	//  [12/2/2014 baiyy]
	//itInnerCheck->m_sCheckPrintParam.sZouBanPara.bLRChangeZouban = FALSE;
	//itInnerCheck->m_sCheckPrintParam.sZouBanPara.bUDChangeZouban = FALSE;
	itInnerCheck->m_bCheckResultMode = m_bCheckResultMode;
	itInnerCheck->WritePara(itInnerCheck->m_strParaIni);
	m_pTempChecker->m_bCheckTimeOut = itInnerCheck->m_sCheckPrintParam.sTimeOut.bValid;
	m_pTempChecker->m_nAllTime = itInnerCheck->m_sCheckPrintParam.sTimeOut.nAllTime;
	m_pTempChecker->WriteTimePara(m_pTempChecker->m_strPathini);
	m_bIsModify = TRUE;
	OnOK();
}

// 退出释放
void CParaDlg::OnDestroy() 
{
	HalconCpp::CloseWindow(m_HWindowID);
	
	if (m_pTempChecker)
	{
		m_pTempChecker->Free(m_sSysAuthority);
		delete m_pTempChecker;
		m_pTempChecker = NULL;
	}
	//if (m_pImageSrc)
	//{
	//	delete [] m_pImageSrc;
	//	m_pImageSrc = NULL;
	//}	
	if (m_dlgSpacialSet)
	{
		m_dlgSpacialSet->OnDestroy();
		delete m_dlgSpacialSet;
		m_dlgSpacialSet = NULL;
	}
	//删除字体
	DeleteObject(m_hFont1);
	CBCGPDialog::OnDestroy();
}

//应用所有
void CParaDlg::OnButtonSaveall() 
{
	//Note:该功能只有开启了服务器端配置文件中的InitLoadTask=1时起作用
	
	// TODO: Add your control notification handler code here
	// 默认弹第一个区域参数 [11/16/2011 Denghl]
	if (!m_bAuthority)
	{
		//AfxMessageBox("无权限修改和保存参数,不能保存！");
		CString str;
		str.Format("%s",m_strMessageText[33]);
		::MessageBoxA(m_hWnd,str,"""",MB_OK);
		return;
	}
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo = m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();

	if (m_bFlagHorVer||m_bFlagInRegion||m_bFlagOutRange||m_bFlagQrloc)
	{
		m_dlgSpacialSet->GetTempCheckParam();
		m_sCheckPrintParam = m_dlgSpacialSet->m_sCheckPrintParam;
		itInnerCheck->m_sCheckPrintParam = m_sCheckPrintParam;	    
	}
	//itInnerCheck->m_sCheckPrintParam.sZouBanPara.bLRChangeZouban= FALSE;
	//itInnerCheck->m_sCheckPrintParam.sZouBanPara.bUDChangeZouban= FALSE;
	itInnerCheck->WritePara(itInnerCheck->m_strParaIni);
	m_pTempChecker->m_bCheckTimeOut = itInnerCheck->m_sCheckPrintParam.sTimeOut.bValid;
	m_pTempChecker->m_nAllTime = itInnerCheck->m_sCheckPrintParam.sTimeOut.nAllTime;
	m_pTempChecker->WriteTimePara(m_pTempChecker->m_strPathini);
	m_bIsModify = TRUE;

	if (m_pChecker->m_lstSingleCellInnerCheck.size()>0)
	{
		std::list<s_InnerCheckInfo>::iterator itInfosrc;
		itInfosrc=m_pChecker->m_lstSingleCellInnerCheck.begin();
		for (;itInfosrc!=m_pChecker->m_lstSingleCellInnerCheck.end();
			itInfosrc++)
		{
			std::list<CInnerCheck>::iterator itInnerCheckP;
			for (itInnerCheckP=itInfosrc->lstInnerCheck.begin();
				itInnerCheckP!=itInfosrc->lstInnerCheck.end();
				itInnerCheckP++)
			{
				if (itInnerCheckP->m_sDstRegionProperty.nRegionType==itInnerCheck->m_sDstRegionProperty.nRegionType
					&&itInnerCheckP->m_sDstRegionProperty.nRegionID!=itInnerCheck->m_sDstRegionProperty.nRegionID)
				{
					itInnerCheckP->m_nChannel = itInnerCheck->m_nChannel;
					// 添加二维码极性 [12/21/2015 TIAN]
					itInnerCheckP->m_strPolar     = itInnerCheck->m_strPolar;
					itInnerCheckP->m_iCheckMethod = itInnerCheck->m_iCheckMethod;
					itInnerCheckP->m_bCheckAllMod = itInnerCheck->m_bCheckAllMod;
					itInnerCheckP->m_bCheckResultMode = itInnerCheck->m_bCheckResultMode;
					itInnerCheckP->m_bCheckGao = itInnerCheck->m_bCheckGao;
					itInnerCheckP->m_fScaleHor = itInnerCheck->m_fScaleHor;
					itInnerCheckP->m_fScaleVer = itInnerCheck->m_fScaleVer;
					itInnerCheckP->m_bCheckPartCode = itInnerCheck->m_bCheckPartCode;
					itInnerCheckP->m_CodeCharL = itInnerCheck->m_CodeCharL;
					itInnerCheckP->m_CodeCharH = itInnerCheck->m_CodeCharH;

					// 图像平滑处理 [12/30/2015 TIAN]
					itInnerCheckP->m_bIsImageSmooth = itInnerCheck->m_bIsImageSmooth;
					itInnerCheckP->m_bIsZoomCheck   = itInnerCheck->m_bIsZoomCheck;
					itInnerCheckP->m_fZoomRadio     = itInnerCheck->m_fZoomRadio;
					
					//  [11/13/2015 zl]中间变量保存原始的纸边位置
					s_ZouBanParam sZBParam;
					sZBParam.m_nLeftPos=itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nLeftPos;
					sZBParam.m_nRightPos=itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nRightPos;
					sZBParam.m_nUpPos=itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nUpPos;
					sZBParam.m_nDownPos=itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nDownPos;
					itInnerCheckP->m_sCheckPrintParam = itInnerCheck->m_sCheckPrintParam;
					
					//  [11/13/2015 zl] 大图时重新提取纸边位置 小图时不写位置参数
					if(m_sAlgImageInfo.nImageType==0)
					{

						if(itInnerCheckP->m_sCheckPrintParam.sZouBanPara.bValid&&
							itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan)
						{
							
							HObject ho_SrcImg;
							HObject hRectangle;

							CRect rcMinRect;// 区域的最小外接矩形 [5/18/2011 Denghl]
							std::list<CPoint>::iterator itRegionPoint;
							if (itInnerCheckP->m_sDstRegionProperty.lstRegionOrg.size()>0)
							{

								itRegionPoint=itInnerCheckP->m_sDstRegionProperty.lstRegionOrg.begin();
								rcMinRect.top = rcMinRect.bottom = itRegionPoint->y;
								rcMinRect.left = rcMinRect.right = itRegionPoint->x;
								for(;itRegionPoint!=itInnerCheckP->m_sDstRegionProperty.lstRegionOrg.end();
									itRegionPoint++)
								{
									rcMinRect.top = min(rcMinRect.top,itRegionPoint->y);
									rcMinRect.bottom = max(rcMinRect.bottom,itRegionPoint->y);
									rcMinRect.left = min(rcMinRect.left,itRegionPoint->x);
									rcMinRect.right = max(rcMinRect.right,itRegionPoint->x);
								}
								//write_image(m_hImgOri,"bmp",0,"D://m_hImgOri.bmp");
								GenRectangle1(&hRectangle,rcMinRect.top,rcMinRect.left,rcMinRect.bottom,rcMinRect.right);

								ReduceDomain(m_hImgOri,hRectangle,&ho_SrcImg);
								CropDomain(ho_SrcImg,&ho_SrcImg);
								HTuple htImgW,htImgH;
								GetImageSize(ho_SrcImg,&htImgW,&htImgH);
								//write_image(ho_SrcImg,"bmp",0,"D://ho_SrcImg.bmp");

								HObject SymbolXLDs,hoRectengle;
								HTuple  Width,Height,DataCodeHandle,ResultHandles,DecodedDataStrings;
								HTuple  Row11 = 0 , Column11 = 0, Row21 = 0, Column21 = 0,htNum = 0;
								GenEmptyObj(&SymbolXLDs);
								CreateDataCode2dModel ("QR Code", HTuple(), HTuple(), &DataCodeHandle);
								GetImageSize (ho_SrcImg, &Width, &Height);
								FindDataCode2d( ho_SrcImg, &SymbolXLDs, DataCodeHandle, "stop_after_result_num", 1, 
									&ResultHandles, &DecodedDataStrings);
								HTuple lNum = 0;
								CountObj(SymbolXLDs,&lNum);
								if (lNum>0)
								{
									/*if(itInnerCheckP->m_sDstRegionProperty.nRegionID==8)
									{
										int ii=0;
									}*/
									SmallestRectangle1Xld (SymbolXLDs, &Row11, &Column11, &Row21, &Column21);
									HTuple htW=Column21-Column11;
									itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nLeftPos=itInnerCheckP->ExWhiteEdge(ho_SrcImg,htW,1);
									itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nRightPos=itInnerCheckP->ExWhiteEdge(ho_SrcImg,htW,2);
									itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nUpPos=itInnerCheckP->ExWhiteEdge(ho_SrcImg,htW,3);
									itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nDownPos=itInnerCheckP->ExWhiteEdge(ho_SrcImg,htW,4);
									if(itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nLeftPos==0&&
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nRightPos>0&&
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nRightPos<htImgW)
									{
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft=0;
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight=1;
									}
									if(itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nLeftPos>0&&
										(itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nRightPos==htImgW-1||
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nRightPos==0))
									{
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft=1;
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight=0;
									}
									if(itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nUpPos==0&&
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nDownPos>0&&
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nDownPos<htImgH)
									{
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp=0;
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown=1;
									}
									if(itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nUpPos>0&&
										(itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nDownPos==htImgH-1||
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nDownPos==0))
									{
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp=1;
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown=0;
									}
									//若无法提取白边 不使用白边检测
									if((itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nUpPos==0&&
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nDownPos==0)||
										(itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nLeftPos==0&&
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nRightPos==0))
									{
										itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan=FALSE;
									}
								}
								else
								{
									itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nLeftPos  = sZBParam.m_nLeftPos;
									itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nRightPos = sZBParam.m_nRightPos;
									itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nUpPos    = sZBParam.m_nUpPos;
									itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nDownPos  = sZBParam.m_nDownPos;
									itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan=FALSE;
								}
							}
						}
					}
					else
					{
						if (m_dlgSpacialSet->m_bChangeZoubanWhite)//白线重新提取的应用全部需要位置归零
						{
							itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nLeftPos   = 0;
							itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nRightPos  = 0;
							itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nUpPos     = 0;
							itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nDownPos   = 0;
						}
						else
						{
							itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nLeftPos   = sZBParam.m_nLeftPos;
							itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nRightPos  = sZBParam.m_nRightPos;
							itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nUpPos     = sZBParam.m_nUpPos;
							itInnerCheckP->m_sCheckPrintParam.sZouBanPara.m_nDownPos   = sZBParam.m_nDownPos;

						}
						

						itInnerCheckP->m_sCheckPrintParam.sZouBanPara.bLRChangeZouban = m_sCheckPrintParam.sZouBanPara.bLRChangeZouban;
						itInnerCheckP->m_sCheckPrintParam.sZouBanPara.bUDChangeZouban = m_sCheckPrintParam.sZouBanPara.bUDChangeZouban;
					}
					itInnerCheckP->WritePara(itInnerCheckP->m_strParaIni);
					m_bIsModify = TRUE;
				}
			}		
		}
	}
	else
	{
		//AfxMessageBox("服务器未初始化加载任务，请检查InitLoadTask!");
		CString str;
		str.Format("%s",m_strMessageText[34]);
		::MessageBoxA(m_hWnd,str,"""",MB_OK);
	}
	OnOK();
}

// 水平滚动条响应
void CParaDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//int nCurPos;
	//int nID = pScrollBar->GetDlgCtrlID ();
	//CString str;

	//// 拖动图像水平滚动条
	//if (nID==IDC_SCROLLBAR_HOR)
	//{
	//	switch( nSBCode )
	//	{
	//	case SB_THUMBPOSITION:
	//		pScrollBar->SetScrollPos( nPos );
	//		break;
	//	case SB_LINERIGHT:
	//		nCurPos = pScrollBar->GetScrollPos();
	//		nCurPos++;
	//		if( nCurPos > m_nRangH )
	//			nCurPos = m_nRangH;
	//		pScrollBar->SetScrollPos( nCurPos );
	//		break;
	//	case SB_LINELEFT:
	//		nCurPos = pScrollBar->GetScrollPos();
	//		nCurPos--;
	//		if( nCurPos < 0 )
	//			nCurPos = 0;
	//		pScrollBar->SetScrollPos( nCurPos );
	//		break;
	//	case SB_PAGERIGHT:
	//		nCurPos = pScrollBar->GetScrollPos();
	//		nCurPos += 10;
	//		if( nCurPos > m_nRangH )
	//			nCurPos = m_nRangH;
	//		pScrollBar->SetScrollPos( nCurPos );
	//		break;
	//	case SB_PAGELEFT:
	//		nCurPos = pScrollBar->GetScrollPos();
	//		nCurPos -= 10;
	//		if( nCurPos < 0 )
	//			nCurPos = 0;
	//		pScrollBar->SetScrollPos( nCurPos );
	//		break;
	//	case SB_THUMBTRACK:
	//		pScrollBar->SetScrollPos( nPos );
	//		break;
	//	}
	//	nCurPos = pScrollBar->GetScrollPos(	);
	//	m_nCurPosx = nCurPos;
	//	InvalidateRect(&m_rectImg,TRUE);
	//}
	CBCGPDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// 垂直滚动条响应
void CParaDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//int nCurPos;
	//int nID = pScrollBar->GetDlgCtrlID ();
	//CString str;
	//if (nID==IDC_SCROLLBAR_VER)
	//{
	//	switch( nSBCode )
	//	{
	//	case SB_THUMBPOSITION:
	//		pScrollBar->SetScrollPos( nPos );
	//		break;
	//	case SB_LINERIGHT:
	//		nCurPos = pScrollBar->GetScrollPos();
	//		nCurPos++;
	//		if( nCurPos > m_nRangV )
	//			nCurPos = m_nRangV;
	//		pScrollBar->SetScrollPos( nCurPos );
	//		break;
	//	case SB_LINELEFT:
	//		nCurPos = pScrollBar->GetScrollPos();
	//		nCurPos--;
	//		if( nCurPos < 0 )
	//			nCurPos = 0;
	//		pScrollBar->SetScrollPos( nCurPos );
	//		break;
	//	case SB_PAGERIGHT:
	//		nCurPos = pScrollBar->GetScrollPos();
	//		nCurPos += 10;
	//		if( nCurPos > m_nRangV )
	//			nCurPos = m_nRangV;
	//		pScrollBar->SetScrollPos( nCurPos );
	//		break;
	//	case SB_PAGELEFT:
	//		nCurPos = pScrollBar->GetScrollPos();
	//		nCurPos -= 10;
	//		if( nCurPos < 0 )
	//			nCurPos = 0;
	//		pScrollBar->SetScrollPos( nCurPos );
	//		break;
	//	case SB_THUMBTRACK:
	//		pScrollBar->SetScrollPos( nPos );
	//		break;
	//	}
	//	nCurPos = pScrollBar->GetScrollPos();
	//	m_nCurPosy = m_nRangV - nCurPos;
	//	InvalidateRect(&m_rectImg,TRUE);
	//}
	CBCGPDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

// 画图函数
void CParaDlg::DrawImage()
{
	//CDC *pDC = GetDlgItem(IDC_PIC)->GetDC();
	//
	//if (m_nShowImgType==0)
	//{
	//	ShowImage(pDC,m_pImageSrc,0,0,m_lWindowWidth,m_lWindowHeight,0,0,m_nImgWid,m_nImgHei,m_nImgWid,m_nImgHei,m_nImageBit);
	//
	//}
	//else if (m_nShowImgType==1)// 原始显示
	//{
	//	ShowImage(pDC,m_pImageSrc,0,0,MIN(m_lWindowWidth,m_nImgWid),MIN(m_lWindowHeight,m_nImgHei),
	//		m_nCurPosx,m_nCurPosy,MIN(m_lWindowWidth,m_nImgWid),MIN(m_lWindowHeight,m_nImgHei),m_nImgWid,m_nImgHei,m_nImageBit);
	//}
	//else if (m_nShowImgType==2)// 按设定比率显示
	//{
	//	ShowImage(pDC,m_pImageSrc,0,0,MIN(m_lWindowWidth,int(m_nImgWid*m_fScaleX)),MIN(m_lWindowHeight,int(m_nImgHei*m_fScaleY)),
	//		      int(m_nCurPosx/m_fScaleX),int(m_nCurPosy/m_fScaleY),
	//			  MIN(int(m_lWindowWidth/m_fScaleX),m_nImgWid),MIN(int(m_lWindowHeight/m_fScaleY),m_nImgHei),
	//			  m_nImgWid,m_nImgHei,m_nImageBit);
	//}

	//ReleaseDC(pDC);	

	// halcon显示，便于画检测区域 [7/31/2014 Denghl]
	HObject hTempImg;
	HObject hShowReg;
	//GenImage1Extern(&hImageSrc,"byte",(HTuple)WIDTHBYTES(m_nImgWid*8),(HTuple)m_nImgHei,(HTuple)m_pImageSrc,NULL);
	//SetPart(m_HWindowID, 0, 0, m_nImgHei-1,WIDTHBYTES(m_nImgWid*8)-1);
	if(m_nChannel!=0)
	{
		HObject hImgR, hImgG, hImgB;
		Decompose3(m_hImgSrc, &hImgR, &hImgG, &hImgB);
		if(m_nChannel==1)
		{
			hTempImg = hImgR;
		}
		else if(m_nChannel==2)
		{
			hTempImg = hImgG;
		}
		else if(m_nChannel==3)
		{
			hTempImg = hImgB;
		}
		else if (m_nChannel==4)
		{
		/*	HObject hMinImg;
			MinImage(hImgR,hImgG,&hMinImg);
			MinImage(hMinImg,hImgB,&hTempImg);*/
			Rgb3ToGray(hImgR,hImgG,hImgB,&hTempImg);
		}
		else
		{
			hTempImg = hImgR;
		}
	}
	else
	{
		hTempImg = m_hImgSrc;
	}
	MirrorImage(hTempImg,&hTempImg,"row");
	DispObj(hTempImg,m_HWindowID);
	SetColor(m_HWindowID, "orange red");

	if (m_pTempChecker->m_lstSingleCellInnerCheck.size()>0)
	{
		std::list<s_InnerCheckInfo>::iterator itInfo;
		itInfo = m_pTempChecker->m_lstSingleCellInnerCheck.begin();
		std::list<CInnerCheck>::iterator itInnerCheck;
		if (itInfo->lstInnerCheck.size()>0)
		{
			itInnerCheck = itInfo->lstInnerCheck.begin();
			if (itInnerCheck->m_sCheckPrintParam.sZouBanPara.bValid)
			{
				//if(!itInnerCheck->m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan)
				//{
					//MirrorRegion(itInnerCheck->m_hZoubanRegion,&hShowReg,"row",m_nImgHei);
					//DispObj(hShowReg,m_HWindowID);
				//}
				/*if (itInnerCheck->m_sCheckPrintParam.sZouBanPara.bIsInLoc)
				{
				SetColor(m_HWindowID, "red");
				MirrorRegion(itInnerCheck->m_hInlocMarkRegion,&hShowReg,"row",m_nImgHei);
				DispObj(hShowReg,m_HWindowID);
				}*/
			}			
		}		
	}
}

// 图像显示函数
//******************************************************************************//
//函数:图像显示函数
//参数:
//	pDC      输入，显示控件
//	lpDest   输入，原始图像数据
//	nDispx   输入，显示起始位置x
//  nDispy   输入，显示起始位置y
//  nDispWidth 输入，显示宽度
//  nDispHeight 输入，显示高度
//  nImagex     输入，显示图像起始位置x
//  nImagey     输入，显示图像起始位置y
//  nImageWidth 输入，显示图像宽度
//  nImageHeight 输入，显示图像高度
//  nWidth       输入，原始图像宽度
//  nHeight      输入，原始图像高度
//  BitsPerPixel 输入，原始图像位数
//返回:
//	是否显示成功
//******************************************************************************//
int CParaDlg::ShowImage(CDC * pDC,unsigned char *lpDest, int nDispx, int nDispy, int nDispWidth, int nDispHeight,
	int nImagex, int nImagey, int nImageWidth, int nImageHeight, 
	int nWidth, int nHeight, int BitsPerPixel)
{
	DWORD dwSize = nWidth*nHeight;//图像数据的宽和高;
	BITMAPFILEHEADER bmFH;
	int nPaletteEntries = 256;

	bmFH.bfType = 19778;
	bmFH.bfSize = (DWORD) ( sizeof(BITMAPFILEHEADER) + 
		sizeof(BITMAPINFOHEADER) +
		nPaletteEntries * sizeof(RGBQUAD)) +
		dwSize;
	bmFH.bfReserved1 = bmFH.bfReserved2 = 0;
	bmFH.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + 
		sizeof(BITMAPINFOHEADER) + 
		nPaletteEntries * sizeof(RGBQUAD);
	BITMAPINFO *FileBMP;
	char bmpBuf[2048];
	FileBMP = (BITMAPINFO *)bmpBuf;
	FileBMP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	FileBMP->bmiHeader.biWidth = nWidth;
	FileBMP->bmiHeader.biHeight = nHeight;
	FileBMP->bmiHeader.biPlanes = 1;
	FileBMP->bmiHeader.biBitCount = BitsPerPixel;
	FileBMP->bmiHeader.biCompression = BI_RGB;
	FileBMP->bmiHeader.biSizeImage = 0;
	FileBMP->bmiHeader.biXPelsPerMeter = 0;
	FileBMP->bmiHeader.biYPelsPerMeter = 0;
	FileBMP->bmiHeader.biClrUsed = 0;
	FileBMP->bmiHeader.biClrImportant = 0;
	//(2003-4-2)
	//对于8位位图来说，其需要调色板。对于256色地位图，调色板数组的个数位256。
	for( int k = 0; k < 256; ++k)
	{
		FileBMP->bmiColors[k].rgbBlue = (BYTE)k;
		FileBMP->bmiColors[k].rgbGreen = (BYTE)k;
		FileBMP->bmiColors[k].rgbRed = (BYTE)k;
		FileBMP->bmiColors[k].rgbReserved = 0;
	}
	//	pDC->SelectPalette(pScrnPalette, FALSE);
	//	pDC->RealizePalette();
	pDC->SetStretchBltMode(COLORONCOLOR);
	StretchDIBits((HDC)*pDC, 
		nDispx, nDispy, nDispWidth, nDispHeight,
		nImagex, nImagey, nImageWidth, nImageHeight, //图像宽高
		lpDest, FileBMP, DIB_RGB_COLORS, SRCCOPY);//
	return 1;
}

// 将图像结构裁切转换为显示图像
//******************************************************************************//
//函数:将RGB通道图裁切转换为显示图像
//参数:
//	sAlgImageInfo      输入，图像结构体
//	pDestImg   输出，裁切后的显示图
//	rcCut   输入，裁切区域
//返回:
//	是否裁切成功
//******************************************************************************//
BOOL CParaDlg::TransAlgImageToBuff(s_AlgImageInfo sAlgImageInfo,char *pDestImg,CRect rcCut)
{
	int i,j;
	int nDestWidth = WIDTHBYTES(rcCut.Width()*8)*(sAlgImageInfo.nBits/8);
	try
	{
		if (sAlgImageInfo.nBits==8)// 灰度图像
		{
			for (i=rcCut.left; i<rcCut.right; i++)
			{
				for (j=rcCut.top; j<rcCut.bottom; j++)
				{
					pDestImg[(j-rcCut.top)*nDestWidth+(i-rcCut.left)] = sAlgImageInfo.pImageData[j*WIDTHBYTES(sAlgImageInfo.sizeImage.cx*8)+i];
				}
			}
		}
		else if (sAlgImageInfo.nBits==24 || sAlgImageInfo.nBits==32)// 彩色图像
		{

			for (i=rcCut.left; i<rcCut.right; i++)
			{
				for (j=rcCut.top; j<rcCut.bottom; j++)
				{
					pDestImg[(j-rcCut.top)*nDestWidth+(i-rcCut.left)*sAlgImageInfo.nBits/8] = sAlgImageInfo.pImageR[j*WIDTHBYTES(sAlgImageInfo.sizeImage.cx*8)+i];
					pDestImg[(j-rcCut.top)*nDestWidth+(i-rcCut.left)*sAlgImageInfo.nBits/8+1] = sAlgImageInfo.pImageG[j*WIDTHBYTES(sAlgImageInfo.sizeImage.cx*8)+i];
					pDestImg[(j-rcCut.top)*nDestWidth+(i-rcCut.left)*sAlgImageInfo.nBits/8+2] = sAlgImageInfo.pImageB[j*WIDTHBYTES(sAlgImageInfo.sizeImage.cx*8)+i];
				}
			}
			/*	for (i=rcCut.left; i<rcCut.right; i++)
			{
			for (j=rcCut.top; j<rcCut.bottom; j++)
			{
			pDestImg[(j-rcCut.top)*nDestWidth+(i-rcCut.left)*sAlgImageInfo.nBits/8] = sAlgImageInfo.pImageB[j*WIDTHBYTES(sAlgImageInfo.sizeImage.cx*8)+i];
			pDestImg[(j-rcCut.top)*nDestWidth+(i-rcCut.left)*sAlgImageInfo.nBits/8+1] = sAlgImageInfo.pImageG[j*WIDTHBYTES(sAlgImageInfo.sizeImage.cx*8)+i];
			pDestImg[(j-rcCut.top)*nDestWidth+(i-rcCut.left)*sAlgImageInfo.nBits/8+2] = sAlgImageInfo.pImageR[j*WIDTHBYTES(sAlgImageInfo.sizeImage.cx*8)+i];
			}
			}*/
		}
	}
	catch (...)
	{
		return FALSE;		
	}
	return TRUE;
}

// 得到控件大小
//******************************************************************************//
//函数:得到控件大小
//参数:
//	nID      输入，控件ID
//	wndParent   输入，窗口
//	Client   输出，控件区域区域
//返回:
//	
//******************************************************************************//
void  CParaDlg::GetDlgItemRect(int nID,CWnd *wndParent,LPRECT Client)
{
	CRect WinRect,ClientRect;
	wndParent->GetClientRect(&WinRect);
	wndParent->ClientToScreen(&WinRect);
	wndParent->GetDlgItem(nID)->GetWindowRect(&ClientRect);
	Client->left = ClientRect.left-WinRect.left;
	Client->top = ClientRect.top-WinRect.top;
	Client->right = ClientRect.right-WinRect.left;
	Client->bottom = ClientRect.bottom-WinRect.top;
}

// 设定比率
void CParaDlg::OnRadioSetrate() 
{
	// TODO: Add your control notification handler code here
	m_bIsMouseWheel = TRUE;
}
// 原始显示
void CParaDlg::OnRadioNorm() 
{
	m_bIsMouseWheel = FALSE;
	m_nMoveXlast = 0;
	m_nMoveYlast  = 0;
	m_fScaleX = 1.0;
	m_fScaleY = 1.0;
	UpdateData(FALSE);
	ClearWindow(m_HWindowID);
	SetPart(m_HWindowID, 0, 0, 
		m_nDlgHeight-1, m_nDlgWidth-1);
	
	CRect recttmp;
	GetDlgItemRect(IDC_PIC,this,&recttmp);
	InvalidateRect(recttmp,TRUE);// 清屏
	//DrawImage();

}

// 合适显示
void CParaDlg::OnRadioAuto() 
{
	// TODO: Add your control notification handler code here
	m_bIsMouseWheel = FALSE;
	m_nMoveXlast = 0;
	m_nMoveYlast  = 0;
	m_fScaleX = (float)(m_nDlgWidth-1)/(m_nImgWid-1);
	m_fScaleY = (float)(m_nDlgHeight-1)/(m_nImgHei-1);
	UpdateData(FALSE);
	ClearWindow(m_HWindowID);
	SetPart(m_HWindowID, 0, 0, (m_nDlgHeight-1)/m_fScaleY,(m_nDlgWidth-1)/m_fScaleX);
	
	CRect recttmp;
	GetDlgItemRect(IDC_PIC,this,&recttmp);
	InvalidateRect(recttmp,TRUE);// 清屏
	//DrawImage();
}

// 对话框回车键不退出
BOOL CParaDlg::PreTranslateMessage(MSG* pMsg) 
{

	// TODO: Add your specialized code here and/or call the base class
	POINT ptTmp;
	GetCursorPos(&ptTmp);
	CRect Rect;
	CRect rcPIC;

	CWnd*  pPICWnd = GetDlgItem(IDC_PIC);
	CDC*   pDCPIC  = pPICWnd->GetDC();
	memset(Rect,0,sizeof(CRect));
	pPICWnd->ClientToScreen(Rect);// 坐标转换
	ptTmp.x = ptTmp.x-Rect.left;
	ptTmp.y = ptTmp.y-Rect.top;

	pPICWnd = GetDlgItem(IDC_PIC);	
	pPICWnd->GetClientRect(&rcPIC);

	if(rcPIC.PtInRect(ptTmp))
	{
		if (pMsg->message == WM_MOUSEWHEEL)
		{
			
		}
		if (pMsg->message == WM_LBUTTONDOWN)
		{	
			m_nMoveX = 0;
			m_nMoveY = 0;
			GetCursorPos(&ptOri);
			ptOri.x = ptOri.x-Rect.left;
			ptOri.y = ptOri.y-Rect.top;
			SetMshape (m_HWindowID, "Size All");
			m_bIsLButtonDown = TRUE;
		} 
		if (pMsg->message == WM_LBUTTONUP)
		{
			POINT pt;
			GetCursorPos(&pt);
			pt.x = pt.x-Rect.left;
			pt.y = pt.y-Rect.top;
			SetMshape (m_HWindowID, "arrow");

			if(rcPIC.PtInRect(pt))
			{
				if(m_bIsLButtonDown)
				{
					m_nMoveXlast = (m_nMoveXlast-m_nMoveX);
					m_nMoveYlast = (m_nMoveYlast-m_nMoveY);
				}
				m_bIsLButtonDown=FALSE;
			}
		}
		if(pMsg->message==WM_MOUSEMOVE && m_bIsLButtonDown)
		{
			GetCursorPos(&ptMove);
			ptMove.x = ptMove.x-Rect.left;
			ptMove.y = ptMove.y-Rect.top;

			pPICWnd = GetDlgItem(IDC_PIC);	
			pPICWnd->GetClientRect(&rcPIC);

			if(rcPIC.PtInRect(ptMove))
			{
				m_nMoveX = ptMove.x-ptOri.x;
				m_nMoveY = ptMove.y-ptOri.y;
				SetPart(m_HWindowID, (m_nMoveYlast-m_nMoveY)/m_fScaleY, (m_nMoveXlast-m_nMoveX)/m_fScaleX, 
					MAX(0,(m_nDlgHeight - 1 + m_nMoveYlast- m_nMoveY))/m_fScaleY, 
					MAX(0,(m_nDlgWidth -1 + m_nMoveXlast- m_nMoveX))/m_fScaleX);
				PostMessage(WM_PAINT);
			}	
		}
	}
	if (pMsg->message == WM_LBUTTONUP)
	{
		SetMshape (m_HWindowID, "arrow");
		m_bIsLButtonDown=FALSE;	
	}
	ReleaseDC(pDCPIC);
	//解决按下回车和ESC界面退出问题
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			return TRUE;
			break;
		case VK_RETURN:
			return TRUE;
		default:
			break;
		}
	}	
	return CBCGPDialog::PreTranslateMessage(pMsg);
}

void CParaDlg::OnBnClickedButtonParaSet()
{
	// TODO: Add your control notification handler code here
	// 默认弹第一个区域参数 [11/16/2011 Denghl]
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	// 显示检测的第一个区域的参数 [11/17/2011 Denghl]
	//	itInnerCheck->CodeCallParamDlg();
}

void CParaDlg::OnBnClickedButtonModifyMode()
{
	// 默认弹第一个区域参数 [11/16/2011 Denghl]
	//std::list<s_InnerCheckInfo>::iterator itInfo;
	//itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	//std::list<CInnerCheck>::iterator itInnerCheck;
	//itInnerCheck = itInfo->lstInnerCheck.begin();
	//// TODO: Add your control notification handler code here
	//if(bOcrDeal)
	//{
	//	itInnerCheck->CodeModifyTemplate();
	//}
}

void CParaDlg::OnBnClickedButtonRecord()
{
	// TODO: Add your control notification handler code here

	// 默认弹第一个区域参数 [11/16/2011 Denghl]
	//std::list<s_InnerCheckInfo>::iterator itInfo;
	//itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	//std::list<CInnerCheck>::iterator itInnerCheck;
	//itInnerCheck = itInfo->lstInnerCheck.begin();
	//if (bOcrDeal)
	//{
	//	Recorddlg.m_nCodeNumber = itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.bytCodeNum;
	//	Recorddlg.DoModal ();
	//}

}
void CParaDlg::OnEnChangeEditExpcode()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CParaDlg::OnEnKillfocusEditExpcode()
{
	// TODO: Add your control notification handler code here
	//CString strTmp = m_strexpCode;

	//// 默认弹第一个区域参数 [11/16/2011 Denghl]
	//std::list<s_InnerCheckInfo>::iterator itInfo;
	//itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	//std::list<CInnerCheck>::iterator itInnerCheck;
	//itInnerCheck = itInfo->lstInnerCheck.begin();
	//UpdateData(TRUE);
	//if (m_strexpCode.GetLength()!=itInnerCheck->g_OcrProExt.g_sCurNoteTypeInfo.uStandardCodeNum
	//	&&m_strexpCode.GetLength()>0)
	//{
	//	AfxMessageBox("输入预知号位数不对！");
	//	m_strexpCode = strTmp;
	//}
	//UpdateData(FALSE);

}

void CParaDlg::OnBnClickedButtonSettight()
{
	// TODO: Add your control notification handler code here

}

void CParaDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	// 默认弹第一个区域参数 [11/16/2011 Denghl]
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();	
	OnCancel();
}

void CParaDlg::OnBnClickedButtonTypeSet()
{
	// TODO: Add your control notification handler code here
	/*std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	itInnerCheck->CodeSetNoteInfo();*/
	//if (itInnerCheck->m_bChangedNoteType)
	//{
	//	AfxMessageBox("类型属性参数已修改，请重新加载任务！");
	//}
}

void CParaDlg::OnBnClickedButtonOnlyread()
{
	// TODO: Add your control notification handler code here
	//std::list<s_InnerCheckInfo>::iterator itInfo;
	//itInfo=m_pChecker->m_lstSingleCellInnerCheck.begin();
	//CLookTightDlg dlg;
	//int n =0;
	//for (;itInfo!=m_pChecker->m_lstSingleCellInnerCheck.end();itInfo++)
	//{
	//	std::list<CInnerCheck>::iterator itInnerCheck;
	//	itInnerCheck = itInfo->lstInnerCheck.begin();

	//	if (itInnerCheck->m_sDstRegionProperty.nProductRowIndex-1<0
	//		||itInnerCheck->m_sDstRegionProperty.nProductRowIndex-1>=8)
	//	{
	//		AfxMessageBox("输入开位信息不对");
	//		continue;
	//	}
	//	dlg.m_OcrProExt[itInnerCheck->m_sDstRegionProperty.nProductRowIndex-1] = &(itInnerCheck->g_OcrProExt);	
	//	n++;
	//}
	//dlg.m_nCaseNum = n;

	//dlg.DoModal();


}

void CParaDlg::OnBnClickedButtonChar()
{
	// TODO: Add your control notification handler code here
	//std::list<s_InnerCheckInfo>::iterator itInfo;
	//itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	//std::list<CInnerCheck>::iterator itInnerCheck;
	//itInnerCheck = itInfo->lstInnerCheck.begin();
	//CString str,tmp;
	//if(itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.bytCodeNum != itInnerCheck->g_OcrProExt.g_sCurNoteTypeInfo.uStandardCodeNum)
	//{
	//	return;
	//}
	////  [9/14/2001]
	//CSize szCode = itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.szRealCode;
	////
	//float m_fHScale = itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.m_fXFactScale,m_VScale =
	//	itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.m_fYFactScale; 

	//int nTmp1 = 0,nTmpBR = itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.bytCodeMergeInkThresh;	
	//for(int i = 0;i<itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.bytCodeNum;i++)
	//{
	//	nTmp1 += itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.lpbSepThresh[i];
	//}
	//nTmp1 /= itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.bytCodeNum;
	//tmp.Format("号码：宽度=%d,高度=%d\n冠字：宽度=%d,高度=%d\n串色灰度差:%d\n;平均分割阈值:%d\n;比例尺:(水平:%6.4f,垂直:%6.4f)",
	//	szCode.cx,szCode.cy,szCode.cx,(int)(szCode.cy*7/6.0+0.5),nTmpBR,nTmp1,m_fHScale,m_VScale);
	//AfxMessageBox (tmp,MB_OK,0);
}

void CParaDlg::OnBnClickedButtonLoookChar()
{
	// TODO: Add your control notification handler code here
	//std::list<s_InnerCheckInfo>::iterator itInfo;
	//itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	//std::list<CInnerCheck>::iterator itInnerCheck;
	//itInnerCheck = itInfo->lstInnerCheck.begin();
	//CString str,tmp;
	//if(itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.bytCodeNum != itInnerCheck->g_OcrProExt.g_sCurNoteTypeInfo.uStandardCodeNum)
	//{
	//	AfxMessageBox("请检测后查看参数");
	//	return;
	//}
	////  [9/14/2001]
	//CSize szCode = itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.szRealCode;
	////
	//float m_fHScale = itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.m_fXFactScale,m_VScale =
	//	itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.m_fYFactScale; 

	//int nTmp1 = 0,nTmpBR = itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.bytCodeMergeInkThresh;	
	//for(int i = 0;i<itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.bytCodeNum;i++)
	//{
	//	nTmp1 += itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.lpbSepThresh[i];
	//}
	//nTmp1 /= itInnerCheck->g_OcrProExt.g_sCodeAddResultInfo.bytCodeNum;
	//tmp.Format("号码：宽度=%d,高度=%d\n冠字：宽度=%d,高度=%d\n串色灰度差:%d\n;平均分割阈值:%d\n;比例尺:(水平:%6.4f,垂直:%6.4f)",
	//	szCode.cx,szCode.cy,szCode.cx,(int)(szCode.cy*7/6.0+0.5),nTmpBR,nTmp1,m_fHScale,m_VScale);
	//AfxMessageBox (tmp,MB_OK,0);
}

//void CParaDlg::OnCbnSelchangeComboProductType()
//{
//	// TODO: Add your control notification handler code here
//	/*std::list<s_InnerCheckInfo>::iterator itInfo;
//	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
//	std::list<CInnerCheck>::iterator itInnerCheck;
//	itInnerCheck = itInfo->lstInnerCheck.begin();
//	int nSel = m_comProductType.GetCurSel();
//	if (nSel<0||nSel>=itInnerCheck->m_nProductNum)
//	{
//		return;
//
//	}
//	itInnerCheck->m_strProductType = itInnerCheck->m_vcstrAllProType.at(nSel);*/
//}

void CParaDlg::OnCbnSelchangeComboCodetype()
{
	// TODO: Add your control notification handler code here
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();

	int nSel = m_comCodeType.GetCurSel();
	if (nSel<0||nSel>3)
	{
		return;
	}
	if (itInnerCheck->m_iCheckMethod !=nSel)
	{
		itInnerCheck->FreeCheckMethod();
		itInnerCheck->m_iCheckMethod = nSel;
		itInnerCheck->MallocCheckMethod();		
	}
	//  [10/11/2016 zb]
	if (m_bCheckDengji)
	{
		if (itInnerCheck->m_iCheckMethod==1||itInnerCheck->m_iCheckMethod==3)//pdf417
		{
			m_comGBType.SetCurSel(0);
			itInnerCheck->m_sCheckPrintParam.sDengjiPara.nGBID = 0;
			GetDlgItem(IDC_COMBO_GBTYPE)->EnableWindow(FALSE);		
		}
		else
		{
			GetDlgItem(IDC_COMBO_GBTYPE)->EnableWindow(TRUE);	
		}
	}
	else
	{
		GetDlgItem(IDC_COMBO_DENGJI)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_GBTYPE)->EnableWindow(FALSE);
	}
	
	if (itInnerCheck->m_iCheckMethod==1)
	{
		m_bFlagHorVer = FALSE;
	}
	else
	{
		m_bFlagHorVer = TRUE;
	}
	m_dlgSpacialSet->m_bFlagHorVer=m_bFlagHorVer;
	m_dlgSpacialSet->m_dlgInRegion->m_bShowHorVer = m_bFlagHorVer;
	m_dlgSpacialSet->m_dlgInRegion->UpdateState();

}
void CParaDlg::OnCbnSelchangeComboPolar()
{
	// TODO: 在此添加控件通知处理程序代码
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	int nSel = m_comPolar.GetCurSel();
	if (nSel<0||nSel>1)
	{
		return;
	}
	//m_comPolar.GetLBText(nSel,m_strPolar);
	//  [10/11/2016 zb]
	//m_comPolar.GetLBText(nSel,m_strPolarChinese);
	if (nSel==1)
	{	
		m_strPolar.Format("light_on_dark");
	} 
	else
	{
		m_strPolar.Format("dark_on_light");
	}
	itInnerCheck->m_strPolar = m_strPolar;
}

void CParaDlg::OnCbnSelchangeComboChannel()
{
	// TODO: Add your control notification handler code here
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	int nSel = m_comChannel.GetCurSel();
	if (nSel<0||nSel>4)
	{
		return;
	}
	itInnerCheck->m_nChannel = nSel;
	m_nChannel = itInnerCheck->m_nChannel;
	bOcrDeal = FALSE;
	CRect recttmp;
	GetDlgItemRect(IDC_PIC,this,&recttmp);
	InvalidateRect(recttmp,TRUE);// 清屏
}

void CParaDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	// TODO:  Add your control notification handler code here
}
void CParaDlg::OnBnClickedCheckImagesmooth()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo = m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	itInnerCheck->m_bIsImageSmooth = m_bIsImageSmooth;
}
void CParaDlg::OnBnClickedCheckImagezoom()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo = m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	itInnerCheck->m_bIsZoomCheck = m_bIsZoomCheck;
}

void CParaDlg::OnBnClickedCheckAll()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	itInnerCheck->m_bCheckAllMod = m_bCheckAllMod;
}

void CParaDlg::OnBnClickedCheckGao()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();	
	itInnerCheck->FreeCheckMethod();
	itInnerCheck->m_bCheckGao = m_bCheckGao;
	itInnerCheck->MallocCheckMethod();	
}

void CParaDlg::OnEnChangeEditHorscal()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	UpdateData(TRUE);
	m_dlgSpacialSet->m_fScaleHor = m_fScaleHor;
	m_dlgSpacialSet->m_dlgOutRange->m_fScaleHor = m_fScaleHor;
	m_dlgSpacialSet->m_dlgZouBan->m_fScaleHor   = m_fScaleHor;
	//m_dlgSpacialSet->m_dlgOutRange->UpdateData(FALSE);
	//m_dlgSpacialSet->m_dlgZouBan->UpdateData(FALSE);
	itInnerCheck->m_fScaleHor = m_fScaleHor;
	//UpdateData(FALSE);
}

void CParaDlg::OnEnChangeEditVerscal()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	UpdateData(TRUE);
	m_dlgSpacialSet->m_fScaleVer = m_fScaleVer;	
	m_dlgSpacialSet->m_dlgOutRange->m_fScaleVer = m_fScaleVer;
	m_dlgSpacialSet->m_dlgZouBan->m_fScaleVer   = m_fScaleVer;
	//m_dlgSpacialSet->m_dlgOutRange->UpdateData(FALSE);
	//m_dlgSpacialSet->m_dlgZouBan->UpdateData(FALSE);
	itInnerCheck->m_fScaleVer = m_fScaleVer;
	//UpdateData(FALSE);
	//纵向分辨率变化时，同步更新错位的像素数，且暂时采用纵向的分辨率计算错位的像素数
	m_dlgSpacialSet->m_sCheckPrintParam.sCorrPixelParam.nCorrPixels = (int)(m_dCorrPixel/m_fScaleVer+0.5);
}

void CParaDlg::OnEnKillfocusEditHorscal()
{
	// TODO: Add your control notification handler code here
	
}

void CParaDlg::OnEnKillfocusEditVerscal()
{
	// TODO: Add your control notification handler code here
	
}
void CParaDlg::OnBnClickedButtonSetdeng()
{
	// TODO: Add your control notification handler code here
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo = m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();

	// 根据产品类型和标准设置，判断应该启用的标准对话框 [12/27/2013 Denghl]
	if (itInnerCheck->m_iCheckMethod==0||itInnerCheck->m_iCheckMethod==2)//ECC200和QR
	{
		if (m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.nGBID==0)
		{
			CDialogDengji dlg;
			dlg.m_bDJ1 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[0];
			dlg.m_bDJ2 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[1];
			dlg.m_bDJ3 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[2];
			dlg.m_bDJ4 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[3];
			dlg.m_bDJ5 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[4];
			dlg.m_bDJ6 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[5];
			dlg.m_bDJ7 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[6];
			//  [4/7/2017 zb]
			dlg.m_nLanguage = m_nLanguage;
			dlg.m_strDefaultParaPath = m_strDefaultParaPath;
			if (dlg.DoModal()==IDOK)
			{
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[0] = dlg.m_bDJ1;
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[1] = dlg.m_bDJ2;
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[2] = dlg.m_bDJ3;
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[3] = dlg.m_bDJ4;
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[4] = dlg.m_bDJ5;
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[5] = dlg.m_bDJ6;
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[6] = dlg.m_bDJ7;
			}
		}
		else if (m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.nGBID==1)
		{
			CDialogDengji2006 dlg;
			dlg.m_bDJ1 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[0];
			dlg.m_bDJ2 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[1];
			dlg.m_bDJ3 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[2];
			dlg.m_bDJ4 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[3];
			dlg.m_bDJ5 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[4];
			dlg.m_bDJ6 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[5];
			dlg.m_bDJ7 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[6];
			if (dlg.DoModal()==IDOK)
			{
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[0] = dlg.m_bDJ1;
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[1] = dlg.m_bDJ2;
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[2] = dlg.m_bDJ3;
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[3] = dlg.m_bDJ4;
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[4] = dlg.m_bDJ5;
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[5] = dlg.m_bDJ6;
				m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[6] = dlg.m_bDJ7;
			}
		}
		else
		{
			//AfxMessageBox("等级标准未选定");
			CString str;
			str.Format("%s",m_strMessageText[26]);
			::MessageBoxA(m_hWnd,str,"""",MB_OK);

		}
	}
	else if (itInnerCheck->m_iCheckMethod==1)
	{
		CDialogDengjiPdf417 dlg;
		dlg.m_bDJ1 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[0];
		dlg.m_bDJ2 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[1];
		dlg.m_bDJ3 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[2];
		dlg.m_bDJ4 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[3];
		dlg.m_bDJ5 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[4];
		dlg.m_bDJ6 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[5];
		//dlg.m_bDJ7 = itInnerCheck->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[6];
		if (dlg.DoModal()==IDOK)
		{
			m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[0] = dlg.m_bDJ1;
			m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[1] = dlg.m_bDJ2;
			m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[2] = dlg.m_bDJ3;
			m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[3] = dlg.m_bDJ4;
			m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[4] = dlg.m_bDJ5;
			m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[5] = dlg.m_bDJ6;
			//itInnerCheck->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[6] = dlg.m_bDJ7;
		}
	}
	else
	{
		//AfxMessageBox("未设置好条码类型，无法选标准");
		CString str;
		str.Format("%s",m_strMessageText[27]);
		::MessageBoxA(m_hWnd,str,"""",MB_OK);
	}
	m_sCheckPrintParam = m_dlgSpacialSet->m_sCheckPrintParam;
}

void CParaDlg::OnCbnSelchangeComboDengji()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_comDengSet.GetCurSel();
	if (nIndex<0)
	{
		return;
	}
	m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.nSetGrade = nIndex;
}

void CParaDlg::OnBnClickedCheckDengji()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.bValid = m_bCheckDengji;
	// 等级模块显示 [12/27/2013 Denghl]
	if (m_bCheckDengji)
	{
		GetDlgItem(IDC_COMBO_DENGJI)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_GBTYPE)->EnableWindow(TRUE);
		//GetDlgItem(IDC_BUTTON_SETDENG)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_DENGJI)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_GBTYPE)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_SETDENG)->EnableWindow(FALSE);
	}
}

void CParaDlg::OnCbnSelchangeComboGbtype()
{
	// TODO: Add your control notification handler code here
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();

	int nIndex = m_comGBType.GetCurSel();
	if (nIndex<0)
	{
		return;
	}
	// 根据条码类型判断能选择的标准 [12/27/2013 Denghl]
	if (itInnerCheck->m_iCheckMethod>=3)
	{
		//AfxMessageBox("请选择合适的条码类型后选择相应标准1");
		CString str;
		str.Format("%s",m_strMessageText[35]);
		::MessageBoxA(m_hWnd,str,"""",MB_OK);
		m_comGBType.SetCurSel(0);
		m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.nGBID = 0;
		itInnerCheck->m_sCheckPrintParam.sDengjiPara.nGBID = 0;
		return ;
	}
	else if (itInnerCheck->m_iCheckMethod==1)//pdf417
	{
		m_comGBType.SetCurSel(0);
		m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.nGBID = 0;
		itInnerCheck->m_sCheckPrintParam.sDengjiPara.nGBID = 0;
		return;
	}
	m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.nGBID = nIndex;
	itInnerCheck->m_sCheckPrintParam.sDengjiPara.nGBID = nIndex;
}

void CParaDlg::OnBnClickedCheckResultmode()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();	
	itInnerCheck->FreeCheckMethod();
	itInnerCheck->m_bCheckResultMode= m_bCheckResultMode;
	itInnerCheck->MallocCheckMethod();	
}
void CParaDlg::OnBnClickedCheckCode()
{
	// TODO: Add your control notification handler code here
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	UpdateData(TRUE);
	if (m_bCheckPartCode)
	{
		//  [12/5/2016 zb]
		if (m_strResultCode.GetLength()<=0)
		{
			m_bCheckPartCode = FALSE;
			UpdateData(FALSE);
			return ;
		}
		GetDlgItem(IDC_EDIT_CODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CODEH)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_TO)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_CODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_CODEH)->EnableWindow(TRUE);

		/*GetDlgItem(IDC_STATIC_EXP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_RECCODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RESULT_CODE)->ShowWindow(SW_SHOW);*/
		//OnEnKillfocusEditResultnum();
		OnEnChangeEditCodeh();
	}
	else
	{
		GetDlgItem(IDC_EDIT_CODEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CODEH)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_CODEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_CODEH)->EnableWindow(FALSE);

		/*GetDlgItem(IDC_STATIC_EXP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RECCODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RESULT_CODE)->ShowWindow(SW_HIDE);*/
	}
	itInnerCheck->m_bCheckPartCode = m_bCheckPartCode;
}

void CParaDlg::OnEnChangeEditCodel()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	UpdateData(TRUE);
	itInnerCheck->m_CodeCharL = m_CodeCharL;
	OnEnKillfocusEditResultnum();
	UpdateData(FALSE);
}

void CParaDlg::OnEnChangeEditCodeh()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	UpdateData(TRUE);
	//  [10/11/2016 zb]
	if (itInnerCheck->m_CheckResultCode.size()>0&&m_CodeCharH>itInnerCheck->m_CheckResultCode.size()-1)
	{
		m_CodeCharH = itInnerCheck->m_CheckResultCode.size()-1;
	}
	itInnerCheck->m_CodeCharH = m_CodeCharH;
	UpdateData(FALSE);
	OnEnKillfocusEditResultnum();
}


void CParaDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


//void CParaDlg::OnBnClickedBtnAdvancepara()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	if (!m_bClickAdv)
//	{
//		SetWindowPos(NULL, 0, 0, m_rectDlg.Width(), m_rectDlg.Height(), SWP_NOMOVE|SWP_NOZORDER);
//		GetDlgItem(IDC_BTN_ADVANCEPARA)->SetWindowText("高级<<");
//		m_bClickAdv = TRUE;
//	}
//	else if (m_bClickAdv)
//	{		
//		SetWindowPos(NULL, 0, 0, m_rectInitDlg.Width(), m_rectInitDlg.Height(), SWP_NOMOVE|SWP_NOZORDER);
//		GetDlgItem(IDC_BTN_ADVANCEPARA)->SetWindowText("高级>>");
//		m_bClickAdv = FALSE;
//	}
//	this->CenterWindow();
//}
void CParaDlg::OnBnClickedButtonAdvpara()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bClickAdv)
	{
		SetWindowPos(NULL, 0, 0, m_rectDlg.Width(), m_rectDlg.Height(), SWP_NOMOVE|SWP_NOZORDER);
		//  [4/7/2017 zb]
		if (m_nLanguage == 1)
		{
			GetDlgItem(IDC_BUTTON_ADVPARA)->SetWindowText("Advance Param<<");
		} 
		else
		{
			GetDlgItem(IDC_BUTTON_ADVPARA)->SetWindowText("高级参数<<");
		}
		
		m_bClickAdv = TRUE;
		//  [12/5/2016 zb]
		GetDlgItem(IDC_STATIC_EXP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_RECCODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RESULT_CODE)->ShowWindow(SW_SHOW);
	}
	else if (m_bClickAdv)
	{		
		SetWindowPos(NULL, 0, 0, m_rectInitDlg.Width(), m_rectInitDlg.Height(), SWP_NOMOVE|SWP_NOZORDER);
		//  [4/7/2017 zb]
		if (m_nLanguage == 1)
		{
			GetDlgItem(IDC_BUTTON_ADVPARA)->SetWindowText("Advance Param>>");
		} 
		else
		{
			GetDlgItem(IDC_BUTTON_ADVPARA)->SetWindowText("高级参数>>");
		}
		
		m_bClickAdv = FALSE;
		//  [12/5/2016 zb]
		GetDlgItem(IDC_STATIC_EXP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RECCODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RESULT_CODE)->ShowWindow(SW_HIDE);
	}
	this->CenterWindow();
}

//void CParaDlg::OnBnClickedCheckHorver()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//	m_dlgSpacialSet->m_sCheckPrintParam.sInHorverPara.bValid = m_bCheckHorVer;
//}


void CParaDlg::OnBnClickedCheckAligncorrect()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bCheckAlignCor)
	{
		GetDlgItem(IDC_EDIT_CORRPIXEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_MAXPIXEL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CORRPIXEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_MAXPIXEL)->EnableWindow(TRUE);
	}
	m_dlgSpacialSet->m_sCheckPrintParam.sCorrPixelParam.bValid = m_bCheckAlignCor;
}


void CParaDlg::OnEnChangeEditCorrpixel()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_dlgSpacialSet->m_sCheckPrintParam.sCorrPixelParam.nCorrPixels = (int)(m_dCorrPixel/m_fScaleVer+0.5);
}


void CParaDlg::OnDeltaposSpinMaxpixel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_dCorrPixel+=0.1;
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_dCorrPixel >= 0.1)
		{
			m_dCorrPixel-=0.1;
		}	
	}
	m_dlgSpacialSet->m_sCheckPrintParam.sCorrPixelParam.nCorrPixels = (int)(m_dCorrPixel/m_fScaleVer+0.5);
	UpdateData(FALSE);
	*pResult = 0;
}


void CParaDlg::OnDeltaposSpinCodel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_CodeCharL++;
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_CodeCharL >= 1)
		{
			m_CodeCharL--;
		}	
	}
	UpdateData(FALSE);
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	itInnerCheck->m_CodeCharL = m_CodeCharL;

	*pResult = 0;
}


void CParaDlg::OnDeltaposSpinCodeh(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_CodeCharH++;
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_CodeCharH >= 1)
		{
			m_CodeCharH--;
		}	
	}
	UpdateData(FALSE);
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	itInnerCheck->m_CodeCharH = m_CodeCharH;

	*pResult = 0;
}


void CParaDlg::OnDeltaposSpinHor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_fScaleHor += 0.01;
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_fScaleHor >= 0.01)
		{
			m_fScaleHor -= 0.01;
		}	
	}
	UpdateData(FALSE);
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	m_dlgSpacialSet->m_fScaleHor = m_fScaleHor;	
	m_dlgSpacialSet->m_dlgOutRange->m_fScaleHor = m_fScaleHor;
	m_dlgSpacialSet->m_dlgZouBan->m_fScaleHor   = m_fScaleHor;
	m_dlgSpacialSet->m_dlgOutRange->UpdateData(FALSE);
	m_dlgSpacialSet->m_dlgZouBan->UpdateData(FALSE);
	itInnerCheck->m_fScaleHor = m_fScaleHor;

	*pResult = 0;
}


void CParaDlg::OnDeltaposSpinVer(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		m_fScaleVer += 0.01;
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_fScaleVer >= 0.01)
		{
			m_fScaleVer -= 0.01;
		}	
	}
	UpdateData(FALSE);
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	m_dlgSpacialSet->m_fScaleVer = m_fScaleVer;	
	m_dlgSpacialSet->m_dlgOutRange->m_fScaleVer = m_fScaleVer;
	m_dlgSpacialSet->m_dlgZouBan->m_fScaleVer   = m_fScaleVer;
	m_dlgSpacialSet->m_dlgOutRange->UpdateData(FALSE);
	m_dlgSpacialSet->m_dlgZouBan->UpdateData(FALSE);
	itInnerCheck->m_fScaleVer = m_fScaleVer;

	*pResult = 0;
}


void CParaDlg::OnStnClickedDrawLoc()
{
	// TODO: 在此添加控件通知处理程序代码
	bOcrDeal = FALSE;
	if (m_bFlagHorVer||m_bFlagInRegion||m_bFlagOutRange||m_bFlagQrloc)
	{
		m_dlgSpacialSet->GetTempCheckParam();
		m_sCheckPrintParam = m_dlgSpacialSet->m_sCheckPrintParam;
		//m_pTempChecker->m_sCheckPrintParam = m_sCheckPrintParam;	    
	}
	DrawImage();
	//画内部定位区域
	if (m_sCheckPrintParam.sZouBanPara.bValid && m_sCheckPrintParam.sZouBanPara.bIsInLoc)
	{
		GetDlgItem(IDC_DRAW_LOC)->EnableWindow(FALSE);
		std::list<s_InnerCheckInfo>::iterator itInfo;
		if (m_pTempChecker->m_lstSingleCellInnerCheck.size()>0)
		{
			itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
			std::list<CInnerCheck>::iterator itInnerCheck;
			if ( itInfo->lstInnerCheck.size()>0)
			{
				itInnerCheck = itInfo->lstInnerCheck.begin();
				itInnerCheck->m_sCheckPrintParam = m_sCheckPrintParam;
				HObject hDrawRegion;
				HTuple hRow1,hCol1,hRow2,hCol2;
				SetColor(m_HWindowID, "yellow");
				DrawRectangle1(m_HWindowID,&hRow1,&hCol1,&hRow2,&hCol2);
				GenRectangle1(&hDrawRegion,hRow1,hCol1,hRow2,hCol2);
				MirrorRegion(hDrawRegion,&itInnerCheck->m_hInlocMarkRegion,"row",m_nImgHei);
				GetDlgItem(IDC_DRAW_LOC)->EnableWindow(TRUE);
				HObject hImageSrc,hPartImg;
				//GenImage1Extern(&hImageSrc,"byte",WIDTHBYTES(m_nImgWid*8),m_nImgHei,(HTuple)m_pImageSrc,NULL);
				//ReduceDomain(hImageSrc,itInnerCheck->m_hInlocMarkRegion,&hPartImg);
				ReduceDomain(m_hImgSrc,itInnerCheck->m_hInlocMarkRegion,&hPartImg);
				//	CropDomain(hPartImg,&hPartImg);
				if (itInnerCheck->m_lMarkID>=0)
				{
					ClearNccModel(itInnerCheck->m_lMarkID);
					itInnerCheck->m_lMarkID = -1;
				}
				try
				{
					CreateNccModel(hPartImg,"auto",-0.39,0.79,
						"auto","use_polarity",
						&itInnerCheck->m_lMarkID);
				}
				catch(...)
				{
					//AfxMessageBox("Failed to create internal positioning！");
					CString str;
					str.Format("%s",m_strMessageText[28]);
					::MessageBoxA(m_hWnd,str,"""",MB_OK);
				}	
			}
		}
		DrawImage();
	}
	else
	{
		//AfxMessageBox("未开启检走版功能！");
		CString str;
		str.Format("%s",m_strMessageText[29]);
		::MessageBoxA(m_hWnd,str,"""",MB_OK);
	}
}


void CParaDlg::OnStnClickedDrawOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	bOcrDeal = FALSE;
	if (m_bFlagHorVer||m_bFlagInRegion||m_bFlagOutRange||m_bFlagQrloc)
	{
		m_dlgSpacialSet->GetTempCheckParam();
		m_sCheckPrintParam = m_dlgSpacialSet->m_sCheckPrintParam;	    
	}
	DrawImage();
	//画走版区域
	if (m_sCheckPrintParam.sZouBanPara.bValid)
	{
		GetDlgItem(IDC_DRAW_OFFSET)->EnableWindow(FALSE);
		std::list<s_InnerCheckInfo>::iterator itInfo;
		if (m_pTempChecker->m_lstSingleCellInnerCheck.size()>0)
		{
			itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
			std::list<CInnerCheck>::iterator itInnerCheck;
			if ( itInfo->lstInnerCheck.size()>0)
			{

				itInnerCheck = itInfo->lstInnerCheck.begin();
				itInnerCheck->m_sCheckPrintParam = m_sCheckPrintParam;
				HObject hDrawRegion;
				HTuple hRow1,hCol1,hRow2,hCol2;
				SetColor(m_HWindowID, "orange red");
				DrawRectangle1(m_HWindowID,&hRow1,&hCol1,&hRow2,&hCol2);
				GenRectangle1(&hDrawRegion,hRow1,hCol1,hRow2,hCol2);
				MirrorRegion(hDrawRegion,&itInnerCheck->m_hZoubanRegion,"row",m_nImgHei);
				GetDlgItem(IDC_DRAW_OFFSET)->EnableWindow(TRUE);
			}
		}
		DrawImage();
	}
	else
	{
		//AfxMessageBox("未开启检走版功能！");
		CString str;
		str.Format("%s",m_strMessageText[29]);
		::MessageBoxA(m_hWnd,str,"""",MB_OK);
	}
}

BOOL CParaDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值 
	if (m_bIsMouseWheel)
	{
		CRect Rect;
		CRect rcPIC;

		CPoint ptPIC = pt;
		CWnd*  pPICWnd = GetDlgItem(IDC_PIC);
		CDC    *pDCPIC = pPICWnd->GetDC();
		memset(Rect,0,sizeof(CRect));
		pPICWnd->ClientToScreen(Rect);// 坐标转换
		ptPIC.x = ptPIC.x-Rect.left;
		ptPIC.y = ptPIC.y-Rect.top;

		pPICWnd = GetDlgItem(IDC_PIC);	
		pPICWnd->GetClientRect(&rcPIC);

		if(rcPIC.PtInRect(ptPIC))
		{
			if(zDelta>0)
			{
				if(m_fScaleX>0.1&&m_fScaleY>0.1)
				{
					m_nZDelta += 120;
					m_fScaleY  = pow(1.25,-m_nZDelta/120);
					m_fScaleX  = pow(1.25,-m_nZDelta/120);
				}
			}
			else
			{
				if(m_fScaleX<10&&m_fScaleY<10)
				{
					m_nZDelta -= 120;
					m_fScaleY  = pow(0.8,m_nZDelta/120);
					m_fScaleX  = pow(0.8,m_nZDelta/120);
				}
			}
			ClearWindow(m_HWindowID);
			if(m_fScaleX>0.1&&m_fScaleY>0.1&&m_fScaleX<10&&m_fScaleY<10)
			{
				SetPart(m_HWindowID, 0, 0,(m_nDlgHeight-1)/m_fScaleY,(m_nDlgWidth-1)/m_fScaleX); 
			}
			//PostMessage(WM_PAINT);
			//DrawImage();
			CRect recttmp;
			GetDlgItemRect(IDC_PIC,this,&recttmp);
			InvalidateRect(recttmp,TRUE);// 清屏
		}
		ReleaseDC(pDCPIC);	
	}
	return CBCGPDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CParaDlg::OnEnKillfocusEditSfradio()
{
	UpdateData(TRUE);
	if (m_fZoomRadio<0.1)
	{
		m_fZoomRadio = 0.1;
	}
	if (m_fZoomRadio>10.0)
	{
		m_fZoomRadio = 10.0;
	}
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	itInnerCheck->m_fZoomRadio = m_fZoomRadio;
	UpdateData(FALSE);
	// TODO: 在此添加控件通知处理程序代码
}


void CParaDlg::OnEnKillfocusEditResultnum()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	std::list<s_InnerCheckInfo>::iterator itInfo;
	itInfo=m_pTempChecker->m_lstSingleCellInnerCheck.begin();
	std::list<CInnerCheck>::iterator itInnerCheck;
	itInnerCheck = itInfo->lstInnerCheck.begin();
	//  [10/11/2016 zb] 局部识别时再次判断校验位数上限
	if (m_bCheckPartCode)
	{
		if (m_nResultNum>(m_CodeCharH-m_CodeCharL+1))
		{
			m_nResultNum = m_CodeCharH-m_CodeCharL+1;
		}
	} 
	UpdateData(FALSE);
	itInnerCheck->m_nResultNum = m_nResultNum;
}
