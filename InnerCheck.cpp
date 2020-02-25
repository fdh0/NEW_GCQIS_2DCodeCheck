// Check.cpp: implementation of the CCheck class.
//
//////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Copyright (c) 2008, ����ͼ���Ӿ��������޹�˾�Ӿ��㷨��     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� InnerCheck.cpp												
/* ժҪ�� �ڲ��㷨����ຯ����
/*
/* ���ߣ� �˺���
/* ������ڣ� 2011��5��10��
/************************************************************************/
#pragma once
#include "stdafx.h"
#include "CInnerCheck.h"
#include "shellapi.h"
#include "SpendTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ��ʼ�����Ա����,������Ҫ��Ӵ���
CInnerCheck::CInnerCheck()
{
	memset(&m_sStatus,0,sizeof(s_Status));
	m_bReadMode = FALSE;
	m_bCheckAllMod = FALSE;//��֧��ȫ��ģʽ

//	HException::InstallHHandler(&MyHalconExceptionHandler);
	m_bCheckGao = TRUE;
	GenEmptyRegion(&m_hZoubanRegion);	
	GenEmptyRegion(&m_hInlocMarkRegion);
	GenEmptyRegion(&m_hInlocSearchRegion);
	m_lMarkID = -1;
	m_bShowDJ = FALSE;
	m_bCheckResultMode = TRUE;
	//m_bAlignCorrected = FALSE;

	m_bCheckPartCode = FALSE;
	m_CodeCharL = 0;
	m_CodeCharH = 1024;
	m_bIsTurnedResult = FALSE;
	m_rcAffineTransReg = CRect(0,0,0,0);


	m_bWriteLog = TRUE;
	hDataCodeHandle=-1;
	hResultHandles=-1;
	for (int i=0;i<3;i++)
	{
		phDataCodeHandle[i]=-1;
	}
	m_lstStr=NULL;  
	m_bCheckPrint = FALSE;

	m_strPolar = _T("dark_on_light");
	
	//m_bIsAutoExtL=FALSE;
 //   m_bIsAutoExtR=FALSE;
	//m_bIsAutoExtU=FALSE;
	//m_bIsAutoExtD=FALSE;
	// ͼ��ƽ������ [12/30/2015 TIAN]
	m_bIsImageSmooth = FALSE;

	m_bIsZoomCheck   = FALSE;    //�Ƿ�ʹ��ѹ�����
	m_fZoomRadio = 2;
	m_bZoomCheckFlag = FALSE;  //�Ƿ��ѽ��й�ѹ�����
	m_dHorEnlargeRate = 0; //����Ŵ����
	m_dVerEnlargeRate = 0; //����Ŵ���� 
	m_htErrorC = "0";
	m_htGrade = 0;
	m_htMask = -1;
	m_htModeType = 0;
	m_htVersion = 0;
	
	m_nLanguage         = 0;
	m_strMessagePath= _T("");
	memset(m_strMessageText,0,512*256*sizeof(char));
}

CInnerCheck::~CInnerCheck()
{


}


