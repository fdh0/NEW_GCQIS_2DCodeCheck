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
// ������ʼ��
//******************************************************************************//
//����:������ʼ��
//����:
//	sAlgInitParam    ���룬ȫ��·��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::Init(const s_AlgInitParam &sAlgInitParam,s_ALGCheckParam* psCheckParams,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//m_strLog.Format("%s\\AlgLab2DCodeCheck\\",sAlgInitParam.cAlglogPath);
	//m_FileOperate.InitFile(m_strLog);

	m_sStatus = m_pAlgLab2DCodeCheck->Init(sAlgInitParam,psCheckParams,sSysAuthority);
	
	//CString strContent="��ǰ��־";
	//((CCheck*)m_pAlgLab2DCodeCheck)->GenCurLog(strContent,1);

	return m_sStatus;
}

// �ͷſռ� 
//******************************************************************************//
//����:�ͷſռ�
//����:

//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::Free(s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->Free(sSysAuthority);	
	//m_FileOperate.FreeFile();
	return m_sStatus;
}

//�����������
//******************************************************************************//
//����:�����������
//����:
//	strTaskName      ������
//	sDHTemplate      ��������ṹ������
//����:
//	�㷨ִ��״̬�ṹ��
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

//������Ҫ���м������������еĶ�λ�����,ÿ�μ�⺯��֮ǰ����
s_Status CDHAlgLab2DCodeCheck::SetMiddleResult(const s_MiddleResult &sMiddleResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->SetMiddleResult(sMiddleResult);	
		
	return m_sStatus;
	

}

// ������⺯�����ýӿ�ʵ�� 
//******************************************************************************//
//����:���������
//����:
//	sAlgCheckInputParam      ���룬�������ṹ��
//	sAlgCheckOutputResult    ��������ؼ��������ṹ��
//����:
//	�㷨ִ��״̬�ṹ��
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
		;// д�쳣��־��������Ҫ��Ӵ���
		//CString strContent;
		//strContent.Format("%s",m_sStatus.chErrorContext);
		//((CCheck*)m_pAlgLab2DCodeCheck)->GenCurLog(strContent,3);
		//m_FileOperate.WriteAlgExceptionLog(m_strLog, "�쳣��־", "CDHAlgLab2DCodeCheck�����","�쳣");
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
	strExPath.Format("E:\\DHAlgLab2DCodeCheck�ַ�ʶ���쳣\\");
	FileOperate.InitFile(strExPath);
	FileOperate.CreateMultiLevelPath(strExPath);
	str.Format("ID=%d, ʶ���ַ�С�ڵ���5:",itlstSigCCR->vsErrorInfo[i].nRegionID);
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
//��λ�㷨���ü���麯������
//******************************************************************************//
//����:��λ�㷨���ü���麯������
//����:
//	sAlgCheckInputParam      ���룬�������ṹ��
//	sMiddleResult    ��������ض�λ�㷨�������
//����:
//	�㷨ִ��״̬�ṹ��
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
		;// д�쳣��־��������Ҫ��Ӵ���		
	}

	return m_sStatus;
}

////��λ�㷨���ö��κ˲����麯������
////******************************************************************************//
////����:��λ�㷨���ü���麯������
////����:
////	sAlgCheckInputParam      ���룬�������ṹ��
////	sMiddleResult    ��������ض�λ�㷨�������
////����:
////	�㷨ִ��״̬�ṹ��
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
//		;// д�쳣��־��������Ҫ��Ӵ���		
//	}
//
//	return m_sStatus;
//
//}
//
//// �������κ˲��⺯�����ýӿ�ʵ�� 
////******************************************************************************//
////����:���������
////����:
////	sAlgAgainCheckInputParam      ���룬�������ṹ��
////	sAlgCheckOutputResult    ��������ؼ��������ṹ��
////����:
////	�㷨ִ��״̬�ṹ��
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
//		;// д�쳣��־��������Ҫ��Ӵ���		
//	}
//
//	return m_sStatus;
//}


//����������
//******************************************************************************//
//����:����������
//����:

//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::UpdateParam()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->UpdateParam();	
	
	return m_sStatus;
}

//������ģ��,ѧϰ�����
//******************************************************************************//
//����:������ģ��,ѧϰ�����
//����:

//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::SaveCheckModel(const char *strTaskName,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->SaveCheckModel(strTaskName,sSysAuthority);	
	
	return m_sStatus;
}

//���ؼ��ģ���ͼ
//******************************************************************************//
//����:���ؼ��ģ���ͼ
//����:
//	iTaskImgType             ���룬��Ҫ���ص�ģ��ͼ�����ͣ�0Ϊ��ģ�壬1ΪСģ��
//	sTaskImgInfo             �����ģ��ͼ����Ϣ
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheck::GetCheckModelImg(const UINT iTaskImgType, s_TaskImageInfo &sTaskImgInfo,
	s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pAlgLab2DCodeCheck->GetCheckModelImg(iTaskImgType, sTaskImgInfo,sSysAuthority);		
	return m_sStatus;
}

////////////////////////////////////////////
// ��������
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

//�������ó�ʼ��
//******************************************************************************//
//����:�������ó�ʼ��
//����:
//	strTaskName      ���룬������
//	sAlgInitParam    ���룬ȫ��·��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG::Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_pPROPDLG->pChecker = pChecker;
	
	m_sStatus = m_pPROPDLG->Init(strTaskName,sAlgInitParam);

	
	
	return m_sStatus;
}

