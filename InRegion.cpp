// InRegion.cpp : implementation file
//

#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "InRegion.h"


// CInRegion dialog

IMPLEMENT_DYNAMIC(CInRegion, CBCGPDialog)

CInRegion::CInRegion(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CInRegion::IDD, pParent)
	, m_nDeviation(0)
	, m_nMaxGray(0)
	, m_bCheckInRegin(FALSE)
	,m_bCheckHorVer(FALSE)
	,m_dArea(0)
	,m_nDirect(1)
	,m_nContrast(3)
	,m_bShowHorVer(TRUE)
	, m_nPercent(0)
{
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
}

CInRegion::~CInRegion()
{
}

void CInRegion::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEVIATION, m_nDeviation);
	DDV_MinMaxInt(pDX, m_nDeviation, 0, 255);
	DDX_Text(pDX, IDC_EDIT_MAXGRAY, m_nMaxGray);
	DDV_MinMaxInt(pDX, m_nMaxGray, 0, 255);
	DDX_Check(pDX, IDC_CHECK_INRGION, m_bCheckInRegin);
	DDX_Control(pDX, IDC_CHECK_INREGION, m_cmdCheckInRegion);
	DDX_Control(pDX, IDC_STATIC_DEVIATION, m_labDeviation);
	DDX_Control(pDX, IDC_STATIC_MAXGRAY, m_labMaxGray);
	DDX_Control(pDX, IDC_STATIC_CONTRAST, m_LabContrast);
	DDX_Control(pDX, IDC_SLIDER_DEV, m_slcDev);
	DDX_Control(pDX, IDC_SLIDER_MAXGRAY, m_slcMaxGray);
	DDX_Control(pDX, IDC_SLIDER_AREA, m_slcArea);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_slcContrast);
	DDX_Check(pDX, IDC_CHECK_HORVER2, m_bCheckHorVer);
	DDX_Control(pDX, IDC_STATIC_AREA, m_LabArea);
	DDX_Control(pDX, IDC_STATIC_DIRECT, m_LabDirect);
	DDX_Control(pDX, IDC_COMBO_DIRECT, m_ComDirect);
	DDX_Text(pDX, IDC_EDIT_AREA2, m_dArea);
	DDV_MinMaxDouble(pDX, m_dArea, 0, 5);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_nContrast);
	DDV_MinMaxInt(pDX, m_nContrast, 1, 30);
	DDX_Control(pDX, IDC_CHECK_HORVER2, m_cmdCheckHorver2);
	DDX_Control(pDX, IDC_STATIC_PERCENT, m_labPencent);
	DDX_Text(pDX, IDC_EDIT_PERCENT, m_nPercent);
	DDX_Control(pDX, IDC_SLIDER_PERCENT, m_slcPercent);
}


BEGIN_MESSAGE_MAP(CInRegion, CBCGPDialog)
	ON_BN_CLICKED(IDC_CHECK_INRGION, &CInRegion::OnBnClickedCheckInrgion)
	ON_EN_CHANGE(IDC_EDIT_DEVIATION, &CInRegion::OnEnChangeEditDeviation)
	ON_EN_CHANGE(IDC_EDIT_MAXGRAY, &CInRegion::OnEnChangeEditMaxgray)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DEV, &CInRegion::OnDeltaposSpinDev)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MAXGRAY, &CInRegion::OnDeltaposSpinMaxgray)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_AREA, &CInRegion::OnDeltaposSpinArea)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_DIRECT, &CInRegion::OnCbnSelchangeComboDirect)
	ON_EN_CHANGE(IDC_EDIT_AREA2, &CInRegion::OnEnChangeEditArea2)
	ON_BN_CLICKED(IDC_CHECK_HORVER2, &CInRegion::OnBnClickedCheckHorver2)
	ON_EN_CHANGE(IDC_EDIT_CONTRAST, &CInRegion::OnEnChangeEditContrast)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CONTRAST, &CInRegion::OnDeltaposSpinContrast)
	ON_EN_CHANGE(IDC_EDIT_PERCENT, &CInRegion::OnEnChangeEditPercent)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PERCENT, &CInRegion::OnDeltaposSpinPercent)
END_MESSAGE_MAP()


// CInRegion message handlers

void CInRegion::OnBnClickedCheckInrgion()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CInRegion::OnEnChangeEditDeviation()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	if (m_nDeviation<0)
	{
		m_nDeviation = 0;
	}
	if (m_nDeviation>255)
	{
		m_nDeviation = 255;
	}
	UpdateData(FALSE);
	m_slcDev.SetPos(m_nDeviation);
	// TODO:  Add your control notification handler code here
}

