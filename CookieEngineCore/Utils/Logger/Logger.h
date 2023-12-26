#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <mutex>

namespace Cookie::Util {

	class Logger {
	public:
		enum class Level {
			Info,
			Warning,
			Error
		};


		void Log(const std::string& message, Level level = Level::Info) {
			std::lock_guard<std::mutex> lock(logMutex);
			std::ostringstream logMessage;
			logMessage << "[" << LevelToString(level) << "] " << message;

			std::cout << logMessage.str() << std::endl;
		}

	private:
		std::mutex logMutex;

		std::string LevelToString(Level level) {
			switch (level) {
			case Level::Info:
				return "INFO";
			case Level::Warning:
				return "WARNING";
			case Level::Error:
				return "ERROR";
			default:
				return "UNKNOWN";
			}
		}
	};

	static Logger logger = Logger();

}