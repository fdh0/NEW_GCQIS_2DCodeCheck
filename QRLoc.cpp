// QRLoc.cpp : implementation file
//

#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "QRLoc.h"


// CQRLoc dialog

IMPLEMENT_DYNAMIC(CQRLoc, CBCGPDialog)

CQRLoc::CQRLoc(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CQRLoc::IDD, pParent)
	, m_bCheckQRloc(FALSE)
{
	EnableVisualManagerStyle(TRUE,TRUE);

}

CQRLoc::~CQRLoc()
{
}

void CQRLoc::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_QRLOC, m_bCheckQRloc);
	DDX_Control(pDX, IDC_CHECK_QRLOC, m_cmdCheckQrLoc);
}


BEGIN_MESSAGE_MAP(CQRLoc, CBCGPDialog)
	ON_BN_CLICKED(IDC_CHECK_QRLOC, &CQRLoc::OnBnClickedCheckQrloc)
END_MESSAGE_MAP()


// CQRLoc message handlers

void CQRLoc::OnBnClickedCheckQrloc()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}


BOOL CQRLoc::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

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
			for (int i=10;i<=nNum;i++)
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
	// TODO:  在此添加额外的初始化
	m_cmdCheckQrLoc.SetTextColor(RGB (255, 255, 255));
	m_cmdCheckQrLoc.SetMouseCursorHand();



	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
