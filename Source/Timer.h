#ifndef _H_TIMER
#define _H_TIMER

#include <chrono>
#include "Instrumentor.h"

//	RAII Timer.
//		Always time in release mode.
class Timer
{
public:
	Timer();
	Timer(const char*);
	void Stop();
	~Timer();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
	const char* m_task;
};


#define PROFILING 1
#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif

class InstrumentationTimer
{
public:
	InstrumentationTimer(const char*);
	void Stop();
	~InstrumentationTimer();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
	const char* m_task;
	bool m_isStopped;
};

#endif