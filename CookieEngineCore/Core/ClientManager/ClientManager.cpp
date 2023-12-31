#include "ClientManager.h"


namespace Cookie::Core {

#ifdef _DEBUG
#ifdef _WIN64
	Graphsic::Direct3DDebug* ClientManager::_debugForD3D = nullptr;
#endif
#endif
	Platform::ClientBase* ClientManager::_client = nullptr;

	void ClientManager::Init(){

	}

	void ClientManager::CreateNewClient(const ClientConfiguration& configuration) {
		//check if _client already has a valid value, if yes, release first
		if (_client) {
			ReleaseClient();
		}

		//TODO: create client by using configuration
		if (configuration.Type == Direct3D12) {
		#ifdef _WIN64
			_debugForD3D = new Graphsic::Direct3DDebug();
			_debugForD3D->Init();
			_client = new Graphsic::Direct3DClient(
				configuration.D3D12Setting.width,
				configuration.D3D12Setting.height,
				configuration.D3D12Setting.windowTitle
			);
		#endif
		} else if(configuration.Type == Vulkan) {
			
		} else if(configuration.Type == Metal) {
		#ifdef __APPLE__
		#endif
		}
	}

	void ClientManager::ReleaseClient() {
		_client->~ClientBase();
		_client = nullptr;
		_debugForD3D->Shutdown();
		_debugForD3D = nullptr;
	}

	Platform::ClientBase* const ClientManager::GetClient() {
		return _client;
	}

	void ClientManager::RunClient() {
		if (!_client->Init()) {
			//TODO: Log out the client is failed to initialized
			return;
		}

		// main loop of the game
		while (!_client->ShouldClose()) {
			//1. update window
			_client->Update();

			//2. begin frame
			_client->BeginFrame();

			//3. pre draw
			_client->Predraw();

			//4. draw
			_client->Draw();

			//5. present
			_client->EndFrame();
			_client->Present();
		}
	}


}