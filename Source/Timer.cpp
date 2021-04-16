#include "Timer.h"
#include <iostream>


Timer::Timer(const char* taskName)
{
	m_startTimepoint = std::chrono::high_resolution_clock::now();
	m_task = taskName;
}

Timer::Timer()
{
	m_startTimepoint = std::chrono::high_resolution_clock::now();
	m_task = " ";
}

Timer::~Timer()
{
	Stop();
}

void Timer::Stop()
{
	auto endTimepoint = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

	auto duration = end - start;
	//	Convert to milliseconds.
	double ms = duration * 0.001;

	std::cout << m_task << "  took" << duration << "us (" << ms << "ms\n";
}



InstrumentationTimer::InstrumentationTimer(const char* taskName)
{
	m_startTimepoint = std::chrono::high_resolution_clock::now();
	m_task = taskName;
	m_isStopped = false;
}

InstrumentationTimer::~InstrumentationTimer()
{
	if (!m_isStopped)
	{
		Stop();
	}
}

void InstrumentationTimer::Stop()
{
	auto endTimepoint = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

	//	Microseconds.
	//auto duration = end - start;

	//uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
	//std::cout << m_task << "  took" << duration << "us (" << ms << "ms\n";
	Instrumentor::Get().WriteProfile(ProfileResult(m_task, start, end));



}