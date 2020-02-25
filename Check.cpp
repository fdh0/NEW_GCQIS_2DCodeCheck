// Check.cpp: implementation of the CCheck class.
//
//////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Copyright (c) 2008, 北京图像视觉技术有限公司视觉算法部     
/* All rights reserved.													
/*																		
/* 文件名称： Check.cpp												
/* 摘要： 算法检测类函数集以及检测参数和属性对话框类接口函数
/*
/* 作者： 邓红丽
/* 完成日期： 2011年5月10日
/************************************************************************/
#include "stdafx.h"
#include "AlgLab2DCodeCheck.h"
#include "Check.h"
#include "PropDlg.h"
#include "ParaDlg.h"
//#include "omp.h"
#include <ppl.h>
using namespace Concurrency;
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 初始化类成员变量,可能需要添加代码
CCheck::CCheck()
{
	memset(&m_sStatus,0,sizeof(s_Status));
	
	if (m_lstSingleCellInnerCheck.size()>0)
	{
		std::list<s_InnerCheckInfo>::iterator itInnerlstInfo;
		itInnerlstInfo = m_lstSingleCellInnerCheck.begin();
		for (;itInnerlstInfo!=m_lstSingleCellInnerCheck.end();itInnerlstInfo++)
		{
			if (itInnerlstInfo->lstInnerCheck.size()>0)
			{
				std::list<CInnerCheck>::iterator itInnerCheck;
				for (itInnerCheck = itInnerlstInfo->lstInnerCheck.begin();
					itInnerCheck!=itInnerlstInfo->lstInnerCheck.end();
					itInnerCheck++)
				{
					itInnerCheck->Free();
				}
			}
			itInnerlstInfo->lstInnerCheck.clear();
		}
	}
	m_lstSingleCellInnerCheck.clear();
	sPendTime.Start();
	nStart = TRUE; 
	m_bCheckTimeOut = FALSE;
	m_nAllTime = 600;
	m_bWriteLog = TRUE;
	m_blststr = FALSE;

	m_nRegionNum = 0;
	m_lstCellReg.clear();
	//m_csWriteLog=NULL;
	//m_bIsStopThread=FALSE;
	//m_bIsTheadLog=TRUE;
	m_nlstStrCell = 1;
	//
	m_sTatusArray = NULL;
	m_sTatusArray = new s_Status[m_nlstStrCell];
	//
	m_lstStr     = NULL;
	m_lstStr     = new std::list<CString>[m_nlstStrCell];
	for (int i=0; i<m_nlstStrCell; i++)
	{
		m_lstStr[i].clear();
	}
	m_blststr=TRUE;
	//  [10/14/2015 zl]
	//m_bcsWriteLog = FALSE;
	//m_csWriteLog=new CCriticalSection;
	//m_bcsWriteLog = TRUE;
	//m_bThreadRun = FALSE;
	//AfxBeginThread((AFX_THREADPROC)WriteLogThreadFunc,this,THREAD_PRIORITY_BELOW_NORMAL);
	//  [4/6/2017 zb]
	m_nLanguage = 0;
	m_nStudyRegNum = 0;
	memset(m_strMessageText,0,512*256*sizeof(char));
}
//void CCheck::WriteLogThreadFunc(LPVOID pParam)
//{
//	CCheck* pAlg = (CCheck*)pParam;
//	if(pAlg->m_bIsStopThread==FALSE)//pAlg->m_bIsTheadLog
//	{
//		pAlg->WriteLogProc();
//	}
//	
//}
//void CCheck::WriteLogProc()
//{
//	try
//	{
//		while(m_bIsStopThread==FALSE)
//		{
//			m_bThreadRun = TRUE;
//			if (m_bcsWriteLog==TRUE)
//			{
//				m_csWriteLog->Lock();
//			}
//
//			if(m_lstStr!=NULL&&m_blststr==TRUE)
//			{
//				int nStrNum=m_lstStr->size();
//				
//				if(nStrNum>0)
//				{
//					CString str;
//					str=m_lstStr->front();
//
//					m_lstStr->pop_front();
//
//					if (m_bcsWriteLog)
//					{
//						m_csWriteLog->Unlock();
//					}
//
//					//分解出日志类型和日志内容
//					int npos=str.Find('-');
//					CString strIndex=str.Mid(npos-1, npos);
//					int Index=_ttoi(strIndex);
//					int nStrLength=str.GetLength();
//					CString strContent=str.Mid(npos+1,nStrLength+1);
//					if(Index==1)
//					{
//					m_FileOperate.WriteAlgCheckLog(m_strLogPath,"CheckLog","",strContent);
//					}
//					if(Index==2)
//					{
//					m_FileOperate.WriteAlgOperateLog(m_strLogPath,"OperateLog","",strContent);
//					}
//					if(Index==3)
//					{
//					m_FileOperate.WriteAlgExceptionLog(m_strLogPath,"ExceptLog","",strContent);
//					}				
//				}
//			}
//			if (m_bcsWriteLog==TRUE)
//			{
//				m_csWriteLog->Unlock();
//			}
//			m_bThreadRun = FALSE;
//			Sleep(10);
//		}	
//	}
//	catch(...)
//	{
//		CString str;
//		str.Format("线程写日志异常");
//		CFileOperate FileOp;
//		FileOp.InitFile(m_strLogPath);
//		FileOp.WriteAlgExceptionLog(m_strLogPath,"ExceptLog","",str);
//		FileOp.FreeFile();
//	}
//	//	TRACE("thread1");
//	//	m_EventThread.SetEvent();
//	//	TRACE("thread2");
//		
//}
CCheck::~CCheck()
{
	if (m_lstSingleCellInnerCheck.size()>0)
	{
		std::list<s_InnerCheckInfo>::iterator itInnerlstInfo;
		itInnerlstInfo = m_lstSingleCellInnerCheck.begin();
		for (;itInnerlstInfo!=m_lstSingleCellInnerCheck.end();itInnerlstInfo++)
		{
			if (itInnerlstInfo->lstInnerCheck.size()>0)
			{
				std::list<CInnerCheck>::iterator itInnerCheck;
				for (itInnerCheck = itInnerlstInfo->lstInnerCheck.begin();
					itInnerCheck!=itInnerlstInfo->lstInnerCheck.end();
					itInnerCheck++)
				{
					itInnerCheck->Free();
				}
			}
			itInnerlstInfo->lstInnerCheck.clear();
		}
	}
	m_lstSingleCellInnerCheck.clear();
	m_lstCellReg.clear();
	m_nRegionNum = 0;
//	m_bIsStopThread=TRUE;
//	TRACE("check1");
//	WaitForSingleObject(m_EventThread,10000);
//	TRACE("check2");
	//Sleep(2);
	/*while(m_bThreadRun)
	{
	Sleep(1);
	}*/
	//////////////////////////////////////////////////////////////////////////
	if(m_lstStr)
	{
		for (int i=0; i<m_nlstStrCell; i++)
		{
			m_lstStr[i].clear();
		}
		//m_lstStr->clear();   //待定
		delete[] m_lstStr;
		m_lstStr=NULL;
	}
	//
	if (m_sTatusArray)
	{
		delete[] m_sTatusArray;
		m_sTatusArray = NULL;
	}
	/*if(m_csWriteLog)
	{
	m_bcsWriteLog = FALSE;
	delete m_csWriteLog;
	m_csWriteLog=NULL;
	}*/
	m_FileOperate.FreeFile();
}


