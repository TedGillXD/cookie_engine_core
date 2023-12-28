#include "Utils/Utils.h"
#include "Platform/Platform.h"
#include "Graphics/Direct3D12/Direct3DClient.h"
#include "ClientManager/ClientManager.h"
#include "ContentLoader/ContentLoader.h"
#include <iostream>

int main() {

	Cookie::Content::AssetGenerator::CreateAssetFileFor2DModel("square");
	Cookie::Content::ContentLoader::LoadModelFromFile("square");

	//TODO: Needs pre-run initialization
	Cookie::Core::ClientManager::Init();

	Cookie::Core::ClientConfiguration config{};
	config.Type = Cookie::Core::Direct3D12;
	config.D3D12Setting.width = 800;
	config.D3D12Setting.height = 600;
	config.D3D12Setting.windowTitle = const_cast<char*>("D3D12 Client");
	Cookie::Core::ClientManager::CreateNewClient(config);

	Cookie::Core::ClientManager::RunClient();

	Cookie::Core::ClientManager::ReleaseClient();

	return 0;
}