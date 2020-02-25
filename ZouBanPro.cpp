// ZouBanPro.cpp : implementation file
//

#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "ZouBanPro.h"
#include "ParaDlg.h"
#include "PropDlg.h"

// CZouBanPro dialog

IMPLEMENT_DYNAMIC(CZouBanPro, CBCGPDialog)

CZouBanPro::CZouBanPro(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CZouBanPro::IDD, pParent)
	, m_bCheckZouban(FALSE)
	, m_bInLoc(FALSE)
	, m_dSearchX(0)
	, m_dSearchY(0)
	, m_bIsCheckZouBanUp(FALSE)
	, m_bIsCheckZouBanDown(FALSE)
	, m_bIsCheckZouBanLeft(FALSE)
	, m_bIsCheckZouBanRight(FALSE)
	, m_fZouBanStdVer(0)
	, m_fZouBanStdHor(0)
	, m_fZouBanRange(0)
	, m_nZouBanSegThresh(0)
	, m_bIsWhiteZouBan(FALSE)
{
	m_fScaleHor = 0;
	m_fScaleVer = 0;
	m_nleftPos=0;
	m_nRightPos=0;
	m_nUpPos=0;
	m_nDownPos=0;
	m_hImgH=0;
	m_hImgW=0;
	GenEmptyRegion(&m_hoLReg);
	GenEmptyRegion(&m_hoRReg);
	GenEmptyRegion(&m_hoUReg);
	GenEmptyRegion(&m_hoDReg);
	GenEmptyRegion(&m_hRealZoubanRegionL);
	GenEmptyRegion(&m_hRealZoubanRegionR);
	GenEmptyRegion(&m_hRealZoubanRegionU);
	GenEmptyRegion(&m_hRealZoubanRegionD);
	m_bChangeZouban = FALSE;

	//BCG Control
	//CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS (CBCGPVisualManagerVS2010));
	EnableVisualManagerStyle(TRUE,TRUE);
	//  [4/6/2017 zb]
	m_nLanguage         = 0;
	m_strDefaultParaPath= _T("");
	m_strLanguagePath   = _T("");
	m_strControlMapPath = _T("");
	m_strMessage        = _T("");
	memset(m_cMessage,0,256);
	memset(m_strMessageText,0,512*256*sizeof(char));
}

CZouBanPro::~CZouBanPro()
{
	
}

void CZouBanPro::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ZOUBAN, m_bCheckZouban);
	DDX_Control(pDX, IDC_CHECK_ZOUBAN, m_cmdCheckZouban);
	DDX_Check(pDX, IDC_CHECK_INLOC, m_bInLoc);
	DDX_Text(pDX, IDC_EDIT_SPSOUS, m_dSearchX);
	DDV_MinMaxDouble(pDX, m_dSearchX, 0, 10);
	DDX_Text(pDX, IDC_EDIT_CZSOUS, m_dSearchY);
	DDV_MinMaxDouble(pDX, m_dSearchY, 0, 10);
	DDX_Control(pDX, IDC_CHECK_ISWHITEZOUBAN, m_cmdCheckWhite);
	DDX_Control(pDX, IDC_CHECK_INLOC, m_cmdCheckInLoc);
	DDX_Control(pDX, IDC_STATIC_SPSOUS, m_labSpsous);
	DDX_Control(pDX, IDC_STATIC_CZSOUS, m_labCzsous);
	DDX_Control(pDX, IDC_SLIDER_SPSOUS, m_slcSpsous);
	DDX_Control(pDX, IDC_SLIDER_CZSOUS, m_slcCzsous);
	DDX_Check(pDX, IDC_CHECK_ISWHITEZOUBAN, m_bIsWhiteZouBan);
	DDX_Check(pDX, IDC_RADIO_UP, m_bIsCheckZouBanUp);
	DDX_Control(pDX, IDC_RADIO_UP, m_cmdZBU);
	DDX_Check(pDX, IDC_RADIO_DOWN, m_bIsCheckZouBanDown);
	DDX_Control(pDX, IDC_RADIO_DOWN, m_cmdZBD);
	DDX_Check(pDX, IDC_RADIO_LEFT, m_bIsCheckZouBanLeft);
	DDX_Control(pDX, IDC_RADIO_LEFT, m_cmdZBL);
	DDX_Check(pDX, IDC_RADIO_RIGHT, m_bIsCheckZouBanRight);
	DDX_Control(pDX, IDC_RADIO_RIGHT, m_cmdZBR);
	DDX_Control(pDX, IDC_SLIDER_VERDIS, m_sliderVerDis);
	DDX_Control(pDX, IDC_SLIDER_HORDIS, m_sliderHorDis);
	DDX_Control(pDX, IDC_SLIDER_ZBRANGE, m_sliderZBRange);
	DDX_Text(pDX, IDC_EDIT_ZOUBANSTDVER, m_fZouBanStdVer);
	DDV_MinMaxDouble(pDX, m_fZouBanStdVer, 0, 10);
	DDX_Text(pDX, IDC_EDIT_ZOUBANSTDHOR, m_fZouBanStdHor);
	DDV_MinMaxDouble(pDX, m_fZouBanStdHor, 0, 10);
	DDX_Text(pDX, IDC_EDIT_ZOUBANRANGE, m_fZouBanRange);
	DDV_MinMaxDouble(pDX, m_fZouBanRange, 0, 10);
	DDX_Control(pDX, IDC_STATIC_VERDIS, m_labDisVer);
	DDX_Control(pDX, IDC_STATIC_HORDIS, m_labDisHor);
	DDX_Control(pDX, IDC_STATIC_RANGE, m_labRange);
}