//******************************************************************************//
//函数:检测类初始化
//参数:
//	sAlgInitParam    输入，全局路径
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::Init(const s_AlgInitParam &sAlgInitParam,s_ALGCheckParam* psCheckParams,s_SysAuthority sSysAuthority)
{

	

	memcpy(&m_sAlgInitParam,&sAlgInitParam,sizeof(s_AlgInitParam));
	// 读取算法类型配置编号 [11/8/2011 Denghl]
	//m_bIsStopThread=FALSE;

	m_strLogPath.Format("%s\\AlgLab2DCodeCheck\\", m_sAlgInitParam.cAlglogPath);
	
	BOOL bCreatePathFlag = TRUE;
	bCreatePathFlag = m_FileOperate.CreateMultiLevelPath(m_strLogPath);
	
	bCreatePathFlag =  m_FileOperate.InitFile(m_strLogPath);
	if (bCreatePathFlag == FALSE)
	{
		m_sStatus.nErrorType = 1;
		CString str;
		str = "AlgLab2DCodeCheck Log Path Is not exist!";
		sprintf_s(m_sStatus.chErrorInfo,str);
		sprintf_s(m_sStatus.chErrorContext,str);

		return m_sStatus;
	}
	CString str;
	str.Format("%s",m_sAlgInitParam.cAlgPath);

	if (str.GetAt(str.GetLength()-1)=='\\')
	{
		m_strDefaultParaIni.Format("%sDefaultParameter\\AlgLab2DCodeCheck\\2DCodeCheckPara.ini", m_sAlgInitParam.cAlgPath);
	}
	else
	{
		m_strDefaultParaIni.Format("%s\\DefaultParameter\\AlgLab2DCodeCheck\\2DCodeCheckPara.ini", m_sAlgInitParam.cAlgPath);
	}
	
	//char strTemp[100];
	//GetPrivateProfileString("检测参数", "是否使用北钞校验库","1", strTemp, sizeof(strTemp), m_strDefaultParaIni);
	//m_bUseDll = atoi(strTemp);	

	//if (m_bUseDll)
	//{		
	//	if (m_hDll==NULL)
	//	{
	//		m_hDll = LoadLibrary("checkcode.dll");
	//		if (m_hDll != NULL)
	//		{
	//			/*checkSerialNumberFun = (lpCheckSerialNumberFun)GetProcAddress(hDll,"CheckSerialNumber");	
	//			checkBarCodeFun = (lpCheckBarCodeFun)GetProcAddress(hDll,"CheckBarCodeFun");
	//			freePrdMemFun = (lpFreePrdMemFun)GetProcAddress(hDll,"FreePrdMem");
	//			getRMBProductInfoFun = (lpGetRMBProductInfoFun)GetProcAddress(hDll,"GetRMBProductInfo");
	//			getRMBProductParamFun = (lpGetRMBProductParamFun)GetProcAddress(hDll,"GetRMBProductParam");*/
	//		}
	//		else
	//		{
	//			AfxMessageBox("checkcode.dll启动失败");
	//			m_hDll = NULL;
	//		}
	//	}

	//}



	//  [4/6/2017 zb]
	// 中英文路径配置读取
	CString strLanguage;
	if (str.GetAt(str.GetLength()-1)=='\\')
	{
		strLanguage.Format("%AlgSystem\\Language.ini",sAlgInitParam.cAlgPath);
	}
	else
	{
		strLanguage.Format("%s\\AlgSystem\\Language.ini",sAlgInitParam.cAlgPath);
	}
	m_nLanguage =sAlgInitParam.nLanguage;// GetPrivateProfileInt("Language Setting","Language ID",0,strLanguage);

	psCheckParams->lst_vec_clear();

	psCheckParams->nAlgorithmTypeID = ALG_TYPE_ID;
	s_TypeCheckParam sTypeParam;
	sTypeParam.nInnerTypeID = 1;
	if (m_nLanguage==1)
	{
		sprintf_s(sTypeParam.cInnerTypeName,"2D Code Quality");
	}
	else
	{
		sprintf_s(sTypeParam.cInnerTypeName,"二维码质量");
	}
	s_InnerTypeParam sInnerTypeParam;
	if (m_nLanguage==1)
	{
		sprintf_s(sInnerTypeParam.cParamName,"Check Code Lose");
	}
	else
	{
		sprintf_s(sInnerTypeParam.cParamName,"缺印检测");
	}
	sInnerTypeParam.nTypeFlag = 0;
	sInnerTypeParam.dMinStep = 1;
	sInnerTypeParam.dMinValue = 0;
	sInnerTypeParam.dMaxValue = 1;	
	sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	if (m_nLanguage==1)
	{
		sprintf_s(sInnerTypeParam.cParamName,"Inky Differences");
	}
	else
	{
		sprintf_s(sInnerTypeParam.cParamName,"墨色差异");
	}
	sInnerTypeParam.nTypeFlag = 1;
	sInnerTypeParam.dMinStep = 1;
	sInnerTypeParam.dMinValue = 0;
	sInnerTypeParam.dMaxValue = 255;	
	sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	if (m_nLanguage==1)
	{
		sprintf_s(sInnerTypeParam.cParamName,"Percentile");
	}
	else
	{
		sprintf_s(sInnerTypeParam.cParamName,"白线分位数");
	}
	sInnerTypeParam.nTypeFlag = 1;
	sInnerTypeParam.dMinStep = 1;
	sInnerTypeParam.dMinValue = 0;
	sInnerTypeParam.dMaxValue = 100;	
	sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	if (m_nLanguage==1)
	{
		sprintf_s(sInnerTypeParam.cParamName,"Check ExCursion");
	}
	else
	{
		sprintf_s(sInnerTypeParam.cParamName,"走版检测");
	}
	sInnerTypeParam.nTypeFlag = 0;
	sInnerTypeParam.dMinStep = 1;
	sInnerTypeParam.dMinValue = 0;
	sInnerTypeParam.dMaxValue = 1;	
	sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	if (m_nLanguage==1)
	{
		sprintf_s(sInnerTypeParam.cParamName,"Excursion Tolerance");
	}
	else
	{
		sprintf_s(sInnerTypeParam.cParamName,"走版公差");
	}
	sprintf_s(sInnerTypeParam.cParamUnit,"mm");
	sInnerTypeParam.nTypeFlag = 1;
	sInnerTypeParam.dMinStep = 0.05;
	sInnerTypeParam.dMinValue = 0;
	sInnerTypeParam.dMaxValue = 20;	
	sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	if (m_nLanguage==1)
	{
		sprintf_s(sInnerTypeParam.cParamName,"Yards Dirty");
	}
	else
	{
		sprintf_s(sInnerTypeParam.cParamName,"脏点检测");
	}
	sInnerTypeParam.nTypeFlag = 0;
	sInnerTypeParam.dMinStep = 1;
	sInnerTypeParam.dMinValue = 0;
	sInnerTypeParam.dMaxValue = 1;	
	sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	if (m_nLanguage==1)
	{
		sprintf_s(sInnerTypeParam.cParamName,"Spot Area");
	}
	else
	{
		sprintf_s(sInnerTypeParam.cParamName,"脏点面积");
	}
	sprintf_s(sInnerTypeParam.cParamUnit,"sq.mm");
	sInnerTypeParam.nTypeFlag = 1;
	sInnerTypeParam.dMinStep = 0.05;
	sInnerTypeParam.dMinValue = 0;
	sInnerTypeParam.dMaxValue = 100;	
	sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	psCheckParams->lstTypeCheckParam.push_back(sTypeParam);

	//m_sAlgInitParam.nLanguage = m_nLanguage;
	///
	int nTextNum = 0;
	if (m_nLanguage == 0)
	{
		m_strMessagePath.Format("%s\\DefaultParameter\\AlgLab2DCodeCheck\\International Config\\MessageChinese.ini", m_sAlgInitParam.cAlgPath);
		nTextNum = GetPrivateProfileInt("中文文本", "文本总数", 0, m_strMessagePath);
		char chMessage[256];
		CString strIndex;
		for(int i=0; i<nTextNum; i++)
		{ 
			memset(chMessage,0,256);
			strIndex.Format("%d",i);
			GetPrivateProfileString("详细内容",strIndex,"",chMessage,255,m_strMessagePath);
			memset(m_strMessageText[i],0,256*sizeof(char));
			memcpy(m_strMessageText[i],chMessage,256*sizeof(char));

		}
	}
	else if (m_nLanguage == 1)
	{
		m_strMessagePath.Format("%s\\DefaultParameter\\AlgLab2DCodeCheck\\International Config\\MessageEnglish.ini", m_sAlgInitParam.cAlgPath);
		nTextNum = GetPrivateProfileInt("English Text", "Total Text Number", 0, m_strMessagePath);
		char chMessage[256];
		CString strIndex;
		for(int i=0; i<nTextNum; i++)
		{ 
			memset(chMessage,0,256);
			strIndex.Format("%d",i);
			GetPrivateProfileString("Detail Content",strIndex,"",chMessage,255,m_strMessagePath);
			memset(m_strMessageText[i],0,256*sizeof(char));
			memcpy(m_strMessageText[i],chMessage,256*sizeof(char));
		}
	}
	//////////////////////////////////////////////////////////////////////////
	CString strContent;
	//0=Check检测:初始化
	strContent.Format("%s",m_strMessageText[0]);
	GenCurLog(strContent,2,0);
	WirteCurLog();
	//m_FileOperate.WriteAlgOperateLog(m_strLogPath, "操作日志", "操作日志", 
		//"检测:初始化");
	//if(m_bIsStopThread==TRUE)
	//{
		//m_bIsStopThread=FALSE;
		//AfxBeginThread((AFX_THREADPROC)WriteLogThreadFunc,this);
	//}
	sPendTime.Start();

	//[4/26/2012 Denghl]
	nStart = TRUE;
	return m_sStatus;
}
void CCheck::ReadSimplePara()
{
	if (m_lstSingleCellInnerCheck.size()>0)
	{
		if (m_lstSingleCellInnerCheck.begin()->lstInnerCheck.size()>0)
		{
			char strTemp[100];
			memset(strTemp,0,100);
			std::list<CInnerCheck>::iterator itInnerCheck;
			itInnerCheck = m_lstSingleCellInnerCheck.begin()->lstInnerCheck.begin();
			
			GetPrivateProfileString("条码外区域脏", "是否检测","0", 
				strTemp, sizeof(strTemp),
				itInnerCheck->m_strParaIni);
			m_sSimplePara.bDirtCheck = atoi(strTemp);
			GetPrivateProfileString("条码外区域脏", "单区域面积","3",
				strTemp, sizeof(strTemp),
				itInnerCheck->m_strParaIni);
			m_sSimplePara.dSingleArea = atoi(strTemp)*m_sAlgInitParam.fResolution_H*m_sAlgInitParam.fResolution_V;
			GetPrivateProfileString("条码内白线缺印", "是否检测","0", 
				strTemp, sizeof(strTemp), itInnerCheck->m_strParaIni);
			m_sSimplePara.bSkipCheck = atoi(strTemp);
			GetPrivateProfileString("条码内严重缺印", "灰度方差","5", 
				strTemp, sizeof(strTemp), itInnerCheck->m_strParaIni);
			m_sSimplePara.nDeviation = atoi(strTemp);
			GetPrivateProfileString("条码内白线缺印", "百分位","70", 
				strTemp, sizeof(strTemp), itInnerCheck->m_strParaIni);
			m_sSimplePara.nPercent = atoi(strTemp);
			GetPrivateProfileString("条码走版", "是否检测","0",
				strTemp, sizeof(strTemp), itInnerCheck->m_strParaIni);
			m_sSimplePara.bZoubanCheck = atoi(strTemp);
			GetPrivateProfileString("条码走版", "边界公差","0.5", 
				strTemp, sizeof(strTemp), itInnerCheck->m_strParaIni);
			m_sSimplePara.dZouBanRange = atof(strTemp);	

		}
	}
	
}
void CCheck::WriteSimplePara()
{
	char strTemp[100];	
	if (m_lstSingleCellInnerCheck.size()>0)
	{
		std::list<s_InnerCheckInfo>::iterator itCellInfo;
		itCellInfo = m_lstSingleCellInnerCheck.begin();
		for (;itCellInfo!=m_lstSingleCellInnerCheck.end();itCellInfo++)
		{
			if (itCellInfo->lstInnerCheck.size()>0)
			{
				std::list<CInnerCheck>::iterator itInnerCheck = itCellInfo->lstInnerCheck.begin();
				for (;itInnerCheck!=itCellInfo->lstInnerCheck.end();itInnerCheck++)
				{
					itInnerCheck->m_sCheckPrintParam.sZouBanPara.bValid = m_sSimplePara.bZoubanCheck;
					itInnerCheck->m_sCheckPrintParam.sZouBanPara.m_fZouBanRange = m_sSimplePara.dZouBanRange;
					itInnerCheck->m_sCheckPrintParam.sOutRangPara.bValid = m_sSimplePara.bDirtCheck;
					itInnerCheck->m_sCheckPrintParam.sOutRangPara.nSingleArea = 
						m_sSimplePara.dSingleArea/(m_sAlgInitParam.fResolution_H*m_sAlgInitParam.fResolution_V);
					itInnerCheck->m_sCheckPrintParam.sOutRangPara.nAllArea = itInnerCheck->m_sCheckPrintParam.sOutRangPara.nSingleArea*10;
					itInnerCheck->m_sCheckPrintParam.sInRegionPara.bValid = m_sSimplePara.bSkipCheck;
					itInnerCheck->m_sCheckPrintParam.sInHorverPara.bValid = m_sSimplePara.bSkipCheck;
					itInnerCheck->m_sCheckPrintParam.sInRegionPara.nDeviation = m_sSimplePara.nDeviation;
					itInnerCheck->m_sCheckPrintParam.sInHorverPara.nPercent = m_sSimplePara.nPercent;
					itInnerCheck->WriteSimplePara();					
				}
			}
		}
		
	}
}

//获取算法参数
s_Status CCheck::GetAlgParam(s_ALGCheckParam  * psCheckParams,s_SysAuthority sSysAuthority)
{	
	s_Status sStatus;
	psCheckParams->lst_vec_clear();
	psCheckParams->nAlgorithmTypeID = ALG_TYPE_ID;
	ReadSimplePara();
	s_TypeCheckParam sTypeParam;
	sTypeParam.nInnerTypeID = 1;
	if (m_nLanguage==1)
	{
		sprintf_s(sTypeParam.cInnerTypeName,"2D Code Quality");
	}
	else
	{
		sprintf_s(sTypeParam.cInnerTypeName,"二维码质量");
	}
	
	s_InnerTypeParam sInnerTypeParam;
	if (m_nLanguage==1)
	{
		sprintf_s(sInnerTypeParam.cParamName,"Check Code Lose");
	}
	else
	{
		sprintf_s(sInnerTypeParam.cParamName,"缺印检测");
	}
	
	sInnerTypeParam.nTypeFlag = 0;
	sInnerTypeParam.dMinStep = 1;
	sInnerTypeParam.dMinValue = 0;
	sInnerTypeParam.dMaxValue = 1;
	sInnerTypeParam.dCurValue = m_sSimplePara.bSkipCheck;
	sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	if (m_sSimplePara.bSkipCheck)
	{
		if (m_nLanguage==1)
		{
			sprintf_s(sInnerTypeParam.cParamName,"Inky Differences");
		}
		else
		{
			sprintf_s(sInnerTypeParam.cParamName,"墨色差异");
		}
		
		sInnerTypeParam.nTypeFlag = 1;
		sInnerTypeParam.dMinStep = 1;
		sInnerTypeParam.dMinValue = 0;
		sInnerTypeParam.dMaxValue = 255;	
		sInnerTypeParam.dCurValue = m_sSimplePara.nDeviation;
		sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
		if (m_nLanguage==1)
		{
			sprintf_s(sInnerTypeParam.cParamName,"Percentile");
		}
		else
		{
			sprintf_s(sInnerTypeParam.cParamName,"白线分位数");
		}
		
		sInnerTypeParam.nTypeFlag = 1;
		sInnerTypeParam.dMinStep = 1;
		sInnerTypeParam.dMinValue = 0;
		sInnerTypeParam.dMaxValue = 100;	
		sInnerTypeParam.dCurValue = m_sSimplePara.nPercent;
		sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	}
	if (m_nLanguage==1)
	{
		sprintf_s(sInnerTypeParam.cParamName,"Check ExCursion");
	}
	else
	{
		sprintf_s(sInnerTypeParam.cParamName,"走版检测");
	}
	
	sInnerTypeParam.nTypeFlag = 0;
	sInnerTypeParam.dMinStep = 1;
	sInnerTypeParam.dMinValue = 0;
	sInnerTypeParam.dMaxValue = 1;	
	sInnerTypeParam.dCurValue = m_sSimplePara.bZoubanCheck;
	sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	if (m_sSimplePara.bZoubanCheck)
	{
		if (m_nLanguage==1)
		{
			sprintf_s(sInnerTypeParam.cParamName,"Excursion Tolerance");
		}
		else
		{
			sprintf_s(sInnerTypeParam.cParamName,"走版公差");
		}
		
		sprintf_s(sInnerTypeParam.cParamUnit,"mm");
		sInnerTypeParam.nTypeFlag = 1;
		sInnerTypeParam.dMinStep = 0.05;
		sInnerTypeParam.dMinValue = 0;
		sInnerTypeParam.dMaxValue = 20;	
		sInnerTypeParam.dCurValue = m_sSimplePara.dZouBanRange;
		sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	}
	if (m_nLanguage==1)
	{
		sprintf_s(sInnerTypeParam.cParamName,"Yards Dirty");
	}
	else
	{
		sprintf_s(sInnerTypeParam.cParamName,"脏点检测");
	}
	
	sInnerTypeParam.nTypeFlag = 0;
	sInnerTypeParam.dMinStep = 1;
	sInnerTypeParam.dMinValue = 0;
	sInnerTypeParam.dMaxValue = 1;	
	sInnerTypeParam.dCurValue = m_sSimplePara.bDirtCheck;
	sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	if (m_sSimplePara.bDirtCheck)
	{
		if (m_nLanguage==1)
		{
			sprintf_s(sInnerTypeParam.cParamName,"Spot Area");
		}
		else
		{
			sprintf_s(sInnerTypeParam.cParamName,"脏点面积");
		}
		
		sprintf_s(sInnerTypeParam.cParamUnit,"sq.mm");
		sInnerTypeParam.nTypeFlag = 1;
		sInnerTypeParam.dMinStep = 0.01;
		sInnerTypeParam.dMinValue = 0;
		sInnerTypeParam.dMaxValue = 100;
		sInnerTypeParam.dCurValue = m_sSimplePara.dSingleArea;
		sTypeParam.lstInnerTypeParam.push_back(sInnerTypeParam);
	}	
	psCheckParams->lstTypeCheckParam.push_back(sTypeParam);
	return sStatus;

}

