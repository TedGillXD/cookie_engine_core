#pragma once
#include "Common/Common.h"
#include "Content/Content.h"
#include "Utils/Utils.h"

namespace Cookie::Core {
	
	/**
	 * ObjectManager class is a static class that manages all textures, points buffers, materials and objects
	 * All of them are stored in array, using index to access it.
	 */
	class ObjectManager {
	public:
		inline static bool IsReady() { return bIsReady; }

		// Read object from file
		static uint32_t Load2DObject(const std::string& objectPath);
		static uint32_t Load3DObject(const std::string& objectPath);

		// Get data from array
		[[nodiscard]] static Content::Object2D& GetObject2D(uint32_t index);
		[[nodiscard]] static Content::Object2D& GetObject2D(const std::string& key);
		[[nodiscard]] static Content::Material& GetMaterial(uint32_t index);
		[[nodiscard]] static Content::Material& GetMaterial(const std::string& key);
		[[nodiscard]] static Content::Texture& GetTexture(uint32_t index);
		[[nodiscard]] static Content::Texture& GetTexture(const std::string& key);
		[[nodiscard]] static Content::PointsBuffer& GetModel(uint32_t index);
		[[nodiscard]] static Content::PointsBuffer& GetModel(const std::string& key);

		// Remove from array
		static void RemoveObject2D(uint32_t index);
		static void RemoveObject2D(const std::string& key);

		static void GarbageCollection();
		
	private:
		static uint32_t LoadMaterial(const std::string& materialPath);

		static uint32_t LoadTexture(const std::string& texturePath);

		static uint32_t LoadModel(const std::string& modelPath);

		static void RemoveMaterial(uint32_t index);
		static void RemoveMaterial(const std::string& key);

		static void RemoveTexture(uint32_t index);
		static void RemoveTexture(const std::string& key);

		static void RemoveModel(uint32_t index);
		static void RemoveModel(const std::string& key);

		static bool Init();

		static uint32_t ReadAssetFromFile(
			const std::string& assetPath,
			const Content::DataType dataType
		);
		static uint32_t ReadAssetFromFile(
			const std::string& assetPath,
			const Content::DataType dataType,
			Content::ObjectType& objectType
		);

	private:
		static Util::HashMapArray<Content::PointsBuffer> _pointsBufferArr;
		static Util::HashMapArray<Content::Texture> _texturesArr;
		static Util::HashMapArray<Content::Material> _materialArr;
		static Util::HashMapArray<Content::Object2D> _object2DArr;
		//	Util::HashMapArray<Content::Object3D> _object3DArr;	// 3D object not be supported yet

		static bool bIsReady;
	};
}