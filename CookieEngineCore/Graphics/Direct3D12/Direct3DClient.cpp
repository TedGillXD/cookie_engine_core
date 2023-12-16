#include "Direct3DClient.h"

namespace Cookie::Graphsic {


#ifdef _WIN64
	Direct3DClient::Direct3DClient() {
		_window = new Cookie::Platform::Window();
	}

	Direct3DClient::Direct3DClient(uint8_t width, uint8_t height) {
		_window = new Cookie::Platform::Window(width, height);
	}

	Direct3DClient::~Direct3DClient() {
		_window->~Window();
		_window = nullptr;
	}

	bool Direct3DClient::Init() {
		if (!_window)
			return false;
		_window->NewWindow("D3D12 Client");
		if(!_window->IsSuccess())
			return false;

		_window->Display();

		//TODO: create swapchain, commandlist, commandqueue, etc.


		return true;
	}

	void Direct3DClient::Update() {

	}

	bool Direct3DClient::ShouldClose() {
		return _window->ShouldClose();
	}

#endif
}