//设置算法参数
s_Status CCheck::SetAlgParam(const s_ALGCheckParam  sCheckParams,s_SysAuthority sSysAuthority)
{
	s_Status sStatus;
	if (sCheckParams.nAlgorithmTypeID==ALG_TYPE_ID&&sCheckParams.lstTypeCheckParam.size()>0)
	{
		if (sCheckParams.lstTypeCheckParam.begin()->nInnerTypeID==1
			&&sCheckParams.lstTypeCheckParam.begin()->lstInnerTypeParam.size()>0)
		{
			//  [4/28/2016 dhl]
			std::list<s_InnerTypeParam>::const_iterator itInnerPara;
			itInnerPara=sCheckParams.lstTypeCheckParam.begin()->lstInnerTypeParam.begin();
			for (;itInnerPara!=sCheckParams.lstTypeCheckParam.begin()->lstInnerTypeParam.end();
				itInnerPara++)
			{
				std::list<s_InnerTypeParam>::const_iterator itInnerPara;
				itInnerPara=sCheckParams.lstTypeCheckParam.begin()->lstInnerTypeParam.begin();
				for (;itInnerPara!=sCheckParams.lstTypeCheckParam.begin()->lstInnerTypeParam.end();
					itInnerPara++)
				{
					CString strName;
					strName.Format("%s",itInnerPara->cParamName);
					if (strName=="缺印检测")
					{
						m_sSimplePara.bSkipCheck = itInnerPara->dCurValue;
					}
					else if (strName=="墨色差异")
					{
						m_sSimplePara.nDeviation = itInnerPara->dCurValue;
					}
					else if (strName=="白线分位数")
					{
						m_sSimplePara.nPercent = itInnerPara->dCurValue;
					}
					else if (strName=="走版检测")
					{
						m_sSimplePara.bZoubanCheck = itInnerPara->dCurValue;
					}
					else if (strName=="走版公差")
					{
						m_sSimplePara.dZouBanRange = itInnerPara->dCurValue;
					}
					else if (strName=="脏点检测")
					{
						m_sSimplePara.bDirtCheck = itInnerPara->dCurValue;
					}
					else if (strName=="脏点面积")
					{
						m_sSimplePara.dSingleArea = itInnerPara->dCurValue;
					}
				}
			}	
			
			//写硬盘和更新文件
			WriteSimplePara();
		}		
	}
	else
	{
		//输入非法
		;
	}
	return sStatus;

}

//******************************************************************************//
//函数:检测类加载任务
//参数:
//	strTaskName    输入，任务名
//  sDHTemplate    输入，任务所有检测区域链表
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::LoadTask(const char *strTaskName, const s_DH_Template  &sDHTemplate,
	s_SysAuthority sSysAuthority,const BOOL bLoadFlag)
{
	s_Status sStatus;
	try
	{
		//生成路径
		m_strTaskName = strTaskName;
		m_strTaskPath.Format("%s\\%s",m_sAlgInitParam.cTaskPath,m_strTaskName);
		m_sDHTemplate = sDHTemplate;
		//m_bIsAgainCheck = sDHTemplate.bIsSecVerify;

		//生成检测链表
		Free(sSysAuthority);
		/*if(m_bIsStopThread==TRUE)
		{
		m_bIsStopThread=FALSE;
		AfxBeginThread((AFX_THREADPROC)WriteLogThreadFunc,this);
		}*/
		//if (m_bUseDll)
		//{		
		//	if (m_hDll==NULL)
		//	{
		//		m_hDll = LoadLibrary("checkcode.dll");
		//		if (m_hDll != NULL)
		//		{
		//			 checkBarCodeFun = (lpCheckBarCodeFun)GetProcAddress(m_hDll,"CheckBarCode");
		//			 freeRMBProductInfoMemFun = (lpFreeRMBProductInfoMemFun)GetProcAddress(m_hDll,"FreeRMBProductInfoMem");
		//			 freeRMBProductParamMemFun = (lpFreeRMBProductParamMemFun)GetProcAddress(m_hDll,"FreeRMBProductParamMem");
		//			 getRMBProductInfoFun = (lpGetRMBProductInfoFun)GetProcAddress(m_hDll,"GetRMBProductInfo");
		//			 getRMBProductParamFun = (lpGetRMBProductParamFun)GetProcAddress(m_hDll,"GetRMBProductParam");
		//			 if(getRMBProductInfoFun!=NULL&&freeRMBProductInfoMemFun!=NULL)
		//			 {
		//				 char ** chPrd = NULL;
		//				 int  iCount = 0;
		//				 int nRet = getRMBProductInfoFun(chPrd,iCount);
		//				 if(nRet==0)		
		//				 {
		//					 m_nProductNum = iCount;
		//					 for(int i = 0; i<iCount;i++)
		//					 {
		//						 if(chPrd&&chPrd[i])
		//						 {
		//							 CString str;
		//							 str.Format("%s",chPrd[i]);
		//							 m_vcstrAllProType.push_back(str);
		//						//	 AfxMessageBox(str);
		//						 }
		//					 }

		//					 freeRMBProductInfoMemFun(chPrd, iCount);
		//					 chPrd = NULL;
		//				 }
		//				 
		//			 }
		//		}
		//		else
		//		{
		//			AfxMessageBox("checkcode.dll启动失败");
		//			m_hDll = NULL;
		//			m_sStatus.nErrorType = 1;
		//			return m_sStatus;
		//		}
		//	}
		//}

		// 读取超时设置 [9/9/2015 dhl]
		CString strPath;
		CString str;
		str.Format("%s",m_sAlgInitParam.cTaskPath);

		//if (m_bIsAgainCheck)
		//{
		//	if (str.GetAt(str.GetLength()-1)=='\\')
		//	{
		//		
		//		strPath.Format("%s%s\\AgainAlgTask\\Com%dCam%d\\%s", 
		//			m_sAlgInitParam.cTaskPath,strTaskName, 
		//			sDHTemplate.nComputerID,
		//			sDHTemplate.nCameraID,
		//			"AlgLab2DCodeCheck");
		//	}
		//	else
		//	{
		//		strPath.Format("%s\\%s\\AgainAlgTask\\Com%dCam%d\\%s", 
		//			m_sAlgInitParam.cTaskPath,strTaskName, 
		//			sDHTemplate.nComputerID,
		//			sDHTemplate.nCameraID,
		//			"AlgLab2DCodeCheck");
		//	}
		//	// 二次核查路径 [3/28/2012 Denghl]
		//}
		//else
		//{
		if (str.GetAt(str.GetLength()-1)=='\\')
		{
			strPath.Format("%s%s\\AlgTask\\Com%dCam%d\\%s", 
				m_sAlgInitParam.cTaskPath,strTaskName, 
				sDHTemplate.nComputerID,
				sDHTemplate.nCameraID,
				"AlgLab2DCodeCheck");
		}
		else
		{
			strPath.Format("%s\\%s\\AlgTask\\Com%dCam%d\\%s", 
				m_sAlgInitParam.cTaskPath,strTaskName, 
				sDHTemplate.nComputerID,
				sDHTemplate.nCameraID,
				"AlgLab2DCodeCheck");
		}

		//}

		m_FileOperate.CreateMultiLevelPath(strPath);
		m_strPathini.Format("%s\\AlgLab2DCodeCheck.ini",strPath);//
		if (m_FileOperate.IsFileExist(m_strPathini))
		{
			char strTemp[100];
			memset(strTemp,0,100);
			GetPrivateProfileString("超时控制", "是否控制超时","0", strTemp, sizeof(strTemp), m_strPathini);
			m_bCheckTimeOut = atoi(strTemp);
			GetPrivateProfileString("超时控制", "大张检测时间","600", strTemp, sizeof(strTemp), m_strPathini);
			m_nAllTime = atoi(strTemp);
			// 添加检测日志控制 [9/29/2015 TIAN]
			GetPrivateProfileString("写日志控制", "是否写检测日志","1", strTemp, sizeof(strTemp), m_strPathini);
			m_bWriteLog = atoi(strTemp);
		}
		else
		{
			char strTemp[100];
			memset(strTemp,0,100);
			GetPrivateProfileString("超时控制", "是否控制超时","0", strTemp, sizeof(strTemp), m_strDefaultParaIni);
			m_bCheckTimeOut = atoi(strTemp);
			GetPrivateProfileString("超时控制", "大张检测时间","600", strTemp, sizeof(strTemp), m_strDefaultParaIni);
			m_nAllTime = atoi(strTemp);
			// 添加检测日志控制 [9/29/2015 TIAN]
			GetPrivateProfileString("写日志控制", "是否写检测日志","1", strTemp, sizeof(strTemp), m_strDefaultParaIni);
			m_bWriteLog = atoi(strTemp);
		}


		int nCellNum = m_sDHTemplate.lstSCellRegion.size();
		m_sCheckResult.lst_vec_clear();
		m_nRegionNum = 0;	
		std::list<S_SingleCell_Or_RegionsInfo>::const_iterator itSingleCell;
		int nCellCount =0;
		CString strTemp;

		//重新分配小开日志记录list数组
		if (m_lstStr)
		{
			for (int i=0; i<m_nlstStrCell; i++)
			{
				m_lstStr[i].clear();
			}
			delete[] m_lstStr;
			m_lstStr = NULL;
		}
		//
		if (m_sTatusArray)
		{
			delete[] m_sTatusArray;
			m_sTatusArray = NULL;
		}
		//按小开个数分配
		m_nlstStrCell = nCellNum;
		m_lstStr      = new std::list<CString>[m_nlstStrCell];
		m_sTatusArray = new s_Status[m_nlstStrCell];
		int nCellIndex = 0;
		//////////////////////////////////////////////////////////////////////////
		for (itSingleCell = (sDHTemplate.lstSCellRegion).begin(); 
			itSingleCell != (sDHTemplate.lstSCellRegion).end(); 
			itSingleCell++)
		{
			CString str1;
			/*str1.Format("%d进入\n",nCellCount);
			strTemp+=str1;*/

			s_InnerCheckInfo sInnerCheckInfo;
			sInnerCheckInfo.sSingleCellPro = itSingleCell->sSingleCellPro;
			//  [12/2/2014 baiyy]
			HObject hCellReg;
			//	GenEmptyObj(&hCellReg);

			HTuple row1,row2,col1,col2;
			HTuple Col, Row;
			Col = HTuple();
			Row = HTuple();
			std::list<CPoint>::iterator itPt, itPtEnd;
			std::list<CPoint> vPoint;
			vPoint =itSingleCell->sSingleCellPro.lstCell; 
			itPt = vPoint.begin();
			itPtEnd = vPoint.end();
			for (; itPt != itPtEnd; ++itPt)
			{
				TupleConcat(Col, HTuple(itPt->x), &Col);
				TupleConcat(Row, HTuple(itPt->y), &Row);
			}
			TupleMax(Row, &row2);
			TupleMin(Row, &row1);
			TupleMax(Col, &col2);
			TupleMin(Col, &col1);
			GenRectangle1(&hCellReg, row1, col1,row2,col2);
			HTuple lrow1,lrow2,lcol1,lcol2;
			SmallestRectangle1(hCellReg, &lrow1, &lcol1, &lrow2, &lcol2);
			//point_list_to_region(itSingleCell->sSingleCellPro.lstCell, hCellReg);
			sAlgCellInfo sAlgCell;
			sAlgCell.HRegCell = hCellReg;
			sAlgCell.nCellIndex = itSingleCell->sSingleCellPro.nCellID;
			m_lstCellReg.push_back(sAlgCell);


			std::list<s_RegionProperty>::const_iterator itRegionPro;		
			int nCellRegion = 0;
			std::list<s_AlgRegions>::const_iterator itAlglstRegions;
			itAlglstRegions = itSingleCell->lstAlgRegions.begin();
			if (itSingleCell->lstAlgRegions.size()>1)
			{
				// 记日志或报警 [3/28/2012 Denghl]
				;
			}
			for (itRegionPro = (itAlglstRegions->lstSRegPro).begin(); itRegionPro != itAlglstRegions->lstSRegPro.end(); itRegionPro++)
			{
				if (itRegionPro->nAlgorithmType==ALG_TYPE_ID)//ALGNAMECHECK)// 注意将算法类型编号计入 [5/16/2011 Denghl]
				{
					CInnerCheck *TmpInnerChecker;
					TmpInnerChecker = new CInnerCheck();
					TmpInnerChecker->m_lstStr = &m_lstStr[nCellIndex];
					// TmpInnerChecker->m_csWriteLog=m_csWriteLog;
					s_RegionProperty sRegionPro = *itRegionPro;
					TmpInnerChecker->m_bWriteLog = m_bWriteLog;
					TmpInnerChecker->m_sCheckPrintParam.sTimeOut.bValid = m_bCheckTimeOut;
					TmpInnerChecker->m_sCheckPrintParam.sTimeOut.nAllTime = m_nAllTime;				 
					sStatus = TmpInnerChecker->Init(m_strTaskName,m_sAlgInitParam,sRegionPro,1);


					if (sStatus.nErrorType!=0)
					{
						m_sStatus = sStatus;
					}
					sInnerCheckInfo.lstInnerCheck.push_back(*TmpInnerChecker);
					m_nRegionNum++;
					s_SigleCell_CheckResult sSigCellResult;
					sSigCellResult.bFlagStudy = 1;
					sSigCellResult.nAlgorithmType = ALG_TYPE_ID;
					sSigCellResult.nErrorRegionCount = 1;
					sSigCellResult.nImageType = 2;
					sSigCellResult.nIndexCell = itSingleCell->sSingleCellPro.nCellID;
					sSigCellResult.nRow = itSingleCell->sSingleCellPro.nCellRowIndex;
					sSigCellResult.nCol = itSingleCell->sSingleCellPro.nCellColumnIndex;
					sSigCellResult.vsErrorInfo = vector<s_sErrorInfo>(1);
					sSigCellResult.vsErrorInfo.at(0).nRegionID = TmpInnerChecker->m_sDstRegionProperty.nRegionID;
					sSigCellResult.vsErrorInfo.at(0).bytErrorType = 199;// 初始化错误 [12/5/2011 Denghl]
					sSigCellResult.vsErrorInfo.at(0).nLocateGroupID = TmpInnerChecker->m_sDstRegionProperty.nLocateGroupID;
					sSigCellResult.vsErrorInfo.at(0).nCol = sRegionPro.nColumnIndex;
					sSigCellResult.vsErrorInfo.at(0).nRow = sRegionPro.nRowIndex;
					sSigCellResult.vsErrorInfo.at(0).bIsPrimary = sRegionPro.bIsPrimaryCode;
					sSigCellResult.vsErrorInfo.at(0).bIsVerifyCode = sRegionPro.bIsVerifyCode;
					//
					CString strInitInfo=_T("");
					//1=初始化错误
					strInitInfo.Format("%s",m_strMessageText[1]);
					sprintf_s(sSigCellResult.vsErrorInfo.at(0).cErrorDescription,strInitInfo);
					
					sSigCellResult.rCircumRect = TmpInnerChecker->m_rcOrgMinRect;
					m_sCheckResult.lstSigCCR.push_back(sSigCellResult);
					m_sCheckResult.iErrorLstSigCCR++;
					delete TmpInnerChecker;
					TmpInnerChecker = NULL;
					// str1.Format("增加区域\n");
					// strTemp += str1;
				}
			}
			nCellIndex++;
			//strTemp+="增加小开";
			m_lstSingleCellInnerCheck.push_back(sInnerCheckInfo);
		}
		//WirteCurLog();
		//m_FileOperate.WriteAlgExceptionLog(m_strLogPath, "异常日志", "检测类",strTemp);
		
		//sPendTime.Start();
		//nStart = TRUE;
	}
	catch (HException &hException)
	{
		CString str;
		str.Format("LoadTask() function is abnormal, %s,%s",hException.ProcName().Text(),hException.ErrorMessage().Text());
		//m_FileOperate.WriteAlgExceptionLog(m_strLogPath,"ExceptionLog","ExceptionLog",str);
		GenCurLog(str,3,0);
	}
	catch(...)
	{
		CString str;
		str.Format("LoadTask() function is abnormal");
		//m_FileOperate.WriteAlgExceptionLog(m_strLogPath,"ExceptionLog","ExceptionLog",str);
		GenCurLog(str,3,0);
	}
	//写日志文件
	WirteCurLog();
	return m_sStatus;
}
int CCheck::point_list_to_region(std::list<CPoint> vPoint, HObject &hRegion)
{
	int nRet = 0;
	GenEmptyRegion(&hRegion);

	HTuple Col, Row;
	Col = HTuple();
	Row = HTuple();
	std::list<CPoint>::iterator itPt, itPtEnd;
	itPt = vPoint.begin();
	itPtEnd = vPoint.end();
	for (; itPt != itPtEnd; ++itPt)
	{
		TupleConcat(Col, HTuple(itPt->x), &Col);
		TupleConcat(Row, HTuple(itPt->y), &Row);
	}
	/*
	int nSize = vPoint.size();
	for (int i = 0; i < nSize; i++)
	{
	TupleConcat(Col, HTuple(vPoint[i].x), &Col);
	TupleConcat(Row, HTuple(vPoint[i].y), &Row);	
	}*/

	GenRegionPolygonFilled(&hRegion, Row, Col);
	
	HTuple lRegNum = 0;
	CountObj(hRegion, &lRegNum);
	///write_region(hRegion, "e:\\hRegion.reg");
	if (1 != lRegNum)
	{
		nRet = 2;
		return nRet;
	}

	return nRet;
}
//******************************************************************************//
//函数:检测释放空间
//参数:
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::Free(s_SysAuthority sSysAuthority)
{
	if (m_lstSingleCellInnerCheck.size()>0)
	{
		std::list<s_InnerCheckInfo>::iterator itInnerlstInfo;
		itInnerlstInfo = m_lstSingleCellInnerCheck.begin();
		for (;itInnerlstInfo!=m_lstSingleCellInnerCheck.end();itInnerlstInfo++)
		{
			if (itInnerlstInfo->lstInnerCheck.size()>0)
			{
				std::list<CInnerCheck>::iterator itInnerCheck;
				for (itInnerCheck = itInnerlstInfo->lstInnerCheck.begin();
					itInnerCheck!=itInnerlstInfo->lstInnerCheck.end();
					itInnerCheck++)
				{
					itInnerCheck->Free();
				}
			}
			itInnerlstInfo->lstInnerCheck.clear();
		}
	}
	//WirteCurLog();
	//if (m_bUseDll)
	//{
	//	if (m_hDll)
	//	{
	//		FreeLibrary(m_hDll);
	//		m_hDll = NULL;
	//	}
	//}

	m_lstSingleCellInnerCheck.clear();

	m_sCheckResult.lst_vec_clear();
	m_lstCellReg.clear();
	
	m_nRegionNum = 0;
	sPendTime.Start();
	m_sMiddleResult.lst_vec_clear();
	m_lstCellReg.clear();

	//写日志文件
	WirteCurLog();
	//m_bIsStopThread=TRUE;
	//while(m_bThreadRun)
	//{
		//Sleep(1);
	//}
	//if(m_lstStr!=NULL&&m_blststr==TRUE)
	//{		
	//	if(m_lstStr->size()==0)
	//	{
	//		;
	//	}
	//	else
	//	{
	//		while(!m_lstStr->empty()&&m_lstStr->size()>0)
	//		{
	//			CString str;
	//			str=m_lstStr->front();
	//			//分解出日志类型和日志内容
	//			int npos=str.Find('-');
	//			CString strIndex=str.Mid(npos-1, npos);
	//			int Index=_ttoi(strIndex);
	//			int nStrLength=str.GetLength();
	//			CString strContent=str.Mid(npos+1,nStrLength+1);
	//			if(Index==1)
	//			{
	//				m_FileOperate.WriteAlgCheckLog(m_strLogPath,"CheckLog","",strContent);
	//			}
	//			if(Index==2)
	//			{
	//				m_FileOperate.WriteAlgOperateLog(m_strLogPath,"OperateLog","",strContent);
	//			}
	//			if(Index==3)
	//			{
	//				m_FileOperate.WriteAlgExceptionLog(m_strLogPath,"ExceptLog","",strContent);
	//			}
	//			m_lstStr->pop_front();
	//		}
	//		m_lstStr->clear();
	//	}
	//	m_bIsStopThread=TRUE;
	//}
	// 状态结构体初始化 [3/22/2017 zb]
	memset(&m_sStatus.chErrorInfo, 0, DH_MAX_TEXTSIZE*sizeof(char));
	memset(&m_sStatus.chErrorContext, 0, DH_MAX_TEXTSIZE*sizeof(char));
	m_sStatus.nErrorType = 0;
	return m_sStatus;
}

