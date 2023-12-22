#include "Window.h"
#include "Input/Input.h"
#include "Core/ClientManager/ClientManager.h"

// The implementation of Window class
namespace Cookie::Platform {
#ifdef _WIN64
	Window::Window() {
		_width = _defaultWidth;
		_height = _defaultHeight;
		_handle = nullptr;
		_bIsFullScreen = false;
		_bShouldClose = false;
		_bNeedResize = true;
		_wndClass = 0;
	}

	Window::Window(uint32_t width, uint32_t height) {
		_width = width;
		_height = height;
		_handle = nullptr;
		_bIsFullScreen = false;
		_bShouldClose = false;
		_bNeedResize = true;
		_wndClass = 0;
	}

	void Window::NewWindow(const std::string& windowTitle) {
		WNDCLASSEXW wcex{};
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_OWNDC;
		wcex.lpfnWndProc = ProcessWindow;
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
			std::wstring(windowTitle.begin(), windowTitle.end()).c_str(),
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

	bool Window::UpdateWidthAndHeight() {
		RECT rect;
		bool ret = GetClientRect(_handle, &rect);
		if (!ret) {
			return ret;
		}

		_width = rect.right - rect.left;
		_height = rect.bottom - rect.top;
		return true;
	}

	Window::~Window() {
		if(_handle) DestroyWindow(_handle);
		if(_wndClass) UnregisterClassW((LPCWSTR)_wndClass, GetModuleHandleW(nullptr));
	}

	LRESULT CALLBACK Window::ProcessWindow(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		auto window = ((Graphsic::Direct3DClient*)Core::ClientManager::GetClient())->GetWindow();
		switch (msg) {
		case WM_SIZE:	//	process resize event
			// Get the window and change the bNeedResize variable, width and height
			if (lParam && (HIWORD(lParam) != window->_height || LOWORD(lParam) != window->_width)) {
				window->_bNeedResize = true;
			}
			break;
		case WM_CLOSE:	// handle window close event
			Core::ClientManager::GetClient()->CloseClient();
			break;
		}
		return ProcessInputs(hwnd, msg, wParam, lParam);
	}

#endif	// _WIN64

#ifdef __APPLE__
#if TARGET_OS_MAC

	

#endif	// TARGET_OS_MAC
#endif	// __APPLE__

#ifdef __linux__


#endif // __linux__

}