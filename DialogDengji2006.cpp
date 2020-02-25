// DialogDengji.cpp : implementation file
//

#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "DialogDengji2006.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDengji dialog


CDialogDengji2006::CDialogDengji2006(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDialogDengji2006::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDengji)
	m_bDJ1 = FALSE;
	m_bDJ2 = FALSE;
	m_bDJ3 = FALSE;
	m_bDJ4 = FALSE;
	m_bDJ5 = FALSE;
	m_bDJ6 = FALSE;
	m_bDJ7 = FALSE;
	m_bCheckAll = FALSE;
	m_bNoAll = FALSE;
	//}}AFX_DATA_INIT
	//CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS (CBCGPVisualManagerVS2010));
	EnableVisualManagerStyle(TRUE,TRUE);
}


void CDialogDengji2006::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDengji)
	DDX_Check(pDX, IDC_CHECK1, m_bDJ1);
	DDX_Check(pDX, IDC_CHECK_ALL, m_bCheckAll);
	DDX_Check(pDX, IDC_CHECK7, m_bDJ7);
	DDX_Check(pDX, IDC_CHECK6, m_bDJ6);
	DDX_Check(pDX, IDC_CHECK4, m_bDJ4);
	DDX_Check(pDX, IDC_CHECK3, m_bDJ3);
	DDX_Check(pDX, IDC_CHECK5, m_bDJ5);
	DDX_Check(pDX, IDC_CHECK2, m_bDJ2);
	DDX_Check(pDX, IDC_CHECK_NOALL, m_bNoAll);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_STD, m_labStd);
	DDX_Control(pDX, IDC_STATIC_STDVALUE, m_labStdValue);
	DDX_Control(pDX, IDC_CHECK_ALL, m_cmdCheckAll);
	DDX_Control(pDX, IDC_CHECK_NOALL, m_cmdCheckNoAll);
	DDX_Control(pDX, IDC_CHECK1, m_cmdCheck1);
	DDX_Control(pDX, IDC_CHECK2, m_cmdCheck2);
	DDX_Control(pDX, IDC_CHECK3, m_cmdCheck3);
	DDX_Control(pDX, IDC_CHECK4, m_cmdCheck4);
	DDX_Control(pDX, IDC_CHECK5, m_cmdCheck5);
	DDX_Control(pDX, IDC_CHECK6, m_cmdCheck6);
	DDX_Control(pDX, IDC_CHECK7, m_cmdCheck7);
	DDX_Control(pDX, IDOK, m_cmdOnOK);
	DDX_Control(pDX, IDCANCEL, m_cmdCancel);
}


BEGIN_MESSAGE_MAP(CDialogDengji2006, CBCGPDialog)
	//{{AFX_MSG_MAP(CDialogDengji)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnCheckAll)
	ON_BN_CLICKED(IDC_CHECK_NOALL, OnCheckNoall)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDialogDengji2006::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDengji message handlers

void CDialogDengji2006::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bDJ1&&m_bDJ2&&m_bDJ3
		&&m_bDJ4&&m_bDJ5&&m_bDJ6&&m_bDJ7) 
	{
		m_bCheckAll = TRUE;
	}
	else
	{
		m_bCheckAll = FALSE;
	}
	if (m_bDJ1||m_bDJ2||m_bDJ3
		||m_bDJ4||m_bDJ5||m_bDJ6||m_bDJ7) 
	{
		m_bNoAll = FALSE;
	}
	else
	{
		m_bNoAll = TRUE;
	}
	UpdateData(FALSE);
}

void CDialogDengji2006::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bDJ1&&m_bDJ2&&m_bDJ3
		&&m_bDJ4&&m_bDJ5&&m_bDJ6&&m_bDJ7) 
	{
		m_bCheckAll = TRUE;
	}
	else
	{
		m_bCheckAll = FALSE;
	}
	if (m_bDJ1||m_bDJ2||m_bDJ3
		||m_bDJ4||m_bDJ5||m_bDJ6||m_bDJ7) 
	{
		m_bNoAll = FALSE;
	}
	else
	{
		m_bNoAll = TRUE;
	}
	UpdateData(FALSE);
}

void CDialogDengji2006::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bDJ1&&m_bDJ2&&m_bDJ3
		&&m_bDJ4&&m_bDJ5&&m_bDJ6&&m_bDJ7) 
	{
		m_bCheckAll = TRUE;
	}
	else
	{
		m_bCheckAll = FALSE;
	}
	if (m_bDJ1||m_bDJ2||m_bDJ3
		||m_bDJ4||m_bDJ5||m_bDJ6||m_bDJ7) 
	{
		m_bNoAll = FALSE;
	}
	else
	{
		m_bNoAll = TRUE;
	}
	UpdateData(FALSE);
}

