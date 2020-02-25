// AlgLab2DCodeCheck.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "Check.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CAlgLab2DCodeCheckApp

BEGIN_MESSAGE_MAP(CAlgLab2DCodeCheckApp, CWinApp)
	//{{AFX_MSG_MAP(CAlgLab2DCodeCheckApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlgLab2DCodeCheckApp construction

CAlgLab2DCodeCheckApp::CAlgLab2DCodeCheckApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAlgLab2DCodeCheckApp object

CAlgLab2DCodeCheckApp theApp;

CDHAlgLab2DCodeCheck::CDHAlgLab2DCodeCheck()
{

	m_pAlgLab2DCodeCheck=new CCheck;
}
CDHAlgLab2DCodeCheck::~CDHAlgLab2DCodeCheck()
{
	if(m_pAlgLab2DCodeCheck!=NULL)
	{
		delete m_pAlgLab2DCodeCheck;
		m_pAlgLab2DCodeCheck = NULL;
	}
}
// 检测类初始化
//******************************************************************************//
//函数:检测类初始化
//参数:
//	sAlgInitParam    输入，全局路径
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::Init(const s_AlgInitParam &sAlgInitParam,s_ALGCheckParam* psCheckParams,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//m_strLog.Format("%s\\AlgLab2DCodeCheck\\",sAlgInitParam.cAlglogPath);
	//m_FileOperate.InitFile(m_strLog);

	m_sStatus = m_pAlgLab2DCodeCheck->Init(sAlgInitParam,psCheckParams,sSysAuthority);
	
	//CString strContent="当前日志";
	//((CCheck*)m_pAlgLab2DCodeCheck)->GenCurLog(strContent,1);

	return m_sStatus;
}

// 释放空间 
//******************************************************************************//
//函数:释放空间
//参数:

//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::Free(s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->Free(sSysAuthority);	
	//m_FileOperate.FreeFile();
	return m_sStatus;
}

//加载任务调用
//******************************************************************************//
//函数:加载任务调用
//参数:
//	strTaskName      任务名
//	sDHTemplate      任务区域结构体链表
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::LoadTask(const char *strTaskName, const s_DH_Template  &sDHTemplate
	,s_SysAuthority sSysAuthority,const BOOL bLoadFlag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->LoadTask(strTaskName,sDHTemplate,sSysAuthority,bLoadFlag);
	
	
	return m_sStatus;
}

s_Status CDHAlgLab2DCodeCheck::GetAlgParam(s_ALGCheckParam  * psCheckParams,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->GetAlgParam(psCheckParams,sSysAuthority);
	return m_sStatus;

}
s_Status CDHAlgLab2DCodeCheck::SetAlgParam(const s_ALGCheckParam  sCheckParams,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->SetAlgParam(sCheckParams,sSysAuthority);
	return m_sStatus;

}

//设置需要的中间检测结果（如所有的定位结果）,每次检测函数之前调用
s_Status CDHAlgLab2DCodeCheck::SetMiddleResult(const s_MiddleResult &sMiddleResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->SetMiddleResult(sMiddleResult);	
		
	return m_sStatus;
	

}

