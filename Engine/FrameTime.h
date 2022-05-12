#pragma once
#include <chrono>
class FrameTime
{
public:
	FrameTime()
	{
		Start = std::chrono::steady_clock::now();
	}
private:
	std::chrono::steady_clock::time_point Start;
public:
	float DeltaTime()
	{
		const auto Now = std::chrono::steady_clock::now();
		std::chrono::duration<float> deltatime = Now - Start;
		const float time = deltatime.count();
		Start = Now;
		return time;
	}

};