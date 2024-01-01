#include "Utils/Utils.h"
#include "Platform/Platform.h"
#include "Graphics/Direct3D12/Direct3DClient.h"
#include "ClientManager/ClientManager.h"
#include "ObjectManager/ObjectManager.h"
#include "Content/Content.h"
#include <iostream>

int main() {
	using namespace Cookie;

	//TODO: Needs pre-run initialization
	Core::ClientManager::Init();

	Core::ClientConfiguration config{};
	config.Type = Core::Direct3D12;
	config.D3D12Setting.width = 800;
	config.D3D12Setting.height = 600;
	config.D3D12Setting.windowTitle = const_cast<char*>("D3D12 Client");
	Core::ClientManager::CreateNewClient(config);

	Core::ClientManager::RunClient();

	Core::ClientManager::ReleaseClient();

	return 0;
}