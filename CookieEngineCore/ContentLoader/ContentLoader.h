#pragma once
#include "Object/Object.h"
#include "Common/Common.h"

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
	 *	4 bytes for the image format
	 *  4 bytes for the length of data
	 *	the rest of data is the data of image.
	 * 
	 * For audio:
	 * 
	 * For material:
	 *	
	 * For model:
	 */

	enum class DataType : uint8_t {
		Texture = 0,
		Audio = 1,
		Material = 2
	};

	// AssetGenerator class is a static class provides functions to create .casset files by jpg, png, mp3 etc.
	class AssetGenerator {
	private:

	public:
		inline static const std::string resourceFolderName = "./Resources/";

		/**
		 * filePath: path of the image, should include file extension
		 * assetFilePath: path of the casset file that is going to be stored, can without extension
		 * Example:
		 *	filePath: "textures/image.jpg",
		 *	assetFilePath: "texture1"
		 */
		static bool CreateAssetFileForImage(const std::string& filePath, const std::string& assetFilePath);
	
		/**
		 * filePath: path of the image, should include file extension
		 * assetFilePath: path of the casset file that is going to be stored, can without extension
		 * Example:
		 *	filePath: "textures/model.fbx",
		 *	assetFilePath: "model1"
		 */
		static bool CreateAssetFileForModel(const std::string& filePath, const std::string& assetFilePath);
	};

	// ContentLoader class is a static class provides functions to load only .casset files,
	// and generator data structures that can be used in game.
	class ContentLoader {
	private:
		//static void GetFileHandle(std::string filePath);

	public:
		static Texture LoadTextureFromFile(std::string filePath);

		static PointsBuffer LoadModelFromFile(std::string filePath);
	};

}