void CDialogDengji2006::OnCheck4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bDJ1&&m_bDJ2&&m_bDJ3
		&&m_bDJ4&&m_bDJ5&&m_bDJ6&&m_bDJ7) 
	{
		m_bCheckAll = TRUE;
	}
	else
	{
		m_bCheckAll = FALSE;
	}
	if (m_bDJ1||m_bDJ2||m_bDJ3
		||m_bDJ4||m_bDJ5||m_bDJ6||m_bDJ7) 
	{
		m_bNoAll = FALSE;
	}
	else
	{
		m_bNoAll = TRUE;
	}
	UpdateData(FALSE);
}

void CDialogDengji2006::OnCheck5() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bDJ1&&m_bDJ2&&m_bDJ3
		&&m_bDJ4&&m_bDJ5&&m_bDJ6&&m_bDJ7) 
	{
		m_bCheckAll = TRUE;
	}
	else
	{
		m_bCheckAll = FALSE;
	}
	if (m_bDJ1||m_bDJ2||m_bDJ3
		||m_bDJ4||m_bDJ5||m_bDJ6||m_bDJ7) 
	{
		m_bNoAll = FALSE;
	}
	else
	{
		m_bNoAll = TRUE;
	}
	UpdateData(FALSE);
}

void CDialogDengji2006::OnCheck6() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bDJ1&&m_bDJ2&&m_bDJ3
		&&m_bDJ4&&m_bDJ5&&m_bDJ6&&m_bDJ7) 
	{
		m_bCheckAll = TRUE;
	}
	else
	{
		m_bCheckAll = FALSE;
	}
	if (m_bDJ1||m_bDJ2||m_bDJ3
		||m_bDJ4||m_bDJ5||m_bDJ6||m_bDJ7) 
	{
		m_bNoAll = FALSE;
	}
	else
	{
		m_bNoAll = TRUE;
	}
	UpdateData(FALSE);
}

void CDialogDengji2006::OnCheck7() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bDJ1&&m_bDJ2&&m_bDJ3
		&&m_bDJ4&&m_bDJ5&&m_bDJ6&&m_bDJ7) 
	{
		m_bCheckAll = TRUE;
	}
	else
	{
		m_bCheckAll = FALSE;
	}
	if (m_bDJ1||m_bDJ2||m_bDJ3
		||m_bDJ4||m_bDJ5||m_bDJ6||m_bDJ7) 
	{
		m_bNoAll = FALSE;
	}
	else
	{
		m_bNoAll = TRUE;
	}
	UpdateData(FALSE);
}


void CDialogDengji2006::OnCheckAll() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bCheckAll)
	{
		m_bDJ1 = TRUE;
		m_bDJ2 = TRUE;
		m_bDJ3 = TRUE;
		m_bDJ4 = TRUE;
		m_bDJ5 = TRUE;
		m_bDJ6 = TRUE;
		m_bDJ7 = TRUE;
		m_bNoAll = FALSE;
	}
	UpdateData(FALSE);
	
}

void CDialogDengji2006::OnCheckNoall() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bNoAll)
	{
		m_bDJ1 = FALSE;
		m_bDJ2 = FALSE;
		m_bDJ3 = FALSE;
		m_bDJ4 = FALSE;
		m_bDJ5 = FALSE;
		m_bDJ6 = FALSE;
		m_bDJ7 = FALSE;
		m_bCheckAll = FALSE;
	}
	UpdateData(FALSE);
}

BOOL CDialogDengji2006::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//BCG Control
	//SetBackgroundImage(IDB_BITMAP_GRADEBACK);

	m_labStd.m_clrText = RGB(255,255,0);
	m_labStdValue.m_clrText = RGB(255,0,0);
	
	m_cmdCheckAll.SetTextColor(RGB (0, 255, 0));
	m_cmdCheckNoAll.SetTextColor(RGB (0, 255, 0));
	m_cmdCheck1.SetTextColor(RGB (255, 255, 255));
	m_cmdCheck2.SetTextColor(RGB (255, 255, 255));
	m_cmdCheck3.SetTextColor(RGB (255, 255, 255));
	m_cmdCheck4.SetTextColor(RGB (255, 255, 255));
	m_cmdCheck5.SetTextColor(RGB (255, 255, 255));
	m_cmdCheck6.SetTextColor(RGB (255, 255, 255));
	m_cmdCheck7.SetTextColor(RGB (255, 255, 255));

	m_cmdOnOK.SetFaceColor(RGB (0, 191, 255));
	m_cmdOnOK.SetMouseCursorHand();
	m_cmdCancel.SetFaceColor(RGB (0, 191, 255));
	m_cmdCancel.SetMouseCursorHand();
	//////////////////////////////////////////////////////////////////////////
	if (m_bDJ1&&m_bDJ2&&m_bDJ3
		&&m_bDJ4&&m_bDJ5&&m_bDJ6&&m_bDJ7) 
	{
		m_bCheckAll = TRUE;
	}
	else
	{
		m_bCheckAll = FALSE;
	}
	if (m_bDJ1||m_bDJ2||m_bDJ3
		||m_bDJ4||m_bDJ5||m_bDJ6||m_bDJ7) 
	{
		m_bNoAll = FALSE;
	}
	else
	{
		m_bNoAll = TRUE;
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogDengji2006::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CBCGPDialog::OnOK();
}
