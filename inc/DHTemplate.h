//DHTemplate.h   模版信息结构
//////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Copyright (c) 2011, 北京大恒图像视觉有限公司视觉系统部     
/* All rights reserved.													
/*																		
/* 文件名称： DHTemplate.h												
/* 摘要：模版信息结构体
/*
/* 在通用基础上添加票面质量检测结构信息
/* 作者李文峰
/* 初始版本为 1.0
/* 修改版本V4.3
/* 修改记录：
/*       文件结束将强制单字节对齐修改为自动字节对齐（该设置可能对调用其他动态库等较多的程序有影响）// dhl
/*       区域结构体新增变量，部分原变量变为数组 // dhl
/*       定位结果新增工序变量 // dhl
/*       开链表中区域链表按算法类型进行分链表 // dhl
/*       增加区域属性比较结果结构体和比较函数 // 2.1 dhl
/*       增加是否和主码关联标识//2.2dhl
/*       检测输入参数中增加是否判产品状态// 2.3 dhl
/*       为二次核查增加区域类型有关变量//2.4 dhl
/*       将大算法链表中增加检测层，检测层下挂区域链表 // 2.5
/*       去掉检测层结构，在大算法中新增个检测层个数参数 // 2.6
         /////////////////////////////V3.0修改记录[7/27/2015 dhl]
/*       增加自动建模需要的结构体 
/*       修改定位结果结构体float改为double类型
/*       去掉区域结构体中备用图结构信息
/*       去掉二次核查有关数据结构
/*       增加权限控制结构体及接口
/*       增加语言显示选项

/*       初始化参数中新增相机横纵分辨率//3.1 dhl
/*       检测结果结构体中新增区域属性结果参数，供前端使用，部分需要算法返回//3.2 dhl 20151216
/*       部分预留字段长度增加//3.2 dhl
/*       区域属性新增是否校验码//3.2 dhl
/*       自动建模增加计算机ID//3.3 dhl
/*       定位结构体赋值bug修改//3.4 dhl
        //////////////////////////////V4.0
/*       承接3.4版本，修改自动建模需要的结构体（更换名字及内容）
/*       区域结构体增加halcon定义的region结构体，用于建模显示及存储、算法校验用

/*       增加系统调节算法参数相关数据结构//4.1 dhl
/*       修改部分结构体比较函数    //4.2
/*       增加错误结构体中报错是否允许学习的标志，默认为0，不允许学习
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
//在区域属性结构体添加区域标识类型
enum eRegionFlag
{
	RT_PreProcessRegion = 0,   //预处理区域，例如定位Mark
	RT_RowCloumnCheckRegion,   //含有行列概念检测区域
	RT_SingleCheckRegion       //独立检测区域
};

//区域操作标志
enum eOperateFlag
{
	OF_DoNothing = 0,       //默认状态  不检测不学习
	OF_Check, 	    		//检测
	OF_Train,				//学习
	OF_CheckAndTrain		//边检测边学习
};

//[2009-4-22 by jzj]: add
//学习标志
enum eTrainFlag
{
	TF_NoTrain = 0,		//默认状态
	TF_DisTrain,		//丢弃学习
	TF_AddTrain			//添加学习
};

//定位组结果结构体
typedef  struct _LOC_GROUP_RESULT
{
	UINT  nLocGroupID;      //定位组号
	double fLineShiftLoc;    //横向偏移位置
	double fColShiftLoc;     //横向偏移位置
	double fRoundAngle;      //旋转角度，弧度制

	double fAffineMAT[9];    //仿射矩阵
	UINT  nLocCount;        //该定位组包含的定位个数

	BOOL  bHaveError;       //True，表示定位组有错

	UINT  nCheckLayerID;        //定位组所属检测层或工序组  [3/6/2012 Denghl]
	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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
	int nSysAuthority;//权限级别
	char cAuthor[DH_MAX_NAMESIZE];//权限操作者姓名
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

// 区域比较返回结果结构体 [3/12/2012 Denghl]
typedef   struct  _COMP_REGION_RESULT
{
	BOOL bCompResult[2];
	_COMP_REGION_RESULT()
	{
		bCompResult[0] = FALSE;
		bCompResult[1] = FALSE;
	}

}s_CompRegionResult;

//区域属性结构体
typedef   struct  _REGION_PROPERTY
{
	UINT    nRegionID;                   //区域唯一标识号
	UINT    nGraphDepth;                 //区域所处图层深度  备用图结构

	//std::list<UINT> lstParentsRegID;     //iRegionID 双亲结点链表      备用图结构
	//std::list<UINT> lstChildrenRegID;    //iRegionID 子结点链表        备用图结构

	eRegionFlag RegionFlag;              //区域标识（如预处理区域、含有行列概念检测区域、独立检测区域） 算法外部用
	eOperateFlag OperateFlag;			 //区域操作标志		
	eTrainFlag TrainFlag;				 //区域学习标志	

	BOOL    bIsValid;                    //是否有效  0：本区域不检测 1：检测	
	std::list<CPoint> lstRegionOrgJunc;  //区域在拼接图像中的原始坐标(做模板时用)
	std::list<CPoint> lstRegionOrg;      //区域在采集图像中的原始坐标
	HObject hRegionOrgJunc;
	HObject hRegionOrg;
	
	UINT    nCellID;					 //开号唯一标识号
	UINT    nRegionType;                 //区域类型（如条码1，监管码1、奖区）			
	UINT    nCellType;                   //区域所属单元类型（如盒标、箱标、联、小开）
	char    cRegionName[DH_MAX_NAMESIZE];//区域名称
	UINT    nComputerID;                 //前端计算机编号
	UINT    nCameraID;                   //前端相机编号
	UINT    nAlgorithmType;              //检测区域使用算法类型  //接着三个改为数组方式，一次为序号0，二次为序号1  /
	UINT    nInnerAlgType;				 //检测区域使用算法类型(小算法类型）
	UINT    nLocateGroupID;              //区域所属定位组号
	BOOL    bIsModifyModel;              //是否使用单个修改后的模板
	UINT    nFlagRegionShape;            //区域形状标示 0：矩形 1：多边形

	BOOL    bIsPrimaryCode;							//是否为主码
	BOOL    bIsExistExpectCode;						//是否存在预知号
	std::vector<char> vcExpectCode;			        // 区域的预知号码
	char    cExpectCodeFileName[DH_MAX_NAMESIZE];   //区域预知号文件名称

	UINT    nColumnIndex;							//区域所属单元全局列号	
	UINT    nRowIndex;								//区域所属单元全局行号
	UINT    nRegionImageBits;						//图像位数  
	UINT    nProductColumnIndex;					//同一区域类型的列号
	UINT    nProductRowIndex;						//同一区域类型的行号

	// 新增区域属性 [3/6/2012 Denghl]
	BOOL    bJudgeProduct;                          //定位区域是否用于判产品（仅定位区域使用）
	UINT    nInheritLocGroup;                       //定位区域所继承的定位组号（仅定位区域使用）
	UINT    nCheckLayerID;                          //定位组所属检测层（或叫工序组）
    BOOL    bIsSetCell;                             //是否人工选择的所属的小开
	//BOOL    bIsOnlineReg;                           //是否是在线检测模板（一次模板）所建区域
	BOOL    bIsAssoPrimaryCode;                     //是否和主码数据相关联
	// \\ [3/6/2012 Denghl]

	// 二次核查模板区域类型有关变量 [3/31/2012 Denghl]
	//char    cDefineMarkAgain[DH_MAX_NAMESIZE];		//客户用于标识区域自定义字符
	//UINT    nRegionTypeAgain;                       //区域类型（如条码1，监管码1、奖区）			
	//char    cRegionNameAgain[DH_MAX_NAMESIZE];      //区域名称

	char    cDefineMark[DH_MAX_NAMESIZE];			//客户用于标识区域自定义字符
	char    cResvered[DH_MAX_NAMESIZE];				//保留
	UINT    nResvered[4];							//保留
	BOOL    bIsVerifyCode;                          //是否是校验码


	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/13/2011 ChenLiang]
	_REGION_PROPERTY()
	{
		nRegionID = 0;
		nGraphDepth = 0;
		//lstParentsRegID.clear();
		//lstChildrenRegID.clear();

		RegionFlag = RT_PreProcessRegion;
		OperateFlag = OF_DoNothing;
		TrainFlag =	TF_NoTrain;

		bIsValid = TRUE;// 改FALSE为TRUE [3/27/2012 Denghl]
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
		//[fdh]暂时修改
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

		return !bFlag;// 等于返回TRUE，不等返回FALSE [4/26/2012 Denghl]

	}
	void lst_vec_clear()
	{
		//清空list 和 vector
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
	UINT    nCellID;								//开号唯一标识号
	UINT    nCellTypeSN;							//小开类型编号
	UINT    nCellColumnIndex;						//小开所在列号
	UINT    nCellRowIndex;							//小开所在行号
	UINT    nCameraID;								//所属相机编号 
	UINT    nFlagRegionShape;						//区域形状标示 0：矩形 1：多边形


	char    cResvered[32];							//保留
	UINT    nResvered[2];							//保留

	std::list<CPoint> lstCell;						//区域在采集图像中的原始坐标

	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/13/2011 ChenLiang]
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
//	std::list<s_RegionProperty> lstSRegPro;			//检测区域链表 
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
	UINT nAlgID;                                    //大算法ID
	std::list<s_RegionProperty> lstSRegPro;			//检测区域链表 

	UINT nCheckLayerNum;// 检测层个数 [4/1/2012 Denghl]

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

//开或区域信息结构体
typedef struct _SINGLECELLOrREGIONSINFO
{
	//std::list<s_RegionProperty> lstSRegPro;			//检测区域链表  //小开下挂大算法链表
	std::list<s_AlgRegions> lstAlgRegions;
	S_SingleCellProperty sSingleCellPro;            //小开信息

	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/13/2011 ChenLiang]	
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

//模版结构体
typedef struct _DH_TEMPLATE
{
	std::list<S_SingleCell_Or_RegionsInfo> lstSCellRegion;    //开区域链表
	UINT    nComputerID;                 //前端计算机编号
	UINT    nCameraID;                   //前端相机编号
//	BOOL    bIsSecVerify;                //是否是二次核查模板

	char    cTemplateName[DH_MAX_NAMESIZE];					  //模版名称
	char    cTemplateTime[DH_MAX_NAMESIZE];					  //模版时间
	char    cResvered[DH_MAX_NAMESIZE];						  //保留
	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/13/2011 ChenLiang]	
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

//错误信息
typedef struct _ERRORINFO
{
	UINT   nRegionID;									//区域唯一标识号
	UINT   nLocateGroupID;								//区域所属定位组号
	UINT   nArea;										//面积值    
	BYTE   bytErrorType;								//区域图错误类型表
	BYTE   bytErrorGrade;								//区域图 错误级别
	char   cErrorDescription[DH_MAX_TEXTSIZE];			//错误描述
	RECT   RectErrorReg;								//错误区域（在实时采集图像中位置）
	RECT   RectErrorModel;								//错误区域（在模版图像中位置）
	UINT   nValueGlimmer;								//荧光值 
	std::vector<char> vcCheckResultCode;				//识别号码

	char    cResvered[DH_MAX_TEXTSIZE];					//保留
	UINT    nResvered[40];								//保留
	char    cRedisName[DH_MAX_TEXTSIZE];                //前端使用，算法不用填
	int    nRow;                                        //和区域或小开行相关
	int   nCol;                                         //和区域或小开列相关
	BOOL  bIsPrimary;                                   //是否主码
	BOOL  bIsVerifyCode;                                //是否是校验码
	UINT  nAllowStudyFlag;                              //是否允许误报学习


	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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

//小开内错误区域链表或者在一个检测区域内所有错误链表
typedef struct _SIGLECELL_CHECKRESULTE
{
	UINT   nErrorRegionCount;									//每小开结果区域向量有效个数（最大个数从配置文件获取）
	std::vector<s_sErrorInfo> vsErrorInfo;						//错误信息链表

	RECT    rCircumRect;										//裁切最小外接矩形 （说明：iImageType=1 直接返回模版区域，iImageType=2返回定位后旋转区域）
	UINT    nIndexCell;											//小开区域号
	std::vector<s_LocGroupResult> vsLocGroupResult;             //保存该小开（或检测区域）错误区域分析图像所有的定位信息
	UINT    nImageType;											//图像类型 0：大图，1：定位后区域图，2：定位后区域外接矩形图，3：小缺陷图 ljc [9/15/2010]
	BOOL   bFlagStudy;											//0:不能学习 1：能学习
	UINT    nAlgorithmType;										//小开内所有错误的大算法类型
	int     nRow;                                               //小开行号
	int     nCol;                                               //小开列号
	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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
		//TRACE("清空后vsErrorInfo.size()=%d\n",vsErrorInfo.size());
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
		//TRACE("清空后vsLocGroupResult.size()=%d\n",vsLocGroupResult.size());

		nErrorRegionCount = 0;
		nIndexCell = 0;
		nImageType = 0;
		bFlagStudy = FALSE;
		SetRect(&rCircumRect, 0, 0, 0, 0);
		nAlgorithmType = 0;
	}



}s_SigleCell_CheckResult;

//算法检测结果结构体
typedef  struct _CHECK_RESULT
{
	std::list<s_SigleCell_CheckResult> lstSigCCR;				//每小开结果链表 
	UINT   iErrorLstSigCCR;										//整大张图像小开结果的向量个数
	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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
		//TRACE("清空后lstSigCCR.size()=%d\n",lstSigCCR.size());

		iErrorLstSigCCR = 0;
	}

}s_CheckResult;

//初始化算法系统信息
typedef struct _ALG_INIT_PARAM
{
	char cTaskPath[DH_MAX_PATH];       //任务绝对路径
	char cAlgPath[DH_MAX_PATH];        //算法默认参数路径及字符模版绝对路径
	char cAlglogPath[DH_MAX_PATH];     //算法日志路径
	UINT     nWidth;        //图像宽
	UINT     nHeight;       //图像高
	UINT     nChannels;     //图像通道数
	double fResolution_H;//  相机横向分辨率
	double fResolution_V;//  相机纵向分辨率
	int nLanguage;//语言 默认 0为中文
	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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

//图像属性结构体  大图
typedef struct _ALG_IMAGE_INFO
{
	UINT  nFlag;			//0:黑白灰度(四个数据指针都相同指向图像首地址);1:彩色分通道(pImageData = NULL);其他:pImageData数据区无效
	BYTE *pImageData;
	BYTE *pImageR;			//R 通道数据
	BYTE *pImageG;			//G 通道数据
	BYTE *pImageB;			//B 通道数据
	SIZE sizeImage;			//输入图像宽高
	UINT  nBits;			//8//24//32

	UINT   nImageType;				//图像类型 0：大图，1：定位后区域图，2：定位后区域外接矩形图；3：小缺陷图 // ljc
	std::vector<CPoint>  vcPoint;	//当前图的区域坐标 

	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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

//模版图像大图结构体
typedef struct _TASK_IMAGE_INFO
{
	UINT nFlag;		    //0:黑白灰度(用pImageR存数据);1:彩色分通道(用RGB三个指针）

	BYTE *pImageR;		//R 通道数据
	BYTE *pImageG;		//G 通道数据
	BYTE *pImageB;		//B 通道数据

	SIZE sizeImage;			//输入图像宽高
	UINT  nBits;			//8//24//32

	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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

//算法中子算法结构体
typedef struct _INNER_ALG_TYPE
{
	UINT nInnerTypeID;					//算法类型ID(各算法层唯一标识)
	BOOL bSupportLearn;					//是否支持学习    TRUE：支持 FASLE：不支持 
	BOOL bPopUpDlg;						//学习是否弹出对话框 TRUE：弹出 FASLE：不弹出
	char cTypeName[DH_MAX_NAMESIZE];	//算法类型名称

	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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

//大算法信息结构体
typedef struct _ALG_TYPE
{
	UINT nTypeID;                               //大算法类型ID唯一标识
	char cTypeName[DH_MAX_NAMESIZE];            //大算法类型名称
	std::list< s_InnerAlgType > lstInnerAlgType;
	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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

//每种算法学习参数结构体
typedef struct _EACH_ALG_LEARN_INFO
{
	UINT nTypeID;         //大算法类型标识
	UINT nPieceStudy;     //大算法学习张数
	BOOL bTrainCancel;    //终止学习 TRUE:终止 FASLE：非终止

	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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

//检测输入参数结构体
typedef struct _ALG_CHECK_INPUT_PARAM
{
	s_DH_Template   sDHTemplate;                 //可变区域信息链表
	s_AlgImageInfo  sAlgImageInfo;				 //输入图像结构
	list<s_EachAlgLearnInfo>   lstAlgLearnInfo;  //每种大算法需学习信息表
	BOOL bJudgeProduct;// 是否判产品 [3/15/2012 Denghl]

	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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

////二次核查检测输入参数结构体
//typedef struct _ALG_AGAINCHECK_INPUT_PARAM
//{
//	s_DH_Template   sDHTemplate;                 //可变区域信息链表
//	s_AlgImageInfo  sAlgImageInfo;				 //输入图像结构，小开图或区域图，包括在原始大图中的座位坐标
//	list<s_EachAlgLearnInfo>   lstAlgLearnInfo;  //每种大算法需学习信息表
//	s_SigleCell_CheckResult sSigCellCheckResult;//在线检测的检测结果信息
//	char strTaskName[DH_MAX_PATH];              //任务路径名
//
//	//////////////////////////////////////////////////////////////////////////
//	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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

//每种大算法学习结果结构体
typedef struct _EACH_TYPLE_TRAIN_INFO
{
	UINT nTypeID;                                    //大算法类型
	UINT nFlagStudy;                                 //大算法本次学习是否成功 0：成功 1：部分学习成功 2：全部失败
	UINT nStatusErrorID;                             //错误ID 0：正常 1：异常
	char chStatusDescription[DH_MAX_TEXTSIZE];       //错误描述

	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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

//算法输出结构体
typedef  struct _ALG_CHECK_OUTPUT_PARAM
{
	s_CheckResult sCheckResult;					//检测结果信息链表
	list<s_EachTypetrainInfo> lstAlgTrainInfo;	//每种大算法训练结果列表

	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
	_ALG_CHECK_OUTPUT_PARAM()
	{
		lstAlgTrainInfo.clear();
	}
	~_ALG_CHECK_OUTPUT_PARAM()
	{
		lstAlgTrainInfo.clear();
	}
	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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
		//TRACE("清空后lstAlgTrainInfo.size()=%d\n", lstAlgTrainInfo.size());
	}

}s_AlgCheckOutputParam;

//函数返回类型  0为正常，1－－N为自己定义的类型
typedef struct _STATUS
{
	UINT nErrorType;		              //返回函数状态 0:正常 1：异常 2：检测此算法需要学习 
	char chErrorInfo[DH_MAX_TEXTSIZE];	  //状态描述
	char chErrorContext[DH_MAX_TEXTSIZE]; //详细信息

	//////////////////////////////////////////////////////////////////////////
	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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
// 增加自动建模需要的结构体 [7/27/2015 dhl]
typedef struct _COVER_IMAGE_INFO
{
	UINT nCoverCamNum;//覆盖相机个数，
	                  //0为无覆盖，结构体中值无效，
	                  //1为和1个相机有重叠，结构体中第一个值有效，
	                  //2为重叠了两个相机最多不能超过2个,超过两个的默认为2个
	INT pnCoverStart[2];//视场覆盖范围
	INT pnCoverEnd[2];//视场覆盖范围
	UINT pnCoverCameraID[2];//重叠相机ID号，等于0的时候，无重叠。
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
//图像属性结构体  大图
typedef struct _ALG_AUTO_IMAGE_INFO
{
	UINT  nFlag;			//0:黑白灰度(四个数据指针都相同指向图像首地址);1:彩色分通道(pImageData = NULL);其他:pImageData数据区无效
	BYTE *pImageData;
	BYTE *pImageR;			//R 通道数据
	BYTE *pImageG;			//G 通道数据
	BYTE *pImageB;			//B 通道数据
	SIZE sizeImage;			//输入图像宽高
	UINT  nBits;			//8//24//32
	s_CoverImageInfo sCoverImageInfo;
	
	UINT nCurCameraID;//当前相机ID号

	std::vector<CPoint>  vcPointROI;	//ROI区域坐标 
	//CRect rcROI;//图像ROI
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
	s_AlgAutoImageInfo  sAlgAutoImageInfo;//输入图像结构
	UINT nStartRegionID;//起始区域ID
	std::list<S_SingleCell_Or_RegionsInfo> lstCurSCellRegion;//查找定位算法需要输入的区域
	s_SysAuthority sSysAuthority;//权限
	int nCurComputerID;//当前计算机ID
	_ALG_LOCMODE_INPUT_PARAM()
	{	
		nStartRegionID = 0;//无效的起始ID
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
	s_AlgAutoImageInfo  sAlgAutoImageInfo;				 //输入图像结构
	//std::list<UINT> lstCreateTypeID; //需要创建的大算法链表	
	UINT nStartRegionID;//起始区域ID
	//std::list<S_SingleCell_Or_RegionsInfo> lstCurSCellRegion;//单独查找定位算法需要该结构体
	S_SingleCellProperty sSingleCellPro;            //小开信息
	s_SysAuthority sSysAuthority;
	int nCurComputerID;//当前计算机ID
	_ALG_VARMODE_INPUT_PARAM()
	{	
		nStartRegionID = 0;//无效的起始ID
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
//	s_AlgAutoImageInfo  sAlgAutoImageInfo;				 //输入图像结构
//	std::list<S_SingleCell_Or_RegionsInfo> lstCurSCellRegion;         //调整前模板的检测输入全模板信息
//	std::list<S_SingleCell_Or_RegionsInfo> lstNearSCellRegion;         //调整前与该相机相邻的相机全区域链表信息
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
	UINT nTypeID;//大算法类型号
	std::list<s_RegionProperty> lstRegionPro;	//返回区域链表
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
//每一个参数结构体
typedef struct _INNER_TYPE_PARAM
{
	char cParamName[20];//参数名称
	char cParamUnit[10];//参数单位名
	int nTypeFlag;//参数类型：0为BOOL型开关量，1为double型
	double dCurValue;//当前值
	double dMinValue;//范围最小值 
	double dMaxValue;//范围最大值
	double dMinStep;//最小调整幅度
	
	_INNER_TYPE_PARAM()
	{
		memset(&cParamName, 0, 20*sizeof(char));
		memset(&cParamUnit, 0, 10*sizeof(char));
		nTypeFlag=1;//参数类型：0为BOOL型开关量，1为double型
		dCurValue=0.0;//当前值
		dMinValue=0.0;//范围最小值 
		dMaxValue=0.0;//范围最大值
		dMinStep=0.000001;//最小调整幅度
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
		nTypeFlag=sAIInfo.nTypeFlag;//参数类型：0为BOOL型开关量，1为double型
		dCurValue=sAIInfo.dCurValue;//当前值
		dMinValue=sAIInfo.dMinValue;//范围最小值 
		dMaxValue=sAIInfo.dMaxValue;//范围最大值
		dMinStep=sAIInfo.dMinStep;//最小调整幅度		
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

//每种类型参数
typedef struct _TYPE_CHECK_PARAM
{
	UINT nInnerTypeID;//参数类型编号
	char cInnerTypeName[20];//参数类型名称
	std::list<s_InnerTypeParam> lstInnerTypeParam;	//该类型下，所有需要调整的参数链表
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
	UINT nAlgorithmTypeID;//大算法类型号
	std::list<s_TypeCheckParam> lstTypeCheckParam;	//该大算法类型下，所有参数链表
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
	std::list<s_ALGCheckParam> lstAlgCheckParam;	//所有算法参数链表
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

#pragma pack()// 文件结束将强制单字节对齐修改为自动对齐方式 [3/6/2012 Denghl]

#endif//_DHTemplate