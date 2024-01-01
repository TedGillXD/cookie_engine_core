#include "ObjectManager.h"


namespace Cookie::Core {
	
	// the initialization order can not be change, the Init() function should be called after these four arrays being initialized
	Util::HashMapArray<Content::PointsBuffer> ObjectManager::_pointsBufferArr;

	Util::HashMapArray<Content::Texture> ObjectManager::_texturesArr;

	Util::HashMapArray<Content::Material> ObjectManager::_materialArr;

	Util::HashMapArray<Content::Object2D> ObjectManager::_object2DArr;

	bool ObjectManager::bIsReady = Init();

	bool ObjectManager::Init() {
		_pointsBufferArr.Insert("", Content::PointsBuffer());
		_texturesArr.Insert("", Content::Texture());
		_materialArr.Insert("", Content::Material());
		_object2DArr.Insert("", Content::Object2D());

		return true;
	}

	uint32_t ObjectManager::ReadAssetFromFile(const std::string& assetPath, const Content::DataType dataType, Content::ObjectType& objectType) {

		switch (dataType)
		{
		case Content::DataType::Texture: {
			auto texture = Content::ContentLoader::LoadTextureFromFile(assetPath);
			if (texture.IsReadSuccessfully()) {
				return _texturesArr.Insert(assetPath, texture);
			}
			break;
		}
		case Content::DataType::Audio: {
			break;
		}
		case Content::DataType::Material: {
			auto material = Content::ContentLoader::LoadMaterialFromFile(assetPath);
			if (material.IsReadSuccessfully()) {
				return _materialArr.Insert(assetPath, material);
			}
			break;
		}
		case Content::DataType::Object: {
			if(objectType == Content::ObjectType::Null) break;

			if (objectType == Content::ObjectType::Obj2D) {
				auto object = Content::ContentLoader::LoadObject2DFromFile(assetPath);
				if(!object.IsReadSuccessfully()) break;

				return _object2DArr.Insert(assetPath, object);
			} else {	// TODO: process Object3D

			}
			break;
		}
		case Content::DataType::Model: {
			Content::PointsBuffer model = Content::ContentLoader::LoadModelFromFile(assetPath);
			if (model.IsReadSuccessfully()) {
				return _pointsBufferArr.Insert(assetPath, model);
			}
			break;
		}
		default:
			break;
		}

		return 0;
	}

	uint32_t ObjectManager::ReadAssetFromFile(const std::string& assetPath, const Content::DataType dataType) {
		Content::ObjectType objectType;
		return ReadAssetFromFile(assetPath, dataType, objectType);
	}

	uint32_t ObjectManager::Load2DObject(const std::string& objectPath) {
		// judge if the object is already exist
		if (_object2DArr.IsContain(objectPath)) {
			Util::logger.Log("Trying to load an exsit object: " + objectPath, Util::Logger::Level::Warning);
			return _object2DArr.GetIndexByKey(objectPath);
		}

		// judge if is 2d object
		Content::ObjectType objectType = Content::ContentLoader::GetObjectType(objectPath);
		if (objectType != Content::ObjectType::Obj2D) {
			Util::logger.Log("file " + objectPath + " is not a 2D object", Util::Logger::Level::Error);
			return 0;
		}

		// loading from file
		uint32_t index = ReadAssetFromFile(objectPath, Content::DataType::Object, objectType);
		if (index == 0) {
			Util::logger.Log("Load object failed: " + objectPath, Util::Logger::Level::Error);
			return 0;
		}

		// loading material and points buffer by using the path stored in object
		Content::Object2D& object2D = _object2DArr.At(index);
		uint32_t materialIndex = LoadMaterial(object2D.GetMaterialPath());
		uint32_t pointsBufferIndex = LoadModel(object2D.GetPointsBufferPath());
		object2D.Init(materialIndex, pointsBufferIndex);

		return index;
	}

