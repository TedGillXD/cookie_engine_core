#pragma once
#include "Actor/Actor.h"
#include <unordered_map>
#include <string>
#include <vector>

namespace Cookie::Core {

	class ActorFactory {
	public:
		virtual Actor* Create() = 0;
		virtual ~ActorFactory() = default;
	};

	class ActorRegistrey {
	public:
		static bool RegisterActorFactory(const std::string& type, ActorFactory* factory);

		static Actor* CreateActor(const std::string& type);

		static std::vector<std::string>& GetAllActorTypes();

	private:
		inline static std::vector<std::string> _types;
		static std::unordered_map<std::string, ActorFactory*>& GetFactories();
	};
}

#ifndef REGISTER_ACTOR
#define REGISTER_ACTOR(ActorType)										\
class ActorType;														\
namespace {																\
	class ActorType##Factory : public Cookie::Core::ActorFactory{		\
	public:																\
		Actor * Create() const override {								\
			return new ActorType();										\
		}																\
	};																	\
	static bool bIsRegistered =										\
	Cookie::Core::ActorRegistrey::RegisterActorFactory(#ActorType, new ActorType##Factory()); \
}	// anonymous namespace
#endif