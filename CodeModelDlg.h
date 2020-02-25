#if !defined(AFX_CODEMODELDLG_H__43216568_208A_46FD_96F4_9CA2D898B24B__INCLUDED_)
#define AFX_CODEMODELDLG_H__43216568_208A_46FD_96F4_9CA2D898B24B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CodeModelDlg.h : header file
//

#include "ocrProDll.h"
#include ".\\OcrFiles\\OcrProExtern.h"
/////////////////////////////////////////////////////////////////////////////
// CCodeModelDlg dialog
typedef struct 
{	
	int m_nLineIndex;

	float m_fKVal;
	float m_fBVal;
	POINT ptPre;
	POINT ptRear;
}LINEVAL;

typedef struct 
{	
	int nRegionDotNum;
	float	m_fMoreInkWeight;
	POINT   ptRegionDot[MaxPointNum];
}s_CodeMoreInkRegion;

class CCodeModelDlg : public CDialog
{
// Construction
public:
	void DrawCodeOCRDot(CDC *pDC);
	CCodeModelDlg(CWnd* pParent = NULL);   // standard constructor
	CString		m_strCurCode,m_strCurAppDotPath,m_strCurAppOCVPath;//��ǰʶ��ĺ���
	UINT			m_nCodeNumber;
// Dialog Data
	//{{AFX_DATA(CCodeModelDlg)
	enum { IDD = IDD_DIALOG_FACTDOT };
	int		m_nModiCodeDot;
	//}}AFX_DATA
    BOOL m_bChangedOCRMode[CODEALLNUMBER];
	BOOL m_bChangedOCVMode[CODEALLNUMBER];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeModelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CRect		m_rcImage,m_rcOrgImg,m_rcChoose;//��ߵ�ͼ�������ұߵ�ͼ����ʾ����ѡ�����������
	CRect		m_rcDispImage,m_rcDispOrgImage;//���ҵ�ʵ��ͼ����ʾ����
	BYTE		*bytTmpCode;                       //ʵ�ʵ�ͼ������ͼ����
	int			bytMargTmpCode[32*32];			//�����������ͼ����
	BYTE		*bytOrgTmpCode;                  //ԭʼģ�������ͼ����
	int			m_nWidth,m_nHeight,m_nStep,m_nIndex;//����ģ���ȡ��߶ȡ���������Ĵ�С�͵�ǰ�����λ�ú�
	int			m_nCurCol;                           //��ǰѡ����к�
	int			m_nCurRow;								//��ǰѡ����к�

	//  [12/14/2007]
	int			m_nGridVal;                             //�޸ĵ�OCRģ�嵥Ԫ���ֵ

	BOOL		m_bShowOrgDot;							//��ʾԭʼģ�廹��ƥ�����
	BOOL		m_bTempMody;                             //ģ���Ƿ�����
	//////////////////
	BOOL		m_bLeftTmpPos;                           //���ѡ���������
	BOOL		m_bRightTmpPos;                          //���ѡ���ұ�ģ������

	PBITMAPINFO m_ptrBmpInfo;							//ͼ����ʾ��λͼ�ṹ��Ϣ
	BYTE		m_pbCodeData[256*256];                  //��������ͼ������
	BYTE		m_pbTmpData[2048];                      //��ʱ����

	CRect		m_lpRectDot[256];                        //�ڵ�����
	POINT		m_lptTmpNode[256];                       //�ڵ�����
	int			m_nCurSelDotIndex,m_nRectSelPtNum;        //��ǰѡ��Ľڵ�ţ��ڵ����
	//  [12/11/2007]
	int			m_nCurMorInkRectNum;                         //��ǰ�����ǶīȨ���������ø���
	/////////////
	LeakLineDot		m_sCurCodeOcvTmp[CODEALLNUMBER];       //�洢���к���Ķ���ģ��
	//  [12/11/2007]
	MoreInkDot		m_sCurCodeMoreTmp[CODEALLNUMBER];       //�洢���к����Ƕīģ��
	BOOL			bLeftDown;                             //��������Ƿ���

	CPoint ptPre1,ptCur1;                                  //��ʱ�Ļ�������1

	CPoint ptPre2,ptCur2;                                  //��ʱ�Ļ�������2 

	CPoint	ptAddCurPos;                                    //����ӽڵ㡢���������

	BOOL bAddNodePoint,bDelNodePoint;                      //�жϵ�ǰ����������ʱ�Ĳ���
	CArray<LINEVAL,LINEVAL> ptLineArray;                    //�����߶��б�
	int			m_nCurNodeLineNum;                           //�߶θ���
	//  [12/11/2007]
	s_CodeMoreInkRegion		m_sCurCodeRegn[MaxRegionNum];           //ǶīȨ������������Ϊ��λ
	int			m_nCurSelRegionIndex;                               //��ǰѡ��������
	//  [12/13/2007]
	CRgn		m_rgnCurCodeTmp[MaxRegionNum];              //Ȩ���������飬�����жϵ�ǰ���Ƿ��ڸ�������
	CPoint		m_ptAddNewRegion;                          //��������������ĵ�
public:
	void InitTmpMoreInkTmpData();
	PBYTE		m_lpImageData;                             //ͼ������
	CSize		m_szCurImage;                               //ͼ���С

	int			m_nCurCodeIndex;     //��ǰ����������ţ�0-9�ĺž���0��9����A'����Z������10

	COcrProExtern *m_OcrProExt;// ȫ�ֲ�����װָ�� [9/30/2011 Denghl]
// Implementation
protected:
	void UpdateCodeMoreInkRegion();
	void UpdateNodeLineInfo();
	LINEVAL GetCurLineValue(CPoint pt1, CPoint pt2,int nIndex);
	BOOL PtIsInLine(CPoint ptCur,LINEVAL lVal, int nError);
	void GetNodeRectInfo();
	CRect GetNodeRect(CPoint ptCent);
	void InitTmpOCVTmpData();
	CRect DrawDotMark(CDC *pDC, CPoint ptCent,int nDotSize);
	void DrawCodeOCVDot(CDC *pDC);
	void UpdateCodeDot();

	// Generated message map functions
	//{{AFX_MSG(CCodeModelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnCheckCodeNo();
	afx_msg void OnRadioOcrdot();
	afx_msg void OnRadioOcvdot();
	afx_msg void OnButtonModyfy();
	afx_msg void OnButtonSaveasdot();
	afx_msg void OnBtnCurdot();
	afx_msg void OnBtnTempmody();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual void OnOK();
	afx_msg void OnMenuAdd();
	afx_msg void OnMenuDelete();
	afx_msg void OnMenuSetdirHorz();
	afx_msg void OnMenuSetdirVert();
	afx_msg void OnMenuSetdirTitle45();
	afx_msg void OnMenuSetdirTitle135();
	afx_msg void OnRadioMoreinkSet();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMenuNewRegion();
	afx_msg void OnMenuDelRegion();
	afx_msg void OnMenuSetdirTitlemo();
	afx_msg void OnBnClickedButtonUpdatedot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEMODELDLG_H__43216568_208A_46FD_96F4_9CA2D898B24B__INCLUDED_)
