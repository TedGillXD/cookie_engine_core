#pragma once
#include "Utils/Utils.h"
#include "Platform/Platform.h"

namespace Cookie::Core {

	class ClientManager {
	private:
		// TODO: use Map to store all clients

	public:
		static void CreateNewClient();

		static void ReleaseClient();

		static const Cookie::Platform::ClientBase* const GetClient();
	};

}