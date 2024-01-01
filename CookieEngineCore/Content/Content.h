#pragma once
#include "Object/Object.h"
#include "Common/Common.h"
#include "Utils/Utils.h"

#ifndef GET_BYTE
#define GET_BYTE(x) reinterpret_cast<const char*>(x)
#endif	// GET_BYTE

#ifndef WRITE_VALUE
#define WRITE_VALUE(x, type) GET_BYTE(&x), sizeof(type)
#endif	// WRITE_VALUE

#ifndef WRITE_BUFFER
#define WRITE_BUFFER(x, length) GET_BYTE(x), length
#endif	// WRITE_BUFFER

#ifndef SET_BYTE
#define SET_BYTE(x) reinterpret_cast<char*>(x)
#endif	// SET_BYTE

#ifndef READ_VALUE
#define READ_VALUE(x, type) SET_BYTE(&x), sizeof(type)
#endif

#ifndef READ_BUFFER
#define READ_BUFFER(x, length) SET_BYTE(x), length
#endif

namespace Cookie::Content {

	/**
	 * .casset file:	the customized file that stores the only games needed informations
	 * Format of .casset file:
	 * 1 byte: data type - texture, audio etc.
	 * Then depends on the type of data
	 * 
	 * For texture:
	 *	4 bytes for width
	 *	4 bytes for height
	 *	1 bytes for the texture format
	 *  4 bytes for the length of data (uint32_t)
	 *	the rest of data is the data of image.
	 * 
	 * For audio:
	 * 
	 * For material:
	 *	4 bytes for lengths of diffuse albedo path (uint32_t)
	 *	diffuse albedo path
	 *	4 bytes for lengths of fresnel map path	(uint32_t)
	 *	fresnel map path
	 *	4 bytes for lengths of normal map path (uint32_t)
	 *	normal map path
	 *	4 bytes for lengths of base color map (uint32_t)
	 *	
	 * For model:
	 *	1 bytes for object type
	 *  4 bytes for vertex buffer stride (uint32_t)
	 *	4 bytes for the length of vertex (uint32_t)
	 *	vertex buffer data
	 *	1 bytes for the type of each index (uint8_t)
	 *  4 bytes for the length of index (uint32_t)
	 *	index buffer data
	 * 
	 * For object:
	 *	1 bytes for obejct type
	 *	4 bytes for points buffer asset file path (uint32_t)
	 *	points buffer path
	 *	4 bytes for material asset file path (uint32_t)
	 *	material path
	 */

	enum class DataType : uint8_t {
		Texture = 0,
		Audio = 1,
		Material = 2,
		Object = 3,
		Model = 4,
	};

	enum class ObjectType : uint8_t {
		Null = 0,
		Obj2D = 1,
		Obj3D = 2,
	};

	class FilePathTools {
	public:
		inline static const std::string resourceFolderName = "./Resources/";

		static std::string getAssetFileStorePath(const std::string& assetFilePath);

	};

	// AssetGenerator class is a static class provides functions to create .casset files by jpg, png, mp3 etc.
	class AssetGenerator {
	public:

		/**
		 * Params:
		 *	filePath: path of the image, should include file extension
		 *	assetFilePath: path of the casset file that is going to be stored, can without extension
		 *	Example:
		 *		filePath: "textures/image.jpg",
		 *		assetFilePath: "texture1"
		 */
		static bool CreateAssetFileForImage(const std::string& filePath, const std::string& assetFilePath);

		/**
		 * Params:
		 *	filePath: path of the image, should include file extension
		 *	assetFilePath: path of the casset file that is going to be stored, can without extension
		 *	Example:
		 *		filePath: "textures/model.fbx",
		 *		assetFilePath: "model1"
		 */
		static bool CreateAssetFileFor3DModel(const std::string& filePath, const std::string& assetFilePath);
	
		/**
		 * This function will generate 4 points square that represent 2D model
		 * 4 points will be (1, 1), (1, -1), (-1, 1), (-1, -1)
		 * Params:
		 *	assetFilePath: path of the casset file that is going to be stored, can without extension
		 *	Example:
		 *		assetFilePath: "model1"
		 */
		static bool CreateAssetFileFor2DModel(const std::string& assetFilePath);

		/**
		 * This function will create casset file that represent material
		 * Params:
		 *	texturesPath[0]: diffuse albedo
		 *	texturesPath[1]: fresnel map
		 *	texturesPath[2]: normal map
		 *	texturesPath[3]: base color
		 *  assetFilePath: path of the casset file that is going to be stored, can without extension
		 */
		static bool CreateAssetFileForMaterial(const std::vector<std::string>& texturesPath, const std::string& assetFilePath);

		static bool CreateAssetFileFor2DObject(const std::string& materialPath, const std::string& modelPath, const std::string& assetFilePath);
	};

	// ContentLoader class is a static class provides functions to load only .casset files,
	// and generator data structures that can be used in game.
	class ContentLoader {
	private:
		static PointsBuffer LoadModel2D(std::ifstream& fileHandle);

		static PointsBuffer LoadModel3D(std::ifstream& fileHandle);

	public:
		/**
		 * Load texture from casset file
		 * Param:
		 *	filePath: the path of the casset file, can input without extension
		 */
		static Texture LoadTextureFromFile(const std::string& filePath);

		/**
		 * Load model from casset file
		 * Param:
		 *	filePath: the path of the casset file, can input without extension
		 */
		static PointsBuffer LoadModelFromFile(const std::string& filePath);

		/**
		 * Load material from casset file
		 * Param:
		 *	filePath: the path of the casset file, can input without extension
		 */
		static Material LoadMaterialFromFile(const std::string& filePath);


		static ObjectType GetObjectType(const std::string& filePath);
		/**
		 * Load object2D from casset file
		 * Param:
		 *	filePath: the path of the casset file, can input without extension
		 */
		static Object2D LoadObject2DFromFile(const std::string& filePath);

		static Object3D LoadObject3DFromFile(const std::string& filePath);
	};

}