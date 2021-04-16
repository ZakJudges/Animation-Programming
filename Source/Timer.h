#ifndef _H_TIMER
#define _H_TIMER

#include <chrono>

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

#endif