//******************************************************************************//
//函数:设置中间结果
//参数:s_MiddleResult 中间结果结构体
//返回:
//	设置中间结果
//******************************************************************************//
s_Status CCheck::SetMiddleResult(const s_MiddleResult &sMiddleResult)
{
	m_sMiddleResult.vsLocGroupResult = sMiddleResult.vsLocGroupResult;
	return m_sStatus;

}

//定位算法调用检测虚函数重载
s_Status CCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage,s_MiddleResult *sMiddleResult,BOOL &bIsProduct)
{
	return m_sStatus;
}

////******************************************************************************//
////函数:检测主函数
////参数:
////	sAlgAgainCheckInputParam      输入，检测输入结构体
////	sAlgCheckOutputResult    输出，返回检测结果输出结构体
////返回:
////	算法执行状态结构体
////******************************************************************************//
//s_Status CCheck::Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
//					   s_AlgCheckOutputParam *sAlgCheckOutputResult)
//{
//	return m_sStatus;
//}
//
////******************************************************************************//
////函数:检测主函数
////参数:
////	sAlgAgainCheckInputParam      输入，检测输入结构体
////	sAlgCheckOutputResult    输出，返回检测结果输出结构体
////返回:
////	算法执行状态结构体
////******************************************************************************//
//s_Status CCheck::Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
//					   s_MiddleResult *sMiddleResult)
//{
//	// 需要填写 [3/26/2012 Denghl]
//	return m_sStatus;
//
//}

