//DHTemplate.h   ģ����Ϣ�ṹ
//////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Copyright (c) 2011, �������ͼ���Ӿ����޹�˾�Ӿ�ϵͳ��     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� DHTemplate.h												
/* ժҪ��ģ����Ϣ�ṹ��
/*
/* ��ͨ�û��������Ʊ���������ṹ��Ϣ
/* �������ķ�
/* ��ʼ�汾Ϊ 1.0
/* �޸İ汾V4.3
/* �޸ļ�¼��
/*       �ļ�������ǿ�Ƶ��ֽڶ����޸�Ϊ�Զ��ֽڶ��루�����ÿ��ܶԵ���������̬��Ƚ϶�ĳ�����Ӱ�죩// dhl
/*       ����ṹ����������������ԭ������Ϊ���� // dhl
/*       ��λ�������������� // dhl
/*       �����������������㷨���ͽ��з����� // dhl
/*       �����������ԱȽϽ���ṹ��ͱȽϺ��� // 2.1 dhl
/*       �����Ƿ�����������ʶ//2.2dhl
/*       �����������������Ƿ��в�Ʒ״̬// 2.3 dhl
/*       Ϊ���κ˲��������������йر���//2.4 dhl
/*       �����㷨���������Ӽ��㣬�����¹��������� // 2.5
/*       ȥ������ṹ���ڴ��㷨������������������� // 2.6
         /////////////////////////////V3.0�޸ļ�¼[7/27/2015 dhl]
/*       �����Զ���ģ��Ҫ�Ľṹ�� 
/*       �޸Ķ�λ����ṹ��float��Ϊdouble����
/*       ȥ������ṹ���б���ͼ�ṹ��Ϣ
/*       ȥ�����κ˲��й����ݽṹ
/*       ����Ȩ�޿��ƽṹ�弰�ӿ�
/*       ����������ʾѡ��

/*       ��ʼ������������������ݷֱ���//3.1 dhl
/*       ������ṹ���������������Խ����������ǰ��ʹ�ã�������Ҫ�㷨����//3.2 dhl 20151216
/*       ����Ԥ���ֶγ�������//3.2 dhl
/*       �������������Ƿ�У����//3.2 dhl
/*       �Զ���ģ���Ӽ����ID//3.3 dhl
/*       ��λ�ṹ�帳ֵbug�޸�//3.4 dhl
        //////////////////////////////V4.0
/*       �н�3.4�汾���޸��Զ���ģ��Ҫ�Ľṹ�壨�������ּ����ݣ�
/*       ����ṹ������halcon�����region�ṹ�壬���ڽ�ģ��ʾ���洢���㷨У����

/*       ����ϵͳ�����㷨����������ݽṹ//4.1 dhl
/*       �޸Ĳ��ֽṹ��ȽϺ���    //4.2
/*       ���Ӵ���ṹ���б����Ƿ�����ѧϰ�ı�־��Ĭ��Ϊ0��������ѧϰ
/************************************************************************/
#if !defined(_DHTemplate)
#define _DHTemplate

#define  DH_MAX_NAMESIZE		64
#define  DH_MAX_TEXTSIZE		255
#define  DH_MAX_PATH			512

#include <list>
#include <vector>
#include <algorithm>
using namespace std;
#include "HalconCpp.h"
using namespace HalconCpp;

#pragma pack(1)
//���������Խṹ����������ʶ����
enum eRegionFlag
{
	RT_PreProcessRegion = 0,   //Ԥ�����������綨λMark
	RT_RowCloumnCheckRegion,   //�������и���������
	RT_SingleCheckRegion       //�����������
};

//���������־
enum eOperateFlag
{
	OF_DoNothing = 0,       //Ĭ��״̬  ����ⲻѧϰ
	OF_Check, 	    		//���
	OF_Train,				//ѧϰ
	OF_CheckAndTrain		//�߼���ѧϰ
};

//[2009-4-22 by jzj]: add
//ѧϰ��־
enum eTrainFlag
{
	TF_NoTrain = 0,		//Ĭ��״̬
	TF_DisTrain,		//����ѧϰ
	TF_AddTrain			//���ѧϰ
};

//��λ�����ṹ��
typedef  struct _LOC_GROUP_RESULT
{
	UINT  nLocGroupID;      //��λ���
	double fLineShiftLoc;    //����ƫ��λ��
	double fColShiftLoc;     //����ƫ��λ��
	double fRoundAngle;      //��ת�Ƕȣ�������

	double fAffineMAT[9];    //�������
	UINT  nLocCount;        //�ö�λ������Ķ�λ����

	BOOL  bHaveError;       //True����ʾ��λ���д�

	UINT  nCheckLayerID;        //��λ���������������  [3/6/2012 Denghl]
	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_LOC_GROUP_RESULT()
	{
		nLocGroupID = 0;
		fLineShiftLoc = 0.0;
		fColShiftLoc = 0.0;
		fRoundAngle = 0.0;
		nLocCount = 0;
		bHaveError = FALSE;

		memset(&fAffineMAT, 0, 9*sizeof(double));
		nCheckLayerID = 0;

	}
	_LOC_GROUP_RESULT(const _LOC_GROUP_RESULT& sLGR)
	{
		*this = sLGR;
	}
	_LOC_GROUP_RESULT& operator=(const _LOC_GROUP_RESULT& sLocGroReuslt)
	{
		nLocGroupID = sLocGroReuslt.nLocGroupID;
		fLineShiftLoc = sLocGroReuslt.fLineShiftLoc;
		fColShiftLoc = sLocGroReuslt.fColShiftLoc;
		fRoundAngle = sLocGroReuslt.fRoundAngle;
		nLocCount = sLocGroReuslt.nLocCount;
		bHaveError = sLocGroReuslt.bHaveError;

		memcpy(&fAffineMAT, sLocGroReuslt.fAffineMAT, 
			9*sizeof(double));
		nCheckLayerID = sLocGroReuslt.nCheckLayerID;

		return (*this);

	}
	bool operator<(const _LOC_GROUP_RESULT& sLocGroReuslt)const
	{
		return(nLocGroupID < sLocGroReuslt.nLocGroupID);
	}
	void lst_vec_clear()
	{
	}
	~_LOC_GROUP_RESULT()
	{
		nLocGroupID = 0;
		fLineShiftLoc = 0.0;
		fColShiftLoc = 0.0;
		fRoundAngle = 0.0;
		nLocCount = 0;
		bHaveError = FALSE;
		nCheckLayerID = 0;

		memset(&fAffineMAT, 0, 9*sizeof(double));
	}

}s_LocGroupResult;
typedef struct _SYS_AUTHORITY
{
	int nSysAuthority;//Ȩ�޼���
	char cAuthor[DH_MAX_NAMESIZE];//Ȩ�޲���������
	_SYS_AUTHORITY()
	{
		nSysAuthority = 0;
		memset(&cAuthor,0,DH_MAX_NAMESIZE*sizeof(char));
	}
	~_SYS_AUTHORITY()
	{

	}
	_SYS_AUTHORITY(const _SYS_AUTHORITY& sAII)
	{
		*this = sAII;
	}
	_SYS_AUTHORITY& operator=(const _SYS_AUTHORITY& sAIInfo)
	{
		nSysAuthority = sAIInfo.nSysAuthority;
		memcpy(&cAuthor,sAIInfo.cAuthor,DH_MAX_NAMESIZE*sizeof(char));
		return (*this);
	}
}s_SysAuthority;

// ����ȽϷ��ؽ���ṹ�� [3/12/2012 Denghl]
typedef   struct  _COMP_REGION_RESULT
{
	BOOL bCompResult[2];
	_COMP_REGION_RESULT()
	{
		bCompResult[0] = FALSE;
		bCompResult[1] = FALSE;
	}

}s_CompRegionResult;

