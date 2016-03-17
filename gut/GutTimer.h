#ifndef _GUTTIMER_
#define _GUTTIMER_


#if defined(__linux__)

#include <stdint.h>

typedef union _LARGE_INTEGER
{
	struct
	{
		int32_t    LowPart;
		int32_t    HighPart;
	};

	int64_t     QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

#endif



class GutTimer
{
public:
	enum TimerState
	{
		GUTTIMER_RUNNING,
		GUTTIMER_STOPPED,
	};

	float m_fTime;
	TimerState m_eState;
	LARGE_INTEGER m_PerformanceCount;

public:
	GutTimer(void);
	float Start(void);
	float Stop(void);
	void  Restart(void);

	float GetTime_MS(void);
	float GetTime_Second(void);
	float GetTime(void) { return GetTime_Second(); }
};

#endif // _GUTTIMER_