//******************************************************************************//
//函数:检测主函数
//参数:
//	sAlgCheckInputParam      输入，检测输入结构体
//	sAlgCheckOutputResult    输出，返回检测结果输出结构体
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage, s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	try
	{
		s_Status sTatusRegion;
		SetSystem("parallelize_operators", "false");// halcon开关并行 [9/17/2015 dhl]
		sPendTime.End();
	    //	CSpendTime sPendCheck;
	    //	sPendCheck.Start();
		//当前大算法需学习信息表
		

		HTuple htStartTime;
		CountSeconds(&htStartTime);
	
		std::list<s_EachTypetrainInfo> lstAlgTrainInfo; 
		s_EachTypetrainInfo itEachTrainInfo;

		int nTimeSpace = sPendTime.GetMillisecondInt();
		if (nTimeSpace>1000)
		{
			nStart = TRUE;
		}
		else
		{
			nStart = FALSE;
		}
		m_nStudyRegNum = 0;//每大张开始检测学习成功张数初始化为0

		m_sAlgImageInfo = sAlgCheckInputParam.sAlgImageInfo;
		m_lstEachAlgLearnInfo = sAlgCheckInputParam.lstAlgLearnInfo; 
		if (sAlgCheckInputParam.sAlgImageInfo.nBits==24)
		{
			if (sAlgCheckInputParam.sAlgImageInfo.pImageR==NULL
				||sAlgCheckInputParam.sAlgImageInfo.pImageG==NULL
				||sAlgCheckInputParam.sAlgImageInfo.pImageB==NULL)
			{
				m_sStatus.nErrorType = 1;
				CString strContent=_T("");
				//2=图像输入数据为空
				strContent.Format("%s",m_strMessageText[2]) ;
				sprintf_s(m_sStatus.chErrorContext,"%s",strContent);
				if (m_bWriteLog)
				{
					CString strContent;
					strContent.Format("%s",m_sStatus.chErrorContext);
					GenCurLog(strContent,3,0);
					//m_FileOperate.WriteAlgExceptionLog(m_strLogPath, "异常日志", "检测类",m_sStatus.chErrorContext);
				}
				WirteCurLog();
				return m_sStatus;
			}
		}
		else if (sAlgCheckInputParam.sAlgImageInfo.nBits==8)
		{
			if (sAlgCheckInputParam.sAlgImageInfo.pImageData==NULL)
			{
				m_sStatus.nErrorType = 1;
				CString strContent=_T("");
				//2=图像输入数据为空
				strContent.Format("%s",m_strMessageText[2]) ;
				sprintf_s(m_sStatus.chErrorContext,"%s",strContent);
				if (m_bWriteLog)
				{
					CString strContent;
					strContent.Format("%s",m_sStatus.chErrorContext);
					GenCurLog(strContent,3,0);
					//m_FileOperate.WriteAlgExceptionLog(m_strLogPath, "异常日志", "检测类",m_sStatus.chErrorContext);
				}
				WirteCurLog();
				return m_sStatus;

			}
		}
		else
		{
			m_sStatus.nErrorType = 1;
			CString strContent=_T("");
			//3=图像输入数据位数不对
			strContent.Format("%s",m_strMessageText[3]) ;
			sprintf_s(m_sStatus.chErrorContext,"%s",strContent);
			if (m_bWriteLog)
			{
				CString strContent;
				strContent.Format("%s",m_sStatus.chErrorContext);
				GenCurLog(strContent,3,0);
				//m_FileOperate.WriteAlgExceptionLog(m_strLogPath, "异常日志", "检测类",m_sStatus.chErrorContext);
			}
			WirteCurLog();
			return m_sStatus;
		}
		
		int  nLearnPiceNum = 0;
		BOOL bLeanCancel = TRUE;
		BOOL bTrainStatus =FALSE;

		std::list<s_EachAlgLearnInfo>::const_iterator itLearn =sAlgCheckInputParam.lstAlgLearnInfo.begin();
		for (;itLearn!=sAlgCheckInputParam.lstAlgLearnInfo.end();itLearn++)
		{
			if (itLearn->nTypeID == ALG_TYPE_ID)
			{
				nLearnPiceNum = itLearn->nPieceStudy;
				bLeanCancel = itLearn->bTrainCancel;
				bTrainStatus = TRUE;
				break;
			}
		}
		//  [6/6/2016 dh]
//		if (m_lstSingleCellInnerCheck.size()<=0
//			||m_lstSingleCellInnerCheck.size()!=m_sCheckResult.lstSigCCR.size())
//		{
////			m_sStatus.nErrorType = 1;
//			CString strTmp;
//			strTmp.Format("加载任务时，检测链表获取异常,lst=%d,rest=%d",
//				m_lstSingleCellInnerCheck.size(),m_sCheckResult.lstSigCCR.size());
//			//sprintf_s(m_sStatus.chErrorContext,"%s","加载任务时，检测链表获取异常");
//			m_FileOperate.WriteAlgExceptionLog(m_strLogPath, "异常日志", "检测类",strTmp);
//
//			return m_sStatus;
//		}
	   //int nThreadNum =  omp_get_thread_num();
		//if(m_lstSingleCellInnerCheck.size()<)
	   //int nThreadNum=4;//min(6,(int)m_lstSingleCellInnerCheck.size());
	   //omp_set_num_threads(nThreadNum);
		
//#ifndef _DEBUG
//#pragma omp parallel for if(true)
//#endif
		//清空状态数组
		memset(m_sTatusArray,0,sizeof(s_Status)*m_nlstStrCell);
		parallel_for ((UINT)0,(UINT)m_lstSingleCellInnerCheck.size(),[&](UINT nCellIndex)
		{	
			int nCountCell = sAlgCheckInputParam.sDHTemplate.lstSCellRegion.size();

			std::list<s_SigleCell_CheckResult>::iterator itResult;
			itResult = m_sCheckResult.lstSigCCR.begin();
			int nResultIndex = 0;
			//  [8/19/2015 dhl]
			std::list<s_InnerCheckInfo>::iterator itlstinnerCheck;
			itlstinnerCheck = m_lstSingleCellInnerCheck.begin();
			for (int j=0;j<m_lstSingleCellInnerCheck.size();
				j++,nResultIndex+=itlstinnerCheck->lstInnerCheck.size(),itlstinnerCheck++)
			{
				
				if (j==nCellIndex)
				{
					break;
				}
			}
			for (int k=0;k<m_sCheckResult.lstSigCCR.size();k++,itResult++)
			{
				if (k==nResultIndex)
				{
					break;
				}
			}

			std::list<S_SingleCell_Or_RegionsInfo>::const_iterator itSingleCell;
			
			
			HObject hRegAffCell;
			HObject hRegCell;


			if (nCountCell>0)//获取变化区域信息;
			{
				
				for (itSingleCell = (sAlgCheckInputParam.sDHTemplate.lstSCellRegion).begin(); 
					itSingleCell != (sAlgCheckInputParam.sDHTemplate.lstSCellRegion).end(); itSingleCell++)
				{
					if (itSingleCell->sSingleCellPro.nCellID==itlstinnerCheck->sSingleCellPro.nCellID)// 首先按开索引 [11/15/2011 Denghl]
					{
						std::list<s_RegionProperty>::const_iterator itRegionPro;
						std::list<CInnerCheck>::iterator itInnerCheck;
						std::list<s_AlgRegions>::const_iterator itAlgRegions;
						itAlgRegions = itSingleCell->lstAlgRegions.begin();
						if (itAlgRegions->lstSRegPro.size()>0)
						{
							for (itRegionPro = (itAlgRegions->lstSRegPro).begin(); 
								itRegionPro != itAlgRegions->lstSRegPro.end(); 
								itRegionPro++)
							{

								for (itInnerCheck=itlstinnerCheck->lstInnerCheck.begin();
									itInnerCheck!=itlstinnerCheck->lstInnerCheck.end();
									itInnerCheck++)
								{
									if (itRegionPro->nAlgorithmType==ALG_TYPE_ID//ALGNAMECHECK)// 注意将算法类型编号计入 [5/16/2011 Denghl]
										&&itInnerCheck->m_sDstRegionProperty.nRegionID==itRegionPro->nRegionID)
									{
										//  [12/2/2014 baiyy]
										//m_hRegCell = m_lstCellReg.at(nCellIndex);
										itInnerCheck->m_sDstRegionProperty = *itRegionPro;
										break;
									}
								}							
							}
						}
					}
					
				}
				//nCellIndex++;
			}
			std::list<CInnerCheck>::iterator itInnerCheck;
			for (itInnerCheck=itlstinnerCheck->lstInnerCheck.begin();
				itInnerCheck!=itlstinnerCheck->lstInnerCheck.end();
				itInnerCheck++)
			{
				

				s_InnerCheckInputParam sInnerCheckInputParam;
				s_InnerCheckOutputParam sInnerCheckOuputParam;
				sInnerCheckInputParam.sAlgImageInfo = m_sAlgImageInfo;
				bool bIsHaveLoc;
				bIsHaveLoc = false;
				for (int i=0;i<int(m_sMiddleResult.vsLocGroupResult.size());i++)
				{
					//if (!m_sMiddleResult.vsLocGroupResult[i].bHaveError)//  [1/9/2017 zb]
					//{
						if (itInnerCheck->m_sDstRegionProperty.nLocateGroupID==m_sMiddleResult.vsLocGroupResult[i].nLocGroupID)
						{
							sInnerCheckInputParam.sLocGroupResult = m_sMiddleResult.vsLocGroupResult[i];
							bIsHaveLoc = true;
							break;
						}
					//}			
				}

				//  [12/2/2014 baiyy]
				std::list<sAlgCellInfo>::const_iterator itAlgCell,itAlgCellEnd;
				itAlgCell = m_lstCellReg.begin();
				itAlgCellEnd = m_lstCellReg.end();
				for (;itAlgCell!=itAlgCellEnd;itAlgCell++)
				{
					if (itAlgCell->nCellIndex == itInnerCheck->m_sDstRegionProperty.nCellID)
					{
						hRegCell = itAlgCell->HRegCell;
						break;

					}
				}
				//是否有定位
				if (bIsHaveLoc)
				{
					s_LocGroupResult LGResult;
					LGResult = sInnerCheckInputParam.sLocGroupResult;
					HTuple htHomMat, htHomMatInv;
					htHomMat[0] = HTuple(LGResult.fAffineMAT[0]);
					htHomMat[1] = HTuple(LGResult.fAffineMAT[3]);
					htHomMat[2] = HTuple(LGResult.fAffineMAT[7]);
					htHomMat[3] = HTuple(LGResult.fAffineMAT[1]);
					htHomMat[4] = HTuple(LGResult.fAffineMAT[4]);
					htHomMat[5] = HTuple(LGResult.fAffineMAT[6]);
					HomMat2dInvert(htHomMat, &htHomMatInv);
					AffineTransRegion(hRegCell, &hRegAffCell, htHomMatInv, "false");
				}
				else
				{
					hRegAffCell = hRegCell;
				}

				sInnerCheckInputParam.sRegionProperty = itInnerCheck->m_sDstRegionProperty;

				sInnerCheckInputParam.nPieceStudy = nLearnPiceNum;
				sInnerCheckInputParam.bTrainCancel = bLeanCancel;
				if (nStart==TRUE)
				{
					sInnerCheckInputParam.nStartStatus = TRUE;
				}
				else
				{
					sInnerCheckInputParam.nStartStatus = FALSE;
				}


				HTuple lR1, lR2, lC1, lC2;
				SmallestRectangle1(hRegAffCell, &lR1, &lC1, &lR2, &lC2);
				lC1 = min(max(lC1[0].D(),0),m_sAlgImageInfo.sizeImage.cx-1);
				lR1 = min(max(lR1[0].D(),0),m_sAlgImageInfo.sizeImage.cy-1);
				lC2 = min(max(lC2[0].D(),0),m_sAlgImageInfo.sizeImage.cx-1);
				lR2 = min(max(lR2[0].D(),0),m_sAlgImageInfo.sizeImage.cy-1);
				itResult->nIndexCell=itlstinnerCheck->sSingleCellPro.nCellID;
				itResult->vsLocGroupResult.clear();
				itResult->vsLocGroupResult.push_back(sInnerCheckInputParam.sLocGroupResult);
				itResult->nRow = itlstinnerCheck->sSingleCellPro.nCellRowIndex;
				itResult->nCol = itlstinnerCheck->sSingleCellPro.nCellColumnIndex;
				HTuple htTime;

				CountSeconds(&htTime);
				
				if ((htTime[0].D()-htStartTime[0].D())*1000>=m_nAllTime&&m_bCheckTimeOut)
				{
					s_sErrorInfo sE;
					sE.bytErrorType = TIMEOUT;
					sE.nRegionID = sInnerCheckInputParam.sRegionProperty.nRegionID;
					sE.nLocateGroupID = sInnerCheckInputParam.sRegionProperty.nLocateGroupID;
					CString strContent=_T("");
					//4=无法识别，检测超时
					strContent.Format("%s",m_strMessageText[4]) ;
					sprintf_s(sE.cErrorDescription,"%s",strContent);

					std::vector<s_sErrorInfo>::iterator itError;

					itError= itResult->vsErrorInfo.begin();
					
					CRect rcMinRect;// 区域的最小外接矩形 [5/18/2011 Denghl]
					std::list<CPoint>::iterator itRegionPoint;
					if (sInnerCheckInputParam.sRegionProperty.lstRegionOrg.size()>0)
					{
						itRegionPoint=sInnerCheckInputParam.sRegionProperty.lstRegionOrg.begin();
						rcMinRect.top = rcMinRect.bottom = itRegionPoint->y;
						rcMinRect.left = rcMinRect.right = itRegionPoint->x;
						for(;itRegionPoint!=sInnerCheckInputParam.sRegionProperty.lstRegionOrg.end();
							itRegionPoint++)
						{
							rcMinRect.top = min(rcMinRect.top,itRegionPoint->y);
							rcMinRect.bottom = max(rcMinRect.bottom,itRegionPoint->y);
							rcMinRect.left = min(rcMinRect.left,itRegionPoint->x);
							rcMinRect.right = max(rcMinRect.right,itRegionPoint->x);
						}
					}			

					sE.RectErrorModel = rcMinRect;
					sE.RectErrorReg = rcMinRect;
					sE.nRow = sInnerCheckInputParam.sRegionProperty.nRowIndex;
					sE.nCol = sInnerCheckInputParam.sRegionProperty.nColumnIndex;
					sE.bIsPrimary = sInnerCheckInputParam.sRegionProperty.bIsPrimaryCode;
					sE.bIsVerifyCode = sInnerCheckInputParam.sRegionProperty.bIsVerifyCode;
					*itError = sE;
					if (itInnerCheck->m_bCheckResultMode)
					{
						itResult->rCircumRect = sE.RectErrorReg;
					}
					else
					{
						SetRect(&(itResult->rCircumRect),
							lC1[0].D(),
							lR1[0].D(), 
							lC2[0].D(),
							lR2[0].D());
					}
					
					itResult++;	
					sInnerCheckInputParam.lst_vec_clear();
					sInnerCheckOuputParam.lst_vec_clear();
					continue;
				}

				s_Status sTatus = itInnerCheck->Check(sInnerCheckInputParam,hSrcImage,&sInnerCheckOuputParam);// 逐个区域检测
			
				if (sTatus.nErrorType!=0)
				{
					sTatusRegion = sTatus;
					if (m_bWriteLog)
					{
						CString str;
						str.Format("ID=%d，InnerCheck-Except，%s",sInnerCheckInputParam.sRegionProperty.nRegionID,sTatus.chErrorContext);
						itInnerCheck->GenCurLog(str,3);
						//m_FileOperate.WriteAlgExceptionLog(m_strLogPath, "异常日志", "检测类",str);
					}
				}
//记录识别异常数据////////////////////////////////////////////////////////////////////////////////
//				if(sInnerCheckOuputParam.sErrorInfo.vcCheckResultCode.size()>0&&
//					sInnerCheckOuputParam.sErrorInfo.vcCheckResultCode.size()<=5)
//				{
////#pragma omp critical
//					{
//						CString str;
//						CFileOperate FileOperate;
//						CString strExPath;
//						strExPath.Format("E:\\CHECK字符识别异常\\");
//						FileOperate.InitFile(strExPath);
//						FileOperate.CreateMultiLevelPath(strExPath);
//						char cCode;
//						str.Format("ID=%d, 识别字符小于等于5:",sInnerCheckInputParam.sRegionProperty.nRegionID);
//						for(int i=0;i<sInnerCheckOuputParam.sErrorInfo.vcCheckResultCode.size();i++)
//						{
//							cCode=sInnerCheckOuputParam.sErrorInfo.vcCheckResultCode[i];
//							str+=cCode;
//						}
//						FileOperate.WriteAlgExceptionLog(strExPath,"","",str);
//						FileOperate.FreeFile();
//					}
//				}

//////////////////////////////////////////////////////////////////////////////////////////////

//				if (sInnerCheckOuputParam.nFlagStudy==0)
//				{
//#pragma omp critical
//					m_nStudyRegNum++;
//				}
				////////////////////////////
				// 临时测试用，张张报出来 [11/16/2011 Denghl]
				////////////////////////////
				std::vector<s_sErrorInfo>::iterator itError;

				itError= itResult->vsErrorInfo.begin();
				*itError = sInnerCheckOuputParam.sErrorInfo;
				itError->nRow = sInnerCheckInputParam.sRegionProperty.nRowIndex;
				itError->nCol = sInnerCheckInputParam.sRegionProperty.nColumnIndex;
				itError->bIsPrimary = sInnerCheckInputParam.sRegionProperty.bIsPrimaryCode;
				itError->bIsVerifyCode = sInnerCheckInputParam.sRegionProperty.bIsVerifyCode;
				SetRect(&(itError->RectErrorModel),
					lC1[0].D(),
					lR1[0].D(), 
					lC2[0].D(),
					lR2[0].D());
				
				if (itInnerCheck->m_bCheckResultMode)
				{
					itResult->rCircumRect = sInnerCheckOuputParam.sErrorInfo.RectErrorReg;
				}
				else
				{
					SetRect(&(itResult->rCircumRect),
						lC1[0].D(),
						lR1[0].D(), 
						lC2[0].D(),
						lR2[0].D());
				}	
				
				itResult++;	
				sInnerCheckInputParam.lst_vec_clear();
				sInnerCheckOuputParam.lst_vec_clear();
			}
			// end for每开内的区域检测
			/////更新没小开的状态记录//////////////////////////////////////////////
			if (sTatusRegion.nErrorType != 0)
			{
				m_sTatusArray[nCellIndex] = sTatusRegion;
			}
		});
		//////////////////////////////////////////////////////////////////////////
		for (int i=0; i<m_nlstStrCell; i++)
		{
			if (m_sTatusArray[i].nErrorType != 0)
			{
				m_sStatus = m_sTatusArray[i];
				break;
			}
		}
		//输出检测结果
		sAlgCheckOutputResult->sCheckResult = m_sCheckResult;
		sPendTime.Start();
		//sPendCheck.End();
		//int nTimeCheck = sPendCheck.GetMillisecondInt();
		//CString str;
		//str.Format("%d",nTimeCheck);
		//m_FileOperate.WriteAlgExceptionLog(m_strLogPath,"检测日志","检测时间",str);

	}
	catch(HException &hException)
	{
		m_sStatus.nErrorType = 1;
		sprintf_s(m_sStatus.chErrorContext,"CCheck()Except-HALCONExcept%s-%s", hException.ProcName().Text(),hException.ErrorMessage().Text());
		if (m_bWriteLog)
		{
			CString strErr;
			strErr.Format("CCheck()Except-HALCONExcept%s-%s",  hException.ProcName().Text(),hException.ErrorMessage().Text());
			GenCurLog(strErr,3,0);
			//m_FileOperate.WriteAlgExceptionLog(m_strLogPath, 
				//"异常日志", "检测类", strErr);
		}
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		sprintf_s(m_sStatus.chErrorContext,"%s","CCheck()Except");
		if (m_bWriteLog)
		{
			CString strContent;
			strContent.Format("%s",m_sStatus.chErrorContext);
			//
			GenCurLog(strContent,3,0);
			//m_FileOperate.WriteAlgExceptionLog(m_strLogPath,"异常日志","CCheck()异常","算法异常");
		}
	}
	WirteCurLog();
	return m_sStatus;
}

