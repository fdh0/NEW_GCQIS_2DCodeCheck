// Check.h: interface for the CCheck class.
//
//////////////////////////////////////////////////////////////////////

/************************************************************************
/* Copyright (c) 2008, �������ͼ���Ӿ����޹�˾�Ӿ��㷨��    
/* All rights reserved.             
/*                  
/* �ļ����ƣ�  Check.h
/* ժҪ�� �ڲ��㷨����ʵ�֡�
/*
/* ��ǰ�汾�� 1.0
/* �����ߣ� �˺���
/* ������ڣ� 2011��5��10��
/* ˵�����ڲ��㷨���
************************************************************************/

#if !defined(AFX_CINNERCHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)
#define AFX_CINNERCHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DHTemplate.h"
#include "FileOperate.h"

////////////////

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#define RECTBOUND(rcCode,rcBound)  CRect(max(rcCode.left,rcBound.left),max(rcCode.top,rcBound.top),min(rcCode.right,rcBound.right),min(rcCode.bottom,rcBound.bottom))


#define    NORMAL_2D      0       
#define    EMPTY_2D       1         
#define    WRONGNUM_2D    2         
#define    ABNORMAL_2D    3 
#define    QRLOCERR       4
#define    OUTRANGE       5
#define    INREGION       6
#define    HORVER         7
#define    PRINTTRY       8
#define    GRADELOW       9
#define    ZOUBAN         10
#define    TIMEOUT        11
#define    MAXCODENUM     1024

// �����ṩ��̬��У���ά�� [4/5/2012 Denghl]
// �����ļ�ͷ�ļ����� [4/26/2012 Denghl]
// ��Ʒ���Ͳ��������������й�����Ϣ
typedef struct S_RMBPRODUCT_PARAM
{
	int iMaxCol;
	int iMaxRow;
	int iTotalPageCount;
	int iStartX;
	int iStartY;
	char * chPrdFormat;
}s_ProductParam;



//�ڲ����������������
typedef struct _INNER_CHECK_INPUTPARA
{
	s_RegionProperty sRegionProperty;
	s_LocGroupResult sLocGroupResult;
	s_AlgImageInfo  sAlgImageInfo;
	UINT nPieceStudy;     //���㷨ѧϰ����
	BOOL bTrainCancel;    //��ֹѧϰ TRUE:��ֹ FASLE������ֹ
	BOOL nStartStatus;
	_INNER_CHECK_INPUTPARA()
	{
		nPieceStudy = 0;
		bTrainCancel = FALSE;
		nStartStatus = FALSE;
	}
	_INNER_CHECK_INPUTPARA(const _INNER_CHECK_INPUTPARA& sPara)
	{
		*this = sPara;
	}
	_INNER_CHECK_INPUTPARA& operator=(const _INNER_CHECK_INPUTPARA& sPara)
	{
		sRegionProperty = sPara.sRegionProperty;
		sLocGroupResult = sPara.sLocGroupResult;
		sAlgImageInfo = sPara.sAlgImageInfo;
		nPieceStudy = sPara.nPieceStudy;
		bTrainCancel = sPara.bTrainCancel;
		nStartStatus = sPara.nStartStatus;
		return (*this);
	}
	void lst_vec_clear()
	{
		sRegionProperty.lst_vec_clear();
		sLocGroupResult.lst_vec_clear();
		sAlgImageInfo.lst_vec_clear();

	}
}s_InnerCheckInputParam;

// QR�붨λ��־��̬��� [9/13/2013 Denghl]
typedef struct _QRLOC_SHAPE_PARAM
{
	BOOL bValid;// �Ƿ���	

	_QRLOC_SHAPE_PARAM()
	{
		bValid = FALSE;
	}
	_QRLOC_SHAPE_PARAM(const _QRLOC_SHAPE_PARAM& sPara)
	{
		*this = sPara;
	}
	_QRLOC_SHAPE_PARAM& operator=(const _QRLOC_SHAPE_PARAM& sPara)
	{
		bValid = sPara.bValid;

		return (*this);
	}
	void lst_vec_clear()
	{
		
	}
}s_QrlocShapeParam;


