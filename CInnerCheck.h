// Check.h: interface for the CCheck class.
//
//////////////////////////////////////////////////////////////////////

/************************************************************************
/* Copyright (c) 2008, 北京大恒图像视觉有限公司视觉算法部    
/* All rights reserved.             
/*                  
/* 文件名称：  Check.h
/* 摘要： 内部算法具体实现。
/*
/* 当前版本： 1.0
/* 创建者： 邓红丽
/* 完成日期： 2011年5月10日
/* 说明：内部算法检测
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

// 北钞提供动态库校验二维码 [4/5/2012 Denghl]
// 加密文件头文件定义 [4/26/2012 Denghl]
// 产品类型参数，涵盖了所有规律信息
typedef struct S_RMBPRODUCT_PARAM
{
	int iMaxCol;
	int iMaxRow;
	int iTotalPageCount;
	int iStartX;
	int iStartY;
	char * chPrdFormat;
}s_ProductParam;



//内部参数设置输入参数
typedef struct _INNER_CHECK_INPUTPARA
{
	s_RegionProperty sRegionProperty;
	s_LocGroupResult sLocGroupResult;
	s_AlgImageInfo  sAlgImageInfo;
	UINT nPieceStudy;     //大算法学习张数
	BOOL bTrainCancel;    //终止学习 TRUE:终止 FASLE：非终止
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

// QR码定位标志形态检测 [9/13/2013 Denghl]
typedef struct _QRLOC_SHAPE_PARAM
{
	BOOL bValid;// 是否检测	

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


// 二维码内由于喷头堵上带来的水平或垂直白条 [9/13/2013 Denghl]
typedef struct _IN_HORVER_PARAM
{
	BOOL bValid;// 是否检测	
	int nDirect; // 方向
	int nArea; //面积
	int nPercent; //白黑百分比
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
// 二维码走版检测 [9/13/2013 Denghl]
typedef struct _ZOUBAN_PARAM
{
	BOOL bValid;// 是否检测	
	BOOL bIsInLoc;//是否内部定位
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

	BOOL  bLRChangeZouban;  //左右白框检测是否修改
	BOOL  bUDChangeZouban;  //上下白框检测是否修改

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
	BOOL bValid;// 是否检测	
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

// 二维码内区域范围严重缺印 [9/13/2013 Denghl]
typedef struct _IN_REGION_PARAM
{
	BOOL bValid;// 是否检测
	int nDeviation; // 方差
	int nMaxGray; // 最大灰度

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

// 二维码外区域范围是否存在脏点 [9/13/2013 Denghl]
typedef struct _OUT_RANGE_PARAM
{
	BOOL bValid;// 是否检测
	int nLeft;
	int nTop;
	int nRight;
	int nBottom;
	int nSingleArea;// 单个最小面积
	int nAllArea;//总面积
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

// 条码等级检测 [9/13/2013 Denghl]
typedef struct _DENGJI_PARAM
{
	BOOL bValid;// 是否检测
	UINT    nSetGrade;//等级设置
	UINT nGBID;//标准序号，规定0为15415,1为2006
	BOOL pbDengjiOpt[8];//等级选项
	
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
	s_OutRangeParam sOutRangPara;//区域外脏检测
	s_InRegionParam sInRegionPara;// 区域内严重缺印
	s_QrlocShapeParam sQrlocShapePara;// QR定位形态查找
	s_InHorverParam sInHorverPara;//喷头堵上水平垂直漏印
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

//内部输出结果
typedef struct 
{
	s_sErrorInfo sErrorInfo; //检测结果信息
	UINT nFlagStudy;//学习成功0，学习失败1
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

class CInnerCheck // 检测实现类
{
public:
	CInnerCheck();
	virtual ~CInnerCheck();
	std::list<CString> *m_lstStr;
	//  [10/15/2015 zl]
	CCriticalSection *m_csWriteLog;
	
	s_Status m_sStatus;// 该类执行状态 [5/12/2011]
	CString m_strTaskPath;//任务路径
	CString m_strParaIni;//参数配置文件路径
	CString m_strRegionPath;//区域路径
	CString m_strAlgName;//算法名
	CString m_strAlgPublicLib;//算法公用库路径
	CString m_strDefaultParaPath;//算法默认参数路径
	s_RegionProperty m_sDstRegionProperty;//区域信息
	s_AlgInitParam m_sAlgInitParam;//全局算法路径
	CString m_strDefaultParaIni;// 全局默认参数路径
	CFileOperate FileOperate;//文件操作
	CString m_strLogPath;// 日志路径 [11/12/2011 Denghl]
	
	//eTrainFlag ETFPreStatus;
	//eOperateFlag OPFPreStatus;

	//CString m_strValidPath;// 参数和模板的有效路径 [11/10/2011 Denghl]
	CRect m_rcAffineTransReg;// 区域仿射变换后，最小外界矩形
	CRect m_rcOrgMinRect;
	//int m_nTrainStatus;      //0 不学习，//1学习开始，2 学习进行中 3 学习结束 4 取消学习
	//int m_nTrainNum;         //正在学习的张数

	int m_iCheckMethod; //检测方法，或二维码型号
	int m_iRealMethod;
	BOOL m_bCheckAllMod;
	BOOL m_bCheckGao;
//	CString m_strProductType;//产品型号
//	CString m_strWordArray;//冠字字符串

//	int m_nProductNum;// 所支持产品种类数 [4/7/2012 Denghl]
//	std::vector<CString> m_vcstrAllProType;// 所有产品序列
//	s_ProductParam *m_sProductParam;//产品类型

	int m_nChannel;// 选取图像颜色通道 [4/5/2012 Denghl]
	//BOOL m_bUseDll;// 是否使用校验库

	// 加密文件 [4/26/2012 Denghl]
	//HINSTANCE m_hDll;//加密动态库句柄
	//lpCheckBarCodeFun checkBarCodeFun;
	//lpFreeRMBProductInfoMemFun freeRMBProductInfoMemFun;
	//lpFreeRMBProductParamMemFun freeRMBProductParamMemFun;
	//lpGetRMBProductInfoFun getRMBProductInfoFun;
	//lpGetRMBProductParamFun getRMBProductParamFun;

	BOOL m_bReadMode;
	HTuple hDataCodeHandle,hResultHandles;
	HTuple phDataCodeHandle[3];// 不知道二维码类型的，所有号码类型全部检测一遍 [4/5/2012 Denghl]
	HObject  hSymbolXLDs, hSymbolXLD;
	HObject m_hImageCheck;
	
	HObject  m_ImageRotate,m_RegionAffineTrans;
	HObject m_InRegion,m_RegionOutRang,m_InRegionQue,m_RegionOutRect,m_InWhiteLine;
    HTuple hv_Indices, hv_IndicesVer, hv_Mean, hv_Deviation,hv_AreaAll;
	int     m_nHorverType;  //喷头阻塞的类别：31,白点；32,白线

	int m_nRealGrade;
	CString m_strRealGrade;

	s_CheckPrintParam m_sCheckPrintParam;
	BOOL m_bShowDJ;
	double m_fScaleHor;
	double m_fScaleVer;

	// 走版设置 [7/31/2014 Denghl]
	HObject m_hZoubanRegion,m_hRealZoubanRegion;
	HObject m_hInlocMarkRegion,m_hInlocSearchRegion;
	HTuple m_lMarkID;
	//  [11/4/2015 zl]
	HObject m_hRealZoubanRegionL,m_hRealZoubanRegionR,m_hRealZoubanRegionU,m_hRealZoubanRegionD;

	//  [10/14/2015 BaiYY]
	HTuple m_hWhiteLineValue;

	// 二维码极性 [12/21/2015 TIAN]
	CString m_strPolar;
	

	BOOL m_bIsAgainCheck;// 是否二次核查 [3/28/2012 Denghl]
	BOOL m_bCheckResultMode;// 检测结果模式，默认为1，存缺陷小图结果;1为小开结果
	//BOOL m_bAlignCorrected;//是否进行错位校正
	//int  m_nCorrectPixels;

	BOOL m_bCheckPartCode;//是否识别部分号码结果
	int  m_CodeCharL;
	int  m_CodeCharH;
	BOOL m_bIsTurnedResult;
	BOOL m_bCheckPrint;
	HTuple m_htVersion,m_htModeType,m_htGrade,m_htErrorC,m_htMask;

	// 添加图像平滑处理 [12/30/2015 TIAN]
	BOOL m_bIsImageSmooth;

	// 控制是否写检测日志 [9/29/2015 TIAN]
	BOOL m_bWriteLog; 
	CFileOperate m_FileOperate;
	std::vector<char> m_CheckResultCode;
	//  [11/13/2015 zl]判断是否已经自动提取过
	//BOOL m_bIsAutoExtL;
	//BOOL m_bIsAutoExtR;
	//BOOL m_bIsAutoExtU;
	//BOOL m_bIsAutoExtD;
	//  [1/6/2016 TIAN]
	BOOL    m_bIsZoomCheck;    //是否使能压缩检测
	double  m_fZoomRadio;      //缩放比率
	BOOL    m_bZoomCheckFlag;  //是否已进行过压缩检测
	double  m_dHorEnlargeRate; //横向放大比率
	double  m_dVerEnlargeRate; //纵向放大比率 
	UINT m_nResultNum;//检测结果检验位数

	
public:

	//初始化
	s_Status Init(const char *strTaskName, const s_AlgInitParam &sAlgInitParam,const s_RegionProperty &sDstRegionProperty,BOOL bIsNewRegion);
	
	// 释放
	s_Status Free();

    // 检测函数
	s_Status Check(const s_InnerCheckInputParam& sAlgInnerCheckInputParam,const HObject hSrcImage,s_InnerCheckOutputParam *sAlgInnerCheckOutputResult);
	
	//保存检测模板,学习后调用
	s_Status SaveCheckModel(const char *strTaskName);
	
	// 返回检测区域模板图
	s_Status GetCheckModelImg(const UINT iTaskImgType, s_TaskImageInfo &sTaskImgInfo);
	int UTF8ToGBK(unsigned char * lpUTF8Str,unsigned char * lpGBKStr,int nGBKStrLen);


public:

	// 写参数文件
	void WritePara(CString strFileName);
	void WriteTypePara();

	//  [10/14/2015 zl]
	BOOL GenCurLog(CString &str,int nLogType);
	// 写默认参数文件
	void WriteAddDefaultPara();

	// 读参数文件
	void ReadPara(CString strParaIni);

	// 保存已经修改区域的临时文件
	int SaveTempPara();

	// 读取已经修改区域的临时文件 	
	int ReadTempPara();

	// 释放内存空间
	void FreeTMPSpace();

	// 申请内存空间
	void RequistSpace();

	int GenCheckImage(const s_InnerCheckInputParam& sAlgInnerCheckInputParam,const HObject hSrcImage);

	//复制区域
	s_Status AlgCopyRegion(const s_AlgImageInfo& sAlgImageInfo,
		s_RegionProperty &sDstRegionProperty,const s_RegionProperty &sSrcRegionProperty);
	void FreeCheckMethod();
	void MallocCheckMethod();
	int CheckPrint();
	int CheckGrade();//检测等级
	int ExWhiteEdge(HObject ho_Src,HTuple htWidth,int nEdgeType);
	void ReadSimplePara();
	void WriteSimplePara();
	//  [4/7/2017 zb]
	int     m_nLanguage;             // 语言
	CString m_strMessagePath;        // 中英文日志或消息配置路径
	char m_strMessageText[512][256];
};


#endif // !defined(AFX_CINNERCHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)