//�������Խṹ��
typedef   struct  _REGION_PROPERTY
{
	UINT    nRegionID;                   //����Ψһ��ʶ��
	UINT    nGraphDepth;                 //��������ͼ�����  ����ͼ�ṹ

	//std::list<UINT> lstParentsRegID;     //iRegionID ˫�׽������      ����ͼ�ṹ
	//std::list<UINT> lstChildrenRegID;    //iRegionID �ӽ������        ����ͼ�ṹ

	eRegionFlag RegionFlag;              //�����ʶ����Ԥ�������򡢺������и��������򡢶���������� �㷨�ⲿ��
	eOperateFlag OperateFlag;			 //���������־		
	eTrainFlag TrainFlag;				 //����ѧϰ��־	

	BOOL    bIsValid;                    //�Ƿ���Ч  0�������򲻼�� 1�����	
	std::list<CPoint> lstRegionOrgJunc;  //������ƴ��ͼ���е�ԭʼ����(��ģ��ʱ��)
	std::list<CPoint> lstRegionOrg;      //�����ڲɼ�ͼ���е�ԭʼ����
	HObject hRegionOrgJunc;
	HObject hRegionOrg;
	
	UINT    nCellID;					 //����Ψһ��ʶ��
	UINT    nRegionType;                 //�������ͣ�������1�������1��������			
	UINT    nCellType;                   //����������Ԫ���ͣ���бꡢ��ꡢ����С����
	char    cRegionName[DH_MAX_NAMESIZE];//��������
	UINT    nComputerID;                 //ǰ�˼�������
	UINT    nCameraID;                   //ǰ��������
	UINT    nAlgorithmType;              //�������ʹ���㷨����  //����������Ϊ���鷽ʽ��һ��Ϊ���0������Ϊ���1  /
	UINT    nInnerAlgType;				 //�������ʹ���㷨����(С�㷨���ͣ�
	UINT    nLocateGroupID;              //����������λ���
	BOOL    bIsModifyModel;              //�Ƿ�ʹ�õ����޸ĺ��ģ��
	UINT    nFlagRegionShape;            //������״��ʾ 0������ 1�������

	BOOL    bIsPrimaryCode;							//�Ƿ�Ϊ����
	BOOL    bIsExistExpectCode;						//�Ƿ����Ԥ֪��
	std::vector<char> vcExpectCode;			        // �����Ԥ֪����
	char    cExpectCodeFileName[DH_MAX_NAMESIZE];   //����Ԥ֪���ļ�����

	UINT    nColumnIndex;							//����������Ԫȫ���к�	
	UINT    nRowIndex;								//����������Ԫȫ���к�
	UINT    nRegionImageBits;						//ͼ��λ��  
	UINT    nProductColumnIndex;					//ͬһ�������͵��к�
	UINT    nProductRowIndex;						//ͬһ�������͵��к�

	// ������������ [3/6/2012 Denghl]
	BOOL    bJudgeProduct;                          //��λ�����Ƿ������в�Ʒ������λ����ʹ�ã�
	UINT    nInheritLocGroup;                       //��λ�������̳еĶ�λ��ţ�����λ����ʹ�ã�
	UINT    nCheckLayerID;                          //��λ���������㣨��й����飩
    BOOL    bIsSetCell;                             //�Ƿ��˹�ѡ���������С��
	//BOOL    bIsOnlineReg;                           //�Ƿ������߼��ģ�壨һ��ģ�壩��������
	BOOL    bIsAssoPrimaryCode;                     //�Ƿ���������������
	// \\ [3/6/2012 Denghl]

	// ���κ˲�ģ�����������йر��� [3/31/2012 Denghl]
	//char    cDefineMarkAgain[DH_MAX_NAMESIZE];		//�ͻ����ڱ�ʶ�����Զ����ַ�
	//UINT    nRegionTypeAgain;                       //�������ͣ�������1�������1��������			
	//char    cRegionNameAgain[DH_MAX_NAMESIZE];      //��������

	char    cDefineMark[DH_MAX_NAMESIZE];			//�ͻ����ڱ�ʶ�����Զ����ַ�
	char    cResvered[DH_MAX_NAMESIZE];				//����
	UINT    nResvered[4];							//����
	BOOL    bIsVerifyCode;                          //�Ƿ���У����


	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/13/2011 ChenLiang]
	_REGION_PROPERTY()
	{
		nRegionID = 0;
		nGraphDepth = 0;
		//lstParentsRegID.clear();
		//lstChildrenRegID.clear();

		RegionFlag = RT_PreProcessRegion;
		OperateFlag = OF_DoNothing;
		TrainFlag =	TF_NoTrain;

		bIsValid = TRUE;// ��FALSEΪTRUE [3/27/2012 Denghl]
		lstRegionOrgJunc.clear();
		lstRegionOrg.clear();
		GenEmptyRegion(&hRegionOrg);
		GenEmptyRegion(&hRegionOrgJunc);

		nCellID = 0;
		nRegionType = 0;
		//nRegionTypeAgain = 0;
		nCellType = 0;
		memset(&cRegionName, 0, DH_MAX_NAMESIZE*sizeof(char));

		//memset(&cRegionNameAgain, 0, DH_MAX_NAMESIZE*sizeof(char));

		nComputerID = 0;
		nCameraID = 0;
		nAlgorithmType= 0;
		nInnerAlgType = 0;
		nLocateGroupID = 0;
		bIsModifyModel = FALSE;
		nFlagRegionShape = 0;
		bIsPrimaryCode = FALSE;
		bIsExistExpectCode = FALSE;
		vcExpectCode.clear();
		memset(&cExpectCodeFileName, 0, DH_MAX_NAMESIZE*sizeof(char));

		nColumnIndex = 0;
		nRowIndex = 0;
		nRegionImageBits = 0;
		nProductColumnIndex = 0;
		nProductRowIndex = 0;

		bJudgeProduct = FALSE;
		nInheritLocGroup = 0;
		nCheckLayerID = 0;
		bIsSetCell = FALSE;
//		bIsOnlineReg = TRUE;
		bIsAssoPrimaryCode = FALSE;

		memset(&cDefineMark, 0, DH_MAX_NAMESIZE*sizeof(char));

		//memset(&cDefineMarkAgain, 0, DH_MAX_NAMESIZE*sizeof(char));
		memset(&cResvered, 0, DH_MAX_NAMESIZE*sizeof(char));
		memset(&nResvered, 0, 4*sizeof(UINT));
		bIsVerifyCode = FALSE;
	}
	~_REGION_PROPERTY()
	{
		//lstParentsRegID.clear();
		//lstChildrenRegID.clear();
		lstRegionOrgJunc.clear();
		lstRegionOrg.clear();
		vcExpectCode.clear();
		GenEmptyRegion(&hRegionOrg);
		GenEmptyRegion(&hRegionOrgJunc);
	}
	_REGION_PROPERTY(const _REGION_PROPERTY& sRgnP)
	{
		*this = sRgnP;
	}
	_REGION_PROPERTY& operator=(const _REGION_PROPERTY& sRgnPro)
	{
		nRegionID = sRgnPro.nRegionID;
		nGraphDepth = sRgnPro.nGraphDepth;

		//lstParentsRegID = sRgnPro.lstParentsRegID;
		//lstChildrenRegID = sRgnPro.lstChildrenRegID;

		RegionFlag = sRgnPro.RegionFlag;
		OperateFlag = sRgnPro.OperateFlag;
		TrainFlag = sRgnPro.TrainFlag;

		bIsValid = sRgnPro.bIsValid;
		lstRegionOrgJunc = sRgnPro.lstRegionOrgJunc;
		//[fdh]��ʱ�޸�
		//lstRegionOrg = sRgnPro.lstRegionOrg;
		lstRegionOrg = sRgnPro.lstRegionOrgJunc;
		hRegionOrg = sRgnPro.hRegionOrg;
		hRegionOrgJunc = sRgnPro.hRegionOrgJunc;

		nCellID = sRgnPro.nCellID;
		nRegionType = sRgnPro.nRegionType;
		//nRegionTypeAgain = sRgnPro.nRegionTypeAgain;
		nCellType = sRgnPro.nCellType;
		memcpy(&cRegionName, sRgnPro.cRegionName, 
			DH_MAX_NAMESIZE*sizeof(char));

		//memcpy(&cRegionNameAgain, sRgnPro.cRegionNameAgain, 
		//	DH_MAX_NAMESIZE*sizeof(char));

		nComputerID = sRgnPro.nComputerID;
		nCameraID = sRgnPro.nCameraID;
		nAlgorithmType = sRgnPro.nAlgorithmType;
		nInnerAlgType = sRgnPro.nInnerAlgType;
		nLocateGroupID = sRgnPro.nLocateGroupID;
	
		bIsModifyModel = sRgnPro.bIsModifyModel;
		nFlagRegionShape = sRgnPro.nFlagRegionShape;
		bIsPrimaryCode = sRgnPro.bIsPrimaryCode;
		bIsExistExpectCode = sRgnPro.bIsExistExpectCode;
		vcExpectCode = sRgnPro.vcExpectCode;

		memcpy(&cExpectCodeFileName, sRgnPro.cExpectCodeFileName, 
			DH_MAX_NAMESIZE*sizeof(char));
		nColumnIndex = sRgnPro.nColumnIndex;
		nRowIndex = sRgnPro.nRowIndex;
		nRegionImageBits = sRgnPro.nRegionImageBits;
		nProductColumnIndex = sRgnPro.nProductColumnIndex;
		nProductRowIndex = sRgnPro.nProductRowIndex;	

		bJudgeProduct = sRgnPro.bJudgeProduct;
		nInheritLocGroup = sRgnPro.nInheritLocGroup;
		nCheckLayerID = sRgnPro.nCheckLayerID;
		bIsSetCell = sRgnPro.bIsSetCell;
//		bIsOnlineReg = sRgnPro.bIsOnlineReg;
		bIsAssoPrimaryCode = sRgnPro.bIsAssoPrimaryCode;

		memcpy(&cDefineMark, sRgnPro.cDefineMark, 
			DH_MAX_NAMESIZE*sizeof(char));

	//	memcpy(&cDefineMarkAgain, sRgnPro.cDefineMarkAgain, 
	//		DH_MAX_NAMESIZE*sizeof(char));

		memcpy(&cResvered, sRgnPro.cResvered, 
			DH_MAX_NAMESIZE*sizeof(char));

		memcpy(&nResvered, sRgnPro.nResvered, 
			4*sizeof(UINT));
		bIsVerifyCode = sRgnPro.bIsVerifyCode;

		return *this;
	}
	BOOL operator==(const _REGION_PROPERTY& sRgnPro)
	{
		BOOL bFlag = FALSE;
		if (nRegionID != sRgnPro.nRegionID
			||nGraphDepth != sRgnPro.nGraphDepth
			//||lstParentsRegID != sRgnPro.lstParentsRegID
			//||lstChildrenRegID != sRgnPro.lstChildrenRegID
			||RegionFlag != sRgnPro.RegionFlag
			||OperateFlag != sRgnPro.OperateFlag
			||TrainFlag != sRgnPro.TrainFlag
			||bIsValid != sRgnPro.bIsValid
			||lstRegionOrgJunc != sRgnPro.lstRegionOrgJunc
			||lstRegionOrg != sRgnPro.lstRegionOrg
			||nCellID != sRgnPro.nCellID
			||nRegionType != sRgnPro.nRegionType
		//	||nRegionTypeAgain != sRgnPro.nRegionTypeAgain
			||nCellType != sRgnPro.nCellType
			||nComputerID != sRgnPro.nComputerID
			||nCameraID != sRgnPro.nCameraID
			||nAlgorithmType != sRgnPro.nAlgorithmType
		    ||nInnerAlgType != sRgnPro.nInnerAlgType
		    ||nLocateGroupID != sRgnPro.nLocateGroupID
		    ||bIsModifyModel != sRgnPro.bIsModifyModel
			||nFlagRegionShape != sRgnPro.nFlagRegionShape
			||bIsPrimaryCode != sRgnPro.bIsPrimaryCode
			||bIsExistExpectCode != sRgnPro.bIsExistExpectCode
			||vcExpectCode != sRgnPro.vcExpectCode
			||nColumnIndex != sRgnPro.nColumnIndex
			||nRowIndex != sRgnPro.nRowIndex
			||nRegionImageBits != sRgnPro.nRegionImageBits
			||nProductColumnIndex != sRgnPro.nProductColumnIndex
			||nProductRowIndex != sRgnPro.nProductRowIndex
			||bJudgeProduct != sRgnPro.bJudgeProduct
			||nInheritLocGroup != sRgnPro.nInheritLocGroup
			||nCheckLayerID != sRgnPro.nCheckLayerID
			||bIsSetCell != sRgnPro.bIsSetCell
			//||bIsOnlineReg != sRgnPro.bIsOnlineReg
			||bJudgeProduct != sRgnPro.bJudgeProduct
			||nInheritLocGroup != sRgnPro.nInheritLocGroup
			||nCheckLayerID != sRgnPro.nCheckLayerID
			||bIsSetCell != sRgnPro.bIsSetCell
			//||bIsOnlineReg != sRgnPro.bIsOnlineReg
			||nResvered[0]!=sRgnPro.nResvered[0]
			||nResvered[1]!=sRgnPro.nResvered[1]
			||nResvered[2]!=sRgnPro.nResvered[2]
			||nResvered[3]!=sRgnPro.nResvered[3]
			||bIsAssoPrimaryCode!=sRgnPro.bIsAssoPrimaryCode
				|| bIsVerifyCode!=sRgnPro.bIsVerifyCode)
		{
			bFlag = TRUE;
		}

		if (bFlag==FALSE)
		{
			CString strSrc,strDst;
			strSrc.Format("%s",cRegionName);
			strDst.Format("%s",sRgnPro.cRegionName);
			if (strSrc!=strDst)
			{
				bFlag = TRUE;
			}
			/*if (bFlag==FALSE)
			{
				strSrc.Format("%s",cRegionNameAgain);
				strDst.Format("%s",sRgnPro.cRegionNameAgain);
				if (strSrc!=strDst)
				{
					bFlag = TRUE;
				}
			}*/
			if (bFlag==FALSE)
			{
				strSrc.Format("%s",cExpectCodeFileName);
				strDst.Format("%s",sRgnPro.cExpectCodeFileName);
				if (strSrc!=strDst)
				{
					bFlag = TRUE;
				}
			}
			if (bFlag==FALSE)
			{
				strSrc.Format("%s",cDefineMark);
				strDst.Format("%s",sRgnPro.cDefineMark);
				if (strSrc!=strDst)
				{
					bFlag = TRUE;
				}
			}
			/*if (bFlag==FALSE)
			{
				strSrc.Format("%s",cDefineMarkAgain);
				strDst.Format("%s",sRgnPro.cDefineMarkAgain);
				if (strSrc!=strDst)
				{
					bFlag = TRUE;
				}
			}*/
			if (bFlag==FALSE)
			{
				strSrc.Format("%s",cResvered);
				strDst.Format("%s",sRgnPro.cResvered);
				if (strSrc!=strDst)
				{
					bFlag = TRUE;
				}
			}
		}

		return !bFlag;// ���ڷ���TRUE�����ȷ���FALSE [4/26/2012 Denghl]

	}
	void lst_vec_clear()
	{
		//���list �� vector
		/*if (0 < lstParentsRegID.size())
		{
		lstParentsRegID.clear();
		}

		if (0 < lstChildrenRegID.size())
		{
		lstChildrenRegID.clear();
		}*/

		if (0 < lstRegionOrgJunc.size())
		{
			lstRegionOrgJunc.clear();
		}

		if (0 < lstRegionOrg.size())
		{
			lstRegionOrg.clear();
		}

		if (0 < vcExpectCode.size())
		{
			vcExpectCode.clear();
		}
		GenEmptyRegion(&hRegionOrg);
		GenEmptyRegion(&hRegionOrgJunc);
	}
	BOOL operator <(const _REGION_PROPERTY& other) const
	{
	//	s_CompRegionResult s_pbCompRegionResult;
		if (nAlgorithmType < other.nAlgorithmType)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
				
		//return s_pbCompRegionResult;
	}
	BOOL operator >(const _REGION_PROPERTY& other) const
	{
		s_CompRegionResult s_pbCompRegionResult;
		if (nAlgorithmType > other.nAlgorithmType)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
				
		//return s_pbCompRegionResult;
	}
}s_RegionProperty;

