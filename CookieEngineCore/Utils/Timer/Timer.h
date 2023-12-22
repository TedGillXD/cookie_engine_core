#pragma once
#include <chrono>
#include "Common/MacroDefinition.h"

namespace Cookie::Util {
	
	class Timer {
	public:
		Timer();
		DISABLE_COPY(Timer);

	public:
		[[nodiscard]] float TotalTime() const;
		[[nodiscard]] float DeltaTime() const;

		void Start();
		void Reset();
		void Pause();
		void UnPause();
		// Should be called every frame
		void Tick();

		[[nodiscard]] inline const bool IsPaused() const { return _paused; }
		[[nodiscard]] inline const bool IsStarted() const { return _isStarted; }

	private:
		bool _paused;
		bool _isStarted;
		float _deltaTime;
		std::chrono::high_resolution_clock::duration _totalPauseDuration;
		std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> _lastTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> _pauseStartTime;
	};

}