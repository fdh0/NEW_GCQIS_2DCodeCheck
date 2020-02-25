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
	CString		m_strCurCode,m_strCurAppDotPath,m_strCurAppOCVPath;//当前识别的号码
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
	CRect		m_rcImage,m_rcOrgImg,m_rcChoose;//左边的图像区域，右边的图像显示区域，选择的网格区域
	CRect		m_rcDispImage,m_rcDispOrgImage;//左右的实际图像显示区域
	BYTE		*bytTmpCode;                       //实际的图像网格图数据
	int			bytMargTmpCode[32*32];			//修正后的网格图数据
	BYTE		*bytOrgTmpCode;                  //原始模板的网格图数据
	int			m_nWidth,m_nHeight,m_nStep,m_nIndex;//网格模板宽度、高度、单个网格的大小和当前号码的位置号
	int			m_nCurCol;                           //当前选择的列号
	int			m_nCurRow;								//当前选择的行号

	//  [12/14/2007]
	int			m_nGridVal;                             //修改的OCR模板单元格的值

	BOOL		m_bShowOrgDot;							//显示原始模板还是匹配后结果
	BOOL		m_bTempMody;                             //模板是否修正
	//////////////////
	BOOL		m_bLeftTmpPos;                           //鼠标选择左边区域
	BOOL		m_bRightTmpPos;                          //鼠标选择右边模板区域

	PBITMAPINFO m_ptrBmpInfo;							//图像显示的位图结构信息
	BYTE		m_pbCodeData[256*256];                  //单个号码图像数据
	BYTE		m_pbTmpData[2048];                      //临时数据

	CRect		m_lpRectDot[256];                        //节点区域
	POINT		m_lptTmpNode[256];                       //节点坐标
	int			m_nCurSelDotIndex,m_nRectSelPtNum;        //当前选择的节点号，节点个数
	//  [12/11/2007]
	int			m_nCurMorInkRectNum;                         //当前号码的嵌墨权重区域设置个数
	/////////////
	LeakLineDot		m_sCurCodeOcvTmp[CODEALLNUMBER];       //存储所有号码的断线模板
	//  [12/11/2007]
	MoreInkDot		m_sCurCodeMoreTmp[CODEALLNUMBER];       //存储所有号码的嵌墨模板
	BOOL			bLeftDown;                             //表明左键是否按下

	CPoint ptPre1,ptCur1;                                  //临时的画点坐标1

	CPoint ptPre2,ptCur2;                                  //临时的画点坐标2 

	CPoint	ptAddCurPos;                                    //新添加节点、区域的坐标

	BOOL bAddNodePoint,bDelNodePoint;                      //判断当前鼠标左键按下时的操作
	CArray<LINEVAL,LINEVAL> ptLineArray;                    //连续线段列表
	int			m_nCurNodeLineNum;                           //线段个数
	//  [12/11/2007]
	s_CodeMoreInkRegion		m_sCurCodeRegn[MaxRegionNum];           //嵌墨权重区域，以像素为单位
	int			m_nCurSelRegionIndex;                               //当前选择的区域号
	//  [12/13/2007]
	CRgn		m_rgnCurCodeTmp[MaxRegionNum];              //权重区域数组，用于判断当前点是否在该区域内
	CPoint		m_ptAddNewRegion;                          //创建新区域的中心点
public:
	void InitTmpMoreInkTmpData();
	PBYTE		m_lpImageData;                             //图象数据
	CSize		m_szCurImage;                               //图像大小

	int			m_nCurCodeIndex;     //当前号码的索引号，0-9的号就是0－9；‘A'－’Z‘加上10

	COcrProExtern *m_OcrProExt;// 全局参数封装指针 [9/30/2011 Denghl]
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