typedef struct _SINGLECELL_PROPERTY
{
	UINT    nCellID;								//����Ψһ��ʶ��
	UINT    nCellTypeSN;							//С�����ͱ��
	UINT    nCellColumnIndex;						//С�������к�
	UINT    nCellRowIndex;							//С�������к�
	UINT    nCameraID;								//���������� 
	UINT    nFlagRegionShape;						//������״��ʾ 0������ 1�������


	char    cResvered[32];							//����
	UINT    nResvered[2];							//����

	std::list<CPoint> lstCell;						//�����ڲɼ�ͼ���е�ԭʼ����

	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/13/2011 ChenLiang]
	_SINGLECELL_PROPERTY()
	{
		nCellID = 0;
		nCellTypeSN = 0;
		nCellColumnIndex = 0;
		nCellRowIndex = 0;
		nCameraID = 0;
		nFlagRegionShape = 0;

		memset(&cResvered, 0, 32*sizeof(char));
		memset(&nResvered, 0, 2*sizeof(UINT));

		lstCell.clear();
	}
	~_SINGLECELL_PROPERTY()
	{
		lstCell.clear();
	}
	_SINGLECELL_PROPERTY(const _SINGLECELL_PROPERTY& sSP)
	{
		*this = sSP;
	}
	_SINGLECELL_PROPERTY& operator=(const _SINGLECELL_PROPERTY& sSCPro)
	{
		nCellID = sSCPro.nCellID;
		nCellTypeSN = sSCPro.nCellTypeSN;
		nCellColumnIndex = sSCPro.nCellColumnIndex;
		nCellRowIndex = sSCPro.nCellRowIndex;
		nCameraID = sSCPro.nCameraID;
		nFlagRegionShape = sSCPro.nFlagRegionShape;

		memcpy(&cResvered, sSCPro.cResvered, 
			32*sizeof(char));

		memcpy(&nResvered, sSCPro.nResvered, 
			2*sizeof(UINT));

		lstCell = sSCPro.lstCell;

		return *this;
	}
	void lst_vec_clear()
	{
		if (0 < lstCell.size())
		{
			lstCell.clear();
		}
	}

}S_SingleCellProperty;

//typedef struct _CHECK_LAYER
//{
//	std::list<s_RegionProperty> lstSRegPro;			//����������� 
//	_CHECK_LAYER()
//	{
//		lstSRegPro.clear();
//	}
//	~_CHECK_LAYER()
//	{
//		lstSRegPro.clear();
//	}
//	_CHECK_LAYER(const _CHECK_LAYER& sAlg)
//	{
//		*this = sAlg;
//	}
//	_CHECK_LAYER& operator=(const _CHECK_LAYER& sAlg)
//	{
//		lstSRegPro = sAlg.lstSRegPro;
//		return *this;
//	}
//	void lst_vec_clear()
//	{
//		if (0 < lstSRegPro.size())
//		{
//			std::list<s_RegionProperty>::iterator itRPro, itRProEnd;
//			itRPro = lstSRegPro.begin();
//			itRProEnd = lstSRegPro.end();
//			for (; itRPro != itRProEnd; ++itRPro)
//			{
//				itRPro->lst_vec_clear();
//			}
//			lstSRegPro.clear();
//		}
//	}
//}s_CheckLayer;

typedef struct _ALGREGIONS
{
	UINT nAlgID;                                    //���㷨ID
	std::list<s_RegionProperty> lstSRegPro;			//����������� 

	UINT nCheckLayerNum;// ������� [4/1/2012 Denghl]

	_ALGREGIONS()
	{
		lstSRegPro.clear();
		nCheckLayerNum = 0;
	}
	~_ALGREGIONS()
	{
		lstSRegPro.clear();
	}
	_ALGREGIONS(const _ALGREGIONS& sAlg)
	{
		*this = sAlg;
	}
	_ALGREGIONS& operator=(const _ALGREGIONS& sAlg)
	{
		lstSRegPro = sAlg.lstSRegPro;
		nAlgID = sAlg.nAlgID;
		nCheckLayerNum = sAlg.nCheckLayerNum;
		return *this;
	}
	void lst_vec_clear()
	{
		if (0 < lstSRegPro.size())
		{
			std::list<s_RegionProperty>::iterator it, itEnd;
			it = lstSRegPro.begin();
			itEnd = lstSRegPro.end();
			for (; it != itEnd; ++it)
			{
				it->lst_vec_clear();
			}
			lstSRegPro.clear();
		}
	}
}s_AlgRegions;

//����������Ϣ�ṹ��
typedef struct _SINGLECELLOrREGIONSINFO
{
	//std::list<s_RegionProperty> lstSRegPro;			//�����������  //С���¹Ҵ��㷨����
	std::list<s_AlgRegions> lstAlgRegions;
	S_SingleCellProperty sSingleCellPro;            //С����Ϣ

	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/13/2011 ChenLiang]	
	_SINGLECELLOrREGIONSINFO()
	{
		lstAlgRegions.clear();
	}
	~_SINGLECELLOrREGIONSINFO()
	{
		if (lstAlgRegions.size()>0)
		{
			std::list<s_AlgRegions>::iterator itsAlgRegions;		
			for (itsAlgRegions=lstAlgRegions.begin();
				itsAlgRegions!=lstAlgRegions.end();
				itsAlgRegions++)
			{
				itsAlgRegions->lst_vec_clear();

			}
		}		
		lstAlgRegions.clear();
	}
	_SINGLECELLOrREGIONSINFO(const _SINGLECELLOrREGIONSINFO& sSC)
	{
		*this = sSC;
	}
	_SINGLECELLOrREGIONSINFO& operator=(const _SINGLECELLOrREGIONSINFO& sSCRInfo)
	{
		lstAlgRegions = sSCRInfo.lstAlgRegions;
		sSingleCellPro = sSCRInfo.sSingleCellPro;

		return *this;
	}
	void lst_vec_clear()
	{
		if (0 < lstAlgRegions.size())
		{
			std::list<s_AlgRegions>::iterator it, itEnd;
			it = lstAlgRegions.begin();
			itEnd = lstAlgRegions.end();
			for (; it != itEnd; ++it)
			{
				it->lst_vec_clear();
			}
			lstAlgRegions.clear();
		}
		sSingleCellPro.lst_vec_clear();
	}

}S_SingleCell_Or_RegionsInfo;

