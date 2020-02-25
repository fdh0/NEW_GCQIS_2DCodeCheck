// CodeModelDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "OcrProDll.h"
#include "resource.h"
#include "CodeModelDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeModelDlg dialog


CCodeModelDlg::CCodeModelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCodeModelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCodeModelDlg)
	m_nModiCodeDot = 0;
	//}}AFX_DATA_INIT
	m_nIndex=0;
	m_nCurCol=0;
	m_nCurRow=0;
	m_nWidth=1;
	m_nHeight=1;
	m_bShowOrgDot=TRUE;
	m_bTempMody=FALSE;
	//////////////////
	m_bLeftTmpPos = TRUE;
	m_bRightTmpPos = FALSE;

	bLeftDown = FALSE;
	m_nCurSelDotIndex = -1;
	//  [12/17/2007]
	memset(m_sCurCodeMoreTmp,0,sizeof(MoreInkDot)*CODEALLNUMBER);
	memset(m_bChangedOCRMode,0,sizeof(BOOL)*CODEALLNUMBER);
	memset(m_bChangedOCVMode,0,sizeof(BOOL)*CODEALLNUMBER);
}


void CCodeModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCodeModelDlg)
	DDX_Radio(pDX, IDC_RADIO_OCRDOT, m_nModiCodeDot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCodeModelDlg, CDialog)
	//{{AFX_MSG_MAP(CCodeModelDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_NO1, OnCheckCodeNo)
	ON_BN_CLICKED(IDC_RADIO_OCRDOT, OnRadioOcrdot)
	ON_BN_CLICKED(IDC_RADIO_OCVDOT, OnRadioOcvdot)
	ON_BN_CLICKED(IDC_BUTTON_MODYFY, OnButtonModyfy)
	ON_BN_CLICKED(IDC_BUTTON_SAVEASDOT, OnButtonSaveasdot)
	ON_BN_CLICKED(IDC_BTN_CURDOT, OnBtnCurdot)
	ON_BN_CLICKED(IDC_BTN_TEMPMODY, OnBtnTempmody)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_ADD, OnMenuAdd)
	ON_COMMAND(ID_MENU_DELETE, OnMenuDelete)
	ON_COMMAND(IDM_MENU_SETDIR_HORZ, OnMenuSetdirHorz)
	ON_COMMAND(IDM_MENU_SETDIR_VERT, OnMenuSetdirVert)
	ON_COMMAND(IDM_MENU_SETDIR_TITLE45, OnMenuSetdirTitle45)
	ON_COMMAND(IDM_MENU_SETDIR_TITLE135, OnMenuSetdirTitle135)
	ON_BN_CLICKED(IDC_RADIO_MOREINK_SET, OnRadioMoreinkSet)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_MENU_NEW_REGION, OnMenuNewRegion)
	ON_COMMAND(ID_MENU_DEL_REGION, OnMenuDelRegion)
	ON_BN_CLICKED(IDC_CHECK_NO2, OnCheckCodeNo)
	ON_BN_CLICKED(IDC_CHECK_NO3, OnCheckCodeNo)
	ON_BN_CLICKED(IDC_CHECK_NO4, OnCheckCodeNo)
	ON_BN_CLICKED(IDC_CHECK_NO5, OnCheckCodeNo)
	ON_BN_CLICKED(IDC_CHECK_NO6, OnCheckCodeNo)
	ON_BN_CLICKED(IDC_CHECK_NO7, OnCheckCodeNo)
	ON_BN_CLICKED(IDC_CHECK_NO8, OnCheckCodeNo)
	ON_BN_CLICKED(IDC_CHECK_NO9, OnCheckCodeNo)
	ON_BN_CLICKED(IDC_CHECK_NO10, OnCheckCodeNo)
	ON_COMMAND(IDM_MENU_SETDIR_TITLEMO, OnMenuSetdirTitlemo)
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(IDOK, &CCodeModelDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_UPDATEDOT, &CCodeModelDlg::OnBnClickedButtonUpdatedot)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeModelDlg message handlers

BOOL CCodeModelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	UINT i;
	//获得当前号码识别的号码个数
	m_nCodeNumber = m_strCurCode.GetLength();
	//设置号码显示按钮
	if(m_nCodeNumber == m_OcrProExt->g_sCurNoteTypeInfo.uStandardCodeNum)
	{
		UINT nID;
		CString tmp;
		for(i = 0;i<m_nCodeNumber;i++)
		{
			nID = IDC_CHECK_NO1+i;
			tmp.Format ("%c",m_strCurCode.GetAt (i));
			////////////////////////////
			GetDlgItem(nID)->ShowWindow (SW_RESTORE);
			////////////////////////////
			SetDlgItemText(nID,tmp);
		}
	}
	//取得图像显示的区域坐标
	GetDlgItemRect(IDC_STATIC_IMAGERECT,this,&m_rcImage);
	GetDlgItemRect(IDC_STATIC_IMAGERECTC,this,&m_rcOrgImg);	
	m_rcChoose.SetRectEmpty();
	/////////更新号码模板
	UpdateCodeDot();
	///////////////////初始化位图显示结构体
	m_ptrBmpInfo = (LPBITMAPINFO)m_pbTmpData;
	
	m_ptrBmpInfo->bmiHeader.biBitCount = 8;
	m_ptrBmpInfo->bmiHeader.biClrImportant = 0;
	m_ptrBmpInfo->bmiHeader.biClrUsed = 256;
	m_ptrBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_ptrBmpInfo->bmiHeader.biHeight = 64;
	m_ptrBmpInfo->bmiHeader.biPlanes = 1;
	m_ptrBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_ptrBmpInfo->bmiHeader.biSizeImage = 0;
	m_ptrBmpInfo->bmiHeader.biWidth = 64;
	m_ptrBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_ptrBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	for (int j=0; j<256; j++)
	{
		m_ptrBmpInfo->bmiColors[j].rgbBlue = (BYTE)j;
		m_ptrBmpInfo->bmiColors[j].rgbGreen = (BYTE)j;
		m_ptrBmpInfo->bmiColors[j].rgbRed = (BYTE)j;
		m_ptrBmpInfo->bmiColors[j].rgbReserved = 0;
	}
	CRect rcCode = m_OcrProExt->g_sSepOutputPara.lpRectCode[0];
	////////////////获得图像显示区域
	m_rcDispImage = GetDisplayRect(rcCode.Size(),m_rcImage,1);
	m_rcDispOrgImage = GetDisplayRect(rcCode.Size(),m_rcOrgImg,1);
	///////////////////初始化OCV模板
	InitTmpOCVTmpData();
	///////////////////
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCodeModelDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(m_nModiCodeDot == 0)
	{
		DrawCodeOCRDot(&dc);//画OCR模板
	}
	else
	{
		DrawCodeOCVDot(&dc);//画OCV模板
	}
}

void CCodeModelDlg::OnCheckCodeNo() 
{
	UINT nID = GetFocus()->GetDlgCtrlID();
	m_nIndex = nID - IDC_CHECK_NO1;
	///////////////////////////
	if(m_nIndex >= (int)m_nCodeNumber)
	{
		return;
	}
	///////////////////////////
	UpdateCodeDot();
	///////////////////////////
	m_bTempMody = FALSE;
	//////////////////////////////
	UpdateData(FALSE);
	char chCur = m_strCurCode.GetAt (m_nIndex);
	if(m_nModiCodeDot == 0)
	{
		if(chCur == '1'||chCur == 'I')
		{
			GetDlgItem (IDC_EDIT_GRIDVALUE)->EnableWindow (FALSE);
			GetDlgItem (IDC_BUTTON_MODYFY)->EnableWindow (FALSE);	
			GetDlgItem (IDC_BTN_TEMPMODY)->EnableWindow (FALSE);
			GetDlgItem (IDC_BUTTON_SAVEASDOT)->EnableWindow (FALSE);
			GetDlgItem (IDC_BTN_CURDOT)->EnableWindow (FALSE);
		}
		else
		{
			GetDlgItem (IDC_BUTTON_MODYFY)->EnableWindow (TRUE);	
			GetDlgItem (IDC_BTN_TEMPMODY)->EnableWindow (TRUE);
			GetDlgItem (IDC_BUTTON_SAVEASDOT)->EnableWindow (TRUE);
			GetDlgItem (IDC_BTN_CURDOT)->EnableWindow (TRUE);
		}
	}
	else
	{
//		GetDlgItem (IDC_BUTTON_MODYFY)->EnableWindow (TRUE);	
//		GetDlgItem (IDC_BTN_TEMPMODY)->EnableWindow (TRUE);
		GetDlgItem (IDC_BUTTON_SAVEASDOT)->EnableWindow (TRUE);
//		GetDlgItem (IDC_BTN_CURDOT)->EnableWindow (TRUE);
	}
	////////////////////
	// 得到各节点区域
	GetNodeRectInfo();
	//////////////////////////////
	InvalidateRect(&m_rcImage);	
	InvalidateRect(&m_rcOrgImg);	
}

