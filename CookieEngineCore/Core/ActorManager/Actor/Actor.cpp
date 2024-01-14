#include "Actor.h"
#include "Utils/Math/Math.h"
#include <cmath>

namespace Cookie::Core {

	// ========== class Actor ==========
	Actor::~Actor() {
		if (_translation != nullptr)
			delete[] _translation;
		_translation = nullptr;

		if (_rotation != nullptr)
			delete[] _rotation;
		_rotation = nullptr;

		if (_scale != nullptr)
			delete[] _scale;
		_scale = nullptr;
	}


	void Actor::ResetMatrix(float*& matrix) {
		if (matrix == nullptr) {
			matrix = new float[16];
		}

		for (uint32_t i = 0; i < 16; i++) {
			matrix[i] = 0;
		}

		matrix[0] = 1.f;
		matrix[5] = 1.f;
		matrix[10] = 1.f;
		matrix[15] = 1.f;
	}

	void Actor::Destroy() {
		_refCount = 0;
	}

	// ========= class Actor2D =========
	Actor2D::Actor2D() {
		_bIsModified = false;
		_zIndex = 0;
		_object2DIndex = 0;

		ResetMatrix(_translation);
		ResetMatrix(_rotation);
		ResetMatrix(_scale);
	}

	Actor2D::~Actor2D() {
		// automatically call ~Actor();
	}

	void Actor2D::SetActorLocation(Location location) {
		// do column-major order
		_translation[12] = location.x;
		_translation[13] = location.y;
		_translation[14] = 0;

		_zIndex = (uint32_t)location.z;

		_bIsModified = true;
	}

	void Actor2D::SetActorRotation(Rotation rotation) {
		float rollRad = rotation.roll * (float)PI / 180.0f;

		_rotation[0] = cos(rollRad);
		_rotation[1] = -sin(rollRad);
		_rotation[4] = sin(rollRad);
		_rotation[5] = cos(rollRad);

		_bIsModified = true;
	}

	void Actor2D::SetActorScale(Scale scale) {
		_scale[0] = scale.x;
		_scale[5] = scale.y;
		_scale[10] = scale.z;
		_bIsModified = true;
	}

	void Actor2D::SetActorTransform(Transform transform){
		SetActorLocation(transform.location);
		SetActorRotation(transform.rotation);
		SetActorScale(transform.scale);
	}

	Location Actor2D::GetActorLocation() const {
		return { _translation[12], _translation[13], _translation[14] };
	}

	Rotation Actor2D::GetActorRotation() const {
		float roll = std::atan2(_rotation[1], _rotation[0]) * 180.0f / (float)PI;
		return { 0.f, 0.f, roll };
	}

	Scale Actor2D::GetActorScale() const {
		return { _scale[0], _scale[5], _scale[10] };
	}

	Transform Actor2D::GetActorTransform() const {
		return { GetActorLocation(), GetActorRotation(), GetActorScale() };
	}

	const float* Actor2D::GetModelMatrix() {
		if (_bIsModified) {
			// do calculation
			auto* tempMatrix = new float[16];
			Util::Math::MatrixMultiplicationSIMD(_scale, _rotation, tempMatrix);
			Util::Math::MatrixMultiplicationSIMD(tempMatrix, _translation, _modelMatrix);

			_bIsModified = false;
		}

		return _modelMatrix;
	}

	void Actor2D::BeginPlay() {
		// should emtpy
	}

	void Actor2D::Tick(float deltaTime) {
		// should empty
	}

}