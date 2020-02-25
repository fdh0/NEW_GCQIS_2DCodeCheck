// Check.h: interface for the CCheck class.
//
//////////////////////////////////////////////////////////////////////

/************************************************************************
/* Copyright (c) 2008, �������ͼ���Ӿ����޹�˾�Ӿ��㷨��    
/* All rights reserved.             
/*                  
/* �ļ����ƣ�  Check.h
/* ժҪ�� �㷨����ʵ�֡�
/*
/* ��ǰ�汾�� 1.0
/* �����ߣ� �˺���
/* ������ڣ� 2011��5��10��
/* ˵���� ��������ӿ���ľ���ʵ���༰��Ա����
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
// �����ڲ��㷨֧�ֵ��㷨����,ע�⣬���㷨�ɸ��������޸�
#define Inner_Type1 1 //֧�ֵĵ�һ���㷨
#define Inner_Type2 2 //֧�ֵĵڶ����㷨
#define Inner_Type3 3 //֧�ֵĵ������㷨
#define ALG_SUM 0     //֧���ڲ��㷨����
//...
static  char strInnerTypeName[20][50]=          // �ڲ��㷨�������б�
{
	"�ڲ��㷨��һ",
	"�ڲ��㷨����",
	"�ڲ��㷨����"
};
static BOOL bInnerTypeStudyFlag[20]=          // ��Ӧ�㷨�������Ƿ�֧��ѧϰ
{
	TRUE,
	TRUE,
	FALSE
};
static BOOL bInnerTypeUPDlgFlag[20]=           // ��Ӧ�㷨�������Ƿ�ɵ�ѧϰ�Ի���
{
	FALSE,
	TRUE,
	FALSE
};
//�ڲ��㷨֧���������ý���
typedef struct _INNER_CHECK_INFO
{
	S_SingleCellProperty sSingleCellPro;//С����Ϣ
	std::list<CInnerCheck> lstInnerCheck;//С���ڼ������
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
}s_InnerCheckInfo;// �ڲ������� [11/14/2011 Denghl]

typedef struct _ALG_SIGLECELLOrREGIONSINFO
{
	int nCellIndex;		//С������
	HObject HRegCell;							//�����ڲɼ�ͼ���е�ԭʼ����

}sAlgCellInfo;

typedef struct _SIMPLE_PARA
{	
	BOOL bSkipCheck;//ȱӡ���
	UINT nDeviation;//īɫ����
	UINT nPercent;//���߰ٷ�λ
	BOOL bZoubanCheck;//�߰���
	double dZouBanRange;//�߰湫��
	BOOL bDirtCheck;//�����
	double dSingleArea;//���
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

class CCheck : public CAlgVirtualCheck  // ���ʵ����
{
public:
	CCheck();
	virtual ~CCheck();

	s_Status m_sStatus;// ����ִ��״̬ [5/12/2011]
	CString m_strTaskPath;//����ȫ·��
	CString m_strTaskName;//������
	s_AlgInitParam m_sAlgInitParam;//ȫ���㷨·��
	CString m_strDefaultParaIni;// ȫ��Ĭ�ϲ���·��
	CString m_strAlgName;//�㷨��

	s_DH_Template m_sDHTemplate;//���յ���ģ�壨����ʱ���Ǳ仯��ģ�壩
    std::list<s_InnerCheckInfo> m_lstSingleCellInnerCheck;// ���С������ [11/14/2011 Denghl]
	int m_nRegionNum;//�������
	s_MiddleResult m_sMiddleResult;//�м���

	CSpendTime sPendTime;

	s_AlgImageInfo m_sAlgImageInfo;//�㷨�����Ҫ��ͼ����Ϣ
	s_EachAlgLearnInfo m_sEachAlgLearnInfo;//�㷨��Ҫ��ѧϰ������Ϣ
	std::list<s_EachAlgLearnInfo> m_lstEachAlgLearnInfo;//����ѧϰ��Ϣ����

	int m_nStudyRegNum;//ÿ����ѧϰ�ɹ�������

	CFileOperate m_FileOperate;//for FileOperate,like read,write file
	CString m_strLogPath;//�쳣��־·��

	s_CheckResult m_sCheckResult;
	int nStart ;
	// �����Ƿ�д�����־ [9/29/2015 TIAN]
	BOOL m_bWriteLog;
	BOOL m_bCheckTimeOut;
	int m_nAllTime;
	CString m_strPathini;//��¼��ʱ�������

//	BOOL m_bUseDll;// �Ƿ�ʹ��У���
//	HINSTANCE m_hDll;//���ܶ�̬����
	//BOOL m_bIsAgainCheck;// �Ƿ���κ˲� [3/28/2012 Denghl]
//	lpCheckBarCodeFun checkBarCodeFun;
//	lpFreeRMBProductInfoMemFun freeRMBProductInfoMemFun;
//	lpFreeRMBProductParamMemFun freeRMBProductParamMemFun;
//	lpGetRMBProductInfoFun getRMBProductInfoFun;
//	lpGetRMBProductParamFun getRMBProductParamFun;

//	int m_nProductNum;// ��֧�ֲ�Ʒ������ [4/7/2012 Denghl]
//	std::vector<CString> m_vcstrAllProType;// ���в�Ʒ����
	//  [12/2/2014 baiyy]
	std::list<sAlgCellInfo>m_lstCellReg;
	//  [10/15/2015 zl]
	//CCriticalSection *m_csWriteLog;  
	//BOOL m_bcsWriteLog;
	//CEvent m_EventThread;

	//  [11/12/2015 TIAN]
	int m_nlstStrCell;  //С������list<CString>�ĸ���
	s_Status* m_sTatusArray;
	std::list<CString> *m_lstStr;
	BOOL m_blststr;

	s_SimplePara m_sSimplePara;
	//BOOL m_bIsStopThread;
	//BOOL m_bThreadRun;
	//BOOL m_bIsTheadLog;

	//  [4/6/2017 zb]
	int     m_nLanguage;              // ����
	CString m_strMessagePath;         // ��Ӣ����־��message����·��
	char m_strMessageText[512][256];
//private:
	//static void WriteLogThreadFunc(LPVOID pParam);
	//void WriteLogProc();
public:

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
	//
	//����������
	s_Status UpdateParam();
	
	//������ģ��,ѧϰ�����
	s_Status SaveCheckModel(const char *strTaskName,s_SysAuthority sSysAuthority);
	
	//���ؼ��ģ���ͼ
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




class CSetPROP :public CAlgVirtualPROPDLG// ����ʵ����
{
public:
	CSetPROP();
	virtual ~CSetPROP();
	s_Status m_sStatus;
	CCheck *m_pChecker;
	CString m_strTaskName;//������
	s_AlgInitParam m_sAlgInitParam;//ȫ���㷨·��
	CString m_strAlgName;
	CFileOperate m_FileOperate;

public:
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


};


class CSetPARA :public CAlgVirtualPARADLG // ��������ʵ����
{
public:
	CSetPARA();
	virtual ~CSetPARA();
	s_Status m_sStatus;
	CCheck *m_pChecker;
	CString m_strTaskName;//������
	s_AlgInitParam m_sAlgInitParam;//ȫ���㷨·��
	CString m_strAlgName;//�㷨��
	
public:	
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
   	
	
};
#endif // !defined(AFX_CHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)