void CCodeModelDlg::UpdateCodeDot()
{
	int nCode = 0;
	char chTmp = m_strCurCode.GetAt(m_nIndex);
	int nSizeDigit = NUMBER_COL*NUMBER_ROW;
	int nSizeChar = CHAR_COL*CHAR_ROW;


	if( chTmp>='0'&&chTmp <='9')
	{
		nCode = (int)(chTmp - '0');
		bytOrgTmpCode = &m_OcrProExt->g_sOcrInputPara.lpbDigitCode[nCode * nSizeDigit];
		m_nWidth = NUMBER_COL;
		m_nHeight = NUMBER_ROW;
			//  [10/22/2007]
		m_nCurCodeIndex = nCode;
	}
	if(chTmp>='A'&&chTmp <='Z')
	{
		nCode = (int)(chTmp - 'A');
		bytOrgTmpCode = &m_OcrProExt->g_sOcrInputPara.lpbCharCode[nCode * nSizeChar];
		m_nWidth = CHAR_COL;
		m_nHeight = CHAR_ROW;
			//  [10/22/2007]
		m_nCurCodeIndex = nCode + DIGIT_NUM;
	}
	//  [11/28/2007]
	bytTmpCode = m_OcrProExt->g_sOcrOutputPara.lpbCurCodeDot[m_nIndex];
	////////////
	m_nStep = min(m_rcImage.Width()/m_nWidth,m_rcImage.Height()/m_nHeight);	
	////////////
	memset(bytMargTmpCode,0,sizeof(int)*m_nWidth*m_nHeight);	

	/////////////////
	int i,j;
	if(m_bShowOrgDot)
	{
		for(i=0;i<m_nHeight;i++)
		{
			for(j=0;j<m_nWidth;j++)
			{
				bytMargTmpCode[i*m_nWidth+j] = bytOrgTmpCode[i*m_nWidth+j];
			}
		}	
	}
	else
	{
		//////////////////////////////////
		int tmp;
		for(i=0;i<m_nHeight;i++)
		{
			for(j=0;j<m_nWidth;j++)
			{
				tmp = bytTmpCode[i*m_nWidth+j] - bytOrgTmpCode[i*m_nWidth+j];
				
				if(bytTmpCode[i*m_nWidth+j]>=BLACK_VALUE&&bytOrgTmpCode[i*m_nWidth+j]<BLACK_VALUE-1)
				{
					bytMargTmpCode[i*m_nWidth+j] = tmp*tmp;
				}
				if(bytTmpCode[i*m_nWidth+j]<BLACK_VALUE-1&&bytOrgTmpCode[i*m_nWidth+j]>=BLACK_VALUE)
				{
					bytMargTmpCode[i*m_nWidth+j] = -tmp*tmp;
				}
				
			}
		}
		//////////////////////////////////
	}
}

void CCodeModelDlg::OnRadioOcrdot() 
{
	// TODO: Add your control notification handler code here
	m_nModiCodeDot = 0;
	////////////
	InvalidateRect(&m_rcOrgImg);
	InvalidateRect(&m_rcImage);
}

void CCodeModelDlg::OnRadioOcvdot() 
{
	// TODO: Add your control notification handler code here
	m_nModiCodeDot = 1;	

	GetDlgItem (IDC_BUTTON_SAVEASDOT)->EnableWindow (TRUE);
	
//	InitTmpOCVTmpData();
	GetNodeRectInfo();
	
	InvalidateRect(&m_rcOrgImg);
	InvalidateRect(&m_rcImage);
}

void CCodeModelDlg::OnButtonModyfy() 
{
// TODO: Add your control notification handler code here
	char sTmp[16];
	GetDlgItemText(IDC_EDIT_GRIDVALUE,sTmp,sizeof(sTmp));
	if(m_nModiCodeDot == 0)
	{
		m_nGridVal = atoi(sTmp);
		if(m_bLeftTmpPos)
		{
			bytTmpCode[m_nCurRow*m_nWidth+m_nCurCol] = m_nGridVal;	
			InvalidateRect(&m_rcImage);	
		}
		if(m_bRightTmpPos)
		{
			bytMargTmpCode[m_nCurRow*m_nWidth+m_nCurCol] = m_nGridVal;
			InvalidateRect(&m_rcOrgImg);	
		}
	}
	//  [12/14/2007]
	if(m_nModiCodeDot == 2)
	{
		if(m_nCurSelRegionIndex != -1)
		{
			m_sCurCodeRegn[m_nCurSelRegionIndex].m_fMoreInkWeight = (float)atof(sTmp);
		}
	}
}

void CCodeModelDlg::OnButtonSaveasdot() 
{
	int ret=IDNO;
	CString strDefName;
	int i,j;
	BYTE tmp;
	char chCur = m_strCurCode.GetAt (m_nIndex);

	if(m_nModiCodeDot == 0)
	{
		if(chCur=='1'||chCur=='I')
		{
			return;
		}
		if(m_bTempMody)
		{
			ret = AfxMessageBox("对模板已作部分修正，保存修正后模板请选择“是”，\n保存当前实际模板请选择“否”，不作任何保存请选择“取消”。",MB_YESNOCANCEL,0);
			if(ret == IDCANCEL)
			{
				return;
			}
		}
		strDefName.Format ("NumberH%c.ndt",chCur);
		
		CFileDialog dlg(FALSE,"*.ndt",strDefName, 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
			"模板文件 (*.ndt)|*.ndt|All Files (*.*)|*.*||", this);
		dlg.m_ofn.lpstrTitle = "另存为";
		///////////////////////////
		
		dlg.m_ofn.lpstrInitialDir = (LPCTSTR)m_strCurAppDotPath;
		CString strName;	
		if (dlg.DoModal() == IDOK)
		{
			strName=dlg.GetPathName();
			//  [12/17/2002]
			if(strName.Find ('.')==-1)
			{
				strName+=".ndt";
			}
		
			////////
			CFile file;
			if(file.Open(strName,CFile::modeReadWrite)==0)
			{
				file.Open (strName,CFile::modeCreate|CFile::modeReadWrite);
			}
			//  [10/21/2002]
			s_CodeTempInfo dot;
			dot.cNumber = m_strCurCode.GetAt (m_nIndex);
			dot.nGridHeight = m_nHeight;
			dot.nGridWidth = m_nWidth;
			int m_offset = sizeof(s_CodeTempInfo);
			BOOL bFlag = FALSE;//默认对齐
			//  [10/21/2002]
			if (m_offset==9)
			{
				bFlag = TRUE;
			}
			if (!bFlag)
			{
				file.Write(&dot,m_offset);
			}
			else
			{
				file.Write(&dot.cNumber,sizeof(char));
				file.Seek(4,CFile::begin);
				file.Write(&dot.nGridWidth,sizeof(int));
				file.Write(&dot.nGridHeight,sizeof(int));
			}
			
			
			////////////////////////
			if(ret == IDYES)
			{
				for(i = 0;i<m_nHeight;i++)
				{
					for(j = 0;j<m_nWidth;j++)
					{
						tmp = (BYTE)bytMargTmpCode[i*m_nWidth+j];
						file.Write(&tmp,sizeof(BYTE));
					}
				}
			}
			if(ret == IDNO)
			{
				file.Write(bytTmpCode,m_nWidth*m_nHeight);
			}
			////////////
			file.Close();
			if (chCur>='0'&&chCur<='9')
			{
				int nCode = chCur-'0';
				m_bChangedOCRMode[nCode] = TRUE;
			}
			else
			{
				int nCode = chCur-'A'+10;
				m_bChangedOCRMode[nCode] = TRUE;
			}
		}
	}
	else//保存OCV模板
	{
		int i;
		memcpy(&m_OcrProExt->g_sOcrInputPara.lpLeaklineTmp[m_nCurCodeIndex],&m_sCurCodeOcvTmp[m_nCurCodeIndex],sizeof(LeakLineDot));
		//  [10/24/2007] 写到文件
		strDefName.Format ("LeakLine%c.qdt",chCur);
		CFileDialog dlg(FALSE,"*.qdt",strDefName, 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
			"模板文件 (*.ndt)|*.qdt|All Files (*.*)|*.*||", this);
		dlg.m_ofn.lpstrTitle = "另存为";
		///////////////////////////
		dlg.m_ofn.lpstrInitialDir = (LPCTSTR)m_strCurAppOCVPath;
		CString strName;	
		if (dlg.DoModal() == IDOK)
		{
			strName = dlg.GetPathName();
			//  [12/17/2002]
			if(strName.Find ('.')==-1)
			{
				strName += ".qdt";
			}
			
			i = chCur-'0'>10?chCur-'A'+10:chCur-'0';
			
			CFile file;
			if (i<10)
			{
				chCur = (char)(i + '0');
			}
			else
			{
				chCur = (char)(i - 10 + 'A');
			}
			//  [12/14/2007]
			if(m_nModiCodeDot == 2)
			{
				UpdateCodeMoreInkRegion();
				m_OcrProExt->g_sOcrInputPara.lpMoreInkTmp[m_nCurCodeIndex] = m_sCurCodeMoreTmp[m_nCurCodeIndex];
			}
			if(file.Open(strName,CFile::modeCreate|CFile::modeReadWrite,0))
			{
				file.Write(&chCur,1);
				int offSet = sizeof(LeakLineDot);
				BOOL bFlag = FALSE;
				if (offSet==604)
				{
					bFlag = TRUE;
				}
				if (!bFlag)
				{
					file.Write(&m_OcrProExt->g_sOcrInputPara.lpLeaklineTmp[m_nCurCodeIndex],offSet);
				}
				else
				{
					file.Write(&m_OcrProExt->g_sOcrInputPara.lpLeaklineTmp[m_nCurCodeIndex].nPointNum,sizeof(int));
					int a = 0;
					file.Write(&a,sizeof(int));
					file.Write(m_OcrProExt->g_sOcrInputPara.lpLeaklineTmp[m_nCurCodeIndex].Point,MaxPointNum*sizeof(LeakLinePoint));
					file.Write(m_OcrProExt->g_sOcrInputPara.lpLeaklineTmp[m_nCurCodeIndex].dir,MaxPointNum*sizeof(int));
				}
				
				//  [12/14/2007]
				file.Write(&m_OcrProExt->g_sOcrInputPara.lpMoreInkTmp[m_nCurCodeIndex],sizeof(MoreInkDot));
				file.Close();
			}
			if (chCur>='0'&&chCur<='9')
			{
				int nCode = chCur-'0';
				m_bChangedOCVMode[nCode] = TRUE;
			}
			else
			{
				int nCode = chCur-'A'+10;
				m_bChangedOCVMode[nCode] = TRUE;
			}
		}
	}
}