BEGIN_MESSAGE_MAP(CZouBanPro, CBCGPDialog)
	ON_BN_CLICKED(IDC_CHECK_ZOUBAN, &CZouBanPro::OnBnClickedCheckZouban)
	ON_EN_KILLFOCUS(IDC_EDIT_SPSOUS, &CZouBanPro::OnEnKillfocusEditSpsous)
	ON_BN_CLICKED(IDC_CHECK_INLOC, &CZouBanPro::OnBnClickedCheckInloc)
	ON_EN_KILLFOCUS(IDC_EDIT_CZSOUS, &CZouBanPro::OnEnKillfocusEditCzsous)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPSOUS, &CZouBanPro::OnDeltaposSpinSpsous)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CZSOUS, &CZouBanPro::OnDeltaposSpinCzsous)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_SPSOUS, &CZouBanPro::OnEnChangeEditSpsous)
	ON_EN_CHANGE(IDC_EDIT_CZSOUS, &CZouBanPro::OnEnChangeEditCzsous)
	ON_BN_CLICKED(IDC_CHECK_ISWHITEZOUBAN, &CZouBanPro::OnBnClickedCheckIsWhiteZouBan)
	ON_BN_CLICKED(IDC_RADIO_LEFT, &CZouBanPro::OnBnClickedRadioLeft)
	ON_BN_CLICKED(IDC_RADIO_RIGHT, &CZouBanPro::OnBnClickedRadioRight)
	ON_BN_CLICKED(IDC_RADIO_UP, &CZouBanPro::OnBnClickedRadioUp)
	ON_BN_CLICKED(IDC_RADIO_DOWN, &CZouBanPro::OnBnClickedRadioDown)
	ON_EN_CHANGE(IDC_EDIT_ZOUBANSTDVER, &CZouBanPro::OnEnChangeEditZoubanstdver)
	ON_EN_CHANGE(IDC_EDIT_ZOUBANSTDHOR, &CZouBanPro::OnEnChangeEditZoubanstdhor)
	ON_EN_CHANGE(IDC_EDIT_ZOUBANRANGE, &CZouBanPro::OnEnChangeEditZoubanrange)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ZBHOR, &CZouBanPro::OnDeltaposSpinZbhor)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ZBVER, &CZouBanPro::OnDeltaposSpinZbver)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPSRANGE, &CZouBanPro::OnDeltaposSpinSpsrange)
END_MESSAGE_MAP()


// CZouBanPro message handlers

void CZouBanPro::OnBnClickedCheckZouban()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bCheckZouban)
	{
		GetDlgItem(IDC_CHECK_INLOC)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ISWHITEZOUBAN)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_UP)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DOWN)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ZOUBANSTDVER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ZOUBANSTDHOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_RANGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_VERDIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_HORDIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ZOUBANRANGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_ZBVER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_ZBHOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_SPSRANGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_VERDIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_HORDIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_ZBRANGE)->EnableWindow(TRUE);
		if(!m_bIsWhiteZouBan)
		{
			m_nleftPos=0;
			m_nRightPos=m_hImgW[0].D()-1;
			m_nUpPos=0;
			m_nDownPos=m_hImgH[0].D()-1;
		}
		if(nDlgSwitch==1)
		{
			m_pPropDlg->bOcrDeal=0;
			DispZBRange();
		}
		if(nDlgSwitch==2)
		{
			m_pParaDlg->bOcrDeal=0;
			DispZBRange();
		}
	}
	else
	{
		GetDlgItem(IDC_CHECK_INLOC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_ISWHITEZOUBAN)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ZOUBANSTDVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ZOUBANSTDHOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_RANGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_VERDIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_HORDIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ZOUBANRANGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_ZBVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_ZBHOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_SPSRANGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_VERDIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_HORDIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_ZBRANGE)->EnableWindow(FALSE);
		if(nDlgSwitch==1)
		{
			m_pPropDlg->bOcrDeal=0;
			m_pPropDlg->PostMessageA(WM_PAINT);
		}
		if(nDlgSwitch==2)
		{
			m_pParaDlg->bOcrDeal=0;
			m_pParaDlg->PostMessageA(WM_PAINT);
		}
	}
}

void CZouBanPro::OnEnKillfocusEditSpsous()
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
}
void CZouBanPro::OnEnKillfocusEditCzsous()
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
}

void CZouBanPro::OnBnClickedCheckInloc()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//if (m_bInLoc)
	//{
	//	GetDlgItem(IDC_EDIT_CZSOUS)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_SPSOUS)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_SPIN_CZSOUS)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_EDIT_SPSOUS)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_SLIDER_CZSOUS)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_SLIDER_SPSOUS)->EnableWindow(TRUE);
	//}
	//else
	//{
	//	GetDlgItem(IDC_EDIT_CZSOUS)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_SPSOUS)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_SPIN_CZSOUS)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_SPSOUS)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_SLIDER_CZSOUS)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_SLIDER_SPSOUS)->EnableWindow(FALSE);
	//}
}