void CInRegion::OnEnChangeEditMaxgray()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	if (m_nMaxGray<0)
	{
		m_nMaxGray = 0;
	}
	if (m_nMaxGray>255)
	{
		m_nMaxGray = 255;
	}
	UpdateData(FALSE);
	m_slcMaxGray.SetPos(m_nMaxGray);
	// TODO:  Add your control notification handler code here
}


BOOL CInRegion::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//  [4/6/2017 zb] 国际化配置，界面英文显示
	m_strLanguagePath.Format("%s\\International Config\\ControlEnglish.ini", m_strDefaultParaPath);
	m_strControlMapPath.Format("%s\\International Config\\ControlMap.ini", m_strDefaultParaPath);
	if (m_nLanguage == 1)//英文
	{
		int nNum = 0;
		nNum = GetPrivateProfileInt("Code in Lack of Printing Parameters", "Total Control Number", 0, m_strLanguagePath);
		if(nNum > 0)
		{
			s_ControlSet *psControlSet = NULL;
			psControlSet = new s_ControlSet[nNum];
			for (int i=1;i<=nNum;i++)
			{
				char str[256];
				memset(str,0,256);
				CString strNo;
				strNo.Format("CLPControl NO %d",i);
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
	//设置背景
	//SetBackgroundImage(IDB_BITMAP_TABBACK);
	m_cmdCheckInRegion.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckInRegion.SetMouseCursorHand();
	m_labDeviation.m_clrText = RGB(218,165,0);
	m_labMaxGray.m_clrText   = RGB(218,165,0);

	//slider control
	m_slcDev.SetRange(0,255);
	m_slcDev.SetPos(m_nDeviation);
	m_slcMaxGray.SetRange(0,255);
	m_slcMaxGray.SetPos(m_nMaxGray);
	//  [4/6/2017 zb]
	if (m_nLanguage == 1)
	{
		m_ComDirect.AddString("Horizontal");
		m_ComDirect.AddString("Vertical");
	} 
	else
	{
		m_ComDirect.AddString("横向");
		m_ComDirect.AddString("纵向");
	}
	
	m_ComDirect.SetCurSel(m_nDirect);
	m_cmdCheckHorver2.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckHorver2.SetMouseCursorHand();
	m_LabDirect.m_clrText = RGB(218,165,0);
	m_LabArea.m_clrText = RGB(218, 165, 0);
	m_LabContrast.m_clrText = RGB(218, 165, 0);
	m_labPencent.m_clrText = RGB(218, 165, 0);
	m_slcArea.SetRange(0,50);
	m_slcArea.SetPos(m_dArea*10);
	m_slcContrast.SetRange(1, 30);
	m_slcContrast.SetPos(m_nContrast);
	m_slcPercent.SetRange(1, 100);
	m_slcPercent.SetPos(m_nPercent);
	UpdateState();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CInRegion::UpdateState()
{
	if (m_bShowHorVer)
	{
		GetDlgItem(IDC_STATIC_SHOW)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_HORVER2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_DIRECT)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_CONTRAST)->ShowWindow(TRUE);
		//GetDlgItem(IDC_STATIC_AREA)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(TRUE);
		GetDlgItem(IDC_COMBO_DIRECT)->ShowWindow(TRUE);
		//GetDlgItem(IDC_SPIN_AREA)->ShowWindow(TRUE);
		GetDlgItem(IDC_SPIN_CONTRAST)->ShowWindow(TRUE);
		GetDlgItem(IDC_SPIN_PERCENT)->ShowWindow(TRUE);
		//GetDlgItem(IDC_EDIT_AREA2)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_CONTRAST)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_PERCENT)->ShowWindow(TRUE);
		//GetDlgItem(IDC_SLIDER_AREA)->ShowWindow(TRUE);
		GetDlgItem(IDC_SLIDER_CONTRAST)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_SHOW)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_HORVER2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_DIRECT)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_CONTRAST)->ShowWindow(FALSE);
		//GetDlgItem(IDC_STATIC_AREA)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(FALSE);
		GetDlgItem(IDC_COMBO_DIRECT)->ShowWindow(FALSE);
		//GetDlgItem(IDC_SPIN_AREA)->ShowWindow(FALSE);
		GetDlgItem(IDC_SPIN_CONTRAST)->ShowWindow(FALSE);
		GetDlgItem(IDC_SPIN_PERCENT)->ShowWindow(FALSE);
		//GetDlgItem(IDC_EDIT_AREA2)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_CONTRAST)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_PERCENT)->ShowWindow(FALSE);
		//GetDlgItem(IDC_SLIDER_AREA)->ShowWindow(FALSE);
		GetDlgItem(IDC_SLIDER_CONTRAST)->ShowWindow(FALSE);
		GetDlgItem(IDC_SLIDER_PERCENT)->ShowWindow(FALSE);
	}

}
void CInRegion::OnDeltaposSpinDev(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_nDeviation<=254)
		{
			m_nDeviation++;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_nDeviation >= 1)
		{
			m_nDeviation--;
		}	
	}
	m_slcDev.SetPos(m_nDeviation);
	UpdateData(FALSE);

	*pResult = 0;
}