//******************************************************************************//
//����:������ʼ��
//����:
//	sAlgInitParam    ���룬ȫ��·��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CInnerCheck::Init(const char *strTaskName, const s_AlgInitParam &sAlgInitParam,
						   const s_RegionProperty &sDstRegionProperty,BOOL bIsNewRegion)
{
	//m_bIsAgainCheck = bIsAgainCheck;
	m_sAlgInitParam = sAlgInitParam;
	m_sDstRegionProperty = sDstRegionProperty;
	m_strAlgName.Format("AlgLab2DCodeCheck");
	m_strLogPath.Format("%s\\AlgLab2DCodeCheck", m_sAlgInitParam.cAlglogPath);

	CString str;
	str.Format("%s",sAlgInitParam.cTaskPath);
	//if (m_bIsAgainCheck)
	//{
	//	if (str.GetAt(str.GetLength()-1)=='\\')
	//	{
	//		m_strTaskPath.Format("%s%s\\AgainAlgTask",sAlgInitParam.cTaskPath,strTaskName);
	//	}
	//	else
	//	{
	//		m_strTaskPath.Format("%s\\%s\\AgainAlgTask",sAlgInitParam.cTaskPath,strTaskName);
	//	}
	//	;// ���κ˲�·�� [3/28/2012 Denghl]
	//}
	//else
	//{
		if (str.GetAt(str.GetLength()-1)=='\\')
		{
			m_strTaskPath.Format("%s%s\\AlgTask",sAlgInitParam.cTaskPath,strTaskName);
		}
		else
		{
			m_strTaskPath.Format("%s\\%s\\AlgTask",sAlgInitParam.cTaskPath,strTaskName);
		}

	//}
	
	m_sAlgInitParam = sAlgInitParam;
	m_sDstRegionProperty = sDstRegionProperty;

	// ��������Ŵ���������Ŀ¼ 
	m_strRegionPath.Format("%s\\Com%dCam%d\\%s\\%d", 
		m_strTaskPath, 
		sDstRegionProperty.nComputerID,
		sDstRegionProperty.nCameraID,
		m_strAlgName,
		sDstRegionProperty.nRegionID);

	FileOperate.CreateMultiLevelPath(m_strRegionPath);

	m_strAlgPublicLib.Format("%s\\AlgPublicLib\\%s",sAlgInitParam.cAlgPath,m_strAlgName);
	FileOperate.CreateMultiLevelPath(m_strAlgPublicLib);

	str.Format("%s",sAlgInitParam.cAlgPath);
	if (str.GetAt(str.GetLength()-1)=='\\')
	{
		m_strDefaultParaPath.Format("%sDefaultParameter\\%s",sAlgInitParam.cAlgPath,m_strAlgName);
	}
	else
	{
		m_strDefaultParaPath.Format("%s\\DefaultParameter\\%s",sAlgInitParam.cAlgPath,m_strAlgName);
	}
	FileOperate.CreateMultiLevelPath(m_strDefaultParaPath);

	//  [4/7/2017 zb]
	//  Ӣ����־����Ϣ����·��������
	m_nLanguage     = sAlgInitParam.nLanguage;
	int nTextNum = 0;
	if (m_nLanguage == 0)
	{
		m_strMessagePath.Format("%s\\International Config\\MessageChinese.ini", m_strDefaultParaPath);
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
		m_strMessagePath.Format("%s\\International Config\\MessageEnglish.ini", m_strDefaultParaPath);
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
	CString strDefConfig;
	strDefConfig.Format("%s\\2DCodeCheckPara.ini",m_strDefaultParaPath);
	m_strDefaultParaIni = strDefConfig;
	m_strParaIni.Format("%s\\2DCodeCheckPara.ini",m_strRegionPath);

	// [11/12/2011 Denghl]
	FreeTMPSpace();
	BOOL nRet = FileOperate.IsFileExist(m_strParaIni);
	if (nRet)
	{
		ReadPara(m_strParaIni);// ��ȡ������ [6/24/2010 dhl]
		WriteAddDefaultPara(); // ���������������
	}
	else
	{
		ReadPara(strDefConfig);// ��ȡ������ [6/24/2010 dhl]
		WriteAddDefaultPara();
		WritePara(m_strParaIni);
	}

	RequistSpace();
	m_bCheckPrint = FALSE;
	m_fScaleHor = m_sAlgInitParam.fResolution_H;
	m_fScaleVer = m_sAlgInitParam.fResolution_V;
	return m_sStatus;
}


//******************************************************************************//
//����:����ͷſռ�
//����:
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CInnerCheck::Free()
{
	//�ͷź���
	// ״̬�ṹ���ʼ�� [3/22/2017 zb]
	memset(&m_sStatus.chErrorInfo, 0, DH_MAX_TEXTSIZE*sizeof(char));
	memset(&m_sStatus.chErrorContext, 0, DH_MAX_TEXTSIZE*sizeof(char));
	m_sStatus.nErrorType = 0;
	try
	{
		if (m_bReadMode)
		{
			if (m_iCheckMethod!=3&&hDataCodeHandle>=0)
			{
				ClearDataCode2dModel(hDataCodeHandle);
				hDataCodeHandle=-1;
				m_bReadMode = FALSE;
			}
			else
			{
				for (int i=0;i<3;i++)
				{
					if (phDataCodeHandle[i]>=0)
					{
						ClearDataCode2dModel(phDataCodeHandle[i]);
						phDataCodeHandle[i]=-1;
					}
					
				}			
				m_bReadMode = FALSE;
			}
		}
		if (m_lMarkID>=0)
		{
			ClearNccModel(m_lMarkID);
			m_lMarkID = -1;
		}
		//if (m_bUseDll&&m_sProductParam!=NULL)// ��ʱ����
		//{
		//	if (m_hDll&&freeRMBProductParamMemFun)
		//	{
		//		freeRMBProductParamMemFun(m_sProductParam);
		//	}
		//}
	}
	catch(HException &hException)
	{
		m_sStatus.nErrorType = 1;
		CString strErr;
		strErr.Format("Free()Except-HALCON Except-%s-%s", hException.ProcName().Text(),hException.ErrorMessage().Text());
		//  [10/14/2015 zl]
		GenCurLog(strErr,3);
		//FileOperate.WriteAlgExceptionLog(m_strLogPath, 
			//"�쳣��־", "�����", strErr);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		//FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "������ͷź���",m_sStatus.chErrorContext);
	}

	FreeTMPSpace();
		
	return m_sStatus;
}

int CInnerCheck::UTF8ToGBK(unsigned char * lpUTF8Str,unsigned char * lpGBKStr,int nGBKStrLen)
{
	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;

	if(!lpUTF8Str)  //���UTF8�ַ���ΪNULL������˳�
		return 0;

	nRetLen = ::MultiByteToWideChar(CP_UTF8,0,(char *)lpUTF8Str,-1,NULL,NULL);  //��ȡת����Unicode���������Ҫ���ַ��ռ䳤��
	lpUnicodeStr = new WCHAR[nRetLen + 1];  //ΪUnicode�ַ����ռ�
	nRetLen = ::MultiByteToWideChar(CP_UTF8,0,(char *)lpUTF8Str,-1,lpUnicodeStr,nRetLen);  //ת����Unicode����
	if(!nRetLen)  //ת��ʧ��������˳�
		return 0;

	nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,NULL,NULL,NULL,NULL);  //��ȡת����GBK���������Ҫ���ַ��ռ䳤��

	if(!lpGBKStr)  //���������Ϊ���򷵻�ת������Ҫ�Ŀռ��С
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return nRetLen;
	}

	if(nGBKStrLen < nRetLen)  //���������������Ȳ������˳�
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return 0;
	}

	nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,(char *)lpGBKStr,nRetLen,NULL,NULL);  //ת����GBK����

	if(lpUnicodeStr)
		delete []lpUnicodeStr;

	return nRetLen;
}
//******************************************************************************//
//����:���������
//����:
//	sAlgCheckInputParam      ���룬�������ṹ��
//	sAlgCheckOutputResult    ��������ؼ��������ṹ��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CInnerCheck::Check(const s_InnerCheckInputParam& sAlgInnerCheckInputParam,const HObject hSrcImage,s_InnerCheckOutputParam *sAlgInnerCheckOutputResult)
{
	// ��ȡͼ������ [11/11/2011 Denghl]
	try
	{
		//CString  strContent = "���Գɹ�";
		//GenCurLog(strContent,3);
		CString strAddInfo=_T("");
		m_htGrade = 0;
		m_bZoomCheckFlag = FALSE;
		m_bCheckPrint = FALSE;
		m_dVerEnlargeRate = 1.0;
		m_dHorEnlargeRate = 1.0;
		m_sDstRegionProperty = sAlgInnerCheckInputParam.sRegionProperty;

		CRect rcMinRect;// �������С��Ӿ��� [5/18/2011 Denghl]
		std::list<CPoint>::iterator itRegionPoint;
		if (m_sDstRegionProperty.lstRegionOrg.size()>0)
		{

			itRegionPoint=m_sDstRegionProperty.lstRegionOrg.begin();
			rcMinRect.top = rcMinRect.bottom = itRegionPoint->y;
			rcMinRect.left = rcMinRect.right = itRegionPoint->x;
			for(;itRegionPoint!=m_sDstRegionProperty.lstRegionOrg.end();
				itRegionPoint++)
			{
				rcMinRect.top = min(rcMinRect.top,itRegionPoint->y);
				rcMinRect.bottom = max(rcMinRect.bottom,itRegionPoint->y);
				rcMinRect.left = min(rcMinRect.left,itRegionPoint->x);
				rcMinRect.right = max(rcMinRect.right,itRegionPoint->x);
			}

			if (sAlgInnerCheckInputParam.sAlgImageInfo.nImageType==0
				&&sAlgInnerCheckInputParam.sAlgImageInfo.sizeImage.cx==m_sAlgInitParam.nWidth
				&&sAlgInnerCheckInputParam.sAlgImageInfo.sizeImage.cy==m_sAlgInitParam.nHeight)
			{
				// ƽ�Ʊ任���� [11/12/2011 Denghl]
				if (sAlgInnerCheckInputParam.sLocGroupResult.bHaveError&&m_sDstRegionProperty.nLocateGroupID>0)
				{
					//				nResult = 1;
					m_rcAffineTransReg.left = MAX(rcMinRect.left ,0);
					m_rcAffineTransReg.right = rcMinRect.right ;
					m_rcAffineTransReg.top = MAX(rcMinRect.top,0);
					m_rcAffineTransReg.bottom = rcMinRect.bottom ;
				}
				else
				{
					m_rcAffineTransReg.left = MAX(rcMinRect.left - (long)sAlgInnerCheckInputParam.sLocGroupResult.fColShiftLoc,0);
					m_rcAffineTransReg.right = rcMinRect.right - (long)sAlgInnerCheckInputParam.sLocGroupResult.fColShiftLoc;
					m_rcAffineTransReg.top = MAX(rcMinRect.top - (long)sAlgInnerCheckInputParam.sLocGroupResult.fLineShiftLoc,0);
					m_rcAffineTransReg.bottom = rcMinRect.bottom - (long)sAlgInnerCheckInputParam.sLocGroupResult.fLineShiftLoc;
				}

			}
			else
			{
				m_rcAffineTransReg.left = 0;
				m_rcAffineTransReg.right = sAlgInnerCheckInputParam.sAlgImageInfo.sizeImage.cx-1;// -1 [10/12/2015 TIAN]
				m_rcAffineTransReg.top = 0;
				m_rcAffineTransReg.bottom = sAlgInnerCheckInputParam.sAlgImageInfo.sizeImage.cy-1;// -1 [10/12/2015 TIAN]

			}
		}
		m_rcAffineTransReg = RECTBOUND(m_rcAffineTransReg,CRect(0,0,m_sAlgInitParam.nWidth-1,m_sAlgInitParam.nHeight-1));

		sAlgInnerCheckOutputResult->nFlagStudy = 0;
		sAlgInnerCheckOutputResult->sErrorInfo.nRegionID     = sAlgInnerCheckInputParam.sRegionProperty.nRegionID;
		sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType  = ABNORMAL_2D;
		sAlgInnerCheckOutputResult->sErrorInfo.bytErrorGrade = 0;
		//memcpy(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
		//	s_CodeOCROutput.chCheckInfor,254);// ����������Ҫ����ȱ����д [11/18/2011 Denghl]
		sAlgInnerCheckOutputResult->sErrorInfo.nLocateGroupID = m_sDstRegionProperty.nLocateGroupID;
		sAlgInnerCheckOutputResult->sErrorInfo.RectErrorModel = m_rcAffineTransReg;
		sAlgInnerCheckOutputResult->sErrorInfo.RectErrorReg = m_rcAffineTransReg;
		sAlgInnerCheckOutputResult->sErrorInfo.vcCheckResultCode.clear();
		//  [4/7/2017 zb]
		CString strContent = _T("");
		//6=ʶ��ʧ��
		strContent.Format("%s",m_strMessageText[6]) ;
		sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,"%s",strContent);
		strAddInfo.Format("RC=%s,Version=%d,DJ=%d,MT=%d,Pattern=%d",
			m_htErrorC[0].S().Text(),m_htVersion[0].D(),m_htGrade[0].D(),m_htModeType[0].D(),m_htMask[0].D());
			//	strAddInfo.Format("Version=%d,DJ=%d,MT=%d,Pattern=%d",
			//m_htVersion[0].D(),m_htGrade[0].D(),m_htModeType[0].D(),m_htMask[0].D());

		sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cResvered,"%s",strAddInfo);

		//double dTimeStart, dTimeCount;
		//CountSeconds(&dTimeStart);//��ʼ��ʱ
		hv_AreaAll = 0;

		m_iRealMethod = -1;
		s_Status sStatus;
		sStatus.nErrorType=0;
		eTrainFlag ETF = sAlgInnerCheckInputParam.sRegionProperty.TrainFlag;
		eOperateFlag OPF = sAlgInnerCheckInputParam.sRegionProperty.OperateFlag;

		if (OF_DoNothing == OPF||sAlgInnerCheckInputParam.sRegionProperty.bIsValid==FALSE)
		{
			//do nothing
			return sStatus;
		}
		if (sAlgInnerCheckInputParam.sAlgImageInfo.nBits==24)
		{
			if (sAlgInnerCheckInputParam.sAlgImageInfo.pImageR==NULL
				||sAlgInnerCheckInputParam.sAlgImageInfo.pImageG==NULL
				||sAlgInnerCheckInputParam.sAlgImageInfo.pImageB==NULL)
			{
				sStatus.nErrorType=1;
				//sprintf_s(sStatus.chErrorContext,"Locate sMiddleResult is empty!");
				//  [4/7/2017 zb]
				CString strContent = _T("");
				//2=ͼ������Ϊ��
				strContent.Format("%s",m_strMessageText[2]) ;
				sprintf_s(sStatus.chErrorContext,"%s","strContent");
				if (m_bWriteLog)
				{
					CString strContent;
					strContent.Format("%s",m_sStatus.chErrorContext);
					GenCurLog(strContent,3);
					//FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",sStatus.chErrorContext);
				}
				return sStatus;
			}
		}
		else if (sAlgInnerCheckInputParam.sAlgImageInfo.nBits==8)
		{
			if (sAlgInnerCheckInputParam.sAlgImageInfo.pImageData==NULL)
			{
				sStatus.nErrorType=1;
				//sprintf_s(sStatus.chErrorContext,"Locate sMiddleResult is empty!");
				//  [4/7/2017 zb]
				CString strContent = _T("");
				//2=ͼ������Ϊ��
				strContent.Format("%s",m_strMessageText[2]) ;
				sprintf_s(sStatus.chErrorContext,"%s","strContent");

				if (m_bWriteLog)
				{
					CString strContent;
					strContent.Format("%s",m_sStatus.chErrorContext);
					GenCurLog(strContent,3);
					//FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",sStatus.chErrorContext);
				}
							
				return sStatus;
			}
		} 
		else
		{
			sStatus.nErrorType=1;
			//sprintf_s(sStatus.chErrorContext,"Locate sMiddleResult is empty!");
			//  [4/7/2017 zb]
			CString strContent = _T("");
			//3=ͼ����������λ������
			strContent.Format("%s",m_strMessageText[3]) ;
			sprintf_s(sStatus.chErrorContext,"%s",strContent);

			if (m_bWriteLog)
			{
				CString strContent;
				strContent.Format("%s",m_sStatus.chErrorContext);
				GenCurLog(strContent,3);
				//FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",sStatus.chErrorContext);
			}
		
			
			return sStatus;

		}
		//AfxMessageBox("2");
		// ��ȡͼ�� [11/12/2011 Denghl]
		if (GenCheckImage(sAlgInnerCheckInputParam,hSrcImage)!=0)
		{
			sStatus.nErrorType=1;
			//sprintf_s(sStatus.chErrorContext,"Locate sMiddleResult is empty!");
			//  [4/7/2017 zb]
			CString strContent = _T("");
			//7=GenCheckImage-��ȡ���ͼ��ʧ��
			strContent.Format("%s",m_strMessageText[7]) ;
			sprintf_s(sStatus.chErrorContext,"%s",strContent);
			
			if (m_bWriteLog)
			{
				CString strContent;
				strContent.Format("%s",sStatus.chErrorContext);
				GenCurLog(strContent,3);
				//FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",sStatus.chErrorContext);
			}
			
			return sStatus;
		}
		//  [12/26/2014 baiyy]
		//CountSeconds(&dTimeCount);
		//dTimeCount-=dTimeStart;
		//CString strInfo;
		//strInfo.Format("���:��ȡͼ��ɹ�,��ʱ:%.3fms\r\n", dTimeCount*1000);
		//m_FileOperate.WriteAlgOperateLog(m_strLogPath,
		//	"������־", "������־", strInfo);

		//dTimeCount =0.0;
		//dTimeStart = 0.0;
		//CountSeconds(&dTimeStart);

		HTuple  hDecodedDataStrings;
		//char  str[MAXCODENUM]; 
		//memset(str,0, MAXCODENUM);
		HTuple lLength = 0;
		//���ؽ����ʼ��
		//SetSystem("filename_encoding", "locale");
		/*HTuple  Information;
		CString strInfo;
		get_system("filename_encoding", &Information);
		strInfo = Information[0].S();*/
		//AfxMessageBox("3");
		if (m_bReadMode==TRUE)
		{
			//if (m_iCheckMethod >= 0&& m_iCheckMethod<3)
			//{
			//	FindDataCode2d( m_hImageCheck, &hSymbolXLDs, hDataCodeHandle, HTuple(), HTuple(), 
			//		&hResultHandles, &hDecodedDataStrings);
			//	//h_DataString = hDataCodeHandle[0].S();

			//	TupleLength(hDecodedDataStrings, &lLength);
			//	if (lLength!=1)
			//	{
			//		double fBaseScale = MIN(m_fScaleHor,m_fScaleVer);
			//		double fScaleWid = fBaseScale/m_fScaleVer;
			//		double fScaleHei = fBaseScale/m_fScaleHor;
			//		ZoomImageFactor(m_hImageCheck,&m_hImageCheck,fScaleWid,fScaleHei,"constant");
			//	//	write_image(m_hImageCheck,"bmp",0,"d:\\1.bmp");
			//		FindDataCode2d( m_hImageCheck, &hSymbolXLDs, hDataCodeHandle, HTuple(), HTuple(), 
			//			&hResultHandles, &hDecodedDataStrings);
			//		TupleLength(hDecodedDataStrings, &lLength);
			//	}
			//}
			//else if (m_iCheckMethod==3)// Ԥ�Ȳ�֪����ά�����͵�
			//{
			//	for (int i=0;i<3;i++)
			//	{					
			//		FindDataCode2d( m_hImageCheck, &hSymbolXLDs, phDataCodeHandle[i], HTuple(), HTuple(), 
			//			&hResultHandles, &hDecodedDataStrings);
			//		TupleLength(hDecodedDataStrings, &lLength);
			//		if (lLength == 1)
			//		{
			//			m_iRealMethod = i;
			//			break;
			//		}
			//	}				
			//}
			sAlgInnerCheckOutputResult->nFlagStudy = 0;
			sAlgInnerCheckOutputResult->sErrorInfo.nRegionID = sAlgInnerCheckInputParam.sRegionProperty.nRegionID;
			sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = NORMAL_2D;
			sAlgInnerCheckOutputResult->sErrorInfo.bytErrorGrade = 0;
			//memcpy(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
			//	s_CodeOCROutput.chCheckInfor,254);// ����������Ҫ����ȱ����д [11/18/2011 Denghl]
			sAlgInnerCheckOutputResult->sErrorInfo.nLocateGroupID = m_sDstRegionProperty.nLocateGroupID;
			sAlgInnerCheckOutputResult->sErrorInfo.RectErrorModel = m_rcAffineTransReg;
			sAlgInnerCheckOutputResult->sErrorInfo.RectErrorReg = m_rcAffineTransReg;
			sAlgInnerCheckOutputResult->sErrorInfo.vcCheckResultCode.clear();
			//  [4/7/2017 zb]
			CString strContent = _T("");
			//8=����
			strContent.Format("%s",m_strMessageText[8]) ;
			sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
				"%s",strContent);

			/*CountSeconds(&dTimeCount);
			dTimeCount-=dTimeStart;
			CString strInfo;
			strInfo.Format("���:��һ��ʶ��,��ʱ:%.3fms\r\n", dTimeCount*1000);
			m_FileOperate.WriteAlgOperateLog(m_strLogPath,
			"������־", "������־", strInfo);

			dTimeCount =0.0;
			dTimeStart = 0.0;
			CountSeconds(&dTimeStart);*/
			if (0/*m_sCheckPrintParam.sCorrPixelParam.bValid*/)//��λУ��
			{
				// ���δ�λУ����������Ҫ�ٴο��� [10/12/2015 TIAN]
				// [12/25/2014 baiyy]
				HObject hRegionCode, hRegTmp,hTempImg;
				HTuple row1,row2,col1,col2;
				int  i;
				HTuple lWid,lHei;
				//HObject hImg;
				//write_image(m_hImageCheck, "bmp", 255, "e:\\er.bmp");
				//MirrorImage(m_hImageCheck, &hImg, "row");
				SelectObj(m_hImageCheck,&hTempImg,1);
				GetImageSize(hTempImg, &lWid,&lHei);
				BinThreshold(m_hImageCheck, &hRegionCode);
				ClosingRectangle1 (hRegionCode, &hRegTmp, 31, 31);
				Connection (hRegTmp, &hRegTmp);

				SelectShapeStd (hRegTmp, &hRegTmp, "max_area", 70);
				Intersection(hRegTmp, hRegionCode, &hRegionCode);

				SmallestRectangle1(hRegionCode, &row1, &col1, &row2, &col2);
				if ((row2-row1<10)||(col2-col1<10))
				{
					//  [4/7/2017 zb]
					CString strContent = _T("");
					//9=����Ϊ�գ���鿴��Ʒ����
					strContent.Format("%s",m_strMessageText[9]) ;
					sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
						"%s",strContent);
					sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = EMPTY_2D;
					return m_sStatus;
				}
				//int r1,r2;
				//r1 = row1[0].D();
				//r2 = row2[0].D();
				HTuple ColXX,Diff;
				ColXX =col2;
				Diff = 0;
				for (i=row1[0].I()+20;i>row1[0].I()+1;i--)
				{
					HObject hReg;
					HTuple r1,r2,c1,c2;
					GenRectangle1(&hReg, i, col1, i, col2);
					Intersection(hReg, hRegionCode, &hReg);
					SmallestRectangle1(hReg, &r1, &c1, &r2, &c2);
					if (c2[0].D()<col2[0].D()-50)
					{
						ColXX = c2;
						Diff = HTuple(i)-row1;
						break;
					}
				}
				if (Diff<HTuple(m_sCheckPrintParam.sCorrPixelParam.nCorrPixels)&&Diff>0&&ColXX>col1)
				{
					HObject hRegionPart1,hRegionPart1Move,hRegionPart2,hRegionUnion;
					HObject hCheckImg;
					GenRectangle1(&hRegionPart1, row1, col1, row2, ColXX);
					Intersection(hRegionPart1, hRegionCode, &hRegionPart1);
					MoveRegion(hRegionPart1, &hRegionPart1Move, Diff, 0);
					Difference(hRegionCode, hRegionPart1, &hRegionPart2);
					Union2(hRegionPart1Move, hRegionPart2, &hRegionUnion);
					RegionToBin(hRegionUnion, &hCheckImg, 0, 255, lWid, lHei);
					if (m_iCheckMethod >= 0&& m_iCheckMethod<3)
					{
						//#pragma omp critical
						FindDataCode2d( hCheckImg, &hSymbolXLDs, hDataCodeHandle, "stop_after_result_num", 1, 
							&hResultHandles, &hDecodedDataStrings);
						//h_DataString = hDataCodeHandle[0].S();

						TupleLength(hDecodedDataStrings, &lLength);
						if (lLength!=1)
						{
							double fBaseScale = MIN(m_fScaleHor,m_fScaleVer);
							double fScaleWid = fBaseScale/m_fScaleVer;
							double fScaleHei = fBaseScale/m_fScaleHor;
							ZoomImageFactor(m_hImageCheck,&m_hImageCheck,fScaleWid,fScaleHei,"constant");
							//	write_image(m_hImageCheck,"bmp",0,"d:\\1.bmp");
							//#pragma omp critical
							FindDataCode2d( m_hImageCheck, &hSymbolXLDs, hDataCodeHandle,"stop_after_result_num", 1, 
								&hResultHandles, &hDecodedDataStrings);
							TupleLength(hDecodedDataStrings, &lLength);
						}
					}
					else if (m_iCheckMethod==3)// Ԥ�Ȳ�֪����ά�����͵�
					{
						for (int i=0;i<3;i++)
						{		
							//#pragma omp critical
							FindDataCode2d( hCheckImg, &hSymbolXLDs, phDataCodeHandle[i],"stop_after_result_num", 1, 
								&hResultHandles, &hDecodedDataStrings);
							TupleLength(hDecodedDataStrings, &lLength);
							if (lLength[0].I() == 1)
							{
								m_iRealMethod = i;
								break;
							}
						}				
					}
				}
				else
				{
					//  [4/7/2017 zb]
					CString strContent = _T("");
					//10=�����λ���أ���鿴��Ʒ
					strContent.Format("%s",m_strMessageText[10]);
					sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
						"%s",strContent);
					sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = ABNORMAL_2D;
					return m_sStatus;
				}
				/*CountSeconds(&dTimeCount);
				dTimeCount-=dTimeStart;
				CString strInfo;
				strInfo.Format("���:�ڶ���ʶ��,��ʱ:%.3fms\r\n", dTimeCount*1000);
				m_FileOperate.WriteAlgOperateLog(m_strLogPath,
				"������־", "������־", strInfo);*/
			}
			else //���
			{
				//AfxMessageBox("4");
				HTuple htChannelNum;
				CountObj(m_hImageCheck,&htChannelNum);
				
				if (m_iCheckMethod >= 0 && m_iCheckMethod<3)
				{	
					//////////////////////////////////////////////////////////////////////////
					if (!m_bIsZoomCheck) //ͼ��ѹ�����
					{
						if (!m_bIsImageSmooth)//��ƽ������
						{
							for (int nIndex=0;nIndex<htChannelNum[0].L();nIndex++)
							{
								HObject hTemp;
								SelectObj(m_hImageCheck,&hTemp,nIndex+1);
								FindDataCode2d(hTemp, &hSymbolXLDs, hDataCodeHandle, "stop_after_result_num", 1, 
									&hResultHandles, &hDecodedDataStrings);
								TupleLength(hDecodedDataStrings, &lLength);
								if (lLength[0].L()==1)
								{
									break;
								}
							}
							
						}		
						else if(m_bIsImageSmooth)//ƽ������
						{
							
							//GrayOpeningRect(m_hImageCheck,&hImageCheck,3,3);
							for (int nIndex=0;nIndex<htChannelNum[0].L();nIndex++)
							{
								HObject hTemp,hImageCheck;;
								SelectObj(m_hImageCheck,&hTemp,nIndex+1);
								GrayOpeningRect(hTemp,&hImageCheck,3,3);
								FindDataCode2d(hImageCheck, &hSymbolXLDs, hDataCodeHandle, "stop_after_result_num", 1, 
									&hResultHandles, &hDecodedDataStrings);
								TupleLength(hDecodedDataStrings, &lLength);
								if (lLength[0].L()==1)
								{
									break;
								}
							}
							
						}
						//TupleLength(hDecodedDataStrings, &lLength);
						if(lLength[0].I()!=1)
						{
						    double fBaseScale = MIN(m_fScaleHor,m_fScaleVer);
							double fScaleWid = fBaseScale/m_fScaleVer;
							double fScaleHei = fBaseScale/m_fScaleHor;
							if (abs(fScaleWid-fScaleHei)>0.01)// ���ݱ��ʲ�һ�� [9/9/2015 dhl]
							{
								HObject hCheckImg;
								//ZoomImageFactor(m_hImageCheck,&hCheckImg,fScaleWid,fScaleHei,"constant");
								m_bZoomCheckFlag = TRUE;
								m_dHorEnlargeRate = 1.0/(fScaleWid);
								m_dVerEnlargeRate = 1.0/(fScaleHei);
								//	write_image(m_hImageCheck,"bmp",0,"d:\\1.bmp");
								//	#pragma omp critical
								for (int nIndex=0;nIndex<htChannelNum[0].L();nIndex++)
								{
									HObject hTemp;
									SelectObj(m_hImageCheck,&hTemp,nIndex+1);
									ZoomImageFactor(hTemp,&hCheckImg,fScaleWid,fScaleHei,"constant");
									FindDataCode2d(hCheckImg, &hSymbolXLDs, hDataCodeHandle, "stop_after_result_num", 1, 
										&hResultHandles, &hDecodedDataStrings);
									TupleLength(hDecodedDataStrings, &lLength);
									if (lLength[0].L()==1)
									{
										break;
									}
								}								
							}
						}
					}
					else if (m_bIsZoomCheck)//ͼ��ѹ�����
					{
						HObject hZoomImageCheck;
						m_bZoomCheckFlag = FALSE;  //ѹ����⣬ʶ��ɹ���־
						double fBaseScale = MIN(m_fScaleHor,m_fScaleVer);
						double fScaleWid = fBaseScale/m_fScaleVer;
						double fScaleHei = fBaseScale/m_fScaleHor;
						for (int nIndex=0;nIndex<htChannelNum[0].L();nIndex++)
						{
							HObject hTemp,hImageCheck;;
							SelectObj(m_hImageCheck,&hTemp,nIndex+1);
							ZoomImageFactor(hTemp,&hZoomImageCheck,fScaleWid*m_fZoomRadio,fScaleHei*m_fZoomRadio,"constant");
							if (!m_bIsImageSmooth)//��ƽ������
							{
								FindDataCode2d(hZoomImageCheck, &hSymbolXLDs, hDataCodeHandle, "stop_after_result_num", 1, 
									&hResultHandles, &hDecodedDataStrings);
							}		
							else if(m_bIsImageSmooth)//ƽ������
							{
								HObject hSmoothImageCheck;
								GrayOpeningRect(hZoomImageCheck,&hSmoothImageCheck,3,3);
								FindDataCode2d(hSmoothImageCheck, &hSymbolXLDs, hDataCodeHandle, "stop_after_result_num", 1, 
									&hResultHandles, &hDecodedDataStrings);	
							}							
							TupleLength(hDecodedDataStrings, &lLength);
							if (lLength[0].L()==1)
							{
								break;
							}
						}
						if (lLength[0].I() == 1)
						{
							m_bZoomCheckFlag  = TRUE;
							m_dHorEnlargeRate = 1.0/(fScaleWid*m_fZoomRadio);
						    m_dVerEnlargeRate = 1.0/(fScaleHei*m_fZoomRadio);	
						}	
					}
					//////////////////////////////////////////////////////////////////////////
					// ���ݱ��ʲ�һ�£��˴��д����ƣ��Ӻ������������
					//if (lLength!=1) 
					//{
					//	double fBaseScale = MIN(m_fScaleHor,m_fScaleVer);
					//	double fScaleWid  = fBaseScale/m_fScaleHor;
					//	double fScaleHei  = fBaseScale/m_fScaleVer;
					//	
					//	if (abs(fScaleWid-fScaleHei)>0.01)// ���ݱ��ʲ�һ�� [9/9/2015 dhl]
					//	{
					//		ZoomImageFactor(m_hImageCheck,&m_hImageCheck,fScaleWid,fScaleHei,"constant");
					//		if (!m_bIsImageSmooth) //m_bIsImageSmooth = 0
					//		{
					//			FindDataCode2d(m_hImageCheck, &hSymbolXLDs, hDataCodeHandle, "stop_after_result_num", 1, 
					//				&hResultHandles, &hDecodedDataStrings);	
					//		}
					//		else
					//		{
					//			HObject hImageCheck;
					//			GrayOpeningRect(m_hImageCheck,&hImageCheck,3,3);
					//			FindDataCode2d(hImageCheck, &hSymbolXLDs, hDataCodeHandle, "stop_after_result_num", 1, 
					//				&hResultHandles, &hDecodedDataStrings);	
					//		}		
					//		TupleLength(hDecodedDataStrings, &lLength);
					//	}					
					//}
					//AfxMessageBox("5");
				}
				else if (m_iCheckMethod==3)// Ԥ�Ȳ�֪����ά�����͵�
				{
					HObject hImageCheck;
					//  [9/7/2016 zb] ��������ά�����ͼ������ͬ��
					if (!m_bIsZoomCheck) //ͼ��ѹ�����
					{
						if (!m_bIsImageSmooth) //m_bIsImageSmooth = 0
						{
							hImageCheck = m_hImageCheck;
						}
						else
						{
							GrayOpeningRect(m_hImageCheck,&hImageCheck,3,3);
						}
					}
					else
					{
						HObject hZoomImageCheck;
						m_bZoomCheckFlag = FALSE;  //ѹ����⣬ʶ��ɹ���־
						double fBaseScale = MIN(m_fScaleHor,m_fScaleVer);
						double fScaleWid = fBaseScale/m_fScaleVer;
						double fScaleHei = fBaseScale/m_fScaleHor;

						ZoomImageFactor(m_hImageCheck,&hZoomImageCheck,fScaleWid*m_fZoomRadio,fScaleHei*m_fZoomRadio,"constant");
						if (!m_bIsImageSmooth) //m_bIsImageSmooth = 0
						{
							hImageCheck = hZoomImageCheck;
						}
						else
						{
							GrayOpeningRect(hZoomImageCheck,&hImageCheck,3,3);
						}
					}
					//////////////////////////////////////////////////////////////////////////
					for (int i=0;i<3;i++)
					{		
						//#pragma omp critical
						FindDataCode2d(hImageCheck, &hSymbolXLDs, phDataCodeHandle[i], "stop_after_result_num", 1, 
							&hResultHandles, &hDecodedDataStrings);
						TupleLength(hDecodedDataStrings, &lLength);
						if (lLength[0].I() == 1)
						{
							m_iRealMethod = i;
							break;
						}
					}				
				}
				/*	CountSeconds(&dTimeCount);
				dTimeCount-=dTimeStart;
				CString strInfo;
				strInfo.Format("���:�ڶ���ʶ��,��ʱ:%.3fms\r\n", dTimeCount*1000);
				m_FileOperate.WriteAlgOperateLog(m_strLogPath,
				"������־", "������־", strInfo);*/
			}
			/*	dTimeCount =0.0;
			dTimeStart = 0.0;
			CountSeconds(&dTimeStart);*/
			//AfxMessageBox("6");
			CString strCode;
			strCode = _T("");
			m_CheckResultCode.clear();
		    BOOL bErrorFlag = FALSE;
			//AfxMessageBox("61");
			if (lLength[0].I() == 1)
			{	
				unsigned char *pDataDst, *pDataSrc;
				pDataSrc = new unsigned char[MAXCODENUM*sizeof(char)];
				pDataDst = new unsigned char[MAXCODENUM*sizeof(char)];
				HTuple hString;
				TupleString(hDecodedDataStrings, "s", &hString);
				//AfxMessageBox("62");
				// ����� [3/24/2016 Administrator]
				memset(pDataSrc,0,MAXCODENUM*sizeof(char));
				memcpy(pDataSrc, hString.S().Text(), hString.S().Length()*sizeof(char));
				//AfxMessageBox("63");
				if (m_bIsTurnedResult)
				{
					UTF8ToGBK(pDataSrc, pDataDst, MAXCODENUM);

					for (int ii=0;ii<MAXCODENUM;ii++)
					{
						if (pDataDst[ii]==0)
						{
							break;
						}
						else if (pDataDst[ii]==13)
						{
							strCode+="\\r";
						}
						else if (pDataDst[ii]==10)
						{
							strCode+="\\n";
						}
						else
						{
							//m_CheckResultCode.push_back(pDataDst[ii]);
							strCode+=pDataDst[ii];
						}				
					}
				}
				else
				{
					for (int ii=0;ii<MAXCODENUM;ii++)
					{

						if (pDataSrc[ii]==0)
						{
							break;
						}
						else if (pDataSrc[ii]==13)
						{
							strCode+="\\r";
						}
						else if (pDataSrc[ii]==10)
						{
							strCode+="\\n";
						}
						else
						{
							//m_CheckResultCode.push_back(pDataDst[ii]);
							strCode+=pDataSrc[ii];
						}	
					}
				}
//				// ������ [10/23/2015 TIAN]
//				//strCode="?";
//				if(strCode.GetLength()<=5)
//				{
////#pragma omp critical
//					//{
//						static int icount=0;
//						CString strEx;
//						strEx.Format("e:\\m_hImageCheck_%d.bmp",icount);
//						write_image(m_hImageCheck,"bmp",255,strEx);
//
//						HObject hRegion;
//						GenRegionContourXld(hSymbolXLDs, &hRegion, "filled");
//						strEx.Format("e:\\hRegion_%d.reg",icount);
//						write_region(hRegion,strEx);
//
//						strEx.Format("e:\\hDataCodeHandle_%d.dcm",icount);
//						write_data_code_2d_model(hDataCodeHandle,HTuple(strEx));
//
//						strEx.Format("e:\\hResultHandles_%d.tup",icount);
//						write_tuple(hResultHandles,HTuple(strEx));
//
//						strEx.Format("e:\\hDecodedDataStrings_%d.tup",icount);
//						write_tuple(hDecodedDataStrings,HTuple(strEx));
//						icount++;
//					//}
//				}
//				// �����ý��� [10/23/2015 TIAN]
				for (int i=0; i<strCode.GetLength();i++)
				{
					m_CheckResultCode.push_back(strCode.GetAt(i));
				}

				CString strPart;
				strPart = _T("");

				int nSize;
				nSize = m_CheckResultCode.size();
				if (m_bCheckPartCode)
				{
					int nCodeCharL=m_CodeCharL;
					int nCodeCharH=m_CodeCharH;

					if (nCodeCharL>nSize-1)//  ���ȷ����仯������Խ��[12/29/2016 zb]
					{
						nCodeCharL = nSize-1;
					}
					if (nCodeCharH>nSize-1)
					{
						nCodeCharH = nSize-1;
					}

					int ibyte;
					BOOL bFirst = TRUE;//�Ƿ��һ��������ֵ
					for (ibyte=nCodeCharL;ibyte<=nCodeCharH;ibyte++)
					{
						char code;
						code = m_CheckResultCode.at(ibyte);
						// ��������Ƿ��Ǻ��ֻ����������ַ����ɶԳ��ֵĸ�ֵ�������ģ�������������ַ�Ϊ����һ����ֵ [11/25/2015 dhl]
						if (code<0)
						{
							if (bFirst)//�ǵ�һ��������ֵ
							{
								if (ibyte+1<nCodeCharH+1)
								{
									char cNextCode = m_CheckResultCode.at(ibyte);
									if (cNextCode>=0)
									{
										code = '?';
										bErrorFlag = TRUE;
									}
								}
								else
								{
									code = '?';
									bErrorFlag = TRUE;
								}
								bFirst = FALSE;		
							}
							else
							{
								bFirst = TRUE;
							}
						}
						else
						{
							bFirst = TRUE;
						}
						strPart+=code;
						sAlgInnerCheckOutputResult->sErrorInfo.vcCheckResultCode.push_back(code);
					}
				}
				else
				{
					BOOL bFirst = TRUE;
					for (int kk=0;kk<nSize;kk++)
					{
						char code;
						code = m_CheckResultCode.at(kk);
						// ��������Ƿ��Ǻ��ֻ����������ַ����ɶԳ��ֵĸ�ֵ�������ģ�������������ַ�Ϊ����һ����ֵ [11/25/2015 dhl]
						if (code<0)
						{
							if (bFirst)
							{
								if (kk+1<nSize)
								{
									char cNextCode = m_CheckResultCode.at(kk);
									if (cNextCode>=0)
									{
										code = '?';
										bErrorFlag = TRUE;
									}
								}
								else
								{
									code = '?';
									bErrorFlag = TRUE;
								}
								bFirst = FALSE;		
							}
							else
							{
								bFirst = TRUE;
							}
						}
						else
						{
							bFirst = TRUE;
						}
						
						strPart+=code;
						sAlgInnerCheckOutputResult->sErrorInfo.vcCheckResultCode.push_back(code);
					}

				}
				if (pDataDst)
				{
					delete[]pDataDst;
				}
				if (pDataSrc)
				{
					delete[]pDataSrc;
				}

				//////////////////////////////////////////////////////////////////////////
				char pcExpectCode[MAXCODENUM];
				memset(pcExpectCode,0,MAXCODENUM);
				if (sAlgInnerCheckInputParam.sRegionProperty.bIsExistExpectCode)
				{
					int nSize = sAlgInnerCheckInputParam.sRegionProperty.vcExpectCode.size();						

					for(int nIndex=0;nIndex<nSize;nIndex++)
					{
						pcExpectCode[nIndex] = sAlgInnerCheckInputParam.sRegionProperty.vcExpectCode[nIndex];
					}
					if ((strcmp(pcExpectCode,strPart)==0||pcExpectCode[0]==0)
						&&bErrorFlag==FALSE)
					{

						;
					}
					else
					{
						sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = WRONGNUM_2D;//���
						//  [4/7/2017 zb]
						CString strContent = _T("");
						//11=���
						strContent.Format("%s",m_strMessageText[11]) ;
						sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
							"%s",strContent);
						return m_sStatus;
					}
				}
				else
				{
					if (bErrorFlag==TRUE)
					{
						sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = WRONGNUM_2D;//���
						//  [4/7/2017 zb]
						CString strContent = _T("");
						//11=���
						strContent.Format("%s",m_strMessageText[11]) ;
						sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
							"%s",strContent);
						return m_sStatus;
					}
					if(m_nResultNum>0&&m_nResultNum!=strPart.GetLength())
					{
						sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = WRONGNUM_2D;//���
						//  [4/7/2017 zb]
						CString strContent = _T("");
						//12=У����,���λ��
						strContent.Format("%s=%d",m_strMessageText[12],strPart.GetLength()) ;
						sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
							"%s",strContent);
						return m_sStatus;
					}
				}
				// �ޱ�������£�����ӡˢ�������ģ�� [9/13/2013 Denghl]
				int nRet=0;
				
				//AfxMessageBox("7");
				try
				{
					nRet = CheckPrint();
				}
				catch(HException &hException)
				{
					nRet = 8;
					if (m_bWriteLog)
					{
						CString strErr;
						strErr.Format("CheckPrint() Except-HALCON Except%s-%s",  hException.ProcName().Text(),hException.ErrorMessage().Text());
						GenCurLog(strErr,3);
						//FileOperate.WriteAlgExceptionLog(m_strLogPath, 
							//"�쳣��־", "�����", strErr);
					}
				}
				catch(...)
				{
					nRet = 8;
				}
				//AfxMessageBox("8");
				strAddInfo.Format("RC=%s,Version=%d,DJ=%d,MT=%d,Pattern=%d",
					m_htErrorC[0].S().Text(),m_htVersion[0].D(),m_htGrade[0].D(),m_htModeType[0].D(),m_htMask[0].D());
				sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cResvered,"%s",strAddInfo);

				if (nRet==1)
				{
					sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = OUTRANGE;//������������
					//  [4/7/2017 zb]
					CString strContent = _T("");
					//13=������������
					strContent.Format("%s",m_strMessageText[13]) ;
					sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
						"%s",strContent);
					return m_sStatus;
				}
				else if (nRet==2)
				{
					sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = INREGION;//����������ȱӡ
					//  [4/7/2017 zb]
					CString strContent = _T("");
					//14=����������ȱӡ
					strContent.Format("%s",m_strMessageText[14]) ;
					sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
						"%s",strContent);
					return m_sStatus;
				}
				else if (nRet==3)
				{
					sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = HORVER;//��ͷ����
					//  [4/7/2017 zb]
					CString strContent = _T("");
					//15=��ͷ����
					strContent.Format("%s",m_strMessageText[15]) ;
					sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
						"%s",strContent);
					return m_sStatus;
				}
				else if (nRet==4)
				{
					sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = QRLOCERR;//QR�붨λʧ��
					//  [4/7/2017 zb]
					CString strContent = _T("");
					//16=QR�붨λʧ��
					strContent.Format("%s",m_strMessageText[16]) ;
					sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
						"%s",strContent);
					return m_sStatus;
				}
				else if (nRet==5)
				{
					sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = ZOUBAN;//�߰�
					//  [4/7/2017 zb]
					CString strContent = _T("");
					//17=�߰�
					strContent.Format("%s",m_strMessageText[17]) ;
					sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
						"%s",strContent);
					return m_sStatus;
				}
				else if (nRet==6)
				{
					sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = ZOUBAN;//QR�붨λʧ��
					//  [4/7/2017 zb]
					CString strContent = _T("");
					//18=���ڲ���λģ��
					strContent.Format("%s",m_strMessageText[18]) ;
					sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
						"%s",strContent);
					return m_sStatus;
				}
				else if (nRet==7)
				{
					sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = ZOUBAN;//QR�붨λʧ��
					//  [4/7/2017 zb]
					CString strContent = _T("");
					//19=�ڲ���λʧ��
					strContent.Format("%s",m_strMessageText[19]) ;
					sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
						"%s",strContent);
					return m_sStatus;
				}				
				else if (nRet==8)
				{
					sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = PRINTTRY;//Print�쳣
					//  [4/7/2017 zb]
					CString strContent = _T("");
					//20=PrintCheck�쳣
					strContent.Format("%s",m_strMessageText[20]) ;
					sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
						"%s",strContent);
					return m_sStatus;
				}
				//21 \r\n�ȼ����:
				m_strRealGrade.Format(m_strMessageText[21]);

				if (m_sCheckPrintParam.sDengjiPara.bValid)
				{
					nRet = CheckGrade();
				}
				if (nRet==-1)
				{
					if (m_bWriteLog)
					{
						CString strErr;
						strErr.Format("CheckGrade()-Except");
						GenCurLog(strErr,3);
						//FileOperate.WriteAlgExceptionLog(m_strLogPath, 
							//"�쳣��־", "�����", strErr);
					}
					//////////////////////////////////////////////////////////////////////////
					sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = PRINTTRY;//Print�쳣
					//  [4/7/2017 zb]
					CString strContent = _T("");
					//22=GradeCheck�쳣
					strContent.Format("%s",m_strMessageText[22]) ;
					sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
						"%s",strContent);
					return m_sStatus;
				}
				else if (nRet>0)
				{
					sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = GRADELOW;//����ȼ���
					//  [4/7/2017 zb]
					//23 ����ȼ���
					CString strTmp;
					strTmp.Format("%s--%d/%d",m_strMessageText[23],m_nRealGrade,m_sCheckPrintParam.sDengjiPara.nSetGrade);
					sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
						"%s",strTmp);
					return m_sStatus;
				}
			}
			else
			{
				//  [4/7/2017 zb]
				CString strContent = _T("");
				//24=ʶ��ʧ�ܣ���鿴�������
				strContent.Format("%s",m_strMessageText[24]) ;
				sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
					"%s",strContent);
				sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = ABNORMAL_2D;
			}
		}
		else
		{
			//  [4/7/2017 zb]
			CString strContent = _T("");
			//25=ʶ��ʧ�ܣ�û����������ģ��
			strContent.Format("%s",m_strMessageText[25]) ;
			sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
				"%s",strContent);
			sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = ABNORMAL_2D;
		}
		return m_sStatus;
	}
	catch(HException &hException)
	{
		if (m_bWriteLog)
		{
			CString strErr;
			strErr.Format("CInnerCheck() Except-HALCON Except%s-%s",  hException.ProcName().Text(),hException.ErrorMessage().Text());
			GenCurLog(strErr,3);
			//FileOperate.WriteAlgExceptionLog(m_strLogPath, 
				//"�쳣��־", "�����", strErr);
		}
		sprintf_s(m_sStatus.chErrorContext,"AlgExcept");
		sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
			"%s", "Identification of failure, normal template is not established");
		sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = ABNORMAL_2D;
		return m_sStatus;
	}
	catch(...)
	{
		if (m_bWriteLog)
		{
			CString strContent;
			strContent.Format("%s","CInnerCheck()-AlgExcept");
			GenCurLog(strContent,3);
			//FileOperate.WriteAlgExceptionLog(m_strLogPath,"�쳣��־","CInnerCheck()��","�㷨�쳣");
		}
		sprintf_s(m_sStatus.chErrorContext,"AlgExcept");
		sprintf_s(sAlgInnerCheckOutputResult->sErrorInfo.cErrorDescription,
			"%s", "Identification of failure, normal template is not established");
		sAlgInnerCheckOutputResult->sErrorInfo.bytErrorType = ABNORMAL_2D;
		return m_sStatus;
	}	
	//AfxMessageBox("9");
}