void CCodeModelDlg::OnBtnCurdot() 
{
	m_bShowOrgDot = !m_bShowOrgDot;
	if(m_nModiCodeDot == 0)
	{	
		if(m_bShowOrgDot)
		{
			GetDlgItem (IDC_BTN_CURDOT)->SetWindowText("原始模板");
			m_bTempMody = FALSE;
		}
		else
		{
			GetDlgItem (IDC_BTN_CURDOT)->SetWindowText("匹配差值");
		}
	}
	UpdateCodeDot();
	//////////
	InvalidateRect(m_rcOrgImg);
}

void CCodeModelDlg::OnBtnTempmody() 
{
	int i,j,tmp;
	if(m_nModiCodeDot == 0)
	{
		for(i = 0;i<m_nHeight;i++)
		{
			for(j = 0;j<m_nWidth;j++)
			{
				tmp = (bytTmpCode[i*m_nWidth+j] + bytOrgTmpCode[i*m_nWidth+j])/2;
				bytMargTmpCode[i*m_nWidth+j] = tmp;
			}
		}
		m_bTempMody = TRUE;
		///////////////////
		GetDlgItem (IDC_EDIT_GRIDVALUE)->EnableWindow (TRUE);
		GetDlgItem (IDC_BUTTON_MODYFY)->EnableWindow (TRUE);
	}
	/////////////////////
	InvalidateRect(m_rcOrgImg);
}

void CCodeModelDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nModiCodeDot == 0)
	{
		if(m_rcImage.PtInRect(point))
		{
			CPoint pt(point.x-m_rcImage.left,point.y-m_rcImage.top);
			m_nCurCol = min(pt.x/m_nStep,m_nWidth-1);
			m_nCurRow = min(pt.y/m_nStep,m_nHeight-1);
			///////////////////////
			BYTE bCurData = bytTmpCode[m_nCurRow*m_nWidth+m_nCurCol];
			///////
			//  [12/14/2007]
			CString str;
			str.Format("%d",bCurData);
			SetDlgItemText(IDC_EDIT_GRIDVALUE,str);
			///////////////
			m_rcChoose.left = m_nCurCol*m_nStep;
			m_rcChoose.right = (m_nCurCol+1)*m_nStep;
			m_rcChoose.top = m_nCurRow*m_nStep;
			m_rcChoose.bottom = (m_nCurRow+1)*m_nStep;
			///////
			m_rcChoose.OffsetRect(m_rcImage.TopLeft());
			
			GetDlgItem (IDC_EDIT_GRIDVALUE)->EnableWindow (TRUE);
			GetDlgItem (IDC_BUTTON_MODYFY)->EnableWindow (TRUE);
			////////////////////////
			GotoDlgCtrl(GetDlgItem (IDC_EDIT_GRIDVALUE));	
			////////////////////////
			m_bLeftTmpPos = TRUE;
			m_bRightTmpPos = FALSE;
			////////////////////////
			InvalidateRect(&m_rcImage);
		}
		if(m_rcOrgImg.PtInRect(point))
		{
			CPoint pt(point.x-m_rcOrgImg.left,point.y-m_rcImage.top);
			m_nCurCol = min(pt.x/m_nStep,m_nWidth-1);
			m_nCurRow = min(pt.y/m_nStep,m_nHeight-1);
			///////////////////////
			int bCurData = bytMargTmpCode[m_nCurRow*m_nWidth+m_nCurCol];
			///////
			CString str;
			str.Format("%d",bCurData);
			SetDlgItemText(IDC_EDIT_GRIDVALUE,str);
			///////////////
			m_rcChoose.left = m_nCurCol*m_nStep;
			m_rcChoose.right = (m_nCurCol+1)*m_nStep;
			m_rcChoose.top = m_nCurRow*m_nStep;
			m_rcChoose.bottom = (m_nCurRow+1)*m_nStep;
			///////
			m_rcChoose.OffsetRect(m_rcOrgImg.TopLeft());
			////////////////////////////////
			GetDlgItem (IDC_EDIT_GRIDVALUE)->EnableWindow (m_bTempMody);
			GetDlgItem (IDC_BUTTON_MODYFY)->EnableWindow (m_bTempMody);
			if(m_bTempMody)
			{
				GotoDlgCtrl(GetDlgItem (IDC_EDIT_GRIDVALUE));
			}
			////////////////////////////////
			m_bLeftTmpPos = FALSE;
			m_bRightTmpPos = TRUE;
			////////////////////////////////
			InvalidateRect(&m_rcOrgImg);
		}	
	}
	else
	{	
		CClientDC dc(this);
		bLeftDown = TRUE;
		m_nCurSelDotIndex = -1;
		///////////////////////////////
		int i,j;
		if(m_nModiCodeDot == 1)
		{
			//////////
			for(i = 0;i<m_nRectSelPtNum;i++)
			{
				if(m_lpRectDot[i].PtInRect(point))
				{
					m_nCurSelDotIndex = i;
					
					if(i == 0)
					{
						ptPre1 = m_lpRectDot[0].CenterPoint();
						ptCur1 = m_lpRectDot[0].CenterPoint();
						/////////
						ptPre2 = m_lpRectDot[1].CenterPoint();
						ptCur2 = m_lpRectDot[1].CenterPoint();
						
					}
					else
					{
						ptPre1 = m_lpRectDot[i-1].CenterPoint();
						ptCur1 = m_lpRectDot[i-1].CenterPoint();
						/////////////////
						if(i < m_nRectSelPtNum-1)
						{
							ptPre2 = m_lpRectDot[i+1].CenterPoint();
							ptCur2 = m_lpRectDot[i+1].CenterPoint();
						}
						else
						{
							ptPre2 = m_lpRectDot[i].CenterPoint();
							ptCur2 = m_lpRectDot[i].CenterPoint();
						}
					}
					break;
				}
			}
		}
		if(m_nModiCodeDot == 2)
		{
			//  [12/12/2007]首先判断当前选择的区域
			CRect  rcTmp;
			BOOL	bFindSel = FALSE;
			for(i = 0;i<m_nCurMorInkRectNum;i++)
			{
				bFindSel = FALSE;
				for(j = 0;j<m_sCurCodeRegn[i].nRegionDotNum;j++)
				{
					rcTmp = GetNodeRect(m_sCurCodeRegn[i].ptRegionDot[j]);
					if(rcTmp.PtInRect(point))
					{
						bFindSel = TRUE;
						m_nCurSelDotIndex = j;
						break;
					}
				}
				if(bFindSel)
				{
					m_nCurSelRegionIndex = i;
					break;
				}
			}
			if(bFindSel)
			{
				if(m_nCurSelDotIndex == 0)
				{
					ptPre1 = m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[m_sCurCodeRegn[m_nCurSelRegionIndex].nRegionDotNum-1];
					ptCur1 = ptPre1;
					/////////
					ptPre2 = m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[1];
					ptCur2 = ptPre2;
				}
				else
				{
					ptPre1 = m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[m_nCurSelDotIndex-1];
					ptCur1 = ptPre1;
					/////////////////
					if(m_nCurSelDotIndex < m_sCurCodeRegn[m_nCurSelRegionIndex].nRegionDotNum-1)
					{
						ptPre2 = m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[m_nCurSelDotIndex+1];
						ptCur2 = ptPre2;
					}
					else
					{	
						ptPre1 = m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[0];
						ptCur1 = ptPre1;
						ptPre2 = m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[m_nCurSelDotIndex-1];
						ptCur2 = ptPre2;
					}
				}
			}
		}
		InvalidateRect(&m_rcDispOrgImage);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CCodeModelDlg::DrawCodeOCRDot(CDC *pDC)
{
	int i,j;
	CRect rc;
	CString str;

	for(i = 0;i<m_nHeight;i++)
	{
		for(j = 0;j<m_nWidth;j++)
		{
			rc = CRect(j*m_nStep,i*m_nStep,(j+1)*m_nStep,(i+1)*m_nStep);
			rc.OffsetRect(m_rcImage.TopLeft());
			if(bytTmpCode[i*m_nWidth+j]>=5)
			{
				pDC->FillRect(&rc,&CBrush(RGB(0,0,0)));
			}
			else
			{
				pDC->FillRect(&rc,&CBrush(RGB(255,255,255)));
			}
		}
	}
	for(i = 0;i<m_nHeight;i++)
	{
		for(j = 0;j<m_nWidth;j++)
		{
			rc = CRect(j*m_nStep,i*m_nStep,(j+1)*m_nStep,(i+1)*m_nStep);
			rc.OffsetRect(m_rcOrgImg.TopLeft());
			if(m_bShowOrgDot)
			{
				if(bytMargTmpCode[i*m_nWidth+j]>=5)
				{
					pDC->FillRect(&rc,&CBrush(RGB(0,0,0)));
				}
				else
				{
					pDC->FillRect(&rc,&CBrush(RGB(255,255,255)));
				}
			}
			else
			{
				if(bytMargTmpCode[i*m_nWidth+j]>5)
				{
					pDC->FillRect(&rc,&CBrush(RGB(255,0,0)));
				}
				else if(bytMargTmpCode[i*m_nWidth+j]<-4)
				{
					pDC->FillRect(&rc,&CBrush(RGB(0,255,0)));
				}
				else
				{
					pDC->FillRect(&rc,&CBrush(RGB(255,255,255)));
				}
			}
		}
	}
	//////////////////
	CPen pen(PS_SOLID,1,RGB(255,0,0)),*oldPen;
	oldPen = pDC->SelectObject(&pen);
	for(i = 0;i<=m_nHeight;i++)
	{
		pDC->MoveTo(m_rcImage.left,m_rcImage.top+i*m_nStep);
		pDC->LineTo(m_nWidth*m_nStep+m_rcImage.left,m_rcImage.top+i*m_nStep);
	}
	for(i = 0;i<=m_nWidth;i++)
	{
		pDC->MoveTo(m_rcImage.left+i*m_nStep,m_rcImage.top);
		pDC->LineTo(m_rcImage.left+i*m_nStep,m_nHeight*m_nStep+m_rcImage.top);
	}
	//////////////////////////////
	for(i = 0;i<=m_nHeight;i++)
	{
		pDC->MoveTo(m_rcOrgImg.left,m_rcOrgImg.top+i*m_nStep);
		pDC->LineTo(m_nWidth*m_nStep+m_rcOrgImg.left,m_rcOrgImg.top+i*m_nStep);
	}
	for(i = 0;i<=m_nWidth;i++)
	{
		pDC->MoveTo(m_rcOrgImg.left+i*m_nStep,m_rcOrgImg.top);
		pDC->LineTo(m_rcOrgImg.left+i*m_nStep,m_nHeight*m_nStep+m_rcOrgImg.top);
	}
	///////////////////////////////
	pDC->SelectObject(oldPen);
	////////
	if(m_rcChoose.IsRectEmpty())
	{
		m_rcChoose = CRect(0,0,m_nStep,m_nStep);
		m_rcChoose.OffsetRect(m_rcImage.TopLeft());
	}	
	pDC->FrameRect(&m_rcChoose,&CBrush(RGB(0,0,255)));
	str.Format("%d,%d",m_nCurRow,m_nCurCol);
	pDC->SetBkMode(1);
	pDC->SetTextColor(RGB(255,0,0));
	pDC->DrawText(str,&m_rcChoose,DT_CENTER|DT_VCENTER);
}

void CCodeModelDlg::DrawCodeOCVDot(CDC *pDC)
{
	int thresh = m_OcrProExt->g_sSepOutputPara.lpbSepThresh[m_nIndex];
	CRect rcCode = m_OcrProExt->g_sSepOutputPara.lpRectCode[m_nIndex];
	////////////////
	//复制指定区域的图像到图像数据区
	int i,j;
	int nCodeWid = rcCode.Width();
	int nCodeHei = rcCode.Height();
	int nScanLineWid =  WIDTHBYTES(nCodeWid *8);  
	for(i = 0;i<nCodeHei;i++)
	{
		for(j = 0;j<nCodeWid;j++)
		{
			m_pbCodeData[i*nScanLineWid + j] = (m_lpImageData[(i + rcCode.top)*m_szCurImage.cx + j + rcCode.left] > thresh) ? 255:0;
			////////
		}
	}
	//////////////
	m_ptrBmpInfo->bmiHeader.biWidth = nCodeWid;
	m_ptrBmpInfo->bmiHeader.biHeight = nCodeHei;
	/////////////////
	::SetStretchBltMode(pDC->m_hDC,COLORONCOLOR);
	::StretchDIBits(pDC->GetSafeHdc(), 
		m_rcDispImage.left, m_rcDispImage.top, m_rcDispImage.Width(),m_rcDispImage.Height(),
		0, 0, nCodeWid, nCodeHei,
		m_pbCodeData, m_ptrBmpInfo, DIB_RGB_COLORS, SRCCOPY);
	//
	::StretchDIBits(pDC->GetSafeHdc(), 
		m_rcDispOrgImage.left, m_rcDispOrgImage.top, m_rcDispOrgImage.Width(),m_rcDispOrgImage.Height(),
		0, 0, nCodeWid, nCodeHei,
		m_pbCodeData, m_ptrBmpInfo, DIB_RGB_COLORS, SRCCOPY);
	///////////////////////////////////
	// 显示节点
	if(m_nModiCodeDot == 1)
	{
		LeakLineDot dotTmp = m_sCurCodeOcvTmp[m_nCurCodeIndex];
		/////////
		CPen pen(PS_SOLID,1,RGB(255,0,0)),*oldPen;
		oldPen = pDC->SelectObject(&pen);
		CString sDotNum;
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,255,0));
		int x,y;
		for(i = 0;i<dotTmp.nPointNum - 1;i ++)
		{
			if(dotTmp.dir[i] != 9)
			{	
				pDC->MoveTo(m_lptTmpNode[i]);
				pDC->FillRect(m_lpRectDot[i],&CBrush(RGB(0,0,255)));
				
				pDC->LineTo(m_lptTmpNode[i+1]);
				pDC->FillRect(m_lpRectDot[i+1],&CBrush(RGB(0,0,255)));
				/////////
			}
			x = m_lptTmpNode[i].x + 5;
			y = m_lptTmpNode[i].y;
			if(x > m_rcDispOrgImage.right - 20)
			{
				x = m_lptTmpNode[i].x - 20;
				y = m_lptTmpNode[i].y + 5;
			}
			if(y > m_rcDispOrgImage.bottom - 20)
			{
				y = m_lptTmpNode[i].y - 16;
			}
			sDotNum.Format("%d,%d",i+1,dotTmp.dir[i]);
			pDC->TextOut(x,y,sDotNum);
		}
		x = m_lptTmpNode[dotTmp.nPointNum - 1].x + 5;
		y = m_lptTmpNode[dotTmp.nPointNum - 1].y;
		if(x > m_rcDispOrgImage.right - 20)
		{
			x = m_lptTmpNode[dotTmp.nPointNum - 1].x - 20;
			y = m_lptTmpNode[dotTmp.nPointNum - 1].y + 4;
		}
		if(y > m_rcDispOrgImage.bottom - 8)
		{
			y = m_lptTmpNode[dotTmp.nPointNum - 1].y - 8;
		}
		sDotNum.Format("%d,%d",dotTmp.nPointNum,dotTmp.dir[dotTmp.nPointNum - 1]);
		pDC->TextOut(x,y,sDotNum);
		
		pDC->SelectObject(oldPen);
		
		if(m_nCurSelDotIndex != -1)
		{
			pDC->FillRect(m_lpRectDot[m_nCurSelDotIndex],&CBrush(RGB(255,0,255)));
		}
	}
	else
	{
		/////////
		if(m_nCurMorInkRectNum == 0)
		{
			return;
		}
		CPen pen(PS_SOLID,1,RGB(255,0,0)),*oldPen;//penSel(PS_DASHDOT,1,RGB(255,0,0));
		oldPen = pDC->SelectObject(&pen);
		CRect rcNodeTmp;
		for(i = 0;i<m_nCurMorInkRectNum;i ++)
		{
			for(j = 0;j<m_sCurCodeRegn[i].nRegionDotNum;j++)
			{
				pDC->MoveTo(m_sCurCodeRegn[i].ptRegionDot[j]);
				pDC->LineTo(m_sCurCodeRegn[i].ptRegionDot[j+1]);
				////////////////////
				rcNodeTmp = GetNodeRect(m_sCurCodeRegn[i].ptRegionDot[j]);
				pDC->FillRect(rcNodeTmp,&CBrush(RGB(0,0,255)));
			}
		}
		if(m_nCurSelDotIndex != -1)
		{
			rcNodeTmp = GetNodeRect(m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[m_nCurSelDotIndex]);

			pDC->FillRect(rcNodeTmp,&CBrush(RGB(255,0,255)));
		}
		if(m_nCurSelRegionIndex != -1)
		{
			CBrush    brSel(2,RGB(80,80,80));
			pDC->FillRgn(&m_rgnCurCodeTmp[m_nCurSelRegionIndex],&brSel);
		}
		pDC->SelectObject(oldPen);
	}
}

