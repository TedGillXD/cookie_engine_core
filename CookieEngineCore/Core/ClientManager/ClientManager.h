#pragma once
#include "Utils/Utils.h"
#include "Platform/Platform.h"
#include <string>

#ifdef _WIN64
#include "Graphics/Direct3D12/Direct3DClient.h"
#endif

namespace Cookie::Core {

	enum ClientType : uint8_t {
		Direct3D12 = 0,
		Vulkan = 1,
		Metal = 2,
	};

	struct D3D12ClientSettings {
		uint32_t width;
		uint32_t height;
		char* windowTitle;
	};

	struct ClientConfiguration {
		ClientType Type;
		union {
			D3D12ClientSettings D3D12Setting;
		};
	};

	class ClientManager {
	private:
		static Platform::ClientBase* _client;

		// game global timer
		static Util::Timer _gameTimer;

		inline static bool _bReadyToRun = false;

#ifdef _DEBUG
#ifdef _WIN64
		static Graphsic::Direct3DDebug* _debugForD3D;	// for debug
#endif
#endif

	public:
		//initalize before running the app
		static void Init();

		//Create a new client and return its key
		static void CreateNewClient(const ClientConfiguration& configuration);

		static void RunClient();

		static void ReleaseClient();

		static Platform::ClientBase* const GetClient();

	};

}