#include "Utils/Utils.h"
#include "Platform/Platform.h"
#include "Graphics/Direct3D12/Direct3DClient.h"
#include "ClientManager/ClientManager.h"
#include <iostream>

int main() {
	//TODO: Needs pre-initialization

	Cookie::Core::ClientConfiguration config{};
	config.Type = Cookie::Core::Direct3D12;
	config.D3D12Setting.width = 800;
	config.D3D12Setting.height = 600;
	config.D3D12Setting.windowTitle = const_cast<char*>("D3D12 Client");
	Cookie::Core::ClientManager::CreateNewClient(config);

	auto client = Cookie::Core::ClientManager::GetClient();
	
	if (!client->Init()) {
		return -1;
	}

	// main loop of the game
	while (!client->ShouldClose()) {
		//1. update window
		client->Update();

		//2. begin frame
		client->BeginFrame();

		//3. pre draw
		client->Predraw();

		//4. draw
		client->Draw();

		//5. present
		client->EndFrame();
		client->Present();
	}

	Cookie::Core::ClientManager::ReleaseClient();

	return 0;
}