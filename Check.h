// Check.h: interface for the CCheck class.
//
//////////////////////////////////////////////////////////////////////

/************************************************************************
/* Copyright (c) 2008, 北京大恒图像视觉有限公司视觉算法部    
/* All rights reserved.             
/*                  
/* 文件名称：  Check.h
/* 摘要： 算法具体实现。
/*
/* 当前版本： 1.0
/* 创建者： 邓红丽
/* 完成日期： 2011年5月10日
/* 说明： 针对三个接口类的具体实现类及成员函数
************************************************************************/

#if !defined(AFX_CHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)
#define AFX_CHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AlgVirtualBaseClass.h"
#include "CInnerCheck.h"
#include "FileOperate.h"
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#include "SpendTime.h"
using namespace std;

#define ALG_TYPE_ID  3
// 定义内部算法支持的算法类型,注意，各算法可根据需求修改
#define Inner_Type1 1 //支持的第一种算法
#define Inner_Type2 2 //支持的第二种算法
#define Inner_Type3 3 //支持的第三种算法
#define ALG_SUM 0     //支持内部算法种数
//...
static  char strInnerTypeName[20][50]=          // 内部算法类型名列表
{
	"内部算法名一",
	"内部算法名二",
	"内部算法名三"
};
static BOOL bInnerTypeStudyFlag[20]=          // 对应算法类型名是否支持学习
{
	TRUE,
	TRUE,
	FALSE
};
static BOOL bInnerTypeUPDlgFlag[20]=           // 对应算法类型名是否可弹学习对话框
{
	FALSE,
	TRUE,
	FALSE
};
//内部算法支持类型设置结束
typedef struct _INNER_CHECK_INFO
{
	S_SingleCellProperty sSingleCellPro;//小开信息
	std::list<CInnerCheck> lstInnerCheck;//小开内检测链表
	_INNER_CHECK_INFO()
	{
		lstInnerCheck.clear();
	}
	~_INNER_CHECK_INFO()
	{
		lstInnerCheck.clear();
	}
	_INNER_CHECK_INFO(const _INNER_CHECK_INFO& sSP)
	{
		*this = sSP;
	}
	_INNER_CHECK_INFO& operator=(const _INNER_CHECK_INFO& sSCPro)
	{
		sSingleCellPro = sSCPro.sSingleCellPro;
		lstInnerCheck = sSCPro.lstInnerCheck;
		return *this;
	}
	void lst_vec_clear()
	{
		if (0 < lstInnerCheck.size())
		{
			std::list<CInnerCheck>::iterator itInner;
			for (itInner=lstInnerCheck.begin();
				itInner!=lstInnerCheck.end();
				itInner++)
			{
				itInner->Free();
			}
			
		}
		lstInnerCheck.clear();
	}
}s_InnerCheckInfo;// 内部检测对象 [11/14/2011 Denghl]

typedef struct _ALG_SIGLECELLOrREGIONSINFO
{
	int nCellIndex;		//小开属性
	HObject HRegCell;							//区域在采集图像中的原始坐标

}sAlgCellInfo;

typedef struct _SIMPLE_PARA
{	
	BOOL bSkipCheck;//缺印检测
	UINT nDeviation;//墨色差异
	UINT nPercent;//白线百分位
	BOOL bZoubanCheck;//走版检测
	double dZouBanRange;//走版公差
	BOOL bDirtCheck;//脏点检测
	double dSingleArea;//面积
	_SIMPLE_PARA()
	{
		bSkipCheck = FALSE;
		nDeviation = 8;
		nPercent = 70;
		bZoubanCheck = FALSE;
		dZouBanRange = 2;
		bDirtCheck = FALSE;
		dSingleArea = 0.01;
	}
	_SIMPLE_PARA& operator=(const _SIMPLE_PARA& sReuslt)
	{
		bSkipCheck = sReuslt.bSkipCheck;
		nDeviation = sReuslt.nDeviation;
		nPercent = sReuslt.nPercent;
		bZoubanCheck = sReuslt.bZoubanCheck;
		dZouBanRange = sReuslt.dZouBanRange;
		bDirtCheck = sReuslt.bDirtCheck;
		dSingleArea = sReuslt.dSingleArea;		
		return (*this);

	}
}s_SimplePara;

class CCheck : public CAlgVirtualCheck  // 检测实现类
{
public:
	CCheck();
	virtual ~CCheck();

