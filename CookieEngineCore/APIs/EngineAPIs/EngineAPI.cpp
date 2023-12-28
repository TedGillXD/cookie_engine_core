#include "EngineAPI.h"
#include "Utils/Logger/Logger.h"
#ifdef _WIN64
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace Cookie::EngineAPI {

	bool ChangeWorkDirectory(std::string path) {
	#ifdef _WIN64
		std::wstring wstr(path.begin(), path.end());
		if (!SetCurrentDirectory(wstr.c_str())) {
			Util::logger.Log("Failed to change program work directory!", Util::Logger::Level::Error);
		}
	#else	// _WIN64
		// handle linux and mac os
		if (chdir(path.c_str()) != 0) {
			Util::logger.Log("Failed to change program work directory!", Util::Logger::Level::Error);
		}

	#endif	// _WIN64
		Util::logger.Log("Work directory changed successfully!", Util::Logger::Level::Info);
		return true;
	}

}