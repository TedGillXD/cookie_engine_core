#include "ActorManager.h"
#include <type_traits>


namespace Cookie::Core {

	Util::HashMapArray<Actor*> ActorManager::_actorArr;

	bool ActorManager::_bIsReady = Init();

	bool ActorManager::Init() {
		_actorArr.Insert("empty actor", new Actor2D());

		return true;
	}

	void ActorManager::Tick(float deltaTime) {
		for (auto actor : _actorArr) {
			actor.second->Tick(deltaTime);
		}
	}

	void ActorManager::GarbageCollection() {
		for (std::pair<uint32_t, Actor*> actor : _actorArr) {
			if (actor.second->GetRefCount() == 0) {
				delete actor.second;
				_actorArr.Remove(actor.first);
			}
		}
	}

	void ActorManager::Release() {
		for (std::pair<uint32_t, Actor*> actor : _actorArr) {
			delete actor.second;
			_actorArr.Remove(actor.first);
		}
	}

	Actor* ActorManager::GetActor(uint32_t index) {
		return _actorArr.At(index);
	}

	void ActorManager::LoadActorFromLevel() {
		// reset ActorManager
		Release();
		Init();

		// TODO: need to implement

		// for test

	}

}