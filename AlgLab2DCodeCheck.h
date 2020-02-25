// AlgLab2DCodeCheck.h : main header file for the ALGNAMECHECK DLL
//

#if !defined(AFX_ALGNAMECHECK_H__7DB5D3A0_1CF2_466A_86A3_524CF5BE22F3__INCLUDED_)
#define AFX_ALGNAMECHECK_H__7DB5D3A0_1CF2_466A_86A3_524CF5BE22F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "AlgVirtualBaseClass.h"
#include "FileOperate.h"
/////////////////////////////////////////////////////////////////////////////
// CAlgLab2DCodeCheckApp
// See AlgLab2DCodeCheck.cpp for the implementation of this class
//
#define DLL_EXT _declspec(dllexport) 

class CAlgLab2DCodeCheckApp : public CWinApp
{
public:
	CAlgLab2DCodeCheckApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgLab2DCodeCheckApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAlgLab2DCodeCheckApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


// 检测算法库接口类
class DLL_EXT CDHAlgLab2DCodeCheck : public CAlgVirtualCheck
{
public:
	CDHAlgLab2DCodeCheck();
	virtual ~CDHAlgLab2DCodeCheck();
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

	//检测参数更新
	s_Status UpdateParam();
	
	//保存检测模板,学习后调用
	s_Status SaveCheckModel(const char *strTaskName,s_SysAuthority sSysAuthority);
	
	//返回检测模板大图
	s_Status GetCheckModelImg(const UINT iTaskImgType, 
		s_TaskImageInfo &sTaskImgInfo,
		s_SysAuthority sSysAuthority);

public:
	CAlgVirtualCheck *m_pAlgLab2DCodeCheck;
	s_Status m_sStatus;

};


//////////////////////////////////////////////////////////////////////////
// 算法属性设置接口类
class DLL_EXT CDHAlgLab2DCodeCheckPROPDLG : public CAlgVirtualPROPDLG
{
public:
	// 检测类指针，用于更新对话框上设置的数据
	CDHAlgLab2DCodeCheck *m_pChecker;

	CDHAlgLab2DCodeCheckPROPDLG();
	virtual ~CDHAlgLab2DCodeCheckPROPDLG();

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

protected:
	CAlgVirtualPROPDLG *m_pPROPDLG;
	s_Status m_sStatus;

};

//////////////////////////////////////////////////////////////////////////
// 检测参数设置接口类
class DLL_EXT CDHAlgLab2DCodeCheckPARADLG : public CAlgVirtualPARADLG
{
public:
	// 检测类指针，用于更新对话框上设置的数据
	CDHAlgLab2DCodeCheck *m_pChecker;

	CDHAlgLab2DCodeCheckPARADLG();
	virtual ~CDHAlgLab2DCodeCheckPARADLG();

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

protected:
	CAlgVirtualPARADLG *m_pPARADLG;
	s_Status m_sStatus;
};

extern "C" __declspec(dllexport) CDHAlgLab2DCodeCheck* CreateDHAlgNameCheck();
extern "C" __declspec(dllexport) CDHAlgLab2DCodeCheckPROPDLG* CreateDHAlgNameCheckPROPDLG();
extern "C" __declspec(dllexport) CDHAlgLab2DCodeCheckPARADLG* CreateDHAlgNameCheckPARADLG();
extern "C" __declspec(dllexport) BOOL FreeAlgCheckDll(CDHAlgLab2DCodeCheck *pCheck,
													  CDHAlgLab2DCodeCheckPROPDLG *pPropDlg,
													  CDHAlgLab2DCodeCheckPARADLG *pParaDlg);


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGNAMECHECK_H__7DB5D3A0_1CF2_466A_86A3_524CF5BE22F3__INCLUDED_)
