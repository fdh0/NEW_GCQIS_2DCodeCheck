//AlgVirtualBaseClass.h   算法各层虚拟基类
//////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Copyright (c) 2011, 北京大恒图像视觉有限公司视觉算法部    
/* All rights reserved.													
/*																		
/* 文件名称： AlgVirtualBaseClass.h												
/* 摘要：算法各层虚拟基类
/*
/* 
/* 作者邓红丽
/* 版本为 1.4
/* 版本修改1.1：
/*            增加二次模板检测或小开检测函数
/*            增加二次模板分析接口
/*            属性类和参数类初始化函数接口加上是否是二次模板的标志
/*            检测函数输入信息加上是否需要判产品标志（定位算法用）
/*            增加二次模板更新一次模板函数
版本修改1.2：
/*           新增二次核查检测定位结果接口函数
版本修改1.3：
/*           烟包二维码接口
版本修改1.4：
/*           标签算法接口
版本修改1.5：
/*           检测类的检测函数新增HObject变量，为传入的利用halcon的ConcatObj
             连起来的图像数据；
/************************************************************************/
#if !defined(_AlgVirtualBaseClass)
#define _AlgVirtualBaseClass
#include "dhtemplate.h"

/////////////////////////////////////////////////////////////////////////////
// CAlgNameCheckApp
// See AlgNameCheck.cpp for the implementation of this class
//

typedef struct _MIDDLE_RESULT
{
    std::vector<s_LocGroupResult> vsLocGroupResult;  //所有的定位信息
	void lst_vec_clear()
	{
		vsLocGroupResult.clear();
	}
}s_MiddleResult;


//检测类虚拟基
class CAlgVirtualCheck  
{
public:
	CAlgVirtualCheck()
	{
	}
	virtual ~CAlgVirtualCheck()
	{
	}

	//初始化虚函数
	virtual s_Status Init(const s_AlgInitParam &sAlgInitParam,s_ALGCheckParam* psCheckParams,s_SysAuthority sSysAuthority)=0;
	
	//加载任务调用
	virtual s_Status LoadTask(const char *strTaskName, const s_DH_Template  &sDHTemplate,
		s_SysAuthority sSysAuthority,const BOOL bLoadFlag)=0;

	//获取检测参数
	virtual s_Status GetAlgParam(s_ALGCheckParam  * psCheckParams,s_SysAuthority sSysAuthority)=0;

	//设置检测参数
	virtual s_Status SetAlgParam(const s_ALGCheckParam  sCheckParams,s_SysAuthority sSysAuthority)=0;

	//释放虚函数
	virtual s_Status Free(s_SysAuthority sSysAuthority)=0;

	//设置需要的中间检测结果（如所有的定位结果）,每次检测函数之前调用
 	virtual s_Status SetMiddleResult(const s_MiddleResult &sMiddleResult)=0;

	//检测虚函数
	virtual s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage,s_AlgCheckOutputParam *sAlgCheckOutputResult)=0;

	//定位算法调用检测虚函数重载
	virtual s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage,s_MiddleResult *sMiddleResult,BOOL&bIsProduct)=0;

	////二次核查检测函数，或小开检测函数,
	////二次核查可不调用loadtask，每次检测时算法自己调用自己loadtask
	////如果检测任务名不存在则使用前一次任务名，
	//virtual s_Status Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
	//	s_AlgCheckOutputParam *sAlgCheckOutputResult)=0;

	////二次核查定位检测中间结果
	//virtual s_Status Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
	//	s_MiddleResult *sMiddleResult)=0;

	//更新虚函数
	virtual s_Status UpdateParam()=0;

	//保存检测模板
	virtual s_Status SaveCheckModel(const char *strTaskName,s_SysAuthority sSysAuthority)=0;

	//返回检测模板大图
	virtual s_Status GetCheckModelImg(const UINT iTaskImgType, s_TaskImageInfo &sTaskImgInfo,
		s_SysAuthority sSysAuthority)=0;	

};


//////////////////////////////////////////////////////////////////////////
// 属性类虚拟基类
class CAlgVirtualPROPDLG 
{
public:
	CAlgVirtualCheck * pChecker;
	CAlgVirtualPROPDLG()
	{
	}
	virtual ~CAlgVirtualPROPDLG()
	{
	}

	//初始化,bIsAgainCheck是否是二次核查路径
	virtual s_Status Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam)=0;
	
	//设置区域算法属性对话框;
	virtual s_Status SetAlgParamDlg(const s_AlgImageInfo& sAlgImageInfo, 
		const s_RegionProperty& sRegionProperty,s_SysAuthority sSysAuthority)=0;
	
	//拼接区域算法属性对话框;
	virtual s_Status SetAlgJuncParamdlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionPropertyA,
		const s_RegionProperty& sRegionPropertyB,s_SysAuthority sSysAuthority)=0;

	//复制区域
	virtual s_Status AlgCopyRegion(const s_AlgImageInfo& sAlgImageInfo,
		s_RegionProperty &sDstRegionProperty,const s_RegionProperty &sSrcRegionProperty,int nDirection,s_SysAuthority sSysAuthority)=0;

	////二次模板更新一次模板区域，从一次模板中拷贝一个区域到对应二次模板区域中
	//virtual s_Status AlgUpdataRegion(const s_AlgImageInfo& sAlgImageInfo,
	//	const list<s_RegionProperty> &lstUpdataRegion)=0;
	
	//修改区域大小
	virtual s_Status AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty &sNewRegionProperty,s_SysAuthority sSysAuthority)=0;

	//拼接区域，修改区域大小重载
	virtual s_Status AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, 
		const s_RegionProperty &sNewRegionProperty,const s_RegionProperty &sOrgRegionProperty,s_SysAuthority sSysAuthority)=0;
	
	//删除区域
	virtual s_Status AlgDeleteRegion(const list<s_RegionProperty> &lstRegionProperty,s_SysAuthority sSysAuthority)=0;

	//得到算法类型
	virtual s_Status GetAlgorithmInfo(list<s_InnerAlgType>  &lstInnerAlgType)=0;

};


//////////////////////////////////////////////////////////////////////////
// 参数类虚拟基类
class CAlgVirtualPARADLG 
{
public:
	CAlgVirtualCheck * pChecker;
	CAlgVirtualPARADLG()
	{
	}
	virtual ~CAlgVirtualPARADLG()
	{
	}


	//初始化,bIsAgainCheck是否是二次核查路径
	virtual s_Status Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam)=0;

	//误报分析对话框及调整检测参数
	virtual s_Status SetCheckparamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_SigleCell_CheckResult &sSigCellCheckResult, 
			const S_SingleCell_Or_RegionsInfo &sSingleCellRegionsInfo,s_SysAuthority sSysAuthority,
			const BOOL bOpenDlg,const std::list<UINT> lstnErrorID)=0;

	//////二次核查参数对话框分析，需要重载上二次核查结果信息，二次核查结果由算法管理层往算法子层传输
	//virtual s_Status SetCheckparamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_SigleCell_CheckResult &sSigCellCheckResult, 
 //       const s_MiddleResult &sMiddleResult,
	//	const S_SingleCell_Or_RegionsInfo &sSingleCellRegionsInfo)=0;

	// 学习设置虚函数
	virtual s_Status  SetTrainParam(const list<s_RegionProperty>& listRegionProperty,s_SysAuthority sSysAuthority)=0;

};

#endif//_AlgVirtualBaseClass
