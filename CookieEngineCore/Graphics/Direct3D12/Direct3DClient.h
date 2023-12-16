#pragma once
#include "Platform/Client.h"
#include "Platform/Window.h"

namespace Cookie::Graphsic {
#ifdef _WIN64

	class Direct3DClient : public Cookie::Platform::ClientBase {
	public:
		Direct3DClient();
		Direct3DClient(uint8_t width, uint8_t height);
		virtual ~Direct3DClient();

	public:
		bool Init() override;
		void Update() override;
		bool ShouldClose() override;

	private:
		Cookie::Platform::Window* _window;
	};

#endif
}