#pragma once
#include <cstdint>
#include "Input/Input.h"
#include "Window.h"

namespace Cookie::Platform {

	class ClientBase {
	public:
		virtual ~ClientBase() {  };

		const Input _inputComponent;

	public:
		virtual void Render() = 0;
		virtual void HandleWindowMessage() = 0;

		virtual bool Init() = 0;
		virtual void Update() = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Predraw() = 0;
		virtual void Draw() = 0;
		virtual void Present() = 0;

		virtual bool ShouldClose() = 0;

		virtual void Shutdown() = 0;
		virtual void CloseClient() = 0;
	};

}