// ��ά����������ͷ���ϴ�����ˮƽ��ֱ���� [9/13/2013 Denghl]
typedef struct _IN_HORVER_PARAM
{
	BOOL bValid;// �Ƿ���	
	int nDirect; // ����
	int nArea; //���
	int nPercent; //�׺ڰٷֱ�
	int nContrast;

	_IN_HORVER_PARAM()
	{
		bValid = FALSE;
		nDirect = 1;
		nArea = 1;
		nPercent = 70;
		nContrast = 3;
	}
	_IN_HORVER_PARAM(const _IN_HORVER_PARAM& sPara)
	{
		*this = sPara;
	}
	_IN_HORVER_PARAM& operator=(const _IN_HORVER_PARAM& sPara)
	{
		bValid = sPara.bValid;
		nDirect = sPara.nDirect;
		nArea = sPara.nArea;
		nPercent = sPara.nPercent;
		nContrast = sPara.nContrast;
	
		return (*this);
	}
}s_InHorverParam;
typedef struct _CORR_PIXEL_PARAM
{
	BOOL bValid;
	int  nCorrPixels;
	_CORR_PIXEL_PARAM()
	{
		bValid = FALSE;
		nCorrPixels = 0;
	}
	_CORR_PIXEL_PARAM(const _CORR_PIXEL_PARAM& sPara)
	{
		*this = sPara;
	}
	_CORR_PIXEL_PARAM&operator=(const _CORR_PIXEL_PARAM& sPara)
	{
		bValid = sPara.bValid;
		nCorrPixels = sPara.nCorrPixels;
		return(*this);
	}

}s_CorrPixelParam;
// ��ά���߰��� [9/13/2013 Denghl]
typedef struct _ZOUBAN_PARAM
{
	BOOL bValid;// �Ƿ���	
	BOOL bIsInLoc;//�Ƿ��ڲ���λ
	int nSearchX;
	int nSearchY;
	//  [11/3/2015 zl]
	//  [11/3/2015 zl]
	BOOL m_bIsCheckZouBanUp;
	BOOL m_bIsCheckZouBanDown;
	BOOL m_bIsCheckZouBanLeft;
	BOOL m_bIsCheckZouBanRight;
	double m_fZouBanStdVer;
	double m_fZouBanStdHor;
	double m_fZouBanRange;
	BOOL m_bIsWhiteZouBan;
	int m_nLeftPos;
	int m_nRightPos;
	int m_nUpPos;
	int m_nDownPos;

	BOOL  bLRChangeZouban;  //���Ұ׿����Ƿ��޸�
	BOOL  bUDChangeZouban;  //���°׿����Ƿ��޸�

	_ZOUBAN_PARAM()
	{
		bValid = FALSE;
		bIsInLoc = TRUE;
		nSearchX = 50;
		nSearchY = 50;
		m_bIsCheckZouBanUp=0;
		m_bIsCheckZouBanDown=0;
		m_bIsCheckZouBanLeft=0;
		m_bIsCheckZouBanRight=0;
		m_fZouBanStdVer=0;
		m_fZouBanStdHor=0;
		m_fZouBanRange=0;
		
		m_nLeftPos=0;
		m_nRightPos=0;
		m_nUpPos=0;
		m_nDownPos=0;

		m_bIsWhiteZouBan = FALSE;
		bLRChangeZouban  = FALSE;
		bUDChangeZouban  = FALSE;
	}
	_ZOUBAN_PARAM(const _ZOUBAN_PARAM& sPara)
	{
		*this = sPara;
	}
	_ZOUBAN_PARAM& operator=(const _ZOUBAN_PARAM& sPara)
	{
		bValid = sPara.bValid;
		bIsInLoc = sPara.bIsInLoc;
		nSearchX = sPara.nSearchX;
		nSearchY = sPara.nSearchY;
		//  [11/3/2015 zl]
		m_bIsCheckZouBanUp=sPara.m_bIsCheckZouBanUp;
		m_bIsCheckZouBanDown=sPara.m_bIsCheckZouBanDown;
		m_bIsCheckZouBanLeft=sPara.m_bIsCheckZouBanLeft;
		m_bIsCheckZouBanRight=sPara.m_bIsCheckZouBanRight;
		m_fZouBanStdVer=sPara.m_fZouBanStdVer;
		m_fZouBanStdHor=sPara.m_fZouBanStdHor;
		m_fZouBanRange=sPara.m_fZouBanRange;
		m_bIsWhiteZouBan=sPara.m_bIsWhiteZouBan;
		m_nLeftPos=sPara.m_nLeftPos;
		m_nRightPos=sPara.m_nRightPos;
		m_nUpPos=sPara.m_nUpPos;
		m_nDownPos=sPara.m_nDownPos;
		bLRChangeZouban = sPara.bLRChangeZouban;
		bUDChangeZouban = sPara.bUDChangeZouban;
		return (*this);
	}
}s_ZouBanParam;