void CInRegion::OnDeltaposSpinMaxgray(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_nMaxGray<=254)
		{
			m_nMaxGray++;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_nMaxGray >= 1)
		{
			m_nMaxGray--;
		}	
	}
	m_slcMaxGray.SetPos(m_nMaxGray);
	UpdateData(FALSE);

	*pResult = 0;
}


void CInRegion::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_DEV) 
	{ 
		m_nDeviation = m_slcDev.GetPos();
		UpdateData(FALSE);
	}
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_MAXGRAY) 
	{ 
		m_nMaxGray = m_slcMaxGray.GetPos();
		UpdateData(FALSE);
	}
	if (pScrollBar->GetDlgCtrlID()==IDC_SLIDER_AREA)
	{
		m_dArea = m_slcArea.GetPos()*0.1;
		UpdateData(FALSE);
	}
	if (pScrollBar->GetDlgCtrlID()==IDC_SLIDER_CONTRAST)
	{
		m_nContrast = m_slcContrast.GetPos();
		UpdateData(FALSE);
	}
	if (pScrollBar->GetDlgCtrlID()==IDC_SLIDER_PERCENT)
	{
		m_nPercent = m_slcPercent.GetPos();
		UpdateData(FALSE);
	}

	CBCGPDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
void CInRegion::OnDeltaposSpinArea(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMArea = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMArea->iDelta == -1)
	{
		if (m_dArea < 5)
		{
			m_dArea = m_dArea+0.1;
		}	
	}
	else if (pNMArea->iDelta == 1)
	{
		if (m_dArea >= 0.1)
		{
			m_dArea = m_dArea-0.1;
		}	
	}
	m_slcArea.SetPos(m_dArea*10);
	UpdateData(FALSE);

	*pResult = 0;

}
void CInRegion::OnCbnSelchangeComboDirect()
{

	UpdateData(TRUE);
	m_nDirect = m_ComDirect.GetCurSel();
}
void CInRegion::OnEnChangeEditArea2()
{
	UpdateData(TRUE);
	if (m_dArea<0)
	{
		m_dArea = 0;
	}
	if(m_dArea>5)
	{
		m_dArea = 5;
	}
	UpdateData(FALSE);
	m_slcArea.SetPos(m_dArea*10);
}


void CInRegion::OnBnClickedCheckHorver2()
{
	UpdateData(TRUE);
}


void CInRegion::OnEnChangeEditContrast()
{
	UpdateData(TRUE);
	if (m_nContrast<1)
	{
		m_nContrast = 1;
	}
	if (m_nContrast>30)
	{
		m_nContrast = 30;
	}
	UpdateData(FALSE);
	m_slcContrast.SetPos(m_nContrast);
}


void CInRegion::OnDeltaposSpinContrast(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_nContrast  <= 29)
		{
			m_nContrast = m_nContrast+1;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_nContrast >= 2)
		{
			m_nContrast = m_nContrast-1;
		}	
	}
	m_slcContrast.SetPos(m_nContrast);
	UpdateData(FALSE);
	*pResult = 0;
}


void CInRegion::OnEnChangeEditPercent()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nPercent<0)
	{
		m_nPercent = 0;
	}
	if(m_nPercent>100)
	{
		m_nPercent = 100;
	}
	UpdateData(FALSE);
	m_slcPercent.SetPos(m_nPercent);
}


void CInRegion::OnDeltaposSpinPercent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMPercent = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMPercent->iDelta == -1)
	{
		if (m_nPercent < 100)
		{
			m_nPercent = m_nPercent+1;
		}	
	}
	else if (pNMPercent->iDelta == 1)
	{
		if (m_nPercent >= 1)
		{
			m_nPercent = m_nPercent-1;
		}	
	}
	m_slcPercent.SetPos(m_nPercent);
	UpdateData(FALSE);
	*pResult = 0;
}