//ģ��ṹ��
typedef struct _DH_TEMPLATE
{
	std::list<S_SingleCell_Or_RegionsInfo> lstSCellRegion;    //����������
	UINT    nComputerID;                 //ǰ�˼�������
	UINT    nCameraID;                   //ǰ��������
//	BOOL    bIsSecVerify;                //�Ƿ��Ƕ��κ˲�ģ��

	char    cTemplateName[DH_MAX_NAMESIZE];					  //ģ������
	char    cTemplateTime[DH_MAX_NAMESIZE];					  //ģ��ʱ��
	char    cResvered[DH_MAX_NAMESIZE];						  //����
	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/13/2011 ChenLiang]	
	_DH_TEMPLATE()
	{
		lstSCellRegion.clear();
		nComputerID = 0;
		nCameraID = 0;
	//	bIsSecVerify = FALSE;

		memset(&cTemplateName, 0, DH_MAX_NAMESIZE*sizeof(char));
		memset(&cTemplateTime, 0, DH_MAX_NAMESIZE*sizeof(char));
		memset(&cResvered, 0, DH_MAX_NAMESIZE*sizeof(char));

	}
	~_DH_TEMPLATE()
	{
		lstSCellRegion.clear();
	}
	_DH_TEMPLATE(const _DH_TEMPLATE& sDHT)
	{
		*this = sDHT;
	}
	_DH_TEMPLATE& operator=(const _DH_TEMPLATE& sDHTemp)
	{
		lstSCellRegion = sDHTemp.lstSCellRegion;
		nComputerID = sDHTemp.nComputerID;
		nCameraID = sDHTemp.nCameraID;
	//	bIsSecVerify = sDHTemp.bIsSecVerify;


		memcpy(&cTemplateName, sDHTemp.cTemplateName, 
			DH_MAX_NAMESIZE*sizeof(char));
		memcpy(&cTemplateTime, sDHTemp.cTemplateTime, 
			DH_MAX_NAMESIZE*sizeof(char));
		memcpy(&cResvered, sDHTemp.cResvered, 
			DH_MAX_NAMESIZE*sizeof(char));

		return *this;
	}
	void lst_vec_clear()
	{
		if (0 < lstSCellRegion.size())
		{
			std::list<S_SingleCell_Or_RegionsInfo>::iterator itCell, itCellEnd;
			itCell = lstSCellRegion.begin();
			itCellEnd = lstSCellRegion.end();
			for (; itCell != itCellEnd; ++itCell)
			{
				itCell->lst_vec_clear();
			}
			lstSCellRegion.clear();
		}
	}

}s_DH_Template;

//������Ϣ
typedef struct _ERRORINFO
{
	UINT   nRegionID;									//����Ψһ��ʶ��
	UINT   nLocateGroupID;								//����������λ���
	UINT   nArea;										//���ֵ    
	BYTE   bytErrorType;								//����ͼ�������ͱ�
	BYTE   bytErrorGrade;								//����ͼ ���󼶱�
	char   cErrorDescription[DH_MAX_TEXTSIZE];			//��������
	RECT   RectErrorReg;								//����������ʵʱ�ɼ�ͼ����λ�ã�
	RECT   RectErrorModel;								//����������ģ��ͼ����λ�ã�
	UINT   nValueGlimmer;								//ӫ��ֵ 
	std::vector<char> vcCheckResultCode;				//ʶ�����

	char    cResvered[DH_MAX_TEXTSIZE];					//����
	UINT    nResvered[40];								//����
	char    cRedisName[DH_MAX_TEXTSIZE];                //ǰ��ʹ�ã��㷨������
	int    nRow;                                        //�������С�������
	int   nCol;                                         //�������С�������
	BOOL  bIsPrimary;                                   //�Ƿ�����
	BOOL  bIsVerifyCode;                                //�Ƿ���У����
	UINT  nAllowStudyFlag;                              //�Ƿ�������ѧϰ


	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_ERRORINFO()
	{
		nRegionID = 0;
		nLocateGroupID = 0;
		nArea = 0;
		bytErrorType = 0;
		bytErrorGrade = 0;
		nValueGlimmer = 0;

		SetRect(&RectErrorReg, 0, 0, 0, 0);
		SetRect(&RectErrorModel, 0, 0, 0, 0);

		memset(&cErrorDescription, 0, DH_MAX_TEXTSIZE*sizeof(char));
		memset(&cResvered, 0, DH_MAX_TEXTSIZE*sizeof(char));
		memset(&nResvered, 0, 40*sizeof(UINT));

		vcCheckResultCode.clear();
		memset(&cRedisName,0,DH_MAX_TEXTSIZE*sizeof(char));
		nRow = 0;
		nCol = 0;
		bIsPrimary = FALSE;
		bIsVerifyCode = FALSE;
		nAllowStudyFlag = 0;
	}
	~_ERRORINFO()
	{
		nRegionID = 0;
		nLocateGroupID = 0;
		nArea = 0;
		bytErrorType = 0;
		bytErrorGrade = 0;
		nValueGlimmer = 0;

		SetRect(&RectErrorReg, 0, 0, 0, 0);
		SetRect(&RectErrorModel, 0, 0, 0, 0);

		memset(&cErrorDescription, 0, DH_MAX_TEXTSIZE*sizeof(char));
		memset(&cResvered, 0, DH_MAX_TEXTSIZE*sizeof(char));
		memset(&nResvered, 0, 40*sizeof(UINT));

		vcCheckResultCode.clear();
		memset(&cRedisName, 0, DH_MAX_TEXTSIZE*sizeof(char));
		nAllowStudyFlag = 0;
	}
	_ERRORINFO(const _ERRORINFO& sEI)
	{
		*this = sEI;
	}
	_ERRORINFO& operator=(const _ERRORINFO& sEInfo)
	{
		nRegionID = sEInfo.nRegionID;
		nLocateGroupID = sEInfo.nLocateGroupID;
		nArea = sEInfo.nArea;
		bytErrorType = sEInfo.bytErrorType;
		bytErrorGrade = sEInfo.bytErrorGrade;
		nValueGlimmer = sEInfo.nValueGlimmer;

		RectErrorReg = sEInfo.RectErrorReg;
		RectErrorModel = sEInfo.RectErrorModel;

		memcpy(&cErrorDescription, sEInfo.cErrorDescription, 
			DH_MAX_NAMESIZE*sizeof(char));
		memcpy(&cResvered, sEInfo.cResvered, 
			DH_MAX_TEXTSIZE*sizeof(char));
		memcpy(&nResvered, sEInfo.nResvered, 
			40*sizeof(UINT));

		vcCheckResultCode = sEInfo.vcCheckResultCode;
		memcpy(&cRedisName, sEInfo.cRedisName, 
			DH_MAX_NAMESIZE*sizeof(char));
		nRow = sEInfo.nRow;
		nCol = sEInfo.nCol;
		bIsPrimary = sEInfo.bIsPrimary;
		bIsVerifyCode = sEInfo.bIsVerifyCode;
		nAllowStudyFlag = sEInfo.nAllowStudyFlag;

		return (*this);
	}
	//  [6/15/2011 ChenLiang]
	bool operator<(const _ERRORINFO& sEInfo)const
	{   
		BOOL bFlag = FALSE;
		if (bytErrorGrade < sEInfo.bytErrorGrade)
		{
			bFlag = TRUE;
		}
		else if ((bytErrorGrade == sEInfo.bytErrorGrade)&&(nArea < sEInfo.nArea))
		{
			bFlag = TRUE;
		}
		return bFlag; 		
	}
	bool operator>(const _ERRORINFO& sEInfo)const
	{   
		BOOL bFlag = FALSE;
		if (bytErrorGrade > sEInfo.bytErrorGrade)
		{
			bFlag = TRUE;
		}
		else if ((bytErrorGrade == sEInfo.bytErrorGrade)&&(nArea > sEInfo.nArea))
		{
			bFlag = TRUE;
		}
		return bFlag; 
	}
	void lst_vec_clear()
	{
		if (0 < vcCheckResultCode.size())
		{
			vcCheckResultCode.clear();
		}
	}

} s_sErrorInfo;

//С���ڴ����������������һ��������������д�������
typedef struct _SIGLECELL_CHECKRESULTE
{
	UINT   nErrorRegionCount;									//ÿС���������������Ч�������������������ļ���ȡ��
	std::vector<s_sErrorInfo> vsErrorInfo;						//������Ϣ����

	RECT    rCircumRect;										//������С��Ӿ��� ��˵����iImageType=1 ֱ�ӷ���ģ������iImageType=2���ض�λ����ת����
	UINT    nIndexCell;											//С�������
	std::vector<s_LocGroupResult> vsLocGroupResult;             //�����С�����������򣩴����������ͼ�����еĶ�λ��Ϣ
	UINT    nImageType;											//ͼ������ 0����ͼ��1����λ������ͼ��2����λ��������Ӿ���ͼ��3��Сȱ��ͼ ljc [9/15/2010]
	BOOL   bFlagStudy;											//0:����ѧϰ 1����ѧϰ
	UINT    nAlgorithmType;										//С�������д���Ĵ��㷨����
	int     nRow;                                               //С���к�
	int     nCol;                                               //С���к�
	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_SIGLECELL_CHECKRESULTE()
	{
		nErrorRegionCount = 0;
		nIndexCell = 0;
		nImageType = 0;
		bFlagStudy = FALSE;
		SetRect(&rCircumRect, 0, 0, 0, 0);

		vsErrorInfo.clear();
		vsLocGroupResult.clear();

		nAlgorithmType = 0;
		nRow = 0;
		nCol = 0;
	}
	~_SIGLECELL_CHECKRESULTE()
	{
		nErrorRegionCount = 0;
		nIndexCell = 0;
		nImageType = 0;
		bFlagStudy = FALSE;
		SetRect(&rCircumRect, 0, 0, 0, 0);
		vsErrorInfo.clear();
		vsLocGroupResult.clear();
		nAlgorithmType = 0;
	}
	_SIGLECELL_CHECKRESULTE(const _SIGLECELL_CHECKRESULTE& sSCCR)
	{
		*this = sSCCR;
	}
	_SIGLECELL_CHECKRESULTE& operator=(const _SIGLECELL_CHECKRESULTE& sSCCResult)
	{
		nErrorRegionCount = sSCCResult.nErrorRegionCount;
		nIndexCell = sSCCResult.nIndexCell;
		nImageType = sSCCResult.nImageType;
		bFlagStudy = sSCCResult.bFlagStudy;

		rCircumRect = sSCCResult.rCircumRect;

		vsErrorInfo= sSCCResult.vsErrorInfo;
		vsLocGroupResult= sSCCResult.vsLocGroupResult;

		nAlgorithmType = sSCCResult.nAlgorithmType;
		nRow = sSCCResult.nRow;
		nCol = sSCCResult.nCol;
		return (*this);
	}
	void lst_vec_clear()
	{
		//TRACE("vsErrorInfo.size()=%d\n",vsErrorInfo.size());
		if (0 < vsErrorInfo.size())
		{
			std::vector<s_sErrorInfo>::iterator itsEI, itsEIEnd;
			itsEI = vsErrorInfo.begin();
			itsEIEnd = vsErrorInfo.end();
			for (; itsEI != itsEIEnd; ++itsEI)
			{
				itsEI->lst_vec_clear();
				//itsEI->vcCheckResultCode.clear();
			}
			vsErrorInfo.clear();
		}
		//TRACE("��պ�vsErrorInfo.size()=%d\n",vsErrorInfo.size());
		//TRACE("vsLocGroupResult.size()=%d\n",vsLocGroupResult.size());
		if (0 < vsLocGroupResult.size())
		{
			std::vector<s_LocGroupResult>::iterator itsLGR, itsLGREnd;
			itsLGR = vsLocGroupResult.begin();
			itsLGREnd = vsLocGroupResult.end();
			for (; itsLGR != itsLGREnd; ++itsLGR)
			{
				itsLGR->lst_vec_clear();
				//itsLGR->vcCheckResultCode.clear();
			}
			vsLocGroupResult.clear();
		}
		//TRACE("��պ�vsLocGroupResult.size()=%d\n",vsLocGroupResult.size());

		nErrorRegionCount = 0;
		nIndexCell = 0;
		nImageType = 0;
		bFlagStudy = FALSE;
		SetRect(&rCircumRect, 0, 0, 0, 0);
		nAlgorithmType = 0;
	}



}s_SigleCell_CheckResult;

