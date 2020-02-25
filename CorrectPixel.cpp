// CorrectPixel.cpp : implementation file
//

#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "CorrectPixel.h"


// CCorrectPixel dialog

IMPLEMENT_DYNAMIC(CCorrectPixel, CDialog)

CCorrectPixel::CCorrectPixel(CWnd* pParent /*=NULL*/)
	: CDialog(CCorrectPixel::IDD, pParent)
	,m_bAlignCorrected(FALSE)
	,m_nCorrectPixels(0)
{

}

CCorrectPixel::~CCorrectPixel()
{
}

void CCorrectPixel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ALIGNCORRECT, m_bAlignCorrected);
	DDX_Text(pDX, IDC_EDIT_CORRPIXEL, m_nCorrectPixels);

}


BEGIN_MESSAGE_MAP(CCorrectPixel, CDialog)
	ON_BN_CLICKED(IDC_CHECK_ALIGNCORRECT, &CCorrectPixel::OnBnClickedCheckAligncorrect)
	ON_EN_CHANGE(IDC_EDIT_CORRPIXEL, &CCorrectPixel::OnEnChangeEditCorrpixel)
END_MESSAGE_MAP()


// CCorrectPixel message handlers

void CCorrectPixel::OnBnClickedCheckAligncorrect()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bAlignCorrected)
	{
		GetDlgItem(IDC_EDIT_CORRPIXEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CORRPIXEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_HIDE);
	}
}

void CCorrectPixel::OnEnChangeEditCorrpixel()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
}

BOOL CCorrectPixel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if (m_bAlignCorrected)
	{
		GetDlgItem(IDC_EDIT_CORRPIXEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_EDIT_CORRPIXEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
