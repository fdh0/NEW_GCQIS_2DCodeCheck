// Check.cpp: implementation of the CCheck class.
//
//////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Copyright (c) 2008, ����ͼ���Ӿ��������޹�˾�Ӿ��㷨��     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� Check.cpp												
/* ժҪ�� �㷨����ຯ�����Լ������������ԶԻ�����ӿں���
/*
/* ���ߣ� �˺���
/* ������ڣ� 2011��5��10��
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

// ��ʼ�����Ա����,������Ҫ��Ӵ���
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
//					//�ֽ����־���ͺ���־����
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
//		str.Format("�߳�д��־�쳣");
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
		//m_lstStr->clear();   //����
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
//����:������ʼ��
//����:
//	sAlgInitParam    ���룬ȫ��·��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::Init(const s_AlgInitParam &sAlgInitParam,s_ALGCheckParam* psCheckParams,s_SysAuthority sSysAuthority)
{

	

	memcpy(&m_sAlgInitParam,&sAlgInitParam,sizeof(s_AlgInitParam));
	// ��ȡ�㷨�������ñ�� [11/8/2011 Denghl]
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
	//GetPrivateProfileString("������", "�Ƿ�ʹ�ñ���У���","1", strTemp, sizeof(strTemp), m_strDefaultParaIni);
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
	//			AfxMessageBox("checkcode.dll����ʧ��");
	//			m_hDll = NULL;
	//		}
	//	}

	//}



	//  [4/6/2017 zb]
	// ��Ӣ��·�����ö�ȡ
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
		sprintf_s(sTypeParam.cInnerTypeName,"��ά������");
	}
	s_InnerTypeParam sInnerTypeParam;
	if (m_nLanguage==1)
	{
		sprintf_s(sInnerTypeParam.cParamName,"Check Code Lose");
	}
	else
	{
		sprintf_s(sInnerTypeParam.cParamName,"ȱӡ���");
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
		sprintf_s(sInnerTypeParam.cParamName,"īɫ����");
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
		sprintf_s(sInnerTypeParam.cParamName,"���߷�λ��");
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
		sprintf_s(sInnerTypeParam.cParamName,"�߰���");
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
		sprintf_s(sInnerTypeParam.cParamName,"�߰湫��");
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
		sprintf_s(sInnerTypeParam.cParamName,"�����");
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
		sprintf_s(sInnerTypeParam.cParamName,"������");
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
		nTextNum = GetPrivateProfileInt("�����ı�", "�ı�����", 0, m_strMessagePath);
		char chMessage[256];
		CString strIndex;
		for(int i=0; i<nTextNum; i++)
		{ 
			memset(chMessage,0,256);
			strIndex.Format("%d",i);
			GetPrivateProfileString("��ϸ����",strIndex,"",chMessage,255,m_strMessagePath);
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
	//0=Check���:��ʼ��
	strContent.Format("%s",m_strMessageText[0]);
	GenCurLog(strContent,2,0);
	WirteCurLog();
	//m_FileOperate.WriteAlgOperateLog(m_strLogPath, "������־", "������־", 
		//"���:��ʼ��");
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
			
			GetPrivateProfileString("������������", "�Ƿ���","0", 
				strTemp, sizeof(strTemp),
				itInnerCheck->m_strParaIni);
			m_sSimplePara.bDirtCheck = atoi(strTemp);
			GetPrivateProfileString("������������", "���������","3",
				strTemp, sizeof(strTemp),
				itInnerCheck->m_strParaIni);
			m_sSimplePara.dSingleArea = atoi(strTemp)*m_sAlgInitParam.fResolution_H*m_sAlgInitParam.fResolution_V;
			GetPrivateProfileString("�����ڰ���ȱӡ", "�Ƿ���","0", 
				strTemp, sizeof(strTemp), itInnerCheck->m_strParaIni);
			m_sSimplePara.bSkipCheck = atoi(strTemp);
			GetPrivateProfileString("����������ȱӡ", "�Ҷȷ���","5", 
				strTemp, sizeof(strTemp), itInnerCheck->m_strParaIni);
			m_sSimplePara.nDeviation = atoi(strTemp);
			GetPrivateProfileString("�����ڰ���ȱӡ", "�ٷ�λ","70", 
				strTemp, sizeof(strTemp), itInnerCheck->m_strParaIni);
			m_sSimplePara.nPercent = atoi(strTemp);
			GetPrivateProfileString("�����߰�", "�Ƿ���","0",
				strTemp, sizeof(strTemp), itInnerCheck->m_strParaIni);
			m_sSimplePara.bZoubanCheck = atoi(strTemp);
			GetPrivateProfileString("�����߰�", "�߽繫��","0.5", 
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

//��ȡ�㷨����
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
		sprintf_s(sTypeParam.cInnerTypeName,"��ά������");
	}
	
	s_InnerTypeParam sInnerTypeParam;
	if (m_nLanguage==1)
	{
		sprintf_s(sInnerTypeParam.cParamName,"Check Code Lose");
	}
	else
	{
		sprintf_s(sInnerTypeParam.cParamName,"ȱӡ���");
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
			sprintf_s(sInnerTypeParam.cParamName,"īɫ����");
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
			sprintf_s(sInnerTypeParam.cParamName,"���߷�λ��");
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
		sprintf_s(sInnerTypeParam.cParamName,"�߰���");
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
			sprintf_s(sInnerTypeParam.cParamName,"�߰湫��");
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
		sprintf_s(sInnerTypeParam.cParamName,"�����");
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
			sprintf_s(sInnerTypeParam.cParamName,"������");
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

//�����㷨����
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
					if (strName=="ȱӡ���")
					{
						m_sSimplePara.bSkipCheck = itInnerPara->dCurValue;
					}
					else if (strName=="īɫ����")
					{
						m_sSimplePara.nDeviation = itInnerPara->dCurValue;
					}
					else if (strName=="���߷�λ��")
					{
						m_sSimplePara.nPercent = itInnerPara->dCurValue;
					}
					else if (strName=="�߰���")
					{
						m_sSimplePara.bZoubanCheck = itInnerPara->dCurValue;
					}
					else if (strName=="�߰湫��")
					{
						m_sSimplePara.dZouBanRange = itInnerPara->dCurValue;
					}
					else if (strName=="�����")
					{
						m_sSimplePara.bDirtCheck = itInnerPara->dCurValue;
					}
					else if (strName=="������")
					{
						m_sSimplePara.dSingleArea = itInnerPara->dCurValue;
					}
				}
			}	
			
			//дӲ�̺͸����ļ�
			WriteSimplePara();
		}		
	}
	else
	{
		//����Ƿ�
		;
	}
	return sStatus;

}

//******************************************************************************//
//����:������������
//����:
//	strTaskName    ���룬������
//  sDHTemplate    ���룬�������м����������
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::LoadTask(const char *strTaskName, const s_DH_Template  &sDHTemplate,
	s_SysAuthority sSysAuthority,const BOOL bLoadFlag)
{
	s_Status sStatus;
	try
	{
		//����·��
		m_strTaskName = strTaskName;
		m_strTaskPath.Format("%s\\%s",m_sAlgInitParam.cTaskPath,m_strTaskName);
		m_sDHTemplate = sDHTemplate;
		//m_bIsAgainCheck = sDHTemplate.bIsSecVerify;

		//���ɼ������
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
		//			AfxMessageBox("checkcode.dll����ʧ��");
		//			m_hDll = NULL;
		//			m_sStatus.nErrorType = 1;
		//			return m_sStatus;
		//		}
		//	}
		//}

		// ��ȡ��ʱ���� [9/9/2015 dhl]
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
		//	// ���κ˲�·�� [3/28/2012 Denghl]
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
			GetPrivateProfileString("��ʱ����", "�Ƿ���Ƴ�ʱ","0", strTemp, sizeof(strTemp), m_strPathini);
			m_bCheckTimeOut = atoi(strTemp);
			GetPrivateProfileString("��ʱ����", "���ż��ʱ��","600", strTemp, sizeof(strTemp), m_strPathini);
			m_nAllTime = atoi(strTemp);
			// ��Ӽ����־���� [9/29/2015 TIAN]
			GetPrivateProfileString("д��־����", "�Ƿ�д�����־","1", strTemp, sizeof(strTemp), m_strPathini);
			m_bWriteLog = atoi(strTemp);
		}
		else
		{
			char strTemp[100];
			memset(strTemp,0,100);
			GetPrivateProfileString("��ʱ����", "�Ƿ���Ƴ�ʱ","0", strTemp, sizeof(strTemp), m_strDefaultParaIni);
			m_bCheckTimeOut = atoi(strTemp);
			GetPrivateProfileString("��ʱ����", "���ż��ʱ��","600", strTemp, sizeof(strTemp), m_strDefaultParaIni);
			m_nAllTime = atoi(strTemp);
			// ��Ӽ����־���� [9/29/2015 TIAN]
			GetPrivateProfileString("д��־����", "�Ƿ�д�����־","1", strTemp, sizeof(strTemp), m_strDefaultParaIni);
			m_bWriteLog = atoi(strTemp);
		}


		int nCellNum = m_sDHTemplate.lstSCellRegion.size();
		m_sCheckResult.lst_vec_clear();
		m_nRegionNum = 0;	
		std::list<S_SingleCell_Or_RegionsInfo>::const_iterator itSingleCell;
		int nCellCount =0;
		CString strTemp;

		//���·���С����־��¼list����
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
		//��С����������
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
			/*str1.Format("%d����\n",nCellCount);
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
				// ����־�򱨾� [3/28/2012 Denghl]
				;
			}
			for (itRegionPro = (itAlglstRegions->lstSRegPro).begin(); itRegionPro != itAlglstRegions->lstSRegPro.end(); itRegionPro++)
			{
				if (itRegionPro->nAlgorithmType==ALG_TYPE_ID)//ALGNAMECHECK)// ע�⽫�㷨���ͱ�ż��� [5/16/2011 Denghl]
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
					sSigCellResult.vsErrorInfo.at(0).bytErrorType = 199;// ��ʼ������ [12/5/2011 Denghl]
					sSigCellResult.vsErrorInfo.at(0).nLocateGroupID = TmpInnerChecker->m_sDstRegionProperty.nLocateGroupID;
					sSigCellResult.vsErrorInfo.at(0).nCol = sRegionPro.nColumnIndex;
					sSigCellResult.vsErrorInfo.at(0).nRow = sRegionPro.nRowIndex;
					sSigCellResult.vsErrorInfo.at(0).bIsPrimary = sRegionPro.bIsPrimaryCode;
					sSigCellResult.vsErrorInfo.at(0).bIsVerifyCode = sRegionPro.bIsVerifyCode;
					//
					CString strInitInfo=_T("");
					//1=��ʼ������
					strInitInfo.Format("%s",m_strMessageText[1]);
					sprintf_s(sSigCellResult.vsErrorInfo.at(0).cErrorDescription,strInitInfo);
					
					sSigCellResult.rCircumRect = TmpInnerChecker->m_rcOrgMinRect;
					m_sCheckResult.lstSigCCR.push_back(sSigCellResult);
					m_sCheckResult.iErrorLstSigCCR++;
					delete TmpInnerChecker;
					TmpInnerChecker = NULL;
					// str1.Format("��������\n");
					// strTemp += str1;
				}
			}
			nCellIndex++;
			//strTemp+="����С��";
			m_lstSingleCellInnerCheck.push_back(sInnerCheckInfo);
		}
		//WirteCurLog();
		//m_FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",strTemp);
		
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
	//д��־�ļ�
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
//����:����ͷſռ�
//����:
//����:
//	�㷨ִ��״̬�ṹ��
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

	//д��־�ļ�
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
	//			//�ֽ����־���ͺ���־����
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
	// ״̬�ṹ���ʼ�� [3/22/2017 zb]
	memset(&m_sStatus.chErrorInfo, 0, DH_MAX_TEXTSIZE*sizeof(char));
	memset(&m_sStatus.chErrorContext, 0, DH_MAX_TEXTSIZE*sizeof(char));
	m_sStatus.nErrorType = 0;
	return m_sStatus;
}

//******************************************************************************//
//����:�����м���
//����:s_MiddleResult �м����ṹ��
//����:
//	�����м���
//******************************************************************************//
s_Status CCheck::SetMiddleResult(const s_MiddleResult &sMiddleResult)
{
	m_sMiddleResult.vsLocGroupResult = sMiddleResult.vsLocGroupResult;
	return m_sStatus;

}

//��λ�㷨���ü���麯������
s_Status CCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage,s_MiddleResult *sMiddleResult,BOOL &bIsProduct)
{
	return m_sStatus;
}