// 检测类检测函数调用接口实现 
//******************************************************************************//
//函数:检测主函数
//参数:
//	sAlgCheckInputParam      输入，检测输入结构体
//	sAlgCheckOutputResult    输出，返回检测结果输出结构体
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage,s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	try
	{
		m_sStatus = m_pAlgLab2DCodeCheck->Check(sAlgCheckInputParam,hSrcImage,sAlgCheckOutputResult);
	}
	catch (...)
	{
		;// 写异常日志，这里需要添加代码
		//CString strContent;
		//strContent.Format("%s",m_sStatus.chErrorContext);
		//((CCheck*)m_pAlgLab2DCodeCheck)->GenCurLog(strContent,3);
		//m_FileOperate.WriteAlgExceptionLog(m_strLog, "异常日志", "CDHAlgLab2DCodeCheck检测类","异常");
		m_sStatus.nErrorType = 1;
		sprintf_s(m_sStatus.chErrorContext,"2DCodeExcept");
	}
	/*std::list<s_SigleCell_CheckResult>::iterator  itlstSigCCR;
	itlstSigCCR=sAlgCheckOutputResult->sCheckResult.lstSigCCR.begin();
	for(;itlstSigCCR!=sAlgCheckOutputResult->sCheckResult.lstSigCCR.end();itlstSigCCR++)
	{
	for(int i=0;i<itlstSigCCR->vsErrorInfo.size();i++)
	{
	if(itlstSigCCR->vsErrorInfo[i].vcCheckResultCode.size()>0&&
	itlstSigCCR->vsErrorInfo[i].vcCheckResultCode.size()<=5)
	{
	CString str;
	CFileOperate FileOperate;
	CString strExPath;
	strExPath.Format("E:\\DHAlgLab2DCodeCheck字符识别异常\\");
	FileOperate.InitFile(strExPath);
	FileOperate.CreateMultiLevelPath(strExPath);
	str.Format("ID=%d, 识别字符小于等于5:",itlstSigCCR->vsErrorInfo[i].nRegionID);
	char cCode;
	for(int j=0;j<itlstSigCCR->vsErrorInfo[i].vcCheckResultCode.size();j++)
	{
	cCode=itlstSigCCR->vsErrorInfo[i].vcCheckResultCode[j];
	str+=cCode;
	}
	FileOperate.WriteAlgExceptionLog(strExPath,"","",str);
	FileOperate.FreeFile();
	}
	}
	}*/
	

	return m_sStatus;
}
//定位算法调用检测虚函数重载
//******************************************************************************//
//函数:定位算法调用检测虚函数重载
//参数:
//	sAlgCheckInputParam      输入，检测输入结构体
//	sMiddleResult    输出，返回定位算法结果链表
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage,s_MiddleResult *sMiddleResult,BOOL&bIsProduct)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		m_sStatus = m_pAlgLab2DCodeCheck->Check(sAlgCheckInputParam,hSrcImage,sMiddleResult,bIsProduct);
	}
	catch (...)
	{
		;// 写异常日志，这里需要添加代码		
	}

	return m_sStatus;
}

////定位算法调用二次核查检测虚函数重载
////******************************************************************************//
////函数:定位算法调用检测虚函数重载
////参数:
////	sAlgCheckInputParam      输入，检测输入结构体
////	sMiddleResult    输出，返回定位算法结果链表
////返回:
////	算法执行状态结构体
////******************************************************************************//
//s_Status CDHAlgLab2DCodeCheck::Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
//								s_MiddleResult *sMiddleResult)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	try
//	{
//		m_sStatus = m_pAlgLab2DCodeCheck->Check(sAlgAgainCheckInputParam,sMiddleResult);
//	}
//	catch (...)
//	{
//		;// 写异常日志，这里需要添加代码		
//	}
//
//	return m_sStatus;
//
//}
//
//// 检测类二次核查检测函数调用接口实现 
////******************************************************************************//
////函数:检测主函数
////参数:
////	sAlgAgainCheckInputParam      输入，检测输入结构体
////	sAlgCheckOutputResult    输出，返回检测结果输出结构体
////返回:
////	算法执行状态结构体
////******************************************************************************//
//s_Status CDHAlgLab2DCodeCheck::Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
//								s_AlgCheckOutputParam *sAlgCheckOutputResult)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	try
//	{
//		m_sStatus = m_pAlgLab2DCodeCheck->Check(sAlgAgainCheckInputParam,sAlgCheckOutputResult);
//	}
//	catch (...)
//	{
//		;// 写异常日志，这里需要添加代码		
//	}
//
//	return m_sStatus;
//}


//检测参数更新
//******************************************************************************//
//函数:检测参数更新
//参数:

//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::UpdateParam()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->UpdateParam();	
	
	return m_sStatus;
}

//保存检测模板,学习后调用
//******************************************************************************//
//函数:保存检测模板,学习后调用
//参数:

//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::SaveCheckModel(const char *strTaskName,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->SaveCheckModel(strTaskName,sSysAuthority);	
	
	return m_sStatus;
}

//返回检测模板大图
//******************************************************************************//
//函数:返回检测模板大图
//参数:
//	iTaskImgType             输入，需要返回的模板图像类型，0为大模板，1为小模板
//	sTaskImgInfo             输出，模板图像信息
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::GetCheckModelImg(const UINT iTaskImgType, s_TaskImageInfo &sTaskImgInfo,
	s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->GetCheckModelImg(iTaskImgType, sTaskImgInfo,sSysAuthority);		
	return m_sStatus;
}

////////////////////////////////////////////
// 属性设置
////////////////////////////////////////////
CDHAlgLab2DCodeCheckPROPDLG::CDHAlgLab2DCodeCheckPROPDLG()
{
	m_pPROPDLG=new CSetPROP;
	memset(&m_sStatus,0,sizeof(s_Status));
}
CDHAlgLab2DCodeCheckPROPDLG::~CDHAlgLab2DCodeCheckPROPDLG()
{
	if(m_pPROPDLG!=NULL)
	{
		delete m_pPROPDLG;
		m_pPROPDLG = NULL;
	}
}

