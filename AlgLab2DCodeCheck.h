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


// ����㷨��ӿ���
class DLL_EXT CDHAlgLab2DCodeCheck : public CAlgVirtualCheck
{
public:
	CDHAlgLab2DCodeCheck();
	virtual ~CDHAlgLab2DCodeCheck();
	//��ʼ��
	s_Status Init(const s_AlgInitParam &sAlgInitParam,s_ALGCheckParam* psCheckParams,s_SysAuthority sSysAuthority);

	//�����������
	s_Status LoadTask(const char *strTaskName, const s_DH_Template  &sDHTemplate,s_SysAuthority sSysAuthority,const BOOL bLoadFlag);

	//��ȡ�㷨����
	s_Status GetAlgParam(s_ALGCheckParam  * psCheckParams,s_SysAuthority sSysAuthority);

	//�����㷨����
	s_Status SetAlgParam(const s_ALGCheckParam  sCheckParams,s_SysAuthority sSysAuthority);

	// �ͷ�
	s_Status Free(s_SysAuthority sSysAuthority);

	//������Ҫ���м������������еĶ�λ�����,ÿ�μ�⺯��֮ǰ����
	s_Status SetMiddleResult(const s_MiddleResult &sMiddleResult);

    // ��⺯��
	s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage,s_AlgCheckOutputParam *sAlgCheckOutputResult);

	// ��λ�㷨���ü���麯������
	s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage,s_MiddleResult *sMiddleResult,BOOL&bIsProduct);

	////���κ˲鶨λ����м���
	//s_Status Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
	//	s_MiddleResult *sMiddleResult);

	////���κ˲��⺯����С�������ú������Ƕ�λ�㷨ר��
	//s_Status Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
	//	s_AlgCheckOutputParam *sAlgCheckOutputResult);

	//����������
	s_Status UpdateParam();
	
	//������ģ��,ѧϰ�����
	s_Status SaveCheckModel(const char *strTaskName,s_SysAuthority sSysAuthority);
	
	//���ؼ��ģ���ͼ
	s_Status GetCheckModelImg(const UINT iTaskImgType, 
		s_TaskImageInfo &sTaskImgInfo,
		s_SysAuthority sSysAuthority);

public:
	CAlgVirtualCheck *m_pAlgLab2DCodeCheck;
	s_Status m_sStatus;

};


//////////////////////////////////////////////////////////////////////////
// �㷨�������ýӿ���
class DLL_EXT CDHAlgLab2DCodeCheckPROPDLG : public CAlgVirtualPROPDLG
{
public:
	// �����ָ�룬���ڸ��¶Ի��������õ�����
	CDHAlgLab2DCodeCheck *m_pChecker;

	CDHAlgLab2DCodeCheckPROPDLG();
	virtual ~CDHAlgLab2DCodeCheckPROPDLG();

	//��ʼ�� [5/10/2011]
	s_Status Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam);

	//���������㷨���ԶԻ���;
	s_Status SetAlgParamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionProperty,s_SysAuthority sSysAuthority);
	
	//ƴ�������㷨���ԶԻ���;
	s_Status SetAlgJuncParamdlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionPropertyA,
		const s_RegionProperty& sRegionPropertyB,s_SysAuthority sSysAuthority);
	
	//��������
	s_Status AlgCopyRegion(const s_AlgImageInfo& sAlgImageInfo,
		s_RegionProperty &sDstRegionProperty,const s_RegionProperty &sSrcRegionProperty,int nDirection,s_SysAuthority sSysAuthority);
	
	//�޸������С
	s_Status AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty &sSrcRegionProperty,s_SysAuthority sSysAuthority);

	//ƴ�������޸������С����
	s_Status AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, 
		const s_RegionProperty &sNewRegionProperty,const s_RegionProperty &sOrgRegionProperty,s_SysAuthority sSysAuthority);
	
	//// ����ģ��ͬ��һ��ģ����º��� [3/26/2012 Denghl]
	//s_Status AlgUpdataRegion(const s_AlgImageInfo& sAlgImageInfo,
	//	const list<s_RegionProperty> &lstUpdataRegion);

	//ɾ������
	s_Status AlgDeleteRegion(const list<s_RegionProperty> &lstRegionProperty,s_SysAuthority sSysAuthority);

	//�õ��㷨����
	s_Status GetAlgorithmInfo(list<s_InnerAlgType>  &lstInnerAlgType);

protected:
	CAlgVirtualPROPDLG *m_pPROPDLG;
	s_Status m_sStatus;

};

//////////////////////////////////////////////////////////////////////////
// ���������ýӿ���
class DLL_EXT CDHAlgLab2DCodeCheckPARADLG : public CAlgVirtualPARADLG
{
public:
	// �����ָ�룬���ڸ��¶Ի��������õ�����
	CDHAlgLab2DCodeCheck *m_pChecker;

	CDHAlgLab2DCodeCheckPARADLG();
	virtual ~CDHAlgLab2DCodeCheckPARADLG();

	//��ʼ��
	s_Status Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam);

	//�󱨷����Ի��򼰵���������
	s_Status SetCheckparamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_SigleCell_CheckResult &sSigCellCheckResult, 
		const S_SingleCell_Or_RegionsInfo &sSingleCellRegionsInfo,s_SysAuthority sSysAuthority,
		const BOOL bOpenDlg,const std::list<UINT> lstnErrorID);

	////���κ˲�����Ի����������Ҫ�����϶��κ˲�����Ϣ�����κ˲������㷨��������㷨�Ӳ㴫��
	//s_Status SetCheckparamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_SigleCell_CheckResult &sSigCellCheckResult, 
	//	const s_MiddleResult &sMiddleResult,
	//	const S_SingleCell_Or_RegionsInfo &sSingleCellRegionsInfo);
	
	// ѧϰ�����麯��
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