CRect CCodeModelDlg::DrawDotMark(CDC *pDC, CPoint ptCent,int nDotSize)
{
	CRect rcDot(ptCent.x - nDotSize,ptCent.y - nDotSize,ptCent.x + nDotSize,ptCent.y + nDotSize);
	pDC->FillRect(rcDot,&CBrush(RGB(0,0,255)));
	return rcDot;
}

void CCodeModelDlg::InitTmpOCVTmpData()
{
	UINT i,nCode;
	char chTmp;
	for(i = 0;i<m_nCodeNumber;i++)
	{
		chTmp = m_strCurCode.GetAt (i);
		if( chTmp>='0'&&chTmp <='9')
		{
			nCode = (int)(chTmp - '0');
		}
		if(chTmp>='A'&&chTmp <='Z')
		{
			nCode = (int)(chTmp - 'A') + DIGIT_NUM;
		}
		m_sCurCodeOcvTmp[nCode] = m_OcrProExt->g_sOcrInputPara.lpLeaklineTmp[nCode];
		//  [12/17/2007]
		m_sCurCodeMoreTmp[nCode] = m_OcrProExt->g_sOcrInputPara.lpMoreInkTmp[nCode];
	}
	return;
}

void CCodeModelDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_rcDispOrgImage.PtInRect(point))
	{
		if(m_nModiCodeDot == 0)
		{
			CDialog::OnMouseMove(nFlags, point);
			return;
		}
		CClientDC dc(this);
		CPen pen(PS_SOLID,1,RGB(255,255,0)),*OldPen;
		OldPen=dc.SelectObject(&pen);
		if(bLeftDown&&m_nCurSelDotIndex != -1)
		{				/////////////////
			dc.SetROP2(R2_XORPEN);
			dc.MoveTo(ptPre1);
			dc.LineTo(ptCur1);
			dc.MoveTo(ptPre2);
			dc.LineTo(ptCur2);
			
			ptCur1=point;
			ptCur2=point;
			
			dc.MoveTo(ptPre1);
			dc.LineTo(ptCur1);
			dc.MoveTo(ptPre2);
			dc.LineTo(ptCur2);		
			dc.SetROP2(R2_COPYPEN);   
			if(m_nModiCodeDot == 1)
			{
				/////////////////
				m_lptTmpNode[m_nCurSelDotIndex] = point;
				m_lpRectDot[m_nCurSelDotIndex] = GetNodeRect(point);
				////////////
			}
			else
			{
				m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[m_nCurSelDotIndex] = point;
			}
			dc.SelectObject(OldPen);
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CCodeModelDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nModiCodeDot != 0)
	{	
		///////////	
		if(m_nCurSelDotIndex != -1)
		{
			if(m_nModiCodeDot == 1)
			{
				////////////
				m_sCurCodeOcvTmp[m_nCurCodeIndex].Point[m_nCurSelDotIndex].dxOffset = (double)(m_lptTmpNode[m_nCurSelDotIndex].x - m_rcDispOrgImage.left)/m_rcDispOrgImage.Width();
				m_sCurCodeOcvTmp[m_nCurCodeIndex].Point[m_nCurSelDotIndex].dyOffset = (double)(m_lptTmpNode[m_nCurSelDotIndex].y - m_rcDispOrgImage.top)/m_rcDispOrgImage.Height();
				/////////////
				UpdateNodeLineInfo();
				/////////////
			}
			if(m_nModiCodeDot == 2)
			{
				//  [12/14/2007]
				int nCurRegionNodeNum = m_sCurCodeRegn[m_nCurSelRegionIndex].nRegionDotNum;
				m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[nCurRegionNodeNum] = m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[0];
				//  [12/14/2007]
				m_rgnCurCodeTmp[m_nCurSelRegionIndex].Detach();
				m_rgnCurCodeTmp[m_nCurSelRegionIndex].CreatePolygonRgn(m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot,nCurRegionNodeNum + 1,WINDING);
			}
		}
		bLeftDown = FALSE;
		m_nCurSelDotIndex = -1;
		////////
		InvalidateRect(&m_rcDispOrgImage);
	}
	CDialog::OnLButtonUp(nFlags, point);
}

CRect CCodeModelDlg::GetNodeRect(CPoint ptCent)
{
	return CRect(ptCent.x - 4,ptCent.y - 4,ptCent.x + 4,ptCent.y + 4);
}

void CCodeModelDlg::GetNodeRectInfo()
{
	/////////
	int x0,y0,x1,y1;
	CRect rcCode = m_OcrProExt->g_sSepOutputPara.lpRectCode[m_nIndex];
	
	m_rcDispImage = GetDisplayRect(rcCode.Size(),m_rcImage,1);
	m_rcDispOrgImage = GetDisplayRect(rcCode.Size(),m_rcOrgImg,1);
	//////////////
	int i;
	if(m_nModiCodeDot == 1)
	{	
		LeakLineDot dotLeakTmp = m_sCurCodeOcvTmp[m_nCurCodeIndex];
		m_nRectSelPtNum = dotLeakTmp.nPointNum ;
		ptLineArray.RemoveAll();
		m_nCurNodeLineNum = 0;
		LINEVAL     sLineTmp;
		for(i = 0;i<m_nRectSelPtNum - 1;i ++)
		{
			if(dotLeakTmp.dir[i] != 9)
			{	
				x0 = (int)(dotLeakTmp.Point[i].dxOffset*m_rcDispOrgImage.Width()) + m_rcDispOrgImage.left;
				y0 = (int)(dotLeakTmp.Point[i].dyOffset*m_rcDispOrgImage.Height()) + m_rcDispOrgImage.top;	
				x1 = (int)(dotLeakTmp.Point[i + 1].dxOffset*m_rcDispOrgImage.Width()) + m_rcDispOrgImage.left;
				y1 = (int)(dotLeakTmp.Point[i + 1].dyOffset*m_rcDispOrgImage.Height()) + m_rcDispOrgImage.top;
				/////////////////
				sLineTmp = GetCurLineValue(CPoint(x0,y0),CPoint(x1,y1),i);
				ptLineArray.Add(sLineTmp);
				m_nCurNodeLineNum ++;
				///////////////////////////
				m_lptTmpNode[i] = CPoint(x0,y0);
				m_lptTmpNode[i+1] = CPoint(x1,y1);
				////////////
				m_lpRectDot[i] = CRect(x0 - 4,y0 - 4,x0 + 4,y0 + 4);
				m_lpRectDot[i+1] = CRect(x1 - 4,y1 - 4,x1 + 4,y1 + 4);
				//////////////
			}
		}
	}
	if(m_nModiCodeDot == 2)
	{	
		MoreInkDot  dotMoreInk = m_sCurCodeMoreTmp[m_nCurCodeIndex];
		m_nCurMorInkRectNum = dotMoreInk.nRegionNum;
		if(m_nCurMorInkRectNum < 0)
		{
			m_nCurMorInkRectNum = 0;
		}
		int j;
		Region    rgnTmp ;
		for(i = 0;i<m_nCurMorInkRectNum;i ++)
		{
			rgnTmp = dotMoreInk.region[i];
			/////////////
			m_sCurCodeRegn[i].nRegionDotNum = rgnTmp.nPointNum;
			m_sCurCodeRegn[i].m_fMoreInkWeight = rgnTmp.fWeight;
			///////////////////////////
			for(j = 0;j<rgnTmp.nPointNum;j++)
			{
				m_sCurCodeRegn[i].ptRegionDot[j].x = (int)(dotMoreInk.region[i].Point[j].x*m_rcDispOrgImage.Width()) + m_rcDispOrgImage.left;
				m_sCurCodeRegn[i].ptRegionDot[j].y = (int)(dotMoreInk.region[i].Point[j].y*m_rcDispOrgImage.Height()) + m_rcDispOrgImage.top;
				//////////
			}
			m_sCurCodeRegn[i].ptRegionDot[j] = m_sCurCodeRegn[i].ptRegionDot[0];
			//  [12/13/2007]
			m_rgnCurCodeTmp[i].Detach();
			m_rgnCurCodeTmp[i].CreatePolygonRgn(m_sCurCodeRegn[i].ptRegionDot,rgnTmp.nPointNum+1,WINDING);
		}
		m_nCurSelRegionIndex = -1;
	}
	m_nCurSelDotIndex = -1;
}

void CCodeModelDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	if(m_nModiCodeDot == 0)
	{
		return;
	}
	CPoint pt = point;
	ScreenToClient(&pt);

	if(m_rcDispOrgImage.PtInRect(pt))
	{
		bAddNodePoint = FALSE;
		bDelNodePoint = FALSE;
		CMenu menu;
		menu.LoadMenu(IDR_MENU_CONTEXT);
		int i,j;
		CRect rc;

		if(m_nModiCodeDot == 1)
		{
			for(i = 0;i<m_nRectSelPtNum;i++)
			{
				rc = m_lpRectDot[i];
				if(rc.PtInRect(pt))
				{
					m_nCurSelDotIndex = i;
					bDelNodePoint = TRUE;
					break;
				}
			}
		}
		if(m_nModiCodeDot == 2)
		{
			if(m_nCurSelRegionIndex > -1)
			{
				for(j = 0;j<m_sCurCodeRegn[m_nCurSelRegionIndex].nRegionDotNum;j++)
				{
					rc = GetNodeRect(m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[j]);
					if(rc.PtInRect(pt))
					{
						m_nCurSelDotIndex = j;
						//  [12/13/2007]
						bDelNodePoint = TRUE;
						break;
					}
				}
			}
		}
		//如果没有删除节点，判断是否是要添加节点，在直线上
		if(!bDelNodePoint)
		{
			LINEVAL lTmp;
			CPoint ptTmp;
			if(m_nModiCodeDot == 1)
			{
				for(i = 0;i<m_nCurNodeLineNum;i++)
				{
					lTmp = ptLineArray.GetAt(i);
					if(PtIsInLine(pt,lTmp,3))
					{
						m_nCurSelDotIndex = lTmp.m_nLineIndex;
						bAddNodePoint = TRUE;
						ptAddCurPos = pt;
						//垂线上或者
						if(lTmp.m_fKVal == -1&&lTmp.ptPre.x == lTmp.ptRear.x)
						{
							ptAddCurPos.x = (int)(lTmp.m_fBVal);
						}
						else
						{
							if(lTmp.m_fKVal == 0)
							{
								ptAddCurPos.y = lTmp.ptPre.y;
							}
							else
							{
								ptTmp.y = (int)(lTmp.m_fKVal*pt.x+lTmp.m_fBVal);
								ptTmp.x = (int)((pt.y - lTmp.m_fBVal)/lTmp.m_fKVal);
								//////////
								if(abs(ptTmp.x - pt.x) < abs(ptTmp.y - pt.y))
								{
									ptAddCurPos.x = ptTmp.x;
								}
								else
								{
									ptAddCurPos.y = ptTmp.y;
								}
							}
						}
						break;
					}
				}
			}
			//  [12/13/2007]
			if(m_nModiCodeDot == 2)
			{
				for(i = 0;i<m_sCurCodeRegn[m_nCurSelRegionIndex].nRegionDotNum;i++)
				{
					lTmp = GetCurLineValue(m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[i],
						m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[i + 1],i);
					if(PtIsInLine(pt,lTmp,3))
					{
						m_nCurSelDotIndex = lTmp.m_nLineIndex;	
						bAddNodePoint = TRUE;
						ptAddCurPos = pt;
						//垂线上或者
						if(lTmp.m_fKVal == -1&&lTmp.ptPre.x == lTmp.ptRear.x)
						{
							ptAddCurPos.x = (int)(lTmp.m_fBVal);
						}
						else
						{
							if(lTmp.m_fKVal == 0)
							{
								ptAddCurPos.y = lTmp.ptPre.y;
							}
							else
							{
								ptTmp.y = (int)(lTmp.m_fKVal*pt.x+lTmp.m_fBVal);
								ptTmp.x = (int)((pt.y - lTmp.m_fBVal)/lTmp.m_fKVal);
								//////////
								if(abs(ptTmp.x - pt.x) < abs(ptTmp.y - pt.y))
								{
									ptAddCurPos.x = ptTmp.x;
								}
								else
								{
									ptAddCurPos.y = ptTmp.y;
								}
							}
						}
						break;
					}
				}
			}
			if(bAddNodePoint)
			{
				int nDirMode = m_sCurCodeOcvTmp[m_nCurCodeIndex].dir[m_nCurSelDotIndex];
				switch(nDirMode)
				{
				case 0:
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_HORZ,MF_CHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_VERT,MF_UNCHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_TITLE45,MF_UNCHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_TITLE135,MF_UNCHECKED);
					break;
				case 1:
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_HORZ,MF_UNCHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_VERT,MF_CHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_TITLE45,MF_UNCHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_TITLE135,MF_UNCHECKED);
					break;
				case 2:
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_HORZ,MF_UNCHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_VERT,MF_UNCHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_TITLE45,MF_CHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_TITLE135,MF_UNCHECKED);
					break;
				case 3:
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_HORZ,MF_UNCHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_VERT,MF_UNCHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_TITLE45,MF_UNCHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_TITLE135,MF_CHECKED);						
					break;
				case 9:
					break;
				case 15:
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_HORZ,MF_UNCHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_VERT,MF_UNCHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_TITLE45,MF_UNCHECKED);
					menu.GetSubMenu(0)->CheckMenuItem(IDM_MENU_SETDIR_TITLE135,MF_UNCHECKED);			
					break;
				default:
					break;
				}
			}
		}
		if(bAddNodePoint||bDelNodePoint)
		{
			if(bAddNodePoint)
			{
				menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_DELETE,MF_BYCOMMAND|MF_GRAYED);
				menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_ADD,MF_BYCOMMAND|MF_ENABLED);

				menu.GetSubMenu(0)->EnableMenuItem(IDM_MENU_SETDIR_HORZ,MF_BYCOMMAND|MF_ENABLED);
				menu.GetSubMenu(0)->EnableMenuItem(IDM_MENU_SETDIR_VERT,MF_BYCOMMAND|MF_ENABLED);
				menu.GetSubMenu(0)->EnableMenuItem(IDM_MENU_SETDIR_TITLE45,MF_BYCOMMAND|MF_ENABLED);
				menu.GetSubMenu(0)->EnableMenuItem(IDM_MENU_SETDIR_TITLE135,MF_BYCOMMAND|MF_ENABLED);	
			}
			if(bDelNodePoint)
			{
				menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_DELETE,MF_BYCOMMAND|MF_ENABLED);
				menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_ADD,MF_BYCOMMAND|MF_GRAYED);

				menu.GetSubMenu(0)->EnableMenuItem(IDM_MENU_SETDIR_HORZ,MF_BYCOMMAND|MF_GRAYED);
				menu.GetSubMenu(0)->EnableMenuItem(IDM_MENU_SETDIR_VERT,MF_BYCOMMAND|MF_GRAYED);
				menu.GetSubMenu(0)->EnableMenuItem(IDM_MENU_SETDIR_TITLE45,MF_BYCOMMAND|MF_GRAYED);
				menu.GetSubMenu(0)->EnableMenuItem(IDM_MENU_SETDIR_TITLE135,MF_BYCOMMAND|MF_GRAYED);	
			}
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,pWnd);
			//////////
		}
		else
		{
			if(m_nModiCodeDot == 2)
			{
				menu.Detach();
				menu.LoadMenu(IDR_MENU_REGION);
				/////////////
				if(m_nCurSelRegionIndex != -1)
				{	
					if(m_rgnCurCodeTmp[m_nCurSelRegionIndex].PtInRegion(pt))
					{
						menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_NEW_REGION,MF_BYCOMMAND|MF_GRAYED);
						menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_DEL_REGION,MF_BYCOMMAND|MF_ENABLED);
					}
					else
					{	
						menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_NEW_REGION,MF_BYCOMMAND|MF_ENABLED);
						for(i = 0;i<m_nCurMorInkRectNum;i++)
						{
							if(m_rgnCurCodeTmp[i].PtInRegion(pt))
							{
								menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_NEW_REGION,MF_BYCOMMAND|MF_GRAYED);
								break;
							}
						}
						menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_DEL_REGION,MF_BYCOMMAND|MF_GRAYED);
					}
				}
				else
				{
					menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_NEW_REGION,MF_BYCOMMAND|MF_ENABLED);
					menu.GetSubMenu(0)->EnableMenuItem(ID_MENU_DEL_REGION,MF_BYCOMMAND|MF_GRAYED);
				}
				/////////////
				menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,pWnd);
				/////////
				//  [12/14/2007]
				m_ptAddNewRegion = pt;
			}
		}
	}
}