//属性设置初始化
//******************************************************************************//
//函数:属性设置初始化
//参数:
//	strTaskName      输入，任务名
//	sAlgInitParam    输入，全局路径
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG::Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_pPROPDLG->pChecker = pChecker;
	
	m_sStatus = m_pPROPDLG->Init(strTaskName,sAlgInitParam);

	
	
	return m_sStatus;
}

//******************************************************************************//
//函数:属性设置主函数
//参数:
//	sAlgImageInfo    输入，图像结构体   
//	sRegionProperty   输入，区域属性

//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG:: SetAlgParamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionProperty,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPROPDLG->SetAlgParamDlg(sAlgImageInfo,sRegionProperty,sSysAuthority);
	
	return m_sStatus;
}

//******************************************************************************//
//函数:拼接图像属性设置
//功能：将拼接的两个区域设置属性
//参数:
//	sAlgImageInfo     输入，图像结构体   
//	sRegionPropertyA  输入，区域属性
//	sRegionPropertyB  输入，区域属性

//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG::SetAlgJuncParamdlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionPropertyA,
													const s_RegionProperty& sRegionPropertyB,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPROPDLG->SetAlgJuncParamdlg(sAlgImageInfo,sRegionPropertyA,sRegionPropertyB,sSysAuthority);
	
	return m_sStatus;
}

//复制区域
//******************************************************************************//
//函数:复制区域
//参数:
//	sAlgImageInfo        输入，图像结构体   
//	sDstRegionProperty   输出，目的区域属性
//  sSrcRegionProperty   输入，源区域属性
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG::AlgCopyRegion(const s_AlgImageInfo& sAlgImageInfo,
					   s_RegionProperty &sDstRegionProperty,const s_RegionProperty &sSrcRegionProperty,int nDirection,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pPROPDLG->AlgCopyRegion(sAlgImageInfo,sDstRegionProperty,sSrcRegionProperty,nDirection,sSysAuthority);	
	return m_sStatus;
}

//修改区域大小
//******************************************************************************//
//函数:修改区域大小
//功能：修改区域大小后，调用函数（可重新设置或生成模板等）
//参数:
//	sAlgImageInfo     输入，图像结构体   
//	sRegionProperty   输入，区域属性
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG::AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty &sSrcRegionProperty,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPROPDLG->AlgModifyRegion(sAlgImageInfo,sSrcRegionProperty,sSysAuthority);
	
	return m_sStatus;

}

//拼接区域，修改区域大小重载
s_Status CDHAlgLab2DCodeCheckPROPDLG::AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, 
								 const s_RegionProperty &sNewRegionProperty,const s_RegionProperty &sOrgRegionProperty,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_sStatus = m_pPROPDLG->AlgModifyRegion(sAlgImageInfo,sNewRegionProperty,sOrgRegionProperty,sSysAuthority);

	return m_sStatus;

}

//s_Status CDHAlgLab2DCodeCheckPROPDLG::AlgUpdataRegion(const s_AlgImageInfo& sAlgImageInfo,
//												 const list<s_RegionProperty> &lstUpdataRegion)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	m_sStatus = m_pPROPDLG->AlgUpdataRegion(sAlgImageInfo,lstUpdataRegion);
//
//	return m_sStatus;
//}

//删除区域
//******************************************************************************//
//函数:删除区域
//参数:
//	lstnRegionID     输入，区域ID链表   

//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG::AlgDeleteRegion(const list<s_RegionProperty> &lstRegionProperty,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPROPDLG->AlgDeleteRegion(lstRegionProperty,sSysAuthority);
	
	return m_sStatus;
}

//得到算法内部类型
//******************************************************************************//
//函数:得到算法内部类型
//参数:
//	lstInnerAlgType     输出，算法内部支持算法类型信息 

//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG::GetAlgorithmInfo(list<s_InnerAlgType>  &lstInnerAlgType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pPROPDLG->GetAlgorithmInfo(lstInnerAlgType);
	
	return m_sStatus;
}
//////////////////////////////////////////////////////////////////////////
// 参数分析类实现
CDHAlgLab2DCodeCheckPARADLG::CDHAlgLab2DCodeCheckPARADLG()
{
	m_pPARADLG = new CSetPARA;
}
CDHAlgLab2DCodeCheckPARADLG::~CDHAlgLab2DCodeCheckPARADLG()
{
   	if(m_pPARADLG!=NULL)
	{
		delete m_pPARADLG;	
		m_pPARADLG = NULL;
	}
}