//******************************************************************************//
//函数:更新函数
//参数:
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::UpdateParam()
{
	//更新参数

	char strTemp[100];
	memset(strTemp,0,100);
	if (m_FileOperate.IsFileExist(m_strPathini))
	{
		GetPrivateProfileString("超时控制", "是否控制超时","0", strTemp, sizeof(strTemp), m_strPathini);
		m_bCheckTimeOut = atoi(strTemp);
		GetPrivateProfileString("超时控制", "大张检测时间","600", strTemp, sizeof(strTemp), m_strPathini);
		m_nAllTime = atoi(strTemp);
		GetPrivateProfileString("写日志控制", "是否写检测日志","1", strTemp, sizeof(strTemp), m_strPathini);
		m_bWriteLog = atoi(strTemp);
	}
	else
	{
		GetPrivateProfileString("超时控制", "是否控制超时","0", strTemp, sizeof(strTemp), m_strDefaultParaIni);
		m_bCheckTimeOut = atoi(strTemp);
		GetPrivateProfileString("超时控制", "大张检测时间","600", strTemp, sizeof(strTemp), m_strDefaultParaIni);
		m_nAllTime = atoi(strTemp);
		GetPrivateProfileString("写日志控制", "是否写检测日志","1", strTemp, sizeof(strTemp), m_strDefaultParaIni);
		m_bWriteLog = atoi(strTemp);
	}
	//////////////////////////////////////////////////////////////////////////
	std::list<CInnerCheck>::iterator itInnerCheck;
	std::list<s_InnerCheckInfo>::iterator itInnerInfo;
	for (itInnerInfo=m_lstSingleCellInnerCheck.begin();
		itInnerInfo!=m_lstSingleCellInnerCheck.end();
		itInnerInfo++)
	{
		for (itInnerCheck = itInnerInfo->lstInnerCheck.begin();itInnerCheck!=itInnerInfo->lstInnerCheck.end();itInnerCheck++)
		{
		    //itInnerCheck->FreeCheckMethod();
			itInnerCheck->m_bWriteLog = m_bWriteLog; //  [9/29/2015 TIAN]
			itInnerCheck->m_sCheckPrintParam.sTimeOut.bValid = m_bCheckTimeOut;
			itInnerCheck->m_sCheckPrintParam.sTimeOut.nAllTime = m_nAllTime;
			itInnerCheck->ReadTempPara();// 更新需要更新的区域 [5/17/2011 Denghl]
		}
	}

	return m_sStatus;
}
int CCheck::WriteTimePara(CString strPathini)
{
	char strTemp[100];
	sprintf(strTemp, "%d", m_bCheckTimeOut);
	WritePrivateProfileString("超时控制", "是否控制超时", strTemp, strPathini);
	sprintf(strTemp, "%d", m_nAllTime);
	WritePrivateProfileString("超时控制", "大张检测时间", strTemp, strPathini);	
	sprintf(strTemp, "%d", m_bWriteLog);
	WritePrivateProfileString("写日志控制", "是否写检测日志", strTemp, strPathini);	
	return 0;
}

//******************************************************************************//
//函数:学习后保存检测模板
//参数:
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::SaveCheckModel(const char *strTaskName,s_SysAuthority sSysAuthority)
{

	//保存算法需要的模板
	//可以不需要调用，因为现在学习完后算法内部保存，暂时留着，保持结构的一致性，每次学习完之后又会重复执行两次保存
	//
	std::list<CInnerCheck>::iterator itInnerCheck;
	std::list<s_InnerCheckInfo>::iterator itInnerInfo;
	for (itInnerInfo=m_lstSingleCellInnerCheck.begin();
		itInnerInfo!=m_lstSingleCellInnerCheck.end();
		itInnerInfo++)
	{
		for (itInnerCheck = itInnerInfo->lstInnerCheck.begin();itInnerCheck!=itInnerInfo->lstInnerCheck.end();itInnerCheck++)
		{
			itInnerCheck->SaveCheckModel(strTaskName);//保存模板
		}
	}
	
	return m_sStatus;

}

