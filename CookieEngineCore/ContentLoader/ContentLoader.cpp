#include "ContentLoader.h"
#include "Utils/Utils.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "Utils/ThirdParty/stb_image.h"


namespace Cookie::Content {

	bool AssetGenerator::CreateAssetFileForImage(const std::string& filePath, const std::string& assetFilePath) {

		//check if the assetFileName is already existed in location
		std::string targetPath = assetFilePath;
		std::filesystem::path assetPath(assetFilePath);
		auto extionsion = assetPath.extension();
		if (extionsion.empty()) {	// if the input doesn't include the extension
			targetPath += ".casset";
		} else if (extionsion != ".casset") {	// if the input include the extionsion but not correct extension
			Util::logger.Log("The extension of asset file path is incorrect, should be .casset", Util::Logger::Level::Error);
			return false;
		}

		// check the asset file is already exist
		if (std::filesystem::exists({ targetPath })) {
			Util::logger.Log("The asset file already exist in " + targetPath, Util::Logger::Level::Error);
			return false;
		}

		//read the image info in
		int width;
		int height;
		int channel;
		// the data return by stbi_load is always be RGBA
		stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channel, 4);
		if (!data) {
			Util::logger.Log("Read image file " + filePath + " failed.", Util::Logger::Level::Error);
			return false;
		}

		// create the asset file that is going to be write
		std::ofstream asset(targetPath, std::ios::binary);
		if (!asset) {
			Util::logger.Log("Failed to create asset file " + targetPath, Util::Logger::Level::Error);
			return false;
		}

		int dataType = (int)DataType::Texture;
		asset.write(reinterpret_cast<const char*>(&dataType), sizeof(int));

		asset.write(reinterpret_cast<const char*>(&width), sizeof(int));	// width
		asset.write(reinterpret_cast<const char*>(&height), sizeof(int));	// height
		int format = (int)TextureFormat::R8G8B8A8_UNORM;
		asset.write(reinterpret_cast<const char*>(&format), sizeof(int));	// format
		int sizeOfData = width * height * 4;
		asset.write(reinterpret_cast<const char*>(&sizeOfData), sizeof(int));	// size of data
		asset.write(reinterpret_cast<const char*>(data), sizeOfData);			// data
		asset.close();

		Util::logger.Log("Load image successfully", Util::Logger::Level::Info);
		stbi_image_free(data);
		return true;
	}

	bool AssetGenerator::CreateAssetFileForModel(const std::string& filePath, const std::string& assetFilePath) {
		return false;
	}

	Texture ContentLoader::LoadTextureFromFile(std::string filePath) {
		return {};
	}

	PointsBuffer ContentLoader::LoadModelFromFile(std::string filePath) {
		return { Util::Array<uint8_t>(), Util::Array<uint8_t>(), 0, IndexType::DEFAULT };
	}

}