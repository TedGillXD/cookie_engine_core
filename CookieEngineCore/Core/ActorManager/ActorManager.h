#pragma once
#include "Actor/Actor.h"
#include "ActorFactory.h"
#include "Utils/Utils.h"
#include "Content/Content.h"
#include "Core/ObjectManager/ObjectManager.h"
#include <unordered_map>
#include <string>

namespace Cookie::Core {

	// ActorManager is a manager that stores all the actors(scripts) and provide functions to spawn, destroy and control the actor
	class ActorManager {
	public:
		[[nodiscard]] inline static bool IsReady() { return _bIsReady; }

	public:
		static void Tick(float deltaTime);

	public:
		template<typename ActorType>
		static uint32_t SpawnActorAtTransform(std::string actorName, std::string objectPath, Transform transform, Content::ObjectType objectType);
		
		template<typename ActorType>
		static uint32_t SpawnActorAtTransform(std::string actorName, uint32_t objectIndex, Transform transform, Content::ObjectType objectType);

		static Actor* GetActor(uint32_t index);

		// clear all actors
		static void Release();

		static void GarbageCollection();

		static void LoadActorFromLevel();

		[[nodiscard]] inline static const Util::HashMapArray<Actor*>& GetAllActors() { return _actorArr; }

	private:
		static bool Init();

	private:
		static bool _bIsReady;

		static Util::HashMapArray<Actor*> _actorArr;
	};

	template<typename ActorType>
	uint32_t ActorManager::SpawnActorAtTransform(std::string actorName, std::string objectPath, Transform transform, Content::ObjectType objectType) {
		static_assert(std::is_base_of<Actor, ActorType>::value, "ActorType should be based of class Actor");

		uint32_t index = 0;

		if (objectType == Content::ObjectType::Obj2D) {
			static_assert(std::is_base_of<Actor, ActorType>::value, "ActorType should be based of class Actor2D to create a 2D actor");
			Actor* actor = new ActorType();
			actor->SetActorTransform(transform);

			uint32_t objectIndex = ObjectManager::Load2DObject(objectPath);
			actor->SetObjectIndex(objectIndex);

			index = _actorArr.Insert(objectPath, actor);
		}
		else if (objectType == Content::ObjectType::Obj3D) {
			// TODO: not support yet
		}

		return index;
	}

	template<typename ActorType>
	uint32_t ActorManager::SpawnActorAtTransform(std::string actorName, uint32_t objectIndex, Transform transform, Content::ObjectType objectType) {
		static_assert(std::is_base_of<Actor, ActorType>::value, "ActorType should be based of class Actor");

		uint32_t index = 0;

		if (objectType == Content::ObjectType::Obj2D) {
			static_assert(std::is_base_of<Actor2D, ActorType>::value, "ActorType should be based of class Actor2D to create a 2D actor");
			Actor* actor = new ActorType();
			actor->SetActorTransform(transform);
			actor->SetObjectIndex(objectIndex);

			index = _actorArr.Insert(actorName, actor);
		}
		else if (objectType == Content::ObjectType::Obj3D) {
			// TODO: not support yet
		}

		return index;
	}

}