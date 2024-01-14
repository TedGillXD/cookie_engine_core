#include "Timer.h"

namespace Cookie::Util {

	Timer::Timer() {
		Reset();
	}

	void Timer::Start() {
		_paused = false;
		if (!_isStarted) {
			_startTime = std::chrono::high_resolution_clock::now();
			_isStarted = true;
		}
		_lastTime = std::chrono::high_resolution_clock::now();
	}

	float Timer::DeltaTime() const {
		return _deltaTime;
	}

	float Timer::TotalTime() const {
		if (!_isStarted) return 0.0;

		auto endTime = _paused ? _pauseStartTime : std::chrono::high_resolution_clock::now();
		auto runningTime = endTime - _startTime;
		return std::chrono::duration<float>(runningTime - _totalPauseDuration).count();
	}

	void Timer::Reset() {
		_paused = true;
		_isStarted = false;
		_deltaTime = 0.0;
		_totalPauseDuration = std::chrono::high_resolution_clock::duration::zero();
		_startTime = std::chrono::high_resolution_clock::now();
		_lastTime = _startTime;
		_pauseStartTime = _startTime;
	}

	void Timer::Pause() {
		if (!_paused) {
			_pauseStartTime = std::chrono::high_resolution_clock::now();
			_paused = true;
		}
	}

	void Timer::UnPause() {
		if (_paused) {
			auto pauseEndTime = std::chrono::high_resolution_clock::now();
			_totalPauseDuration += pauseEndTime - _pauseStartTime;
			_lastTime = pauseEndTime;
			_paused = false;
		}
	}

	void Timer::Tick() {
		if (_paused) {
			_deltaTime = 0.0;
			return;
		}

		auto currentTime = std::chrono::high_resolution_clock::now();
		_deltaTime = std::chrono::duration<float>(currentTime - _lastTime).count();
		_lastTime = currentTime;
	}

}