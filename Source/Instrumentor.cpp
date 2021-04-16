#include "Instrumentor.h"
#include <algorithm>

Instrumentor::Instrumentor()
{
	m_currentSession = nullptr;
	m_profileCount = 0;
}

//	Start the instrumentation session. During each session, all InstrumentationTimer objects will write to the same JSON file.
void Instrumentor::BeginSession(const std::string& name, const std::string& filepath)
{
	//	Open the file.
	m_outputStream.open(filepath);
	WriteHeader();
	m_currentSession = new InstrumentationSession(name);
}

void Instrumentor::EndSession()
{
	WriteFooter();
	//	Close the file.
	m_outputStream.close();
	delete m_currentSession;
	m_currentSession = nullptr;
	m_profileCount = 0;

}

//	Output the result of the timer to the JSON file.
void Instrumentor::WriteProfile(const ProfileResult& result)
{
	//	One profile per call of this function.
	if (m_profileCount++ > 0)
	{
		m_outputStream << ",";
	}

	std::string name = result.Name;
	std::replace(name.begin(), name.end(), '"', '\'');

	m_outputStream << "{";
	m_outputStream << "\"cat\":\"function\",";
	m_outputStream << "\"dur\":" << (result.End - result.Start) << ',';
	m_outputStream << "\"name\":\"" << name << "\",";
	m_outputStream << "\"ph\":\"X\",";
	m_outputStream << "\"pid\":0,";
	m_outputStream << "\"tid\":0,";
	m_outputStream << "\"ts\":" << result.Start;
	m_outputStream << "}";

	//	Flushing the stream ensures that the data will still be available if the program crashes.
	m_outputStream.flush();
	
}

//	Write the beginning of the JSON file.
void Instrumentor::WriteFooter()
{
	m_outputStream << "]}";
	m_outputStream.flush();
}

//	Write the end of the JSON file.
void Instrumentor::WriteHeader()
{
	m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
	m_outputStream.flush();
}

//	Singleton instrumentor.
Instrumentor& Instrumentor::Get()
{
	static Instrumentor instance;
	return instance;
}