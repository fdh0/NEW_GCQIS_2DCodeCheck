/************************************************************************/
/* Copyright (c) 2007, 中国大恒集团北京图像视觉技术分公司视觉系统部  
/* All rights reserved.													
/*																		
/* 文件名称： SpendTime.h
/* 摘要： 获得毫秒级的精确耗时。
/*
/* 当前版本： 1.0
/* 作者： 江卓佳
/* 完成日期： 2007年10月24日
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
		//起始时间
		int iStartDay = systemtimeStart.wDay;
		int iStartHour = systemtimeStart.wHour;
		int iStartMinute = systemtimeStart.wMinute;
		int iStartSecond = systemtimeStart.wSecond;
		int iStartMilSec = systemtimeStart.wMilliseconds;
		//结束时间
		int iEndDay = systemtimeEnd.wDay;
		int iEndHour = systemtimeEnd.wHour;
		int iEndMinute = systemtimeEnd.wMinute;
		int iEndSecond = systemtimeEnd.wSecond;
		int iEndMilSec = systemtimeEnd.wMilliseconds;
		//计算
		double dresult = 0.0;
		dresult = iEndDay - iStartDay;//天数差
		dresult = dresult * 24.0 + iEndHour - iStartHour;//小时差
		dresult = dresult * 60.0 + iEndMinute - iStartMinute;//分钟差
		dresult = dresult * 60.0 +iEndSecond-iStartSecond;//秒差
        dresult = dresult * 1000.0 +iEndMilSec-iStartMilSec;//毫秒差
		if(dresult<0.0)
		{
			return 0.0;
		}
		else
		{
			return dresult;
		}
	}
	// 开始
	void Start()
	{
		GetSystemTime(&m_lgiCounterStart);
	}

	// 结束
	int End()	// 返回值：耗时（单位：毫秒）
	{
		GetSystemTime(&m_lgiCounterEnd);
		
		m_dSpendTime = ComputeSpendTime(m_lgiCounterStart,m_lgiCounterEnd);

		return (int)m_dSpendTime;
	}
	
	// 获得耗时（单位：毫秒）
	int GetMillisecondInt()
	{
		return (int)(m_dSpendTime);
	}
	
	// 获得耗时（单位：毫秒）
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

	// 开始
	void Start()
	{
		QueryPerformanceCounter(&m_lgiCounterStart);
	}

	// 结束
	int End()	// 返回值：耗时（单位：毫秒）
	{
		QueryPerformanceCounter(&m_lgiCounterEnd);
		
		m_dSpendTime = (m_lgiCounterEnd.QuadPart - m_lgiCounterStart.QuadPart)*1.0 / m_lgiFrequency.QuadPart;

		return (int)m_dSpendTime;
	}
	
	// 获得耗时（单位：毫秒）
	int GetMillisecondInt()
	{
		return (int)(m_dSpendTime*1000);
	}
	
	// 获得耗时（单位：毫秒）
	double GetMillisecondDouble()
	{
		return (m_dSpendTime*1000);
	}*/

};

#endif// !defined(SpendTime)