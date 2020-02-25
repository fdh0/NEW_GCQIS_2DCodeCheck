//AlgVirtualBaseClass.h   �㷨�����������
//////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Copyright (c) 2011, �������ͼ���Ӿ����޹�˾�Ӿ��㷨��    
/* All rights reserved.													
/*																		
/* �ļ����ƣ� AlgVirtualBaseClass.h												
/* ժҪ���㷨�����������
/*
/* 
/* ���ߵ˺���
/* �汾Ϊ 1.4
/* �汾�޸�1.1��
/*            ���Ӷ���ģ�����С����⺯��
/*            ���Ӷ���ģ������ӿ�
/*            ������Ͳ������ʼ�������ӿڼ����Ƿ��Ƕ���ģ��ı�־
/*            ��⺯��������Ϣ�����Ƿ���Ҫ�в�Ʒ��־����λ�㷨�ã�
/*            ���Ӷ���ģ�����һ��ģ�庯��
�汾�޸�1.2��
/*           �������κ˲��ⶨλ����ӿں���
�汾�޸�1.3��
/*           �̰���ά��ӿ�
�汾�޸�1.4��
/*           ��ǩ�㷨�ӿ�
�汾�޸�1.5��
/*           �����ļ�⺯������HObject������Ϊ���������halcon��ConcatObj
             ��������ͼ�����ݣ�
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
    std::vector<s_LocGroupResult> vsLocGroupResult;  //���еĶ�λ��Ϣ
	void lst_vec_clear()
	{
		vsLocGroupResult.clear();
	}
}s_MiddleResult;


//����������
class CAlgVirtualCheck  
{
public:
	CAlgVirtualCheck()
	{
	}
	virtual ~CAlgVirtualCheck()
	{
	}

	//��ʼ���麯��
	virtual s_Status Init(const s_AlgInitParam &sAlgInitParam,s_ALGCheckParam* psCheckParams,s_SysAuthority sSysAuthority)=0;
	
	//�����������
	virtual s_Status LoadTask(const char *strTaskName, const s_DH_Template  &sDHTemplate,
		s_SysAuthority sSysAuthority,const BOOL bLoadFlag)=0;

	//��ȡ������
	virtual s_Status GetAlgParam(s_ALGCheckParam  * psCheckParams,s_SysAuthority sSysAuthority)=0;

	//���ü�����
	virtual s_Status SetAlgParam(const s_ALGCheckParam  sCheckParams,s_SysAuthority sSysAuthority)=0;

	//�ͷ��麯��
	virtual s_Status Free(s_SysAuthority sSysAuthority)=0;

	//������Ҫ���м������������еĶ�λ�����,ÿ�μ�⺯��֮ǰ����
 	virtual s_Status SetMiddleResult(const s_MiddleResult &sMiddleResult)=0;

	//����麯��
	virtual s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage,s_AlgCheckOutputParam *sAlgCheckOutputResult)=0;

	//��λ�㷨���ü���麯������
	virtual s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage,s_MiddleResult *sMiddleResult,BOOL&bIsProduct)=0;

	////���κ˲��⺯������С����⺯��,
	////���κ˲�ɲ�����loadtask��ÿ�μ��ʱ�㷨�Լ������Լ�loadtask
	////��������������������ʹ��ǰһ����������
	//virtual s_Status Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
	//	s_AlgCheckOutputParam *sAlgCheckOutputResult)=0;

	////���κ˲鶨λ����м���
	//virtual s_Status Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
	//	s_MiddleResult *sMiddleResult)=0;

	//�����麯��
	virtual s_Status UpdateParam()=0;

	//������ģ��
	virtual s_Status SaveCheckModel(const char *strTaskName,s_SysAuthority sSysAuthority)=0;

	//���ؼ��ģ���ͼ
	virtual s_Status GetCheckModelImg(const UINT iTaskImgType, s_TaskImageInfo &sTaskImgInfo,
		s_SysAuthority sSysAuthority)=0;	

};


//////////////////////////////////////////////////////////////////////////
// �������������
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

	//��ʼ��,bIsAgainCheck�Ƿ��Ƕ��κ˲�·��
	virtual s_Status Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam)=0;
	
	//���������㷨���ԶԻ���;
	virtual s_Status SetAlgParamDlg(const s_AlgImageInfo& sAlgImageInfo, 
		const s_RegionProperty& sRegionProperty,s_SysAuthority sSysAuthority)=0;
	
	//ƴ�������㷨���ԶԻ���;
	virtual s_Status SetAlgJuncParamdlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionPropertyA,
		const s_RegionProperty& sRegionPropertyB,s_SysAuthority sSysAuthority)=0;

	//��������
	virtual s_Status AlgCopyRegion(const s_AlgImageInfo& sAlgImageInfo,
		s_RegionProperty &sDstRegionProperty,const s_RegionProperty &sSrcRegionProperty,int nDirection,s_SysAuthority sSysAuthority)=0;

	////����ģ�����һ��ģ�����򣬴�һ��ģ���п���һ�����򵽶�Ӧ����ģ��������
	//virtual s_Status AlgUpdataRegion(const s_AlgImageInfo& sAlgImageInfo,
	//	const list<s_RegionProperty> &lstUpdataRegion)=0;
	
	//�޸������С
	virtual s_Status AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty &sNewRegionProperty,s_SysAuthority sSysAuthority)=0;

	//ƴ�������޸������С����
	virtual s_Status AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, 
		const s_RegionProperty &sNewRegionProperty,const s_RegionProperty &sOrgRegionProperty,s_SysAuthority sSysAuthority)=0;
	
	//ɾ������
	virtual s_Status AlgDeleteRegion(const list<s_RegionProperty> &lstRegionProperty,s_SysAuthority sSysAuthority)=0;

	//�õ��㷨����
	virtual s_Status GetAlgorithmInfo(list<s_InnerAlgType>  &lstInnerAlgType)=0;

};


//////////////////////////////////////////////////////////////////////////
// �������������
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


	//��ʼ��,bIsAgainCheck�Ƿ��Ƕ��κ˲�·��
	virtual s_Status Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam)=0;

	//�󱨷����Ի��򼰵���������
	virtual s_Status SetCheckparamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_SigleCell_CheckResult &sSigCellCheckResult, 
			const S_SingleCell_Or_RegionsInfo &sSingleCellRegionsInfo,s_SysAuthority sSysAuthority,
			const BOOL bOpenDlg,const std::list<UINT> lstnErrorID)=0;

	//////���κ˲�����Ի����������Ҫ�����϶��κ˲�����Ϣ�����κ˲������㷨��������㷨�Ӳ㴫��
	//virtual s_Status SetCheckparamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_SigleCell_CheckResult &sSigCellCheckResult, 
 //       const s_MiddleResult &sMiddleResult,
	//	const S_SingleCell_Or_RegionsInfo &sSingleCellRegionsInfo)=0;

	// ѧϰ�����麯��
	virtual s_Status  SetTrainParam(const list<s_RegionProperty>& listRegionProperty,s_SysAuthority sSysAuthority)=0;

};

#endif//_AlgVirtualBaseClass