BOOL CZouBanPro::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	//SetBackgroundImage(IDB_BITMAP_TABBACK);
	m_cmdCheckZouban.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckZouban.SetMouseCursorHand();

	m_cmdCheckWhite.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckWhite.SetMouseCursorHand();
	m_cmdZBL.SetTextColor(RGB (255, 255, 255));
	m_cmdZBL.SetMouseCursorHand();
	m_cmdZBR.SetTextColor(RGB (255, 255, 255));
	m_cmdZBR.SetMouseCursorHand();
	m_cmdZBU.SetTextColor(RGB (255, 255, 255));
	m_cmdZBU.SetMouseCursorHand();
	m_cmdZBD.SetTextColor(RGB (255, 255, 255));
	m_cmdZBD.SetMouseCursorHand();

	m_cmdCheckInLoc.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckInLoc.SetMouseCursorHand();
	m_labSpsous.m_clrText = RGB(218,165,0);
	m_labCzsous.m_clrText   = RGB(218,165,0);

	m_labDisHor.m_clrText = RGB(218,165,0);
	m_labDisVer.m_clrText = RGB(218,165,0);
	m_labRange.m_clrText = RGB(218,165,0);
	//  [4/7/2017 zb] 国际化配置，界面英文显示
	m_strLanguagePath.Format("%s\\International Config\\ControlEnglish.ini", m_strDefaultParaPath);
	m_strControlMapPath.Format("%s\\International Config\\ControlMap.ini", m_strDefaultParaPath);
	if (m_nLanguage == 1)//英文
	{
		int nNum = 0;
		nNum = GetPrivateProfileInt("ExCursion Parameters", "Total Control Number", 0, m_strLanguagePath);
		if(nNum > 0)
		{
			s_ControlSet *psControlSet = NULL;
			psControlSet = new s_ControlSet[nNum];
			for (int i=1;i<=nNum;i++)
			{
				char str[256];
				memset(str,0,256);
				CString strNo;
				strNo.Format("EControl NO %d",i);
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
	GetImageSize(m_hImgSrc,&m_hImgW,&m_hImgH);
	CheckRadioButton(IDC_RADIO_LEFT, //第一个参数为该组的第一个单选按钮的ID  		
		IDC_RADIO_RIGHT, //第二个参数为该组的最后一个单选按钮的ID  		
		IDC_RADIO_LEFT);
	CheckRadioButton(IDC_RADIO_UP, //第一个参数为该组的第一个单选按钮的ID  		
		IDC_RADIO_DOWN, //第二个参数为该组的最后一个单选按钮的ID  		
		IDC_RADIO_UP);

	if(m_bIsCheckZouBanLeft)
	{
		CheckRadioButton(IDC_RADIO_LEFT, //第一个参数为该组的第一个单选按钮的ID  		
			IDC_RADIO_RIGHT, //第二个参数为该组的最后一个单选按钮的ID  		
			IDC_RADIO_LEFT);
	}
	if(m_bIsCheckZouBanRight)
	{
		CheckRadioButton(IDC_RADIO_LEFT, //第一个参数为该组的第一个单选按钮的ID  		
			IDC_RADIO_RIGHT, //第二个参数为该组的最后一个单选按钮的ID  		
			IDC_RADIO_LEFT);
	}
	if(m_bIsCheckZouBanUp)
	{
		CheckRadioButton(IDC_RADIO_UP, //第一个参数为该组的第一个单选按钮的ID  		
			IDC_RADIO_DOWN, //第二个参数为该组的最后一个单选按钮的ID  		
			IDC_RADIO_UP);
	}
	if(m_bIsCheckZouBanDown)
	{
		CheckRadioButton(IDC_RADIO_UP, //第一个参数为该组的第一个单选按钮的ID  		
			IDC_RADIO_DOWN, //第二个参数为该组的最后一个单选按钮的ID  		
			IDC_RADIO_DOWN);
	}
	if (m_bCheckZouban)
	{
		//GetDlgItem(IDC_CHECK_INLOC)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ISWHITEZOUBAN)->EnableWindow(TRUE);
		if(!m_bIsWhiteZouBan)
		{
			m_nleftPos=0;
			m_nRightPos=m_hImgW[0].D()-1;
			m_nUpPos=0;
			m_nDownPos=m_hImgH[0].D()-1;
		}
	}
	else
	{
		GetDlgItem(IDC_CHECK_INLOC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_ISWHITEZOUBAN)->EnableWindow(FALSE);
	}
	//////////////////////////////////////////////////////////////
	if (0/*m_bInLoc*/)
	{
		GetDlgItem(IDC_EDIT_CZSOUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SPSOUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_CZSOUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SPSOUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_CZSOUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_SPSOUS)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CZSOUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SPSOUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_CZSOUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SPSOUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_CZSOUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_SPSOUS)->EnableWindow(FALSE);
	}
	m_slcSpsous.SetRange(0,100);
	m_slcSpsous.SetPos(m_dSearchX*10);
	m_slcCzsous.SetRange(0,100);
	m_slcCzsous.SetPos(m_dSearchY*10);
	//  [11/5/2015 zl]
	m_sliderHorDis.SetRange(0,100);
	m_sliderHorDis.SetPos(m_fZouBanStdHor*10);
	m_sliderVerDis.SetRange(0,100);
	m_sliderVerDis.SetPos(m_fZouBanStdVer*10);
	m_sliderZBRange.SetRange(0,100);
	m_sliderZBRange.SetPos(m_fZouBanRange*10);

	//  [11/6/2015 TIAN]
	if (nDlgSwitch == 1)  //属性对话框
	{
		DispZBRange();
		m_pPropDlg->PostMessageA(WM_PAINT);
	}
	if (nDlgSwitch == 2)  //属性对话框
	{
		DispZBRange();
		m_pParaDlg->PostMessageA(WM_PAINT);
	}

	if(m_bCheckZouban)
	{
		//使能白框提取控件
		GetDlgItem(IDC_RADIO_UP)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DOWN)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ZOUBANSTDVER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ZOUBANSTDHOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_RANGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_VERDIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_HORDIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ZOUBANRANGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_ZBVER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_ZBHOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_SPSRANGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_VERDIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_HORDIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_ZBRANGE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ZOUBANSTDVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ZOUBANSTDHOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_RANGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_VERDIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_HORDIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ZOUBANRANGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_ZBVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_ZBHOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_SPSRANGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_VERDIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_HORDIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_ZBRANGE)->EnableWindow(FALSE);
	}
	//if (nDlgSwitch == 2)  //参数对话框
	//{
	//	GetDlgItem(IDC_CHECK_ISWHITEZOUBAN)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_RADIO_UP)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_RADIO_DOWN)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(FALSE);
	//}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CZouBanPro::OnDeltaposSpinSpsous(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_dSearchX<=9.99)
		{
			m_dSearchX += 0.01;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_dSearchX >= 0.01)
		{
			m_dSearchX -= 0.01;
		}	
	}
	m_slcSpsous.SetPos(m_dSearchX*10);
	UpdateData(FALSE);
	*pResult = 0;
}


void CZouBanPro::OnDeltaposSpinCzsous(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_dSearchY<=9.99)
		{
			m_dSearchY += 0.01;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_dSearchY >= 0.01)
		{
			m_dSearchY -= 0.01;
		}	
	}
	m_slcCzsous.SetPos(m_dSearchY*10);
	UpdateData(FALSE);
	*pResult = 0;
}