//�㷨������ṹ��
typedef  struct _CHECK_RESULT
{
	std::list<s_SigleCell_CheckResult> lstSigCCR;				//ÿС��������� 
	UINT   iErrorLstSigCCR;										//������ͼ��С���������������
	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_CHECK_RESULT()
	{
		iErrorLstSigCCR = 0;

		lstSigCCR.clear();
	}
	~_CHECK_RESULT()
	{
		iErrorLstSigCCR = 0;
		lstSigCCR.clear();
	}
	_CHECK_RESULT(const _CHECK_RESULT& sCR)
	{
		*this = sCR;
	}
	_CHECK_RESULT& operator=(const _CHECK_RESULT& sCReuslt)
	{
		lstSigCCR = sCReuslt.lstSigCCR;
		iErrorLstSigCCR = sCReuslt.iErrorLstSigCCR;

		return (*this);
	}
	void lst_vec_clear()
	{
		//TRACE("lstSigCCR.size()=%d\n",lstSigCCR.size());
		if (0 < lstSigCCR.size())
		{
			std::list<s_SigleCell_CheckResult>::iterator itsSCCR, itsSCCREnd;
			itsSCCR = lstSigCCR.begin();
			itsSCCREnd = lstSigCCR.end();

			for (; itsSCCR != itsSCCREnd; ++itsSCCR)
			{
				itsSCCR->lst_vec_clear();
			}
			lstSigCCR.clear();
		}
		//TRACE("��պ�lstSigCCR.size()=%d\n",lstSigCCR.size());

		iErrorLstSigCCR = 0;
	}

}s_CheckResult;

//��ʼ���㷨ϵͳ��Ϣ
typedef struct _ALG_INIT_PARAM
{
	char cTaskPath[DH_MAX_PATH];       //�������·��
	char cAlgPath[DH_MAX_PATH];        //�㷨Ĭ�ϲ���·�����ַ�ģ�����·��
	char cAlglogPath[DH_MAX_PATH];     //�㷨��־·��
	UINT     nWidth;        //ͼ���
	UINT     nHeight;       //ͼ���
	UINT     nChannels;     //ͼ��ͨ����
	double fResolution_H;//  �������ֱ���
	double fResolution_V;//  �������ֱ���
	int nLanguage;//���� Ĭ�� 0Ϊ����
	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_ALG_INIT_PARAM()
	{
		nWidth = 0;
		nHeight = 0;
		nChannels = 0;
		nLanguage = 0;
		fResolution_H = 0.1;
		fResolution_V = 0.1;

		memset(&cTaskPath, 0, DH_MAX_PATH*sizeof(char));
		memset(&cAlgPath, 0, DH_MAX_PATH*sizeof(char));
		memset(&cAlglogPath, 0, DH_MAX_PATH*sizeof(char));
	}
	_ALG_INIT_PARAM(const _ALG_INIT_PARAM& sAIP)
	{
		*this = sAIP;
	}
	_ALG_INIT_PARAM& operator=(const _ALG_INIT_PARAM& sAIParam)
	{
		nWidth = sAIParam.nWidth;
		nHeight = sAIParam.nHeight;
		nChannels = sAIParam.nChannels;
		nLanguage = sAIParam.nLanguage;
		fResolution_H = sAIParam.fResolution_H;
		fResolution_V = sAIParam.fResolution_V;

		memcpy(&cTaskPath, sAIParam.cTaskPath, 
			DH_MAX_PATH*sizeof(char));
		memcpy(&cAlgPath, sAIParam.cAlgPath, 
			DH_MAX_PATH*sizeof(char));
		memcpy(&cAlglogPath, sAIParam.cAlglogPath, 
			DH_MAX_PATH*sizeof(char));

		return (*this);
	}

}s_AlgInitParam;

//ͼ�����Խṹ��  ��ͼ
typedef struct _ALG_IMAGE_INFO
{
	UINT  nFlag;			//0:�ڰ׻Ҷ�(�ĸ�����ָ�붼��ָͬ��ͼ���׵�ַ);1:��ɫ��ͨ��(pImageData = NULL);����:pImageData��������Ч
	BYTE *pImageData;
	BYTE *pImageR;			//R ͨ������
	BYTE *pImageG;			//G ͨ������
	BYTE *pImageB;			//B ͨ������
	SIZE sizeImage;			//����ͼ����
	UINT  nBits;			//8//24//32

	UINT   nImageType;				//ͼ������ 0����ͼ��1����λ������ͼ��2����λ��������Ӿ���ͼ��3��Сȱ��ͼ // ljc
	std::vector<CPoint>  vcPoint;	//��ǰͼ���������� 

	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_ALG_IMAGE_INFO()
	{
		nFlag = 0;
		pImageData = 0;
		pImageR = 0;
		pImageG = 0;
		pImageB= 0;
		sizeImage.cx = sizeImage.cy = 0;
		nBits = 0;
		nImageType = 0;
		vcPoint.clear();
	}
	~_ALG_IMAGE_INFO()
	{
		vcPoint.clear();
	}
	_ALG_IMAGE_INFO(const _ALG_IMAGE_INFO& sAII)
	{
		*this = sAII;
	}
	_ALG_IMAGE_INFO& operator=(const _ALG_IMAGE_INFO& sAIInfo)
	{
		nFlag = sAIInfo.nFlag;
		pImageData = sAIInfo.pImageData;
		pImageR = sAIInfo.pImageR;
		pImageG = sAIInfo.pImageG;
		pImageB= sAIInfo.pImageB;
		sizeImage = sAIInfo.sizeImage;

		nBits = sAIInfo.nBits;
		nImageType = sAIInfo.nImageType;
		vcPoint=sAIInfo.vcPoint;

		return (*this);
	}
	void lst_vec_clear()
	{
		vcPoint.clear();
	}
}s_AlgImageInfo;

//ģ��ͼ���ͼ�ṹ��
typedef struct _TASK_IMAGE_INFO
{
	UINT nFlag;		    //0:�ڰ׻Ҷ�(��pImageR������);1:��ɫ��ͨ��(��RGB����ָ�룩

	BYTE *pImageR;		//R ͨ������
	BYTE *pImageG;		//G ͨ������
	BYTE *pImageB;		//B ͨ������

	SIZE sizeImage;			//����ͼ����
	UINT  nBits;			//8//24//32

	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_TASK_IMAGE_INFO()
	{
		nFlag = 0;
		pImageR = 0;
		pImageG = 0;
		pImageB = 0;
		sizeImage.cx = sizeImage.cy =0;
	}
	_TASK_IMAGE_INFO(const _TASK_IMAGE_INFO& sTII)
	{
		*this = sTII;
	}
	_TASK_IMAGE_INFO& operator=(const _TASK_IMAGE_INFO& sTIInfo)
	{
		nFlag = sTIInfo.nFlag;

		pImageR = sTIInfo.pImageR;
		pImageG = sTIInfo.pImageG;
		pImageB = sTIInfo.pImageB;

		sizeImage = sTIInfo.sizeImage;
		nBits = sTIInfo.nBits;

		return (*this);
	}
}s_TaskImageInfo;

//�㷨�����㷨�ṹ��
typedef struct _INNER_ALG_TYPE
{
	UINT nInnerTypeID;					//�㷨����ID(���㷨��Ψһ��ʶ)
	BOOL bSupportLearn;					//�Ƿ�֧��ѧϰ    TRUE��֧�� FASLE����֧�� 
	BOOL bPopUpDlg;						//ѧϰ�Ƿ񵯳��Ի��� TRUE������ FASLE��������
	char cTypeName[DH_MAX_NAMESIZE];	//�㷨��������

	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_INNER_ALG_TYPE()
	{
		nInnerTypeID = 0;
		bSupportLearn = FALSE;
		bPopUpDlg = FALSE;

		memset(&cTypeName, 0, DH_MAX_NAMESIZE*sizeof(char));

	}
	_INNER_ALG_TYPE(const _INNER_ALG_TYPE& sIAT)
	{
		*this = sIAT;
	}
	_INNER_ALG_TYPE& operator=(const _INNER_ALG_TYPE& sIAType)
	{
		nInnerTypeID = sIAType.nInnerTypeID;
		bSupportLearn = sIAType.bSupportLearn;
		bPopUpDlg = sIAType.bPopUpDlg;

		memcpy(&cTypeName, sIAType.cTypeName, 
			DH_MAX_NAMESIZE*sizeof(char));

		return (*this);
	}
}s_InnerAlgType;

//���㷨��Ϣ�ṹ��
typedef struct _ALG_TYPE
{
	UINT nTypeID;                               //���㷨����IDΨһ��ʶ
	char cTypeName[DH_MAX_NAMESIZE];            //���㷨��������
	std::list< s_InnerAlgType > lstInnerAlgType;
	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_ALG_TYPE()
	{
		nTypeID = 0;
		lstInnerAlgType.clear();

		memset(&cTypeName, 0, DH_MAX_NAMESIZE*sizeof(char));
	}
	~_ALG_TYPE()
	{
		lstInnerAlgType.clear();
	}
	_ALG_TYPE(const _ALG_TYPE& sAT)
	{
		*this = sAT;
	}
	_ALG_TYPE& operator=(const _ALG_TYPE& sAType)
	{
		nTypeID = sAType.nTypeID;
		lstInnerAlgType = sAType.lstInnerAlgType;

		memcpy(&cTypeName, sAType.cTypeName, 
			DH_MAX_NAMESIZE*sizeof(char));

		return (*this);
	}
	void lst_vec_clear()
	{
		if (0 < lstInnerAlgType.size())
		{
			lstInnerAlgType.clear();
		}

	}
}s_AlgType;

