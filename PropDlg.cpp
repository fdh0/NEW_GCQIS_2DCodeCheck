// PropDlg.cpp : implementation file
//
/************************************************************************
/* Copyright (c) 2008, 北京大恒图像视觉有限公司视觉算法部    
/* All rights reserved.             
/*                  
/* 文件名称：  PropDlg.cpp
/* 摘要： 属性对话框。
/*
/* 当前版本： 1.0
/* 创建者： 邓红丽
/* 完成日期： 2010年8月8日
/* 说明： 属性对话框中，修改参数、保存参数、参数分析和现实函数
************************************************************************/
#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "PropDlg.h"
#include "DialogDengji.h"
#include "DialogDengji2006.h"
#include "DialogDengjiPDF417.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropDlg dialog


CPropDlg::CPropDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CPropDlg::IDD, pParent)
	/*, m_strWord(_T(""))*/
	, m_bCheckAllMod(FALSE)
	, m_bCheckGao(FALSE)
	, m_fScaleHor(0)
	, m_fScaleVer(0)
	, m_bCheckDengji(FALSE)
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
	//{{AFX_DATA_INIT(CPropDlg)
	//m_strCheckResult = _T("");
	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;
	//}}AFX_DATA_INIT
	m_nShowImgType = 0;
	m_pImageSrc = NULL;
	m_pTempChecker = NULL;

	m_dlgSpacialSet = NULL;
	m_bFlagInRegion = TRUE;	
	m_bFlagQrloc    = TRUE;
	m_bFlagOutRange = TRUE;
	m_bFlagHorVer   = TRUE;
	m_dlgSpacialSet = NULL;
	m_bCheckResult = FALSE;	
	bOcrDeal = FALSE;
	m_bCheckPartCode = FALSE;
	m_CodeCharL = 0;
	m_CodeCharH = 0;
	m_bIsTurnedResult = FALSE;
	m_nZDelta   = 0;
	m_nMoveX    = 0;
	m_nMoveY    = 0;
	m_bIsLButtonDown = FALSE;
	m_nMoveXlast=0;
	m_nMoveYlast=0;
	m_bIsMouseWheel = FALSE;
	// 初始化不显示高级参数 [9/14/2015 TIAN]
	m_bClickAdv = FALSE;

	GenEmptyRegion(&m_hZoubanRegL);
	GenEmptyRegion(&m_hZoubanRegR);
	GenEmptyRegion(&m_hZoubanRegU);
	GenEmptyRegion(&m_hZoubanRegD);

	//BCG Control
	//CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS (CBCGPVisualManagerVS2010));
	EnableVisualManagerStyle(TRUE,TRUE);
	//m_strPolarChinese = _T("");
	//  [4/6/2017 zb]
	m_nLanguage         = 0;
	m_strLanguagePath   = _T("");
	m_strControlMapPath = _T("");
	m_strMessage        = _T("");
	memset(m_cMessage,0,256);
	m_strMessagePath= _T("");
	memset(m_strMessageText,0,512*256*sizeof(char));
}


void CPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropDlg)
	//DDX_Control(pDX, IDC_SCROLLBAR_VER, m_scrollVer);
	//DDX_Control(pDX, IDC_SCROLLBAR_HOR, m_scrollHor);
	//DDX_Text(pDX, IDC_STATIC_CHECKRESULT, m_strCheckResult);
	//DDX_Text(pDX, IDC_EDIT_SHOWRATE_X, m_fScaleX);
	//DDX_Text(pDX, IDC_EDIT_SHOWRATE_Y, m_fScaleY);
	//	DDX_Check(pDX, IDC_CHECK_ROTATE, m_bRotate);
	//}}AFX_DATA_MAP
	//DDX_Control(pDX, IDC_COMBO_PRODUCT_TYPE, m_comProductType);
	DDX_Control(pDX, IDC_COMBO_CODETYPE, m_comCodeType);
	//DDX_Text(pDX, IDC_EDIT_WORD, m_strWord);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_comChannel);
	DDX_Check(pDX, IDC_CHECK_ALL, m_bCheckAllMod);
	DDX_Check(pDX, IDC_CHECK_GAO, m_bCheckGao);
	DDX_Text(pDX, IDC_EDIT_HORSCAL, m_fScaleHor);
	DDX_Text(pDX, IDC_EDIT_VERSCAL, m_fScaleVer);
	DDX_Control(pDX, IDC_COMBO_DENGJI2, m_comDengSet);
	DDX_Check(pDX, IDC_CHECK_DENGJI, m_bCheckDengji);
	DDX_Control(pDX, IDC_COMBO_GBTYPE2, m_comGBType);

	DDX_Check(pDX, IDC_CHECK_CODE, m_bCheckPartCode);
	DDX_Check(pDX, IDC_CHECK_IsTurnedResult, m_bIsTurnedResult);
	DDX_Text(pDX, IDC_EDIT_CODEL, m_CodeCharL);
	DDV_MinMaxInt(pDX, m_CodeCharL, 0, 1024);
	DDX_Text(pDX, IDC_EDIT_CODEH, m_CodeCharH);
	DDV_MinMaxInt(pDX, m_CodeCharH, 0, 1024);
	DDX_Control(pDX, IDC_STATIC_SHOWMODE, m_labShowMode);
	DDX_Control(pDX, IDC_RADIO_AUTO, m_cmdRadioAuto);
	DDX_Control(pDX, IDC_RADIO_NORM, m_cmdRadioNorm);
	DDX_Control(pDX, IDC_RADIO_SETRATE, m_cmdRadioSetrate);
	DDX_Control(pDX, IDC_STATIC_2DTYPE, m_labType);
	DDX_Control(pDX, IDC_BTN_ADVANCEPARA, m_cmdAdvancePara);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_cmdTest);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_cmdSave);
	DDX_Control(pDX, IDC_BTN_EXIT, m_cmdExit);
	DDX_Control(pDX, IDC_BUTTON_READDEFAULT, m_cmdReadDefault);
	DDX_Control(pDX, IDC_BUTTON_WRITEDEFAULT, m_cmdWriteDefault);
	DDX_Control(pDX, IDC_STATIC_CHAN, m_labChannel);
	DDX_Control(pDX, IDC_CHECK_GAO, m_cmdCheckGao);
	DDX_Control(pDX, IDC_CHECK_ALL, m_cmdCheckAll);
	DDX_Control(pDX, IDC_CHECK_IsTurnedResult, m_cmdCheckTurnResult);
	//DDX_Control(pDX, IDC_CHECK_HORVER, m_cmdCheckHorVer);
	DDX_Control(pDX, IDC_CHECK_ALIGNCORRECT, m_cmdCheckAlignCor);
	DDX_Control(pDX, IDC_STATIC_MAXPIXEL, m_labMaxPixel);
	DDX_Control(pDX, IDC_CHECK_CODE, m_cmdCheckCode);
	DDX_Control(pDX, IDC_STATIC_CHECKRANGE, m_labCheckRange);
	DDX_Control(pDX, IDC_STATIC_HOR, m_labHor);
	DDX_Control(pDX, IDC_STATIC_VER, m_labVer);
	DDX_Control(pDX, IDC_CHECK_DENGJI, m_cmdCheckDengji);
	DDX_Control(pDX, IDC_STATIC_CHECKGRADE, m_labCheckGrade);
	DDX_Control(pDX, IDC_STATIC_CHECKSTD, m_labCheckStd);
	DDX_Control(pDX, IDC_BUTTON_SETDENG2, m_cmdSetDengji);
	//DDX_Check(pDX, IDC_CHECK_HORVER, m_bCheckHorVer);
	DDX_Check(pDX, IDC_CHECK_ALIGNCORRECT, m_bCheckAlignCor);
	DDX_Text(pDX, IDC_EDIT_CORRPIXEL, m_dCorrPixel);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_labTitle);
	DDX_Control(pDX, IDC_STATIC_TO, m_labTo);
	DDX_Control(pDX, IDC_DRAW_LOC, m_labDrawLoc);
	DDX_Control(pDX, IDC_DRAW_OFFSET, m_labDrawOffset);
	DDX_Text(pDX, IDC_EDIT_CHECKRESULT, m_strCheckResult);
	DDX_Control(pDX, IDC_COMBO_POLAR, m_comPolar);
	//DDX_CBString(pDX, IDC_COMBO_POLAR, m_strPolarChinese);
	DDX_Control(pDX, IDC_STATIC_POLAR, m_labPolar);
	DDX_Check(pDX, IDC_CHECK_IMAGESMOOTH, m_bIsImageSmooth);
	DDX_Control(pDX, IDC_CHECK_IMAGESMOOTH, m_cmdCheckImageSmooth);
	DDX_Check(pDX, IDC_CHECK_IMAGEZOOM, m_bIsZoomCheck);
	DDX_Control(pDX, IDC_CHECK_IMAGEZOOM, m_cmdCheckImageZoom);
	DDX_Text(pDX, IDC_EDIT_SFRADIO, m_fZoomRadio);
	DDV_MinMaxDouble(pDX, m_fZoomRadio, 0.1, 10.0);
	DDX_Control(pDX, IDC_STATICSFBL, m_bcgBilv);
	DDX_Text(pDX, IDC_EDIT_RESULTNUM, m_nResultNum);
	DDV_MinMaxInt(pDX, m_nResultNum, 0, 500);
	DDX_Control(pDX, IDC_STATIC_RESULT, m_labResultNum);
//	DDX_Control(pDX, IDC_EDIT_RESULTNUM, m_editResult);
}