//参数对话框初始化
//******************************************************************************//
//函数:参数分析设置初始化
//参数:
//	strTaskName      输入，任务路径
//	sAlgInitParam    输入，全局路径
//返回:
//	算法错误类型结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPARADLG::Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_pPARADLG->pChecker = pChecker;

	m_sStatus = m_pPARADLG->Init(strTaskName,sAlgInitParam);
	
	return m_sStatus;   
}

//******************************************************************************//
//函数:参数对话框调用接口实现
//参数:
//	sAlgImageInfo     输入，图像结构体
//	pSetOCRChecker    输入，检测对象及属性结构体
//	sListSetOCRChecker    输入，检测对象属性链表
//返回:
//	算法错误类型结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPARADLG::SetCheckparamDlg(const s_AlgImageInfo& sAlgImageInfo, 
	const s_SigleCell_CheckResult &sSigCellCheckResult, 
	const S_SingleCell_Or_RegionsInfo &sSingleCellRegionsInfo,
	s_SysAuthority sSysAuthority,
	const BOOL bOpenDlg,const std::list<UINT> lstnErrorID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPARADLG->SetCheckparamDlg(sAlgImageInfo,
		sSigCellCheckResult,
		sSingleCellRegionsInfo,
		sSysAuthority,
		bOpenDlg,
		lstnErrorID);
	
	return m_sStatus;
}
////******************************************************************************//
////函数:参数对话框调用接口实现
////参数:
////	sAlgImageInfo     输入，图像结构体
////	pSetOCRChecker    输入，检测对象及属性结构体
////	sListSetOCRChecker    输入，检测对象属性链表
////返回:
////	算法错误类型结构体
////******************************************************************************//
//s_Status CDHAlgLab2DCodeCheckPARADLG::SetCheckparamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_SigleCell_CheckResult &sSigCellCheckResult, 
//												  const s_MiddleResult &sMiddleResult,
//												  const S_SingleCell_Or_RegionsInfo &sSingleCellRegionsInfo)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//
//	m_sStatus = m_pPARADLG->SetCheckparamDlg(sAlgImageInfo, sSigCellCheckResult,sMiddleResult,sSingleCellRegionsInfo);
//
//	return m_sStatus;
//}

//******************************************************************************//
//函数:学习设置接口函数实现
//参数:
//  strTaskPath           输入，任务路径
//	listRegionProperty    输入，区域链表
//返回:
//	算法错误类型结构体
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPARADLG::SetTrainParam(const list<s_RegionProperty>& listRegionProperty,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPARADLG->SetTrainParam(listRegionProperty,sSysAuthority);
	
	return m_sStatus;
}

//动态调用检测类
extern "C" __declspec(dllexport) CDHAlgLab2DCodeCheck* CreateDHAlgNameCheck()
{
	CDHAlgLab2DCodeCheck *A;
	A  = new CDHAlgLab2DCodeCheck;
	return A;
}

//动态调用属性类
extern "C" __declspec(dllexport) CDHAlgLab2DCodeCheckPROPDLG* CreateDHAlgNameCheckPROPDLG()
{
	CDHAlgLab2DCodeCheckPROPDLG *A;
	A  = new CDHAlgLab2DCodeCheckPROPDLG;
	return A;
}

//动态调用参数设置类
extern "C" __declspec(dllexport) CDHAlgLab2DCodeCheckPARADLG* CreateDHAlgNameCheckPARADLG()
{
	CDHAlgLab2DCodeCheckPARADLG *A;
	A  = new CDHAlgLab2DCodeCheckPARADLG;
	return A;
}
extern "C" __declspec(dllexport) BOOL FreeAlgCheckDll(CDHAlgLab2DCodeCheck *pCheck,
													  CDHAlgLab2DCodeCheckPROPDLG *pPropDlg,
													  CDHAlgLab2DCodeCheckPARADLG *pParaDlg)
{
	try
	{
		if (pCheck)
		{
			delete pCheck;
			pCheck = NULL;
		}
		if (pPropDlg)
		{
			delete pPropDlg;
			pPropDlg = NULL;
		}
		if (pParaDlg)
		{
			delete pParaDlg;
			pParaDlg = NULL;
		}
	}
	catch (...)
	{
		return TRUE;
	}
	return FALSE;
}
