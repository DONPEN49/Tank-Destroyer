#include "StopWatch.h"

bool operator <(const Stopwatch& s, const MicrosecondsF& time)
{
	return s.elapsed() < time;
}

bool operator <=(const Stopwatch& s, const MicrosecondsF& time)
{
	return s.elapsed() <= time;
}

bool operator >(const Stopwatch& s, const MicrosecondsF& time)
{
	return s.elapsed() > time;
}

bool operator >=(const Stopwatch& s, const MicrosecondsF& time)
{
	return s.elapsed() >= time;
}


bool operator <(const MicrosecondsF& time, const Stopwatch& s)
{
	return time < s.elapsed();
}

bool operator <=(const MicrosecondsF& time, const Stopwatch& s)
{
	return time <= s.elapsed();
}

bool operator >(const MicrosecondsF& time, const Stopwatch& s)
{
	return time > s.elapsed();
}

bool operator >=(const MicrosecondsF& time, const Stopwatch& s)
{
	return time >= s.elapsed();
}
