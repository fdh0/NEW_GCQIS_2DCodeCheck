// SetTimeout.cpp : implementation file
//

#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "SetTimeout.h"


// CSetTimeout dialog

IMPLEMENT_DYNAMIC(CSetTimeout, CBCGPDialog)

CSetTimeout::CSetTimeout(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CSetTimeout::IDD, pParent)
	, m_bCheckTime(FALSE)
	, m_nSingleTime(10)
	, m_nAllTime(600)
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

CSetTimeout::~CSetTimeout()
{
}

void CSetTimeout::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_TIMEOUT, m_bCheckTime);
	DDX_Text(pDX, IDC_EDIT_SINGLETIME, m_nSingleTime);
	DDX_Text(pDX, IDC_EDIT_ALLTIME, m_nAllTime);
	DDV_MinMaxInt(pDX, m_nAllTime, 10, 1000);
	DDV_MinMaxInt(pDX, m_nSingleTime, 10, 100);
	DDX_Control(pDX, IDC_CHECK_TIMEOUT, m_cmdCheckTimeout);
	DDX_Control(pDX, IDC_STATIC_SINGLETIME, m_labSingleTime);
	DDX_Control(pDX, IDC_STATIC_ALLTIME, m_labAllTime);
	DDX_Control(pDX, IDC_SLIDER_SINGLETIME, m_slcSingleTime);
	DDX_Control(pDX, IDC_SLIDER_ALLTIME, m_slcAllTime);
}


BEGIN_MESSAGE_MAP(CSetTimeout, CBCGPDialog)
	ON_BN_CLICKED(IDC_CHECK_TIMEOUT, &CSetTimeout::OnBnClickedCheckTimeout)
	ON_EN_KILLFOCUS(IDC_EDIT_SINGLETIME, &CSetTimeout::OnEnKillfocusEditSingletime)
	ON_EN_KILLFOCUS(IDC_EDIT_ALLTIME, &CSetTimeout::OnEnKillfocusEditAlltime)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SINGLETIME, &CSetTimeout::OnDeltaposSpinSingletime)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ALLTIME, &CSetTimeout::OnDeltaposSpinAlltime)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_SINGLETIME, &CSetTimeout::OnEnChangeEditSingletime)
	ON_EN_CHANGE(IDC_EDIT_ALLTIME, &CSetTimeout::OnEnChangeEditAlltime)
END_MESSAGE_MAP()


// CSetTimeout message handlers

void CSetTimeout::OnBnClickedCheckTimeout()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//if (m_bCheckTime)
	//{
	//	GetDlgItem(IDC_EDIT_SINGLETIME)->ShowWindow(SW_SHOW);
	//	GetDlgItem(IDC_EDIT_ALLTIME)->ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	GetDlgItem(IDC_EDIT_SINGLETIME)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_EDIT_ALLTIME)->ShowWindow(SW_HIDE);
	//}
}

void CSetTimeout::OnEnKillfocusEditSingletime()
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
}

void CSetTimeout::OnEnKillfocusEditAlltime()
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
}

BOOL CSetTimeout::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//设置背景
	//SetBackgroundImage(IDB_BITMAP_TABBACK);
	m_cmdCheckTimeout.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckTimeout.SetMouseCursorHand();
	
	m_labSingleTime.m_clrText = RGB(218,165,0);
	m_labAllTime.m_clrText   = RGB(218,165,0);

	m_slcSingleTime.SetRange(10,100);
	m_slcSingleTime.SetPos(m_nSingleTime);
	m_slcAllTime.SetRange(10,1000);
	m_slcAllTime.SetPos(m_nAllTime);
	//  [4/7/2017 zb] 国际化配置，界面英文显示
	m_strLanguagePath.Format("%s\\International Config\\ControlEnglish.ini", m_strDefaultParaPath);
	m_strControlMapPath.Format("%s\\International Config\\ControlMap.ini", m_strDefaultParaPath);
	if (m_nLanguage == 1)//英文
	{
		int nNum = 0;
		nNum = GetPrivateProfileInt("Timeout Control Parameters", "Total Control Number", 0, m_strLanguagePath);
		if(nNum > 0)
		{
			s_ControlSet *psControlSet = NULL;
			psControlSet = new s_ControlSet[nNum];
			for (int i=1;i<=nNum;i++)
			{
				char str[256];
				memset(str,0,256);
				CString strNo;
				strNo.Format("TCControl NO %d",i);
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
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSetTimeout::OnDeltaposSpinSingletime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_nSingleTime < 100)
		{
			m_nSingleTime++;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_nSingleTime >10)
		{
			m_nSingleTime--;
		}	
	}
	m_slcSingleTime.SetPos(m_nSingleTime);
	UpdateData(FALSE);
	*pResult = 0;
}


void CSetTimeout::OnDeltaposSpinAlltime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (pNMUpDown->iDelta == -1)
	{
		if (m_nAllTime < 1000)
		{
			m_nAllTime++;
		}
	}
	else if (pNMUpDown->iDelta == 1)
	{
		if (m_nAllTime > 10)
		{
			m_nAllTime--;
		}	
	}
	m_slcAllTime.SetPos(m_nAllTime);
	UpdateData(FALSE);
	*pResult = 0;
}


void CSetTimeout::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_SINGLETIME) 
	{ 
		m_nSingleTime = m_slcSingleTime.GetPos();
		UpdateData(FALSE);
	}
	if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER_ALLTIME) 
	{ 
		m_nAllTime = m_slcAllTime.GetPos();
		UpdateData(FALSE);
	}
	CBCGPDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSetTimeout::OnEnChangeEditSingletime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	if (m_nSingleTime<10)
	{
		m_nSingleTime = 10;
	}
	if (m_nSingleTime>100)
	{
		m_nSingleTime = 100;
	}
	UpdateData(FALSE);
	m_slcSingleTime.SetPos(m_nSingleTime);
	// TODO:  在此添加控件通知处理程序代码
}


void CSetTimeout::OnEnChangeEditAlltime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CBCGPDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	if (m_nAllTime<10)
	{
		m_nAllTime = 10;
	}
	if (m_nAllTime>1000)
	{
		m_nAllTime = 1000;
	}
	UpdateData(FALSE);
	m_slcAllTime.SetPos(m_nAllTime);
	// TODO:  在此添加控件通知处理程序代码
}
