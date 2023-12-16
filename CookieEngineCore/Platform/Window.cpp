#include "Window.h"
#include "Input/Input.h"

// The implementation of Window class
namespace Cookie::Platform {
#ifdef _WIN64
	Window::Window() {
		_width = _defaultWidth;
		_height = _defaultHeight;
		_handle = nullptr;
		_bIsFullScreen = false;
		_bShouldClose = false;
		_wndClass = 0;
	}

	Window::Window(uint8_t width, uint8_t height) {
		_width = width;
		_height = height;
		_handle = nullptr;
		_bIsFullScreen = false;
		_bShouldClose = false;
		_wndClass = 0;
	}

	void Window::NewWindow(std::string windowTitle) {
		WNDCLASSEXW wcex{};
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_OWNDC;
		wcex.lpfnWndProc = OnWindowMessage;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandleW(nullptr);
		wcex.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
		wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = L"D3D12ExWndCls";
		wcex.hIconSm = LoadIconW(nullptr, IDI_APPLICATION);
		_wndClass = RegisterClassExW(&wcex);

		if (_wndClass == 0) {
			return;
		}

		_handle = CreateWindowExW(
			WS_EX_OVERLAPPEDWINDOW | WS_EX_APPWINDOW,
			(LPCWSTR)_wndClass,
			L"D3D12 Window",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			100, 100,
			_width, _height,
			nullptr,
			nullptr,
			wcex.hInstance,
			nullptr
		);

	}

	void Window::SetFullScreen(bool bIsFullScreen) {
		_bIsFullScreen = bIsFullScreen;
	}

	void Window::Display() {
		if (_bIsFullScreen) {
			ShowWindow(_handle, SHOW_FULLSCREEN);
		} else {
			ShowWindow(_handle, SW_SHOWNORMAL);
		}
	}

	Window::~Window() {
		if(_handle) DestroyWindow(_handle);
		if(_wndClass) UnregisterClassW((LPCWSTR)_wndClass, GetModuleHandleW(nullptr));
	}

#endif	// _WIN64

#ifdef __APPLE__
#if TARGET_OS_MAC

	

#endif	// TARGET_OS_MAC
#endif	// __APPLE__

#ifdef __linux__


#endif // __linux__

}