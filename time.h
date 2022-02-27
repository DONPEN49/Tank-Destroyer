#pragma once
#include	<cinttypes>

namespace detail {
	inline double GetPerformanceFrequency();
}

namespace Time {
	uint64_t GetSec();
	uint64_t GetMillisec();
	uint64_t GetMicrosec();
	uint64_t GetNanosec();
	uint64_t GetSecSinceEpoch();
	uint64_t GetMillisecSinceEpoch();
	uint64_t GetMicrosecSinceEpoch();
	int32_t UTCOffsetMinutes();
}