BEGIN_MESSAGE_MAP(CPropDlg, CBCGPDialog)
	//{{AFX_MSG_MAP(CPropDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_RADIO_AUTO, OnRadioAuto)
	ON_BN_CLICKED(IDC_RADIO_NORM, OnRadioNorm)
	ON_BN_CLICKED(IDC_RADIO_SETRATE, OnRadioSetrate)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//	ON_BN_CLICKED(IDC_CHECK_ROTATE, OnCheckRotate)
	ON_BN_CLICKED(IDC_BUTTON_READDEFAULT, OnButtonReaddefault)
	ON_BN_CLICKED(IDC_BUTTON_WRITEDEFAULT, OnButtonWritedefault)
	//}}AFX_MSG_MAP
	//	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_TYPE, &CPropDlg::OnCbnSelchangeComboProductType)
	ON_BN_CLICKED(IDC_BUTTON_SETTYPE, &CPropDlg::OnBnClickedButtonSettype)
	ON_BN_CLICKED(IDC_BUTTON_PARA_SET, &CPropDlg::OnBnClickedButtonParaSet)
	//	ON_EN_CHANGE(IDC_EDIT_WORD, &CPropDlg::OnEnChangeEditWord)
	ON_CBN_SELCHANGE(IDC_COMBO_CODETYPE, &CPropDlg::OnCbnSelchangeComboCodetype)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, &CPropDlg::OnCbnSelchangeComboChannel)
	ON_BN_CLICKED(IDC_CHECK_ALL, &CPropDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_CHECK_GAO, &CPropDlg::OnBnClickedCheckGao)
	ON_EN_CHANGE(IDC_EDIT_HORSCAL, &CPropDlg::OnEnChangeEditHorscal)
	ON_EN_CHANGE(IDC_EDIT_VERSCAL, &CPropDlg::OnEnChangeEditVerscal)
	ON_EN_KILLFOCUS(IDC_EDIT_HORSCAL, &CPropDlg::OnEnKillfocusEditHorscal)
	ON_EN_KILLFOCUS(IDC_EDIT_VERSCAL, &CPropDlg::OnEnKillfocusEditVerscal)
	ON_BN_CLICKED(IDC_BUTTON_SETDENG2, &CPropDlg::OnBnClickedButtonSetdeng)
	ON_CBN_SELCHANGE(IDC_COMBO_DENGJI2, &CPropDlg::OnCbnSelchangeComboDengji)
	ON_BN_CLICKED(IDC_CHECK_DENGJI, &CPropDlg::OnBnClickedCheckDengji)
	ON_CBN_SELCHANGE(IDC_COMBO_GBTYPE2, &CPropDlg::OnCbnSelchangeComboGbtype)
	//ON_BN_CLICKED(IDC_BUTTON1, &CPropDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK_CODE, &CPropDlg::OnBnClickedCheckCode)
	ON_EN_CHANGE(IDC_EDIT_CODEL, &CPropDlg::OnEnChangeEditCodel)
	ON_EN_CHANGE(IDC_EDIT_CODEH, &CPropDlg::OnEnChangeEditCodeh)
	ON_BN_CLICKED(IDC_CHECK_IsTurnedResult, &CPropDlg::OnBnClickedCheckIsturnedresult)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CPropDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_ADVANCEPARA, &CPropDlg::OnBnClickedBtnAdvancepara)
	//ON_BN_CLICKED(IDC_CHECK_HORVER, &CPropDlg::OnBnClickedCheckHorver)
	ON_BN_CLICKED(IDC_CHECK_ALIGNCORRECT, &CPropDlg::OnBnClickedCheckAligncorrect)
	ON_EN_CHANGE(IDC_EDIT_CORRPIXEL, &CPropDlg::OnEnChangeEditCorrpixel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MAXPIXEL, &CPropDlg::OnDeltaposSpinMaxpixel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CODEL, &CPropDlg::OnDeltaposSpinCodel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CODEH, &CPropDlg::OnDeltaposSpinCodeh)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HOR, &CPropDlg::OnDeltaposSpinHor)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_VER, &CPropDlg::OnDeltaposSpinVer)
	ON_STN_CLICKED(IDC_DRAW_LOC, &CPropDlg::OnStnClickedDrawLoc)
	ON_STN_CLICKED(IDC_DRAW_OFFSET, &CPropDlg::OnStnClickedDrawOffset)
	ON_WM_MOUSEWHEEL()
	ON_CBN_SELCHANGE(IDC_COMBO_POLAR, &CPropDlg::OnCbnSelchangeComboPolar)
	ON_BN_CLICKED(IDC_CHECK_IMAGESMOOTH, &CPropDlg::OnBnClickedCheckImagesmooth)
	ON_BN_CLICKED(IDC_CHECK_IMAGEZOOM, &CPropDlg::OnBnClickedCheckImagezoom)
	ON_EN_KILLFOCUS(IDC_EDIT_SFRADIO, &CPropDlg::OnEnKillfocusEditSfradio)
	ON_EN_KILLFOCUS(IDC_EDIT_RESULTNUM, &CPropDlg::OnEnKillfocusEditResultnum)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropDlg message handlers

void CPropDlg::OnOK() 
{
	// TODO: Add extra validation here

	CBCGPDialog::OnOK();
}

// 画图
void CPropDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int a = 0;
	try
	{
		ClearWindow(m_HWindowID);
		if (!bOcrDeal)
		{
			DrawImage();
			if (m_bFlagHorVer||m_bFlagInRegion||m_bFlagOutRange||m_bFlagQrloc)
			{
				m_dlgSpacialSet->GetTempCheckParam();
				m_sCheckPrintParam = m_dlgSpacialSet->m_sCheckPrintParam;
				m_pTempChecker->m_sCheckPrintParam = m_sCheckPrintParam;	    
			}
			SetColor(m_HWindowID,"blue");
			if(m_pTempChecker->m_sCheckPrintParam.sZouBanPara.bValid)
			{
				//if(m_pTempChecker->m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan)
				{
					if(m_pTempChecker->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft)
					{
						DispObj(m_hZoubanRegL,m_HWindowID);
					}
					if(m_pTempChecker->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight)
					{
						DispObj(m_hZoubanRegR,m_HWindowID);
					}
					if(m_pTempChecker->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp)
					{
						DispObj(m_hZoubanRegU,m_HWindowID);
					}
					if(m_pTempChecker->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown)
					{
						DispObj(m_hZoubanRegD,m_HWindowID);
					}
				}
			}	
		}
		else
		{
			a = 1;
			// 显示实际检测旋转后的图像 [9/17/2013 Denghl]
			if (m_pTempChecker->m_bCheckPrint
				&&m_bCheckResult)
			{
				HObject hTempImg;
				HTuple hWid,hHei;
				GetImageSize(m_pTempChecker->m_ImageRotate,&hWid,&hHei);
				//SetPart(m_HWindowID, 0, 0, hHei-1,hWid-1);	
				MirrorImage(m_pTempChecker->m_ImageRotate,&hTempImg,"row");
				DispObj(hTempImg,m_HWindowID);
				SetColor(m_HWindowID, "green");	
				HObject hShowReg;
				MirrorRegion(m_pTempChecker->m_RegionAffineTrans,&hShowReg,"row",hHei);
				DispObj(hShowReg,m_HWindowID);
				// 显示 [9/17/2013 Denghl]
				if (m_pTempChecker->m_sCheckPrintParam.sInHorverPara.bValid)//显示喷头阻塞
				{
					SetColor(m_HWindowID, "blue");
					MirrorRegion(m_pTempChecker->m_InWhiteLine,&hShowReg,"row",hHei);
					DispObj(hShowReg,m_HWindowID);

					//////////////////////////////////////////////////////////////////////////
					HTuple lentemp,lenVer;
					TupleLength(m_pTempChecker->hv_Indices,&lentemp);
					TupleLength(m_pTempChecker->hv_IndicesVer,&lenVer);
					int HorStart = -1,VerStart=-1;
					if (lentemp>0)
					{
						HorStart = m_pTempChecker->hv_Indices[0].D();
					}
					if (lenVer>0)
					{
						VerStart = m_pTempChecker->hv_IndicesVer[0].D();
					}
					if (HorStart>=0||VerStart>=0)
					{
						HTuple hRow1,hRow2,hCol1,hCol2;
						SmallestRectangle1(m_pTempChecker->m_InRegion,&hRow1,&hCol1,&hRow2,&hCol2);
						HTuple len;

						HObject hunion;
						GenEmptyRegion(&hunion);
						TupleLength(m_pTempChecker->hv_Indices,&len);
						if (len>0&&m_pTempChecker->hv_Indices[0].D()>=0)
						{					
							for (int i=0;i<len;i++)
							{
								HObject hReg;
								GenRectangle1(&hReg,hRow1[0].I()+m_pTempChecker->hv_Indices[i].I(),hCol1[0].I(),hRow1[0].I()+m_pTempChecker->hv_Indices[i].I(),hCol2[0].I());
								Union2(hunion,hReg,&hunion);
							}
						}
						TupleLength(m_pTempChecker->hv_IndicesVer,&len);
						if (len>0&&m_pTempChecker->hv_IndicesVer[0].D()>=0)
						{					
							for (int i=0;i<len;i++)
							{
								HObject hReg;
								GenRectangle1(&hReg,hRow1[0].I(),hCol1[0].I()+m_pTempChecker->hv_IndicesVer[i].I(),hRow2[0].I(),hCol1[0].I()+m_pTempChecker->hv_IndicesVer[i].I());
								Union2(hunion,hReg,&hunion);
							}
						}
						SetColor(m_HWindowID, "yellow");
						MirrorRegion(hunion,&hShowReg,"row",hHei);
						DispObj(hShowReg,m_HWindowID);
					}				
				}
				a = 2;
				if (m_pTempChecker->m_sCheckPrintParam.sOutRangPara.bValid)//显示区域外脏
				{
					SetColor(m_HWindowID, "red");
					MirrorRegion(m_pTempChecker->m_RegionOutRang,&hShowReg,"row",hHei);
					DispObj(hShowReg,m_HWindowID);

					SetColor(m_HWindowID, "blue");
					MirrorRegion(m_pTempChecker->m_RegionOutRect,&hShowReg,"row",hHei);
					DispObj(hShowReg,m_HWindowID);
				}
				a = 3;
				if (m_pTempChecker->m_sCheckPrintParam.sInRegionPara.bValid)
				{
					SetColor(m_HWindowID, "blue");
					MirrorRegion(m_pTempChecker->m_InRegionQue,&hShowReg,"row",hHei);
					DispObj(hShowReg,m_HWindowID);
				}
				a = 4;
			}
			else
			{
				 a = 5;
				HObject hTempImg;
				HTuple hWid,hHei;
				SelectObj(m_pTempChecker->m_hImageCheck,&hTempImg,1);
				GetImageSize(hTempImg,&hWid,&hHei);
				SetPart(m_HWindowID, 0, 0, hHei-1,hWid-1);	
				MirrorImage(m_pTempChecker->m_hImageCheck,&hTempImg,"row");
				DispObj(hTempImg,m_HWindowID);
				a = 6;
				if (m_bCheckResult)
				{
					SetColor(m_HWindowID, "green");	
					HObject hShowReg,hRegionOrg;
					GenRegionContourXld(m_pTempChecker->hSymbolXLDs, &hRegionOrg, "filled");
					if (m_pTempChecker->m_bIsZoomCheck || m_pTempChecker->m_bZoomCheckFlag)
					{
						ZoomRegion(hRegionOrg,&hRegionOrg,m_pTempChecker->m_dHorEnlargeRate,m_pTempChecker->m_dVerEnlargeRate);
					}
					MirrorRegion(hRegionOrg,&hShowReg,"row",hHei);
					SetDraw(m_HWindowID,"margin");
					DispObj(hShowReg,m_HWindowID);
				}
				a = 7;

			}
			a = 8;
			if (m_pTempChecker->m_sCheckPrintParam.sZouBanPara.bValid)
			{
				HObject hShowReg;
				SetColor(m_HWindowID, "orange red");
				//if(m_pTempChecker->m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan)
				{
					if(m_pTempChecker->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft)
					{
						DispObj(m_pTempChecker->m_hRealZoubanRegionL,m_HWindowID);
					}
					if(m_pTempChecker->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight)
					{
						DispObj(m_pTempChecker->m_hRealZoubanRegionR,m_HWindowID);
					}
					if(m_pTempChecker->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp)
					{
						MirrorRegion(m_pTempChecker->m_hRealZoubanRegionU,&hShowReg,"row",m_nImgHei);
						DispObj(hShowReg,m_HWindowID);
					}
					if(m_pTempChecker->m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown)
					{
						MirrorRegion(m_pTempChecker->m_hRealZoubanRegionD,&hShowReg,"row",m_nImgHei);
						DispObj(hShowReg,m_HWindowID);
					}
				}
				a = 9;
				//else
				//{
				//MirrorRegion(m_pTempChecker->m_hRealZoubanRegion,&hShowReg,"row",m_nImgHei);
				//DispObj(hShowReg,m_HWindowID);
				//}
			}
		}
	}
	catch (HException hExcept)
	{
		CString strContent;
		strContent.Format("Fail(H):%d---%s--%s",a,hExcept.ProcName().Text(),hExcept.ErrorMessage().Text());
		//AfxMessageBox(strContent);
		::MessageBoxA(m_hWnd,strContent,"""",MB_OK);
	}
	catch (...)
	{
		CString strContent;
		strContent.Format("Fail(H):%d",a);
		//AfxMessageBox(strContent);
		::MessageBoxA(m_hWnd,strContent,"""",MB_OK);
	}
	
}

