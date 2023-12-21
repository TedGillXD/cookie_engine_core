#include "Utils/Utils.h"
#include "Platform/Platform.h"
#include "Graphics/Direct3D12/Direct3DClient.h"
#include <iostream>

int main() {
	//TODO: Needs pre-initialization

	Cookie::Util::HashSet<int> s;
	s.Insert(100);
	s.Insert(200);
	s.Insert(300);
	s.Insert(400);
	s.Insert(500);
	s.Insert(600);
	s.Insert(700);
	s.Insert(800);
	s.Insert(900);
	s.Insert(1000);
	s.Insert(1100);
	s.Insert(1200);
	s.Insert(1300);
	s.Insert(1400);

	for (auto& i : s) {
		std::cout << i << " ";
	}

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