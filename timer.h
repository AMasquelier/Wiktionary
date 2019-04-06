#pragma once
#include <chrono>

#ifdef WIN32
#include <windows.h>
#elif UNIX
#include <unistd.h>
#endif

class Clock
{
public:
	Clock() {}

	inline void start()
	{
		_start = std::chrono::high_resolution_clock::now();
	}

	// Return the duration in microseconds
	inline int duration()
	{
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::microseconds>(end - _start).count();
	}

	static void sleep(double ms)
	{
#ifdef WIN32
		Sleep(ms);
#elif UNIX
		usleep(ms);
#endif
	}

	// Return actual time in microseconds
	static int now()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}


private:
	std::chrono::high_resolution_clock::time_point _start;
};