int CInnerCheck::GenCheckImage(const s_InnerCheckInputParam& sAlgInnerCheckInputParam,const HObject hSrcImage)
{
	int nResult = 0;

	try
	{
		//WriteObject(hSrcImage,"d://Src0.obj");
	//	HObject hImageSrc;
		HObject hCheckRegion;
		GenRectangle1(&hCheckRegion,
			m_rcAffineTransReg.top,
			m_rcAffineTransReg.left,
			m_rcAffineTransReg.bottom,
			m_rcAffineTransReg.right);
		HTuple htLen;
		CountObj(hSrcImage,&htLen);
		//  [12/12/2016 zb] ��ͼ������������
		HObject hSrcRegion;
		GenEmptyRegion(&hSrcRegion);
		GetDomain(hSrcImage,&hSrcRegion);
		Intersection(hCheckRegion,hSrcRegion,&hCheckRegion);
		//  [3/20/2017 zb]
		HTuple hNumCheckRegion; 
		HObject hCheckRegions;
		Connection(hCheckRegion,&hCheckRegions);
		SelectShape(hCheckRegions,&hCheckRegions,"area","and",1,9999999);
		CountObj(hCheckRegions,&hNumCheckRegion);
		if (hNumCheckRegion[0].I()==0)
		{
			nResult = 3;// �쳣
			CString strErr;
			HTuple htLen,hImageWidth,hImageHeight;
			CountObj(hSrcImage,&htLen);
			GetImageSize(hSrcImage,&hImageWidth,&hImageHeight);
			if (htLen[0].I()==3)
			{
				strErr.Format("GenCheckImage()-CheckRegion-hCheckRegion Null!");
				GenCurLog(strErr,3);
				strErr.Format("m_rcAffineTransReg:top-%d,bottom-%d,right-%d,left-%d,hSrcImage channel-%d,hSrcImage width-%d-%d-%d,hSrcImage height-%d-%d-%d",
					m_rcAffineTransReg.top,m_rcAffineTransReg.bottom,m_rcAffineTransReg.right,m_rcAffineTransReg.left,htLen[0].I(),hImageWidth[0].I(),hImageWidth[1].I(),hImageWidth[2].I(),hImageHeight[0].I(),hImageHeight[1].I(),hImageHeight[2].I());	
			} 
			else
			{
				strErr.Format("GenCheckImage()-CheckRegion-hCheckRegion Null!");
				GenCurLog(strErr,3);
				strErr.Format("m_rcAffineTransReg:top-%d,bottom-%d,right-%d,left-%d,hSrcImage channel-%d,hSrcImage width-%d,hSrcImage height-%d",
					m_rcAffineTransReg.top,m_rcAffineTransReg.bottom,m_rcAffineTransReg.right,m_rcAffineTransReg.left,htLen[0].I(),hImageWidth[0].I(),hImageHeight[0].I());
			}
			GenCurLog(strErr,3);
			return nResult;
		}


		if (sAlgInnerCheckInputParam.sAlgImageInfo.nBits == 8)
		{		
			ReduceDomain(hSrcImage,hCheckRegion,&m_hImageCheck);
			CropDomain(m_hImageCheck,&m_hImageCheck);
		}
		else if(m_nChannel==0)
		{
			HObject hImgR, hImgG, hImgB,hImgTmp,m_hImageCheckR,m_hImageCheckG,m_hImageCheckB;
			SelectObj(hSrcImage,&hImgR,1);
			ReduceDomain(hImgR,hCheckRegion,&m_hImageCheckR);
			CropDomain(m_hImageCheckR,&m_hImageCheckR);
			SelectObj(hSrcImage,&hImgG,2);
			ReduceDomain(hImgG,hCheckRegion,&m_hImageCheckG);
			CropDomain(m_hImageCheckG,&m_hImageCheckG);
			SelectObj(hSrcImage,&hImgB,3);		
			ReduceDomain(hImgB,hCheckRegion,&m_hImageCheckB);
			CropDomain(m_hImageCheckB,&m_hImageCheckB);
			//Compose3(m_hImageCheckR, m_hImageCheckG, m_hImageCheckB, &m_hImageCheck);	
			GenEmptyObj(&m_hImageCheck);
			ConcatObj(m_hImageCheckR,m_hImageCheckG,&m_hImageCheck);
			ConcatObj(m_hImageCheck,m_hImageCheckB,&m_hImageCheck);
		}
		else if(m_nChannel==1)
		{			
			HObject hSelImg;
			SelectObj(hSrcImage,&hSelImg,1);
			ReduceDomain(hSelImg,hCheckRegion,&m_hImageCheck);
			CropDomain(m_hImageCheck,&m_hImageCheck);
		}
		else if(m_nChannel==2)
		{
			HObject hSelImg;
			SelectObj(hSrcImage,&hSelImg,2);
			ReduceDomain(hSelImg,hCheckRegion,&m_hImageCheck);
			CropDomain(m_hImageCheck,&m_hImageCheck);
		}
		else if (m_nChannel==4)
		{
			HObject hImgR, hImgG, hImgB,hImgTmp,hImageCheckR,hImageCheckG,hImageCheckB;
			SelectObj(hSrcImage,&hImgR,1);
			ReduceDomain(hImgR,hCheckRegion,&hImageCheckR);
			CropDomain(hImageCheckR,&hImageCheckR);
			SelectObj(hSrcImage,&hImgG,2);
			ReduceDomain(hImgG,hCheckRegion,&hImageCheckG);
			CropDomain(hImageCheckG,&hImageCheckG);
			SelectObj(hSrcImage,&hImgB,3);		
			ReduceDomain(hImgB,hCheckRegion,&hImageCheckB);
			CropDomain(hImageCheckB,&hImageCheckB);
			/*HObject hMinImage;
			MinImage(hImageCheckR,hImageCheckG,&hMinImage);
			MinImage(hMinImage,hImageCheckB,&m_hImageCheck);*/
			Rgb3ToGray(hImageCheckR,hImageCheckG,hImageCheckB,&m_hImageCheck);
		}
		else
		{
			HObject hSelImg;
			SelectObj(hSrcImage,&hSelImg,3);
			ReduceDomain(hSelImg,hCheckRegion,&m_hImageCheck);
			CropDomain(m_hImageCheck,&m_hImageCheck);
		}
		//WriteObject(m_hImageCheck,"d://check.obj");
		//write_image(hImageSrc,"bmp",0,"d://123.bmp");

		//// ���ݷ���任���������ԭͼ����ROI���� [11/12/2011 Denghl]
		//m_sDstRegionProperty = sAlgInnerCheckInputParam.sRegionProperty;

		//CRect rcMinRect;// �������С��Ӿ��� [5/18/2011 Denghl]
		//std::list<CPoint>::iterator itRegionPoint;
		//if (m_sDstRegionProperty.lstRegionOrg.size()<=0)
		//{
		//	nResult = 2;
		//	CString str;
		//	str.Format("����ID=%dGenCheckImage()�����쳣");
		//	GenCurLog(str,3);
		//	//FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",str);	
		//}
		//itRegionPoint=m_sDstRegionProperty.lstRegionOrg.begin();
		//rcMinRect.top = rcMinRect.bottom = itRegionPoint->y;
		//rcMinRect.left = rcMinRect.right = itRegionPoint->x;
		//for(;itRegionPoint!=m_sDstRegionProperty.lstRegionOrg.end();
		//	itRegionPoint++)
		//{
		//	rcMinRect.top = min(rcMinRect.top,itRegionPoint->y);
		//	rcMinRect.bottom = max(rcMinRect.bottom,itRegionPoint->y);
		//	rcMinRect.left = min(rcMinRect.left,itRegionPoint->x);
		//	rcMinRect.right = max(rcMinRect.right,itRegionPoint->x);
		//}

		//if (sAlgInnerCheckInputParam.sAlgImageInfo.nImageType==0
		//	&&sAlgInnerCheckInputParam.sAlgImageInfo.sizeImage.cx==m_sAlgInitParam.nWidth
		//	&&sAlgInnerCheckInputParam.sAlgImageInfo.sizeImage.cy==m_sAlgInitParam.nHeight)
		//{
		//	// ƽ�Ʊ任���� [11/12/2011 Denghl]
		//	if (sAlgInnerCheckInputParam.sLocGroupResult.bHaveError&&m_sDstRegionProperty.pnLocateGroupID[m_bIsAgainCheck]>0)
		//	{
		//		//				nResult = 1;
		//		m_rcAffineTransReg.left = MAX(rcMinRect.left ,0);
		//		m_rcAffineTransReg.right = rcMinRect.right ;
		//		m_rcAffineTransReg.top = MAX(rcMinRect.top,0);
		//		m_rcAffineTransReg.bottom = rcMinRect.bottom ;
		//	}
		//	else
		//	{
		//		m_rcAffineTransReg.left = MAX(rcMinRect.left - (long)sAlgInnerCheckInputParam.sLocGroupResult.fColShiftLoc,0);
		//		m_rcAffineTransReg.right = rcMinRect.right - (long)sAlgInnerCheckInputParam.sLocGroupResult.fColShiftLoc;
		//		m_rcAffineTransReg.top = MAX(rcMinRect.top - (long)sAlgInnerCheckInputParam.sLocGroupResult.fLineShiftLoc,0);
		//		m_rcAffineTransReg.bottom = rcMinRect.bottom - (long)sAlgInnerCheckInputParam.sLocGroupResult.fLineShiftLoc;
		//	}

		//}
		//else
		//{
		//	m_rcAffineTransReg.left = 0;
		//	m_rcAffineTransReg.right = sAlgInnerCheckInputParam.sAlgImageInfo.sizeImage.cx-1;// -1 [10/12/2015 TIAN]
		//	m_rcAffineTransReg.top = 0;
		//	m_rcAffineTransReg.bottom = sAlgInnerCheckInputParam.sAlgImageInfo.sizeImage.cy-1;// -1 [10/12/2015 TIAN]

		//}
		//m_rcAffineTransReg = RECTBOUND(m_rcAffineTransReg,CRect(0,0,m_sAlgInitParam.nWidth-1,m_sAlgInitParam.nHeight-1));


		

		//write_image(m_hImageCheck,"bmp",0,"D:\\m_hImageCheck.bmp");

	}
	catch(HException &hException)
	{
		try
		{
			nResult = 3;// �쳣 [11/12/2011 Denghl]
			CString strErr;
			HTuple htLen,hImageWidth,hImageHeight;
			CountObj(hSrcImage,&htLen);
			GetImageSize(hSrcImage,&hImageWidth,&hImageHeight);
			if (htLen[0].I()==3)
			{
				//strErr.Format("CInnerCheck()�쳣-HALCON�쳣-%s",  hException.ProcName().Text(),hException.ErrorMessage().Text());
				strErr.Format("CInnerCheck() Except-HALCON Except:%s-%s,m_rcAffineTransReg:top-%d,bottom-%d,right-%d,left-%d,hSrcImage channel-%d,hSrcImage width-%d-%d-%d,hSrcImage height-%d-%d-%d",  hException.ProcName().Text(),hException.ErrorMessage().Text(),
					m_rcAffineTransReg.top,m_rcAffineTransReg.bottom,m_rcAffineTransReg.right,m_rcAffineTransReg.left,htLen[0].I(),hImageWidth[0].I(),hImageWidth[1].I(),hImageWidth[2].I(),hImageHeight[0].I(),hImageHeight[1].I(),hImageHeight[2].I());
			} 
			else
			{
				strErr.Format("CInnerCheck() Except-HALCON Except:%s-%s,m_rcAffineTransReg:top-%d,bottom-%d,right-%d,left-%d,hSrcImage channel-%d,hSrcImage width-%d,hSrcImage height-%d",  hException.ProcName().Text(),hException.ErrorMessage().Text(),
					m_rcAffineTransReg.top,m_rcAffineTransReg.bottom,m_rcAffineTransReg.right,m_rcAffineTransReg.left,htLen[0].I(),hImageWidth[0].I(),hImageHeight[0].I());
			}
			GenCurLog(strErr,3);
			//FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",strErr);	
		}
		catch(HException &hException)
		{	
			CString strErr;
			strErr.Format("GenCheckImage-catch1 Except-HALCON Except%-%s",  hException.ProcName().Text(),hException.ErrorMessage().Text());
			GenCurLog(strErr,3);
			//FileOperate.WriteAlgExceptionLog(m_strLogPath, 
			//"�쳣��־", "�����", strErr);
		}
		catch(...)
		{
			CString strContent;
			strContent.Format("%s","CInnerCheck()-GenCheckImage-catch1 Except-AlgExcept");
			GenCurLog(strContent,3);
			//FileOperate.WriteAlgExceptionLog(m_strLogPath,"�쳣��־","CInnerCheck()��","�㷨�쳣");
		}	
	}
	catch (...)
	{
		nResult = 3;// �쳣 [11/12/2011 Denghl]
		CString str;
		str.Format("RegionID=%dGenCheckImage() Except",sAlgInnerCheckInputParam.sRegionProperty.nRegionID);
		GenCurLog(str,3);
		//FileOperate.WriteAlgExceptionLog(m_strLogPath, "�쳣��־", "�����",str);		
	}
	return nResult;

}


