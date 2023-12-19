#include "Utils/Utils.h"
#include "Platform/Platform.h"
#include "Graphics/Direct3D12/Direct3DClient.h"
#include <iostream>

int main() {
	//TODO: Needs pre-initialization

	Cookie::Graphsic::Direct3DClient client;
	
	if (client.Init()) {
		while (!client.ShouldClose()) {
			//1. update window
			client.Update();

			//2. begin frame
			client.BeginFrame();

			//3. pre draw
			client.Predraw();

			//4. draw
			client.Draw();

			//5. present
			client.EndFrame();
			client.Present();
			
		}
	}

	return 0;
}