void CZouBanPro::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_SPSOUS) 
	{ 
		m_dSearchX = m_slcSpsous.GetPos()*0.1;
		DispZBRange();
		UpdateData(FALSE);
	}
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_CZSOUS) 
	{ 
		m_dSearchY = m_slcCzsous.GetPos()*0.1;
		DispZBRange();
		UpdateData(FALSE);
	}
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_HORDIS) 
	{ 
		m_fZouBanStdHor = m_sliderHorDis.GetPos()*0.1;
		DispZBRange();
		UpdateData(FALSE);
	}
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_VERDIS) 
	{ 
		m_fZouBanStdVer = m_sliderVerDis.GetPos()*0.1;
		DispZBRange();
		UpdateData(FALSE);
	}
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_ZBRANGE) 
	{ 
		m_fZouBanRange = m_sliderZBRange.GetPos()*0.1;
		DispZBRange();
		UpdateData(FALSE);
	}
	if(m_bIsCheckZouBanLeft)
	{
		CheckRadioButton(IDC_RADIO_LEFT, //第一个参数为该组的第一个单选按钮的ID  		
			IDC_RADIO_RIGHT, //第二个参数为该组的最后一个单选按钮的ID  		
			IDC_RADIO_LEFT);
	}
	if(m_bIsCheckZouBanRight)
	{
		CheckRadioButton(IDC_RADIO_LEFT, //第一个参数为该组的第一个单选按钮的ID  		
			IDC_RADIO_RIGHT, //第二个参数为该组的最后一个单选按钮的ID  		
			IDC_RADIO_RIGHT);
	}
	if(m_bIsCheckZouBanUp)
	{
		CheckRadioButton(IDC_RADIO_UP, //第一个参数为该组的第一个单选按钮的ID  		
			IDC_RADIO_DOWN ,//第二个参数为该组的最后一个单选按钮的ID  		
			IDC_RADIO_UP);
	}
	if(m_bIsCheckZouBanDown)
	{
		CheckRadioButton(IDC_RADIO_UP, //第一个参数为该组的第一个单选按钮的ID  		
			IDC_RADIO_DOWN ,//第二个参数为该组的最后一个单选按钮的ID  		
			IDC_RADIO_DOWN);
	}
	UpdateData(FALSE);
	CBCGPDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CZouBanPro::OnEnChangeEditSpsous()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	if(m_dSearchX<0)
	{
		m_dSearchX = 0;
	}
	if(m_dSearchX>10)
	{
		m_dSearchX = 10;
	}
	UpdateData(FALSE);
	m_slcSpsous.SetPos(m_dSearchX*10);
	// TODO:  在此添加控件通知处理程序代码
}


void CZouBanPro::OnEnChangeEditCzsous()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	if(m_dSearchY<0)
	{
		m_dSearchY = 0;
	}
	if(m_dSearchY>10)
	{
		m_dSearchY = 10;
	}
	UpdateData(FALSE);
	m_slcCzsous.SetPos(m_dSearchY*10);
	// TODO:  在此添加控件通知处理程序代码
}