//******************************************************************************//
//����:ѧϰ�󱣴���ģ��
//����:
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CInnerCheck::SaveCheckModel(const char *strTaskName)
{
	//������Ҫ��Ӵ���
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
s_Status CInnerCheck::GetCheckModelImg(const UINT iTaskImgType, s_TaskImageInfo &sTaskImgInfo)
{
	//������Ҫ��Ӵ���
	return m_sStatus;

}
// д�����ļ�
void CInnerCheck::WritePara(CString strFileName)
{
	if (strFileName.GetLength()<=0)
	{
		strFileName = m_strParaIni;
	}
	char strTemp[100];
	sprintf(strTemp, "%d", m_iCheckMethod);
	WritePrivateProfileString("������", "��ⷽ��", strTemp, strFileName);

	sprintf(strTemp, "%s", m_strPolar);
	WritePrivateProfileString("������", "��ά�뼫��", strTemp, strFileName);

	sprintf(strTemp, "%d", m_nResultNum);
	WritePrivateProfileString("������", "�����У��λ��", strTemp, strFileName);

	sprintf(strTemp, "%d", m_bIsImageSmooth);
	WritePrivateProfileString("������", "ͼ��ƽ������", strTemp, strFileName);

	sprintf(strTemp, "%d", m_bIsZoomCheck);
	WritePrivateProfileString("������", "ͼ�����Ŵ���", strTemp, strFileName);

	sprintf(strTemp, "%f", m_fZoomRadio);
	WritePrivateProfileString("������", "���ű���", strTemp, strFileName);

	//sprintf(strTemp, "%f", m_fScaleHor);
	//WritePrivateProfileString("���Բ���", "����ֱ���", strTemp, strFileName);
	//sprintf(strTemp, "%f", m_fScaleVer);
	//WritePrivateProfileString("���Բ���", "����ֱ���", strTemp, strFileName);

	sprintf(strTemp, "%d", m_bCheckResultMode);
	WritePrivateProfileString("���Բ���", "�Ƿ��ȱ��Сͼ", strTemp, strFileName);
	//sprintf(strTemp, "%d", m_bAlignCorrected);
	//WritePrivateProfileString("���Բ���", "�Ƿ��λУ��", strTemp, strFileName);
	//sprintf(strTemp, "%d", m_nCorrectPixels);
	//WritePrivateProfileString("���Բ���","���У������ֵ", strTemp, strFileName);

	sprintf(strTemp, "%d", m_bCheckPartCode);
	WritePrivateProfileString("���Բ���", "�Ƿ�ʶ��ֲ�����", strTemp, strFileName);
	sprintf(strTemp, "%d", m_CodeCharL);
	WritePrivateProfileString("���Բ���","������ʾ��λ", strTemp, strFileName);
	sprintf(strTemp, "%d", m_CodeCharH);
	WritePrivateProfileString("���Բ���","������ʾ��λ", strTemp, strFileName);
	sprintf(strTemp, "%d", m_bIsTurnedResult);
	WritePrivateProfileString("���Բ���", "�Ƿ�ת��ʶ����", strTemp, strFileName);

	sprintf(strTemp, "%d", m_nChannel);
	WritePrivateProfileString("������", "��ɫͨ��", strTemp, strFileName);

	sprintf(strTemp, "%d", m_bCheckAllMod);
	WritePrivateProfileString("������", "�Ƿ�֧��ȫ��ģʽ", strTemp, strFileName);

	sprintf(strTemp, "%d", m_bCheckGao);
	WritePrivateProfileString("������", "�Ƿ��ʶ��ģʽ", strTemp, strFileName);

	// ���ӡˢ����������д [9/13/2013 Denghl]
	sprintf(strTemp, "%d", m_sCheckPrintParam.sOutRangPara.bValid);
	WritePrivateProfileString("������������", "�Ƿ���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sOutRangPara.nLeft);
	WritePrivateProfileString("������������", "��߾���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sOutRangPara.nRight);
	WritePrivateProfileString("������������", "�ұ߾���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sOutRangPara.nTop);
	WritePrivateProfileString("������������", "�ϱ߾���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sOutRangPara.nBottom);
	WritePrivateProfileString("������������", "�±߾���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sOutRangPara.nSingleArea);
	WritePrivateProfileString("������������", "���������", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sOutRangPara.nAllArea);
	WritePrivateProfileString("������������", "�����", strTemp, strFileName);			

	sprintf(strTemp, "%d", m_sCheckPrintParam.sInRegionPara.bValid);
	WritePrivateProfileString("����������ȱӡ", "�Ƿ���", strTemp, strFileName);	
	sprintf(strTemp, "%d", m_sCheckPrintParam.sInRegionPara.nDeviation);
	WritePrivateProfileString("����������ȱӡ", "�Ҷȷ���", strTemp, strFileName);	
	sprintf(strTemp, "%d", m_sCheckPrintParam.sInRegionPara.nMaxGray);
	WritePrivateProfileString("����������ȱӡ", "���Ҷ�", strTemp, strFileName);	

	sprintf(strTemp, "%d", m_sCheckPrintParam.sInHorverPara.bValid);
	WritePrivateProfileString("�����ڰ���ȱӡ", "�Ƿ���", strTemp, strFileName);	
	sprintf(strTemp, "%d", m_sCheckPrintParam.sInHorverPara.nDirect);
	WritePrivateProfileString("�����ڰ���ȱӡ", "����", strTemp, strFileName);	
	sprintf(strTemp, "%d", m_sCheckPrintParam.sInHorverPara.nArea);
	WritePrivateProfileString("�����ڰ���ȱӡ", "���", strTemp, strFileName);	
	sprintf(strTemp, "%d", m_sCheckPrintParam.sInHorverPara.nPercent);
	WritePrivateProfileString("�����ڰ���ȱӡ", "�ٷ�λ", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sInHorverPara.nContrast);
	WritePrivateProfileString("�����ڰ���ȱӡ", "�Աȶ�", strTemp, strFileName);	

	//sprintf(strTemp, "%d", m_sCheckPrintParam.sInHorverPara.bValid);
	//WritePrivateProfileString("�����ں�����", "�Ƿ���", strTemp, strFileName);	

	sprintf(strTemp, "%d", m_sCheckPrintParam.sQrlocShapePara.bValid);
	WritePrivateProfileString("QR�붨λ", "�Ƿ���", strTemp, strFileName);	

	// �ȼ����������� [12/26/2013 Denghl]
	sprintf(strTemp, "%d",m_bShowDJ);
	WritePrivateProfileString("�ȼ����", "�Ƿ���ʾ�ȼ�����", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sDengjiPara.bValid);
	WritePrivateProfileString("�ȼ����", "�Ƿ���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sDengjiPara.nSetGrade);
	WritePrivateProfileString("�ȼ����", "���ü���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sDengjiPara.nGBID);
	WritePrivateProfileString("�ȼ����", "��׼���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[0]);
	WritePrivateProfileString("�ȼ����", "ѡ��1���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[1]);
	WritePrivateProfileString("�ȼ����", "ѡ��2���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[2]);
	WritePrivateProfileString("�ȼ����", "ѡ��3���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[3]);
	WritePrivateProfileString("�ȼ����", "ѡ��4���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[4]);
	WritePrivateProfileString("�ȼ����", "ѡ��5���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[5]);
	WritePrivateProfileString("�ȼ����", "ѡ��6���", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[6]);
	WritePrivateProfileString("�ȼ����", "ѡ��7���", strTemp, strFileName);

	//  [1/6/2015 baiyy]
	sprintf(strTemp, "%d", m_sCheckPrintParam.sCorrPixelParam.bValid);
	WritePrivateProfileString("��λУ��", "�Ƿ�У��", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sCorrPixelParam.nCorrPixels);
	WritePrivateProfileString("��λУ��", "���У������ֵ", strTemp, strFileName);

	// ��ʱ�������� 
	sprintf(strTemp, "%d", m_sCheckPrintParam.sTimeOut.nSingleTime);
	WritePrivateProfileString("��ʱ����", "��ö���ʱ��", strTemp, strFileName);	

	// �߰�������� [7/31/2014 Denghl]
	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.bValid);
	WritePrivateProfileString("�����߰�", "�Ƿ���", strTemp, strFileName);	
	
	//int a  = strFileName.ReverseFind('\\');
	//CString str;
	//str = strFileName.Left(a);
	//str += "\\ZouBanRegion.reg";
	//BOOL bZouBanFlag = FileOperate.IsFileExist(str);	
	//write_region(m_hZoubanRegion,str);
	/////
	//sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.bIsInLoc);
	//WritePrivateProfileString("�����߰�", "�Ƿ��ڲ���λ", strTemp, strFileName);
	//if (0)//m_sCheckPrintParam.sZouBanPara.bIsInLoc//ʹ���ڲ���λ��������ز�����ģ��
	//{
	//	str = strFileName.Left(a);
	//	CString strT;
	//	strT =str+ "\\InLocMod.mod";
	//	if (m_lMarkID>=0)
	//	{
	//		write_ncc_model(m_lMarkID,strT);
	//	}
	//	strT =str+ "\\InLocMark.reg";
	//	write_region(m_hInlocMarkRegion,strT);
	//	///
	//	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.nSearchX);
	//	WritePrivateProfileString("�����߰�", "ˮƽ������Χ", strTemp, strFileName);
	//	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.nSearchY);
	//	WritePrivateProfileString("�����߰�", "��ֱ������Χ", strTemp, strFileName);
	//}
	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan);
	WritePrivateProfileString("�����߰�", "�Ƿ�׿����߰�", strTemp, strFileName);

	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.bLRChangeZouban);
	WritePrivateProfileString("�����߰�", "���Ұ׿����Ƿ��޸�", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.bUDChangeZouban);
	WritePrivateProfileString("�����߰�", "���°׿����Ƿ��޸�", strTemp, strFileName);

	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft);
	WritePrivateProfileString("�����߰�", "�Ƿ�����߽�", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight);
	WritePrivateProfileString("�����߰�", "�Ƿ����ұ߽�", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp);
	WritePrivateProfileString("�����߰�", "�Ƿ����ϱ߽�", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown);
	WritePrivateProfileString("�����߰�", "�Ƿ����±߽�", strTemp, strFileName);
	
	sprintf(strTemp, "%f", m_sCheckPrintParam.sZouBanPara.m_fZouBanStdHor);
	WritePrivateProfileString("�����߰�", "�߽�����׼", strTemp, strFileName);
	
	sprintf(strTemp, "%f", m_sCheckPrintParam.sZouBanPara.m_fZouBanStdVer);
	WritePrivateProfileString("�����߰�", "�߽������׼", strTemp, strFileName);
	
	sprintf(strTemp, "%f", m_sCheckPrintParam.sZouBanPara.m_fZouBanRange);
	WritePrivateProfileString("�����߰�", "�߽繫��", strTemp, strFileName);
	
	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_nLeftPos);
	WritePrivateProfileString("�����߰�", "��߽�λ��", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_nRightPos);
	WritePrivateProfileString("�����߰�", "�ұ߽�λ��", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_nUpPos);
	WritePrivateProfileString("�����߰�", "�ϱ߽�λ��", strTemp, strFileName);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_nDownPos);
	WritePrivateProfileString("�����߰�", "�±߽�λ��", strTemp, strFileName);

	//д�����Ƿ��޸��ļ�
	if (strFileName == m_strParaIni)
	{
		SaveTempPara();
	}
}
void CInnerCheck::WriteTypePara()
{
	//CString str;
	//str.Format("%d",m_nNoteType);
	//WritePrivateProfileString("ϵͳ����","��Ʒ����",str,m_strRMBConfig);
}

// дĬ�ϲ����ļ�
void CInnerCheck::WriteAddDefaultPara()
{
	// ������Ҫ��Ӵ���
	//��Ӱٷ�λ�������Ĭ�ϲ���
	if (m_sCheckPrintParam.sInHorverPara.nPercent == 70)  //Ĭ��ֵ
	{
		WritePrivateProfileString("�����ڰ���ȱӡ", "�ٷ�λ", "70", m_strDefaultParaIni);
	}
	//��Ӷ�ά�뼫�Բ�������Ĭ�ϲ���
	if (m_strPolar == _T("dark_on_light"))
	{
		WritePrivateProfileString("������", "��ά�뼫��", "dark_on_light", m_strDefaultParaIni);
	}
	//���ͼ��ƽ�������������Ĭ�ϲ���
	if (m_bIsImageSmooth == 0)
	{
		WritePrivateProfileString("������", "ͼ��ƽ������", "0", m_strDefaultParaIni);
	}
	//���ͼ��ѹ�������������Ĭ�ϲ���
	if (m_bIsZoomCheck == 0)
	{
		WritePrivateProfileString("������", "ͼ�����Ŵ���", "0", m_strDefaultParaIni);
	}
	if (m_fZoomRadio == 2)
	{
		WritePrivateProfileString("������", "���ű���", "2", m_strDefaultParaIni);
	}
	//������һ����°׿����Ƿ��޸Ĳ�������Ĭ�ϲ���
	if (m_sCheckPrintParam.sZouBanPara.bLRChangeZouban == 0)
	{
		WritePrivateProfileString("�����߰�", "���Ұ׿����Ƿ��޸�", "0", m_strDefaultParaIni);
	}
	if (m_sCheckPrintParam.sZouBanPara.bUDChangeZouban == 0)
	{
		WritePrivateProfileString("�����߰�", "���°׿����Ƿ��޸�", "0", m_strDefaultParaIni);
	}
}

void CInnerCheck::FreeCheckMethod()
{
	if (m_bReadMode)
	{
		if (m_iCheckMethod!=3)
		{
			ClearDataCode2dModel(hDataCodeHandle);
			m_bReadMode = FALSE;
		}
		else
		{
			for (int i=0;i<3;i++)
			{
				ClearDataCode2dModel(phDataCodeHandle[i]);
			}			
			m_bReadMode = FALSE;
		}

	}	
}
void CInnerCheck::MallocCheckMethod()
{
	if(m_iCheckMethod==0)
	{
		CreateDataCode2dModel("Data Matrix ECC 200", HTuple(), HTuple(), &hDataCodeHandle);
		if (m_bCheckGao)
		{
			SetDataCode2dParam(hDataCodeHandle, "default_parameters", "enhanced_recognition");
		}
		else
		{
			SetDataCode2dParam(hDataCodeHandle, "default_parameters", "standard_recognition");
		}
		if (m_sCheckPrintParam.sTimeOut.bValid)
		{
			//��ʱ���Σ����������һ��
			//SetDataCode2dParam(hDataCodeHandle,"timeout",m_sCheckPrintParam.sTimeOut.nSingleTime);
		}
		//SetDataCode2dParam(hDataCodeHandle,"contrast_min",25);
		SetDataCode2dParam(hDataCodeHandle,"polarity",HTuple(m_strPolar));
		m_bReadMode = TRUE;
	}
	else if (m_iCheckMethod==1)
	{
		CreateDataCode2dModel("PDF417", HTuple(), HTuple(), &hDataCodeHandle);
		if (m_bCheckGao)
		{
			SetDataCode2dParam(hDataCodeHandle, "default_parameters", "enhanced_recognition");
		}
		else
		{
			SetDataCode2dParam(hDataCodeHandle, "default_parameters", "standard_recognition");
		}
		if (m_sCheckPrintParam.sTimeOut.bValid)
		{
			//��ʱ���Σ����������һ��
			//SetDataCode2dParam(hDataCodeHandle,"timeout",m_sCheckPrintParam.sTimeOut.nSingleTime);
		}
		//SetDataCode2dParam(hDataCodeHandle,"contrast_min",25);
		SetDataCode2dParam(hDataCodeHandle,"polarity",HTuple(m_strPolar));
		m_bReadMode = TRUE;
	}
	else if (m_iCheckMethod==2)
	{
		CreateDataCode2dModel("QR Code", HTuple(), HTuple(), &hDataCodeHandle);
		if (m_bCheckGao)
		{
			SetDataCode2dParam(hDataCodeHandle, "default_parameters", "enhanced_recognition");
		}
		else
		{
			SetDataCode2dParam(hDataCodeHandle, "default_parameters", "standard_recognition");
		}
		if (m_sCheckPrintParam.sTimeOut.bValid)
		{
			//��ʱ���Σ����������һ��
			//SetDataCode2dParam(hDataCodeHandle,"timeout",m_sCheckPrintParam.sTimeOut.nSingleTime);
		}
		//SetDataCode2dParam(hDataCodeHandle,"contrast_min",25);//-zb0906
		//SetDataCode2dParam(hDataCodeHandle,"polarity","dark_on_light");
		SetDataCode2dParam(hDataCodeHandle,"polarity",HTuple(m_strPolar));
		//SetDataCode2dParam(hDataCodeHandle,"pr_mode","low");//-zb0906
		m_bReadMode = TRUE;
	}
	else if (m_iCheckMethod==3)
	{
		CreateDataCode2dModel("Data Matrix ECC 200", HTuple(), HTuple(), &phDataCodeHandle[0]);
		CreateDataCode2dModel("PDF417", HTuple(), HTuple(), &phDataCodeHandle[1]);
		CreateDataCode2dModel("QR Code", HTuple(), HTuple(), &phDataCodeHandle[2]);
		if (m_bCheckGao)
		{
			SetDataCode2dParam(phDataCodeHandle[0], "default_parameters", "enhanced_recognition");	
		//	SetDataCode2dParam(phDataCodeHandle[0],"contrast_min",25);
			SetDataCode2dParam(phDataCodeHandle[0],"polarity",HTuple(m_strPolar));

			SetDataCode2dParam(phDataCodeHandle[1], "default_parameters", "enhanced_recognition");	
		//	SetDataCode2dParam(phDataCodeHandle[1],"contrast_min",25);
			SetDataCode2dParam(phDataCodeHandle[1],"polarity",HTuple(m_strPolar));

			SetDataCode2dParam(phDataCodeHandle[2], "default_parameters", "enhanced_recognition");
		//	SetDataCode2dParam(phDataCodeHandle[2],"contrast_min",25);
			SetDataCode2dParam(phDataCodeHandle[2],"polarity",HTuple(m_strPolar));
		}
		else
		{
			SetDataCode2dParam(phDataCodeHandle[0], "default_parameters", "standard_recognition");
		//	SetDataCode2dParam(phDataCodeHandle[0],"contrast_min",25);
			SetDataCode2dParam(phDataCodeHandle[0],"polarity",HTuple(m_strPolar));

			SetDataCode2dParam(phDataCodeHandle[1], "default_parameters", "standard_recognition");	
		//	SetDataCode2dParam(phDataCodeHandle[1],"contrast_min",25);
			SetDataCode2dParam(phDataCodeHandle[1],"polarity",HTuple(m_strPolar));

			SetDataCode2dParam(phDataCodeHandle[2], "default_parameters", "standard_recognition");
		//	SetDataCode2dParam(phDataCodeHandle[2],"contrast_min",25);
			SetDataCode2dParam(phDataCodeHandle[2],"polarity",HTuple(m_strPolar));
		}	
		if (m_sCheckPrintParam.sTimeOut.bValid)
		{
			//��ʱ���Σ����������һ��
			//SetDataCode2dParam(phDataCodeHandle[0],"timeout",m_sCheckPrintParam.sTimeOut.nSingleTime);
			//SetDataCode2dParam(phDataCodeHandle[1],"timeout",m_sCheckPrintParam.sTimeOut.nSingleTime);
			//SetDataCode2dParam(phDataCodeHandle[2],"timeout",m_sCheckPrintParam.sTimeOut.nSingleTime);

		}

		m_bReadMode = TRUE;		
	}
	else
	{
		m_bReadMode = FALSE;		
	}

}


// �������ļ�
void CInnerCheck::ReadPara(CString strParaIni)
{
	// ������Ҫ��Ӵ���
	char strTemp[100];
	memset(strTemp,0,100);
	GetPrivateProfileString("������", "��ⷽ��","3", strTemp, sizeof(strTemp), strParaIni);
	m_iCheckMethod = atoi(strTemp);
	/*if (m_sDstRegionProperty.nInnerAlgType==1)
	{
		m_iCheckMethod = 2;
	}
	else if (m_sDstRegionProperty.nInnerAlgType==2)
	{
		m_iCheckMethod = 0;
	}
	else if (m_sDstRegionProperty.nInnerAlgType==3)
	{
		m_iCheckMethod = 1;
	}
	else if (m_sDstRegionProperty.nInnerAlgType==4)
	{
		m_iCheckMethod = 3;
	}*/

	//GetPrivateProfileString("���Բ���", "����ֱ���","0.05", strTemp, sizeof(strTemp), strParaIni);
	//m_fScaleHor = atof(strTemp);
	
	//GetPrivateProfileString("���Բ���", "����ֱ���","0.05", strTemp, sizeof(strTemp), strParaIni);
	//m_fScaleVer = atof(strTemp);


	//  [12/2/2014 baiyy]
	GetPrivateProfileString("���Բ���", "�Ƿ��ȱ��Сͼ", "1", strTemp, sizeof(strTemp), strParaIni);
	m_bCheckResultMode = atoi(strTemp);

	//GetPrivateProfileString("���Բ���", "�Ƿ��λУ��", "0", strTemp, sizeof(strTemp), strParaIni);
	//m_bAlignCorrected = atoi(strTemp);

	//GetPrivateProfileString("���Բ���", "���У������ֵ", "0", strTemp, sizeof(strTemp), strParaIni);
	//m_nCorrectPixels = atoi(strTemp);

	GetPrivateProfileString("���Բ���", "�Ƿ�ʶ��ֲ�����", "0", strTemp, sizeof(strTemp), strParaIni);
	m_bCheckPartCode = atoi(strTemp);
	GetPrivateProfileString("���Բ���","������ʾ��λ", "0", strTemp, sizeof(strTemp), strParaIni);
	m_CodeCharL = atoi(strTemp);
	GetPrivateProfileString("���Բ���","������ʾ��λ", "1024", strTemp, sizeof(strTemp), strParaIni);
	m_CodeCharH = atoi(strTemp);
	GetPrivateProfileString("���Բ���", "�Ƿ�ת��ʶ����", "0", strTemp, sizeof(strTemp), strParaIni);
	m_bIsTurnedResult = atoi(strTemp);


	GetPrivateProfileString("������", "��ɫͨ��","0", strTemp, sizeof(strTemp), strParaIni);
	m_nChannel = atoi(strTemp);
	GetPrivateProfileString("������", "�Ƿ�֧��ȫ��ģʽ","0", strTemp, sizeof(strTemp), strParaIni);
	m_bCheckAllMod = atoi(strTemp);	
	GetPrivateProfileString("������", "�Ƿ��ʶ��ģʽ","1", strTemp, sizeof(strTemp), strParaIni);
	m_bCheckGao = atoi(strTemp);
	GetPrivateProfileString("������", "��ά�뼫��","dark_on_light", strTemp, sizeof(strTemp), strParaIni);
	m_strPolar.Format("%s",strTemp);
	GetPrivateProfileString("������", "ͼ��ƽ������","0", strTemp, sizeof(strTemp), strParaIni);
	m_bIsImageSmooth = atoi(strTemp);	
	GetPrivateProfileString("������", "ͼ�����Ŵ���","0", strTemp, sizeof(strTemp), strParaIni);
	m_bIsZoomCheck = atoi(strTemp);	
	GetPrivateProfileString("������", "���ű���","2", strTemp, sizeof(strTemp), strParaIni);
	m_fZoomRadio= atof(strTemp);
	GetPrivateProfileString("��ʱ����", "��ö���ʱ��","10", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sTimeOut.nSingleTime = atoi(strTemp);

	GetPrivateProfileString("������", "�����У��λ��","0", strTemp, sizeof(strTemp), strParaIni);
	m_nResultNum = atoi(strTemp);


	if (m_bReadMode)
	{
		if (m_iCheckMethod!=3)
		{
			ClearDataCode2dModel(hDataCodeHandle);
			m_bReadMode = FALSE;
		}
		else
		{
			for (int i=0;i<3;i++)
			{
				ClearDataCode2dModel(phDataCodeHandle[i]);
			}			
			m_bReadMode = FALSE;
		}		
	}
	if(m_iCheckMethod==0)
	{
		CreateDataCode2dModel("Data Matrix ECC 200", HTuple(), HTuple(), &hDataCodeHandle);
		if (m_bCheckGao)
		{
			SetDataCode2dParam(hDataCodeHandle, "default_parameters", "maximum_recognition");
		}
		else
		{
			SetDataCode2dParam(hDataCodeHandle, "default_parameters", "standard_recognition");
		}
		if (m_sCheckPrintParam.sTimeOut.bValid)
		{
			//��ʱ���Σ����������һ��
			//SetDataCode2dParam(hDataCodeHandle,"timeout",m_sCheckPrintParam.sTimeOut.nSingleTime);
		}
	//	SetDataCode2dParam(hDataCodeHandle,"contrast_min",25);
		//SetDataCode2dParam(hDataCodeHandle,"polarity","dark_on_light");
		SetDataCode2dParam(hDataCodeHandle,"polarity",HTuple(m_strPolar));
		m_bReadMode = TRUE;
	}
	else if (m_iCheckMethod==1)
	{
		CreateDataCode2dModel("PDF417", HTuple(), HTuple(), &hDataCodeHandle);
		if (m_bCheckGao)
		{
			SetDataCode2dParam(hDataCodeHandle, "default_parameters", "enhanced_recognition");
		}
		else
		{
			SetDataCode2dParam(hDataCodeHandle, "default_parameters", "standard_recognition");
		}
		if (m_sCheckPrintParam.sTimeOut.bValid)
		{
			//��ʱ���Σ����������һ��
			//SetDataCode2dParam(hDataCodeHandle,"timeout",m_sCheckPrintParam.sTimeOut.nSingleTime);
		}
	//	SetDataCode2dParam(hDataCodeHandle,"contrast_min",25);
		//SetDataCode2dParam(hDataCodeHandle,"polarity","dark_on_light");
		SetDataCode2dParam(hDataCodeHandle,"polarity",HTuple(m_strPolar));
		m_bReadMode = TRUE;
	}
	else if (m_iCheckMethod==2)
	{
		CreateDataCode2dModel("QR Code", HTuple(), HTuple(), &hDataCodeHandle);
		if (m_bCheckGao)
		{
			SetDataCode2dParam(hDataCodeHandle, "default_parameters", "enhanced_recognition");
		}
		else
		{
			SetDataCode2dParam(hDataCodeHandle, "default_parameters", "standard_recognition");
		}
		if (m_sCheckPrintParam.sTimeOut.bValid)
		{
			//��ʱ���Σ����������һ��
			//SetDataCode2dParam(hDataCodeHandle,"timeout",m_sCheckPrintParam.sTimeOut.nSingleTime);
		}
		//SetDataCode2dParam(hDataCodeHandle,"contrast_min",25);//-zb0906
		//SetDataCode2dParam(hDataCodeHandle,"polarity","dark_on_light");
		SetDataCode2dParam(hDataCodeHandle,"polarity",HTuple(m_strPolar));
		//SetDataCode2dParam(hDataCodeHandle,"pr_mode","low");//-zb0906
		m_bReadMode = TRUE;
	}
	else if (m_iCheckMethod==3)
	{

		if (m_bCheckGao)
		{

			CreateDataCode2dModel("Data Matrix ECC 200", HTuple(), HTuple(), &phDataCodeHandle[0]);
			SetDataCode2dParam(phDataCodeHandle[0], "default_parameters", "enhanced_recognition");
		//	SetDataCode2dParam(phDataCodeHandle[0],"contrast_min",25);
			SetDataCode2dParam(phDataCodeHandle[0],"polarity",HTuple(m_strPolar));
			
			CreateDataCode2dModel("PDF417", HTuple(), HTuple(), &phDataCodeHandle[1]);
			SetDataCode2dParam(phDataCodeHandle[1], "default_parameters", "enhanced_recognition");	
		//	SetDataCode2dParam(phDataCodeHandle[1],"contrast_min",25);
			SetDataCode2dParam(phDataCodeHandle[1],"polarity",HTuple(m_strPolar));

			CreateDataCode2dModel("QR Code", HTuple(), HTuple(), &phDataCodeHandle[2]);				
			SetDataCode2dParam(phDataCodeHandle[2], "default_parameters", "enhanced_recognition");
	//		SetDataCode2dParam(phDataCodeHandle[2],"contrast_min",25);
			SetDataCode2dParam(phDataCodeHandle[2],"polarity",HTuple(m_strPolar));

		}
		else
		{

			CreateDataCode2dModel("Data Matrix ECC 200", HTuple(), HTuple(), &phDataCodeHandle[0]);
			SetDataCode2dParam(phDataCodeHandle[0], "default_parameters", "standard_recognition");
	//		SetDataCode2dParam(phDataCodeHandle[0],"contrast_min",25);
			SetDataCode2dParam(phDataCodeHandle[0],"polarity",HTuple(m_strPolar));

			CreateDataCode2dModel("PDF417", HTuple(), HTuple(), &phDataCodeHandle[1]);
			SetDataCode2dParam(phDataCodeHandle[1], "default_parameters", "standard_recognition");	
	//		SetDataCode2dParam(phDataCodeHandle[1],"contrast_min",25);
			SetDataCode2dParam(phDataCodeHandle[1],"polarity",HTuple(m_strPolar));

			CreateDataCode2dModel("QR Code", HTuple(), HTuple(), &phDataCodeHandle[2]);		
			SetDataCode2dParam(phDataCodeHandle[2], "default_parameters", "standard_recognition");
		//	SetDataCode2dParam(phDataCodeHandle[2],"contrast_min",25);
			SetDataCode2dParam(phDataCodeHandle[2],"polarity",HTuple(m_strPolar));

		}
		if (m_sCheckPrintParam.sTimeOut.bValid)
		{
			//��ʱ���Σ����������һ��
			//SetDataCode2dParam(phDataCodeHandle[0],"timeout",m_sCheckPrintParam.sTimeOut.nSingleTime);
			//SetDataCode2dParam(phDataCodeHandle[1],"timeout",m_sCheckPrintParam.sTimeOut.nSingleTime);
			//SetDataCode2dParam(phDataCodeHandle[2],"timeout",m_sCheckPrintParam.sTimeOut.nSingleTime);
		}
		m_bReadMode = TRUE;		
	}
	else
	{
		m_bReadMode = FALSE;		
	}		

	// ���ӡˢ����������д [9/13/2013 Denghl]
	GetPrivateProfileString("������������", "�Ƿ���","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sOutRangPara.bValid = atoi(strTemp);
	GetPrivateProfileString("������������", "��߾���","100", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sOutRangPara.nLeft = atoi(strTemp);
	GetPrivateProfileString("������������", "�ұ߾���","100", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sOutRangPara.nRight = atoi(strTemp);
	GetPrivateProfileString("������������", "�ϱ߾���","100", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sOutRangPara.nTop = atoi(strTemp);
	GetPrivateProfileString("������������", "�±߾���","100", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sOutRangPara.nBottom = atoi(strTemp);
	GetPrivateProfileString("������������", "���������","3", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sOutRangPara.nSingleArea = atoi(strTemp);
	GetPrivateProfileString("������������", "�����","10", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sOutRangPara.nAllArea = atoi(strTemp);

	GetPrivateProfileString("����������ȱӡ", "�Ƿ���","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sInRegionPara.bValid = atoi(strTemp);
	GetPrivateProfileString("����������ȱӡ", "�Ҷȷ���","5", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sInRegionPara.nDeviation = atoi(strTemp);
	GetPrivateProfileString("����������ȱӡ", "���Ҷ�","90", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sInRegionPara.nMaxGray = atoi(strTemp);

	GetPrivateProfileString("�����ڰ���ȱӡ", "�Ƿ���","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sInHorverPara.bValid = atoi(strTemp);
	GetPrivateProfileString("�����ڰ���ȱӡ", "����","1", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sInHorverPara.nDirect = atoi(strTemp);
	GetPrivateProfileString("�����ڰ���ȱӡ", "���","30", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sInHorverPara.nArea = atoi(strTemp);
	GetPrivateProfileString("�����ڰ���ȱӡ", "�ٷ�λ","70", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sInHorverPara.nPercent = atoi(strTemp);
	GetPrivateProfileString("�����ڰ���ȱӡ", "�Աȶ�","3", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sInHorverPara.nContrast = atoi(strTemp);

	//GetPrivateProfileString("�����ں�����", "�Ƿ���","0", strTemp, sizeof(strTemp), strParaIni);
	//m_sCheckPrintParam.sInHorverPara.bValid = atoi(strTemp);

	GetPrivateProfileString("QR�붨λ", "�Ƿ���","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sQrlocShapePara.bValid = atoi(strTemp);	

	//�ȼ���������д
	GetPrivateProfileString("�ȼ����", "�Ƿ���ʾ�ȼ�����","0", strTemp, sizeof(strTemp), strParaIni);
	m_bShowDJ = atoi(strTemp);

	GetPrivateProfileString("�ȼ����", "�Ƿ���","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sDengjiPara.bValid = atoi(strTemp);	
	GetPrivateProfileString("�ȼ����", "���ü���","1", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sDengjiPara.nSetGrade = atoi(strTemp);	
	GetPrivateProfileString("�ȼ����", "��׼���","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sDengjiPara.nGBID = atoi(strTemp);
	GetPrivateProfileString("�ȼ����", "ѡ��1���","1", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[0] = atoi(strTemp);
	GetPrivateProfileString("�ȼ����", "ѡ��2���","1", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[1] = atoi(strTemp);
	GetPrivateProfileString("�ȼ����", "ѡ��3���","1", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[2] = atoi(strTemp);
	GetPrivateProfileString("�ȼ����", "ѡ��4���","1", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[3] = atoi(strTemp);
	GetPrivateProfileString("�ȼ����", "ѡ��5���","1", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[4] = atoi(strTemp);
	GetPrivateProfileString("�ȼ����", "ѡ��6���","1", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[5] = atoi(strTemp);
	GetPrivateProfileString("�ȼ����", "ѡ��7���","1", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[6] = atoi(strTemp);

	GetPrivateProfileString("��λУ��", "�Ƿ�У��", "0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sCorrPixelParam.bValid = atoi(strTemp);
	GetPrivateProfileString("��λУ��", "���У������ֵ", "0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sCorrPixelParam.nCorrPixels = atoi(strTemp);

	// �߰������д [7/31/2014 Denghl]
	GetPrivateProfileString("�����߰�", "�Ƿ���","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.bValid = atoi(strTemp);
	if (m_sCheckPrintParam.sZouBanPara.bValid)
	{
		//int a  = strParaIni.ReverseFind('\\');
		//CString str;
		//str = strParaIni.Left(a);
		//str += "\\ZouBanRegion.reg";
		//BOOL bZouBanFlag = FileOperate.IsFileExist(str);
		//if (bZouBanFlag)
		//{
		//	read_region(&m_hZoubanRegion,str);
		//}
		//else
		//{
		//	GenEmptyRegion(&m_hZoubanRegion);
		//}
		//m_hRealZoubanRegion = m_hZoubanRegion;

		//GetPrivateProfileString("�����߰�", "�Ƿ��ڲ���λ","1", strTemp, sizeof(strTemp), strParaIni);
		//m_sCheckPrintParam.sZouBanPara.bIsInLoc = atoi(strTemp);		
		//if (0)//m_sCheckPrintParam.sZouBanPara.bIsInLoc
		//{
		//	GetPrivateProfileString("�����߰�", "ˮƽ������Χ","50", strTemp, sizeof(strTemp), strParaIni);
		//	m_sCheckPrintParam.sZouBanPara.nSearchX = atoi(strTemp);
		//	GetPrivateProfileString("�����߰�", "��ֱ������Χ","50", strTemp, sizeof(strTemp), strParaIni);
		//	m_sCheckPrintParam.sZouBanPara.nSearchY = atoi(strTemp);
		//	//////////////////////////////////////////////////////////////////////////
		//	str = strParaIni.Left(a);
		//	CString strT;
		//	strT =str+ "\\InLocMod.mod";
		//	if (m_lMarkID>=0)
		//	{
		//		ClearNccModel(m_lMarkID);
		//		m_lMarkID = -1;
		//	}
		//	bZouBanFlag = FileOperate.IsFileExist(strT);
		//	if (bZouBanFlag)
		//	{
		//		read_ncc_model(strT,&m_lMarkID);
		//	}
		//	else
		//	{
		//		m_lMarkID = -1;
		//	}

		//	strT =str+ "\\InLocMark.reg";
		//	bZouBanFlag = FileOperate.IsFileExist(strT);
		//	if (bZouBanFlag)
		//	{
		//		read_region(&m_hInlocMarkRegion,strT);
		//	}
		//	else
		//	{
		//		GenEmptyRegion(&m_hInlocMarkRegion);
		//	}

		//	DilationRectangle1(m_hInlocMarkRegion,&m_hInlocSearchRegion,
		//		m_sCheckPrintParam.sZouBanPara.nSearchX,
		//		m_sCheckPrintParam.sZouBanPara.nSearchY);
		//}
	}
	else //
	{
		//m_sCheckPrintParam.sZouBanPara.bIsInLoc = 0;		
		//GetPrivateProfileString("�����߰�", "ˮƽ������Χ","50", strTemp, sizeof(strTemp), strParaIni);
		//m_sCheckPrintParam.sZouBanPara.nSearchX = atoi(strTemp);
		//GetPrivateProfileString("�����߰�", "��ֱ������Χ","50", strTemp, sizeof(strTemp), strParaIni);
		//m_sCheckPrintParam.sZouBanPara.nSearchY = atoi(strTemp);
	}
	//  [11/3/2015 zl]��ֽ�߽��׼�߰���
	GetPrivateProfileString("�����߰�", "�Ƿ�׿����߰�","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan = atoi(strTemp);	

	GetPrivateProfileString("�����߰�", "���Ұ׿����Ƿ��޸�","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.bLRChangeZouban = atoi(strTemp);	
	GetPrivateProfileString("�����߰�", "���°׿����Ƿ��޸�","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.bUDChangeZouban = atoi(strTemp);	

	GetPrivateProfileString("�����߰�", "�Ƿ�����߽�","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft = atoi(strTemp);		
	GetPrivateProfileString("�����߰�", "�Ƿ����ұ߽�","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight = atoi(strTemp);		
	GetPrivateProfileString("�����߰�", "�Ƿ����ϱ߽�","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp = atoi(strTemp);		
	GetPrivateProfileString("�����߰�", "�Ƿ����±߽�","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown = atoi(strTemp);		

	GetPrivateProfileString("�����߰�", "�߽�����׼","1.5", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_fZouBanStdHor = atof(strTemp);		
	
	GetPrivateProfileString("�����߰�", "�߽������׼","1.5", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_fZouBanStdVer = atof(strTemp);		
	
	
	GetPrivateProfileString("�����߰�", "�߽繫��","0.5", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_fZouBanRange= atof(strTemp);		
	

	GetPrivateProfileString("�����߰�", "��߽�λ��","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_nLeftPos = atoi(strTemp);	
	GetPrivateProfileString("�����߰�", "�ұ߽�λ��","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_nRightPos = atoi(strTemp);	
	GetPrivateProfileString("�����߰�", "�ϱ߽�λ��","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_nUpPos = atoi(strTemp);	
	GetPrivateProfileString("�����߰�", "�±߽�λ��","0", strTemp, sizeof(strTemp), strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_nDownPos = atoi(strTemp);	
}


// �����Ѿ��޸��������ʱ�ļ� 
int CInnerCheck::SaveTempPara()
{
	CString sFileName, str;
	sFileName.Format("%s\\Com%dCam%d\\%s\\%d\\GCIAlgLab2DCodeCheckUpdateFile.ini", // ע���޸ĸ����ļ��� [6/24/2010 dhl]
		m_strTaskPath,
		m_sDstRegionProperty.nComputerID,
		m_sDstRegionProperty.nCameraID,
		m_strAlgName,
		m_sDstRegionProperty.nRegionID);

	str.Format("�����Ƿ��޸�");
	WritePrivateProfileString("�޸ı�־", str, "1", sFileName); 

	// ������Ҫ��Ӵ���
	return 0;
}
void CInnerCheck::ReadSimplePara()
{
	char strTemp[100];
	memset(strTemp,0,100);
	GetPrivateProfileString("������������", "�Ƿ���","0", strTemp, sizeof(strTemp), m_strParaIni);
	m_sCheckPrintParam.sOutRangPara.bValid = atoi(strTemp);
	GetPrivateProfileString("������������", "���������","3", strTemp, sizeof(strTemp), m_strParaIni);
	m_sCheckPrintParam.sOutRangPara.nSingleArea = atoi(strTemp);
	GetPrivateProfileString("������������", "�����","10", strTemp, sizeof(strTemp), m_strParaIni);
	m_sCheckPrintParam.sOutRangPara.nAllArea = atoi(strTemp);

	GetPrivateProfileString("����������ȱӡ", "�Ƿ���","0", strTemp, sizeof(strTemp), m_strParaIni);
	m_sCheckPrintParam.sInRegionPara.bValid = atoi(strTemp);
	GetPrivateProfileString("����������ȱӡ", "�Ҷȷ���","5", strTemp, sizeof(strTemp), m_strParaIni);
	m_sCheckPrintParam.sInRegionPara.nDeviation = atoi(strTemp);
	
	GetPrivateProfileString("�����ڰ���ȱӡ", "�Ƿ���","0", strTemp, sizeof(strTemp), m_strParaIni);
	m_sCheckPrintParam.sInHorverPara.bValid = atoi(strTemp);
	GetPrivateProfileString("�����ڰ���ȱӡ", "�ٷ�λ","70", strTemp, sizeof(strTemp), m_strParaIni);
	m_sCheckPrintParam.sInHorverPara.nPercent = atoi(strTemp);
	
	GetPrivateProfileString("�����߰�", "�߽繫��","0.5", strTemp, sizeof(strTemp), m_strParaIni);
	m_sCheckPrintParam.sZouBanPara.m_fZouBanRange= atof(strTemp);		
	GetPrivateProfileString("�����߰�", "�Ƿ���","0", strTemp, sizeof(strTemp), m_strParaIni);
	m_sCheckPrintParam.sZouBanPara.bValid = atoi(strTemp);


}
void CInnerCheck::WriteSimplePara()
{
	char strTemp[100];
	// ���ӡˢ����������д [9/13/2013 Denghl]
	sprintf(strTemp, "%d", m_sCheckPrintParam.sOutRangPara.bValid);
	WritePrivateProfileString("������������", "�Ƿ���", strTemp, m_strParaIni);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sOutRangPara.nSingleArea);
	WritePrivateProfileString("������������", "���������", strTemp, m_strParaIni);
	sprintf(strTemp, "%d", m_sCheckPrintParam.sOutRangPara.nAllArea);
	WritePrivateProfileString("������������", "�����", strTemp, m_strParaIni);			

	sprintf(strTemp, "%d", m_sCheckPrintParam.sInRegionPara.bValid);
	WritePrivateProfileString("����������ȱӡ", "�Ƿ���", strTemp, m_strParaIni);	
	sprintf(strTemp, "%d", m_sCheckPrintParam.sInRegionPara.nDeviation);
	WritePrivateProfileString("����������ȱӡ", "�Ҷȷ���", strTemp, m_strParaIni);	
	
	sprintf(strTemp, "%d", m_sCheckPrintParam.sInHorverPara.bValid);
	WritePrivateProfileString("�����ڰ���ȱӡ", "�Ƿ���", strTemp, m_strParaIni);	
	sprintf(strTemp, "%d", m_sCheckPrintParam.sInHorverPara.nPercent);
	WritePrivateProfileString("�����ڰ���ȱӡ", "�ٷ�λ", strTemp, m_strParaIni);
	// �߰�������� [7/31/2014 Denghl]
	sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.bValid);
	WritePrivateProfileString("�����߰�", "�Ƿ���", strTemp, m_strParaIni);	

	sprintf(strTemp, "%f", m_sCheckPrintParam.sZouBanPara.m_fZouBanRange);
	WritePrivateProfileString("�����߰�", "�߽繫��", strTemp, m_strParaIni);
	WritePrivateProfileString("Update_Simple","Flag","1",m_strParaIni);

	
}

// ��ȡ�Ѿ��޸��������ʱ�ļ�
int CInnerCheck::ReadTempPara()
{
	CString sFileName, str;
	sFileName.Format("%s\\Com%dCam%d\\%s\\%d\\GCIAlgLab2DCodeCheckUpdateFile.ini", // ע���޸ĸ����ļ��� [6/24/2010 dhl]
		m_strTaskPath,
		m_sDstRegionProperty.nComputerID,
		m_sDstRegionProperty.nCameraID,
		m_strAlgName,
		m_sDstRegionProperty.nRegionID);
	int nUpdateFlag = GetPrivateProfileInt("Update_Simple", "Flag",0, m_strParaIni);

	int nChanged = GetPrivateProfileInt("�޸ı�־", "�����Ƿ��޸�",0,sFileName);
	if (nChanged)
	{
		ReadPara(m_strParaIni);
		WritePrivateProfileString("�޸ı�־", "�����Ƿ��޸�","0",sFileName);
	}
	else if (nUpdateFlag)
	{
		ReadSimplePara();
	}
	WritePrivateProfileString("Update_Simple", "Flag", "0", m_strParaIni);


	return 0;
}

// �ͷ��ڴ�ռ�
void CInnerCheck::FreeTMPSpace()
{
	GenEmptyObj(&m_ImageRotate);
	GenEmptyObj(&m_RegionAffineTrans);
	GenEmptyObj(&m_InRegion);
	GenEmptyObj(&m_RegionOutRang);
	GenEmptyObj(&m_InRegionQue);
	GenEmptyObj(&m_RegionOutRect);		
	GenEmptyObj(&m_hImageCheck);
	GenEmptyObj(&hSymbolXLD);
	GenEmptyObj(&hSymbolXLDs);
	GenEmptyObj(&m_InWhiteLine);
	GenEmptyRegion(&m_hZoubanRegion);
}

// �����ڴ�ռ�
void CInnerCheck::RequistSpace()
{

}
int CInnerCheck::CheckGrade()
{
	int nRet = 0;
	HTuple hv_GradeValue;
	m_nRealGrade = -1;
	try
	{
		if (m_iCheckMethod==3)
		{
			if (m_iRealMethod==1||m_sCheckPrintParam.sDengjiPara.nGBID==0)
			{
				GetDataCode2dResults(phDataCodeHandle[m_iRealMethod], hResultHandles[0], "quality_isoiec15415", 
					&hv_GradeValue);
			}
			else
			{
				GetDataCode2dResults(phDataCodeHandle[m_iRealMethod], hResultHandles[0], "quality_aimdpm_1_2006", 
					&hv_GradeValue);
			}
		}
		else
		{
			if (m_iCheckMethod==1||m_sCheckPrintParam.sDengjiPara.nGBID==0)
			{
				GetDataCode2dResults(hDataCodeHandle, hResultHandles[0], "quality_isoiec15415", 
					&hv_GradeValue);
			}
			else
			{
				GetDataCode2dResults(hDataCodeHandle, hResultHandles[0], "quality_aimdpm_1_2006", 
					&hv_GradeValue);
			}

		}

	}
	catch(...)
	{
		// д�쳣��־ [12/27/2013 Denghl]
		nRet = -1;
		return nRet;
	}

	// �������� [12/27/2013 Denghl]	
	HTuple hLen =0;
	TupleLength(hv_GradeValue,&hLen);
	if (hLen>=8)
	{
		m_nRealGrade = 5;
		for (int i=1;i<8;i++)
		{
			if (m_sCheckPrintParam.sDengjiPara.pbDengjiOpt[i-1])
			{
				m_nRealGrade = min(m_nRealGrade,hv_GradeValue[i].I());
				CString str;
				str.Format("%d-%d/",i,hv_GradeValue[i].I());
				m_strRealGrade += str;
			}
		}
		if (m_nRealGrade<m_sCheckPrintParam.sDengjiPara.nSetGrade)// С�����õȼ���ʱ�򱨴� [12/27/2013 Denghl]
		{
			nRet = 1;
		}
	}
	else
	{
		nRet = -1;
		return nRet;
	}
	return nRet;
}
int CInnerCheck::ExWhiteEdge(HObject ho_Src,HTuple htWidth,int nEdgeType)
{
	HTuple htMean,htDeviation,htNum;
	HTuple Row12, Column12, Row22, Column22;
	HObject ho_SearchRange,htRegThresh,ho_ConnReg,ho_SelectedReg;
	int nLeftPos,nRightPos,nUpPos,nDownPos;
	nLeftPos=nRightPos=nUpPos=nDownPos=0;
	//write_image(ho_Src,"bmp",0,"D://ho_Src.bmp");
	Intensity (ho_Src, ho_Src, &htMean, &htDeviation);
	if(htMean[0].D()+0.3*htDeviation[0].D()<255)
	{
		Threshold (ho_Src, &htRegThresh, htMean[0].D()+0.3*htDeviation[0].D(), 255);
	}
	else
	{
		Threshold (ho_Src, &htRegThresh, 150, 255);
	}
	//write_region(htRegThresh,"D://htRegThresh.reg");
	Connection (htRegThresh, &ho_ConnReg);
	SelectShape (ho_ConnReg, &ho_SelectedReg, "width", "and",htWidth[0].D()*1.1, 99999);
	CountObj(ho_SelectedReg,&htNum);
	if(htNum[0].I()==1)
	{
		//SmallestRectangle1 (ho_SelectedReg, &Row12, &Column12, &Row22, &Column22);
		HObject hoBinImg;
		HTuple htHorProj,htVerProj,Width,Height;
		HTuple htIndexHor,htIndexVer,htLengthHor,hLengthVer;
		GetImageSize(ho_Src,&Width,&Height);
		RegionToBin(ho_SelectedReg,&hoBinImg,255,0,Width,Height);
		GrayProjections(hoBinImg,hoBinImg,"simple",&htHorProj,&htVerProj);
		TupleMin2(htHorProj,40,&htHorProj);
		TupleMin2(htVerProj,40,&htVerProj);
		TupleFind(htHorProj,40,&htIndexHor);
		TupleFind(htVerProj,40,&htIndexVer);
		TupleLength(htIndexHor,&htLengthHor);
		TupleLength(htIndexVer,&hLengthVer);
		if(htLengthHor[0].D()>0)
		{
			Row12=htIndexHor[0];
			Row22=htIndexHor[htLengthHor-1];
		}
		if(hLengthVer[0].D()>0)
		{
			Column12=htIndexVer[0];
			Column22=htIndexVer[hLengthVer-1];
		}

		if(nEdgeType==1)//��߽�
		{
			if(Column12[0].D()>0)
			{
				nLeftPos=Column12[0].D();
			}
				return nLeftPos;
		}
		if(nEdgeType==2)//�ұ߽�
		{
			if(Column22[0].D()>0)
			{
				nRightPos=Column22[0].D();
			}
			return nRightPos;
		}
		if(nEdgeType==3)//�ϱ߽�
		{
			if(Row12[0].D()>0)
			{
				nUpPos=Row12[0].D();
			}
			return nUpPos;
		}
		if(nEdgeType==4)//�±߽�
		{
			if(Row22[0].D()>0)
			{
				nDownPos=Row22[0].D();
			}
			return nDownPos;
		}
	}
	else
	{
		return 0;
	}
	return 0;
}
int CInnerCheck::CheckPrint()
{
	int nRet = 0;
	// Local iconic variables 
	HObject  ho_Image, hRegionOrg;
	HObject  ho_Rectangle,ho_InnerRectangle;
	HObject  ho_ImageReduced1, ho_Region1, ho_ImageMean, ho_ImageMean1;
	HObject  ho_Region3, ho_RegionDynThresh, ho_RegionDynThresh1;
	HObject  ho_RegionIntersection2, ho_RegionDifference4, ho_RegionUnion;
	HObject  ho_RegionIntersection3, ho_RegionOpening, ho_RegionDifference2;
	HObject  ho_RegionDilation1, ho_RegionDifference1, ho_RegionIntersection1;
	HObject  ho_ConnectedRegions2, ho_SelectedRegions1;
	HObject  ho_Region2, ho_RegionDilation2, ho_RegionErosion;
	HObject  ho_RegionDifference3, ho_ConnectedRegions1;
	HObject  ho_BinImage, ho_Rectangle1, ho_Rectangle2;
	HObject  ho_Rectangle3, ho_Rectangle4, ho_RegionPart, ho_ImageReduced;
	HObject  ho_ImagePart, ho_ConnectedRegions, ho_SelectedRegions;
	HObject  ho_ObjectSelected, ho_RegionFillUp, ho_RegionDifference;
	HObject  ho_RegionIntersection;

	// Local control variables 
	HTuple  hv_ImageFiles, hv_Length, hv_DataCodeHandle;
	HTuple  hv_Index, hv_ResultHandles, hv_DecodedDataStrings;
	HTuple  hv_Length3, hv_CountNum, hv_Size, hv_Row, hv_Column;
	HTuple  hv_Phi, hv_Length1, hv_Length2, hv_HomMat2DIdentity;
	HTuple  hv_HomMat2DRotate, hv_Row1, hv_Column1, hv_Row2;
	HTuple  hv_Column2, hv_Mean1, hv_Deviation1,  hv_Row3;
	HTuple  hv_Column3, hv_Number3, hv_And3;
	HTuple  hv_Number4, hv_And4, hv_HorProjection, hv_VertProjection;
	HTuple  hv_And5, hv_Number, hv_Number2;
	HTuple  hv_Index1, hv_Number1, hv_Area, hv_Area1, hv_Row0;
	HTuple  hv_Column0, hv_And, hv_AbsRow, hv_AbsCol, hv_And1;
	HTuple  hv_And2;
	HTuple  htWid,htHei;
	HTuple  ht_SizeWid,ht_SizeHei;
	HTuple  htszDMW,htszDMH;
	hv_CountNum = 0;

	if(m_iCheckMethod==2)
	{
	GetDataCode2dResults(hDataCodeHandle, "all_results", "version", 
		&m_htVersion);
	GetDataCode2dResults(hDataCodeHandle, "all_results", "model_type", 
		&m_htModeType);
	GetDataCode2dResults(hDataCodeHandle,  "all_results", "error_correction_level", 
		&m_htErrorC);
	GetDataCode2dResults(hDataCodeHandle,  "all_results", "mask_pattern_ref", 
		&m_htMask);
	}
	
	if (m_sCheckPrintParam.sOutRangPara.bValid
		||m_sCheckPrintParam.sInHorverPara.bValid
		||m_sCheckPrintParam.sQrlocShapePara.bValid
		||m_sCheckPrintParam.sInRegionPara.bValid
		||m_sCheckPrintParam.sZouBanPara.bValid)
	{
		m_bCheckPrint= TRUE;
		if (m_iCheckMethod!=3)
		{
			if (m_iCheckMethod!=1)
			{
				if (m_iCheckMethod==2)
				{
					GetDataCode2dResults(hDataCodeHandle, hResultHandles[0], "symbol_size", 
						&hv_Size);
					//if (m_bIsZoomCheck && m_bZoomCheckFlag)
					//{
					//	hv_Size = hv_Size*m_dHorEnlargeRate;
					//}
				}
				else 
				{
					GetDataCode2dResults(hDataCodeHandle, hResultHandles[0], "symbol_cols", 
						&htszDMW);					
					GetDataCode2dResults(hDataCodeHandle, hResultHandles[0], "symbol_rows", 
						&htszDMH);
					/*if (m_bIsZoomCheck && m_bZoomCheckFlag)
					{
						htszDMW = htszDMW*m_dHorEnlargeRate;
						htszDMH = htszDMH*m_dVerEnlargeRate;
					}*/
				}
			}
			else
			{
				GetDataCode2dResults(hDataCodeHandle, hResultHandles[0], "module_width", 
					&ht_SizeWid);
				GetDataCode2dResults(hDataCodeHandle, hResultHandles[0], "module_height", 
					&ht_SizeHei);
				if (m_bIsZoomCheck && m_bZoomCheckFlag)
				{
					ht_SizeWid = ht_SizeWid[0].D()*m_dHorEnlargeRate;
					ht_SizeHei = ht_SizeHei[0].D()*m_dVerEnlargeRate;
				}
				ht_SizeWid = ht_SizeWid[0].D() + 1;
				ht_SizeHei = ht_SizeHei[0].D() + 1;
			}
		}
		else
		{
			if (m_iRealMethod!=1)
			{
				if (m_iCheckMethod==2)
				{
					GetDataCode2dResults(phDataCodeHandle[m_iRealMethod], hResultHandles[0], "symbol_size", 
						&hv_Size);
				/*	if (m_bIsZoomCheck && m_bZoomCheckFlag)
					{
						hv_Size = hv_Size*m_dHorEnlargeRate;
					}*/
				}
				else 
				{
					GetDataCode2dResults(phDataCodeHandle[m_iRealMethod], hResultHandles[0], "symbol_cols", 
						&htszDMW);
					GetDataCode2dResults(phDataCodeHandle[m_iRealMethod], hResultHandles[0], "symbol_rows", 
						&htszDMH);
				/*	if (m_bIsZoomCheck && m_bZoomCheckFlag)
					{
						htszDMW = htszDMW*m_dHorEnlargeRate;
						htszDMH = htszDMH*m_dVerEnlargeRate;
					}*/
				}
			}
			else
			{
				GetDataCode2dResults(phDataCodeHandle[m_iRealMethod], hResultHandles[0], "module_width", 
					&ht_SizeWid);
				GetDataCode2dResults(phDataCodeHandle[m_iRealMethod], hResultHandles[0], "module_height", 
					&ht_SizeHei);
				if (m_bIsZoomCheck && m_bZoomCheckFlag)
				{
					ht_SizeWid = ht_SizeWid[0].D()*m_dHorEnlargeRate;
					ht_SizeHei = ht_SizeHei[0].D()*m_dVerEnlargeRate;
				}
				ht_SizeWid = ht_SizeWid[0].D() + 1;
				ht_SizeHei = ht_SizeHei[0].D() + 1;
			}
		}	
		//hRegionOrg ��ά��������Ӿ���
		GenRegionContourXld(hSymbolXLDs, &hRegionOrg, "filled");
		if (m_bIsZoomCheck && m_bZoomCheckFlag)
		{
			ZoomRegion(hRegionOrg,&hRegionOrg,HTuple(m_dHorEnlargeRate),HTuple(m_dVerEnlargeRate));
		}
		SmallestRectangle2(hRegionOrg, &hv_Row, &hv_Column, &hv_Phi, &hv_Length1, &hv_Length2);
		if (hv_Phi[0].D()>3.1415926/4)
		{
			hv_Phi = hv_Phi[0].D() - 3.1415926/2;
		}
		if (hv_Phi[0].D()<-3.1415926/4)
		{
			hv_Phi = hv_Phi[0].D() + 3.1415926/2;
		}
		HomMat2dIdentity(&hv_HomMat2DIdentity);
		HomMat2dRotate(hv_HomMat2DIdentity, -hv_Phi, hv_Row, hv_Column, &hv_HomMat2DRotate);	
		//m_RegionAffineTrans ����任��Ķ�ά������
		//m_ImageRotate ����任���ͼ��
		AffineTransRegion(hRegionOrg, &m_RegionAffineTrans, hv_HomMat2DRotate, "false");
		AffineTransImage(m_hImageCheck,&m_ImageRotate,hv_HomMat2DRotate,"constant", 
			"false");
		InnerRectangle1(m_RegionAffineTrans, &hv_Row1, &hv_Column1, &hv_Row2, 
			&hv_Column2);
		//ho_InnerRectangle ����任��Ķ�ά���ڽӾ�������
		GenRectangle1(&ho_InnerRectangle, hv_Row1, hv_Column1, hv_Row2, hv_Column2);

		//rotate_image(m_hImageCheck, &ho_ImageRotate, hv_Phi, "constant");
		SmallestRectangle1(m_RegionAffineTrans, &hv_Row1, &hv_Column1, &hv_Row2, 
			&hv_Column2);
		//ho_Rectangle ����任��Ķ�ά����Ӿ�������
		GenRectangle1(&ho_Rectangle, hv_Row1, hv_Column1, hv_Row2, hv_Column2);

		//	DilationRectangle1(ho_Rectangle, &m_RegionOutRect, 100, 110);
		//  [9/16/2013 Denghl]
		
		double left1,right1,top1,bottom1;
		double left2,right2,top2,bottom2;

		left1   = hv_Column1[0].D() - 20;//m_sCheckPrintParam.sOutRangPara.nLeft;
		right1  = hv_Column2[0].D() + 20;//m_sCheckPrintParam.sOutRangPara.nRight;
		top1    = hv_Row1[0].D()    - 20;//m_sCheckPrintParam.sOutRangPara.nTop;
		bottom1 = hv_Row2[0].D()    + 20;//m_sCheckPrintParam.sOutRangPara.nBottom;

		left2   = hv_Column1[0].D() - m_sCheckPrintParam.sOutRangPara.nLeft;
		right2  = hv_Column2[0].D() + m_sCheckPrintParam.sOutRangPara.nRight;
		top2    = hv_Row1[0].D()    - m_sCheckPrintParam.sOutRangPara.nTop;
		bottom2 = hv_Row2[0].D()    + m_sCheckPrintParam.sOutRangPara.nBottom;

		// halconͼ��δ���о�����
		//top    = hv_Row1[0].D() - m_sCheckPrintParam.sOutRangPara.nBottom;
		//bottom = hv_Row2[0].D() + m_sCheckPrintParam.sOutRangPara.nTop;
		//m_RegionOutRect ��ά������������
		HObject hRegionOutRect;

		// ���ڼ����ά������ [12/21/2015 TIAN]
		GenRectangle1(&hRegionOutRect,
			top1,
			left1,
			bottom1,
			right1);
		// ������������ [12/21/2015 TIAN]
		GenRectangle1(&m_RegionOutRect,
			top2,
			left2,
			bottom2,
			right2);
		//////////////////////////////////////////////////////////////////////////
		ReduceDomain(m_ImageRotate, hRegionOutRect, &ho_ImageReduced1);
		BinThreshold(ho_ImageReduced1, &ho_Region1);
		Intensity(ho_Region1, m_ImageRotate, &hv_Mean1, &hv_Deviation1);	

		MeanImage(m_ImageRotate, &ho_ImageMean, 15, 15);
		MeanImage(ho_ImageMean, &ho_ImageMean1, 3, 3);
		Threshold(m_ImageRotate, &ho_Region3, 0, hv_Mean1+(2.5*hv_Deviation1));
		DynThreshold(ho_ImageMean1, ho_ImageMean, &ho_RegionDynThresh, 10, "dark");
		DynThreshold(m_ImageRotate, ho_ImageMean, &ho_RegionDynThresh1, 10, "dark");

		Intersection(ho_RegionDynThresh1, ho_Rectangle, &ho_RegionIntersection2);
		Difference(ho_RegionDynThresh, ho_Rectangle, &ho_RegionDifference4);
		Union2(ho_RegionDifference4, ho_RegionIntersection2, &ho_RegionDynThresh);

		Union2(ho_RegionDynThresh, ho_Region3, &ho_RegionUnion);
		Intersection(ho_Rectangle, ho_RegionUnion, &ho_RegionIntersection3);
		//opening_circle(ho_RegionIntersection3, &ho_RegionOpening,0.5);
		Difference(ho_RegionUnion, ho_Rectangle, &ho_RegionDifference2);

		if (m_iCheckMethod!=1||(m_iCheckMethod==3&&m_iRealMethod!=1))
		{
			Union2(ho_RegionIntersection3, ho_RegionDifference2, &ho_Region1);
		}
		//////////////////////////////////////////////////////////////////////////
		GetImageSize(m_ImageRotate,&htWid,&htHei);
		if (m_iCheckMethod==2||(m_iCheckMethod==3&&m_iRealMethod==2))
		{
			ht_SizeWid = (hv_Column2-hv_Column1)/hv_Size;
			ht_SizeHei = (hv_Row2-hv_Row1)/hv_Size;
		}
		else if (m_iCheckMethod==0||(m_iCheckMethod==3&&m_iRealMethod==0))
		{
			ht_SizeWid = (hv_Column2-hv_Column1)/htszDMW;
			ht_SizeHei = (hv_Row2-hv_Row1)/htszDMH;
		}

	}

	//1. ��ά���������Ƿ����ж� [9/13/2013 Denghl]	
	if (m_sCheckPrintParam.sOutRangPara.bValid)
	{		
		DilationRectangle1(ho_Rectangle, &ho_RegionDilation1, 3.5, 3.5);
		Difference(m_RegionOutRect, ho_RegionDilation1, &ho_RegionDifference1);
		Intersection(ho_Region1, ho_RegionDifference1, &ho_RegionIntersection1);
		//  [9/17/2013 Denghl]
		HObject hRegionOrgAll,hAffAll;
		GenRectangle1(&hRegionOrgAll,0,0,htHei[0].D()-1,htWid[0].D()-1);
		AffineTransRegion(hRegionOrgAll, &hAffAll, hv_HomMat2DRotate, "false");
		ErosionRectangle1(hAffAll,&hAffAll,3,3);
		Intersection(ho_RegionIntersection1,hAffAll,&ho_RegionIntersection1);

		Connection(ho_RegionIntersection1, &ho_ConnectedRegions2);
		SelectShape(ho_ConnectedRegions2, &ho_SelectedRegions1, "area", "and", m_sCheckPrintParam.sOutRangPara.nSingleArea, 
			99999999);

		Union1(ho_SelectedRegions1, &m_RegionOutRang);
		AreaCenter(m_RegionOutRang, &hv_AreaAll, &hv_Row3, &hv_Column3);
		TupleLength(hv_AreaAll,&hv_Number3);
		if (0 != (hv_Number3[0].I()>0)&&hv_AreaAll[0].D()>m_sCheckPrintParam.sOutRangPara.nAllArea)
		{
			nRet = 1;
		}
	}

	//2. ��������������ȱӡ [9/13/2013 Denghl]
	if (m_sCheckPrintParam.sInRegionPara.bValid && ht_SizeHei[0].D()>5 && ht_SizeWid[0].D()>5)
	{	
		Intersection(ho_Region1, ho_Rectangle, &ho_Region2);

		ClosingRectangle1(ho_Region2, &ho_RegionDilation2, (3*ht_SizeWid[0].D()), 
			3*ht_SizeHei[0].D());
		ErosionRectangle1(ho_Rectangle, &ho_RegionErosion, ht_SizeWid, 
			ht_SizeHei);
		Difference(ho_RegionErosion, ho_RegionDilation2, &ho_RegionDifference3);
		Connection(ho_RegionDifference3, &ho_ConnectedRegions1);
		SelectShape(ho_ConnectedRegions1, &m_InRegionQue, "area", "and", (16*ht_SizeWid[0].D())*ht_SizeHei[0].D(), 
			9999999);	

	    //  [1/6/2016 Baiyy]
		HObject hImgMin,hRegionFor, hRegionForErosion, hRegionQue;
		HTuple   hArea, lNum;
		GenEmptyObj(&hRegionQue);
		GrayErosionRect(m_ImageRotate, &hImgMin, 3, 3);
		BinThreshold(hImgMin, &hRegionFor);		
		ErosionRectangle1(hRegionFor, &hRegionForErosion, 3, 3);
		Intensity(hRegionFor, hImgMin, &hv_Mean, &hv_Deviation);
		Threshold(hImgMin, &hRegionQue, hv_Mean+hv_Deviation, 255);
		Intersection(hRegionForErosion, hRegionQue, &hRegionQue);
		Connection(hRegionQue, &hRegionQue);
		SelectShape(hRegionQue, &hRegionQue, "area", "and", ht_SizeWid*ht_SizeHei, 9999999);
		CountObj(hRegionQue, &lNum);

		if (lNum[0].I()>0)
		{
			Intensity(hRegionQue, m_ImageRotate, &hv_Mean, &hv_Deviation);
			TupleAnd(hv_Deviation[0].D()>(m_sCheckPrintParam.sInRegionPara.nDeviation), 
				(hv_Mean[0].D()+(2*hv_Deviation[0].D()))>(m_sCheckPrintParam.sInRegionPara.nMaxGray), &hv_And3);
			
			// ��������ȱӡ��ʱ���� [10/11/2013 Denghl]
			GenEmptyObj(&m_InRegionQue);		
			CountObj(m_InRegionQue, &hv_Number4);
			TupleOr(hv_Number4[0].I()>0, hv_And3, &hv_And4);
			if (0 != hv_And4[0].I())
			{
				nRet = 2;
			}
		}
	}
	//3. �����ڴ�ֱ��ˮƽȱӡ [9/13/2013 Denghl]
	if (m_sCheckPrintParam.sInHorverPara.bValid&&(m_iCheckMethod!=1||(m_iCheckMethod==3&&m_iRealMethod!=1)))
	{
		HObject hTestRegion;
		HObject hRegionClosing;
		HObject hImageMean;
		HObject hDataReg;
		HTuple   hArea, hNum;
		HTuple  hSelArea, hWBPercent, hWid ,hhei;
		int nDirect, nContrast;
		nDirect   = m_sCheckPrintParam.sInHorverPara.nDirect;
		//hSelArea  = m_sCheckPrintParam.sInHorverPara.nArea;   
		hWBPercent  = HTuple(m_sCheckPrintParam.sInHorverPara.nPercent*0.01); //�׺ڰٷֱ�
		nContrast = m_sCheckPrintParam.sInHorverPara.nContrast;
		m_hWhiteLineValue = 0;
		hWid = hv_Column2-hv_Column1;
		hhei = hv_Row2-hv_Row1;
		GenEmptyObj(&m_InWhiteLine);
        
		//���������������ò��������ܵ��¸�������ȡ��׼
		Intersection(ho_Region1, ho_Rectangle, &hTestRegion);

		//  ����[11/28/2015 TIAN]
		//write_image(m_ImageRotate,"bmp",255,"d:\\m_ImageRotate.bmp");
		//write_region(ho_Rectangle,"d:\\ho_Rectangle.reg");
		//write_region(hTestRegion,"d:\\hTestRegion.reg");

		// ���ά���ڵİ����� [11/28/2015 TIAN]
		ClosingRectangle1 (hTestRegion, &hRegionClosing, 3, 3);
		ErosionRectangle1 (hRegionClosing, &hRegionClosing, 5, 5);
		
		MeanImage (m_ImageRotate, &hImageMean, 7, 7);   
		DynThreshold (m_ImageRotate, hImageMean, &hDataReg, nContrast, "light");
		Intersection (hRegionClosing, hDataReg, &hDataReg);
		//Intersection (hTestRegion, hDataReg, &hDataReg);
		//CountObj(hDataReg, &hNum);
		//end

		HTuple  htWidth,htHeight;
		HObject hoBinWhite,hoBinBlack;
		GetImageSize (m_ImageRotate, &htWidth, &htHeight);

		RegionToBin(hTestRegion,&hoBinBlack,255,0,htWidth,htHeight);
		RegionToBin(hDataReg,&hoBinWhite,255,0,htWidth,htHeight);

		HTuple hv_HorProjBlk,hv_VertProjBlk,hv_HorProjWhite,hv_VertProjWhite;
		GrayProjections (ho_Rectangle, hoBinBlack, "simple", &hv_HorProjBlk, &hv_VertProjBlk);
		GrayProjections (ho_Rectangle, hoBinWhite, "simple", &hv_HorProjWhite, &hv_VertProjWhite);
		
		HTuple htLength,htPercent,htMin,htIndices;
		if (nDirect==0)
		{
			htPercent=hv_HorProjWhite/(hv_HorProjBlk+1);
			TupleMin2 (htPercent, hWBPercent, &htMin); //0.7 -> hWBPercent
			TupleFind (htMin, hWBPercent, &hv_Indices); //htIndices->hv_Indices

			if (0 != (hv_Indices==-1))
			{
				hv_Indices = HTuple();
			}

			HTuple hlWhiteNum;
			TupleLength(hv_Indices,&hlWhiteNum);

			m_nHorverType = 0;  //��Ϊ��ʼֵ
			if(hlWhiteNum[0].I()>0)
			{
				nRet = 3;
				m_nHorverType = 31;
				m_InRegion    = hTestRegion;
			}
		}
		else
		{
			htPercent=hv_VertProjWhite[0].D()/(hv_VertProjBlk[0].D()+1);
			TupleMin2 (htPercent, hWBPercent, &htMin); //0.7 -> hWBPercent
			TupleFind (htMin, hWBPercent, &hv_IndicesVer); //htIndices->hv_IndicesVer

			if (0 != (hv_IndicesVer[0].I()==-1))
			{
				hv_IndicesVer = HTuple();
			}

			HTuple hlWhiteNum;
			TupleLength(hv_IndicesVer,&hlWhiteNum);

			m_nHorverType = 0;  //��Ϊ��ʼֵ
			if(hlWhiteNum[0].I()>0)
			{
				nRet = 3;
				m_nHorverType = 31;
				m_InRegion    = hTestRegion;
			}
		}
		//if (0 != (htIndices==-1))
		//{
		//	htIndices = HTuple();
		//}

		//HTuple hlWhiteNum;
		//TupleLength(htIndices,&hlWhiteNum);
		//
		//m_nHorverType = 0;  //��Ϊ��ʼֵ
		//if(hlWhiteNum>0)
		//{
		//		nRet = 3;
		//		m_nHorverType = 31;
		//}
		/*HObject hPartitioned;
		HTuple hResultWidth, hResultHeight;
		HTuple Row11, Row21,Column11, Column21;
		GetDataCode2dResults(hDataCodeHandle, hResultHandles, "module_height", &hResultHeight);
		GetDataCode2dResults (hDataCodeHandle, hResultHandles, "module_width", &hResultWidth);

	

		if (hNum>0)
		{
			HObject hPartitioned;
			HTuple hMean;
			HObject hErrorRegs;
			GenEmptyObj(&hErrorRegs);
			if (nDirect==0)
			{
				HObject hRects;
				HObject hCheckForRegs;
				HTuple lNum;
				HObject hParatiton2;
				//hParatiton2 ��Ӿ�������ֿ飬����ÿ��߶�
				partition_rectangle (ho_Rectangle, &hParatiton2,99999, hResultHeight*1.2);
				SmallestRectangle1 (hParatiton2, &Row11, &Column11, &Row21, &Column21);
				GenRectangle1 (&hRects, (Row11+Row21)/2, Column11,  (Row11+Row21)/2, Column21);
				CountObj(hParatiton2, &lNum);
				for (int i=0; i<lNum; i++)
				{
					HObject hRectSel, hForReg,hErrReg ;
					HTuple lwidth;
				//	SelectObj (hCheckForRegs, &hForReg, i);
					SelectObj (hRects, &hRectSel, i+1);
					Intersection (hTestRegion, hRectSel, &hForReg);
					AreaCenter (hForReg, &lwidth, NULL, NULL);
					//SelectObj (hPartitioned, &hErrReg, i+1);
					Intersection(hDataReg, hRectSel, &hErrReg);
					SelectShape (hErrReg, &hErrReg, "height", "and", lwidth*2/3, 99999);  	
					Union2(hErrorRegs, hErrReg, &hErrorRegs);
				}		
			}
			else
			{
				//hPartitioned ȱ�ݷֿ�,���ں����߶�ɸѡ
				//partition_rectangle (hDataReg, &hPartitioned, hResultWidth*1.2, 99999);
				//  [11/2/2015 Administrator]
				HObject hRects;
				HObject hCheckForRegs;
				HTuple lNum;
				HObject hParatiton2;
				//hParatiton2 ��Ӿ�������ֿ飬����ÿ��߶�
				partition_rectangle (ho_Rectangle, &hParatiton2, hResultWidth*1.2, 99999);
				SmallestRectangle1 (hParatiton2, &Row11, &Column11, &Row21, &Column21);
				GenRectangle1 (&hRects, Row11, (Column11+Column21)/2, Row21, (Column11+Column21)/2);
				CountObj(hParatiton2, &lNum);
				for (int i=0; i<lNum; i++)
				{
					HObject hRectSel, hForReg,hErrReg ;
					HTuple lheight;
				//	SelectObj (hCheckForRegs, &hForReg, i);
					SelectObj (hRects, &hRectSel, i+1);
					Intersection (hTestRegion, hRectSel, &hForReg);
					AreaCenter (hForReg, &lheight, NULL, NULL);
					//SelectObj (hPartitioned, &hErrReg, i+1);
					Intersection(hDataReg, hRectSel, &hErrReg);
					SelectShape (hErrReg, &hErrReg, "height", "and", lheight*2/3, 99999);  	
					Union2(hErrorRegs, hErrReg, &hErrorRegs);
				}		
			}
			SelectShape (hErrorRegs, &hErrorRegs, "area", "and", hSelArea, 99999999);
			Union1(hErrorRegs, &m_InWhiteLine);
			CountObj(m_InWhiteLine, &hNum);
			if (hNum>0)
			{
				AreaCenter (m_InWhiteLine, &m_hWhiteLineValue, NULL, NULL);
				Intensity (m_InWhiteLine, m_ImageRotate, &hMean, NULL);
				if (m_hWhiteLineValue>0)
				{
					nRet = 3;
				}
			}	
		}*/
		if (nRet!=3)//��ͷ���������߹ᴩ
		{
			RegionToBin(hTestRegion, &ho_BinImage, 255, 0, htWid, htHei);
			//write_image(ho_BinImage,"bmp",0,"d:\\1.bmp");

			ErosionRectangle1(ho_InnerRectangle, &m_InRegion, ht_SizeWid, 
				ht_SizeHei);
			GrayProjections(m_InRegion, ho_BinImage, "simple", &hv_HorProjection, 
				&hv_VertProjection);

			TupleFind(hv_HorProjection, 0, &hv_Indices);
			TupleFind(hv_VertProjection, 0, &hv_IndicesVer);
			TupleAnd(hv_Indices[0].I()==-1, hv_IndicesVer[0].I()==-1, &hv_And5);
			if (0 != (hv_And5[0].I()==0))
			{
				nRet = 3;
				m_nHorverType = 32;
			}
		}
	}
	//4. �ж��߰� [7/31/2014 Denghl]
	if (m_sCheckPrintParam.sZouBanPara.bValid)
	{
		//write_region(m_hZoubanRegion,"d:\\m_hZoubanRegion.reg");
		//write_region(hRegionOrg,"d:\\hRegionOrg.reg");
		if(0)//!m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan
		{
			HObject hZoubanRegTmp;
			m_hRealZoubanRegion = m_hZoubanRegion;
			// �����ڲ���λ����ƫ�� [8/8/2014 Denghl]
			if (0)//m_sCheckPrintParam.sZouBanPara.bIsInLoc
			{
				if (m_lMarkID>=0)
				{
					HTuple htModRow,htModCol,htRealRow,htRealCol,htScore,htAngle;
					HObject hReduceImg;
					ReduceDomain(m_ImageRotate,m_hInlocSearchRegion,&hReduceImg);
					FindNccModel(m_ImageRotate,m_lMarkID,-0.39,0.78,0.4,1,0.5,"true",0,
						&htRealRow,&htRealCol,&htAngle,&htScore);
					HTuple hNum;
					TupleLength(htScore,&hNum);
					if (hNum[0].I()>0 )
					{
						HTuple htAreaTmp;
						AreaCenter(m_hInlocMarkRegion,&htAreaTmp,&htModRow,&htModCol);
						MoveRegion(m_hZoubanRegion,&m_hRealZoubanRegion,
							htRealRow-htModRow,htRealCol-htModCol);
					}
					else
					{
						nRet = 7;//�ڲ���λʧ��
					}
				}
				else
				{
					nRet = 6;//���ڲ���λģ��
				}

			}
			
			if (1)//nRet!=6&&nRet!=7
			{
				Difference(hRegionOrg,m_hRealZoubanRegion,&hZoubanRegTmp);
				SelectShape(hZoubanRegTmp,&hZoubanRegTmp,"area","and",1,99999999);
				HTuple htZBCount;
				CountObj(hZoubanRegTmp,&htZBCount);
				if (htZBCount[0].I()>0)
				{
					nRet = 5;
				}
			}
		}
		else   //����߰�
		{
			SmallestRectangle1(m_RegionAffineTrans, &hv_Row1, &hv_Column1, &hv_Row2, 
				&hv_Column2);
			HObject htImgReduce;
			HTuple htImgW,htImgH,htNum,htMean,htDeviation;
			HTuple Row12, Column12, Row22, Column22;
			GetImageSize(m_ImageRotate,&htImgW,&htImgH);
			int nLeftPos,nRightPos,nUpPos,nDownPos;
			nLeftPos=nRightPos=nUpPos=nDownPos=0;
			// ���׿����ұ߽�
			if(m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanLeft)
			{
				//������߽�
				if(m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan)
				{
					HObject ho_SearchRange,htRegThresh,ho_ConnReg,ho_SelectedReg;
					//�ж� �ױ߽��Ƿ��Ѿ����ã���û����������ȡ
					if(m_sCheckPrintParam.sZouBanPara.m_nLeftPos==0 && m_sCheckPrintParam.sZouBanPara.bLRChangeZouban)
					{
						HTuple htW = hv_Column2-hv_Column1;
						nLeftPos = ExWhiteEdge(m_ImageRotate, htW,1);
						char strTemp[100];
						if(nLeftPos>0)
						{
							m_sCheckPrintParam.sZouBanPara.m_nLeftPos = nLeftPos;			
							sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_nLeftPos);
							WritePrivateProfileString("�����߰�", "��߽�λ��", strTemp, m_strParaIni);
						}
						else
						{
							m_sCheckPrintParam.sZouBanPara.m_nLeftPos = 0;
						}
						m_sCheckPrintParam.sZouBanPara.bLRChangeZouban = FALSE;
						sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.bLRChangeZouban);
						WritePrivateProfileString("�����߰�", "���Ұ׿����Ƿ��޸�", strTemp, m_strParaIni);		
					}
					else
					{
						GenRectangle1(&ho_SearchRange,0,MAX(MIN(m_sCheckPrintParam.sZouBanPara.m_nLeftPos-50,htImgW[0].D()-1),0),
							htImgH[0].D()-1,MIN(MAX(m_sCheckPrintParam.sZouBanPara.m_nLeftPos+50,0),htImgW[0].D()-1));
						ReduceDomain(m_ImageRotate,ho_SearchRange,&htImgReduce);
						Intensity (ho_SearchRange, m_ImageRotate, &htMean, &htDeviation);
						if(htMean[0].D()+0.3*htDeviation[0].D()<255&&htMean[0].D()+0.3*htDeviation[0].D()>100)
						{
							Threshold (htImgReduce, &htRegThresh, htMean[0].D()+0.3*htDeviation[0].D(), 255);
						}
						else
						{
							Threshold (htImgReduce, &htRegThresh, 150, 255);
						}
						//Threshold(htImgReduce,&htRegThresh,150,255);
						Connection (htRegThresh, &ho_ConnReg);
						SelectShape (ho_ConnReg, &ho_SelectedReg, "width", "and", (hv_Column2[0].D()-hv_Column1[0].D())*1.1, 99999);
						CountObj(ho_SelectedReg,&htNum);
						if(htNum[0].I()==1)
						{
							SmallestRectangle1 (ho_SelectedReg, &Row12, &Column12, &Row22, &Column22);
							/*HObject hoBinImg;
							HTuple htHorProj,htVerProj,Width,Height;
							HTuple htIndexHor,htIndexVer,htLengthHor,hLengthVer;
							GetImageSize(m_ImageRotate,&Width,&Height);
							RegionToBin(ho_SelectedReg,&hoBinImg,255,0,Width,Height);
							GrayProjections(hoBinImg,hoBinImg,"simple",&htHorProj,&htVerProj);
							TupleMin2(htHorProj,40,&htHorProj);
							TupleMin2(htVerProj,40,&htVerProj);
							TupleFind(htHorProj,40,&htIndexHor);
							TupleFind(htVerProj,40,&htIndexVer);
							TupleLength(htIndexHor,&htLengthHor);
							TupleLength(htIndexVer,&hLengthVer);
							if(htLengthHor[0].D()>0)
							{
							Row12=htIndexHor[0];
							Row22=htIndexHor[htLengthHor-1];
							}
							if(hLengthVer[0].D()>0)
							{
							Column12=htIndexVer[0];
							Column22=htIndexVer[hLengthVer-1];
							}*/
							if(Column12[0].D()>0)
							{
								nLeftPos=Column12[0].D();
							}
						}
						else
						{
							nLeftPos=m_sCheckPrintParam.sZouBanPara.m_nLeftPos;
						}
					}
				}
				else
				{
					nLeftPos=0;
				}
				//�ж��߰�
				int nZouBanMin=nLeftPos
					+(m_sCheckPrintParam.sZouBanPara.m_fZouBanStdHor-m_sCheckPrintParam.sZouBanPara.m_fZouBanRange)/m_fScaleHor;
				int nZouBanMax=nLeftPos
					+(m_sCheckPrintParam.sZouBanPara.m_fZouBanStdHor+m_sCheckPrintParam.sZouBanPara.m_fZouBanRange)/m_fScaleHor;
				if(hv_Column1[0].D()>nZouBanMax||hv_Column1[0].D()<nZouBanMin)
				{
					nRet = 5;
				}
				GenRectangle1(&m_hRealZoubanRegionL,0,MAX(MIN(nZouBanMin,htImgW[0].D()-1),0),
					htImgH[0].D()-1,MIN(MAX(nZouBanMax,0),htImgW[0].D()-1));
			}
			else if(m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanRight)
			{
				//�����ұ߽�
				if(m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan)
				{
					if(m_sCheckPrintParam.sZouBanPara.m_nRightPos==0 && m_sCheckPrintParam.sZouBanPara.bLRChangeZouban)
					{
						char strTemp[100];
						HTuple htW=hv_Column2-hv_Column1;
						nRightPos=ExWhiteEdge(m_ImageRotate, htW,2);
						if(nRightPos>0) 
						{
							m_sCheckPrintParam.sZouBanPara.m_nRightPos = nRightPos;
							sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_nRightPos);
							WritePrivateProfileString("�����߰�", "�ұ߽�λ��", strTemp, m_strParaIni);
						}
						else
						{
							m_sCheckPrintParam.sZouBanPara.m_nRightPos = 0;
						}
						m_sCheckPrintParam.sZouBanPara.bLRChangeZouban = FALSE;
						sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.bLRChangeZouban);
						WritePrivateProfileString("�����߰�", "���Ұ׿����Ƿ��޸�", strTemp, m_strParaIni);
					}
					else
					{
						HObject ho_SearchRange,htRegThresh,ho_ConnReg,ho_SelectedReg;
						GenRectangle1(&ho_SearchRange,0,MAX(MIN(m_sCheckPrintParam.sZouBanPara.m_nRightPos-50,
							htImgW[0].D()-1),0),
							htImgH[0].D()-1,MIN(MAX(m_sCheckPrintParam.sZouBanPara.m_nRightPos+50,0),htImgW[0].D()-1));
						ReduceDomain(m_ImageRotate,ho_SearchRange,&htImgReduce);
						Intensity (ho_SearchRange, m_ImageRotate, &htMean, &htDeviation);
						if(htMean[0].D()+0.3*htDeviation[0].D()<255&&htMean[0].D()+0.3*htDeviation[0].D()>100)
						{
							Threshold (htImgReduce, &htRegThresh, htMean[0].D()+0.3*htDeviation[0].D(), 255);
						}
						else
						{
							Threshold (htImgReduce, &htRegThresh, 150, 255);
						}
						Connection (htRegThresh, &ho_ConnReg);
						SelectShape (ho_ConnReg, &ho_SelectedReg, "width", "and", (hv_Column2[0].D()-hv_Column1[0].D())*1.1, 99999);
						CountObj(ho_SelectedReg,&htNum);
						if(htNum[0].I()==1)
						{
							SmallestRectangle1 (ho_SelectedReg, &Row12, &Column12, &Row22, &Column22);
							/*HObject hoBinImg;
							HTuple htHorProj,htVerProj,Width,Height;
							HTuple htIndexHor,htIndexVer,htLengthHor,hLengthVer;
							GetImageSize(m_ImageRotate,&Width,&Height);
							RegionToBin(ho_SelectedReg,&hoBinImg,255,0,Width,Height);
							GrayProjections(hoBinImg,hoBinImg,"simple",&htHorProj,&htVerProj);
							TupleMin2(htHorProj,40,&htHorProj);
							TupleMin2(htVerProj,40,&htVerProj);
							TupleFind(htHorProj,40,&htIndexHor);
							TupleFind(htVerProj,40,&htIndexVer);
							TupleLength(htIndexHor,&htLengthHor);
							TupleLength(htIndexVer,&hLengthVer);
							if(htLengthHor[0].D()>0)
							{
								Row12=htIndexHor[0];
								Row22=htIndexHor[htLengthHor-1];
							}
							if(hLengthVer[0].D()>0)
							{
								Column12=htIndexVer[0];
								Column22=htIndexVer[hLengthVer-1];
							}*/
							if(Column22[0].D()<htImgW[0].D()-1)
							{
								nRightPos=Column22[0].D();
							}
						}
						else
						{
							nRightPos=m_sCheckPrintParam.sZouBanPara.m_nRightPos;
						}
					}
				}
				else
				{
					nRightPos=htImgW[0].D()-1;
				}
				//�ж��߰�
				int nZouBanMin=nRightPos
					-(m_sCheckPrintParam.sZouBanPara.m_fZouBanStdHor+m_sCheckPrintParam.sZouBanPara.m_fZouBanRange)/m_fScaleHor;
				int nZouBanMax=nRightPos
					-(m_sCheckPrintParam.sZouBanPara.m_fZouBanStdHor-m_sCheckPrintParam.sZouBanPara.m_fZouBanRange)/m_fScaleHor;
				if(hv_Column2[0].D()>nZouBanMax||hv_Column2[0].D()<nZouBanMin)
				{
					nRet = 5;
				}
				GenRectangle1(&m_hRealZoubanRegionR,0,MAX(MIN(nZouBanMin,htImgW[0].D()-1),0),
					htImgH[0].D()-1,MIN(MAX(nZouBanMax,0),htImgW[0].D()-1));
			}
			// ���׿����±߽�
			if(m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanUp)
			{
				//�����ϱ߽�
				if(m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan)
				{
					if(m_sCheckPrintParam.sZouBanPara.m_nUpPos==0 && m_sCheckPrintParam.sZouBanPara.bUDChangeZouban)
					{
						HTuple htW=hv_Column2-hv_Column1;
						nUpPos=ExWhiteEdge(m_ImageRotate, htW,3);
						char strTemp[100];
						if(nUpPos>0) 
						{
							m_sCheckPrintParam.sZouBanPara.m_nUpPos=nUpPos;		
							sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_nUpPos);
							WritePrivateProfileString("�����߰�", "�ϱ߽�λ��", strTemp, m_strParaIni);
						}
						else
						{
							m_sCheckPrintParam.sZouBanPara.m_nUpPos=0;
						}
						m_sCheckPrintParam.sZouBanPara.bUDChangeZouban = FALSE;
						sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.bUDChangeZouban);
						WritePrivateProfileString("�����߰�", "���°׿����Ƿ��޸�", strTemp, m_strParaIni);
					}
					else
					{
						HObject ho_SearchRange,htRegThresh,ho_ConnReg,ho_SelectedReg;
						GenRectangle1(&ho_SearchRange,MAX(MIN(m_sCheckPrintParam.sZouBanPara.m_nUpPos-50,htImgH[0].D()-1),0),0,
							MIN(MAX(m_sCheckPrintParam.sZouBanPara.m_nUpPos+50,0),htImgH[0].D()-1),htImgW[0].D()-1);
						ReduceDomain(m_ImageRotate,ho_SearchRange,&htImgReduce);
						Intensity (ho_SearchRange, m_ImageRotate, &htMean, &htDeviation);
						if(htMean[0].D()+0.3*htDeviation[0].D()<255&&htMean[0].D()+0.3*htDeviation[0].D()>100)
						{
							Threshold (htImgReduce, &htRegThresh, htMean[0].D()+0.3*htDeviation[0].D(), 255);
						}
						else
						{
							Threshold (htImgReduce, &htRegThresh, 150, 255);
						}
						Connection (htRegThresh, &ho_ConnReg);
						SelectShape (ho_ConnReg, &ho_SelectedReg, "width", "and", (hv_Column2[0].D()-hv_Column1[0].D())*1.1, 99999);
						CountObj(ho_SelectedReg,&htNum);
						if(htNum[0].I()==1)
						{
							SmallestRectangle1 (ho_SelectedReg, &Row12, &Column12, &Row22, &Column22);
							/*HObject hoBinImg;
							HTuple htHorProj,htVerProj,Width,Height;
							HTuple htIndexHor,htIndexVer,htLengthHor,hLengthVer;
							GetImageSize(m_ImageRotate,&Width,&Height);
							RegionToBin(ho_SelectedReg,&hoBinImg,255,0,Width,Height);
							GrayProjections(hoBinImg,hoBinImg,"simple",&htHorProj,&htVerProj);
							TupleMin2(htHorProj,40,&htHorProj);
							TupleMin2(htVerProj,40,&htVerProj);
							TupleFind(htHorProj,40,&htIndexHor);
							TupleFind(htVerProj,40,&htIndexVer);
							TupleLength(htIndexHor,&htLengthHor);
							TupleLength(htIndexVer,&hLengthVer);
							if(htLengthHor[0].D()>0)
							{
								Row12=htIndexHor[0];
								Row22=htIndexHor[htLengthHor-1];
							}
							if(hLengthVer[0].D()>0)
							{
								Column12=htIndexVer[0];
								Column22=htIndexVer[hLengthVer-1];
							}*/
							if(Row12[0].D()>0)
							{
								nUpPos=Row12[0].D();
							}
						}
						else
						{
							nUpPos=m_sCheckPrintParam.sZouBanPara.m_nUpPos;
						}
					}
				}
				else
				{
					nUpPos=0;
				}
				//�ж��߰�
				int nZouBanMin=nUpPos
					+(m_sCheckPrintParam.sZouBanPara.m_fZouBanStdVer-m_sCheckPrintParam.sZouBanPara.m_fZouBanRange)/m_fScaleVer;
				int nZouBanMax=nUpPos
					+(m_sCheckPrintParam.sZouBanPara.m_fZouBanStdVer+m_sCheckPrintParam.sZouBanPara.m_fZouBanRange)/m_fScaleVer;
				if(hv_Row1[0].D()>nZouBanMax||hv_Row1[0].D()<nZouBanMin)
				{
					nRet = 5;
				}
				GenRectangle1(&m_hRealZoubanRegionU,MAX(MIN(nZouBanMin,htImgH[0].D()-1),0),0,
					MIN(MAX(nZouBanMax,0),htImgH[0].D()-1),htImgW[0].D()-1);
			}
			else if(m_sCheckPrintParam.sZouBanPara.m_bIsCheckZouBanDown)
			{
				//�����±߽�
				if(m_sCheckPrintParam.sZouBanPara.m_bIsWhiteZouBan)
				{
					if(m_sCheckPrintParam.sZouBanPara.m_nDownPos==0 && m_sCheckPrintParam.sZouBanPara.bUDChangeZouban)
					{
						char strTemp[100];
						HTuple htW=hv_Column2-hv_Column1;
						nDownPos=ExWhiteEdge(m_ImageRotate, htW,4);
						if(nDownPos>0) 
						{
							m_sCheckPrintParam.sZouBanPara.m_nDownPos=nDownPos;		
							sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.m_nDownPos);
							WritePrivateProfileString("�����߰�", "�±߽�λ��", strTemp, m_strParaIni);
						}
						else
						{
							m_sCheckPrintParam.sZouBanPara.m_nDownPos=0;
						}
						m_sCheckPrintParam.sZouBanPara.bUDChangeZouban = FALSE;
						sprintf(strTemp, "%d", m_sCheckPrintParam.sZouBanPara.bUDChangeZouban);
						WritePrivateProfileString("�����߰�", "���°׿����Ƿ��޸�", strTemp, m_strParaIni);
					}
					else
					{
						HObject ho_SearchRange,htRegThresh,ho_ConnReg,ho_SelectedReg;
						GenRectangle1(&ho_SearchRange,MAX(MIN(m_sCheckPrintParam.sZouBanPara.m_nDownPos-50,htImgH[0].D()-1),0),0,
							MIN(MAX(m_sCheckPrintParam.sZouBanPara.m_nDownPos+50,0),htImgH[0].D()-1),htImgW[0].D()-1);
						ReduceDomain(m_ImageRotate,ho_SearchRange,&htImgReduce);
						Intensity (ho_SearchRange, m_ImageRotate, &htMean, &htDeviation);
						Threshold (htImgReduce, &htRegThresh, htMean[0].D()+0.3*htDeviation[0].D(), 255);
						if(htMean[0].D()+0.3*htDeviation[0].D()<255&&htMean[0].D()+0.3*htDeviation[0].D()>100)
						{
							Threshold (htImgReduce, &htRegThresh, htMean[0].D()+0.3*htDeviation[0].D(), 255);
						}
						else
						{
							Threshold (htImgReduce, &htRegThresh, 150, 255);
						}
						Connection (htRegThresh, &ho_ConnReg);
						SelectShape (ho_ConnReg, &ho_SelectedReg, "width", "and", (hv_Column2[0].D()-hv_Column1[0].D())*1.1, 99999);
						CountObj(ho_SelectedReg,&htNum);
						if(htNum[0].I()==1)
						{
							SmallestRectangle1 (ho_SelectedReg, &Row12, &Column12, &Row22, &Column22);
							/*HObject hoBinImg;
							HTuple htHorProj,htVerProj,Width,Height;
							HTuple htIndexHor,htIndexVer,htLengthHor,hLengthVer;
							GetImageSize(m_ImageRotate,&Width,&Height);
							RegionToBin(ho_SelectedReg,&hoBinImg,255,0,Width,Height);
							GrayProjections(hoBinImg,hoBinImg,"simple",&htHorProj,&htVerProj);
							TupleMin2(htHorProj,40,&htHorProj);
							TupleMin2(htVerProj,40,&htVerProj);
							TupleFind(htHorProj,40,&htIndexHor);
							TupleFind(htVerProj,40,&htIndexVer);
							TupleLength(htIndexHor,&htLengthHor);
							TupleLength(htIndexVer,&hLengthVer);
							if(htLengthHor[0].D()>0)
							{
							Row12=htIndexHor[0];
							Row22=htIndexHor[htLengthHor-1];
							}
							if(hLengthVer[0].D()>0)
							{
							Column12=htIndexVer[0];
							Column22=htIndexVer[hLengthVer-1];
							}*/
							if(Row22[0].D()<htImgH[0].D())
							{
								nDownPos=Row22[0].D();
							}
						}
						else
						{
							nDownPos=m_sCheckPrintParam.sZouBanPara.m_nDownPos;
						}
					}
				}
				else
				{
					nDownPos=htImgH[0].D()-1;
				}
				//�ж��߰�
				int nZouBanMin=nDownPos
					-(m_sCheckPrintParam.sZouBanPara.m_fZouBanStdVer+m_sCheckPrintParam.sZouBanPara.m_fZouBanRange)/m_fScaleVer;
				int nZouBanMax=nDownPos
					-(m_sCheckPrintParam.sZouBanPara.m_fZouBanStdVer-m_sCheckPrintParam.sZouBanPara.m_fZouBanRange)/m_fScaleVer;
				if(hv_Row2[0].D()>nZouBanMax||hv_Row2[0].D()<nZouBanMin)
				{
					nRet = 5;
				}
				GenRectangle1(&m_hRealZoubanRegionD,MAX(MIN(nZouBanMin,htImgH[0].D()-1),0),0,
					MIN(MAX(nZouBanMax,0),htImgH[0].D()-1),htImgW[0].D()-1);
			}	
		}
	}

	// QR���ж��Ƿ�λʧ�� [9/13/2013 Denghl]
	if(m_iCheckMethod==2||m_iRealMethod==2)
	{
		//���θò��ֵļ��
		if (m_sCheckPrintParam.sQrlocShapePara.bValid)
		{
			GenRectangle1(&ho_Rectangle1, hv_Row1[0].D()-10, hv_Column1[0].D()-10, hv_Row1[0].D()+(10*ht_SizeHei[0].D()), 
				hv_Column1[0].D()+(10*ht_SizeWid[0].D()));
			GenRectangle1(&ho_Rectangle2, hv_Row1[0].D()-10, hv_Column2[0].D()-(10*ht_SizeWid[0].D()), 
				hv_Row1[0].D()+(10*ht_SizeHei[0].D()), hv_Column2[0].D()+10);
			GenRectangle1(&ho_Rectangle3, hv_Row2[0].D()-(10*ht_SizeHei[0].D()), hv_Column1[0].D()-10, 
				hv_Row2[0].D()+10, hv_Column1[0].D()+(10*ht_SizeWid[0].D()));
			GenRectangle1(&ho_Rectangle4, hv_Row2[0].D()-(10*ht_SizeHei[0].D()), hv_Column2[0].D()-(10*ht_SizeWid[0].D()), 
				hv_Row2[0].D()+10, hv_Column2[0].D()+10);

			Intersection(ho_Rectangle1, ho_Region1, &ho_RegionPart);			
			Connection(ho_RegionPart, &ho_ConnectedRegions);
			SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "holes_num", "and", 	1, 99999);
			CountObj(ho_SelectedRegions, &hv_Number);
			if (0 != (hv_Number[0].I()>0))
			{
				CountObj(ho_SelectedRegions, &hv_Number2);

				HTuple end_val122 = hv_Number2;
				HTuple step_val122 = 1;
				for (hv_Index1=1; hv_Index1.Continue(end_val122, step_val122); hv_Index1 += step_val122)
				{
					SelectObj(ho_SelectedRegions, &ho_ObjectSelected, hv_Index1);
					FillUp(ho_ObjectSelected, &ho_RegionFillUp);
					Difference(ho_RegionPart, ho_ObjectSelected, &ho_RegionDifference);
					Intersection(ho_RegionDifference, ho_RegionFillUp, &ho_RegionIntersection);
					OpeningRectangle1(ho_RegionIntersection,&ho_RegionIntersection,ht_SizeWid,ht_SizeHei);
					CountObj(ho_RegionIntersection, &hv_Number1);
					AreaCenter(ho_RegionIntersection, &hv_Area, &hv_Row, &hv_Column);
					AreaCenter(ho_ObjectSelected, &hv_Area1, &hv_Row0, &hv_Column0);
					HTuple htLen1,htLen2;
					TupleLength(hv_Area,&htLen1);
					TupleLength(hv_Area1,&htLen2);
					if (htLen1[0].I()>0&&htLen2[0].I()>0)
					{
						TupleAnd(hv_Area[0].D()>50, hv_Area1[0].D()>50, &hv_And);
						TupleAbs(hv_Row0-hv_Row, &hv_AbsRow);
						TupleAbs(hv_Column0-hv_Column, &hv_AbsCol);
						TupleAnd(hv_And, hv_AbsRow[0].D()<3, &hv_And1);
						TupleAnd(hv_And1, hv_AbsCol[0].D()<3, &hv_And2);
						if (0 != hv_And2[0].I())
						{
							hv_CountNum += 1;
							break;
						}
					}
					
				}
			}
			Intersection(ho_Rectangle2, ho_Region1, &ho_RegionPart);			
			Connection(ho_RegionPart, &ho_ConnectedRegions);
			SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "holes_num", "and", 
				1, 99999);
			CountObj(ho_SelectedRegions, &hv_Number);
			if (0 != (hv_Number[0].I()>0))
			{
				CountObj(ho_SelectedRegions, &hv_Number2);

				HTuple end_val151 = hv_Number2;
				HTuple step_val151 = 1;
				for (hv_Index1=1; hv_Index1.Continue(end_val151, step_val151); hv_Index1 += step_val151)
				{
					SelectObj(ho_SelectedRegions, &ho_ObjectSelected, hv_Index1);
					FillUp(ho_ObjectSelected, &ho_RegionFillUp);
					Difference(ho_RegionPart, ho_ObjectSelected, &ho_RegionDifference);
					Intersection(ho_RegionDifference, ho_RegionFillUp, &ho_RegionIntersection);
					OpeningRectangle1(ho_RegionIntersection,&ho_RegionIntersection,ht_SizeWid,ht_SizeHei);
					CountObj(ho_RegionIntersection, &hv_Number1);					
					AreaCenter(ho_RegionIntersection, &hv_Area, &hv_Row, &hv_Column);
					AreaCenter(ho_ObjectSelected, &hv_Area1, &hv_Row0, &hv_Column0);
					HTuple htLen1,htLen2;
					TupleLength(hv_Area,&htLen1);
					TupleLength(hv_Area1,&htLen2);
					if (htLen1[0].I()>0&&htLen2[0].I()>0)
					{
						TupleAnd(hv_Area[0].D()>50, hv_Area1[0].D()>50, &hv_And);
						TupleAbs(hv_Row0-hv_Row, &hv_AbsRow);
						TupleAbs(hv_Column0-hv_Column, &hv_AbsCol);
						TupleAnd(hv_And, hv_AbsRow[0].D()<3, &hv_And1);
						TupleAnd(hv_And1, hv_AbsCol[0].D()<3, &hv_And2);
						if (0 != hv_And2[0].I())
						{
							hv_CountNum = hv_CountNum[0].D() + 1;
							break;
						}
					}
					
				}
			}
			Intersection(ho_Rectangle3, ho_Region1, &ho_RegionPart);			
			Connection(ho_RegionPart, &ho_ConnectedRegions);
			SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "holes_num", "and", 
				1, 999999);
			CountObj(ho_SelectedRegions, &hv_Number);
			if (0 != (hv_Number[0].I()>0))
			{
				CountObj(ho_SelectedRegions, &hv_Number2);

				HTuple end_val180 = hv_Number2;
				HTuple step_val180 = 1;
				for (hv_Index1=1; hv_Index1.Continue(end_val180, step_val180); hv_Index1 += step_val180)
				{
					SelectObj(ho_SelectedRegions, &ho_ObjectSelected, hv_Index1);
					FillUp(ho_ObjectSelected, &ho_RegionFillUp);
					Difference(ho_RegionPart, ho_ObjectSelected, &ho_RegionDifference);
					Intersection(ho_RegionDifference, ho_RegionFillUp, &ho_RegionIntersection);
					OpeningRectangle1(ho_RegionIntersection,&ho_RegionIntersection,ht_SizeWid,ht_SizeHei);
					CountObj(ho_RegionIntersection, &hv_Number1);
					AreaCenter(ho_RegionIntersection, &hv_Area, &hv_Row, &hv_Column);
					AreaCenter(ho_ObjectSelected, &hv_Area1, &hv_Row0, &hv_Column0);
					HTuple htLen1,htLen2;
					TupleLength(hv_Area,&htLen1);
					TupleLength(hv_Area1,&htLen2);
					if (htLen1[0].I()>0&&htLen2[0].I()>0)
					{
						TupleAnd(hv_Area[0].D()>50, hv_Area1[0].D()>50, &hv_And);
						TupleAbs(hv_Row0-hv_Row, &hv_AbsRow);
						TupleAbs(hv_Column0-hv_Column, &hv_AbsCol);
						TupleAnd(hv_And, hv_AbsRow[0].D()<3, &hv_And1);
						TupleAnd(hv_And1, hv_AbsCol[0].D()<3, &hv_And2);
						if (0 != hv_And2[0].I())
						{
							hv_CountNum = hv_CountNum[0].D()+ 1;
							break;
						}
					}
					
				}
			}
			Intersection(ho_Rectangle4, ho_Region1, &ho_RegionPart);			
			Connection(ho_RegionPart, &ho_ConnectedRegions);
			SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "holes_num", "and", 
				1, 99999);
			CountObj(ho_SelectedRegions, &hv_Number);
			if (0 != (hv_Number[0].I()>0))
			{
				CountObj(ho_SelectedRegions, &hv_Number2);
				HTuple end_val209 = hv_Number2;
				HTuple step_val209 = 1;
				for (hv_Index1=1; hv_Index1.Continue(end_val209, step_val209); hv_Index1 += step_val209)
				{
					SelectObj(ho_SelectedRegions, &ho_ObjectSelected, hv_Index1);
					FillUp(ho_ObjectSelected, &ho_RegionFillUp);
					Difference(ho_RegionPart, ho_ObjectSelected, &ho_RegionDifference);
					Intersection(ho_RegionDifference, ho_RegionFillUp, &ho_RegionIntersection);
					OpeningRectangle1(ho_RegionIntersection,&ho_RegionIntersection,ht_SizeWid,ht_SizeHei);
					CountObj(ho_RegionIntersection, &hv_Number1);
					AreaCenter(ho_RegionIntersection, &hv_Area, &hv_Row, &hv_Column);
					AreaCenter(ho_ObjectSelected, &hv_Area1, &hv_Row0, &hv_Column0);
					HTuple htLen1,htLen2;
					TupleLength(hv_Area,&htLen1);
					TupleLength(hv_Area1,&htLen2);
					if (htLen1[0].I()>0&&htLen2[0].I()>0)
					{
						TupleAnd(hv_Area[0].D()>50, hv_Area1[0].D()>50, &hv_And);
						TupleAbs(hv_Row0-hv_Row, &hv_AbsRow);
						TupleAbs(hv_Column0-hv_Column, &hv_AbsCol);
						TupleAnd(hv_And, hv_AbsRow[0].D()<3, &hv_And1);
						TupleAnd(hv_And1, hv_AbsCol[0].D()<3, &hv_And2);
						if (0 != hv_And2[0].I())
						{
							hv_CountNum = hv_CountNum[0].D() + 1;
							break;
						}
					}
					
				}
			}
			if (0 != (hv_CountNum[0].D()<3))
			{
				nRet = 4;
			}
		}			
	}//QR���ж��Ƿ�λʧ�ܽ���
	return nRet;
}

//��������
s_Status CInnerCheck::AlgCopyRegion(const s_AlgImageInfo& sAlgImageInfo,
	s_RegionProperty &sDstRegionProperty,const s_RegionProperty &sSrcRegionProperty)
{
	// �����㷨�������������Ҫ���Ӵ��� [5/18/2011 Denghl]
	return m_sStatus;
}
BOOL CInnerCheck::GenCurLog(CString &str,int nLogType)
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

		if(m_lstStr!=NULL)
		{
			if(m_lstStr->size()>1000)
			{
				m_lstStr->clear();
			}
			m_lstStr->push_back(str);
		}
	}
	catch(...)
	{
		return FALSE;
	}
	return TRUE;
}