typedef struct _TIME_OUT
{
	BOOL bValid;// �Ƿ���	
	int nSingleTime;
	int nAllTime;

	_TIME_OUT()
	{
		bValid = FALSE;
		nSingleTime = 10;
		nAllTime = 600;
	}
	_TIME_OUT(const _TIME_OUT& sPara)
	{
		*this = sPara;
	}
	_TIME_OUT& operator=(const _TIME_OUT& sPara)
	{
		bValid = sPara.bValid;
		nSingleTime = sPara.nSingleTime;
		nAllTime = sPara.nAllTime;

		return (*this);
	}
}s_TimeOut;

// ��ά��������Χ����ȱӡ [9/13/2013 Denghl]
typedef struct _IN_REGION_PARAM
{
	BOOL bValid;// �Ƿ���
	int nDeviation; // ����
	int nMaxGray; // ���Ҷ�

	_IN_REGION_PARAM()
	{
		bValid = FALSE;
		nDeviation = 5;
		nMaxGray = 90;
	}
	_IN_REGION_PARAM(const _IN_REGION_PARAM& sPara)
	{
		*this = sPara;
	}
	_IN_REGION_PARAM& operator=(const _IN_REGION_PARAM& sPara)
	{
		bValid = sPara.bValid;
		nDeviation = sPara.nDeviation;
		nMaxGray = sPara.nMaxGray;
		return (*this);
	}
}s_InRegionParam;

// ��ά��������Χ�Ƿ������� [9/13/2013 Denghl]
typedef struct _OUT_RANGE_PARAM
{
	BOOL bValid;// �Ƿ���
	int nLeft;
	int nTop;
	int nRight;
	int nBottom;
	int nSingleArea;// ������С���
	int nAllArea;//�����
	_OUT_RANGE_PARAM()
	{
		bValid = FALSE;
		nLeft = 100;
		nRight = 100;
		nTop = 100;
		nBottom = 100;
		nSingleArea = 3;
		nAllArea = 30;
	}
	_OUT_RANGE_PARAM(const _OUT_RANGE_PARAM& sPara)
	{
		*this = sPara;
	}
	_OUT_RANGE_PARAM& operator=(const _OUT_RANGE_PARAM& sPara)
	{
		bValid = sPara.bValid;
		nLeft = sPara.nLeft;
		nRight = sPara.nRight;
		nBottom = sPara.nBottom;
		nTop = sPara.nTop;
		nSingleArea = sPara.nSingleArea;
		nAllArea = sPara.nAllArea;
		return (*this);

	}
}s_OutRangeParam;

