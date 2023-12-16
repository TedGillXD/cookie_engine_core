#pragma once
#include <cstdint>
#include "Window.h"

namespace Cookie::Platform {

	enum ClientType : uint8_t {
		Direct3D12 = 0,
		Vulkan = 1,
		Metal = 2,
	};
	
	struct ClientConfiguration {
		ClientType Type;
		union {
			
		};
	};

	class ClientBase {
	public:
		virtual ~ClientBase() {  };

	public:
		virtual bool Init() = 0;
		virtual void Update() = 0;
		virtual bool ShouldClose() = 0;
	};

	class ClientFactory {
	public:
		static ClientBase* NewClient(ClientConfiguration& configuration);
	};


}