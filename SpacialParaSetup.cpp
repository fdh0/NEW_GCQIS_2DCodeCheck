// SpacialParaSetup.cpp : implementation file
//

#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "SpacialParaSetup.h"

#include "ParaDlg.h"
#include "PropDlg.h"
// CSpacialParaSetup dialog

IMPLEMENT_DYNAMIC(CSpacialParaSetup, CBCGPDialog)

CSpacialParaSetup::CSpacialParaSetup(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CSpacialParaSetup::IDD, pParent)
{
	m_dlgInRegion = NULL;
	m_dlgQrloc = NULL;
	m_dlgOutRange = NULL;
	//m_dlgHorVer = NULL;	
	m_dlgZouBan = NULL;
	//m_dlgCorrPixel = NULL;
	m_dlgTimeOut = NULL;
	m_bChangeZoubanWhite = FALSE;

	//CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManagerVS2010));
	EnableVisualManagerStyle(TRUE,TRUE);
	m_nLanguage = 0;
	m_strDefaultParaPath = _T("");
	memset(m_strMessageText,0,512*256*sizeof(char));
}

CSpacialParaSetup::~CSpacialParaSetup()
{
}

void CSpacialParaSetup::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ADDTAB, m_labAddTab);
}


BEGIN_MESSAGE_MAP(CSpacialParaSetup, CBCGPDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSpacialParaSetup message handlers
void CSpacialParaSetup::GetTempCheckParam()
{
	if (m_dlgQrloc)
	{		
		m_sCheckPrintParam.sQrlocShapePara.bValid = m_dlgQrloc->m_bCheckQRloc;
	}
	if (m_dlgInRegion)
	{
		m_sCheckPrintParam.sInRegionPara.bValid     = m_dlgInRegion->m_bCheckInRegin;
		m_sCheckPrintParam.sInRegionPara.nDeviation = m_dlgInRegion->m_nDeviation;
		m_sCheckPrintParam.sInRegionPara.nMaxGray   = m_dlgInRegion->m_nMaxGray;
		//喷头阻塞
		m_sCheckPrintParam.sInHorverPara.bValid     = m_dlgInRegion->m_bCheckHorVer;
		m_sCheckPrintParam.sInHorverPara.nDirect    = m_dlgInRegion->m_nDirect;
		m_sCheckPrintParam.sInHorverPara.nArea      = (int)(m_dlgInRegion->m_dArea/(m_fScaleHor*m_fScaleVer)+0.5);
		m_sCheckPrintParam.sInHorverPara.nPercent   = m_dlgInRegion->m_nPercent;
		m_sCheckPrintParam.sInHorverPara.nContrast  = m_dlgInRegion->m_nContrast;
	}
	if (m_dlgOutRange)
	{
		m_sCheckPrintParam.sOutRangPara.bValid      = m_dlgOutRange->m_bCheckOutRange;
		m_sCheckPrintParam.sOutRangPara.nLeft       = (int)(m_dlgOutRange->m_dLeft/m_fScaleHor+0.5);
		m_sCheckPrintParam.sOutRangPara.nRight      = (int)(m_dlgOutRange->m_dRight/m_fScaleHor+0.5);
		m_sCheckPrintParam.sOutRangPara.nTop        = (int)(m_dlgOutRange->m_dTop/m_fScaleVer+0.5);
		m_sCheckPrintParam.sOutRangPara.nBottom     = (int)(m_dlgOutRange->m_dBottom/m_fScaleVer+0.5);
		m_sCheckPrintParam.sOutRangPara.nSingleArea = (int)(m_dlgOutRange->m_dSingleArea/(m_fScaleHor*m_fScaleVer)+0.5);
		m_sCheckPrintParam.sOutRangPara.nAllArea    = (int)(m_dlgOutRange->m_dAllArea/(m_fScaleHor*m_fScaleVer)+0.5);
	}
	if (m_dlgZouBan)
	{
		m_sCheckPrintParam.sZouBanPara.bValid   = m_dlgZouBan->m_bCheckZouban;
		m_sCheckPrintParam.sZouBanPara.bIsInLoc = m_dlgZouBan->m_bInLoc;
		m_sCheckPrintParam.sZouBanPara.nSearchX = (int)(m_dlgZouBan->m_dSearchX/m_fScaleHor+0.5);
		m_sCheckPrintParam.sZouBanPara.nSearchY = (int)(m_dlgZouBan->m_dSearchY/m_fScaleVer+0.5);
		//  [11/4/2015 zl]
		m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft=m_dlgZouBan->m_bIsCheckZouBanLeft;
		m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight=m_dlgZouBan->m_bIsCheckZouBanRight;
		m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp=m_dlgZouBan->m_bIsCheckZouBanUp;
		m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown=m_dlgZouBan->m_bIsCheckZouBanDown;

		m_sCheckPrintParam.sZouBanPara.m_nLeftPos=m_dlgZouBan->m_nleftPos;
		m_sCheckPrintParam.sZouBanPara.m_nRightPos=m_dlgZouBan->m_nRightPos;
		m_sCheckPrintParam.sZouBanPara.m_nUpPos=m_dlgZouBan->m_nUpPos;
		m_sCheckPrintParam.sZouBanPara.m_nDownPos=m_dlgZouBan->m_nDownPos;

		m_sCheckPrintParam.sZouBanPara.m_fZouBanRange=m_dlgZouBan->m_fZouBanRange;
		m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan=m_dlgZouBan->m_bIsWhiteZouBan;

		m_sCheckPrintParam.sZouBanPara.m_fZouBanStdHor=m_dlgZouBan->m_fZouBanStdHor;
		m_sCheckPrintParam.sZouBanPara.m_fZouBanStdVer=m_dlgZouBan->m_fZouBanStdVer;
		if (m_dlgZouBan->m_bChangeZouban)
		{
			m_sCheckPrintParam.sZouBanPara.bLRChangeZouban = m_dlgZouBan->m_bChangeZouban;
			m_sCheckPrintParam.sZouBanPara.bUDChangeZouban = m_dlgZouBan->m_bChangeZouban;
			if (m_dlgZouBan->m_bIsWhiteZouBan)
			{
				m_bChangeZoubanWhite = TRUE;
			}
			else
			{
				m_bChangeZoubanWhite = FALSE;
			}
		}
	}	
	if (m_dlgTimeOut)
	{
		m_sCheckPrintParam.sTimeOut.bValid      = m_dlgTimeOut->m_bCheckTime;
		m_sCheckPrintParam.sTimeOut.nSingleTime = m_dlgTimeOut->m_nSingleTime;
		m_sCheckPrintParam.sTimeOut.nAllTime    = m_dlgTimeOut->m_nAllTime;
	}

}
void CSpacialParaSetup::UpdateTempCheckParam()
{
	
	//缺印
	m_dlgInRegion->m_bCheckInRegin  = m_sCheckPrintParam.sInRegionPara.bValid;
	m_dlgInRegion->m_nDeviation     = m_sCheckPrintParam.sInRegionPara.nDeviation;
	m_dlgInRegion->m_nMaxGray       = m_sCheckPrintParam.sInRegionPara.nMaxGray;
	m_dlgInRegion->m_bCheckHorVer = m_sCheckPrintParam.sInHorverPara.bValid;
	m_dlgInRegion->m_dArea = m_sCheckPrintParam.sInHorverPara.nArea*(m_fScaleHor*m_fScaleVer)*100/100;
	m_dlgInRegion->m_nDirect = m_sCheckPrintParam.sInHorverPara.nDirect;
	m_dlgInRegion->m_nContrast = m_sCheckPrintParam.sInHorverPara.nContrast;
	m_dlgInRegion->m_nPercent = m_sCheckPrintParam.sInHorverPara.nPercent;

	m_dlgInRegion->UpdateData(FALSE);
	
	//外脏
	m_dlgOutRange->m_bCheckOutRange = m_sCheckPrintParam.sOutRangPara.bValid;	
	m_dlgOutRange->m_dLeft          = m_sCheckPrintParam.sOutRangPara.nLeft*m_fScaleHor;
	m_dlgOutRange->m_dRight         = m_sCheckPrintParam.sOutRangPara.nRight*m_fScaleHor;
	m_dlgOutRange->m_dTop           = m_sCheckPrintParam.sOutRangPara.nTop*m_fScaleVer;
	m_dlgOutRange->m_dBottom        = m_sCheckPrintParam.sOutRangPara.nBottom*m_fScaleVer;
	m_dlgOutRange->m_dSingleArea    = m_sCheckPrintParam.sOutRangPara.nSingleArea*(m_fScaleHor*m_fScaleVer)*100/100;
	m_dlgOutRange->m_dAllArea       = m_sCheckPrintParam.sOutRangPara.nAllArea*(m_fScaleHor*m_fScaleVer)*100/100;
	m_dlgOutRange->UpdateData(FALSE);

	//走版
	m_dlgZouBan->m_bCheckZouban = m_sCheckPrintParam.sZouBanPara.bValid;	
	m_dlgZouBan->m_bInLoc       = m_sCheckPrintParam.sZouBanPara.bIsInLoc;
	m_dlgZouBan->m_dSearchX     = m_sCheckPrintParam.sZouBanPara.nSearchX*m_fScaleHor;
	m_dlgZouBan->m_dSearchY     = m_sCheckPrintParam.sZouBanPara.nSearchY*m_fScaleVer;
	m_dlgZouBan->UpdateData(FALSE);

	//超时
	m_dlgTimeOut->m_bCheckTime  = m_sCheckPrintParam.sTimeOut.bValid;
	m_dlgTimeOut->m_nAllTime    = m_sCheckPrintParam.sTimeOut.nAllTime;
	m_dlgTimeOut->m_nSingleTime = m_sCheckPrintParam.sTimeOut.nSingleTime;
	m_dlgTimeOut->UpdateData(FALSE);

	if (m_bFlagQrloc)
	{
		m_dlgQrloc->m_bCheckQRloc = m_sCheckPrintParam.sQrlocShapePara.bValid;	
		m_dlgQrloc->UpdateData(FALSE);
	}
}
BOOL CSpacialParaSetup::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();	

	//Tab页
	CRect rectTab;
	m_labAddTab.GetWindowRect(rectTab);
	ScreenToClient(rectTab);

	m_wndTab.Create(CBCGPTabWnd::STYLE_3D_ONENOTE, rectTab, this, 1, CBCGPTabWnd::LOCATION_TOP);
	
	//内部缺印
	m_dlgInRegion = new CInRegion;
	m_dlgInRegion->m_bCheckInRegin   = m_sCheckPrintParam.sInRegionPara.bValid;
	m_dlgInRegion->m_nDeviation      = m_sCheckPrintParam.sInRegionPara.nDeviation;
	m_dlgInRegion->m_nMaxGray        = m_sCheckPrintParam.sInRegionPara.nMaxGray;
	m_dlgInRegion->m_bCheckHorVer    = m_sCheckPrintParam.sInHorverPara.bValid;
	m_dlgInRegion->m_dArea           = m_sCheckPrintParam.sInHorverPara.nArea*(m_fScaleHor*m_fScaleVer)*100/100;
	m_dlgInRegion->m_nDirect         = m_sCheckPrintParam.sInHorverPara.nDirect;
	m_dlgInRegion->m_nContrast       = m_sCheckPrintParam.sInHorverPara.nContrast;
	m_dlgInRegion->m_nPercent        = m_sCheckPrintParam.sInHorverPara.nPercent;
	m_dlgInRegion->m_bShowHorVer     = m_bFlagHorVer;
	m_dlgInRegion->m_nLanguage		 = m_nLanguage;
	m_dlgInRegion->m_strDefaultParaPath = m_strDefaultParaPath;
	m_dlgInRegion->Create(CInRegion::IDD,&m_wndTab);	

	//外部脏污
	m_dlgOutRange = new COutRange;
	m_dlgOutRange->m_bCheckOutRange = m_sCheckPrintParam.sOutRangPara.bValid;	
	m_dlgOutRange->m_dLeft          = (int)(m_sCheckPrintParam.sOutRangPara.nLeft*m_fScaleHor*100)/100.0;
	m_dlgOutRange->m_dRight         = (int)(m_sCheckPrintParam.sOutRangPara.nRight*m_fScaleHor*100)/100.0;
	m_dlgOutRange->m_dTop           = (int)(m_sCheckPrintParam.sOutRangPara.nTop*m_fScaleVer*100)/100.0;
	m_dlgOutRange->m_dBottom        = (int)(m_sCheckPrintParam.sOutRangPara.nBottom*m_fScaleVer*100)/100.0;
	m_dlgOutRange->m_dSingleArea    = (int)(m_sCheckPrintParam.sOutRangPara.nSingleArea*(m_fScaleHor*m_fScaleVer)*10000)/10000.0;
	m_dlgOutRange->m_dAllArea       = (int)(m_sCheckPrintParam.sOutRangPara.nAllArea*(m_fScaleHor*m_fScaleVer)*10000)/10000.0;
	m_dlgOutRange->m_fScaleHor      = m_fScaleHor;
	m_dlgOutRange->m_fScaleVer      = m_fScaleVer;
	m_dlgOutRange->m_nLanguage		= m_nLanguage;
	m_dlgOutRange->m_strDefaultParaPath = m_strDefaultParaPath;
	m_dlgOutRange->Create(COutRange::IDD,&m_wndTab);	
	
	//走版检测
	m_dlgZouBan = new CZouBanPro;
	m_dlgZouBan->m_bCheckZouban = m_sCheckPrintParam.sZouBanPara.bValid;
	m_dlgZouBan->m_bInLoc       = m_sCheckPrintParam.sZouBanPara.bIsInLoc;
	m_dlgZouBan->m_dSearchX     = m_sCheckPrintParam.sZouBanPara.nSearchX*m_fScaleHor;
	m_dlgZouBan->m_dSearchY     = m_sCheckPrintParam.sZouBanPara.nSearchY*m_fScaleVer;
	m_dlgZouBan->m_fScaleHor    = m_fScaleHor;
	m_dlgZouBan->m_fScaleVer    = m_fScaleVer;
	memcpy((void *)&m_dlgZouBan->m_strMessageText,(void *)&m_strMessageText,512*256*sizeof(TCHAR));
	//  [11/3/2015 zl]
	m_dlgZouBan->m_bIsCheckZouBanUp      = m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp;
	m_dlgZouBan->m_bIsCheckZouBanDown     = m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown;
	m_dlgZouBan->m_bIsCheckZouBanLeft     = m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft;
	m_dlgZouBan->m_bIsCheckZouBanRight     = m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight;

	m_dlgZouBan->m_fZouBanRange     = m_sCheckPrintParam.sZouBanPara.m_fZouBanRange;
	m_dlgZouBan->m_bIsWhiteZouBan   = m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan;

	m_dlgZouBan->m_fZouBanStdVer     = m_sCheckPrintParam.sZouBanPara.m_fZouBanStdVer;
	m_dlgZouBan->m_fZouBanStdHor     = m_sCheckPrintParam.sZouBanPara.m_fZouBanStdHor;

	m_dlgZouBan->m_nleftPos     = m_sCheckPrintParam.sZouBanPara.m_nLeftPos;
	m_dlgZouBan->m_nRightPos    = m_sCheckPrintParam.sZouBanPara.m_nRightPos;
	m_dlgZouBan->m_nUpPos     = m_sCheckPrintParam.sZouBanPara.m_nUpPos;
	m_dlgZouBan->m_nDownPos    = m_sCheckPrintParam.sZouBanPara.m_nDownPos;
	//  [11/4/2015 zl]
	m_dlgZouBan->m_hImgSrc=m_hImgSrc;
	m_dlgZouBan->m_pPropDlg=m_pPropDlg;
	m_dlgZouBan->m_pParaDlg=m_pParaDlg;
	m_dlgZouBan->nDlgSwitch=nDlgSwitch;

	m_dlgZouBan->m_nLanguage		= m_nLanguage;
	m_dlgZouBan->m_strDefaultParaPath = m_strDefaultParaPath;
	m_dlgZouBan->Create(CZouBanPro::IDD,&m_wndTab);	

	//检测超时设定
	m_dlgTimeOut = new CSetTimeout;
	m_dlgTimeOut->m_bCheckTime   = m_sCheckPrintParam.sTimeOut.bValid;	
	m_dlgTimeOut->m_nSingleTime  = m_sCheckPrintParam.sTimeOut.nSingleTime;
	m_dlgTimeOut->m_nAllTime     = m_sCheckPrintParam.sTimeOut.nAllTime;
	m_dlgTimeOut->m_nLanguage	 = m_nLanguage;
	m_dlgTimeOut->m_strDefaultParaPath = m_strDefaultParaPath;
	m_dlgTimeOut->Create(CSetTimeout::IDD,&m_wndTab);

	if (m_bFlagQrloc)
	{
		m_dlgQrloc = new CQRLoc;
		m_dlgQrloc->m_bCheckQRloc = m_sCheckPrintParam.sQrlocShapePara.bValid;	
		m_dlgQrloc->m_nLanguage	 = m_nLanguage;
		m_dlgQrloc->m_strDefaultParaPath = m_strDefaultParaPath;
		m_dlgQrloc->Create(CQRLoc::IDD,&m_wndTab);
		//m_ctrlTab.AddTab (m_dlgQrloc,"QR定位检测",1);		
	}

	//  [4/6/2017 zb]
	int i=0;
	if (m_nLanguage == 1)
	{
		
		
		
		m_wndTab.AddTab(m_dlgInRegion,_T("Lose"),i++);
		m_wndTab.AddTab(m_dlgOutRange,_T("Dirty"),i++);
		m_wndTab.AddTab(m_dlgZouBan,_T("ExCursion"),i++);
		m_wndTab.AddTab(m_dlgTimeOut,_T("Timeout"),i++);
		if (m_bFlagQrloc)
		{
			m_wndTab.AddTab(m_dlgQrloc,_T("QR Loc"),i++);			
		}
	} 
	else
	{
		
		m_wndTab.AddTab(m_dlgInRegion,_T("缺印"),i++);
		m_wndTab.AddTab(m_dlgOutRange,_T("外脏"),i++);
		m_wndTab.AddTab(m_dlgZouBan,_T("走版"),i++);
		m_wndTab.AddTab(m_dlgTimeOut,_T("超时"),i++);
		if (m_bFlagQrloc)
		{
			m_wndTab.AddTab(m_dlgQrloc,_T("QR定位"),i++);			
		}
	}
	

	m_wndTab.SetButtonsVisible(FALSE);
	m_wndTab.SetFlatFrame();
	
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSpacialParaSetup::OnDestroy()
{
	if (m_dlgQrloc)
	{
		m_dlgQrloc->DestroyWindow();
		delete m_dlgQrloc;
		m_dlgQrloc = NULL;
	}
	if (m_dlgInRegion)
	{
		m_dlgInRegion->DestroyWindow();
		delete m_dlgInRegion;
		m_dlgInRegion = NULL;
	}
	if (m_dlgOutRange)
	{
		m_dlgOutRange->DestroyWindow();
		delete m_dlgOutRange;
		m_dlgOutRange = NULL;
	}
	if (m_dlgZouBan)
	{
		m_dlgZouBan->DestroyWindow();
		delete m_dlgZouBan;
		m_dlgZouBan = NULL;
	}
	if (m_dlgTimeOut)
	{
		m_dlgTimeOut->DestroyWindow();
		delete m_dlgTimeOut;
		m_dlgTimeOut = NULL;
	}
	CBCGPDialog::OnDestroy();
}
