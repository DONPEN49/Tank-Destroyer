
#include	<cinttypes>
#include	<ctime>
#include	<Windows.h>

namespace detail
{
	inline double GetPerformanceFrequency()
	{
		::LARGE_INTEGER frequency;
		::QueryPerformanceFrequency(&frequency);
		return static_cast<double>(frequency.QuadPart);
	}

	inline int64_t GetPerformanceCount()
	{
		::LARGE_INTEGER counter;
		::QueryPerformanceCounter(&counter);
		return counter.QuadPart;
	}

	inline uint64_t Get100NanosecSinceEpoch()
	{
		::FILETIME fileTime;
		::GetSystemTimeAsFileTime(&fileTime);
		return (static_cast<uint64_t>(fileTime.dwHighDateTime) << 32) + fileTime.dwLowDateTime - (11'644'473'600 * 10'000'000);
	}
}

namespace Time
{
	uint64_t GetSec()
	{
		static const double scale = 1 / detail::GetPerformanceFrequency();
		return static_cast<uint64_t>(detail::GetPerformanceCount() * scale);
	}

	uint64_t GetMillisec()
	{
		static const double scale = 1'000 / detail::GetPerformanceFrequency();
		return static_cast<uint64_t>(detail::GetPerformanceCount() * scale);
	}

	uint64_t GetMicrosec()
	{
		static const double scale = 1'000'000 / detail::GetPerformanceFrequency();
		return static_cast<uint64_t>(detail::GetPerformanceCount() * scale);
	}

	uint64_t GetNanosec()
	{
		static const double scale = 1'000'000'000 / detail::GetPerformanceFrequency();
		return static_cast<uint64_t>(detail::GetPerformanceCount() * scale);
	}

	uint64_t GetSecSinceEpoch()
	{
		return detail::Get100NanosecSinceEpoch() / 10'000'000;
	}

	uint64_t GetMillisecSinceEpoch()
	{
		return detail::Get100NanosecSinceEpoch() / 10'000;
	}

	uint64_t GetMicrosecSinceEpoch()
	{
		return detail::Get100NanosecSinceEpoch() / 10;
	}

	int32_t UTCOffsetMinutes()
	{
		long timeZone;
		::_get_timezone(&timeZone);
		return -timeZone / 60;
	}
}
