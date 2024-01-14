#include "ActorFactory.h"

namespace Cookie::Core {
	bool ActorRegistrey::RegisterActorFactory(const std::string& type, ActorFactory* factory) {
		GetFactories()[type] = factory;
		_types.push_back(type);
		return true;
	}

	Actor* ActorRegistrey::CreateActor(const std::string& type) {
		auto& factories = GetFactories();
		if (factories.find(type) != factories.end()) {
			return factories[type]->Create();
		}

		return nullptr;
	}

	std::unordered_map<std::string, ActorFactory*>& ActorRegistrey::GetFactories() {
		static std::unordered_map<std::string, ActorFactory*> factories;
		return factories;
	}

	std::vector<std::string>& ActorRegistrey::GetAllActorTypes() {
		return _types;
	}

}