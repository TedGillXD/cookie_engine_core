#pragma once
#include "Common/Common.h"

#ifdef _WIN64
#include <windows.h>
#elif defined __APPLE__
#if TARGET_OS_MAC

#endif
#elif defined __linux__

#endif

namespace Cookie::Platform {

#ifdef _WIN64

	/**
	 * class Window is for creating window application in different operating system.
	 */
	class Window {
	public:
		Window();
		Window(uint8_t width, uint8_t height);
		~Window();
		DISABLE_MOVE_AND_COPY(Window);

	public:
		void NewWindow(const std::string& windowTitle);
		void SetFullScreen(bool bIsFullScreen);
		void Display();

		[[nodiscard]] inline const bool ShouldClose() const { return _bShouldClose; }
		[[nodiscard]] inline const bool IsSuccess() const { return _handle != nullptr; }
		[[nodiscard]] inline const HWND GetHandle() const { return _handle; }
		[[nodiscard]] inline const uint32_t GetWidth() const { return _width; }
		[[nodiscard]] inline const uint32_t GetHeight() const { return _height; }

	private:
		static constexpr uint32_t _defaultWidth = (uint32_t)800;
		static constexpr uint32_t _defaultHeight = (uint32_t)600;

		uint32_t _width;
		uint32_t _height;
		HWND _handle;
		ATOM _wndClass;
		bool _bIsFullScreen;
		bool _bShouldClose;
	};

#endif	// _WIN64

#ifdef __APPLE__
#if TARGET_OS_MAC

	

#endif	// TARGET_OS_MAC
#endif	// __APPLE__

#ifdef __linux__

	

#endif // __linux__
}