// OutRange.cpp : implementation file
//

#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "OutRange.h"


// COutRange dialog

IMPLEMENT_DYNAMIC(COutRange, CBCGPDialog)

COutRange::COutRange(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(COutRange::IDD, pParent)
	, m_bCheckOutRange(FALSE)
	, m_dTop(0)
	, m_dLeft(0)
	, m_dBottom(0)
	, m_dRight(0)
	, m_dSingleArea(0)
	, m_dAllArea(0)	
{
	m_fScaleHor = 0;
	m_fScaleVer = 0;
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

COutRange::~COutRange()
{
}

void COutRange::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_OUTRANGE, m_bCheckOutRange);
	DDX_Text(pDX, IDC_EDIT_TOP, m_dTop);
	DDV_MinMaxDouble(pDX, m_dTop, 0, 10);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_dLeft);
	DDV_MinMaxDouble(pDX, m_dLeft, 0, 10);
	DDX_Text(pDX, IDC_EDIT_BOTTOM, m_dBottom);
	DDV_MinMaxDouble(pDX, m_dBottom, 0, 10);
	DDX_Text(pDX, IDC_EDITRIGHT, m_dRight);
	DDV_MinMaxDouble(pDX, m_dRight, 0, 10);
	DDX_Text(pDX, IDC_EDIT_SINGLEAREA, m_dSingleArea);
	DDV_MinMaxDouble(pDX, m_dSingleArea, 0, 5);
	DDX_Text(pDX, IDC_EDIT_ALLAREA, m_dAllArea);
	DDV_MinMaxDouble(pDX, m_dAllArea, 0, 25);

	DDX_Control(pDX, IDC_CHECK_OUTRANGE, m_cmdCheckOutRange);
	DDX_Control(pDX, IDC_STATIC_TOP, m_labTop);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_labLeft);
	DDX_Control(pDX, IDC_STATIC_BOTTOM, m_labBottom);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_labRight);
	DDX_Control(pDX, IDC_STATIC_SINGLEAREA, m_labSingleArea);
	DDX_Control(pDX, IDC_STATIC_ALLAREA, m_labAllArea);
	DDX_Control(pDX, IDC_SLIDER_TOP, m_slcTop);
	DDX_Control(pDX, IDC_SLIDER_LEFT, m_slcLeft);
	DDX_Control(pDX, IDC_SLIDER_BOTTOM, m_slcBottom);
	DDX_Control(pDX, IDC_SLIDER_RIGHT, m_slcRight);
	DDX_Control(pDX, IDC_SLIDER_SINGLEAREA, m_slcSingleArea);
	DDX_Control(pDX, IDC_SLIDER_ALLAREA, m_slcAllArea);
}

BEGIN_MESSAGE_MAP(COutRange, CBCGPDialog)
	ON_BN_CLICKED(IDC_CHECK_OUTRANGE, &COutRange::OnBnClickedCheckOutrange)
	ON_EN_CHANGE(IDC_EDIT_TOP, &COutRange::OnEnChangeEditTop)
	ON_EN_CHANGE(IDC_EDIT_BOTTOM, &COutRange::OnEnChangeEditBottom)
	ON_EN_CHANGE(IDC_EDIT_LEFT, &COutRange::OnEnChangeEditLeft)
	ON_EN_CHANGE(IDC_EDITRIGHT, &COutRange::OnEnChangeEditright)
	ON_EN_CHANGE(IDC_EDIT_SINGLEAREA, &COutRange::OnEnChangeEditSinglearea)
	ON_EN_CHANGE(IDC_EDIT_ALLAREA, &COutRange::OnEnChangeEditAllarea)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TOP, &COutRange::OnDeltaposSpinTop)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LEFT, &COutRange::OnDeltaposSpinLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BOTTOM, &COutRange::OnDeltaposSpinBottom)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RIGHT, &COutRange::OnDeltaposSpinRight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SINGLEAREA, &COutRange::OnDeltaposSpinSinglearea)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ALLAREA, &COutRange::OnDeltaposSpinAllarea)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// COutRange message handlers

void COutRange::OnBnClickedCheckOutrange()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void COutRange::OnEnChangeEditTop()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	if (m_dTop<0)
	{
		m_dTop = 0;
	}
	if (m_dTop>10)
	{
		m_dTop = 10;
	}
	UpdateData(FALSE);
	m_slcTop.SetPos(m_dTop*10);
	// TODO:  Add your control notification handler code here
}

void COutRange::OnEnChangeEditBottom()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	if (m_dBottom<0)
	{
		m_dBottom = 0;
	}
	if (m_dBottom>10)
	{
		m_dBottom = 10;
	}
	UpdateData(FALSE);
	m_slcBottom.SetPos(m_dBottom*10);
	// TODO:  Add your control notification handler code here
}

void COutRange::OnEnChangeEditLeft()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	if (m_dLeft<0)
	{
		m_dLeft = 0;
	}
	if (m_dLeft>10)
	{
		m_dLeft = 10;
	}
	UpdateData(FALSE);
	m_slcLeft.SetPos(m_dLeft*10);
	// TODO:  Add your control notification handler code here
}

void COutRange::OnEnChangeEditright()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	if (m_dRight<0)
	{
		m_dRight = 0;
	}
	if (m_dRight>10)
	{
		m_dRight = 10;
	}
	UpdateData(FALSE);
	m_slcRight.SetPos(m_dRight*10);
	// TODO:  Add your control notification handler code here
}