BOOL CCodeModelDlg::PtIsInLine(CPoint ptCur, LINEVAL lVal, int nError)
{
	float fTmp = 1000;

	int nMinY = min(lVal.ptPre.y,lVal.ptRear.y),nMaxY = max(lVal.ptPre.y,lVal.ptRear.y);
	int nMinX = min(lVal.ptPre.x,lVal.ptRear.x),nMaxX = max(lVal.ptPre.x,lVal.ptRear.x);
	if(lVal.m_fKVal == -1&&lVal.ptPre.x == lVal.ptRear.x)//垂线
	{
		if(ptCur.y>nMinY&&ptCur.y<nMaxY)
		{
			fTmp = (float)fabs(lVal.m_fBVal - ptCur.x);	
		}
	}
	else
	{
		if(lVal.m_fKVal == 0)
		{
			if(ptCur.x>nMinX&&ptCur.x<nMaxX)
			{
				fTmp = (abs(long(lVal.ptPre.y - ptCur.y)));	
			}
		}
		else
		{
			int nDir = 3;
			if(nMaxY - nMinY < 5)
			{
				nDir = 0;
			}  
			if(nMaxX - nMinX < 5)
			{
				nDir = 1;
			}
			////////////////////
			if(nDir == 3)
			{
				if(ptCur.x>nMinX&&ptCur.x<nMaxX&&ptCur.y>nMinY&&ptCur.y<nMaxY)
				{
					fTmp = (float)((lVal.m_fKVal*ptCur.x-ptCur.y+lVal.m_fBVal)/sqrt(lVal.m_fKVal*lVal.m_fKVal+1));
				}
			}
			else
			{
				if(nDir == 0)
				{
					if(ptCur.x>nMinX&&ptCur.x<nMaxX)
					{
						fTmp = (float)fabs((lVal.m_fKVal*ptCur.x-ptCur.y+lVal.m_fBVal)/sqrt(lVal.m_fKVal*lVal.m_fKVal+1));
					}
				}
				else
				{
					if(ptCur.y>nMinY&&ptCur.y<nMaxY)
					{
						fTmp = (float)fabs((lVal.m_fKVal*ptCur.x-ptCur.y+lVal.m_fBVal)/sqrt(lVal.m_fKVal*lVal.m_fKVal+1));
					}
				}
			}
		}
	}
	if(fabs(fTmp)<nError)
	{
		return TRUE;
	}
	////////
	return FALSE;
}

void CCodeModelDlg::OnOK() 
{
	// TODO: Add extra validation here
	ptLineArray.RemoveAll();
	CDialog::OnOK();
}

LINEVAL CCodeModelDlg::GetCurLineValue(CPoint pt1, CPoint pt2, int nIndex)
{
	LINEVAL lTmp;
	lTmp.m_fBVal = 0;
	lTmp.m_fKVal = 0;
	/////////////
	lTmp.ptPre = pt1;
	lTmp.ptRear = pt2;
	lTmp.m_nLineIndex = nIndex;
	/////////////
	if(pt1.x == pt2.x)
	{
		lTmp.m_fKVal = -1;
		lTmp.m_fBVal = (float)pt1.x;
	}
	else
	{
		lTmp.m_fKVal = (float)(pt2.y - pt1.y)/(pt2.x - pt1.x);
		lTmp.m_fBVal = (float)(pt1.y*pt2.x - pt2.y*pt1.x)/(pt2.x - pt1.x);
	}
	///////////
	return lTmp;
}

void CCodeModelDlg::OnMenuAdd() 
{
	if(!bAddNodePoint) return;
	/////////
	int i;
	if(m_nModiCodeDot == 1)
	{
		m_nRectSelPtNum ++;
		
		for(i = m_nRectSelPtNum - 1;i>m_nCurSelDotIndex;i--)
		{
			m_lptTmpNode[i + 1] = m_lptTmpNode[i];
			m_lpRectDot[i + 1] = m_lpRectDot[i] ;
			m_sCurCodeOcvTmp[m_nCurCodeIndex].Point[i + 1] = m_sCurCodeOcvTmp[m_nCurCodeIndex].Point[i];
			m_sCurCodeOcvTmp[m_nCurCodeIndex].dir[i + 1] = m_sCurCodeOcvTmp[m_nCurCodeIndex].dir[i];
		}
		m_lptTmpNode[m_nCurSelDotIndex+1] = ptAddCurPos;
		m_lpRectDot[m_nCurSelDotIndex+1] = GetNodeRect(ptAddCurPos);
		////////
		m_sCurCodeOcvTmp[m_nCurCodeIndex].Point[m_nCurSelDotIndex+1].dxOffset = (double)(ptAddCurPos.x - m_rcDispOrgImage.left)/m_rcDispOrgImage.Width();
		m_sCurCodeOcvTmp[m_nCurCodeIndex].Point[m_nCurSelDotIndex+1].dyOffset = (double)(ptAddCurPos.y - m_rcDispOrgImage.top)/m_rcDispOrgImage.Height();
		m_sCurCodeOcvTmp[m_nCurCodeIndex].dir[m_nCurSelDotIndex+1]	= 15;
		m_sCurCodeOcvTmp[m_nCurCodeIndex].nPointNum = m_nRectSelPtNum;
		//////////////更新直线段信息
		UpdateNodeLineInfo();
	}
	if(m_nModiCodeDot == 2)
	{
		m_sCurCodeRegn[m_nCurSelRegionIndex].nRegionDotNum ++;
		for(i =m_sCurCodeRegn[m_nCurSelRegionIndex].nRegionDotNum - 1;i>m_nCurSelDotIndex;i--)
		{
			m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[i + 1] = m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[i];
		}
		//  [12/13/2007]
		m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[m_nCurSelDotIndex + 1] = ptAddCurPos;
		m_rgnCurCodeTmp[m_nCurSelRegionIndex].Detach();
		m_rgnCurCodeTmp[m_nCurSelRegionIndex].CreatePolygonRgn(m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot,m_sCurCodeRegn[m_nCurSelRegionIndex].nRegionDotNum+1,WINDING);
	}
	//////////////
	InvalidateRect(&m_rcDispOrgImage);
}

void CCodeModelDlg::OnMenuDelete() 
{
	if(!bDelNodePoint)
	{
		return;
	}	/////////////
	int i;
	if(m_nModiCodeDot == 1)
	{
		m_nRectSelPtNum --;
		for(i = m_nCurSelDotIndex;i<m_nRectSelPtNum;i++)
		{
			m_lptTmpNode[i] = m_lptTmpNode[i + 1];
			m_lpRectDot[i] = m_lpRectDot[i + 1] ;
			
			m_sCurCodeOcvTmp[m_nCurCodeIndex].Point[i] = m_sCurCodeOcvTmp[m_nCurCodeIndex].Point[i + 1];
			m_sCurCodeOcvTmp[m_nCurCodeIndex].dir[i] = m_sCurCodeOcvTmp[m_nCurCodeIndex].dir[i + 1];
		}
		UpdateNodeLineInfo();
	}
	if(m_nModiCodeDot == 2)
	{
		if(m_sCurCodeRegn[m_nCurSelRegionIndex].nRegionDotNum < 4)
		{
			return;
		}
		m_sCurCodeRegn[m_nCurSelRegionIndex].nRegionDotNum --;
		
		for(i = m_nCurSelDotIndex;i<m_sCurCodeRegn[m_nCurSelRegionIndex].nRegionDotNum;i++)
		{
			m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[i] = m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[i + 1];
		}
		m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[i] = m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot[0];
			//  [12/13/2007]
		m_rgnCurCodeTmp[m_nCurSelRegionIndex].Detach();
		m_rgnCurCodeTmp[m_nCurSelRegionIndex].CreatePolygonRgn(m_sCurCodeRegn[m_nCurSelRegionIndex].ptRegionDot,m_sCurCodeRegn[m_nCurSelRegionIndex].nRegionDotNum+1,WINDING);
	}
	InvalidateRect(&m_rcDispOrgImage);
}

void CCodeModelDlg::UpdateNodeLineInfo()
{
	ptLineArray.RemoveAll();
	m_nCurNodeLineNum = 0;
	//////////////
	int i;
	LINEVAL     sLineTmp;
	LeakLineDot dotTmp = m_sCurCodeOcvTmp[m_nCurCodeIndex];
	int x0,y0,x1,y1;
	for(i = 0;i<m_nRectSelPtNum - 1;i ++)
	{
		if(dotTmp.dir[i] != 9)
		{	
			x0 = (int)(dotTmp.Point[i].dxOffset*m_rcDispOrgImage.Width()) + m_rcDispOrgImage.left;
			y0 = (int)(dotTmp.Point[i].dyOffset*m_rcDispOrgImage.Height()) + m_rcDispOrgImage.top;	
			x1 = (int)(dotTmp.Point[i + 1].dxOffset*m_rcDispOrgImage.Width()) + m_rcDispOrgImage.left;
			y1 = (int)(dotTmp.Point[i + 1].dyOffset*m_rcDispOrgImage.Height()) + m_rcDispOrgImage.top;
			//////////////
			sLineTmp = GetCurLineValue(CPoint(x0,y0),CPoint(x1,y1),i);
			ptLineArray.Add(sLineTmp);
			m_nCurNodeLineNum ++;
		}
	}
}

