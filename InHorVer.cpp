// InHorVer.cpp : implementation file
//

#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "InHorVer.h"


// CInHorVer dialog

IMPLEMENT_DYNAMIC(CInHorVer, CDialog)

CInHorVer::CInHorVer(CWnd* pParent /*=NULL*/)
	: CDialog(CInHorVer::IDD, pParent)
	, m_bCheckHorVer(FALSE)
{
	m_nDirect = 1;
	m_nArea = 1.0;

}

CInHorVer::~CInHorVer()
{
}

void CInHorVer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_HORVER, m_bCheckHorVer);
	DDX_Control(pDX, IDC_STATIC_DIRECT, m_LabDirect);
	DDX_Control(pDX, IDC_COMBO_DIRECT, m_ComDirect);
	DDX_Text(pDX, IDC_EDIT_AREA, m_nArea);
}


BEGIN_MESSAGE_MAP(CInHorVer, CDialog)
	ON_BN_CLICKED(IDC_CHECK_HORVER, &CInHorVer::OnBnClickedCheckHorver)
END_MESSAGE_MAP()


// CInHorVer message handlers

void CInHorVer::OnBnClickedCheckHorver()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}
BOOL CInHorVer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComDirect.AddString("横向");
	m_ComDirect.AddString("纵向");
	m_ComDirect.SetCurSel(m_nDirect);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