void COutRange::OnEnChangeEditSinglearea()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	if (m_dSingleArea<0)
	{
		m_dSingleArea = 0;
	}
	if (m_dSingleArea>5)
	{
		m_dSingleArea = 5;
	}
	UpdateData(FALSE);
	m_slcSingleArea.SetPos(m_dSingleArea*10);
	// TODO:  Add your control notification handler code here
}

void COutRange::OnEnChangeEditAllarea()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	if (m_dAllArea<0)
	{
		m_dAllArea = 0;
	}
	if (m_dAllArea>25)
	{
		m_dAllArea = 25;
	}
	UpdateData(FALSE);
	m_slcAllArea.SetPos(m_dAllArea*10);
	// TODO:  Add your control notification handler code here
}

BOOL COutRange::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//SetBackgroundImage(IDB_BITMAP_TABBACK);
	m_cmdCheckOutRange.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckOutRange.SetMouseCursorHand();
	m_labTop.m_clrText = RGB(218,165,0);
	m_labLeft.m_clrText   = RGB(218,165,0);
	m_labBottom.m_clrText = RGB(218,165,0);
	m_labRight.m_clrText   = RGB(218,165,0);
	m_labSingleArea.m_clrText = RGB(218,165,0);
	m_labAllArea.m_clrText   = RGB(218,165,0);

	m_slcTop.SetRange(0,100);
	m_slcTop.SetPos(m_dTop*10);
	m_slcLeft.SetRange(0,100);
	m_slcLeft.SetPos(m_dLeft*10);
	m_slcBottom.SetRange(0,100);
	m_slcBottom.SetPos(m_dBottom*10);
	m_slcRight.SetRange(0,100);
	m_slcRight.SetPos(m_dRight*10);
	
	m_slcSingleArea.SetRange(0,50);
	m_slcSingleArea.SetPos(m_dSingleArea*10);
	m_slcAllArea.SetRange(0,250);
	m_slcAllArea.SetPos(m_dAllArea*10);
	//  [4/7/2017 zb] 国际化配置，界面英文显示
	m_strLanguagePath.Format("%s\\International Config\\ControlEnglish.ini", m_strDefaultParaPath);
	m_strControlMapPath.Format("%s\\International Config\\ControlMap.ini", m_strDefaultParaPath);
	if (m_nLanguage == 1)//英文
	{
		int nNum = 0;
		nNum = GetPrivateProfileInt("Yards Dirty Parameters", "Total Control Number", 0, m_strLanguagePath);
		if(nNum > 0)
		{
			s_ControlSet *psControlSet = NULL;
			psControlSet = new s_ControlSet[nNum];
			for (int i=1;i<=nNum;i++)
			{
				char str[256];
				memset(str,0,256);
				CString strNo;
				strNo.Format("YDControl NO %d",i);
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
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void COutRange::OnDeltaposSpinTop(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_dTop<=9.99)
		{
			m_dTop += 0.01;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_dTop >= 0.01)
		{
			m_dTop -= 0.01;
		}	
	}
	m_slcTop.SetPos(m_dTop*10);
	UpdateData(FALSE);

	*pResult = 0;
}


void COutRange::OnDeltaposSpinLeft(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_dLeft<=9.99)
		{
			m_dLeft += 0.01;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_dLeft >= 0.01)
		{
			m_dLeft -= 0.01;
		}	
	}
	m_slcLeft.SetPos(m_dLeft*10);
	UpdateData(FALSE);
	*pResult = 0;
}


void COutRange::OnDeltaposSpinBottom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_dBottom<=9.99)
		{
			m_dBottom += 0.01;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_dBottom >= 0.01)
		{
			m_dBottom -= 0.01;
		}	
	}
	m_slcBottom.SetPos(m_dBottom*10);
	UpdateData(FALSE);
	*pResult = 0;
}


void COutRange::OnDeltaposSpinRight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_dRight<=9.99)
		{
			m_dRight += 0.01;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_dRight >= 0.01)
		{
			m_dRight -= 0.01;
		}	
	}
	m_slcRight.SetPos(m_dRight*10);
	UpdateData(FALSE);
	*pResult = 0;
}


void COutRange::OnDeltaposSpinSinglearea(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_dSingleArea<=4.99)
		{
			m_dSingleArea += 0.01;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_dSingleArea >= 0.01)
		{
			m_dSingleArea -= 0.01;
		}	
	}
	m_slcSingleArea.SetPos(m_dSingleArea*10);
	UpdateData(FALSE);
	*pResult = 0;
}


void COutRange::OnDeltaposSpinAllarea(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_dAllArea<=24.99)
		{
			m_dAllArea += 0.01;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_dAllArea >= 0.01)
		{
			m_dAllArea -= 0.01;
		}	
	}
	m_slcAllArea.SetPos(m_dAllArea*10);
	UpdateData(FALSE);
	*pResult = 0;
}


void COutRange::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_TOP) 
	{ 
		m_dTop = m_slcTop.GetPos()*0.1;
		UpdateData(FALSE);
	}
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_LEFT) 
	{ 
		m_dLeft = m_slcLeft.GetPos()*0.1;
		UpdateData(FALSE);
	}
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_BOTTOM) 
	{ 
		m_dBottom = m_slcBottom.GetPos()*0.1;
		UpdateData(FALSE);
	}
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_RIGHT) 
	{ 
		m_dRight = m_slcRight.GetPos()*0.1;
		UpdateData(FALSE);
	}
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_SINGLEAREA) 
	{ 
		m_dSingleArea = m_slcSingleArea.GetPos()*0.1;
		UpdateData(FALSE);
	}
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_ALLAREA) 
	{ 
		m_dAllArea = m_slcAllArea.GetPos()*0.1;
		UpdateData(FALSE);
	}
	CBCGPDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}