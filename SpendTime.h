/************************************************************************/
/* Copyright (c) 2007, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��  
/* All rights reserved.													
/*																		
/* �ļ����ƣ� SpendTime.h
/* ժҪ�� ��ú��뼶�ľ�ȷ��ʱ��
/*
/* ��ǰ�汾�� 1.0
/* ���ߣ� ��׿��
/* ������ڣ� 2007��10��24��
/************************************************************************/

#if !defined(SpendTime_h_)
#define SpendTime_h_
	
class CSpendTime
{
	protected:
	SYSTEMTIME m_lgiCounterStart;
	SYSTEMTIME m_lgiCounterEnd;
	double m_dSpendTime;
public:
	CSpendTime()
	{
		m_dSpendTime = 0.0;
        GetSystemTime(&m_lgiCounterStart);
		GetSystemTime(&m_lgiCounterEnd);
	}

	virtual ~CSpendTime()
	{
	}
    double ComputeSpendTime(SYSTEMTIME systemtimeStart,SYSTEMTIME systemtimeEnd)
	{
		//��ʼʱ��
		int iStartDay = systemtimeStart.wDay;
		int iStartHour = systemtimeStart.wHour;
		int iStartMinute = systemtimeStart.wMinute;
		int iStartSecond = systemtimeStart.wSecond;
		int iStartMilSec = systemtimeStart.wMilliseconds;
		//����ʱ��
		int iEndDay = systemtimeEnd.wDay;
		int iEndHour = systemtimeEnd.wHour;
		int iEndMinute = systemtimeEnd.wMinute;
		int iEndSecond = systemtimeEnd.wSecond;
		int iEndMilSec = systemtimeEnd.wMilliseconds;
		//����
		double dresult = 0.0;
		dresult = iEndDay - iStartDay;//������
		dresult = dresult * 24.0 + iEndHour - iStartHour;//Сʱ��
		dresult = dresult * 60.0 + iEndMinute - iStartMinute;//���Ӳ�
		dresult = dresult * 60.0 +iEndSecond-iStartSecond;//���
        dresult = dresult * 1000.0 +iEndMilSec-iStartMilSec;//�����
		if(dresult<0.0)
		{
			return 0.0;
		}
		else
		{
			return dresult;
		}
	}
	// ��ʼ
	void Start()
	{
		GetSystemTime(&m_lgiCounterStart);
	}

	// ����
	int End()	// ����ֵ����ʱ����λ�����룩
	{
		GetSystemTime(&m_lgiCounterEnd);
		
		m_dSpendTime = ComputeSpendTime(m_lgiCounterStart,m_lgiCounterEnd);

		return (int)m_dSpendTime;
	}
	
	// ��ú�ʱ����λ�����룩
	int GetMillisecondInt()
	{
		return (int)(m_dSpendTime);
	}
	
	// ��ú�ʱ����λ�����룩
	double GetMillisecondDouble()
	{
		return m_dSpendTime;
	}
/*protected:
	LARGE_INTEGER m_lgiCounterStart;
	LARGE_INTEGER m_lgiCounterEnd;
	LARGE_INTEGER m_lgiFrequency;
	double m_dSpendTime;
public:
	CSpendTime()
	{
		m_dSpendTime = 0.0;

		QueryPerformanceFrequency(&m_lgiFrequency);
	}

	virtual ~CSpendTime()
	{
	}

	// ��ʼ
	void Start()
	{
		QueryPerformanceCounter(&m_lgiCounterStart);
	}

	// ����
	int End()	// ����ֵ����ʱ����λ�����룩
	{
		QueryPerformanceCounter(&m_lgiCounterEnd);
		
		m_dSpendTime = (m_lgiCounterEnd.QuadPart - m_lgiCounterStart.QuadPart)*1.0 / m_lgiFrequency.QuadPart;

		return (int)m_dSpendTime;
	}
	
	// ��ú�ʱ����λ�����룩
	int GetMillisecondInt()
	{
		return (int)(m_dSpendTime*1000);
	}
	
	// ��ú�ʱ����λ�����룩
	double GetMillisecondDouble()
	{
		return (m_dSpendTime*1000);
	}*/

};

#endif// !defined(SpendTime)