void CZouBanPro::OnBnClickedCheckIsWhiteZouBan()
{
	UpdateData(TRUE);
	m_bChangeZouban = TRUE;
	if(m_bIsWhiteZouBan)
	{
		//使能白框提取控件
		//GetDlgItem(IDC_RADIO_UP)->EnableWindow(TRUE);
		//GetDlgItem(IDC_RADIO_DOWN)->EnableWindow(TRUE);
		//GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(TRUE);
		//GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(TRUE);
		//GetDlgItem(IDC_EDIT_ZOUBANSTDVER)->EnableWindow(TRUE);
		//GetDlgItem(IDC_EDIT_ZOUBANSTDHOR)->EnableWindow(TRUE);
		//GetDlgItem(IDC_STATIC_RANGE)->EnableWindow(TRUE);
		//GetDlgItem(IDC_STATIC_VERDIS)->EnableWindow(TRUE);
		//GetDlgItem(IDC_STATIC_HORDIS)->EnableWindow(TRUE);
		//GetDlgItem(IDC_EDIT_ZOUBANRANGE)->EnableWindow(TRUE);
		//GetDlgItem(IDC_SPIN_ZBVER)->EnableWindow(TRUE);
		//GetDlgItem(IDC_SPIN_ZBHOR)->EnableWindow(TRUE);
		//GetDlgItem(IDC_SPIN_SPSRANGE)->EnableWindow(TRUE);
		//GetDlgItem(IDC_SLIDER_VERDIS)->EnableWindow(TRUE);
		//GetDlgItem(IDC_SLIDER_HORDIS)->EnableWindow(TRUE);
		//GetDlgItem(IDC_SLIDER_ZBRANGE)->EnableWindow(TRUE);
		
		UpdateData(FALSE);

		CheckRadioButton(IDC_RADIO_LEFT, //第一个参数为该组的第一个单选按钮的ID  		
			IDC_RADIO_RIGHT, //第二个参数为该组的最后一个单选按钮的ID  		
			IDC_RADIO_LEFT);
		CheckRadioButton(IDC_RADIO_UP, //第一个参数为该组的第一个单选按钮的ID  		
			IDC_RADIO_DOWN ,//第二个参数为该组的最后一个单选按钮的ID  		
			IDC_RADIO_DOWN);

		//if (nDlgSwitch == 1)  //属性对话框
		//{
			//自动提取边界
			HObject SymbolXLDs,hoRectengle;
			HTuple  Width,Height,DataCodeHandle,ResultHandles,DecodedDataStrings;
			HTuple  Row11 = 0 , Column11 = 0, Row21 = 0, Column21 = 0,htNum = 0;
			GenEmptyObj(&SymbolXLDs);
			CreateDataCode2dModel ("QR Code", HTuple(), HTuple(), &DataCodeHandle);
			GetImageSize (m_hImgSrc, &Width, &Height);
			FindDataCode2d( m_hImgSrc, &SymbolXLDs, DataCodeHandle, "stop_after_result_num", 1, 
				&ResultHandles, &DecodedDataStrings);
			HTuple lNum = 0;
			CountObj(SymbolXLDs,&lNum);
			if (lNum[0].I()>0)
			{
				SmallestRectangle1Xld (SymbolXLDs, &Row11, &Column11, &Row21, &Column21);
			}
			//////////////////////////////////////////////////////////////////////////
			HObject ho_RegTh,ho_ConnReg,ho_SelectedReg;
			HTuple  ht_Threshold,Row12, Column12, Row22, Column22,htMean, htDeviation;
			HTuple   lArea = 0;
			ht_Threshold = m_nZouBanSegThresh;
			GenEmptyRegion(&ho_RegTh);
			GenEmptyRegion(&ho_SelectedReg);
			
			Intensity (m_hImgSrc, m_hImgSrc, &htMean, &htDeviation);
			if(htMean[0].D()+0.3*htDeviation[0].D()<255)
			{
				Threshold (m_hImgSrc, &ho_RegTh, htMean[0].D()+0.3*htDeviation[0].D(), 255);
			}
			else
			{
				Threshold (m_hImgSrc, &ho_RegTh, 150, 255);
			}
			HTuple htRowCenter,htColCenter;
			AreaCenter(ho_RegTh,&lArea,&htRowCenter, &htRowCenter);
			if (lArea[0].D() > 0)
			{
				Connection (ho_RegTh, &ho_ConnReg);
				SelectShape (ho_ConnReg, &ho_SelectedReg, "width", "and", (Column21[0].D()-Column11[0].D())*1.1, 99999);
				CountObj(ho_SelectedReg,&htNum);
			}
			if(htNum[0].I() != 1)
			{
					m_bIsWhiteZouBan=FALSE;
					UpdateData(FALSE);
					if (nDlgSwitch == 1)  //属性对话框
					{
						m_pPropDlg->PostMessageA(WM_PAINT);
					}
					if (nDlgSwitch == 2)  //参数对话框
					{
						m_pParaDlg->PostMessageA(WM_PAINT);
					}
					//AfxMessageBox("提取白纸边界失败");
					CString str;
					str.Format("%s",m_strMessageText[36]);
					::MessageBoxA(m_hWnd,str,"""",MB_OK);
					return;
			}
			else
			{
				if(htNum[0].I()==1)
				{
					CString strErrorInfo = _T("");
					//SmallestRectangle1 (ho_SelectedReg, &Row12, &Column12, &Row22, &Column22);
					//利用投影提取白纸边界
					HObject hoBinImg;
					HTuple htHorProj,htVerProj;
					HTuple htIndexHor,htIndexVer,htLengthHor,hLengthVer;
					RegionToBin(ho_SelectedReg,&hoBinImg,255,0,Width,Height);
					GrayProjections(hoBinImg,hoBinImg,"simple",&htHorProj,&htVerProj);
					TupleMin2(htHorProj,40,&htHorProj);
					TupleMin2(htVerProj,40,&htVerProj);
					TupleFind(htHorProj,40,&htIndexHor);
					TupleFind(htVerProj,40,&htIndexVer);
					TupleLength(htIndexHor,&htLengthHor);
					TupleLength(htIndexVer,&hLengthVer);
					if(htLengthHor[0].D()>0)
					{
						Row12=htIndexHor[0];
						Row22=htIndexHor[htLengthHor-1];
					}
					if(hLengthVer[0].D()>0)
					{
						Column12=htIndexVer[0];
						Column22=htIndexVer[hLengthVer-1];
					}

					GenRectangle1(&hoRectengle,Row12, Column12, Row22, Column22);
					m_nleftPos=Column12[0].D();
					m_nRightPos=Column22[0].D();
					m_nUpPos=Row12[0].D();
					m_nDownPos=Row22[0].D();
					if(m_nleftPos==0&&m_nRightPos==Width-1&&m_nUpPos==0&&m_nDownPos==Height-1)
					{
						m_bIsWhiteZouBan=FALSE;
						
						UpdateData(FALSE);
						if (nDlgSwitch == 1)  //属性对话框
						{
							m_pPropDlg->PostMessageA(WM_PAINT);
						}
						if (nDlgSwitch == 2)  //参数对话框
						{
							m_pParaDlg->PostMessageA(WM_PAINT);
						}
						//AfxMessageBox("无白纸边界");
						CString str;
						str.Format("%s",m_strMessageText[37]);
						::MessageBoxA(m_hWnd,str,"""",MB_OK);
						return;
					}
					else if(m_nleftPos==0&&m_nRightPos==Width-1)
					{
						m_bIsWhiteZouBan=FALSE;
						UpdateData(FALSE);
						//AfxMessageBox("无左右边界");
						CString str;
						str.Format("%s",m_strMessageText[38]);
						::MessageBoxA(m_hWnd,str,"""",MB_OK);
						return;
					}
					else if(m_nUpPos==0&&m_nDownPos==Height-1)
					{
						m_bIsWhiteZouBan=FALSE;
						UpdateData(FALSE);
						//AfxMessageBox("无上下边界");
						CString str;
						str.Format("%s",m_strMessageText[39]);
						::MessageBoxA(m_hWnd,str,"""",MB_OK);
						return;
					}
					else 
					{
						if(m_nleftPos==0)
						{
							GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(FALSE);
							CheckRadioButton(IDC_RADIO_LEFT, //第一个参数为该组的第一个单选按钮的ID  		
								IDC_RADIO_RIGHT, //第二个参数为该组的最后一个单选按钮的ID  		
								IDC_RADIO_RIGHT);
							//AfxMessageBox("左边界未找到");
							CString str;
							str.Format("%s",m_strMessageText[40]);
							::MessageBoxA(m_hWnd,str,"""",MB_OK);
						}
						if(m_nUpPos==0)
						{
							CheckRadioButton(IDC_RADIO_UP, //第一个参数为该组的第一个单选按钮的ID  		
								IDC_RADIO_DOWN, //第二个参数为该组的最后一个单选按钮的ID  		
								IDC_RADIO_DOWN);
							GetDlgItem(IDC_RADIO_UP)->EnableWindow(FALSE);
							//AfxMessageBox("下边界未找到");
							CString str;
							str.Format("%s",m_strMessageText[41]);
							::MessageBoxA(m_hWnd,str,"""",MB_OK);
						}
						if(m_nRightPos==Width-1)
						{
							CheckRadioButton(IDC_RADIO_LEFT, //第一个参数为该组的第一个单选按钮的ID  		
								IDC_RADIO_RIGHT, //第二个参数为该组的最后一个单选按钮的ID  		
								IDC_RADIO_LEFT);
							GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(FALSE);
							//AfxMessageBox("右边界未找到");
							CString str;
							str.Format("%s",m_strMessageText[42]);
							::MessageBoxA(m_hWnd,str,"""",MB_OK);
						}
						if(m_nDownPos==Height-1)
						{
							CheckRadioButton(IDC_RADIO_UP, //第一个参数为该组的第一个单选按钮的ID  		
								IDC_RADIO_DOWN, //第二个参数为该组的最后一个单选按钮的ID  		
								IDC_RADIO_UP);
							GetDlgItem(IDC_RADIO_DOWN)->EnableWindow(FALSE);
							//AfxMessageBox("上边界未找到");
							CString str;
							str.Format("%s",m_strMessageText[43]);
							::MessageBoxA(m_hWnd,str,"""",MB_OK);
						}
					}
					GenRectangle1(&m_hoLReg,Row12,Column12,Row22,Column12);
					GenRectangle1(&m_hoRReg,Row12,Column22,Row22,Column22);
					GenRectangle1(&m_hoUReg,Row12,Column12,Row12,Column22);
					GenRectangle1(&m_hoDReg,Row22,Column12,Row22,Column22);
					MirrorRegion(m_hoUReg,&m_hoUReg,"row",Height);
					MirrorRegion(m_hoDReg,&m_hoDReg,"row",Height);
					MirrorRegion(m_hoLReg,&m_hoLReg,"row",Height);
					MirrorRegion(m_hoRReg,&m_hoRReg,"row",Height);
					if (nDlgSwitch == 1)  //属性对话框
					{
						UpdateData(TRUE);
						m_pPropDlg->bOcrDeal=0;
						if(m_bIsCheckZouBanLeft)
						{
							m_pPropDlg->m_hZoubanRegL=m_hoLReg;
							m_pPropDlg->PostMessageA(WM_PAINT);
						}
						if(m_bIsCheckZouBanRight)
						{
							m_pPropDlg->m_hZoubanRegR=m_hoRReg;
							m_pPropDlg->PostMessageA(WM_PAINT);
						}
						if(m_bIsCheckZouBanUp)
						{
							m_pPropDlg->m_hZoubanRegU=m_hoUReg;
							m_pPropDlg->PostMessageA(WM_PAINT);
						}
						if(m_bIsCheckZouBanDown)
						{
							m_pPropDlg->m_hZoubanRegD=m_hoDReg;
							m_pPropDlg->PostMessageA(WM_PAINT);
						}
					}
					else if (nDlgSwitch == 2)  //参数对话框
					{
						UpdateData(TRUE);
						m_pParaDlg->bOcrDeal=0;
						if(m_bIsCheckZouBanLeft)
						{
							m_pParaDlg->m_hZoubanRegL=m_hoLReg;
							m_pParaDlg->PostMessageA(WM_PAINT);
						}
						if(m_bIsCheckZouBanRight)
						{
							m_pParaDlg->m_hZoubanRegR=m_hoRReg;
							m_pParaDlg->PostMessageA(WM_PAINT);
						}
						if(m_bIsCheckZouBanUp)
						{
							m_pParaDlg->m_hZoubanRegU=m_hoUReg;
							m_pParaDlg->PostMessageA(WM_PAINT);
						}
						if(m_bIsCheckZouBanDown)
						{
							m_pParaDlg->m_hZoubanRegD=m_hoDReg;
							m_pParaDlg->PostMessageA(WM_PAINT);
						}
					}
				}
			}
		}
		else
		{
			//GetDlgItem(IDC_RADIO_UP)->EnableWindow(FALSE);
			//GetDlgItem(IDC_RADIO_DOWN)->EnableWindow(FALSE);
			//GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(FALSE);
			//GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(FALSE);
			//GetDlgItem(IDC_EDIT_ZOUBANSTDVER)->EnableWindow(FALSE);
			//GetDlgItem(IDC_EDIT_ZOUBANSTDHOR)->EnableWindow(FALSE);
			//GetDlgItem(IDC_STATIC_RANGE)->EnableWindow(FALSE);
			//GetDlgItem(IDC_STATIC_VERDIS)->EnableWindow(FALSE);
			//GetDlgItem(IDC_STATIC_HORDIS)->EnableWindow(FALSE);
			//GetDlgItem(IDC_EDIT_ZOUBANRANGE)->EnableWindow(FALSE);
			//GetDlgItem(IDC_SPIN_ZBVER)->EnableWindow(FALSE);
			//GetDlgItem(IDC_SPIN_ZBHOR)->EnableWindow(FALSE);
			//GetDlgItem(IDC_SPIN_SPSRANGE)->EnableWindow(FALSE);
			//GetDlgItem(IDC_SLIDER_VERDIS)->EnableWindow(FALSE);
			//GetDlgItem(IDC_SLIDER_HORDIS)->EnableWindow(FALSE);
			if(m_bCheckZouban)
			{
				m_nleftPos=0;
				m_nRightPos=m_hImgW[0].D()-1;
				m_nUpPos=0;
				m_nDownPos=m_hImgH[0].D()-1;
				GenRectangle1(&m_hoLReg,0,0,m_hImgH[0].D()-1,0);
				GenRectangle1(&m_hoRReg,0,m_hImgW[0].D()-1,m_hImgH[0].D()-1,m_hImgW[0].D()-1);
				GenRectangle1(&m_hoUReg,0,0,0,m_hImgW[0].D()-1);
				GenRectangle1(&m_hoDReg,m_hImgH[0].D()-1,0,m_hImgH[0].D()-1,m_hImgW[0].D()-1);
				MirrorRegion(m_hoUReg,&m_hoUReg,"row",m_hImgH);
				MirrorRegion(m_hoDReg,&m_hoDReg,"row",m_hImgH);
				MirrorRegion(m_hoLReg,&m_hoLReg,"row",m_hImgH);
				MirrorRegion(m_hoRReg,&m_hoRReg,"row",m_hImgH);
			}
			
			if(nDlgSwitch==1)
			{
				m_pPropDlg->bOcrDeal=0;
				DispZBRange();
			}
			if(nDlgSwitch==2)
			{
				m_pParaDlg->bOcrDeal=0;
				DispZBRange();
			}
		}
	
}


void CZouBanPro::OnBnClickedRadioLeft()
{
	UpdateData(TRUE);

	DispZBRange();
	return;

	//////////////////////////////////////////////////////////////////////////
	if (nDlgSwitch == 1)  //属性对话框
	{
		m_pPropDlg->bOcrDeal=0;
		if(m_bIsWhiteZouBan)
		{
			GenRectangle1(&m_hoLReg,0,m_nleftPos,
				m_hImgH[0].D()-1,m_nleftPos);
			m_pPropDlg->m_hZoubanRegL=m_hoLReg;
		}
		else
		{
			DispZBRange();
		}
		m_pPropDlg->PostMessageA(WM_PAINT);
	}
	if (nDlgSwitch == 2)  //参数对话框
	{
		m_pParaDlg->bOcrDeal=0;
		if(m_bIsWhiteZouBan)
		{
			GenRectangle1(&m_hoLReg,0,m_nleftPos,
				m_hImgH[0].D()-1,m_nleftPos);
			m_pParaDlg->m_hZoubanRegL=m_hoLReg;
		}
		else
		{
			DispZBRange();
		}
		m_pParaDlg->PostMessageA(WM_PAINT);
	}
	UpdateData(FALSE);
}


void CZouBanPro::OnBnClickedRadioRight()
{
	UpdateData(TRUE);

	DispZBRange();
	return;

	//////////////////////////////////////////////////////////////////////////
	if (nDlgSwitch == 1)  //属性对话框
	{
		m_pPropDlg->bOcrDeal=0;
		if(m_bIsWhiteZouBan)
		{
			GenRectangle1(&m_hoRReg,0,m_nRightPos,
				m_hImgH[0].D()-1,m_nRightPos);
			m_pPropDlg->m_hZoubanRegR=m_hoRReg;
		}
		else
		{
			DispZBRange();
		}
		m_pPropDlg->PostMessageA(WM_PAINT);
	}
	if (nDlgSwitch == 2)  //属性对话框
	{
		m_pParaDlg->bOcrDeal=0;
		if(m_bIsWhiteZouBan)
		{
			GenRectangle1(&m_hoRReg,0,m_nRightPos,
				m_hImgH[0].D()-1,m_nRightPos);
			m_pParaDlg->m_hZoubanRegR=m_hoRReg;
		}
		else
		{
			DispZBRange();
		}
		m_pParaDlg->PostMessageA(WM_PAINT);
	}
	UpdateData(FALSE);
}


void CZouBanPro::OnBnClickedRadioUp()
{
	UpdateData(TRUE);

	DispZBRange();
	return;
	
	//////////////////////////////////////////////////////////////////////////
	if (nDlgSwitch == 1)  //属性对话框
	{
		m_pPropDlg->bOcrDeal=0;
		if(m_bIsWhiteZouBan)
		{
			GenRectangle1(&m_hoUReg,m_nUpPos,0
				,m_nUpPos,m_hImgW[0].D()-1);
			MirrorRegion(m_hoUReg,&m_hoUReg,"row",m_hImgH);
			m_pPropDlg->m_hZoubanRegU=m_hoUReg;
		}
		else
		{
			DispZBRange();
		}
		m_pPropDlg->PostMessageA(WM_PAINT);
	}
	if (nDlgSwitch == 2)  //属性对话框
	{
		m_pParaDlg->bOcrDeal=0;
		if(m_bIsWhiteZouBan)
		{
			GenRectangle1(&m_hoUReg,m_nUpPos,0
				,m_nUpPos,m_hImgW[0].D()-1);
			MirrorRegion(m_hoUReg,&m_hoUReg,"row",m_hImgH);
			m_pParaDlg->m_hZoubanRegU=m_hoUReg;
		}
		else
		{
			DispZBRange();
		}
		m_pParaDlg->PostMessageA(WM_PAINT);
	}
	UpdateData(FALSE);
}


void CZouBanPro::OnBnClickedRadioDown()
{
	UpdateData(TRUE);

	DispZBRange();
	return;

	//////////////////////////////////////////////////////////////////////////
	if (nDlgSwitch == 1)  //属性对话框
	{
		m_pPropDlg->bOcrDeal=0;
		if(m_bIsWhiteZouBan)
		{
			GenRectangle1(&m_hoDReg,m_nDownPos,0
				,m_nDownPos,m_hImgW[0].D()-1);
			MirrorRegion(m_hoDReg,&m_hoDReg,"row",m_hImgH);
			m_pPropDlg->m_hZoubanRegD=m_hoDReg;
		}
		else
		{
			DispZBRange();
		}
		m_pPropDlg->PostMessageA(WM_PAINT);
	}
	if (nDlgSwitch == 2)  //参数对话框
	{
		m_pParaDlg->bOcrDeal=0;
		if(m_bIsWhiteZouBan)
		{
			GenRectangle1(&m_hoDReg,m_nDownPos,0
				,m_nDownPos,m_hImgW[0].D()-1);
			MirrorRegion(m_hoDReg,&m_hoDReg,"row",m_hImgH);
			m_pParaDlg->m_hZoubanRegD = m_hoDReg;
		}
		else
		{
			DispZBRange();
		}
		m_pParaDlg->PostMessageA(WM_PAINT);
	}
	UpdateData(FALSE);
}

void CZouBanPro::OnEnChangeEditZoubanstdver()
{
	UpdateData(TRUE);
	if(m_fZouBanStdVer>10)
	{
		m_fZouBanStdVer=10;
	}
	if(m_fZouBanStdVer<0)
	{
		m_fZouBanStdVer=0;
	}
	m_sliderVerDis.SetPos(m_fZouBanStdVer*10);
	DispZBRange();
	UpdateData(FALSE);
}


void CZouBanPro::OnEnChangeEditZoubanstdhor()
{
	UpdateData(TRUE);
	if(m_fZouBanStdHor>10)
	{
		m_fZouBanStdHor=10;
	}
	if(m_fZouBanStdHor<0)
	{
		m_fZouBanStdHor=0;
	}
	m_sliderHorDis.SetPos(m_fZouBanStdHor*10);
	DispZBRange();
	UpdateData(FALSE);
}

void CZouBanPro::OnEnChangeEditZoubanrange()
{
	UpdateData(TRUE);
	if(m_fZouBanRange>10)
	{
		m_fZouBanRange=10;
	}
	if(m_fZouBanRange<0)
	{
		m_fZouBanRange=0;
	}
	m_sliderZBRange.SetPos(m_fZouBanRange*10);
	DispZBRange();
	UpdateData(FALSE);
}


void CZouBanPro::OnDeltaposSpinSpsrange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_fZouBanRange<=9.99)
		{
			m_fZouBanRange += 0.01;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_fZouBanRange >= 0.01)
		{
			m_fZouBanRange -= 0.01;
		}	
	}
	m_sliderZBRange.SetPos(m_fZouBanRange*10);
	DispZBRange();
	UpdateData(FALSE);
	*pResult = 0;
}


