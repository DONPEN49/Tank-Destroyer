#pragma once

#include	<cinttypes>
#include	<chrono>
#include	"time.h"

// using 宣言
using Duration = std::chrono::duration<double>;
using MicrosecondsF = std::chrono::duration<double, std::micro>;
using SecondsF = std::chrono::duration<double>;

class Stopwatch
{
private:

	int64_t m_startTimeMicrosec = 0;

	int64_t m_accumulationMicrosec = 0;

	bool m_isStarted = false;

	bool m_pausing = true;

public:

	/// <summary>
	/// ストップウォッチを作成します。
	/// </summary>
	/// <param name="startImmediately">
	/// 即座に計測を開始する場合は true
	/// </param>
	explicit Stopwatch(bool startImmediately = false)
	{
		if (startImmediately)
		{
			start();
		}
	}

	/// <summary>
	/// ストップウォッチを作成します。
	/// </summary>
	/// <param name="startImmediately">
	/// 即座に計測を開始する場合は true
	/// </param>
	explicit Stopwatch(const Duration& time, bool startImmediately = false)
	{
		set(time);

		if (startImmediately)
		{
			start();
		}
	}

	/// <summary>
	/// ストップウォッチを開始・再開します。
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void start()
	{
		if (!m_pausing)
		{
			return;
		}

		m_isStarted = true;

		m_pausing = false;

		m_startTimeMicrosec = Time::GetMicrosec();
	}

	/// <summary>
	/// ストップウォッチの経過時間を[日]で返します。
	/// </summary>
	/// <returns>
	/// ストップウォッチの経過時間[日]
	/// </returns>
	int32_t d() const { return static_cast<int32_t>(d64()); }

	int64_t d64() const { return us() / (1000LL * 1000LL * 60LL * 60LL * 24LL); }

	double dF() const { return static_cast<double>(us() / static_cast<double>(1000LL * 1000LL * 60LL * 60LL * 24LL)); }

	/// <summary>
	/// ストップウォッチの経過時間を[時]で返します。
	/// </summary>
	/// <returns>
	/// ストップウォッチの経過時間[時]
	/// </returns>
	int32_t h() const { return static_cast<int32_t>(h64()); }

	int64_t us() const
	{
		const int64_t t = Time::GetMicrosec();

		if (!m_isStarted)
		{
			return 0;
		}

		if (m_pausing)
		{
			return m_accumulationMicrosec;
		}

		return m_accumulationMicrosec + (t - m_startTimeMicrosec);
	}

	int64_t h64() const { return us() / (1000LL * 1000LL * 60LL * 60LL); }

	double hF() const { return static_cast<double>(us() / static_cast<double>(1000LL * 1000LL * 60LL * 60LL)); }

	/// <summary>
	/// ストップウォッチの経過時間を[分]で返します。
	/// </summary>
	/// <returns>
	/// ストップウォッチの経過時間[分]
	/// </returns>
	int32_t min32() const { return static_cast<int32_t>(min64()); }

	int64_t min64() const { return us() / (1000LL * 1000LL * 60LL); }

	double minF() const { return static_cast<double>(us() / static_cast<double>(1000LL * 1000LL * 60LL)); }

	/// <summary>
	/// ストップウォッチの経過時間を[秒]で返します。
	/// </summary>
	/// <returns>
	/// ストップウォッチの経過時間[秒]
	/// </returns>
	int32_t s() const { return static_cast<int32_t>(s64()); }

	int64_t s64() const { return us() / (1000LL * 1000LL); }

	double sF() const { return static_cast<double>(us() / static_cast<double>(1000LL * 1000LL)); }

	/// <summary>
	/// ストップウォッチの経過時間を[ミリ秒]で返します。
	/// </summary>
	/// <returns>
	/// ストップウォッチの経過時間[ミリ秒]
	/// </returns>
	int32_t ms() const { return static_cast<int32_t>(ms64()); }

	int64_t ms64() const { return us() / (1000LL); }

	double msF() const { return static_cast<double>(us() / static_cast<double>(1000LL)); }

	/// <summary>
	/// ストップウォッチの経過時間を[マイクロ秒]で返します。
	/// </summary>
	/// <returns>
	/// ストップウォッチの経過時間[マイクロ秒]
	/// </returns>
	

	int64_t us64() const
	{
		return us();
	}

	double usF() const { return static_cast<double>(us()); }

	/// <summary>
	/// ストップウォッチの経過時間を返します。
	/// </summary>
	/// <returns>
	/// ストップウォッチの経過時間
	/// </returns>
	Duration elapsed() const { return SecondsF(sF()); }

	/// <summary>
	/// ストップウォッチが動作中であるかを示します（一時停止していることもあります）。
	/// </summary>
	/// <remarks>
	/// ストップウォッチが開始されている、または開始後一時停止中である場合 true, それ以外の場合は false
	/// </remarks>
	bool isStarted() const { return m_isStarted; }

	/// <summary>
	/// ストップウォッチが一時停止中であるかを示します。
	/// </summary>
	/// <remarks>
	/// ストップウォッチが開始後一時停止中である場合 true, それ以外の場合は false
	/// </remarks>
	bool isPaused() const { return m_isStarted && m_pausing; }

	/// <summary>
	/// ストップウォッチが時間を計測中であるかを示します。
	/// </summary>
	/// <remarks>
	/// ストップウォッチが開始されていて、なおかつ一時停止中でない場合 true, それ以外の場合は false
	/// </remarks>
	bool isRunning() const { return m_isStarted && !m_pausing; }

	/// <summary>
	/// ストップウォッチを一時停止します。
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void pause()
	{
		m_accumulationMicrosec = us();

		m_pausing = true;
	}

	/// <summary>
	/// ストップウォッチが一時停止中である場合、再開します。
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void resume()
	{
		if (!isPaused())
		{
			return;
		}

		start();
	}

	/// <summary>
	/// ストップウォッチを停止し、経過時間を 0 にリセットします。
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void reset() noexcept
	{
		m_accumulationMicrosec = 0;

		m_isStarted = false;

		m_pausing = true;
	}

	/// <summary>
	/// 経過時間を 0 にリセットして、ストップウォッチを開始します。
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void restart()
	{
		reset();

		start();
	}

	/// <summary>
	/// ストップウォッチの経過時間を変更します。
	/// </summary>
	/// <param name="time">
	/// 新しく設定する経過時間
	/// </param>
	/// <returns>
	/// なし
	/// </returns>
	void set(const Duration& time)
	{
		m_isStarted = true;

		m_accumulationMicrosec = static_cast<int64_t>(time.count() * (1000LL * 1000LL));

		m_startTimeMicrosec = Time::GetMicrosec();
	}
};

bool operator <(const Stopwatch& s, const MicrosecondsF& time);

bool operator <=(const Stopwatch& s, const MicrosecondsF& time);

bool operator >(const Stopwatch& s, const MicrosecondsF& time);

bool operator >=(const Stopwatch& s, const MicrosecondsF& time);


bool operator <(const MicrosecondsF& time, const Stopwatch& s);

bool operator <=(const MicrosecondsF& time, const Stopwatch& s);

bool operator >(const MicrosecondsF& time, const Stopwatch& s);

bool operator >=(const MicrosecondsF& time, const Stopwatch& s);