// ����ȼ���� [9/13/2013 Denghl]
typedef struct _DENGJI_PARAM
{
	BOOL bValid;// �Ƿ���
	UINT    nSetGrade;//�ȼ�����
	UINT nGBID;//��׼��ţ��涨0Ϊ15415,1Ϊ2006
	BOOL pbDengjiOpt[8];//�ȼ�ѡ��
	
	_DENGJI_PARAM()
	{
		bValid = FALSE;
		nSetGrade = 0;
		nGBID = 0;
		memset(pbDengjiOpt,0,8*sizeof(BOOL));
	}
	_DENGJI_PARAM(const _DENGJI_PARAM& sPara)
	{
		*this = sPara;
	}
	_DENGJI_PARAM& operator=(const _DENGJI_PARAM& sPara)
	{
		bValid = sPara.bValid;
		nSetGrade = sPara.nSetGrade;
		nGBID = sPara.nGBID;
		memcpy(pbDengjiOpt,sPara.pbDengjiOpt,8*sizeof(BOOL));
		return (*this);
	}
}s_DengjiParam;


typedef struct _CHECK_PRINT_PARAM
{
	s_OutRangeParam sOutRangPara;//����������
	s_InRegionParam sInRegionPara;// ����������ȱӡ
	s_QrlocShapeParam sQrlocShapePara;// QR��λ��̬����
	s_InHorverParam sInHorverPara;//��ͷ����ˮƽ��ֱ©ӡ
	s_DengjiParam sDengjiPara;
	s_ZouBanParam sZouBanPara;
	s_CorrPixelParam sCorrPixelParam;
	s_TimeOut sTimeOut;
	
	_CHECK_PRINT_PARAM()
	{
		
		
	}
	_CHECK_PRINT_PARAM(const _CHECK_PRINT_PARAM& sPara)
	{
		*this = sPara;
	}
	_CHECK_PRINT_PARAM& operator=(const _CHECK_PRINT_PARAM& sPara)
	{
		sOutRangPara = sPara.sOutRangPara;
		sInRegionPara = sPara.sInRegionPara;
		sInHorverPara = sPara.sInHorverPara;
		sQrlocShapePara = sPara.sQrlocShapePara;
		sDengjiPara = sPara.sDengjiPara;
		sZouBanPara = sPara.sZouBanPara;
		sCorrPixelParam = sPara.sCorrPixelParam;
		sTimeOut = sPara.sTimeOut;
		
		return (*this);
	}
}s_CheckPrintParam;

//�ڲ�������
typedef struct 
{
	s_sErrorInfo sErrorInfo; //�������Ϣ
	UINT nFlagStudy;//ѧϰ�ɹ�0��ѧϰʧ��1
	void lst_vec_clear()
	{
		if (0 < sErrorInfo.vcCheckResultCode.capacity())
		{
			//vcCheckResultCode.clear();
			vector<char>().swap(sErrorInfo.vcCheckResultCode); 
		}
	}

}s_InnerCheckOutputParam;

void inline MyHalconExceptionHandler(const HException& hException)
{
	throw hException;
}

class CInnerCheck // ���ʵ����
{
public:
	CInnerCheck();
	virtual ~CInnerCheck();
	std::list<CString> *m_lstStr;
	//  [10/15/2015 zl]
	CCriticalSection *m_csWriteLog;
	
	s_Status m_sStatus;// ����ִ��״̬ [5/12/2011]
	CString m_strTaskPath;//����·��
	CString m_strParaIni;//���������ļ�·��
	CString m_strRegionPath;//����·��
	CString m_strAlgName;//�㷨��
	CString m_strAlgPublicLib;//�㷨���ÿ�·��
	CString m_strDefaultParaPath;//�㷨Ĭ�ϲ���·��
	s_RegionProperty m_sDstRegionProperty;//������Ϣ
	s_AlgInitParam m_sAlgInitParam;//ȫ���㷨·��
	CString m_strDefaultParaIni;// ȫ��Ĭ�ϲ���·��
	CFileOperate FileOperate;//�ļ�����
	CString m_strLogPath;// ��־·�� [11/12/2011 Denghl]
	
