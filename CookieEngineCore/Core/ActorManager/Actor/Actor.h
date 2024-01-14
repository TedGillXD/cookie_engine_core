#pragma once
#include "Common/Common.h"
#include "Utils/Utils.h"

namespace Cookie::Core {

	struct Location {
		float x;
		float y;
		float z;
	};

	struct Rotation {
		float pitch;
		float yaw;
		float roll;
	};

	struct Scale {
		float x;
		float y;
		float z;
	};

	struct Transform {
		Location location;
		Rotation rotation;
		Scale scale;
	};

	class Actor {
	public:
		virtual ~Actor();

	// script
	public:
		virtual void BeginPlay() = 0;
		virtual void Tick(float deltaTime) = 0;

	public:
		[[nodiscard]] inline uint32_t GetRefCount() { return _refCount; }

	public:
		inline void SetObjectIndex(uint32_t index) { _object2DIndex = index; }

	public:
		virtual void SetActorLocation(Location location) = 0;
		// input should be in angle
		virtual void SetActorRotation(Rotation rotation) = 0;
		virtual void SetActorScale(Scale scale) = 0;
		virtual void SetActorTransform(Transform transform) = 0;

		virtual Location GetActorLocation() const = 0;
		virtual Rotation GetActorRotation() const = 0;
		virtual Scale GetActorScale() const = 0;
		virtual Transform GetActorTransform() const = 0;

		// the length of the return value should be 16
		virtual const float* GetModelMatrix() = 0;

		virtual void Destroy();

		void ResetMatrix(float*& matrix);

	protected:
		uint32_t _refCount = 1;
		
		float* _translation = nullptr;
		float* _rotation = nullptr;
		float* _scale = nullptr;
		float* _modelMatrix = nullptr;

		uint32_t _object2DIndex = 0;
	};

	// Actor2D contains the per object data to support rendering
	// also this class is a base script class, all actor2D should inherit from this class and override the tick and beginplay function
	class Actor2D : public Actor {
	public:
		Actor2D();
		~Actor2D() override;

	public:
		void BeginPlay() override;
		void Tick(float deltaTime) override;

	// functions that provide for derived class
	public:

		/**
		 * Set actor location
		 * z will change the z index of this 2D actor
		 */
		void SetActorLocation(Location location) final;
		/**
		 * Set actor rotation
		 * pitch and yaw will never be used, only roll works
		 */
		void SetActorRotation(Rotation rotation) final;
		void SetActorScale(Scale scale) final;
		void SetActorTransform(Transform transform) final;

		Location GetActorLocation() const final;
		Rotation GetActorRotation() const final;
		Scale GetActorScale() const final;
		Transform GetActorTransform() const final;

		const float* GetModelMatrix() final;

	private:
		bool _bIsModified = false;
		uint32_t _zIndex = 0;
	};

	// TODO: Actor 3D not being support yet
	class Actor3D : public Actor {

	};

	class ActorTest : public Actor2D {
	public:
		ActorTest() { Actor2D(); }

		void BeginPlay() override {

		}

		void Tick(float deltaTime) override {
			Util::logger.Log("tick called " + std::to_string(deltaTime), Util::Logger::Level::Info);
		}
	};
}