	uint32_t ObjectManager::LoadMaterial(const std::string& materialPath) {
		if (_materialArr.IsContain(materialPath)) {
			Util::logger.Log("Trying to load an exsit material: " + materialPath, Util::Logger::Level::Warning);
			return _materialArr.GetIndexByKey(materialPath);
		}

		uint32_t index = ReadAssetFromFile(materialPath, Content::DataType::Material);
		if (index == 0) {
			Util::logger.Log("Load material failed: " + materialPath, Util::Logger::Level::Error);
			return 0;
		}

		// load textures by paths stores in material
		Content::Material& material = _materialArr.At(index);
		uint32_t diffuseAlbedoIndex = LoadTexture(material.GetDiffuseAlbedoPath());
		uint32_t fresnelMapIndex = LoadTexture(material.GetFresnelMapPath());
		uint32_t normalMapIndex = LoadTexture(material.GetNormalMapPath());
		uint32_t baseColorIndex = LoadTexture(material.GetBaseColorPath());
		material.Init(diffuseAlbedoIndex, fresnelMapIndex, normalMapIndex, baseColorIndex);

		return index;
	}

	uint32_t ObjectManager::LoadTexture(const std::string& texturePath) {
		if (texturePath.empty()) {
			return 0;
		}

		if (_texturesArr.IsContain(texturePath)) {
			Util::logger.Log("Trying to load an exsit texture: " + texturePath, Util::Logger::Level::Warning);
			return _texturesArr.GetIndexByKey(texturePath);
		}

		uint32_t index = ReadAssetFromFile(texturePath, Content::DataType::Texture);
		if (index == 0) {
			Util::logger.Log("Load texture failed: " + texturePath, Util::Logger::Level::Error);
			return 0;
		}

		return index;
	}

	uint32_t ObjectManager::LoadModel(const std::string& modelPath) {
		if (_pointsBufferArr.IsContain(modelPath)) {
			Util::logger.Log("Trying to load an exsit model: " + modelPath, Util::Logger::Level::Warning);
			return _pointsBufferArr.GetIndexByKey(modelPath);
		}

		uint32_t index = ReadAssetFromFile(modelPath, Content::DataType::Model);
		if (index == 0) {
			Util::logger.Log("Load model failed: " + modelPath, Util::Logger::Level::Error);
			return 0;
		}

		return index;
	}

	Content::Object2D& ObjectManager::GetObject2D(uint32_t index) {
		return _object2DArr.At(index);
	}

	Content::Object2D& ObjectManager::GetObject2D(const std::string& key) {
		return _object2DArr.At(key);
	}

	Content::Material& ObjectManager::GetMaterial(uint32_t index) {
		return _materialArr.At(index);
	}

	Content::Material& ObjectManager::GetMaterial(const std::string& key) {
		return _materialArr.At(key);
	}

	Content::Texture& ObjectManager::GetTexture(uint32_t index) {
		return _texturesArr.At(index);
	}

	Content::Texture& ObjectManager::GetTexture(const std::string& key) {
		return _texturesArr.At(key);
	}

	Content::PointsBuffer& ObjectManager::GetModel(uint32_t index) {
		return _pointsBufferArr.At(index);
	}

	Content::PointsBuffer& ObjectManager::GetModel(const std::string& key) {
		return _pointsBufferArr.At(key);
	}

	void ObjectManager::RemoveObject2D(uint32_t inedx) {
		_object2DArr.Remove(inedx);
	}

	void ObjectManager::RemoveObject2D(const std::string& key) {
		_object2DArr.Remove(key);
	}

	void ObjectManager::RemoveMaterial(uint32_t index) {
		_materialArr.Remove(index);
	}

	void ObjectManager::RemoveMaterial(const std::string& key) {
		_materialArr.Remove(key);
	}

	void ObjectManager::RemoveTexture(uint32_t index) {
		_texturesArr.Remove(index);
	}

	void ObjectManager::RemoveTexture(const std::string& key) {
		_texturesArr.Remove(key);
	}

	void ObjectManager::RemoveModel(uint32_t index) {
		_pointsBufferArr.Remove(index);
	}

	void ObjectManager::RemoveModel(const std::string& key) {
		_pointsBufferArr.Remove(key);
	}

}