	//eTrainFlag ETFPreStatus;
	//eOperateFlag OPFPreStatus;

	//CString m_strValidPath;// ������ģ�����Ч·�� [11/10/2011 Denghl]
	CRect m_rcAffineTransReg;// �������任����С������
	CRect m_rcOrgMinRect;
	//int m_nTrainStatus;      //0 ��ѧϰ��//1ѧϰ��ʼ��2 ѧϰ������ 3 ѧϰ���� 4 ȡ��ѧϰ
	//int m_nTrainNum;         //����ѧϰ������

	int m_iCheckMethod; //��ⷽ�������ά���ͺ�
	int m_iRealMethod;
	BOOL m_bCheckAllMod;
	BOOL m_bCheckGao;
//	CString m_strProductType;//��Ʒ�ͺ�
//	CString m_strWordArray;//�����ַ���

//	int m_nProductNum;// ��֧�ֲ�Ʒ������ [4/7/2012 Denghl]
//	std::vector<CString> m_vcstrAllProType;// ���в�Ʒ����
//	s_ProductParam *m_sProductParam;//��Ʒ����

	int m_nChannel;// ѡȡͼ����ɫͨ�� [4/5/2012 Denghl]
	//BOOL m_bUseDll;// �Ƿ�ʹ��У���

	// �����ļ� [4/26/2012 Denghl]
	//HINSTANCE m_hDll;//���ܶ�̬����
	//lpCheckBarCodeFun checkBarCodeFun;
	//lpFreeRMBProductInfoMemFun freeRMBProductInfoMemFun;
	//lpFreeRMBProductParamMemFun freeRMBProductParamMemFun;
	//lpGetRMBProductInfoFun getRMBProductInfoFun;
	//lpGetRMBProductParamFun getRMBProductParamFun;

	BOOL m_bReadMode;
	HTuple hDataCodeHandle,hResultHandles;
	HTuple phDataCodeHandle[3];// ��֪����ά�����͵ģ����к�������ȫ�����һ�� [4/5/2012 Denghl]
	HObject  hSymbolXLDs, hSymbolXLD;
	HObject m_hImageCheck;
	
	HObject  m_ImageRotate,m_RegionAffineTrans;
	HObject m_InRegion,m_RegionOutRang,m_InRegionQue,m_RegionOutRect,m_InWhiteLine;
    HTuple hv_Indices, hv_IndicesVer, hv_Mean, hv_Deviation,hv_AreaAll;
	int     m_nHorverType;  //��ͷ���������31,�׵㣻32,����

	int m_nRealGrade;
	CString m_strRealGrade;

	s_CheckPrintParam m_sCheckPrintParam;
	BOOL m_bShowDJ;
	double m_fScaleHor;
	double m_fScaleVer;

	// �߰����� [7/31/2014 Denghl]
	HObject m_hZoubanRegion,m_hRealZoubanRegion;
	HObject m_hInlocMarkRegion,m_hInlocSearchRegion;
	HTuple m_lMarkID;
	//  [11/4/2015 zl]
	HObject m_hRealZoubanRegionL,m_hRealZoubanRegionR,m_hRealZoubanRegionU,m_hRealZoubanRegionD;

	//  [10/14/2015 BaiYY]
	HTuple m_hWhiteLineValue;

	// ��ά�뼫�� [12/21/2015 TIAN]
	CString m_strPolar;
	

	BOOL m_bIsAgainCheck;// �Ƿ���κ˲� [3/28/2012 Denghl]
	BOOL m_bCheckResultMode;// �����ģʽ��Ĭ��Ϊ1����ȱ��Сͼ���;1ΪС�����
	//BOOL m_bAlignCorrected;//�Ƿ���д�λУ��
	//int  m_nCorrectPixels;