//ÿ���㷨ѧϰ�����ṹ��
typedef struct _EACH_ALG_LEARN_INFO
{
	UINT nTypeID;         //���㷨���ͱ�ʶ
	UINT nPieceStudy;     //���㷨ѧϰ����
	BOOL bTrainCancel;    //��ֹѧϰ TRUE:��ֹ FASLE������ֹ

	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_EACH_ALG_LEARN_INFO()
	{
		nTypeID = 0;
		nPieceStudy = 0;
		bTrainCancel = TRUE;
	}
	_EACH_ALG_LEARN_INFO(const _EACH_ALG_LEARN_INFO& sEALI)
	{
		*this = sEALI;
	}
	_EACH_ALG_LEARN_INFO& operator=(const _EACH_ALG_LEARN_INFO& sEALInfo)
	{
		nTypeID = sEALInfo.nTypeID;
		nPieceStudy = sEALInfo.nPieceStudy;
		bTrainCancel = sEALInfo.bTrainCancel;

		return (*this);
	}
}s_EachAlgLearnInfo;

//�����������ṹ��
typedef struct _ALG_CHECK_INPUT_PARAM
{
	s_DH_Template   sDHTemplate;                 //�ɱ�������Ϣ����
	s_AlgImageInfo  sAlgImageInfo;				 //����ͼ��ṹ
	list<s_EachAlgLearnInfo>   lstAlgLearnInfo;  //ÿ�ִ��㷨��ѧϰ��Ϣ��
	BOOL bJudgeProduct;// �Ƿ��в�Ʒ [3/15/2012 Denghl]

	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_ALG_CHECK_INPUT_PARAM()
	{
		lstAlgLearnInfo.clear();
		bJudgeProduct = FALSE;
	}
	~_ALG_CHECK_INPUT_PARAM()
	{
		lstAlgLearnInfo.clear();
	}
	_ALG_CHECK_INPUT_PARAM(const _ALG_CHECK_INPUT_PARAM& sACIP)
	{
		*this = sACIP;
	}
	_ALG_CHECK_INPUT_PARAM& operator=(const _ALG_CHECK_INPUT_PARAM& sACIParam)
	{
		sDHTemplate = sACIParam.sDHTemplate;
		sAlgImageInfo = sACIParam.sAlgImageInfo;
		lstAlgLearnInfo = sACIParam.lstAlgLearnInfo;
		bJudgeProduct = sACIParam.bJudgeProduct;

		return (*this);
	}
	void lst_vec_clear()
	{
		sDHTemplate.lst_vec_clear();
		sAlgImageInfo.lst_vec_clear();
		if (0 < lstAlgLearnInfo.size())
		{
			lstAlgLearnInfo.clear();
		}
	}
}s_AlgCheckInputParam;

////���κ˲�����������ṹ��
//typedef struct _ALG_AGAINCHECK_INPUT_PARAM
//{
//	s_DH_Template   sDHTemplate;                 //�ɱ�������Ϣ����
//	s_AlgImageInfo  sAlgImageInfo;				 //����ͼ��ṹ��С��ͼ������ͼ��������ԭʼ��ͼ�е���λ����
//	list<s_EachAlgLearnInfo>   lstAlgLearnInfo;  //ÿ�ִ��㷨��ѧϰ��Ϣ��
//	s_SigleCell_CheckResult sSigCellCheckResult;//���߼��ļ������Ϣ
//	char strTaskName[DH_MAX_PATH];              //����·����
//
//	//////////////////////////////////////////////////////////////////////////
//	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
//	_ALG_AGAINCHECK_INPUT_PARAM()
//	{
//		lstAlgLearnInfo.clear();
//		memset(strTaskName,0,DH_MAX_PATH);
//	}
//	~_ALG_AGAINCHECK_INPUT_PARAM()
//	{
//		lstAlgLearnInfo.clear();
//
//	}
//	_ALG_AGAINCHECK_INPUT_PARAM(const _ALG_AGAINCHECK_INPUT_PARAM& sACIP)
//	{
//		*this = sACIP;
//	}
//	_ALG_AGAINCHECK_INPUT_PARAM& operator=(const _ALG_AGAINCHECK_INPUT_PARAM& sACIParam)
//	{
//		sDHTemplate = sACIParam.sDHTemplate;
//		sAlgImageInfo = sACIParam.sAlgImageInfo;
//		sSigCellCheckResult = sACIParam.sSigCellCheckResult;
//		lstAlgLearnInfo = sACIParam.lstAlgLearnInfo;
//		memcpy(strTaskName,sACIParam.strTaskName,DH_MAX_PATH);
//
//		return (*this);
//	}
//	void lst_vec_clear()
//	{
//		sDHTemplate.lst_vec_clear();
//		sAlgImageInfo.lst_vec_clear();
//		sSigCellCheckResult.lst_vec_clear();
//		if (0 < lstAlgLearnInfo.size())
//		{
//			lstAlgLearnInfo.clear();
//		}
//		memset(strTaskName,0,DH_MAX_PATH);
//	}
//}s_AlgAgainCheckInputParam;

//ÿ�ִ��㷨ѧϰ����ṹ��
typedef struct _EACH_TYPLE_TRAIN_INFO
{
	UINT nTypeID;                                    //���㷨����
	UINT nFlagStudy;                                 //���㷨����ѧϰ�Ƿ�ɹ� 0���ɹ� 1������ѧϰ�ɹ� 2��ȫ��ʧ��
	UINT nStatusErrorID;                             //����ID 0������ 1���쳣
	char chStatusDescription[DH_MAX_TEXTSIZE];       //��������

	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_EACH_TYPLE_TRAIN_INFO()
	{
		nTypeID = 0;
		nFlagStudy = 0;
		nStatusErrorID = 0;

		memset(&chStatusDescription, 0, DH_MAX_TEXTSIZE*sizeof(char));
	}
	~_EACH_TYPLE_TRAIN_INFO()
	{
		nTypeID = 0;
		nFlagStudy = 0;
		nStatusErrorID = 0;

		memset(&chStatusDescription, 0, DH_MAX_TEXTSIZE*sizeof(char));
	}
	_EACH_TYPLE_TRAIN_INFO(const _EACH_TYPLE_TRAIN_INFO& sETTI)
	{
		*this = sETTI;
	}
	_EACH_TYPLE_TRAIN_INFO& operator=(const _EACH_TYPLE_TRAIN_INFO& sETTInfo)
	{
		nTypeID = sETTInfo.nTypeID;
		nFlagStudy = sETTInfo.nFlagStudy;
		nStatusErrorID = sETTInfo.nStatusErrorID;

		memcpy(&chStatusDescription, sETTInfo.chStatusDescription, 
			DH_MAX_TEXTSIZE*sizeof(char));
		return (*this);
	}

}s_EachTypetrainInfo;

//�㷨����ṹ��
typedef  struct _ALG_CHECK_OUTPUT_PARAM
{
	s_CheckResult sCheckResult;					//�������Ϣ����
	list<s_EachTypetrainInfo> lstAlgTrainInfo;	//ÿ�ִ��㷨ѵ������б�

	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_ALG_CHECK_OUTPUT_PARAM()
	{
		lstAlgTrainInfo.clear();
	}
	~_ALG_CHECK_OUTPUT_PARAM()
	{
		lstAlgTrainInfo.clear();
	}
	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_ALG_CHECK_OUTPUT_PARAM(const _ALG_CHECK_OUTPUT_PARAM& sACOP)
	{
		*this = sACOP;
	}
	_ALG_CHECK_OUTPUT_PARAM& operator=(const _ALG_CHECK_OUTPUT_PARAM& sACOParam)
	{
		sCheckResult = sACOParam.sCheckResult;
		lstAlgTrainInfo = sACOParam.lstAlgTrainInfo;

		return (*this);
	}
	void lst_vec_clear()
	{
		sCheckResult.lst_vec_clear();
		//TRACE("lstAlgTrainInfo.size()=%d\n", lstAlgTrainInfo.size());
		if (0 < lstAlgTrainInfo.size())
		{
			lstAlgTrainInfo.clear();
		}
		//TRACE("��պ�lstAlgTrainInfo.size()=%d\n", lstAlgTrainInfo.size());
	}

}s_AlgCheckOutputParam;

//������������  0Ϊ������1����NΪ�Լ����������
typedef struct _STATUS
{
	UINT nErrorType;		              //���غ���״̬ 0:���� 1���쳣 2�������㷨��Ҫѧϰ 
	char chErrorInfo[DH_MAX_TEXTSIZE];	  //״̬����
	char chErrorContext[DH_MAX_TEXTSIZE]; //��ϸ��Ϣ

	//////////////////////////////////////////////////////////////////////////
	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_STATUS()
	{
		nErrorType = 0;

		memset(&chErrorInfo, 0, DH_MAX_TEXTSIZE*sizeof(char));
		memset(&chErrorContext, 0, DH_MAX_TEXTSIZE*sizeof(char));
	}
	_STATUS(const _STATUS& sS)
	{
		*this = sS;
	}
	_STATUS& operator=(const _STATUS& sStatus)
	{
		nErrorType = sStatus.nErrorType;

		memcpy(&chErrorInfo, sStatus.chErrorInfo, 
			DH_MAX_TEXTSIZE*sizeof(char));
		memcpy(&chErrorContext, sStatus.chErrorContext, 
			DH_MAX_TEXTSIZE*sizeof(char));

		return (*this);
	}

}s_Status;