	s_Status m_sStatus;// 该类执行状态 [5/12/2011]
	CString m_strTaskPath;//任务全路径
	CString m_strTaskName;//任务名
	s_AlgInitParam m_sAlgInitParam;//全局算法路径
	CString m_strDefaultParaIni;// 全局默认参数路径
	CString m_strAlgName;//算法名

	s_DH_Template m_sDHTemplate;//接收到的模板（检测的时候是变化的模板）
    std::list<s_InnerCheckInfo> m_lstSingleCellInnerCheck;// 检测小开链表 [11/14/2011 Denghl]
	int m_nRegionNum;//区域个数
	s_MiddleResult m_sMiddleResult;//中间结果

	CSpendTime sPendTime;

	s_AlgImageInfo m_sAlgImageInfo;//算法检测需要的图像信息
	s_EachAlgLearnInfo m_sEachAlgLearnInfo;//算法需要的学习输入信息
	std::list<s_EachAlgLearnInfo> m_lstEachAlgLearnInfo;//接受学习信息链表

	int m_nStudyRegNum;//每大张学习成功区域数

	CFileOperate m_FileOperate;//for FileOperate,like read,write file
	CString m_strLogPath;//异常日志路径

	s_CheckResult m_sCheckResult;
	int nStart ;
	// 控制是否写检测日志 [9/29/2015 TIAN]
	BOOL m_bWriteLog;
	BOOL m_bCheckTimeOut;
	int m_nAllTime;
	CString m_strPathini;//记录超时整体参数

//	BOOL m_bUseDll;// 是否使用校验库
//	HINSTANCE m_hDll;//加密动态库句柄
	//BOOL m_bIsAgainCheck;// 是否二次核查 [3/28/2012 Denghl]
//	lpCheckBarCodeFun checkBarCodeFun;
//	lpFreeRMBProductInfoMemFun freeRMBProductInfoMemFun;
//	lpFreeRMBProductParamMemFun freeRMBProductParamMemFun;
//	lpGetRMBProductInfoFun getRMBProductInfoFun;
//	lpGetRMBProductParamFun getRMBProductParamFun;

//	int m_nProductNum;// 所支持产品种类数 [4/7/2012 Denghl]
//	std::vector<CString> m_vcstrAllProType;// 所有产品序列
	//  [12/2/2014 baiyy]
	std::list<sAlgCellInfo>m_lstCellReg;
	//  [10/15/2015 zl]
	//CCriticalSection *m_csWriteLog;  
	//BOOL m_bcsWriteLog;
	//CEvent m_EventThread;

	//  [11/12/2015 TIAN]
	int m_nlstStrCell;  //小开开辟list<CString>的个数
	s_Status* m_sTatusArray;
	std::list<CString> *m_lstStr;
	BOOL m_blststr;

	s_SimplePara m_sSimplePara;
	//BOOL m_bIsStopThread;
	//BOOL m_bThreadRun;
	//BOOL m_bIsTheadLog;

	//  [4/6/2017 zb]
	int     m_nLanguage;              // 语言
	CString m_strMessagePath;         // 中英文日志或message配置路径
	char m_strMessageText[512][256];
//private:
	//static void WriteLogThreadFunc(LPVOID pParam);
	//void WriteLogProc();
public:

	//初始化
	s_Status Init(const s_AlgInitParam &sAlgInitParam,s_ALGCheckParam* psCheckParams,s_SysAuthority sSysAuthority);
	
	//加载任务调用
	s_Status LoadTask(const char *strTaskName, const s_DH_Template  &sDHTemplate,s_SysAuthority sSysAuthority,const BOOL bLoadFlag);
	
	//获取算法参数
	s_Status GetAlgParam(s_ALGCheckParam  * psCheckParams,s_SysAuthority sSysAuthority);

	//设置算法参数
	s_Status SetAlgParam(const s_ALGCheckParam  sCheckParams,s_SysAuthority sSysAuthority);

	// 释放
	s_Status Free(s_SysAuthority sSysAuthority);

	//设置需要的中间检测结果（如所有的定位结果）,每次检测函数之前调用
 	s_Status SetMiddleResult(const s_MiddleResult &sMiddleResult);
	