// 对话框初始化
BOOL CPropDlg::OnInitDialog() 
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
	m_labType.m_clrText       = RGB(218,165,0);
	m_labChannel.m_clrText    = RGB(218,165,0);
	m_labMaxPixel.m_clrText   = RGB(218,165,0);
	m_labCheckRange.m_clrText = RGB(218,165,0);
	m_labHor.m_clrText        = RGB(218,165,0);
	m_labVer.m_clrText        = RGB(218,165,0);
	m_labCheckGrade.m_clrText = RGB(218,165,0);
	m_labCheckStd.m_clrText   = RGB(218,165,0);
	m_labTo.m_clrText         = RGB(218,165,0); 
	m_labPolar.m_clrText      = RGB(218,165,0); 
	m_labDrawLoc.m_clrText    = RGB(0,255,0);
	m_labDrawOffset.m_clrText = RGB(0,255,0);
	m_bcgBilv.m_clrText = RGB(218,165,0);
	m_labResultNum.m_clrText = RGB(218,165,0);
	
	//Radio Buttom
	m_cmdRadioAuto.SetTextColor(RGB (255, 255, 255));
	m_cmdRadioAuto.SetMouseCursorHand();
	m_cmdRadioNorm.SetTextColor(RGB (255, 255, 255));
	m_cmdRadioNorm.SetMouseCursorHand();
	m_cmdRadioSetrate.SetTextColor(RGB (255, 255, 255));
	m_cmdRadioSetrate.SetMouseCursorHand();
	
	//Push buttom
	m_cmdAdvancePara.SetFaceColor(RGB (40, 105, 151));
	m_cmdAdvancePara.SetMouseCursorHand();
	m_cmdTest.SetFaceColor(RGB (40, 105, 151));
	m_cmdTest.SetMouseCursorHand();
	m_cmdSave.SetFaceColor(RGB (40, 105, 151));
	m_cmdSave.SetMouseCursorHand();
	m_cmdExit.SetFaceColor(RGB (40, 105, 151));
	m_cmdExit.SetMouseCursorHand();
	m_cmdReadDefault.SetFaceColor(RGB (40, 105, 151));
	m_cmdReadDefault.SetMouseCursorHand();
	m_cmdWriteDefault.SetFaceColor(RGB (40, 105, 151));
	m_cmdWriteDefault.SetMouseCursorHand();
	m_cmdSetDengji.SetFaceColor(RGB (40, 105, 151));
	m_cmdSetDengji.SetMouseCursorHand();

	//Check Buttom
	m_cmdCheckGao.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckGao.SetMouseCursorHand();
	m_cmdCheckAll.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckAll.SetMouseCursorHand();
	m_cmdCheckTurnResult.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckTurnResult.SetMouseCursorHand();
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
	long m_lWindowRow = rect.top;
	long m_lWindowColumn = rect.left;
	m_lWindowHeight = rect.Height();
	m_lWindowWidth = rect.Width(); 	
	m_nShowImgType = 0;

	CRect rcMinRect;// 区域的最小外接矩形 [5/18/2011 Denghl]
	std::list<CPoint>::iterator itRegionPoint;
	if (m_sDstRegionProperty.lstRegionOrg.size()<=0)
	{
		//AfxMessageBox("无原始区域坐标");
		::MessageBoxA(m_hWnd,"Region Coordinate Error!","""",MB_OK);
		return FALSE;
	}

	itRegionPoint  = m_sDstRegionProperty.lstRegionOrgJunc.begin();
	rcMinRect.top  = rcMinRect.bottom = itRegionPoint->y;
	rcMinRect.left = rcMinRect.right  = itRegionPoint->x;
	for(;itRegionPoint!=m_sDstRegionProperty.lstRegionOrgJunc.end();
		itRegionPoint++)
	{
		rcMinRect.top    = min(rcMinRect.top,itRegionPoint->y);
		rcMinRect.bottom = max(rcMinRect.bottom,itRegionPoint->y);
		rcMinRect.left   = min(rcMinRect.left,itRegionPoint->x);
		rcMinRect.right  = max(rcMinRect.right,itRegionPoint->x);
	}
	m_nImgWid = rcMinRect.right-rcMinRect.left;
	m_nImgHei = rcMinRect.bottom-rcMinRect.top;
	m_nImageBit = m_sAlgImageInfo.nBits;

	//m_pImageSrc = new BYTE[WIDTHBYTES(m_nImgWid*8)*m_nImgHei*(m_nImageBit/8)];
	//TransAlgImageToBuff(m_sAlgImageInfo,(char*)m_pImageSrc,rcMinRect);

	//GenImage1Extern(&m_hImgSrc,"byte",WIDTHBYTES(m_nImgWid*8),m_nImgHei,(HTuple)m_pImageSrc,NULL);
	//write_image(m_hImgSrc,"bmp",0,"d:\\m_hImgSrc.bmp");
	HObject hImgSrc,hImgSrcReduce;
	GenEmptyObj(&hImgSrc);
	if (m_sAlgImageInfo.nBits == 8)
	{	
		GenImage1Extern(&hImgSrc, "byte",
			(m_sAlgImageInfo.sizeImage.cx+3)/4*4,
			m_sAlgImageInfo.sizeImage.cy,(Hlong)m_sAlgImageInfo.pImageData, NULL);
	}
	else if(m_sAlgImageInfo.nBits == 24)
	{
		HObject HImgR, HImgG, HImgB;
		GenEmptyObj(&HImgR);
		GenEmptyObj(&m_hImgSrc);

		GenImage1Extern(&HImgR,"byte",WIDTHBYTES(m_sAlgImageInfo.sizeImage.cx*8),
			m_sAlgImageInfo.sizeImage.cy, 
			(Hlong)m_sAlgImageInfo.pImageR, NULL);	
		ConcatObj(m_hImgSrc,HImgR,&m_hImgSrc);
		GenImage1Extern(&HImgG,"byte",WIDTHBYTES(m_sAlgImageInfo.sizeImage.cx*8),
			m_sAlgImageInfo.sizeImage.cy, 
			(Hlong)m_sAlgImageInfo.pImageG, NULL);	
		ConcatObj(m_hImgSrc,HImgG,&m_hImgSrc);
		GenImage1Extern(&HImgB,"byte",WIDTHBYTES(m_sAlgImageInfo.sizeImage.cx*8),
			m_sAlgImageInfo.sizeImage.cy, 
			(Hlong)m_sAlgImageInfo.pImageB, NULL);	
		ConcatObj(m_hImgSrc,HImgB,&m_hImgSrc);

		/*GenImage1Extern(&HImgR, "byte", 
		WIDTHBYTES(m_sAlgImageInfo.sizeImage.cx+3)/4*4,
		m_sAlgImageInfo.sizeImage.cy, (Hlong)m_sAlgImageInfo.pImageR, NULL);
		GenImage1Extern(&HImgG, "byte", 
		(m_sAlgImageInfo.sizeImage.cx+3)/4*4, 
		m_sAlgImageInfo.sizeImage.cy, (Hlong)m_sAlgImageInfo.pImageG, NULL);
		GenImage1Extern(&HImgB, "byte", 
		(m_sAlgImageInfo.sizeImage.cx+3)/4*4, 
		m_sAlgImageInfo.sizeImage.cy, (Hlong)m_sAlgImageInfo.pImageB, NULL);
		Compose3(HImgR, HImgG, HImgB, &hImgSrc);*/

//#ifdef DEBUG 
//		hImgSrc=HImgR;
//		WriteImage(HImgR,"jpg",0,"D:\\HImgR.jpg");
//		WriteImage(HImgG,"jpg",0,"D:\\HImgG.jpg");
//		WriteImage(HImgB,"jpg",0,"D:\\HImgB.jpg");
//		//WriteImage(hImgSrc,"jpg",0,"D:\\hImgSrc.jpg");
//#endif

	}
	// 生成最小外接矩形 [10/10/2015 TIAN]
	HObject hRectangle;
	GenRectangle1(&hRectangle,rcMinRect.top,rcMinRect.left,rcMinRect.bottom,rcMinRect.right);

	ReduceDomain(hImgSrc,hRectangle,&hImgSrcReduce);
	CropDomain(hImgSrcReduce,&m_hImgSrc);
	//write_image(m_hImgSrc,"bmp",0,"d:\\m_hImgSrc.bmp");

	GetHWindowID(IDC_PIC, m_HWindowID);	 
	m_pTempChecker = new CInnerCheck;
	//  [3/13/2017 zb]
	if (m_pChecker->m_lstStr==NULL)
	{
		m_pChecker->m_lstStr = new std::list<CString>[1];
	}
	m_pTempChecker->m_lstStr = &(m_pChecker->m_lstStr[0]);
	m_pTempChecker->Init(m_strTaskName,m_pChecker->m_sAlgInitParam,m_sDstRegionProperty,TRUE);
	
	//  [9/9/2015 dhl]
	m_pTempChecker->m_sCheckPrintParam.sTimeOut.bValid   = m_pChecker->m_bCheckTimeOut;
	m_pTempChecker->m_sCheckPrintParam.sTimeOut.nAllTime = m_pChecker->m_nAllTime;
	m_sCheckPrintParam = m_pTempChecker->m_sCheckPrintParam;

	//  [4/6/2017 zb] 国际化配置，界面英文显示
	m_nLanguage = m_pChecker->m_sAlgInitParam.nLanguage;//0 中文 1英文
	m_strLanguagePath.Format("%s\\International Config\\ControlEnglish.ini", m_pTempChecker->m_strDefaultParaPath);
	m_strControlMapPath.Format("%s\\International Config\\ControlMap.ini", m_pTempChecker->m_strDefaultParaPath);
	if (m_nLanguage == 1)//英文
	{
		int nNum = 0;
		nNum = GetPrivateProfileInt("Property Parameters", "Total Control Number", 0, m_strLanguagePath);
		if(nNum > 0)
		{
			s_ControlSet *psControlSet = NULL;
			psControlSet = new s_ControlSet[nNum];
			for (int i=1;i<=nNum;i++)
			{
				char str[256];
				memset(str,0,256);
				CString strNo;
				strNo.Format("PControl NO %d",i);
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
		m_strMessagePath.Format("%s\\International Config\\MessageChinese.ini", m_pTempChecker->m_strDefaultParaPath);
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
		m_strMessagePath.Format("%s\\International Config\\MessageEnglish.ini", m_pTempChecker->m_strDefaultParaPath);
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
	//生成走版白边区域
	if(m_sCheckPrintParam.sZouBanPara.bValid)
	{
		HTuple hWid,hHei;
		GetImageSize(m_hImgSrc,&hWid,&hHei);
		if(m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft)
		{
			GenRectangle1(&m_hZoubanRegL,0,m_sCheckPrintParam.sZouBanPara.m_nLeftPos,
				hHei-1,m_sCheckPrintParam.sZouBanPara.m_nLeftPos);
		}
		if(m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight)
		{
			GenRectangle1(&m_hZoubanRegR,0,m_sCheckPrintParam.sZouBanPara.m_nRightPos,
				hHei-1,m_sCheckPrintParam.sZouBanPara.m_nRightPos);
		}
		if(m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp)
		{
			GenRectangle1(&m_hZoubanRegU,m_sCheckPrintParam.sZouBanPara.m_nUpPos,0
				,m_sCheckPrintParam.sZouBanPara.m_nUpPos,hWid[0].I()-1);
			MirrorRegion(m_hZoubanRegU,&m_hZoubanRegU,"row",hHei);
		}
		if(m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown)
		{
			GenRectangle1(&m_hZoubanRegD,m_sCheckPrintParam.sZouBanPara.m_nDownPos,0
				,m_sCheckPrintParam.sZouBanPara.m_nDownPos,hWid[0].I()-1);
			MirrorRegion(m_hZoubanRegD,&m_hZoubanRegD,"row",hHei);
		}
	}
	// 请填写参数，将m_pTempChecker中的参数放到界面上，并且界面参数修改时，注意更新m_pTempChecker中的参数 [8/16/2010 dhl]
	int nIndex=0;

	m_comCodeType.AddString("Data Matrix");
	m_comCodeType.AddString("PDF417");
	m_comCodeType.AddString("QR");
	m_comCodeType.AddString("Auto");
	m_comCodeType.SetCurSel(m_pTempChecker->m_iCheckMethod);
	m_fScaleHor = m_pTempChecker->m_fScaleHor;
	m_fScaleVer = m_pTempChecker->m_fScaleVer;

	m_bCheckAllMod   = m_pTempChecker->m_bCheckAllMod;
	m_bCheckGao      = m_pTempChecker->m_bCheckGao;
	m_bIsTurnedResult= m_pTempChecker->m_bIsTurnedResult;
	m_bCheckPartCode = m_pTempChecker->m_bCheckPartCode;
	m_CodeCharL      = m_pTempChecker->m_CodeCharL;
	m_CodeCharH      = m_pTempChecker->m_CodeCharH;

	//  [12/30/2015 TIAN]
	m_bIsImageSmooth = m_pTempChecker->m_bIsImageSmooth;
	m_bIsZoomCheck   = m_pTempChecker->m_bIsZoomCheck;
	m_fZoomRadio     = m_pTempChecker->m_fZoomRadio;
	if (m_bCheckPartCode)
	{
		GetDlgItem(IDC_EDIT_CODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CODEH)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_CODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_CODEH)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CODEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CODEH)->EnableWindow(FALSE);
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
		m_comChannel.SetCurSel(m_pTempChecker->m_nChannel);
	}
	else
	{
		m_comChannel.AddString("Gray");
		m_comChannel.SetCurSel(0);
		m_pTempChecker->m_nChannel = 0;
		GetDlgItem(IDC_COMBO_CHANNEL)->EnableWindow(FALSE);
		//GetDlgItem(IDC_STATIC_CHAN)->EnableWindow(FALSE);
	}
	//  [4/6/2017 zb]
	// 添加二维码极性 
	m_strPolar = m_pTempChecker->m_strPolar;
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
		//m_strPolarChinese.Format("light_on_dark");
		m_comPolar.SetCurSel(1);
	} 
	else
	{
		m_comPolar.SetCurSel(0);
		//m_strPolarChinese.Format("dark_on_light");
	}
	m_nResultNum = m_pTempChecker->m_nResultNum;

	//横纵
	//m_bCheckHorVer   = m_sCheckPrintParam.sInHorverPara.bValid;
	//错位
	//m_bCheckAlignCor = m_sCheckPrintParam.sCorrPixelParam.bValid;
	//m_dCorrPixel     = m_sCheckPrintParam.sCorrPixelParam.nCorrPixels*m_fScaleVer;
	if (m_bCheckAlignCor)
	{
		GetDlgItem(IDC_EDIT_CORRPIXEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_MAXPIXEL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CORRPIXEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_MAXPIXEL)->EnableWindow(FALSE);
	}
	//等级
	m_bCheckDengji = m_sCheckPrintParam.sDengjiPara.bValid;
	m_comDengSet.SetCurSel(m_sCheckPrintParam.sDengjiPara.nSetGrade);
	m_comGBType.SetCurSel(m_sCheckPrintParam.sDengjiPara.nGBID);

	// 等级模块显示 [12/27/2013 Denghl]
	if (m_bCheckDengji)
	{
		GetDlgItem(IDC_COMBO_DENGJI2)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_GBTYPE2)->EnableWindow(TRUE);
		//GetDlgItem(IDC_BUTTON_SETDENG2)->EnableWindow(TRUE);
		if (m_pTempChecker->m_iCheckMethod==1||m_pTempChecker->m_iCheckMethod==3)
		{
			m_comGBType.SetCurSel(0);
			m_pTempChecker->m_sCheckPrintParam.sDengjiPara.nGBID = 0;
			GetDlgItem(IDC_COMBO_GBTYPE2)->EnableWindow(FALSE);			
		}
		else
		{
			GetDlgItem(IDC_COMBO_GBTYPE2)->EnableWindow(TRUE);	
		}
	}
	else
	{
		GetDlgItem(IDC_COMBO_DENGJI2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_GBTYPE2)->EnableWindow(FALSE);
	   // GetDlgItem(IDC_BUTTON_SETDENG2)->EnableWindow(FALSE);
	}
	
	if (m_pTempChecker->m_iCheckMethod==2||m_pTempChecker->m_iCheckMethod==3)
	{
		m_bFlagQrloc = TRUE;
	}
	else
	{
		m_bFlagQrloc = FALSE;
	}
	if (m_pTempChecker->m_iCheckMethod==1)
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
		m_dlgSpacialSet->m_bFlagHorVer      = m_bFlagHorVer;//控制是否显示喷头阻塞检测
		//  [11/4/2015 zl]
		m_dlgSpacialSet->m_hImgSrc=m_hImgSrc;
		m_dlgSpacialSet->m_pPropDlg         = this;
		m_dlgSpacialSet->nDlgSwitch         = 1;
		//  [4/6/2017 zb]
		m_dlgSpacialSet->m_nLanguage	    = m_nLanguage;
		m_dlgSpacialSet->m_strDefaultParaPath = m_pTempChecker->m_strDefaultParaPath;
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
	// 记录初始化化日志 [3/13/2017 zb]
	m_pChecker->WirteCurLog();
	//  [12/5/2016 zb]
	OnButtonTest();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

bool CPropDlg::GetHWindowID(int nControlID, HTuple& lHWindowID,BOOL bFlag)
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

// 对话框退出释放空间
void CPropDlg::OnDestroy() 
{
	CBCGPDialog::OnDestroy();
	HalconCpp::CloseWindow(m_HWindowID[0].D());
	m_pTempChecker->Free();
	if (m_pTempChecker)
	{
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
		//m_dlgSpacialSet->DestroyWindow();
		delete m_dlgSpacialSet;
		m_dlgSpacialSet = NULL;
	}
	DeleteObject(m_hFont1);
}

// 检测
void CPropDlg::OnButtonTest() 
{
	//int a = 0;
	try
	{
		bOcrDeal = FALSE;
		m_strCheckResult = _T("");

		s_InnerCheckInputParam sAlgCheckInput;
		sAlgCheckInput.sAlgImageInfo   = m_sAlgImageInfo;
		sAlgCheckInput.sRegionProperty = m_sDstRegionProperty;
		sAlgCheckInput.sRegionProperty.lstRegionOrg.clear();
		sAlgCheckInput.sRegionProperty.lstRegionOrg = sAlgCheckInput.sRegionProperty.lstRegionOrgJunc;
		s_InnerCheckOutputParam sAlgCheckOutput;

		if (m_bFlagHorVer||m_bFlagInRegion||m_bFlagOutRange||m_bFlagQrloc)
		{			
			m_dlgSpacialSet->GetTempCheckParam();
			m_sCheckPrintParam= m_dlgSpacialSet->m_sCheckPrintParam;
			m_pTempChecker->m_sCheckPrintParam = m_sCheckPrintParam;	    
		}
		m_pTempChecker->FreeCheckMethod();
		m_pTempChecker->MallocCheckMethod();
		//a = 1;
		//AfxMessageBox("1");
		HObject hSrcImage;
		HObject hImgTmp;
		GenEmptyObj(&hSrcImage);
		GenImage1Extern(&hSrcImage,"byte",WIDTHBYTES(sAlgCheckInput.sAlgImageInfo.sizeImage.cx*8),
			sAlgCheckInput.sAlgImageInfo.sizeImage.cy, 
			(Hlong)sAlgCheckInput.sAlgImageInfo.pImageR, NULL);
		if (sAlgCheckInput.sAlgImageInfo.nBits==24)
		{
			GenImage1Extern(&hImgTmp,"byte",WIDTHBYTES(sAlgCheckInput.sAlgImageInfo.sizeImage.cx*8),
				sAlgCheckInput.sAlgImageInfo.sizeImage.cy, 
				(Hlong)sAlgCheckInput.sAlgImageInfo.pImageG, NULL);
			ConcatObj(hSrcImage,hImgTmp,&hSrcImage);
			GenImage1Extern(&hImgTmp,"byte",WIDTHBYTES(sAlgCheckInput.sAlgImageInfo.sizeImage.cx*8),
				sAlgCheckInput.sAlgImageInfo.sizeImage.cy, 
				(Hlong)sAlgCheckInput.sAlgImageInfo.pImageB, NULL);
			ConcatObj(hSrcImage,hImgTmp,&hSrcImage);
		}
		//WriteObject(hSrcImage,"d://srcRes.obj");
	//	WriteImage(hSrcImage,"bmp",0,"d://srcRes.bmp");
		s_Status sTatus = m_pTempChecker->Check(sAlgCheckInput,hSrcImage,&sAlgCheckOutput);
		//  [3/13/2017 zb]
		m_pChecker->WirteCurLog();
		//AfxMessageBox("2");
		if (sTatus.nErrorType !=0)
		{
			return;
		}
		//a = 2;
		bOcrDeal = TRUE;
		if (m_pTempChecker->m_iRealMethod>=0&&m_pTempChecker->m_iRealMethod<3)
		{
			if (!m_bCheckAllMod)
			{
				m_pTempChecker->FreeCheckMethod();
				m_pTempChecker->m_iCheckMethod = m_pTempChecker->m_iRealMethod;
				m_pTempChecker->MallocCheckMethod();
				m_comCodeType.SetCurSel(m_pTempChecker->m_iCheckMethod);			
				UpdateData(FALSE);			
			}
		}
		
		// 显示检测结果 [11/16/2011 Denghl]
		if (sTatus.nErrorType!=0)
		{
			if (m_nLanguage == 1)
			{
				m_strCheckResult.Format("Test Result Error:\r\nErrorID=%d,\r\nState Description=%s,\r\nError Details=%s",
					sTatus.nErrorType,
					sTatus.chErrorInfo,
					sTatus.chErrorContext);
			} 
			else
			{
				m_strCheckResult.Format("检测结果错误:\r\n错误ID=%d,\r\n状态描述=%s,\r\n错误详细信息=%s",
					sTatus.nErrorType,
					sTatus.chErrorInfo,
					sTatus.chErrorContext);
			}
			
			UpdateData(FALSE);
			return;
		}
		//a = 3;
		CString str;
		CString strCode;
		CString strAllCode;
		if (sAlgCheckOutput.sErrorInfo.vcCheckResultCode.size()>0)
		{
			for (int j=0;j<int(sAlgCheckOutput.sErrorInfo.vcCheckResultCode.size());j++)
			{
				if (sAlgCheckOutput.sErrorInfo.vcCheckResultCode.at(j)!=0)
				{
					strCode += sAlgCheckOutput.sErrorInfo.vcCheckResultCode.at(j);
				}
				else
				{
					break;
				}
			}

		}
		if (m_pTempChecker->m_CheckResultCode.size()>0)
		{
			for (int j=0;j<int(m_pTempChecker->m_CheckResultCode.size());j++)
			{
				if (m_pTempChecker->m_CheckResultCode.at(j)!=0)
				{
					strAllCode += m_pTempChecker->m_CheckResultCode.at(j);
				}
				else
				{
					break;
				}
			}

		}
		//a = 4;
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

		if (!bFlag)
		{
			if (m_bCheckPartCode)//  [12/19/2016 zb]/*检测结果错误:\r\n*/
			{
				if (m_nLanguage == 1)
				{
					m_strCheckResult.Format("Region ID=%d,Error Type=%d,Error Grade =%d,Error Description=%s\r\nIdentification Number=%s\r\nLocal Number=%s\r\nAdditional Information=%s\r\n",
						sAlgCheckOutput.sErrorInfo.nRegionID,
						sAlgCheckOutput.sErrorInfo.bytErrorType,
						sAlgCheckOutput.sErrorInfo.bytErrorGrade,
						sAlgCheckOutput.sErrorInfo.cErrorDescription,
						strAllCode,
						strCode,
						sAlgCheckOutput.sErrorInfo.cResvered);
				} 
				else
				{
					m_strCheckResult.Format("区域ID=%d,错误类型=%d,错误级别=%d,错误描述=%s\r\n识别号码=%s\r\n局部号码=%s\r\n附加信息=%s\r\n",
						sAlgCheckOutput.sErrorInfo.nRegionID,
						sAlgCheckOutput.sErrorInfo.bytErrorType,
						sAlgCheckOutput.sErrorInfo.bytErrorGrade,
						sAlgCheckOutput.sErrorInfo.cErrorDescription,
						strAllCode,
						strCode,
						sAlgCheckOutput.sErrorInfo.cResvered);
				}
				
			}
			else
			{
				if (m_nLanguage == 1)
				{
					m_strCheckResult.Format("Region ID=%d,Error Type=%d,Error Grade=%d,Error Description=%s\r\nIdentification Number=%s\r\nAdditional Information=%s\r\n",
						sAlgCheckOutput.sErrorInfo.nRegionID,
						sAlgCheckOutput.sErrorInfo.bytErrorType,
						sAlgCheckOutput.sErrorInfo.bytErrorGrade,
						sAlgCheckOutput.sErrorInfo.cErrorDescription,
						strAllCode,
						sAlgCheckOutput.sErrorInfo.cResvered);
				} 
				else
				{
					m_strCheckResult.Format("区域ID=%d,错误类型=%d,错误级别=%d,错误描述=%s\r\n识别号码=%s\r\n附加信息=%s\r\n",
						sAlgCheckOutput.sErrorInfo.nRegionID,
						sAlgCheckOutput.sErrorInfo.bytErrorType,
						sAlgCheckOutput.sErrorInfo.bytErrorGrade,
						sAlgCheckOutput.sErrorInfo.cErrorDescription,
						strAllCode,
						sAlgCheckOutput.sErrorInfo.cResvered);
				}
			}
			
			//AfxMessageBox(m_strCheckResult);
		}
		else
		{
			if (m_bCheckPartCode)//  [12/19/2016 zb]
			{
				if (m_nLanguage == 1)
				{
					m_strCheckResult.Format("Region ID=%d,Error Type=%d,Error Grade =%d,Error Description=%s\r\nIdentification Number=%s\r\nLocal Number=%s\r\nAdditional Information=%s\r\nEscape Show Identification Character\r\n",
						sAlgCheckOutput.sErrorInfo.nRegionID,
						sAlgCheckOutput.sErrorInfo.bytErrorType,
						sAlgCheckOutput.sErrorInfo.bytErrorGrade,
						sAlgCheckOutput.sErrorInfo.cErrorDescription,
						strShow,
						strCode,
						sAlgCheckOutput.sErrorInfo.cResvered);
				} 
				else
				{
					m_strCheckResult.Format("区域ID=%d,错误类型=%d,错误级别=%d,错误描述=%s\r\n识别号码=%s\r\n局部号码=%s\r\n附加信息=%s\r\n对识别字符进行了转义显示\r\n",
						sAlgCheckOutput.sErrorInfo.nRegionID,
						sAlgCheckOutput.sErrorInfo.bytErrorType,
						sAlgCheckOutput.sErrorInfo.bytErrorGrade,
						sAlgCheckOutput.sErrorInfo.cErrorDescription,
						strShow,
						strCode,
						sAlgCheckOutput.sErrorInfo.cResvered);
				}
			}
			else
			{
				if (m_nLanguage == 1)
				{
					m_strCheckResult.Format("Region ID=%d,Error Type=%d,Error Grade =%d,Error Description=%s\r\nIdentification Number=%s\r\nAdditional Information=%s\r\nEscape Show Identification Character\r\n",
						sAlgCheckOutput.sErrorInfo.nRegionID,
						sAlgCheckOutput.sErrorInfo.bytErrorType,
						sAlgCheckOutput.sErrorInfo.bytErrorGrade,
						sAlgCheckOutput.sErrorInfo.cErrorDescription,
						strShow,
						sAlgCheckOutput.sErrorInfo.cResvered);
				} 
				else
				{
					m_strCheckResult.Format("区域ID=%d,错误类型=%d,错误级别=%d,错误描述=%s\r\n识别号码=%s\r\n附加信息=%s\r\n对识别字符进行了转义显示\r\n",
						sAlgCheckOutput.sErrorInfo.nRegionID,
						sAlgCheckOutput.sErrorInfo.bytErrorType,
						sAlgCheckOutput.sErrorInfo.bytErrorGrade,
						sAlgCheckOutput.sErrorInfo.cErrorDescription,
						strShow,
						sAlgCheckOutput.sErrorInfo.cResvered);
				}
			}		
		}	
		/*if (m_bCheckPartCode)
		{
		CString str;
		str.Format("\r\n局部号码显示=%s",strCode);
		m_strCheckResult+=str;
		}*/
		if (sAlgCheckOutput.sErrorInfo.bytErrorType!=ABNORMAL_2D
			&&sAlgCheckOutput.sErrorInfo.bytErrorType!=PRINTTRY)
		{
			m_bCheckResult = TRUE;
		}
		else
		{
			m_bCheckResult = FALSE;
		}
		if (m_pTempChecker->m_sCheckPrintParam.sDengjiPara.bValid
			&&(sAlgCheckOutput.sErrorInfo.bytErrorType==GRADELOW
			||sAlgCheckOutput.sErrorInfo.bytErrorType==NORMAL_2D))
		{
			m_strCheckResult += m_pTempChecker->m_strRealGrade;
		}

		if (m_pTempChecker->m_sCheckPrintParam.sInRegionPara.bValid
			&&sAlgCheckOutput.sErrorInfo.bytErrorType==INREGION)
		{
			CString strInR;
			if (m_nLanguage == 1)
			{
				strInR.Format("\r\nInky Ualue=%f,Inky Difference Degree=%f",
					m_pTempChecker->hv_Mean[0].D()+2*m_pTempChecker->hv_Deviation[0].D(),
					m_pTempChecker->hv_Deviation[0].D());
			} 
			else
			{
				strInR.Format("\r\n墨色值=%f,墨色差异度=%f",
					m_pTempChecker->hv_Mean[0].D()+2*m_pTempChecker->hv_Deviation[0].D(),
					m_pTempChecker->hv_Deviation[0].D());
			}
			
			m_strCheckResult += strInR;
		}
		if (m_pTempChecker->m_sCheckPrintParam.sInHorverPara.bValid
			&&sAlgCheckOutput.sErrorInfo.bytErrorType==HORVER)
		{
			CString strInR;
			if (m_pTempChecker->m_hWhiteLineValue>0)
			{
				//  [Baiyy]
				//strInR.Format("\r\n白线面积值=%.1f",
				//	m_pTempChecker->m_hWhiteLineValue[0].D()*m_fScaleHor*m_fScaleVer);
				if (m_nLanguage == 1)
				{
					strInR.Format("\r\nWhite Line Area=%.1f",
						m_pTempChecker->m_hWhiteLineValue[0].D()*m_pTempChecker->m_fScaleHor*m_pTempChecker->m_fScaleVer);
				} 
				else
				{
					strInR.Format("\r\n白线面积值=%.1f",
						m_pTempChecker->m_hWhiteLineValue[0].D()*m_pTempChecker->m_fScaleHor*m_pTempChecker->m_fScaleVer);
				}
				m_strCheckResult += strInR;
			}
			//////////////////////////////////////////////////////////////////////////
			if (m_pTempChecker->m_nHorverType == 31)
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
			else if (m_pTempChecker->m_nHorverType == 32)
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
		if (m_pTempChecker->m_sCheckPrintParam.sOutRangPara.bValid
			&&sAlgCheckOutput.sErrorInfo.bytErrorType==OUTRANGE)
		{
			CString strInR;
			//转换成平方毫米
			double dArea = 0;
			if (m_nLanguage == 1)
			{
				dArea = m_pTempChecker->hv_AreaAll[0].D()*m_pTempChecker->m_fScaleHor*m_pTempChecker->m_fScaleVer;
				strInR.Format("\nSmearing Area=%f",dArea);
			} 
			else
			{
				dArea = m_pTempChecker->hv_AreaAll[0].D()*m_pTempChecker->m_fScaleHor*m_pTempChecker->m_fScaleVer;
				strInR.Format("\n蹭脏面积=%f",dArea);
			}
			
			m_strCheckResult += strInR;
		}
		bOcrDeal = TRUE;
		UpdateData(FALSE);
		//a = 5;

		InvalidateRect(&m_rectImg);
	}
	catch (HException hExcept)
	{
		CString strContent;
		strContent.Format("Fail(H):%s--%s",hExcept.ProcName().Text(),hExcept.ErrorMessage().Text());
		//AfxMessageBox(strContent);
		::MessageBoxA(m_hWnd,strContent,"""",MB_OK);
	}
	catch (...)
	{
		CString strContent;
		strContent.Format("Fail(H):Abnormal");
		//AfxMessageBox(strContent);
		::MessageBoxA(m_hWnd,strContent,"""",MB_OK);
	}

}

// 保存
void CPropDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	// 这里需要添加代码
	if (m_bFlagHorVer||m_bFlagInRegion||m_bFlagOutRange||m_bFlagQrloc)
	{
		m_dlgSpacialSet->GetTempCheckParam();
		m_sCheckPrintParam = m_dlgSpacialSet->m_sCheckPrintParam;
		m_pTempChecker->m_sCheckPrintParam = m_sCheckPrintParam;	    
	}
	//m_pTempChecker->m_sCheckPrintParam.sZouBanPara.bLRChangeZouban = FALSE;
	//m_pTempChecker->m_sCheckPrintParam.sZouBanPara.bUDChangeZouban = FALSE;
	m_pTempChecker->WritePara(m_pTempChecker->m_strParaIni);
	// 写产品设置类型配置文件 [11/24/2011 Denghl]
	m_pTempChecker->WriteTypePara();
	m_pChecker->m_bCheckTimeOut = m_sCheckPrintParam.sTimeOut.bValid;
	m_pChecker->m_nAllTime = m_sCheckPrintParam.sTimeOut.nAllTime;
	m_pChecker->WriteTimePara(m_pChecker->m_strPathini);

	OnOK();
}

// 合适缩放显示
void CPropDlg::OnRadioAuto() // 合适时，全屏缩放显示
{
	// TODO: Add your control notification handler code here
	m_bIsMouseWheel = FALSE;
	m_nMoveXlast = 0;
	m_nMoveYlast = 0;
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

// 原始显示
void CPropDlg::OnRadioNorm() 
{
	// TODO: Add your control notification handler code here
	m_bIsMouseWheel = FALSE;
	m_nMoveXlast = 0;
	m_nMoveYlast = 0;
	m_fScaleX = (float)1.0;
	m_fScaleY = (float)1.0;
	UpdateData(FALSE);

	ClearWindow(m_HWindowID);
	SetPart(m_HWindowID, 0, 0, 
		(m_nDlgHeight - 1)/m_fScaleY, 
		(m_nDlgWidth - 1)/m_fScaleX);

	CRect recttmp;
	GetDlgItemRect(IDC_PIC,this,&recttmp);
	InvalidateRect(recttmp,TRUE);// 清屏
	//DrawImage();
}

// 设定比率显示
void CPropDlg::OnRadioSetrate() 
{
	// TODO: Add your control notification handler code here
    m_bIsMouseWheel = TRUE;
}

// 水平滚动条响应
void CPropDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
void CPropDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	/*int nCurPos;
	int nID = pScrollBar->GetDlgCtrlID ();
	CString str;
	if (nID==IDC_SCROLLBAR_VER)
	{
	switch( nSBCode )
	{
	case SB_THUMBPOSITION:
	pScrollBar->SetScrollPos( nPos );
	break;
	case SB_LINERIGHT:
	nCurPos = pScrollBar->GetScrollPos();
	nCurPos++;
	if( nCurPos > m_nRangV )
	nCurPos = m_nRangV;
	pScrollBar->SetScrollPos( nCurPos );
	break;
	case SB_LINELEFT:
	nCurPos = pScrollBar->GetScrollPos();
	nCurPos--;
	if( nCurPos < 0 )
	nCurPos = 0;
	pScrollBar->SetScrollPos( nCurPos );
	break;
	case SB_PAGERIGHT:
	nCurPos = pScrollBar->GetScrollPos();
	nCurPos += 10;
	if( nCurPos > m_nRangV )
	nCurPos = m_nRangV;
	pScrollBar->SetScrollPos( nCurPos );
	break;
	case SB_PAGELEFT:
	nCurPos = pScrollBar->GetScrollPos();
	nCurPos -= 10;
	if( nCurPos < 0 )
	nCurPos = 0;
	pScrollBar->SetScrollPos( nCurPos );
	break;
	case SB_THUMBTRACK:
	pScrollBar->SetScrollPos( nPos );
	break;
	}
	nCurPos = pScrollBar->GetScrollPos();
	m_nCurPosy = m_nRangV - nCurPos;
	InvalidateRect(&m_rectImg,TRUE);
	}*/
	CBCGPDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

// 画图函数
void CPropDlg::DrawImage()
{
	//CDC *pDC = GetDlgItem(IDC_PIC)->GetDC();
	//
	//if (m_nShowImgType==0)
	//{
	//	ShowImage(pDC,m_pImageSrc,0,0,m_lWindowWidth,m_lWindowHeight,0,0,m_nImgWid,m_nImgHei,m_nImgWid,m_nImgHei,m_nImageBit);
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
	//ClearWindow(m_HWindowID);
	HObject hTempImg;
	HObject hShowReg;

	if(m_pTempChecker->m_nChannel!=0)
	{
		HObject hImgR, hImgG, hImgB;
		Decompose3(m_hImgSrc, &hImgR, &hImgG, &hImgB);
		if(m_pTempChecker->m_nChannel==1)
		{
			hTempImg = hImgR;
		}
		else if(m_pTempChecker->m_nChannel==2)
		{
			hTempImg = hImgG;
		}
		else if (m_pTempChecker->m_nChannel==3)
		{
			hTempImg = hImgB;
		}
		else if (m_pTempChecker->m_nChannel==4)
		{
			//HObject hMinImg;
			//MinImage(hImgR,hImgG,&hMinImg);
			//MinImage(hMinImg,hImgB,&hTempImg);
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
	//SetPart(m_HWindowID, 0, 0, m_nImgHei-1,WIDTHBYTES(m_nImgWid*8)-1);	
	MirrorImage(hTempImg,&hTempImg,"row");
	DispObj(hTempImg,m_HWindowID);
	SetColor(m_HWindowID, "orange red");

	if (m_pTempChecker->m_sCheckPrintParam.sZouBanPara.bValid)
	{
		//if(!m_pTempChecker->m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan)
		//{
			//MirrorRegion(m_pTempChecker->m_hZoubanRegion,&hShowReg,"row",m_nImgHei);
			//DispObj(hShowReg,m_HWindowID);
		//}
		/*if (m_pTempChecker->m_sCheckPrintParam.sZouBanPara.bIsInLoc)
		{
			SetColor(m_HWindowID, "red");
			MirrorRegion(m_pTempChecker->m_hInlocMarkRegion,&hShowReg,"row",m_nImgHei);
			DispObj(hShowReg,m_HWindowID);
		}*/
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
int CPropDlg::ShowImage(CDC * pDC,unsigned char *lpDest, int nDispx, int nDispy, int nDispWidth, int nDispHeight,
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
BOOL CPropDlg::TransAlgImageToBuff(s_AlgImageInfo sAlgImageInfo,char *pDestImg,CRect rcCut)
{
	int i,j;
	int nDestWidth = WIDTHBYTES(rcCut.Width()*8)*sAlgImageInfo.nBits/8;
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
					pDestImg[(j-rcCut.top)*nDestWidth+(i-rcCut.left)*sAlgImageInfo.nBits/8] = sAlgImageInfo.pImageB[j*WIDTHBYTES(sAlgImageInfo.sizeImage.cx*8)+i];
					pDestImg[(j-rcCut.top)*nDestWidth+(i-rcCut.left)*sAlgImageInfo.nBits/8+1] = sAlgImageInfo.pImageG[j*WIDTHBYTES(sAlgImageInfo.sizeImage.cx*8)+i];
					pDestImg[(j-rcCut.top)*nDestWidth+(i-rcCut.left)*sAlgImageInfo.nBits/8+2] = sAlgImageInfo.pImageR[j*WIDTHBYTES(sAlgImageInfo.sizeImage.cx*8)+i];
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
void  CPropDlg::GetDlgItemRect(int nID,CWnd *wndParent,LPRECT Client)
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

// 对话框回车键不退出
BOOL CPropDlg::PreTranslateMessage(MSG* pMsg) 
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
					m_nMoveXlast=(m_nMoveXlast-m_nMoveX);
					m_nMoveYlast=(m_nMoveYlast-m_nMoveY);
				}
				m_bIsLButtonDown=FALSE;
			}
		}
		if(pMsg->message==WM_MOUSEMOVE && m_bIsLButtonDown==TRUE)
		{
			//GetDlgItem(IDC_PIC)->GetFocus();
			GetCursorPos(&ptMove);
			ptMove.x = ptMove.x-Rect.left;
			ptMove.y = ptMove.y-Rect.top;

			pPICWnd = GetDlgItem(IDC_PIC);	
			pPICWnd->GetClientRect(&rcPIC);

			if(rcPIC.PtInRect(ptMove))
			{
				m_nMoveX = ptMove.x-ptOri.x;
				m_nMoveY = ptMove.y-ptOri.y;
				ClearWindow(m_HWindowID);
				SetPart(m_HWindowID, (m_nMoveYlast-m_nMoveY)/m_fScaleY, (m_nMoveXlast-m_nMoveX)/m_fScaleX, 
					MAX(0,(m_nDlgHeight - 1 + m_nMoveYlast- m_nMoveY))/m_fScaleY, 
					MAX(0,(m_nDlgWidth -1 + m_nMoveXlast- m_nMoveX))/m_fScaleX);
				DrawImage();
			}	
		}
	}
	if (pMsg->message == WM_LBUTTONUP)
	{
		SetMshape (m_HWindowID, "arrow");
		m_bIsLButtonDown = FALSE;	
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

//void CPropDlg::OnCheckRotate() 
//{
//	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
//
//}

void CPropDlg::OnButtonReaddefault() 
{
	// TODO: Add your control notification handler code here
	char strTemp[100];
	memset(strTemp,0,100);
	GetPrivateProfileString("超时控制", "是否控制超时","0", strTemp, sizeof(strTemp), m_pChecker->m_strDefaultParaIni);
	m_pChecker->m_bCheckTimeOut = atoi(strTemp);
	GetPrivateProfileString("超时控制", "大张检测时间","600", strTemp, sizeof(strTemp), m_pChecker->m_strDefaultParaIni);
	m_pChecker->m_nAllTime = atoi(strTemp);

	m_pTempChecker->FreeCheckMethod();
	m_pTempChecker->ReadPara(m_pTempChecker->m_strDefaultParaIni);
	m_pTempChecker->m_sCheckPrintParam.sTimeOut.bValid = m_pChecker->m_bCheckTimeOut;
	m_pTempChecker->m_sCheckPrintParam.sTimeOut.nAllTime = m_pChecker->m_nAllTime;

	// 显示 [10/12/2013 Denghl]
	m_sCheckPrintParam = m_pTempChecker->m_sCheckPrintParam;

	// 请填写参数，将m_pTempChecker中的参数放到界面上，并且界面参数修改时，注意更新m_pTempChecker中的参数 [8/16/2010 dhl]

	m_comCodeType.SetCurSel(m_pTempChecker->m_iCheckMethod);
	m_fScaleHor = m_pTempChecker->m_fScaleHor;
	m_fScaleVer = m_pTempChecker->m_fScaleVer;

	m_bCheckAllMod = m_pTempChecker->m_bCheckAllMod;
	m_bCheckGao    = m_pTempChecker->m_bCheckGao;

	m_bCheckPartCode  = m_pTempChecker->m_bCheckPartCode;
	m_CodeCharL       = m_pTempChecker->m_CodeCharL;
	m_CodeCharH       = m_pTempChecker->m_CodeCharH;
	m_bIsTurnedResult = m_pTempChecker->m_bIsTurnedResult;


	if (m_sAlgImageInfo.nBits==24)
	{
		m_comChannel.AddString("R");
		m_comChannel.AddString("G");
		m_comChannel.AddString("B");
		m_comChannel.AddString("RGB");
		m_comChannel.SetCurSel(m_pTempChecker->m_nChannel);
	}
	else
	{
		GetDlgItem(IDC_COMBO_CHANNEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CHAN)->ShowWindow(SW_HIDE);

	}
	if (m_pTempChecker->m_iCheckMethod==2||m_pTempChecker->m_iCheckMethod==3)
	{
		m_bFlagQrloc = TRUE;
	}
	else
	{
		m_bFlagQrloc = FALSE;
	}
	if (m_pTempChecker->m_iCheckMethod==1)
	{
		m_bFlagHorVer = FALSE;
	}
	else
	{
		m_bFlagHorVer = TRUE;
	}
	if (m_bFlagInRegion||m_bFlagQrloc||m_bFlagOutRange||m_bFlagHorVer)
	{
		//m_dlgSpacialSet->m_bFlagInRegion = m_bFlagInRegion;
		m_dlgSpacialSet->m_bFlagQrloc    = m_bFlagQrloc;
		//m_dlgSpacialSet->m_bFlagOutRange = m_bFlagOutRange;
		//m_dlgSpacialSet->m_bFlagHorVer   = m_bFlagHorVer;		
		m_dlgSpacialSet->m_sCheckPrintParam = m_sCheckPrintParam;
		m_dlgSpacialSet->m_fScaleVer = m_fScaleVer;
		m_dlgSpacialSet->m_fScaleHor = m_fScaleHor;
		m_dlgSpacialSet->UpdateTempCheckParam();
	}
	UpdateData(FALSE);

}

void CPropDlg::OnButtonWritedefault() 
{
	// TODO: Add your control notification handler code here
	if (m_bFlagHorVer||m_bFlagInRegion||m_bFlagOutRange||m_bFlagQrloc)
	{
		m_dlgSpacialSet->GetTempCheckParam();
		m_sCheckPrintParam = m_dlgSpacialSet->m_sCheckPrintParam;
		m_pTempChecker->m_sCheckPrintParam = m_sCheckPrintParam;	    
	}
	m_pChecker->m_bCheckTimeOut = m_sCheckPrintParam.sTimeOut.bValid;
	m_pChecker->m_nAllTime      = m_sCheckPrintParam.sTimeOut.nAllTime;
	m_pChecker->WriteTimePara(m_pChecker->m_strDefaultParaIni);

	m_pTempChecker->WritePara(m_pTempChecker->m_strDefaultParaIni);
	// 写产品设置类型配置文件 [11/24/2011 Denghl]
	m_pTempChecker->WriteTypePara();
}

//void CPropDlg::OnCbnSelchangeComboProductType()
//{
//	// TODO: Add your control notification handler code here
//	//int nSel = m_comProductType.GetCurSel();
//	//if (nSel<0||nSel>=m_pTempChecker->m_nProductNum)
//	//{
//	//	return;
//
//	//}
//	//m_pTempChecker->m_strProductType = m_pTempChecker->m_vcstrAllProType.at(nSel);
//	//
//}

void CPropDlg::OnBnClickedButtonSettype()
{
	// TODO: Add your control notification handler code here
	//	m_pTempChecker->CodeSetNoteInfo();
	//	FormatProductTypeList();
}

void CPropDlg::OnBnClickedButtonParaSet()
{
	// TODO: Add your control notification handler code here
	//	m_pTempChecker->CodeCallParamDlg();
}

//void CPropDlg::OnEnChangeEditWord()
//{
//	// TODO:  If this is a RICHEDIT control, the control will not
//	// send this notification unless you override the CDialog::OnInitDialog()
//	// function and call CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//	UpdateData(TRUE);
//
//	// TODO:  Add your control notification handler code here
//}

void CPropDlg::OnCbnSelchangeComboCodetype()
{
	// TODO: Add your control notification handler code here
	int nSel = m_comCodeType.GetCurSel();
	if (nSel<0||nSel>3)
	{
		return;
	}
	if (m_pTempChecker->m_iCheckMethod !=nSel)
	{
		m_pTempChecker->FreeCheckMethod();
		m_pTempChecker->m_iCheckMethod = nSel;
		m_pTempChecker->MallocCheckMethod();		
	}
	//  [10/11/2016 zb]
	if (m_bCheckDengji)
	{
		if (m_pTempChecker->m_iCheckMethod==1||m_pTempChecker->m_iCheckMethod==3)//pdf417
		{
			m_comGBType.SetCurSel(0);
			m_pTempChecker->m_sCheckPrintParam.sDengjiPara.nGBID = 0;
			GetDlgItem(IDC_COMBO_GBTYPE2)->EnableWindow(FALSE);		
		}
		else
		{
			GetDlgItem(IDC_COMBO_GBTYPE2)->EnableWindow(TRUE);	
		}
	}
	else
	{
		GetDlgItem(IDC_COMBO_DENGJI2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_GBTYPE2)->EnableWindow(FALSE);
	}
	//if (m_pTempChecker->m_iCheckMethod==1||m_pTempChecker->m_iCheckMethod==3)//pdf417
	//{
	//	m_comGBType.SetCurSel(0);
	//	m_pTempChecker->m_sCheckPrintParam.sDengjiPara.nGBID = 0;
	//	GetDlgItem(IDC_COMBO_GBTYPE2)->EnableWindow(FALSE);		
	//}
	//else
	//{
	//	GetDlgItem(IDC_COMBO_GBTYPE2)->EnableWindow(TRUE);	
	//}
	if (m_pTempChecker->m_iCheckMethod==1)
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

void CPropDlg::OnCbnSelchangeComboChannel()
{
	// TODO: Add your control notification handler code here
	int nSel = m_comChannel.GetCurSel();
	if (nSel<0||nSel>4)
	{
		return;
	}
	m_pTempChecker->m_nChannel = nSel;
	bOcrDeal = FALSE;
	CRect recttmp;
	GetDlgItemRect(IDC_PIC,this,&recttmp);
	InvalidateRect(recttmp,TRUE);// 清屏
}
void CPropDlg::OnCbnSelchangeComboPolar()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_comPolar.GetCurSel();
	if (nSel<0||nSel>1)
	{
		return;
	}

	//  [4/6/2017 zb]

	if (nSel==1)
	{	
		m_strPolar.Format("light_on_dark");
	} 
	else
	{
		m_strPolar.Format("dark_on_light");
	}

	m_pTempChecker->m_strPolar = m_strPolar;
}

void CPropDlg::OnBnClickedCheckAll()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_pTempChecker->m_bCheckAllMod = m_bCheckAllMod;
}

void CPropDlg::OnBnClickedCheckGao()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_pTempChecker->FreeCheckMethod();
	m_pTempChecker->m_bCheckGao = m_bCheckGao;
	m_pTempChecker->MallocCheckMethod();
}

void CPropDlg::OnEnChangeEditHorscal()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	m_dlgSpacialSet->m_fScaleHor = m_fScaleHor;
	m_dlgSpacialSet->m_dlgOutRange->m_fScaleHor = m_fScaleHor;
	m_dlgSpacialSet->m_dlgZouBan->m_fScaleHor = m_fScaleHor;
	//m_dlgSpacialSet->m_dlgOutRange->UpdateData(FALSE);
	//m_dlgSpacialSet->m_dlgZouBan->UpdateData(FALSE);
	m_pTempChecker->m_fScaleHor = m_fScaleHor;
	//UpdateData(FALSE);
}

void CPropDlg::OnEnChangeEditVerscal()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	m_dlgSpacialSet->m_fScaleVer = m_fScaleVer;
	m_dlgSpacialSet->m_dlgOutRange->m_fScaleVer = m_fScaleVer;
	m_dlgSpacialSet->m_dlgZouBan->m_fScaleVer = m_fScaleVer;
	//m_dlgSpacialSet->m_dlgOutRange->UpdateData(FALSE);
	//m_dlgSpacialSet->m_dlgZouBan->UpdateData(FALSE);
	m_pTempChecker->m_fScaleVer = m_fScaleVer;
	//UpdateData(FALSE);
	
	//纵向分辨率变化时，同步更新错位的像素数，且暂时采用纵向的分辨率计算错位的像素数
	m_dlgSpacialSet->m_sCheckPrintParam.sCorrPixelParam.nCorrPixels = (int)(m_dCorrPixel/m_fScaleVer+0.5);
}

void CPropDlg::OnEnKillfocusEditHorscal()
{
	// TODO: Add your control notification handler code here
	
}

void CPropDlg::OnEnKillfocusEditVerscal()
{
	// TODO: Add your control notification handler code here
	
}

void CPropDlg::OnBnClickedButtonSetdeng()
{
	// TODO: Add your control notification handler code here
	// 根据产品类型和标准设置，判断应该启用的标准对话框 [12/27/2013 Denghl]
	if (m_pTempChecker->m_iCheckMethod==0||m_pTempChecker->m_iCheckMethod==2)//ECC200和QR
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
			dlg.m_nLanguage			 = m_nLanguage;
			dlg.m_strDefaultParaPath = m_pTempChecker->m_strDefaultParaPath;
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
	else if (m_pTempChecker->m_iCheckMethod==1)
	{
		CDialogDengjiPdf417 dlg;
		dlg.m_bDJ1 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[0];
		dlg.m_bDJ2 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[1];
		dlg.m_bDJ3 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[2];
		dlg.m_bDJ4 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[3];
		dlg.m_bDJ5 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[4];
		dlg.m_bDJ6 = m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[5];
		//dlg.m_bDJ7 = m_pTempChecker->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[6];
		if (dlg.DoModal()==IDOK)
		{
			m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[0] = dlg.m_bDJ1;
			m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[1] = dlg.m_bDJ2;
			m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[2] = dlg.m_bDJ3;
			m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[3] = dlg.m_bDJ4;
			m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[4] = dlg.m_bDJ5;
			m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[5] = dlg.m_bDJ6;
			//m_pTempChecker->m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[6] = dlg.m_bDJ7;
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

void CPropDlg::OnCbnSelchangeComboDengji()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_comDengSet.GetCurSel();
	if (nIndex<0)
	{
		return;
	}
	m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.nSetGrade = nIndex;
}

void CPropDlg::OnBnClickedCheckDengji()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.bValid = m_bCheckDengji;
	// 等级模块显示 [12/27/2013 Denghl]
	if (m_bCheckDengji)
	{
		GetDlgItem(IDC_COMBO_DENGJI2)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_GBTYPE2)->EnableWindow(TRUE);
		//GetDlgItem(IDC_BUTTON_SETDENG2)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_DENGJI2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_GBTYPE2)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_SETDENG2)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);

}

void CPropDlg::OnCbnSelchangeComboGbtype()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_comGBType.GetCurSel();
	if (nIndex<0)
	{
		return;
	}
	m_dlgSpacialSet->m_sCheckPrintParam.sDengjiPara.nGBID = nIndex;
	m_pTempChecker->m_sCheckPrintParam.sDengjiPara.nGBID = nIndex;
}

void CPropDlg::OnBnClickedCheckCode()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bCheckPartCode)
	{
		if (m_pTempChecker->m_CheckResultCode.size()<=0)
		{
			m_bCheckPartCode = FALSE;
			UpdateData(FALSE);
			return ;
		}
		GetDlgItem(IDC_EDIT_CODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CODEH)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_CODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_CODEH)->EnableWindow(TRUE);
		OnEnChangeEditCodeh();
		//OnEnKillfocusEditResultnum();
	}
	else
	{
		GetDlgItem(IDC_EDIT_CODEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CODEH)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_CODEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_CODEH)->EnableWindow(FALSE);
	}
	m_pTempChecker->m_bCheckPartCode = m_bCheckPartCode;
}

void CPropDlg::OnEnChangeEditCodel()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	m_pTempChecker->m_CodeCharL = m_CodeCharL;
	OnEnKillfocusEditResultnum();
	UpdateData(FALSE);
}

void CPropDlg::OnEnChangeEditCodeh()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	//  [10/11/2016 zb]
	if (m_pTempChecker->m_CheckResultCode.size()>0&&m_CodeCharH>m_pTempChecker->m_CheckResultCode.size()-1)
	{
		m_CodeCharH = m_pTempChecker->m_CheckResultCode.size()-1;
	}
	m_pTempChecker->m_CodeCharH = m_CodeCharH;
	UpdateData(FALSE);
	OnEnKillfocusEditResultnum();
}

void CPropDlg::OnBnClickedCheckIsturnedresult()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_pTempChecker->m_bIsTurnedResult = m_bIsTurnedResult;

}
void CPropDlg::OnBnClickedCheckImagesmooth()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pTempChecker->m_bIsImageSmooth = m_bIsImageSmooth;
}
void CPropDlg::OnBnClickedCheckImagezoom()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pTempChecker->m_bIsZoomCheck = m_bIsZoomCheck;
}
void CPropDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void CPropDlg::OnBnClickedBtnAdvancepara()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bClickAdv)
	{
		SetWindowPos(NULL, 0, 0, m_rectDlg.Width(), m_rectDlg.Height(), SWP_NOMOVE|SWP_NOZORDER);
		if (m_nLanguage == 1)
		{
			GetDlgItem(IDC_BTN_ADVANCEPARA)->SetWindowText("Advance Param<<");
		} 
		else
		{
			GetDlgItem(IDC_BTN_ADVANCEPARA)->SetWindowText("高级<<");
		}

		m_bClickAdv = TRUE;
	}
	else if (m_bClickAdv)
	{		
		SetWindowPos(NULL, 0, 0, m_rectInitDlg.Width(), m_rectInitDlg.Height(), SWP_NOMOVE|SWP_NOZORDER);
		if (m_nLanguage == 1)
		{
			GetDlgItem(IDC_BTN_ADVANCEPARA)->SetWindowText("Advance Param>>");
		} 
		else
		{
			GetDlgItem(IDC_BTN_ADVANCEPARA)->SetWindowText("高级>>");
		}
		m_bClickAdv = FALSE;
	}
	this->CenterWindow();
}


//void CPropDlg::OnBnClickedCheckHorver()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//	m_dlgSpacialSet->m_sCheckPrintParam.sInHorverPara.bValid = m_bCheckHorVer;
//}


void CPropDlg::OnBnClickedCheckAligncorrect()
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
		GetDlgItem(IDC_SPIN_MAXPIXEL)->EnableWindow(FALSE);
	}
	m_dlgSpacialSet->m_sCheckPrintParam.sCorrPixelParam.bValid = m_bCheckAlignCor;
}


void CPropDlg::OnEnChangeEditCorrpixel()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//暂时采用纵向的分辨率计算错位的像素数
	m_dlgSpacialSet->m_sCheckPrintParam.sCorrPixelParam.nCorrPixels = (int)(m_dCorrPixel/m_fScaleVer+0.5);
}


void CPropDlg::OnDeltaposSpinMaxpixel(NMHDR *pNMHDR, LRESULT *pResult)
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


void CPropDlg::OnDeltaposSpinCodel(NMHDR *pNMHDR, LRESULT *pResult)
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
	m_pTempChecker->m_CodeCharL = m_CodeCharL;	
	UpdateData(FALSE);
	*pResult = 0;
}


void CPropDlg::OnDeltaposSpinCodeh(NMHDR *pNMHDR, LRESULT *pResult)
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
	m_pTempChecker->m_CodeCharH = m_CodeCharH;	
	UpdateData(FALSE);
	*pResult = 0;
}


void CPropDlg::OnDeltaposSpinHor(NMHDR *pNMHDR, LRESULT *pResult)
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
	m_dlgSpacialSet->m_fScaleHor = m_fScaleHor;
	m_dlgSpacialSet->m_dlgOutRange->m_fScaleHor = m_fScaleHor;
	m_dlgSpacialSet->m_dlgZouBan->m_fScaleHor = m_fScaleHor;
	m_dlgSpacialSet->m_dlgOutRange->UpdateData(FALSE);
	m_dlgSpacialSet->m_dlgZouBan->UpdateData(FALSE);
	m_pTempChecker->m_fScaleHor = m_fScaleHor;
	*pResult = 0;
}


void CPropDlg::OnDeltaposSpinVer(NMHDR *pNMHDR, LRESULT *pResult)
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
	m_dlgSpacialSet->m_fScaleVer = m_fScaleVer;
	m_dlgSpacialSet->m_dlgOutRange->m_fScaleVer = m_fScaleVer;
	m_dlgSpacialSet->m_dlgZouBan->m_fScaleVer = m_fScaleVer;
	m_dlgSpacialSet->m_dlgOutRange->UpdateData(FALSE);
	m_dlgSpacialSet->m_dlgZouBan->UpdateData(FALSE);
	m_pTempChecker->m_fScaleVer = m_fScaleVer;
	*pResult = 0;
}


void CPropDlg::OnStnClickedDrawLoc()
{
	// TODO: 在此添加控件通知处理程序代码
	bOcrDeal = FALSE;
	if (m_bFlagHorVer||m_bFlagInRegion||m_bFlagOutRange||m_bFlagQrloc)
	{
		m_dlgSpacialSet->GetTempCheckParam();
		m_sCheckPrintParam = m_dlgSpacialSet->m_sCheckPrintParam;
		m_pTempChecker->m_sCheckPrintParam = m_sCheckPrintParam;	    
	}
	DrawImage();
	//画内部定位区域
	if (m_sCheckPrintParam.sZouBanPara.bValid && m_sCheckPrintParam.sZouBanPara.bIsInLoc)
	{
		HObject hDrawRegion;
		HTuple hRow1,hCol1,hRow2,hCol2;
		GetDlgItem(IDC_DRAW_LOC)->EnableWindow(FALSE);
		SetColor(m_HWindowID, "yellow");
		DrawRectangle1(m_HWindowID,&hRow1,&hCol1,&hRow2,&hCol2);
		GenRectangle1(&hDrawRegion,hRow1,hCol1,hRow2,hCol2);
		MirrorRegion(hDrawRegion,&m_pTempChecker->m_hInlocMarkRegion,"row",m_nImgHei);
		GetDlgItem(IDC_DRAW_LOC)->EnableWindow(TRUE);
		HObject hImageSrc,hPartImg;
		//GenImage1Extern(&hImageSrc,"byte",WIDTHBYTES(m_nImgWid*8),m_nImgHei,(HTuple)m_pImageSrc,NULL);
		//ReduceDomain(hImageSrc,m_pTempChecker->m_hInlocMarkRegion,&hPartImg);
		ReduceDomain(m_hImgSrc,m_pTempChecker->m_hInlocMarkRegion,&hPartImg);
		if (m_pTempChecker->m_lMarkID>=0)
		{
			ClearNccModel(m_pTempChecker->m_lMarkID);
			m_pTempChecker->m_lMarkID = -1;
		}
		
		try
		{
			CreateNccModel(hPartImg,"auto",-0.39,0.79,"auto","use_polarity",&m_pTempChecker->m_lMarkID);
		}
		catch(...)
		{
			//AfxMessageBox("Failed to create internal positioning！");
			CString str;
			str.Format("%s",m_strMessageText[28]);
			::MessageBoxA(m_hWnd,str,"""",MB_OK);
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


void CPropDlg::OnStnClickedDrawOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	bOcrDeal = FALSE;
	
	if (m_bFlagHorVer||m_bFlagInRegion||m_bFlagOutRange||m_bFlagQrloc)
	{
		m_dlgSpacialSet->GetTempCheckParam();
		m_sCheckPrintParam = m_dlgSpacialSet->m_sCheckPrintParam;
		m_pTempChecker->m_sCheckPrintParam = m_sCheckPrintParam;	    
	}
	DrawImage();
	//画走版区域
	if (m_sCheckPrintParam.sZouBanPara.bValid)
	{	
		HObject hDrawRegion;
		HTuple hRow1,hCol1,hRow2,hCol2;
		GetDlgItem(IDC_DRAW_OFFSET)->EnableWindow(FALSE);
		SetColor(m_HWindowID, "orange red");
		DrawRectangle1(m_HWindowID,&hRow1,&hCol1,&hRow2,&hCol2);
		GenRectangle1(&hDrawRegion,hRow1,hCol1,hRow2,hCol2);
		MirrorRegion(hDrawRegion,&m_pTempChecker->m_hZoubanRegion,"row",m_nImgHei);
		GetDlgItem(IDC_DRAW_OFFSET)->EnableWindow(TRUE);
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


BOOL CPropDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bIsMouseWheel)
	{
		CRect Rect;
		CRect rcPIC;

		CPoint ptPIC = pt;
		CWnd*  pPICWnd = GetDlgItem(IDC_PIC);
		//CDC    *pDCPIC = pPICWnd->GetDC();
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
		//ReleaseDC(pDCPIC);
	}
	return CBCGPDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CPropDlg::OnEnKillfocusEditSfradio()
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
	m_pTempChecker->m_fZoomRadio = m_fZoomRadio;
	UpdateData(FALSE);
	// TODO: 在此添加控件通知处理程序代码
}


void CPropDlg::OnEnKillfocusEditResultnum()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//  [10/11/2016 zb] 局部识别时再次判断校验位数上限
	if (m_bCheckPartCode)
	{
		if (m_nResultNum>(m_CodeCharH-m_CodeCharL+1))
		{
			m_nResultNum = m_CodeCharH-m_CodeCharL+1;
		}
	} 
	m_pTempChecker->m_nResultNum = m_nResultNum;
	UpdateData(FALSE);
}