//////////////////////////////////////////////////////////////////////////
// �����Զ���ģ��Ҫ�Ľṹ�� [7/27/2015 dhl]
typedef struct _COVER_IMAGE_INFO
{
	UINT nCoverCamNum;//�������������
	                  //0Ϊ�޸��ǣ��ṹ����ֵ��Ч��
	                  //1Ϊ��1��������ص����ṹ���е�һ��ֵ��Ч��
	                  //2Ϊ�ص������������಻�ܳ���2��,����������Ĭ��Ϊ2��
	INT pnCoverStart[2];//�ӳ����Ƿ�Χ
	INT pnCoverEnd[2];//�ӳ����Ƿ�Χ
	UINT pnCoverCameraID[2];//�ص����ID�ţ�����0��ʱ�����ص���
	_COVER_IMAGE_INFO()
	{
		nCoverCamNum = 0;
		pnCoverStart[0] = -1;
		pnCoverStart[1] = -1;
		pnCoverEnd[0] = -1;
		pnCoverEnd[1] = -1;
		pnCoverCameraID[0] = 0;
		pnCoverCameraID[1] = 0;
	}
	_COVER_IMAGE_INFO(const _COVER_IMAGE_INFO& sAII)
	{ 
		*this = sAII;
	}
	_COVER_IMAGE_INFO& operator=(const _COVER_IMAGE_INFO& sAIInfo)
	{
		nCoverCamNum = sAIInfo.nCoverCamNum;
		pnCoverCameraID[0] = sAIInfo.pnCoverCameraID[0];		
		pnCoverStart[0] = sAIInfo.pnCoverStart[0];
		pnCoverEnd[0] = sAIInfo.pnCoverEnd[0];
		pnCoverCameraID[1] = sAIInfo.pnCoverCameraID[1];
		pnCoverStart[1] = sAIInfo.pnCoverStart[1];
		pnCoverEnd[1] = sAIInfo.pnCoverEnd[1];
		return *this;
	}
}s_CoverImageInfo;
//ͼ�����Խṹ��  ��ͼ
typedef struct _ALG_AUTO_IMAGE_INFO
{
	UINT  nFlag;			//0:�ڰ׻Ҷ�(�ĸ�����ָ�붼��ָͬ��ͼ���׵�ַ);1:��ɫ��ͨ��(pImageData = NULL);����:pImageData��������Ч
	BYTE *pImageData;
	BYTE *pImageR;			//R ͨ������
	BYTE *pImageG;			//G ͨ������
	BYTE *pImageB;			//B ͨ������
	SIZE sizeImage;			//����ͼ����
	UINT  nBits;			//8//24//32
	s_CoverImageInfo sCoverImageInfo;
	
	UINT nCurCameraID;//��ǰ���ID��

	std::vector<CPoint>  vcPointROI;	//ROI�������� 
	//CRect rcROI;//ͼ��ROI
	_ALG_AUTO_IMAGE_INFO()
	{
		nFlag = 0;
		pImageData = 0;
		pImageR = 0;
		pImageG = 0;
		pImageB= 0;
		sizeImage.cx = sizeImage.cy = 0;
		nBits = 0;
		nCurCameraID = 0;
		//rcROI = CRect(0,0,0,0);
		vcPointROI.clear();	
	}
	~_ALG_AUTO_IMAGE_INFO()
	{
		vcPointROI.clear();			
	}
	_ALG_AUTO_IMAGE_INFO(const _ALG_AUTO_IMAGE_INFO& sAII)
	{
		*this = sAII;
	}
	_ALG_AUTO_IMAGE_INFO& operator=(const _ALG_AUTO_IMAGE_INFO& sAIInfo)
	{
		nFlag = sAIInfo.nFlag;
		pImageData = sAIInfo.pImageData;
		pImageR = sAIInfo.pImageR;
		pImageG = sAIInfo.pImageG;
		pImageB= sAIInfo.pImageB;
		sizeImage = sAIInfo.sizeImage;

		nBits = sAIInfo.nBits;
		sCoverImageInfo = sAIInfo.sCoverImageInfo;
		nCurCameraID = sAIInfo.nCurCameraID;
		vcPointROI = sAIInfo.vcPointROI;
		//rcROI = sAIInfo.rcROI;

		return (*this);
	}
	void lst_vec_clear()
	{
		vcPointROI.clear();	
	}
}s_AlgAutoImageInfo;

typedef struct _ALG_LOCMODE_INPUT_PARAM
{
	s_AlgAutoImageInfo  sAlgAutoImageInfo;//����ͼ��ṹ
	UINT nStartRegionID;//��ʼ����ID
	std::list<S_SingleCell_Or_RegionsInfo> lstCurSCellRegion;//���Ҷ�λ�㷨��Ҫ���������
	s_SysAuthority sSysAuthority;//Ȩ��
	int nCurComputerID;//��ǰ�����ID
	_ALG_LOCMODE_INPUT_PARAM()
	{	
		nStartRegionID = 0;//��Ч����ʼID
		//lstCreateTypeID.clear();
		lstCurSCellRegion.clear();
		nCurComputerID = 0;
	}
	~_ALG_LOCMODE_INPUT_PARAM()
	{
		/*if (lstCreateTypeID.size()>0)
		{
			lstCreateTypeID.clear();
		}*/			
	}
	_ALG_LOCMODE_INPUT_PARAM(const _ALG_LOCMODE_INPUT_PARAM& sAII)
	{
		*this = sAII;
	}
	_ALG_LOCMODE_INPUT_PARAM& operator=(const _ALG_LOCMODE_INPUT_PARAM& sAIInfo)
	{
		sAlgAutoImageInfo = sAIInfo.sAlgAutoImageInfo;
		//lstCreateTypeID = sAIInfo.lstCreateTypeID;
		nStartRegionID = sAIInfo.nStartRegionID;
		sSysAuthority = sAIInfo.sSysAuthority;
		lstCurSCellRegion = sAIInfo.lstCurSCellRegion;
		nCurComputerID = sAIInfo.nCurComputerID;
		return *this;		
	}
	void lst_vec_clear()
	{
		/*if (lstCreateTypeID.size()>0)
		{
			lstCreateTypeID.clear();
		}*/
		if (lstCurSCellRegion.size()>0)
		{
			std::list<S_SingleCell_Or_RegionsInfo>::iterator it;
			it = lstCurSCellRegion.begin();
			for (;it!=lstCurSCellRegion.end();it++)
			{
				it->lst_vec_clear();
			}
			lstCurSCellRegion.clear();
		}
	}

}s_AlgLocModeInputParam;

typedef struct _ALG_VARMODE_INPUT_PARAM
{
	s_AlgAutoImageInfo  sAlgAutoImageInfo;				 //����ͼ��ṹ
	//std::list<UINT> lstCreateTypeID; //��Ҫ�����Ĵ��㷨����	
	UINT nStartRegionID;//��ʼ����ID
	//std::list<S_SingleCell_Or_RegionsInfo> lstCurSCellRegion;//�������Ҷ�λ�㷨��Ҫ�ýṹ��
	S_SingleCellProperty sSingleCellPro;            //С����Ϣ
	s_SysAuthority sSysAuthority;
	int nCurComputerID;//��ǰ�����ID
	_ALG_VARMODE_INPUT_PARAM()
	{	
		nStartRegionID = 0;//��Ч����ʼID
		//lstCreateTypeID.clear();
		//lstCurSCellRegion.clear();
		nCurComputerID = 0;
	}
	~_ALG_VARMODE_INPUT_PARAM()
	{
		/*if (lstCreateTypeID.size()>0)
		{
			lstCreateTypeID.clear();
		}*/			
	}
	_ALG_VARMODE_INPUT_PARAM(const _ALG_VARMODE_INPUT_PARAM& sAII)
	{
		*this = sAII;
	}
	_ALG_VARMODE_INPUT_PARAM& operator=(const _ALG_VARMODE_INPUT_PARAM& sAIInfo)
	{
		sAlgAutoImageInfo = sAIInfo.sAlgAutoImageInfo;
		//lstCreateTypeID = sAIInfo.lstCreateTypeID;
		nStartRegionID = sAIInfo.nStartRegionID;
		sSysAuthority = sAIInfo.sSysAuthority;
		sSingleCellPro = sAIInfo.sSingleCellPro;
		//lstCurSCellRegion = sAIInfo.lstCurSCellRegion;
		nCurComputerID = sAIInfo.nCurComputerID;
		return *this;		
	}
	void lst_vec_clear()
	{
		/*if (lstCreateTypeID.size()>0)
		{
			lstCreateTypeID.clear();
		}
		if (lstCurSCellRegion.size()>0)
		{
			std::list<S_SingleCell_Or_RegionsInfo>::iterator it;
			it = lstCurSCellRegion.begin();
			for (;it!=lstCurSCellRegion.end();it++)
			{
				it->lst_vec_clear();
			}
			lstCurSCellRegion.clear();
		}*/
	}

}s_AlgVarModeInputParam;

//typedef struct _ALG_ADJUST_INPUT_PARAM
//{
//	s_AlgAutoImageInfo  sAlgAutoImageInfo;				 //����ͼ��ṹ
//	std::list<S_SingleCell_Or_RegionsInfo> lstCurSCellRegion;         //����ǰģ��ļ������ȫģ����Ϣ
//	std::list<S_SingleCell_Or_RegionsInfo> lstNearSCellRegion;         //����ǰ���������ڵ����ȫ����������Ϣ
//	s_SysAuthority sSysAuthority;
//	_ALG_ADJUST_INPUT_PARAM()
//	{
//		lstNearSCellRegion.clear();
//		lstCurSCellRegion.clear();
//	}
//	~_ALG_ADJUST_INPUT_PARAM()
//	{
//
//	}
//	_ALG_ADJUST_INPUT_PARAM(const _ALG_ADJUST_INPUT_PARAM& sAII)
//	{
//		*this = sAII;
//	}
//	_ALG_ADJUST_INPUT_PARAM& operator=(const _ALG_ADJUST_INPUT_PARAM& sAIInfo)
//	{
//		sAlgAutoImageInfo = sAIInfo.sAlgAutoImageInfo;
//		lstCurSCellRegion = sAIInfo.lstCurSCellRegion;
//		lstNearSCellRegion = sAIInfo.lstNearSCellRegion;
//		sSysAuthority = sAIInfo.sSysAuthority;
//		return *this;
//	}
//	void lst_vec_clear()
//	{
//		if (lstCurSCellRegion.size()>0)
//		{
//			std::list<S_SingleCell_Or_RegionsInfo>::iterator it;
//			it = lstCurSCellRegion.begin();
//			for (;it!=lstCurSCellRegion.end();it++)
//			{
//				it->lst_vec_clear();
//			}
//			lstCurSCellRegion.clear();
//		}
//		if (lstNearSCellRegion.size()>0)
//		{
//			std::list<S_SingleCell_Or_RegionsInfo>::iterator it;
//			it = lstNearSCellRegion.begin();
//			for (;it!=lstNearSCellRegion.end();it++)
//			{
//				it->lst_vec_clear();
//			}
//			lstNearSCellRegion.clear();
//		}
//	}
//}s_AlgAdjustInputParam;

