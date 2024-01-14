#include "ClientManager.h"
#include "Core/ObjectManager/ObjectManager.h"
#include "Core/ActorManager/ActorManager.h"


namespace Cookie::Core {

#ifdef _DEBUG
#ifdef _WIN64
	Graphsic::Direct3DDebug* ClientManager::_debugForD3D = nullptr;
#endif
#endif
	Platform::ClientBase* ClientManager::_client = nullptr;

	Cookie::Util::Timer ClientManager::_gameTimer;

	void ClientManager::Init(){
		if (!_client->Init()) {
			//TODO: Log out the client is failed to initialized
			_bReadyToRun = false;
			return;
		}

		_bReadyToRun = true;
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
		if (!_bReadyToRun) {
			return;
		}

		// before game running, start the global timer.
		_gameTimer.Start();

		// main loop of the game
		while (!_client->ShouldClose()) {
			// garbage collection
			Core::ObjectManager::GarbageCollection();

			// handle input and window resize
			_client->HandleWindowMessage();

			// update scripts and TODO: animations(animation not support yet)
			_gameTimer.Tick();
			Core::ActorManager::Tick(_gameTimer.DeltaTime());

			// handle input and render
			_client->Render();
		}

		Core::ActorManager::Release();
	}

}