	// 检测函数
	s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage,s_AlgCheckOutputParam *sAlgCheckOutputResult);

	// 定位算法调用检测虚函数重载
	s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage,s_MiddleResult *sMiddleResult,BOOL&bIsProduct);

	////二次核查定位检测中间结果
	//s_Status Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
	//	s_MiddleResult *sMiddleResult);

	////二次核查检测函数或小开检测调用函数，非定位算法专用
	//s_Status Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
	//	s_AlgCheckOutputParam *sAlgCheckOutputResult);
	//
	//检测参数更新
	s_Status UpdateParam();
	
	//保存检测模板,学习后调用
	s_Status SaveCheckModel(const char *strTaskName,s_SysAuthority sSysAuthority);
	
	//返回检测模板大图
	s_Status GetCheckModelImg(const UINT iTaskImgType, 
		s_TaskImageInfo &sTaskImgInfo,
		s_SysAuthority sSysAuthority);
	int point_list_to_region(std::list<CPoint> vPoint, HObject &hRegion);
	int WriteTimePara(CString strPathini);

	BOOL GenCurLog(CString &str,int nLogType,int nIndex);
	BOOL WirteCurLog();
	void ReadSimplePara();
	void WriteSimplePara();
};




class CSetPROP :public CAlgVirtualPROPDLG// 属性实现类
{
public:
	CSetPROP();
	virtual ~CSetPROP();
	s_Status m_sStatus;
	CCheck *m_pChecker;
	CString m_strTaskName;//任务名
	s_AlgInitParam m_sAlgInitParam;//全局算法路径
	CString m_strAlgName;
	CFileOperate m_FileOperate;

public:
	//初始化 [5/10/2011]
	s_Status Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam);
	
	//设置区域算法属性对话框;
	s_Status SetAlgParamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionProperty,s_SysAuthority sSysAuthority);
	
	//拼接区域算法属性对话框;
	s_Status SetAlgJuncParamdlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionPropertyA,
		const s_RegionProperty& sRegionPropertyB,s_SysAuthority sSysAuthority);
	
	//复制区域
	s_Status AlgCopyRegion(const s_AlgImageInfo& sAlgImageInfo,
		s_RegionProperty &sDstRegionProperty,const s_RegionProperty &sSrcRegionProperty,int nDirection,s_SysAuthority sSysAuthority);
	
	//修改区域大小
	s_Status AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty &sSrcRegionProperty,s_SysAuthority sSysAuthority);

	//拼接区域，修改区域大小重载
	s_Status AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, 
		const s_RegionProperty &sNewRegionProperty,const s_RegionProperty &sOrgRegionProperty,s_SysAuthority sSysAuthority);
	
	//// 二次模板同步一次模板更新函数 [3/26/2012 Denghl]
	//s_Status AlgUpdataRegion(const s_AlgImageInfo& sAlgImageInfo,
	//	const list<s_RegionProperty> &lstUpdataRegion);

	//删除区域
	s_Status AlgDeleteRegion(const list<s_RegionProperty> &lstRegionProperty,s_SysAuthority sSysAuthority);
	
	//得到算法类型
	s_Status GetAlgorithmInfo(list<s_InnerAlgType>  &lstInnerAlgType);


};


class CSetPARA :public CAlgVirtualPARADLG // 参数分析实现类
{
public:
	CSetPARA();
	virtual ~CSetPARA();
	s_Status m_sStatus;
	CCheck *m_pChecker;
	CString m_strTaskName;//任务名
	s_AlgInitParam m_sAlgInitParam;//全局算法路径
	CString m_strAlgName;//算法名
	
public:	
	//初始化
	s_Status Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam);
	
	//误报分析对话框及调整检测参数
	s_Status SetCheckparamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_SigleCell_CheckResult &sSigCellCheckResult, 
		const S_SingleCell_Or_RegionsInfo &sSingleCellRegionsInfo,s_SysAuthority sSysAuthority,
		const BOOL bOpenDlg,const std::list<UINT> lstnErrorID);

	////二次核查参数对话框分析，需要重载上二次核查结果信息，二次核查结果由算法管理层往算法子层传输
	//s_Status SetCheckparamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_SigleCell_CheckResult &sSigCellCheckResult, 
	//	const s_MiddleResult &sMiddleResult,
	//	const S_SingleCell_Or_RegionsInfo &sSingleCellRegionsInfo);
	
	// 学习设置虚函数
	s_Status  SetTrainParam(const list<s_RegionProperty>& listRegionProperty,s_SysAuthority sSysAuthority);
   	
	
};
#endif // !defined(AFX_CHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)