	BOOL m_bCheckPartCode;//�Ƿ�ʶ�𲿷ֺ�����
	int  m_CodeCharL;
	int  m_CodeCharH;
	BOOL m_bIsTurnedResult;
	BOOL m_bCheckPrint;
	HTuple m_htVersion,m_htModeType,m_htGrade,m_htErrorC,m_htMask;

	// ���ͼ��ƽ������ [12/30/2015 TIAN]
	BOOL m_bIsImageSmooth;

	// �����Ƿ�д�����־ [9/29/2015 TIAN]
	BOOL m_bWriteLog; 
	CFileOperate m_FileOperate;
	std::vector<char> m_CheckResultCode;
	//  [11/13/2015 zl]�ж��Ƿ��Ѿ��Զ���ȡ��
	//BOOL m_bIsAutoExtL;
	//BOOL m_bIsAutoExtR;
	//BOOL m_bIsAutoExtU;
	//BOOL m_bIsAutoExtD;
	//  [1/6/2016 TIAN]
	BOOL    m_bIsZoomCheck;    //�Ƿ�ʹ��ѹ�����
	double  m_fZoomRadio;      //���ű���
	BOOL    m_bZoomCheckFlag;  //�Ƿ��ѽ��й�ѹ�����
	double  m_dHorEnlargeRate; //����Ŵ����
	double  m_dVerEnlargeRate; //����Ŵ���� 
	UINT m_nResultNum;//���������λ��

	
public:

	//��ʼ��
	s_Status Init(const char *strTaskName, const s_AlgInitParam &sAlgInitParam,const s_RegionProperty &sDstRegionProperty,BOOL bIsNewRegion);
	
	// �ͷ�
	s_Status Free();

    // ��⺯��
	s_Status Check(const s_InnerCheckInputParam& sAlgInnerCheckInputParam,const HObject hSrcImage,s_InnerCheckOutputParam *sAlgInnerCheckOutputResult);
	
	//������ģ��,ѧϰ�����
	s_Status SaveCheckModel(const char *strTaskName);
	
	// ���ؼ������ģ��ͼ
	s_Status GetCheckModelImg(const UINT iTaskImgType, s_TaskImageInfo &sTaskImgInfo);
	int UTF8ToGBK(unsigned char * lpUTF8Str,unsigned char * lpGBKStr,int nGBKStrLen);


public:

	// д�����ļ�
	void WritePara(CString strFileName);
	void WriteTypePara();

	//  [10/14/2015 zl]
	BOOL GenCurLog(CString &str,int nLogType);
	// дĬ�ϲ����ļ�
	void WriteAddDefaultPara();

	// �������ļ�
	void ReadPara(CString strParaIni);

	// �����Ѿ��޸��������ʱ�ļ�
	int SaveTempPara();

	// ��ȡ�Ѿ��޸��������ʱ�ļ� 	
	int ReadTempPara();

	// �ͷ��ڴ�ռ�
	void FreeTMPSpace();

	// �����ڴ�ռ�
	void RequistSpace();

	int GenCheckImage(const s_InnerCheckInputParam& sAlgInnerCheckInputParam,const HObject hSrcImage);

	//��������
	s_Status AlgCopyRegion(const s_AlgImageInfo& sAlgImageInfo,
		s_RegionProperty &sDstRegionProperty,const s_RegionProperty &sSrcRegionProperty);
	void FreeCheckMethod();
	void MallocCheckMethod();
	int CheckPrint();
	int CheckGrade();//���ȼ�
	int ExWhiteEdge(HObject ho_Src,HTuple htWidth,int nEdgeType);
	void ReadSimplePara();
	void WriteSimplePara();
	//  [4/7/2017 zb]
	int     m_nLanguage;             // ����
	CString m_strMessagePath;        // ��Ӣ����־����Ϣ����·��
	char m_strMessageText[512][256];
};


#endif // !defined(AFX_CINNERCHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)