////******************************************************************************//
////����:���������
////����:
////	sAlgAgainCheckInputParam      ���룬�������ṹ��
////	sAlgCheckOutputResult    ��������ؼ��������ṹ��
////����:
////	�㷨ִ��״̬�ṹ��
////******************************************************************************//
//s_Status CCheck::Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
//					   s_AlgCheckOutputParam *sAlgCheckOutputResult)
//{
//	return m_sStatus;
//}
//
////******************************************************************************//
////����:���������
////����:
////	sAlgAgainCheckInputParam      ���룬�������ṹ��
////	sAlgCheckOutputResult    ��������ؼ��������ṹ��
////����:
////	�㷨ִ��״̬�ṹ��
////******************************************************************************//
//s_Status CCheck::Check(const s_AlgAgainCheckInputParam& sAlgAgainCheckInputParam,
//					   s_MiddleResult *sMiddleResult)
//{
//	// ��Ҫ��д [3/26/2012 Denghl]
//	return m_sStatus;
//
//}

//******************************************************************************//
//����:���������
//����:
//	sAlgCheckInputParam      ���룬�������ṹ��
//	sAlgCheckOutputResult    ��������ؼ��������ṹ��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam,const HObject hSrcImage, s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	try
	{
		s_Status sTatusRegion;
		SetSystem("parallelize_operators", "false");// halcon���ز��� [9/17/2015 dhl]
		sPendTime.End();
	    //	CSpendTime sPendCheck;
	    //	sPendCheck.Start();
		//��ǰ���㷨��ѧϰ��Ϣ��
		

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
		m_nStudyRegNum = 0;//ÿ���ſ�ʼ���ѧϰ�ɹ�������ʼ��Ϊ0

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
				//2=ͼ����������Ϊ��
				strContent.Format("%s",m_strMessageText[2]) ;
				sprintf_s(m_sStatus.chErrorContext,"%s",strContent);
				if (m_bWriteLog)
				{
					CString strContent;
					strContent.Format("%s",m_sStatus.chErrorContext);
					GenCurLog(strContent,3,0);
					//m_FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",m_sStatus.chErrorContext);
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
				//2=ͼ����������Ϊ��
				strContent.Format("%s",m_strMessageText[2]) ;
				sprintf_s(m_sStatus.chErrorContext,"%s",strContent);
				if (m_bWriteLog)
				{
					CString strContent;
					strContent.Format("%s",m_sStatus.chErrorContext);
					GenCurLog(strContent,3,0);
					//m_FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",m_sStatus.chErrorContext);
				}
				WirteCurLog();
				return m_sStatus;

			}
		}
		else
		{
			m_sStatus.nErrorType = 1;
			CString strContent=_T("");
			//3=ͼ����������λ������
			strContent.Format("%s",m_strMessageText[3]) ;
			sprintf_s(m_sStatus.chErrorContext,"%s",strContent);
			if (m_bWriteLog)
			{
				CString strContent;
				strContent.Format("%s",m_sStatus.chErrorContext);
				GenCurLog(strContent,3,0);
				//m_FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",m_sStatus.chErrorContext);
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
//			strTmp.Format("��������ʱ����������ȡ�쳣,lst=%d,rest=%d",
//				m_lstSingleCellInnerCheck.size(),m_sCheckResult.lstSigCCR.size());
//			//sprintf_s(m_sStatus.chErrorContext,"%s","��������ʱ����������ȡ�쳣");
//			m_FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",strTmp);
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
		//���״̬����
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


			if (nCountCell>0)//��ȡ�仯������Ϣ;
			{
				
				for (itSingleCell = (sAlgCheckInputParam.sDHTemplate.lstSCellRegion).begin(); 
					itSingleCell != (sAlgCheckInputParam.sDHTemplate.lstSCellRegion).end(); itSingleCell++)
				{
					if (itSingleCell->sSingleCellPro.nCellID==itlstinnerCheck->sSingleCellPro.nCellID)// ���Ȱ������� [11/15/2011 Denghl]
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
									if (itRegionPro->nAlgorithmType==ALG_TYPE_ID//ALGNAMECHECK)// ע�⽫�㷨���ͱ�ż��� [5/16/2011 Denghl]
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
				//�Ƿ��ж�λ
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
					//4=�޷�ʶ�𣬼�ⳬʱ
					strContent.Format("%s",m_strMessageText[4]) ;
					sprintf_s(sE.cErrorDescription,"%s",strContent);

					std::vector<s_sErrorInfo>::iterator itError;

					itError= itResult->vsErrorInfo.begin();
					
					CRect rcMinRect;// �������С��Ӿ��� [5/18/2011 Denghl]
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

				s_Status sTatus = itInnerCheck->Check(sInnerCheckInputParam,hSrcImage,&sInnerCheckOuputParam);// ���������
			
				if (sTatus.nErrorType!=0)
				{
					sTatusRegion = sTatus;
					if (m_bWriteLog)
					{
						CString str;
						str.Format("ID=%d��InnerCheck-Except��%s",sInnerCheckInputParam.sRegionProperty.nRegionID,sTatus.chErrorContext);
						itInnerCheck->GenCurLog(str,3);
						//m_FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",str);
					}
				}
//��¼ʶ���쳣����////////////////////////////////////////////////////////////////////////////////
//				if(sInnerCheckOuputParam.sErrorInfo.vcCheckResultCode.size()>0&&
//					sInnerCheckOuputParam.sErrorInfo.vcCheckResultCode.size()<=5)
//				{
////#pragma omp critical
//					{
//						CString str;
//						CFileOperate FileOperate;
//						CString strExPath;
//						strExPath.Format("E:\\CHECK�ַ�ʶ���쳣\\");
//						FileOperate.InitFile(strExPath);
//						FileOperate.CreateMultiLevelPath(strExPath);
//						char cCode;
//						str.Format("ID=%d, ʶ���ַ�С�ڵ���5:",sInnerCheckInputParam.sRegionProperty.nRegionID);
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
				// ��ʱ�����ã����ű����� [11/16/2011 Denghl]
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
			// end forÿ���ڵ�������
			/////����ûС����״̬��¼//////////////////////////////////////////////
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
		//��������
		sAlgCheckOutputResult->sCheckResult = m_sCheckResult;
		sPendTime.Start();
		//sPendCheck.End();
		//int nTimeCheck = sPendCheck.GetMillisecondInt();
		//CString str;
		//str.Format("%d",nTimeCheck);
		//m_FileOperate.WriteAlgExceptionLog(m_strLogPath,"�����־","���ʱ��",str);

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
				//"�쳣��־", "�����", strErr);
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
			//m_FileOperate.WriteAlgExceptionLog(m_strLogPath,"�쳣��־","CCheck()�쳣","�㷨�쳣");
		}
	}
	WirteCurLog();
	return m_sStatus;
}

//******************************************************************************//
//����:���º���
//����:
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::UpdateParam()
{
	//���²���

	char strTemp[100];
	memset(strTemp,0,100);
	if (m_FileOperate.IsFileExist(m_strPathini))
	{
		GetPrivateProfileString("��ʱ����", "�Ƿ���Ƴ�ʱ","0", strTemp, sizeof(strTemp), m_strPathini);
		m_bCheckTimeOut = atoi(strTemp);
		GetPrivateProfileString("��ʱ����", "���ż��ʱ��","600", strTemp, sizeof(strTemp), m_strPathini);
		m_nAllTime = atoi(strTemp);
		GetPrivateProfileString("д��־����", "�Ƿ�д�����־","1", strTemp, sizeof(strTemp), m_strPathini);
		m_bWriteLog = atoi(strTemp);
	}
	else
	{
		GetPrivateProfileString("��ʱ����", "�Ƿ���Ƴ�ʱ","0", strTemp, sizeof(strTemp), m_strDefaultParaIni);
		m_bCheckTimeOut = atoi(strTemp);
		GetPrivateProfileString("��ʱ����", "���ż��ʱ��","600", strTemp, sizeof(strTemp), m_strDefaultParaIni);
		m_nAllTime = atoi(strTemp);
		GetPrivateProfileString("д��־����", "�Ƿ�д�����־","1", strTemp, sizeof(strTemp), m_strDefaultParaIni);
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
			itInnerCheck->ReadTempPara();// ������Ҫ���µ����� [5/17/2011 Denghl]
		}
	}

	return m_sStatus;
}
int CCheck::WriteTimePara(CString strPathini)
{
	char strTemp[100];
	sprintf(strTemp, "%d", m_bCheckTimeOut);
	WritePrivateProfileString("��ʱ����", "�Ƿ���Ƴ�ʱ", strTemp, strPathini);
	sprintf(strTemp, "%d", m_nAllTime);
	WritePrivateProfileString("��ʱ����", "���ż��ʱ��", strTemp, strPathini);	
	sprintf(strTemp, "%d", m_bWriteLog);
	WritePrivateProfileString("д��־����", "�Ƿ�д�����־", strTemp, strPathini);	
	return 0;
}

//******************************************************************************//
//����:ѧϰ�󱣴���ģ��
//����:
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::SaveCheckModel(const char *strTaskName,s_SysAuthority sSysAuthority)
{

	//�����㷨��Ҫ��ģ��
	//���Բ���Ҫ���ã���Ϊ����ѧϰ����㷨�ڲ����棬��ʱ���ţ����ֽṹ��һ���ԣ�ÿ��ѧϰ��֮���ֻ��ظ�ִ�����α���
	//
	std::list<CInnerCheck>::iterator itInnerCheck;
	std::list<s_InnerCheckInfo>::iterator itInnerInfo;
	for (itInnerInfo=m_lstSingleCellInnerCheck.begin();
		itInnerInfo!=m_lstSingleCellInnerCheck.end();
		itInnerInfo++)
	{
		for (itInnerCheck = itInnerInfo->lstInnerCheck.begin();itInnerCheck!=itInnerInfo->lstInnerCheck.end();itInnerCheck++)
		{
			itInnerCheck->SaveCheckModel(strTaskName);//����ģ��
		}
	}
	
	return m_sStatus;

}

//******************************************************************************//
//����:���ؼ��ģ���ͼ
//����:
//	iTaskImgType             ���룬��Ҫ���ص�ģ��ͼ�����ͣ�0Ϊ��ģ�壬1ΪСģ��
//	sTaskImgInfo             �����ģ��ͼ����Ϣ
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::GetCheckModelImg(const UINT iTaskImgType, s_TaskImageInfo &sTaskImgInfo,s_SysAuthority sSysAuthority)
{
	//�����ģ��ƴ�����������ش�ͼ
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

					//�ֽ����־���ͺ���־����
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
// ����
CSetPROP::CSetPROP()
{
}
CSetPROP::~CSetPROP()
{

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
//����:��������������
//����:
//	sAlgImageInfo    ���룬ͼ��ṹ��   
//	sRegionProperty   ���룬��������

//����:
//	�㷨ִ��״̬�ṹ��
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
		GetPrivateProfileString("��ʱ����", "�Ƿ���Ƴ�ʱ","0", strTemp, sizeof(strTemp), strPathini);
		m_pChecker->m_bCheckTimeOut = atoi(strTemp);
		GetPrivateProfileString("��ʱ����", "���ż��ʱ��","600", strTemp, sizeof(strTemp), strPathini);
		m_pChecker->m_nAllTime = atoi(strTemp);
	}
	else
	{
		char strTemp[100];
		memset(strTemp,0,100);
		GetPrivateProfileString("��ʱ����", "�Ƿ���Ƴ�ʱ","0", strTemp, sizeof(strTemp), m_pChecker->m_strDefaultParaIni);
		m_pChecker->m_bCheckTimeOut = atoi(strTemp);
		GetPrivateProfileString("��ʱ����", "���ż��ʱ��","600", strTemp, sizeof(strTemp), m_pChecker->m_strDefaultParaIni);
		m_pChecker->m_nAllTime = atoi(strTemp);
	}
	Prodlg.m_pChecker = m_pChecker;

	// ���Ի���
	if (Prodlg.DoModal()==IDOK)
	{
		;
	}
	
	return m_sStatus;
}

//******************************************************************************//
//����:ƴ��ͼ����������
//���ܣ���ƴ�ӵ�����������������
//����:
//	sAlgImageInfo     ���룬ͼ��ṹ��   
//	sRegionPropertyA  ���룬��������
//	sRegionPropertyB  ���룬��������

//����
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CSetPROP::SetAlgJuncParamdlg(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty& sRegionPropertyA,
													const s_RegionProperty& sRegionPropertyB,s_SysAuthority sSysAuthority)
{
	// ���Ի���
	// �����㷨��Ҫƴ��ͼ�����������������Բ�������ǰͨ���㷨����
	
	m_sStatus.nErrorType = 1;
	CString strContent=_T("");
	//5=���㷨��֧�ֿ����ƴ��������!
	strContent.Format("%s",m_pChecker->m_strMessageText[5]) ;
	sprintf_s(m_sStatus.chErrorContext,"%s",strContent);
	//AfxMessageBox(strContent);
	::MessageBoxA(NULL,strContent,"""",MB_OK);
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
s_Status CSetPROP::AlgCopyRegion(const s_AlgImageInfo& sAlgImageInfo,
					   s_RegionProperty &sDstRegionProperty,const s_RegionProperty &sSrcRegionProperty,int nDirection,s_SysAuthority sSysAuthority)
{
	int nSrcRegionID = sSrcRegionProperty.nRegionID;
	int nDstRegionID = sDstRegionProperty.nRegionID;

	

	CString strSrcComCam;
	CString strDstComCam;
	strSrcComCam.Format("Com%dCam%d",sSrcRegionProperty.nComputerID,sSrcRegionProperty.nCameraID);
	strDstComCam.Format("Com%dCam%d",sDstRegionProperty.nComputerID,sDstRegionProperty.nCameraID);
	

	if (1)// �ڲ��㷨ֻ�漰�����ļ����ݵ� [5/17/2011 Denghl]
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

		//�޸Ŀ���������߰���Ʋ���
		CString strParaIniPath;
		strParaIniPath.Format("%s\\2DCodeCheckPara.ini",strDst);
		WritePrivateProfileString("�����߰�", "���Ұ׿����Ƿ��޸�", "1", strParaIniPath);
		WritePrivateProfileString("�����߰�", "���°׿����Ƿ��޸�", "1", strParaIniPath);
		WritePrivateProfileString("�����߰�", "��߽�λ��", "0", strParaIniPath);
		WritePrivateProfileString("�����߰�", "�ұ߽�λ��", "0", strParaIniPath);
		WritePrivateProfileString("�����߰�", "�ϱ߽�λ��", "0", strParaIniPath);
		WritePrivateProfileString("�����߰�", "�±߽�λ��", "0", strParaIniPath);
	}
	else//��Ҫ�����ڲ�copyRegion����
	{
		CInnerCheck InnerCheckTmp;
		InnerCheckTmp.AlgCopyRegion(sAlgImageInfo,sDstRegionProperty,sSrcRegionProperty);
	}
	
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
s_Status CSetPROP::AlgModifyRegion(const s_AlgImageInfo& sAlgImageInfo, const s_RegionProperty &sSrcRegionProperty,s_SysAuthority sSysAuthority)
{

	//�����㷨�޸������С����Ҫ����Ӧ��������ǰͨ���㷨����
	//�޸Ŀ���������߰���Ʋ���
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
	WritePrivateProfileString("�����߰�", "���Ұ׿����Ƿ��޸�", "1", strParaIniPath);
	WritePrivateProfileString("�����߰�", "���°׿����Ƿ��޸�", "1", strParaIniPath);
	WritePrivateProfileString("�����߰�", "��߽�λ��", "0", strParaIniPath);
	WritePrivateProfileString("�����߰�", "�ұ߽�λ��", "0", strParaIniPath);
	WritePrivateProfileString("�����߰�", "�ϱ߽�λ��", "0", strParaIniPath);
	WritePrivateProfileString("�����߰�", "�±߽�λ��", "0", strParaIniPath);
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

	if (1)// �ڲ��㷨ֻ�漰�����ļ����ݵ� [5/17/2011 Denghl]
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
		
		//�޸Ŀ���������߰���Ʋ���
		CString strParaIniPath;
		strParaIniPath.Format("%s\\2DCodeCheckPara.ini",strDst);
		WritePrivateProfileString("�����߰�", "���Ұ׿����Ƿ��޸�", "1", strParaIniPath);
		WritePrivateProfileString("�����߰�", "���°׿����Ƿ��޸�", "1", strParaIniPath);
		WritePrivateProfileString("�����߰�", "��߽�λ��", "0", strParaIniPath);
		WritePrivateProfileString("�����߰�", "�ұ߽�λ��", "0", strParaIniPath);
		WritePrivateProfileString("�����߰�", "�ϱ߽�λ��", "0", strParaIniPath);
		WritePrivateProfileString("�����߰�", "�±߽�λ��", "0", strParaIniPath);
	}
	return sTatus;
}

//s_Status CSetPROP::AlgUpdataRegion(const s_AlgImageInfo& sAlgImageInfo,
//								   const list<s_RegionProperty> &lstUpdataRegion)
//{
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
s_Status CSetPROP::AlgDeleteRegion(const list<s_RegionProperty> &lstRegionProperty,s_SysAuthority sSysAuthority)
{
 	std::list<s_RegionProperty>::const_iterator itRegionProperty;
	if (lstRegionProperty.size()<=0)
	{
		return m_sStatus;
	}
	for (itRegionProperty=lstRegionProperty.begin();itRegionProperty!=lstRegionProperty.end();itRegionProperty++)
	{
		//�Ȼ�ȡ��ӦID�ŵ�ǰ�˺ź������
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

//�õ��㷨�ڲ�����
//******************************************************************************//
//����:�õ��㷨�ڲ�����
//����:
//	lstInnerAlgType     ������㷨�ڲ�֧���㷨������Ϣ 

//����:
//	�㷨ִ��״̬�ṹ��
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
		sprintf_s(sAlgType.cTypeName, "��ά��ʶ��");
	}
	
	lstInnerAlgType.push_back(sAlgType);

	//sAlgType.nInnerTypeID = 1;
	//sAlgType.bPopUpDlg = FALSE;
	//sAlgType.bSupportLearn = FALSE;
	//sprintf_s(sAlgType.cTypeName, "QR��ʶ��");
	//lstInnerAlgType.push_back(sAlgType);

	//sAlgType.nInnerTypeID = 2;
	//sAlgType.bPopUpDlg = FALSE;
	//sAlgType.bSupportLearn = FALSE;
	//sprintf_s(sAlgType.cTypeName, "DM��ʶ��");
	//lstInnerAlgType.push_back(sAlgType);

	//sAlgType.nInnerTypeID = 3;
	//sAlgType.bPopUpDlg = FALSE;
	//sAlgType.bSupportLearn = FALSE;
	//sprintf_s(sAlgType.cTypeName, "PDF417��ʶ��");
	//lstInnerAlgType.push_back(sAlgType);

	

	return m_sStatus;
}

/////////////////////////////////////////////////////////////////////////////////////
// ����
CSetPARA::CSetPARA()
{

}
CSetPARA::~CSetPARA()
{

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
//����:�����Ի�����ýӿ�ʵ��
//����:
//	sAlgImageInfo          ���룬ͼ��ṹ��
//	sSigCellCheckResult    ���룬�������Ϣ���м���
//	sRegionProperty        ���룬��������
//����:
//	�㷨ִ��״̬�ṹ��
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
	//  �޸�[10/10/2015 baiyy]
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

	// ���Ի���
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
//����:ѧϰ���ýӿں���ʵ��
//����:
//  strTaskPath           ���룬����·��
//	listRegionProperty    ���룬��������
//����:
//	�㷨�������ͽṹ��
//******************************************************************************//
s_Status CSetPARA::SetTrainParam(const list<s_RegionProperty>& listRegionProperty,s_SysAuthority sSysAuthority)
{
	//�����㷨ֻ������ӺͶ���ѧϰ��û������ѧϰģʽ��Ҫ���õģ����߲�֧��ѧϰ�ģ������ݲ������
	return m_sStatus;
}