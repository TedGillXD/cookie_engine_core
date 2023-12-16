#include "Utils/Utils.h"
#include "Platform/Platform.h"
#include "Graphics/Direct3D12/Direct3DClient.h"
#include <iostream>

int main() {
	Cookie::Graphsic::Direct3DClient client;
	
	if (client.Init()) {
		while (!client.ShouldClose()) {
			//1. update window
			client.Update();
			//2. update objects status(add new object, update object position, etc.)

			
		}
	}

	return 0;
}