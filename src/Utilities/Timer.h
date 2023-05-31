#pragma once
#include <chrono>
class Timer
{
public:
	Timer() {}

	~Timer() {}
	
	void Start()
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	double Stop()
	{
		m_End = std::chrono::high_resolution_clock::now();
		m_Duration = m_End - m_Start;
		return m_Duration.count();
	}

private:
	std::chrono::time_point<std::chrono::steady_clock> m_Start, m_End;
	std::chrono::duration<double> m_Duration;
};