//******************************************************************************//
//函数:返回检测模板大图
//参数:
//	iTaskImgType             输入，需要返回的模板图像类型，0为大模板，1为小模板
//	sTaskImgInfo             输出，模板图像信息
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::GetCheckModelImg(const UINT iTaskImgType, s_TaskImageInfo &sTaskImgInfo,s_SysAuthority sSysAuthority)
{
	//将检测模板拼接起来，返回大图
	return m_sStatus;
}
BOOL CCheck::GenCurLog(CString &str,int nLogType,int nIndex)
{
	try
	{
		CString strTime;
		SYSTEMTIME curTime;
		GetLocalTime(&curTime);
		strTime.Format("%d-%02d%s%02d%s%02d%s%03d:",nLogType,curTime.wHour, ":", curTime.wMinute, ":", curTime.wSecond,":",
			curTime.wMilliseconds);
		CString strContent;
		strContent=strTime+str;
		str=strContent;

		if(m_lstStr[nIndex].size()>1000)
		{
			m_lstStr[nIndex].clear();
		}
		m_lstStr[nIndex].push_back(str);
	}
	catch(...)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CCheck::WirteCurLog()
{
	try
	{
		if(m_lstStr != NULL)
		{
			for (int i=0; i<m_nlstStrCell; i++)
			{
				while(m_lstStr[i].size()>0)
				{
					CString str;

					str = m_lstStr[i].front();

					m_lstStr[i].pop_front();

					//分解出日志类型和日志内容
					int npos=str.Find('-');
					CString strIndex=str.Mid(npos-1, npos);
					int Index=_ttoi(strIndex);
					int nStrLength=str.GetLength();
					CString strContent=str.Mid(npos+1,nStrLength+1);

					if(Index==1)
					{
						m_FileOperate.WriteAlgCheckLog(m_strLogPath,"CheckLog","",strContent);
					}
					if(Index==2)
					{
						m_FileOperate.WriteAlgOperateLog(m_strLogPath,"OperateLog","",strContent);
					}
					if(Index==3)
					{
						m_FileOperate.WriteAlgExceptionLog(m_strLogPath,"ExceptLog","",strContent);
					}
				}
			}
		}
	}
	catch(...)
	{
		CString str;
		str.Format("WirteCurLog Except");
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath,"ExceptLog","",str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// 属性
CSetPROP::CSetPROP()
{
}
CSetPROP::~CSetPROP()
{

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
s_Status CSetPROP::Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam)
{

	m_pChecker = (CCheck*)((CDHAlgLab2DCodeCheck*)pChecker)->m_pAlgLab2DCodeCheck;
	m_sAlgInitParam = sAlgInitParam;
	m_pChecker->m_sAlgInitParam = m_sAlgInitParam;
	m_strTaskName = strTaskName;
	m_pChecker->m_strTaskName = strTaskName;
	m_strAlgName.Format("LibAlg2DCodeCheck");
	//m_pChecker->m_bIsAgainCheck = bIsAgainCheck;

	//  [9/9/2015 dhl]

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
s_Status CSetPROP:: SetAlgParamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionProperty,s_SysAuthority sSysAuthority)
{

	CPropDlg Prodlg;
	m_pChecker->m_sAlgInitParam.nWidth = sAlgImageInfo.sizeImage.cx;
	m_pChecker->m_sAlgInitParam.nHeight = sAlgImageInfo.sizeImage.cy;
	m_pChecker->m_sAlgInitParam.nChannels = sAlgImageInfo.nBits/8;

	Prodlg.m_sAlgImageInfo = sAlgImageInfo;
	
	Prodlg.m_sDstRegionProperty = sRegionProperty;
	Prodlg.m_strTaskName = m_strTaskName;
	//  [4/6/2017 zb]
	m_pChecker->m_sAlgInitParam.nLanguage = m_pChecker->m_nLanguage;

	CString strPath,str,strPathini;
	str.Format("%s",m_sAlgInitParam.cTaskPath);
	if (str.GetAt(str.GetLength()-1)=='\\')
	{		
		strPath.Format("%s%s\\AlgTask\\Com%dCam%d\\%s", 
			m_sAlgInitParam.cTaskPath,
			m_strTaskName,
			sRegionProperty.nComputerID,
			sRegionProperty.nCameraID,
			"AlgLab2DCodeCheck");
	}
	else
	{
		strPath.Format("%s\\%s\\AlgTask\\Com%dCam%d\\%s", 
			m_sAlgInitParam.cTaskPath,
			m_strTaskName,
			sRegionProperty.nComputerID,
			sRegionProperty.nCameraID,
			"AlgLab2DCodeCheck");
	}	
	m_FileOperate.CreateMultiLevelPath(strPath);
	strPathini.Format("%s\\AlgLab2DCodeCheck.ini",strPath);
	m_pChecker->m_strPathini = strPathini;
	if (m_FileOperate.IsFileExist(strPathini))
	{
		char strTemp[100];
		memset(strTemp,0,100);
		GetPrivateProfileString("超时控制", "是否控制超时","0", strTemp, sizeof(strTemp), strPathini);
		m_pChecker->m_bCheckTimeOut = atoi(strTemp);
		GetPrivateProfileString("超时控制", "大张检测时间","600", strTemp, sizeof(strTemp), strPathini);
		m_pChecker->m_nAllTime = atoi(strTemp);
	}
	else
	{
		char strTemp[100];
		memset(strTemp,0,100);
		GetPrivateProfileString("超时控制", "是否控制超时","0", strTemp, sizeof(strTemp), m_pChecker->m_strDefaultParaIni);
		m_pChecker->m_bCheckTimeOut = atoi(strTemp);
		GetPrivateProfileString("超时控制", "大张检测时间","600", strTemp, sizeof(strTemp), m_pChecker->m_strDefaultParaIni);
		m_pChecker->m_nAllTime = atoi(strTemp);
	}
	Prodlg.m_pChecker = m_pChecker;

	// 弹对话框
	if (Prodlg.DoModal()==IDOK)
	{
		;
	}
	
	return m_sStatus;
}

//******************************************************************************//
//函数:拼接图像属性设置
//功能：将拼接的两个区域设置属性
//参数:
//	sAlgImageInfo     输入，图像结构体   
//	sRegionPropertyA  输入，区域属性
//	sRegionPropertyB  输入，区域属性

//返回
//	算法执行状态结构体
//******************************************************************************//
s_Status CSetPROP::SetAlgJuncParamdlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionPropertyA,
													const s_RegionProperty& sRegionPropertyB,s_SysAuthority sSysAuthority)
{
	// 弹对话框
	// 部分算法需要拼接图像两个区域设置属性参数，以前通用算法不用
	
	m_sStatus.nErrorType = 1;
	CString strContent=_T("");
	//5=该算法不支持跨相机拼接区域检测!
	strContent.Format("%s",m_pChecker->m_strMessageText[5]) ;
	sprintf_s(m_sStatus.chErrorContext,"%s",strContent);
	//AfxMessageBox(strContent);
	::MessageBoxA(NULL,strContent,"""",MB_OK);
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
s_Status CSetPROP::AlgCopyRegion(const s_AlgImageInfo& sAlgImageInfo,
					   s_RegionProperty &sDstRegionProperty,const s_RegionProperty &sSrcRegionProperty,int nDirection,s_SysAuthority sSysAuthority)
{
	int nSrcRegionID = sSrcRegionProperty.nRegionID;
	int nDstRegionID = sDstRegionProperty.nRegionID;

	

	CString strSrcComCam;
	CString strDstComCam;
	strSrcComCam.Format("Com%dCam%d",sSrcRegionProperty.nComputerID,sSrcRegionProperty.nCameraID);
	strDstComCam.Format("Com%dCam%d",sDstRegionProperty.nComputerID,sDstRegionProperty.nCameraID);
	

	if (1)// 内部算法只涉及复制文件内容的 [5/17/2011 Denghl]
	{
		SHFILEOPSTRUCT   fs;   
		char strSrc[DH_MAX_PATH] = {0};
		char strDst[DH_MAX_PATH] = {0};
		ZeroMemory(strSrc, MAX_PATH);
		ZeroMemory(strDst, MAX_PATH);
		CString strTmp;
		ZeroMemory(&fs,sizeof(SHFILEOPSTRUCT));   
		fs.hwnd   =   NULL;   
		fs.wFunc   =   FO_COPY;   
		fs.fFlags   =   FOF_ALLOWUNDO   |   FOF_NOCONFIRMATION   |   FOF_NOERRORUI  |FOF_MULTIDESTFILES  ;   
		fs.lpszProgressTitle   =   "copyfile";   
		CString str;
		str.Format("%s",m_sAlgInitParam.cTaskPath);

		if (str.GetAt(str.GetLength()-1)=='\\')
		{
			strTmp.Format("%s%s\\ALGTask\\%s\\%s\\%d",m_sAlgInitParam.cTaskPath,m_strTaskName,strSrcComCam,m_strAlgName,nSrcRegionID);
		}
		else
		{
			strTmp.Format("%s\\%s\\ALGTask\\%s\\%s\\%d",m_sAlgInitParam.cTaskPath,m_strTaskName,strSrcComCam,m_strAlgName,nSrcRegionID);
		}		

		memcpy(strSrc, strTmp, strTmp.GetLength());
		fs.pFrom   =  strSrc;
		strTmp.Empty();
		if (str.GetAt(str.GetLength()-1)=='\\')
		{
			strTmp.Format("%s%s\\ALGTask\\%s\\%s\\%d",m_sAlgInitParam.cTaskPath,m_strTaskName,strDstComCam,m_strAlgName,nDstRegionID);
		}
		else
		{
			strTmp.Format("%s\\%s\\ALGTask\\%s\\%s\\%d",m_sAlgInitParam.cTaskPath,m_strTaskName,strDstComCam,m_strAlgName,nDstRegionID);
		}		

		memcpy(strDst, strTmp, strTmp.GetLength());
		fs.pTo=strDst;		
		m_FileOperate.CreateMultiLevelPath(strDst);	
		SHFileOperation(&fs);

		//修改拷贝区域的走版控制参数
		CString strParaIniPath;
		strParaIniPath.Format("%s\\2DCodeCheckPara.ini",strDst);
		WritePrivateProfileString("条码走版", "左右白框检测是否修改", "1", strParaIniPath);
		WritePrivateProfileString("条码走版", "上下白框检测是否修改", "1", strParaIniPath);
		WritePrivateProfileString("条码走版", "左边界位置", "0", strParaIniPath);
		WritePrivateProfileString("条码走版", "右边界位置", "0", strParaIniPath);
		WritePrivateProfileString("条码走版", "上边界位置", "0", strParaIniPath);
		WritePrivateProfileString("条码走版", "下边界位置", "0", strParaIniPath);
	}
	else//需要调用内部copyRegion函数
	{
		CInnerCheck InnerCheckTmp;
		InnerCheckTmp.AlgCopyRegion(sAlgImageInfo,sDstRegionProperty,sSrcRegionProperty);
	}
	
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
s_Status CSetPROP::AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty &sSrcRegionProperty,s_SysAuthority sSysAuthority)
{

	//部分算法修改区域大小后，需要做相应操作，以前通用算法不用
	//修改拷贝区域的走版控制参数
	CString strParaIniPath,strDstReg,strSrcDef;
	CString str;
	str = m_sAlgInitParam.cTaskPath;
	if (str.GetAt(str.GetLength()-1)=='\\')
	{
		strParaIniPath.Format("%s%s\\ALGTask\\Com%dCam%d\\%s\\%d\\2DCodeCheckPara.ini",
			m_sAlgInitParam.cTaskPath,
			m_strTaskName,
			sSrcRegionProperty.nComputerID,
			sSrcRegionProperty.nCameraID,
			m_strAlgName,
			sSrcRegionProperty.nRegionID);
		strDstReg.Format("%s%s\\ALGTask\\Com%dCam%d\\%s\\%d",
			m_sAlgInitParam.cTaskPath,
			m_strTaskName,
			sSrcRegionProperty.nComputerID,
			sSrcRegionProperty.nCameraID,
			m_strAlgName,
			sSrcRegionProperty.nRegionID);
	}
	else
	{
		strParaIniPath.Format("%s\\%s\\ALGTask\\Com%dCam%d\\%s\\%d\\2DCodeCheckPara.ini",
			m_sAlgInitParam.cTaskPath,
			m_strTaskName,
			sSrcRegionProperty.nComputerID,
			sSrcRegionProperty.nCameraID,
			m_strAlgName,
			sSrcRegionProperty.nRegionID);
		strDstReg.Format("%s\\%s\\ALGTask\\Com%dCam%d\\%s\\%d",
			m_sAlgInitParam.cTaskPath,
			m_strTaskName,
			sSrcRegionProperty.nComputerID,
			sSrcRegionProperty.nCameraID,
			m_strAlgName,
			sSrcRegionProperty.nRegionID);
	}

	if (!m_FileOperate.IsFileExist(strParaIniPath))
	{
		SHFILEOPSTRUCT   fs;   
		char strSrc[DH_MAX_PATH] = {0};
		char strDst[DH_MAX_PATH] = {0};
		ZeroMemory(strSrc, MAX_PATH);
		ZeroMemory(strDst, MAX_PATH);
		
		ZeroMemory(&fs,sizeof(SHFILEOPSTRUCT));   
		fs.hwnd   =   NULL;   
		fs.wFunc   =   FO_COPY;   
		fs.fFlags   =   FOF_ALLOWUNDO   |   FOF_NOCONFIRMATION   |   FOF_NOERRORUI  |FOF_MULTIDESTFILES  ;   
		fs.lpszProgressTitle   =   "copyfile";   
		
		str.Format("%s",m_sAlgInitParam.cAlgPath);
		if (str.GetAt(str.GetLength()-1)=='\\')
		{
			strSrcDef.Format("%sDefaultParameter\\%s",m_sAlgInitParam.cAlgPath,m_strAlgName);
		}
		else
		{
			strSrcDef.Format("%s\\DefaultParameter\\%s",m_sAlgInitParam.cAlgPath,m_strAlgName);
		}
		
		memcpy(strSrc, strSrcDef, strSrcDef.GetLength());
		fs.pFrom   =  strSrc;
				
		memcpy(strDst, strDstReg, strDstReg.GetLength());
		fs.pTo=strDst;		
		m_FileOperate.CreateMultiLevelPath(strDst);	
		SHFileOperation(&fs);
	}
	WritePrivateProfileString("条码走版", "左右白框检测是否修改", "1", strParaIniPath);
	WritePrivateProfileString("条码走版", "上下白框检测是否修改", "1", strParaIniPath);
	WritePrivateProfileString("条码走版", "左边界位置", "0", strParaIniPath);
	WritePrivateProfileString("条码走版", "右边界位置", "0", strParaIniPath);
	WritePrivateProfileString("条码走版", "上边界位置", "0", strParaIniPath);
	WritePrivateProfileString("条码走版", "下边界位置", "0", strParaIniPath);
	return m_sStatus;

}

s_Status CSetPROP::AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, 
								const s_RegionProperty &sNewRegionProperty,const s_RegionProperty &sOrgRegionProperty,s_SysAuthority sSysAuthority)
{
	s_Status sTatus;
	int nSrcRegionID = sOrgRegionProperty.nRegionID;
	int nDstRegionID = sNewRegionProperty.nRegionID;

	CString strSrcComCam;
	CString strDstComCam;
	strSrcComCam.Format("Com%dCam%d",sOrgRegionProperty.nComputerID,sOrgRegionProperty.nCameraID);
	strDstComCam.Format("Com%dCam%d",sNewRegionProperty.nComputerID,sNewRegionProperty.nCameraID);

	if (1)// 内部算法只涉及复制文件内容的 [5/17/2011 Denghl]
	{
		SHFILEOPSTRUCT   fs;   
		char strSrc[DH_MAX_PATH] = {0};
		char strDst[DH_MAX_PATH] = {0};
		ZeroMemory(strSrc, MAX_PATH);
		ZeroMemory(strDst, MAX_PATH);
		CString strTmp;
		ZeroMemory(&fs,sizeof(SHFILEOPSTRUCT));   
		fs.hwnd   =   NULL;   
		fs.wFunc   =   FO_COPY;   
		fs.fFlags   =   FOF_ALLOWUNDO   |   FOF_NOCONFIRMATION   |   FOF_NOERRORUI  |FOF_MULTIDESTFILES  ;   
		fs.lpszProgressTitle   =   "copyfile";   
		CString str;
		str.Format("%s",m_sAlgInitParam.cTaskPath);

		if (str.GetAt(str.GetLength()-1)=='\\')
		{
			strTmp.Format("%s%s\\ALGTask\\%s\\%s\\%d",m_sAlgInitParam.cTaskPath,m_strTaskName,strSrcComCam,m_strAlgName,nSrcRegionID);
		}
		else
		{
			strTmp.Format("%s\\%s\\ALGTask\\%s\\%s\\%d",m_sAlgInitParam.cTaskPath,m_strTaskName,strSrcComCam,m_strAlgName,nSrcRegionID);
		}
		CString strIni;
		strIni.Format("%s\\2DCodeCheckPara.ini",strTmp);
		if(!m_FileOperate.IsFileExist(strIni))
		{
			CString str1;
			str1.Format("%s",m_sAlgInitParam.cAlgPath);
			if (str1.GetAt(str1.GetLength()-1)=='\\')
			{
				strTmp.Format("%sDefaultParameter\\%s",m_sAlgInitParam.cAlgPath,m_strAlgName);
			}
			else
			{
				strTmp.Format("%s\\DefaultParameter\\%s",m_sAlgInitParam.cAlgPath,m_strAlgName);
			}
		}
		memcpy(strSrc, strTmp, strTmp.GetLength());
		fs.pFrom   =  strSrc;
		strTmp.Empty();
		if (str.GetAt(str.GetLength()-1)=='\\')
		{
			strTmp.Format("%s%s\\ALGTask\\%s\\%s\\%d",m_sAlgInitParam.cTaskPath,m_strTaskName,strDstComCam,m_strAlgName,nDstRegionID);
		}
		else
		{
			strTmp.Format("%s\\%s\\ALGTask\\%s\\%s\\%d",m_sAlgInitParam.cTaskPath,m_strTaskName,strDstComCam,m_strAlgName,nDstRegionID);
		}		

		memcpy(strDst, strTmp, strTmp.GetLength());
		fs.pTo=strDst;		
		m_FileOperate.CreateMultiLevelPath(strDst);	
		SHFileOperation(&fs);
		
		//修改拷贝区域的走版控制参数
		CString strParaIniPath;
		strParaIniPath.Format("%s\\2DCodeCheckPara.ini",strDst);
		WritePrivateProfileString("条码走版", "左右白框检测是否修改", "1", strParaIniPath);
		WritePrivateProfileString("条码走版", "上下白框检测是否修改", "1", strParaIniPath);
		WritePrivateProfileString("条码走版", "左边界位置", "0", strParaIniPath);
		WritePrivateProfileString("条码走版", "右边界位置", "0", strParaIniPath);
		WritePrivateProfileString("条码走版", "上边界位置", "0", strParaIniPath);
		WritePrivateProfileString("条码走版", "下边界位置", "0", strParaIniPath);
	}
	return sTatus;
}

//s_Status CSetPROP::AlgUpdataRegion(const s_AlgImageInfo& sAlgImageInfo,
//								   const list<s_RegionProperty> &lstUpdataRegion)
//{
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
s_Status CSetPROP::AlgDeleteRegion(const list<s_RegionProperty> &lstRegionProperty,s_SysAuthority sSysAuthority)
{
 	std::list<s_RegionProperty>::const_iterator itRegionProperty;
	if (lstRegionProperty.size()<=0)
	{
		return m_sStatus;
	}
	for (itRegionProperty=lstRegionProperty.begin();itRegionProperty!=lstRegionProperty.end();itRegionProperty++)
	{
		//先获取对应ID号的前端号和相机号
		CString strComCam;
		strComCam.Format("Com%dCam%d",itRegionProperty->nComputerID,itRegionProperty->nCameraID);


		SHFILEOPSTRUCT   fs;   
		char strDelete[DH_MAX_PATH]={0};
		CString strTmp;
		ZeroMemory(&fs,sizeof(SHFILEOPSTRUCT));   
		fs.hwnd   =   NULL;   
		fs.wFunc   =   FO_DELETE;   
		//	fs.pFrom   =   "c:\\temp";   
		fs.pTo   =   NULL;   
		fs.fFlags   =   FOF_ALLOWUNDO   |   FOF_NOCONFIRMATION   |   FOF_NOERRORUI   ;   
		fs.lpszProgressTitle   =   "delefile";   
		strTmp.Format("%s\\%s\\ALGTask\\%s\\%s\\%d",m_sAlgInitParam.cTaskPath,m_strTaskName,strComCam,m_strAlgName,itRegionProperty->nRegionID);
		int nLength=strTmp.GetLength();
		strcpy(strDelete,strTmp);
		fs.pFrom   =  strDelete;
		SHFileOperation(&fs);
	}
	
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
s_Status CSetPROP::GetAlgorithmInfo(list<s_InnerAlgType>  &lstInnerAlgType)
{
	lstInnerAlgType.clear();

	s_InnerAlgType sAlgType;
	sAlgType.nInnerTypeID = 1;
	sAlgType.bPopUpDlg = FALSE;
	sAlgType.bSupportLearn = FALSE;
	if (m_pChecker->m_nLanguage==1)
	{
		sprintf_s(sAlgType.cTypeName, "2D Code Check");
	} 
	else
	{
		sprintf_s(sAlgType.cTypeName, "二维码识别");
	}
	
	lstInnerAlgType.push_back(sAlgType);

	//sAlgType.nInnerTypeID = 1;
	//sAlgType.bPopUpDlg = FALSE;
	//sAlgType.bSupportLearn = FALSE;
	//sprintf_s(sAlgType.cTypeName, "QR码识别");
	//lstInnerAlgType.push_back(sAlgType);

	//sAlgType.nInnerTypeID = 2;
	//sAlgType.bPopUpDlg = FALSE;
	//sAlgType.bSupportLearn = FALSE;
	//sprintf_s(sAlgType.cTypeName, "DM码识别");
	//lstInnerAlgType.push_back(sAlgType);

	//sAlgType.nInnerTypeID = 3;
	//sAlgType.bPopUpDlg = FALSE;
	//sAlgType.bSupportLearn = FALSE;
	//sprintf_s(sAlgType.cTypeName, "PDF417码识别");
	//lstInnerAlgType.push_back(sAlgType);

	

	return m_sStatus;
}

/////////////////////////////////////////////////////////////////////////////////////
// 参数
CSetPARA::CSetPARA()
{

}
CSetPARA::~CSetPARA()
{

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
s_Status CSetPARA::Init(const char *strTaskName,const s_AlgInitParam &sAlgInitParam)
{

	m_pChecker = (CCheck*)((CDHAlgLab2DCodeCheck*)pChecker)->m_pAlgLab2DCodeCheck;
	m_sAlgInitParam = sAlgInitParam;
	m_pChecker->m_sAlgInitParam = m_sAlgInitParam;
	m_strTaskName = strTaskName;
	m_pChecker->m_strTaskName = strTaskName;
	m_strAlgName.Format("AlgLab2DCodeCheck");
//	m_pChecker->m_bIsAgainCheck = bIsAgainCheck;
	
	return m_sStatus;   
}

//******************************************************************************//
//函数:参数对话框调用接口实现
//参数:
//	sAlgImageInfo          输入，图像结构体
//	sSigCellCheckResult    输入，检测结果信息含中间结果
//	sRegionProperty        输入，区域属性
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CSetPARA::SetCheckparamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_SigleCell_CheckResult &sSigCellCheckResult, 
												 const S_SingleCell_Or_RegionsInfo &sSingleCellRegionsInfo,
												 s_SysAuthority sSysAuthority,
												 const BOOL bOpenDlg,const std::list<UINT> lstnErrorID)
{
	s_Status sStatus;

	CParaDlg Paradlg;
	Paradlg.m_pChecker = m_pChecker;
	Paradlg.m_sSysAuthority = sSysAuthority;
	//  修改[10/10/2015 baiyy]
	//Paradlg.m_sAlgImageInfo = sAlgImageInfo;
 //   PBYTE   pImgMode, pImgModeR, pImgModeG, pImgModeB;
	//pImgMode = NULL;
	//pImgModeR = NULL;
	//pImgModeG = NULL;
	//pImgModeB = NULL;
	//if (sAlgImageInfo.nImageType != 0)
	//{
	//	HObject hReg;
	//	HTuple row1,col1,row2,col2;
	//	col1 = sSigCellCheckResult.vsErrorInfo[0].RectErrorReg.left-sSigCellCheckResult.rCircumRect.left;
	//	row1 = sSigCellCheckResult.vsErrorInfo[0].RectErrorReg.top - sSigCellCheckResult.rCircumRect.top;
	//	col2 = sSigCellCheckResult.vsErrorInfo[0].RectErrorReg.right
	//		-sSigCellCheckResult.vsErrorInfo[0].RectErrorReg.left+1+col1;
	//	row2 = sSigCellCheckResult.vsErrorInfo[0].RectErrorReg.bottom
	//		-sSigCellCheckResult.vsErrorInfo[0].RectErrorReg.top+1+row1;
	//	GenRectangle1(&hReg, row1, col1, row2, col2);
	//

	//	HTuple lWid,lHei;
	//	HTuple lModelWid, lModelHei;
	//	lWid = (sAlgImageInfo.sizeImage.cx+3)/4*4;
	//	lHei = sAlgImageInfo.sizeImage.cy;
	//	if (sAlgImageInfo.nFlag == 0)
	//	{	
	//		HObject hImg, hRoiImg;
	//		GenImage1Extern(&hImg, "byte", lWid, lHei, (HTuple)sAlgImageInfo.pImageData, NULL);
	//	    ReduceDomain(hImg, hReg, &hRoiImg);
	//		CropDomain(hRoiImg, &hRoiImg);
	//		unsigned char *ptr;
	//		char     type[128];
	//		GetImagePointer1(hRoiImg, (HTuple*)&ptr, type, &lModelWid, &lModelHei);
	//		pImgMode = new BYTE[WIDTHBYTES(lModelWid*8)*lModelHei];
	//		for (long i =0; i<lModelHei;i++)
	//		{
	//			memcpy(pImgMode+i*WIDTHBYTES(lModelWid*8), ptr+i*lModelWid, lModelWid);
	//		}
	//		Paradlg.m_sAlgImageInfo.pImageData = pImgMode;
	//		Paradlg.m_sAlgImageInfo.sizeImage.cx = WIDTHBYTES(lModelWid*8);
	//		Paradlg.m_sAlgImageInfo.sizeImage.cy = lModelHei;
	//		Paradlg.m_sAlgImageInfo.nBits = sAlgImageInfo.nBits;
	//	}
	//	else
	//	{
	//		if (sAlgImageInfo.nFlag == 1)
	//		{
	//			HObject hImgR, hImgG, hImgB,hRoiImg;
	//			HObject hImg;
	//			GenImage1Extern(&hImgR, "byte", lWid, lHei, (HTuple)sAlgImageInfo.pImageR ,NULL);
	//			GenImage1Extern(&hImgG, "byte", lWid, lHei, (HTuple)sAlgImageInfo.pImageG, NULL);
	//			GenImage1Extern(&hImgB, "byte", lWid, lHei, (HTuple)sAlgImageInfo.pImageB, NULL);
	//			Compose3(hImgR, hImgG, hImgB, &hImg);

	//			ReduceDomain(hImg, hReg, &hRoiImg);
	//			CropDomain(hRoiImg, &hRoiImg);

	//			PBYTE ptrR, ptrG, ptrB;
	//			char     type[128];
	//			GetImagePointer3(hRoiImg, (HTuple*)&ptrR, (HTuple*)&ptrG,(HTuple*)&ptrB,
	//				type, &lModelWid, &lModelHei);
	//			pImgModeR = new BYTE[WIDTHBYTES(lModelWid*8)*lModelHei];
	//			pImgModeG = new BYTE[WIDTHBYTES(lModelWid*8)*lModelHei];
	//			pImgModeB = new BYTE[WIDTHBYTES(lModelWid*8)*lModelHei];

	//			for (long i =0; i<lModelHei;i++)
	//			{
	//				memcpy(pImgModeR+i*WIDTHBYTES(lModelWid*8), ptrR+i*lModelWid, lModelWid);
	//				memcpy(pImgModeG+i*WIDTHBYTES(lModelWid*8), ptrG+i*lModelWid, lModelWid);
	//				memcpy(pImgModeB+i*WIDTHBYTES(lModelWid*8), ptrB+i*lModelWid, lModelWid);
	//			}
	//			Paradlg.m_sAlgImageInfo.pImageR = pImgModeR;
	//			Paradlg.m_sAlgImageInfo.pImageG = pImgModeG;
	//			Paradlg.m_sAlgImageInfo.pImageB = pImgModeB;
	//			Paradlg.m_sAlgImageInfo.sizeImage.cx = WIDTHBYTES(lModelWid*8);
	//			Paradlg.m_sAlgImageInfo.sizeImage.cy = lModelHei;
	//			Paradlg.m_sAlgImageInfo.nBits=sAlgImageInfo.nBits;
	//		}

	//	}
	//}
	//else
	//{
	//	Paradlg.m_sAlgImageInfo = sAlgImageInfo;

	//}
	//  [10/10/2015 baiyy]
	Paradlg.m_sAlgImageInfo = sAlgImageInfo;
	//  [12/2/2014 baiyy]
	Paradlg.m_sSingleCellRegionsInfo = sSingleCellRegionsInfo;
	Paradlg.m_sSingleCellCheckResult = sSigCellCheckResult;
	//  [4/6/2017 zb]
	m_pChecker->m_sAlgInitParam.nLanguage = m_pChecker->m_nLanguage;

	if (sSysAuthority.nSysAuthority==1||sSysAuthority.nSysAuthority==2)
	{
		Paradlg.m_bAuthority = TRUE;
	}
	else
	{
		Paradlg.m_bAuthority = FALSE;
	}
	/*if (sSingleCellRegionsInfo.sSingleCellPro.nResvered[0]==1)
	{
		Paradlg.m_bAuthority = FALSE;
	}
	else if (sSingleCellRegionsInfo.sSingleCellPro.nResvered[0]==0)
	{
		Paradlg.m_bAuthority = TRUE;
	}
	else
	{
		Paradlg.m_bAuthority = TRUE;
	}*/
	std::list<s_AlgRegions>::const_iterator itLstSrc;
	std::list<s_AlgRegions>::iterator itLstDst;
	itLstDst = Paradlg.m_sSingleCellRegionsInfo.lstAlgRegions.begin();
	itLstSrc = sSingleCellRegionsInfo.lstAlgRegions.begin();
	std::list<s_RegionProperty>::const_iterator itRegSrc;
	itLstDst->lstSRegPro.clear();
	for (itRegSrc = itLstSrc->lstSRegPro.begin();itRegSrc!=itLstSrc->lstSRegPro.end();itRegSrc++)
	{
		if (sSigCellCheckResult.vsErrorInfo.size()>0)
		{
			if (itRegSrc->nRegionID==sSigCellCheckResult.vsErrorInfo.at(0).nRegionID)
			{
				itLstDst->lstSRegPro.push_back(*itRegSrc);
				break;
			}
		}
		else
		{
			itLstDst->lstSRegPro.push_back(*itRegSrc);
			break;
		}		
	}

	// 弹对话框
	if (Paradlg.DoModal()==IDOK)
	{
		;
	}
	if (Paradlg.m_bIsModify)
	{
		sprintf_s(sStatus.chErrorInfo,"ZT=1");
	}
	else
	{
		sprintf_s(sStatus.chErrorInfo,"ZT=0");
	}

	//if (pImgMode)
	//{
	//	delete[] pImgMode;
	//	pImgMode = NULL;
	//}
	//if (pImgModeR)
	//{
	//	delete[]pImgModeR;
	//	pImgModeR = NULL;
	//}
	//if (pImgModeG)
	//{
	//	delete[]pImgModeG;
	//	pImgModeG = NULL;
	//}
	//if (pImgModeB)
	//{
	//	delete[]pImgModeB;
	//	pImgModeB = NULL;
	//}

	return sStatus;
}

//s_Status  CSetPARA::SetCheckparamDlg(const s_AlgImageInfo& sAlgImageInfo, const s_SigleCell_CheckResult &sSigCellCheckResult, 
//									 const s_MiddleResult &sMiddleResult,
//									 const S_SingleCell_Or_RegionsInfo &sSingleCellRegionsInfo)
//{
//	s_Status sStatus;
//	return sStatus;
//}


//******************************************************************************//
//函数:学习设置接口函数实现
//参数:
//  strTaskPath           输入，任务路径
//	listRegionProperty    输入，区域链表
//返回:
//	算法错误类型结构体
//******************************************************************************//
s_Status CSetPARA::SetTrainParam(const list<s_RegionProperty>& listRegionProperty,s_SysAuthority sSysAuthority)
{
	//部分算法只存在添加和丢弃学习，没有其他学习模式需要设置的，或者不支持学习的，该内容不用添加
	return m_sStatus;
}