typedef struct _TYPE_MODE_RESULT
{
	UINT nTypeID;//���㷨���ͺ�
	std::list<s_RegionProperty> lstRegionPro;	//������������
	_TYPE_MODE_RESULT()
	{
		nTypeID = 0;
		lstRegionPro.clear();
	}
	~_TYPE_MODE_RESULT()
	{

	}
	_TYPE_MODE_RESULT(const _TYPE_MODE_RESULT& sAII)
	{
		*this = sAII;
	}
	_TYPE_MODE_RESULT& operator=(const _TYPE_MODE_RESULT& sAIInfo)
	{
		nTypeID = sAIInfo.nTypeID;
		lstRegionPro = sAIInfo.lstRegionPro;
		return *this;
	}
	void lst_vec_clear()
	{
		if (lstRegionPro.size()>0)
		{
			std::list<s_RegionProperty>::iterator it, itEnd;
			it = lstRegionPro.begin();
			itEnd = lstRegionPro.end();
			for (; it != itEnd; ++it)
			{
				it->lst_vec_clear();
			}
			lstRegionPro.clear();
		}
	}
}s_TypeModeResult;

typedef struct _ALG_MODE_RESULT
{
	std::list<s_TypeModeResult> lstTypeModeResult;	
	_ALG_MODE_RESULT()
	{
		lstTypeModeResult.clear();
	}
	~_ALG_MODE_RESULT()
	{
		lstTypeModeResult.clear();
	}
	_ALG_MODE_RESULT(const _ALG_MODE_RESULT& sAII)
	{
		*this = sAII;
	}
	_ALG_MODE_RESULT& operator=(const _ALG_MODE_RESULT& sAIInfo)
	{
		lstTypeModeResult = sAIInfo.lstTypeModeResult;
		return *this;
	}
	void lst_vec_clear()
	{
		if (lstTypeModeResult.size()>0)
		{
			std::list<s_TypeModeResult>::iterator it, itEnd;
			it = lstTypeModeResult.begin();
			itEnd = lstTypeModeResult.end();
			for (; it != itEnd; ++it)
			{
				it->lst_vec_clear();
			}
			lstTypeModeResult.clear();			
		}
	}
}s_AlgModeResult;

//  [4/5/2016 dhl]
//ÿһ�������ṹ��
typedef struct _INNER_TYPE_PARAM
{
	char cParamName[20];//��������
	char cParamUnit[10];//������λ��
	int nTypeFlag;//�������ͣ�0ΪBOOL�Ϳ�������1Ϊdouble��
	double dCurValue;//��ǰֵ
	double dMinValue;//��Χ��Сֵ 
	double dMaxValue;//��Χ���ֵ
	double dMinStep;//��С��������
	
	_INNER_TYPE_PARAM()
	{
		memset(&cParamName, 0, 20*sizeof(char));
		memset(&cParamUnit, 0, 10*sizeof(char));
		nTypeFlag=1;//�������ͣ�0ΪBOOL�Ϳ�������1Ϊdouble��
		dCurValue=0.0;//��ǰֵ
		dMinValue=0.0;//��Χ��Сֵ 
		dMaxValue=0.0;//��Χ���ֵ
		dMinStep=0.000001;//��С��������
	}
	~_INNER_TYPE_PARAM()
	{
		
	}
	_INNER_TYPE_PARAM(const _INNER_TYPE_PARAM& sAII)
	{
		*this = sAII;
	}
	_INNER_TYPE_PARAM& operator=(const _INNER_TYPE_PARAM& sAIInfo)
	{
		memcpy(&cParamName, sAIInfo.cParamName, 20*sizeof(char));
		memcpy(&cParamUnit, sAIInfo.cParamUnit, 10*sizeof(char));
		nTypeFlag=sAIInfo.nTypeFlag;//�������ͣ�0ΪBOOL�Ϳ�������1Ϊdouble��
		dCurValue=sAIInfo.dCurValue;//��ǰֵ
		dMinValue=sAIInfo.dMinValue;//��Χ��Сֵ 
		dMaxValue=sAIInfo.dMaxValue;//��Χ���ֵ
		dMinStep=sAIInfo.dMinStep;//��С��������		
		return *this;
	}
	BOOL operator==(const _INNER_TYPE_PARAM& sAIInfo)
	{
		BOOL bFlag = FALSE;
		if (nTypeFlag==sAIInfo.nTypeFlag
			&&dCurValue==sAIInfo.dCurValue
			&&dMinValue==sAIInfo.dMinValue
			&&dMaxValue==sAIInfo.dMaxValue
			&&dMinStep==sAIInfo.dMinStep)
		{
			CString strSrc,strDst;
			strSrc.Format("%s",cParamName);
			strDst.Format("%s",sAIInfo.cParamName);
			if (strSrc==strDst)
			{
				strDst.Format("%s",sAIInfo.cParamUnit);
				strSrc.Format("%s",cParamUnit);
				if (strSrc==strDst)
				{
					bFlag = TRUE;
				}
			}
		}
		return bFlag;
	}
}s_InnerTypeParam;

//ÿ�����Ͳ���
typedef struct _TYPE_CHECK_PARAM
{
	UINT nInnerTypeID;//�������ͱ��
	char cInnerTypeName[20];//������������
	std::list<s_InnerTypeParam> lstInnerTypeParam;	//�������£�������Ҫ�����Ĳ�������
	_TYPE_CHECK_PARAM()
	{
		nInnerTypeID = 0;
		memset(&cInnerTypeName, 0, 20*sizeof(char));
		lstInnerTypeParam.clear();
	}
	~_TYPE_CHECK_PARAM()
	{
		lstInnerTypeParam.clear();
	}
	_TYPE_CHECK_PARAM(const _TYPE_CHECK_PARAM& sAII)
	{
		*this = sAII;
	}
	_TYPE_CHECK_PARAM& operator=(const _TYPE_CHECK_PARAM& sAIInfo)
	{
		lstInnerTypeParam = sAIInfo.lstInnerTypeParam;
		nInnerTypeID = sAIInfo.nInnerTypeID;
		memcpy(&cInnerTypeName, sAIInfo.cInnerTypeName, 20*sizeof(char));
		return *this;
	}
	BOOL operator==(const _TYPE_CHECK_PARAM& sAIInfo)
	{
		BOOL bFlag = TRUE;
		if (nInnerTypeID==sAIInfo.nInnerTypeID
			&&lstInnerTypeParam.size()==sAIInfo.lstInnerTypeParam.size()
			&&lstInnerTypeParam.size()>0)
		{
			std::list<s_InnerTypeParam>::iterator it, itEnd;
			std::list<s_InnerTypeParam>::const_iterator itAI;
			it = lstInnerTypeParam.begin();
			itEnd = lstInnerTypeParam.end();
			itAI = sAIInfo.lstInnerTypeParam.begin();
			for (; it != itEnd; ++it,++itAI)
			{
				if (*it==*itAI)
				{
					;
				}
				else
				{
					bFlag= FALSE;
				}
			}				

		}
		else
		{
			bFlag = FALSE;
		}
		return bFlag;
	}
	void lst_vec_clear()
	{
		if (lstInnerTypeParam.size()>0)
		{			
			lstInnerTypeParam.clear();			
		}
	}
}s_TypeCheckParam;

typedef struct _ALG_CHECK_PARAM
{
	UINT nAlgorithmTypeID;//���㷨���ͺ�
	std::list<s_TypeCheckParam> lstTypeCheckParam;	//�ô��㷨�����£����в�������
	_ALG_CHECK_PARAM()
	{
		nAlgorithmTypeID = 0;
		lstTypeCheckParam.clear();
	}
	~_ALG_CHECK_PARAM()
	{
		lstTypeCheckParam.clear();
	}
	_ALG_CHECK_PARAM(const _ALG_CHECK_PARAM& sAII)
	{
		*this = sAII;
	}
	_ALG_CHECK_PARAM& operator=(const _ALG_CHECK_PARAM& sAIInfo)
	{
		lstTypeCheckParam = sAIInfo.lstTypeCheckParam;
		nAlgorithmTypeID = sAIInfo.nAlgorithmTypeID;
		return *this;
	}
	BOOL operator==(const _ALG_CHECK_PARAM& sAIInfo)
	{
		BOOL bFlag=TRUE;
		if (nAlgorithmTypeID==sAIInfo.nAlgorithmTypeID
			&&lstTypeCheckParam.size()==sAIInfo.lstTypeCheckParam.size()
			&&lstTypeCheckParam.size()>0)
		{
			std::list<s_TypeCheckParam>::iterator it, itEnd;
			std::list<s_TypeCheckParam>::const_iterator itAI;
			it = lstTypeCheckParam.begin();
			itEnd = lstTypeCheckParam.end();
			itAI = sAIInfo.lstTypeCheckParam.begin();
			for (; it != itEnd; ++it,++itAI)
			{
				if (*it==*itAI)
				{
					;
				}
				else
				{
					bFlag= FALSE;
				}
			}				

		}
		else
		{
			bFlag = FALSE;
		}
		return bFlag;
	}
	void lst_vec_clear()
	{
		if (lstTypeCheckParam.size()>0)
		{
			std::list<s_TypeCheckParam>::iterator it, itEnd;
			it = lstTypeCheckParam.begin();
			itEnd = lstTypeCheckParam.end();
			for (; it != itEnd; ++it)
			{
				it->lst_vec_clear();
			}
			lstTypeCheckParam.clear();			
		}
	}
}s_ALGCheckParam;

typedef struct _CHECK_PARAMS
{
	std::list<s_ALGCheckParam> lstAlgCheckParam;	//�����㷨��������
	_CHECK_PARAMS()
	{
		lstAlgCheckParam.clear();
	}
	~_CHECK_PARAMS()
	{
		lstAlgCheckParam.clear();
	}
	_CHECK_PARAMS(const _CHECK_PARAMS& sAII)
	{
		*this = sAII;
	}
	_CHECK_PARAMS& operator=(const _CHECK_PARAMS& sAIInfo)
	{
		lstAlgCheckParam = sAIInfo.lstAlgCheckParam;
		return *this;
	}
	void lst_vec_clear()
	{
		if (lstAlgCheckParam.size()>0)
		{
			std::list<s_ALGCheckParam>::iterator it, itEnd;
			it = lstAlgCheckParam.begin();
			itEnd = lstAlgCheckParam.end();
			for (; it != itEnd; ++it)
			{
				it->lst_vec_clear();
			}
			lstAlgCheckParam.clear();			
		}
	}
}s_CheckParams;

#pragma pack()// �ļ�������ǿ�Ƶ��ֽڶ����޸�Ϊ�Զ����뷽ʽ [3/6/2012 Denghl]

#endif//_DHTemplate