void CCodeModelDlg::OnMenuSetdirHorz() 
{
	// TODO: Add your command handler code here
	m_sCurCodeOcvTmp[m_nCurCodeIndex].dir[m_nCurSelDotIndex] = 0;
	InvalidateRect(&m_rcDispOrgImage);
}

void CCodeModelDlg::OnMenuSetdirVert() 
{
	// TODO: Add your command handler code here
	m_sCurCodeOcvTmp[m_nCurCodeIndex].dir[m_nCurSelDotIndex] = 1;
	InvalidateRect(&m_rcDispOrgImage);
}

void CCodeModelDlg::OnMenuSetdirTitle45() 
{
	// TODO: Add your command handler code here
	m_sCurCodeOcvTmp[m_nCurCodeIndex].dir[m_nCurSelDotIndex] = 2;
	InvalidateRect(&m_rcDispOrgImage);
}

void CCodeModelDlg::OnMenuSetdirTitle135() 
{
	// TODO: Add your command handler code here
	m_sCurCodeOcvTmp[m_nCurCodeIndex].dir[m_nCurSelDotIndex] = 3;
	InvalidateRect(&m_rcDispOrgImage);
}

void CCodeModelDlg::OnRadioMoreinkSet() 
{
	// TODO: Add your control notification handler code here
	m_nModiCodeDot = 2;	
	GetDlgItem (IDC_BUTTON_SAVEASDOT)->EnableWindow (TRUE);
	//  [12/12/2007]
	m_nCurSelDotIndex = -1;
	m_nCurSelRegionIndex = -1;
	///////////////////////////////////
//	InitTmpMoreInkTmpData();
	GetNodeRectInfo();
	
	InvalidateRect(&m_rcOrgImg);
	InvalidateRect(&m_rcImage);
}

void CCodeModelDlg::InitTmpMoreInkTmpData()
{
	UINT i,nCode;
	char chTmp;
	for(i = 0;i<m_nCodeNumber;i++)
	{
		chTmp = m_strCurCode.GetAt (i);
		if( chTmp>='0'&&chTmp <='9')
		{
			nCode = (int)(chTmp - '0');
		}
		if(chTmp>='A'&&chTmp <='Z')
		{
			nCode = (int)(chTmp - 'A') + DIGIT_NUM;
		}
		m_sCurCodeMoreTmp[nCode] = m_OcrProExt->g_sOcrInputPara.lpMoreInkTmp[nCode];
	}
}



void CCodeModelDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int i;
	m_nCurSelRegionIndex = -1;
	for(i = 0;i<m_nCurMorInkRectNum;i++)
	{
		if(m_rgnCurCodeTmp[i].PtInRegion(point))
		{
			m_nCurSelRegionIndex = i;
			break;
		}
	}
	if(m_nCurSelRegionIndex != -1)
	{
		CString str;
		str.Format("%3.2f",m_sCurCodeRegn[m_nCurSelRegionIndex].m_fMoreInkWeight);
		SetDlgItemText(IDC_EDIT_GRIDVALUE,str);
	}
	InvalidateRect(m_rcOrgImg);
	CDialog::OnLButtonDblClk(nFlags, point);
}



void CCodeModelDlg::OnMenuNewRegion() 
{
	// TODO: Add your command handler code here
	//  [12/14/2007]
	m_sCurCodeRegn[m_nCurMorInkRectNum].m_fMoreInkWeight = 1;
	m_sCurCodeRegn[m_nCurMorInkRectNum].nRegionDotNum = 4;
	//////////////////////
	m_sCurCodeRegn[m_nCurMorInkRectNum].ptRegionDot[0] = CPoint(max(m_ptAddNewRegion.x - 10,m_rcDispOrgImage.left + 2),max(m_rcDispOrgImage.top + 2,m_ptAddNewRegion.y - 10));
	m_sCurCodeRegn[m_nCurMorInkRectNum].ptRegionDot[1] = CPoint(min(m_ptAddNewRegion.x + 10,m_rcDispOrgImage.right - 2),max(m_rcDispOrgImage.top + 2,m_ptAddNewRegion.y - 10));
	m_sCurCodeRegn[m_nCurMorInkRectNum].ptRegionDot[2] = CPoint(min(m_ptAddNewRegion.x + 10,m_rcDispOrgImage.right - 2),min(m_rcDispOrgImage.bottom - 2,m_ptAddNewRegion.y + 10));
	m_sCurCodeRegn[m_nCurMorInkRectNum].ptRegionDot[3] = CPoint(max(m_ptAddNewRegion.x - 10,m_rcDispOrgImage.left + 2),min(m_rcDispOrgImage.bottom - 2,m_ptAddNewRegion.y + 10));
	m_sCurCodeRegn[m_nCurMorInkRectNum].ptRegionDot[4] = m_sCurCodeRegn[m_nCurMorInkRectNum].ptRegionDot[0];
	//////////////////////
	m_rgnCurCodeTmp[m_nCurMorInkRectNum].Detach();
	m_rgnCurCodeTmp[m_nCurMorInkRectNum].CreatePolygonRgn(m_sCurCodeRegn[m_nCurMorInkRectNum].ptRegionDot,5,WINDING);
	//  [12/14/2007]
	m_nCurSelRegionIndex = m_nCurMorInkRectNum;
	/////////////////
	CString str;
	str.Format("%3.2f",m_sCurCodeRegn[m_nCurSelRegionIndex].m_fMoreInkWeight);
	SetDlgItemText(IDC_EDIT_GRIDVALUE,str);	
	/////////////////
	m_nCurMorInkRectNum ++;
	//////////
	InvalidateRect(m_rcOrgImg);

}

void CCodeModelDlg::OnMenuDelRegion() 
{
	// TODO: Add your command handler code here
	if(m_nCurSelRegionIndex == -1)
	{
		AfxMessageBox("没有选择要删除的区域!");
		return;
	}
	for(int i = m_nCurSelRegionIndex;i<m_nCurMorInkRectNum-1;i++)
	{
		m_sCurCodeRegn[i] = m_sCurCodeRegn[i + 1];
		
		m_rgnCurCodeTmp[i].CopyRgn(&m_rgnCurCodeTmp[i + 1]);
	}
	m_rgnCurCodeTmp[m_nCurMorInkRectNum - 1].Detach();
	m_nCurMorInkRectNum --;	
	///////////////////////////
	//  [12/14/2007]
	m_nCurSelRegionIndex = -1;
	SetDlgItemText(IDC_EDIT_GRIDVALUE,"");	
	InvalidateRect(m_rcOrgImg);
}

void CCodeModelDlg::UpdateCodeMoreInkRegion()
{
	int i,j;

	m_sCurCodeMoreTmp[m_nCurCodeIndex].nRegionNum = m_nCurMorInkRectNum;
	for(i = 0;i<m_nCurMorInkRectNum;i++)
	{
		m_sCurCodeMoreTmp[m_nCurCodeIndex].region[i].nPointNum = m_sCurCodeRegn[i].nRegionDotNum;
		m_sCurCodeMoreTmp[m_nCurCodeIndex].region[i].fWeight = m_sCurCodeRegn[i].m_fMoreInkWeight;
		/////////////////////////////////////////////////
		for(j = 0;j<m_sCurCodeRegn[i].nRegionDotNum;j++)
		{
			m_sCurCodeMoreTmp[m_nCurCodeIndex].region[i].Point[j].x = (double)(m_sCurCodeRegn[i].ptRegionDot[j].x - m_rcDispOrgImage.left)/m_rcDispOrgImage.Width();
			m_sCurCodeMoreTmp[m_nCurCodeIndex].region[i].Point[j].y = (double)(m_sCurCodeRegn[i].ptRegionDot[j].y - m_rcDispOrgImage.top)/m_rcDispOrgImage.Height();
		}
	}
}

void CCodeModelDlg::OnMenuSetdirTitlemo() 
{
	// TODO: Add your command handler code here
	m_sCurCodeOcvTmp[m_nCurCodeIndex].dir[m_nCurSelDotIndex] = 9;
	InvalidateRect(&m_rcDispOrgImage);
	
}



void CCodeModelDlg::OnBnClickedButtonUpdatedot()
{
	// TODO: Add your control notification handler code here

	if(m_nIndex >= (int)m_nCodeNumber)
	{
		return;
	}
	char chTmp = m_strCurCode.GetAt(m_nIndex);
	int nCode;
	if( chTmp>='0'&&chTmp <='9')
	{
		nCode = (int)(chTmp - '0');
	}
	if(chTmp>='A'&&chTmp <='Z')
	{
		nCode = (int)(chTmp - 'A')+DIGIT_NUM;
	}
	m_OcrProExt->UpdateNewCodeTmp(nCode);
}