//******************************************************************************//
//����:��������������
//����:
//	sAlgImageInfo    ���룬ͼ��ṹ��   
//	sRegionProperty   ���룬��������

//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG:: SetAlgParamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionProperty,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPROPDLG->SetAlgParamDlg(sAlgImageInfo,sRegionProperty,sSysAuthority);
	
	return m_sStatus;
}

//******************************************************************************//
//����:ƴ��ͼ����������
//���ܣ���ƴ�ӵ�����������������
//����:
//	sAlgImageInfo     ���룬ͼ��ṹ��   
//	sRegionPropertyA  ���룬��������
//	sRegionPropertyB  ���룬��������

//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG::SetAlgJuncParamdlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionPropertyA,
													const s_RegionProperty& sRegionPropertyB,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPROPDLG->SetAlgJuncParamdlg(sAlgImageInfo,sRegionPropertyA,sRegionPropertyB,sSysAuthority);
	
	return m_sStatus;
}

//��������
//******************************************************************************//
//����:��������
//����:
//	sAlgImageInfo        ���룬ͼ��ṹ��   
//	sDstRegionProperty   �����Ŀ����������
//  sSrcRegionProperty   ���룬Դ��������
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG::AlgCopyRegion(const s_AlgImageInfo& sAlgImageInfo,
					   s_RegionProperty &sDstRegionProperty,const s_RegionProperty &sSrcRegionProperty,int nDirection,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pPROPDLG->AlgCopyRegion(sAlgImageInfo,sDstRegionProperty,sSrcRegionProperty,nDirection,sSysAuthority);	
	return m_sStatus;
}

//�޸������С
//******************************************************************************//
//����:�޸������С
//���ܣ��޸������С�󣬵��ú��������������û�����ģ��ȣ�
//����:
//	sAlgImageInfo     ���룬ͼ��ṹ��   
//	sRegionProperty   ���룬��������
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG::AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty &sSrcRegionProperty,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPROPDLG->AlgModifyRegion(sAlgImageInfo,sSrcRegionProperty,sSysAuthority);
	
	return m_sStatus;

}

//ƴ�������޸������С����
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

//ɾ������
//******************************************************************************//
//����:ɾ������
//����:
//	lstnRegionID     ���룬����ID����   

//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG::AlgDeleteRegion(const list<s_RegionProperty> &lstRegionProperty,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPROPDLG->AlgDeleteRegion(lstRegionProperty,sSysAuthority);
	
	return m_sStatus;
}

//�õ��㷨�ڲ�����
//******************************************************************************//
//����:�õ��㷨�ڲ�����
//����:
//	lstInnerAlgType     ������㷨�ڲ�֧���㷨������Ϣ 

//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPROPDLG::GetAlgorithmInfo(list<s_InnerAlgType>  &lstInnerAlgType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pPROPDLG->GetAlgorithmInfo(lstInnerAlgType);
	
	return m_sStatus;
}
//////////////////////////////////////////////////////////////////////////
// ����������ʵ��
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

//�����Ի����ʼ��
//******************************************************************************//
//����:�����������ó�ʼ��
//����:
//	strTaskName      ���룬����·��
//	sAlgInitParam    ���룬ȫ��·��
//����:
//	�㷨�������ͽṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPARADLG::Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_pPARADLG->pChecker = pChecker;

	m_sStatus = m_pPARADLG->Init(strTaskName,sAlgInitParam);
	
	return m_sStatus;   
}

//******************************************************************************//
//����:�����Ի�����ýӿ�ʵ��
//����:
//	sAlgImageInfo     ���룬ͼ��ṹ��
//	pSetOCRChecker    ���룬���������Խṹ��
//	sListSetOCRChecker    ���룬��������������
//����:
//	�㷨�������ͽṹ��
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
////����:�����Ի�����ýӿ�ʵ��
////����:
////	sAlgImageInfo     ���룬ͼ��ṹ��
////	pSetOCRChecker    ���룬���������Խṹ��
////	sListSetOCRChecker    ���룬��������������
////����:
////	�㷨�������ͽṹ��
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
//����:ѧϰ���ýӿں���ʵ��
//����:
//  strTaskPath           ���룬����·��
//	listRegionProperty    ���룬��������
//����:
//	�㷨�������ͽṹ��
//******************************************************************************//
s_Status CDHAlgLab2DCodeCheckPARADLG::SetTrainParam(const list<s_RegionProperty>& listRegionProperty,s_SysAuthority sSysAuthority)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPARADLG->SetTrainParam(listRegionProperty,sSysAuthority);
	
	return m_sStatus;
}

//��̬���ü����
extern "C" __declspec(dllexport) CDHAlgLab2DCodeCheck* CreateDHAlgNameCheck()
{
	CDHAlgLab2DCodeCheck *A;
	A  = new CDHAlgLab2DCodeCheck;
	return A;
}

//��̬����������
extern "C" __declspec(dllexport) CDHAlgLab2DCodeCheckPROPDLG* CreateDHAlgNameCheckPROPDLG()
{
	CDHAlgLab2DCodeCheckPROPDLG *A;
	A  = new CDHAlgLab2DCodeCheckPROPDLG;
	return A;
}

//��̬���ò���������
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