void CZouBanPro::OnDeltaposSpinZbver(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_fZouBanStdVer<=9.99)
		{
			m_fZouBanStdVer += 0.01;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_fZouBanStdVer >= 0.01)
		{
			m_fZouBanStdVer -= 0.01;
		}	
	}
	m_sliderVerDis.SetPos(m_fZouBanStdVer*10);
	DispZBRange();
	UpdateData(FALSE);
	*pResult = 0;
}


void CZouBanPro::OnDeltaposSpinZbhor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_fZouBanStdHor<=9.99)
		{
			m_fZouBanStdHor += 0.01;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_fZouBanStdHor >= 0.01)
		{
			m_fZouBanStdHor -= 0.01;
		}	
	}
	m_sliderHorDis.SetPos(m_fZouBanStdHor*10);
	DispZBRange();
	UpdateData(FALSE);
	*pResult = 0;
}

void CZouBanPro::DispZBRange()
{
	if(m_bIsCheckZouBanLeft)
	{
		int nZouBanMin=MAX(m_nleftPos+(m_fZouBanStdHor-m_fZouBanRange)/m_fScaleHor,0);
		int nZouBanMax=MAX(m_nleftPos+(m_fZouBanStdHor+m_fZouBanRange)/m_fScaleHor,0);
		GenRectangle1(&m_hRealZoubanRegionL,0,MAX(nZouBanMin,0),
			m_hImgH[0].D()-1,MIN(nZouBanMax,m_hImgW[0].D()-1));
		MirrorRegion(m_hRealZoubanRegionL,&m_hRealZoubanRegionL,"row",m_hImgH[0].D());
		if(nDlgSwitch==1)
		{
			m_pPropDlg->bOcrDeal=0;
			m_pPropDlg->m_hZoubanRegL=m_hRealZoubanRegionL;
			m_pPropDlg->PostMessageA(WM_PAINT);
		}
		if(nDlgSwitch==2)
		{
			m_pParaDlg->bOcrDeal=0;
			m_pParaDlg->m_hZoubanRegL=m_hRealZoubanRegionL;
			m_pParaDlg->PostMessageA(WM_PAINT);
		}
	}
	if(m_bIsCheckZouBanRight)
	{
		int nZouBanMin = 0;
		int nZouBanMax = 0;
		if (m_nRightPos == 0)
		{
			nZouBanMin=MAX(m_hImgW[0].D()-1-(m_fZouBanStdHor+m_fZouBanRange)/m_fScaleHor,0);
			nZouBanMax=MAX(m_hImgW[0].D()-1-(m_fZouBanStdHor-m_fZouBanRange)/m_fScaleHor,0);
		}
		else
		{
			nZouBanMin=MAX(m_nRightPos-(m_fZouBanStdHor+m_fZouBanRange)/m_fScaleHor,0);
			nZouBanMax=MAX(m_nRightPos-(m_fZouBanStdHor-m_fZouBanRange)/m_fScaleHor,0);
		}
		
		GenRectangle1(&m_hRealZoubanRegionR,0,MAX(nZouBanMin,0),
			m_hImgH[0].D()-1,MIN(nZouBanMax,m_hImgW[0].D()-1));
		MirrorRegion(m_hRealZoubanRegionR,&m_hRealZoubanRegionR,"row",m_hImgH);
		if(nDlgSwitch==1)
		{
			m_pPropDlg->bOcrDeal=0;
			m_pPropDlg->m_hZoubanRegR=m_hRealZoubanRegionR;
			m_pPropDlg->PostMessageA(WM_PAINT);
		}
		if(nDlgSwitch==2)
		{
			m_pParaDlg->bOcrDeal=0;
			m_pParaDlg->m_hZoubanRegR=m_hRealZoubanRegionR;
			m_pParaDlg->PostMessageA(WM_PAINT);
		}
		
	}
	if(m_bIsCheckZouBanUp)
	{
		int nZouBanMin=MAX(m_nUpPos+(m_fZouBanStdVer-m_fZouBanRange)/m_fScaleVer,0);
		int nZouBanMax=MAX(m_nUpPos+(m_fZouBanStdVer+m_fZouBanRange)/m_fScaleVer,0);
		GenRectangle1(&m_hRealZoubanRegionU,MAX(nZouBanMin,0),0,
			MIN(nZouBanMax,m_hImgH[0].D()-1),m_hImgW[0].D()-1);
		MirrorRegion(m_hRealZoubanRegionU,&m_hRealZoubanRegionU,"row",m_hImgH);
		
		if(nDlgSwitch==1)
		{
			m_pPropDlg->bOcrDeal=0;
			m_pPropDlg->m_hZoubanRegU=m_hRealZoubanRegionU;
			m_pPropDlg->PostMessageA(WM_PAINT);
		}
		if(nDlgSwitch==2)
		{
			m_pParaDlg->bOcrDeal=0;
			m_pParaDlg->m_hZoubanRegU=m_hRealZoubanRegionU;
			m_pParaDlg->PostMessageA(WM_PAINT);
		}
		
	}
	if(m_bIsCheckZouBanDown)
	{
		int nZouBanMin = 0;
		int nZouBanMax = 0;
		if (m_nDownPos == 0)
		{
			nZouBanMin=MAX(m_hImgH[0].D()-1-(m_fZouBanStdVer+m_fZouBanRange)/m_fScaleVer,0);
			nZouBanMax=MAX(m_hImgH[0].D()-1-(m_fZouBanStdVer-m_fZouBanRange)/m_fScaleVer,0);
		}
		else
		{
			nZouBanMin=MAX(m_nDownPos-(m_fZouBanStdVer+m_fZouBanRange)/m_fScaleVer,0);
			nZouBanMax=MAX(m_nDownPos-(m_fZouBanStdVer-m_fZouBanRange)/m_fScaleVer,0);
		}
		GenRectangle1(&m_hRealZoubanRegionD,MAX(nZouBanMin,0),0,
			MIN(nZouBanMax,m_hImgH[0].D()-1),m_hImgW[0].D()-1);
		MirrorRegion(m_hRealZoubanRegionD,&m_hRealZoubanRegionD,"row",m_hImgH);
		
		if(nDlgSwitch==1)
		{
			m_pPropDlg->bOcrDeal=0;
			m_pPropDlg->m_hZoubanRegD=m_hRealZoubanRegionD;
			m_pPropDlg->PostMessageA(WM_PAINT);
		}
		if(nDlgSwitch==2)
		{
			m_pParaDlg->bOcrDeal=0;
			m_pParaDlg->m_hZoubanRegD=m_hRealZoubanRegionD;
			m_pParaDlg->PostMessageA(WM_PAINT);
		}

	}
}
