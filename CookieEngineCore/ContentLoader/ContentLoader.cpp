#include "ContentLoader.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "Utils/ThirdParty/stb_image.h"


namespace Cookie::Content {

	std::string FilePathTools::getAssetFileStorePath(const std::string& assetFilePath) {
		std::string targetPath = resourceFolderName + assetFilePath;
		std::filesystem::path assetPath(assetFilePath);
		auto extionsion = assetPath.extension();
		if (extionsion.empty()) {	// if the input doesn't include the extension
			targetPath += ".casset";
		}
		else if (extionsion != ".casset") {	// if the input include the extionsion but not correct extension
			return "";
		}

		return targetPath;
	}

	bool AssetGenerator::CreateAssetFileForImage(const std::string& filePath, const std::string& assetFilePath) {

		//check if the assetFileName is already existed in location
		std::string targetPath = FilePathTools::getAssetFileStorePath(assetFilePath);
		if (targetPath.empty()) {
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

		uint8_t dataType = (uint8_t)DataType::Texture;
		asset.write(GET_BYTE(&dataType), sizeof(uint8_t));
		uint32_t uWidth = width, uHeight = height;
		asset.write(GET_BYTE(&uWidth), sizeof(uint32_t));	// width
		asset.write(GET_BYTE(&uHeight), sizeof(uint32_t));	// height

		uint8_t format = (uint8_t)TextureFormat::R8G8B8A8_UNORM;
		asset.write(GET_BYTE(&format), sizeof(uint8_t));	// format

		uint32_t sizeOfData = width * height * 4;
		asset.write(GET_BYTE(&sizeOfData), sizeof(uint32_t));	// size of data
		asset.write(GET_BYTE(data), sizeOfData);			// data
		asset.close();

		Util::logger.Log("Create texture successfully", Util::Logger::Level::Info);
		stbi_image_free(data);
		return true;
	}

	bool AssetGenerator::CreateAssetFileFor3DModel(const std::string& filePath, const std::string& assetFilePath) {
		// TODO: need to implement
		return false;
	}

	bool AssetGenerator::CreateAssetFileFor2DModel(const std::string& assetFilePath) {
		// check input and get the legal file path to store
		std::string targetPath = FilePathTools::getAssetFileStorePath(assetFilePath);
		if (targetPath.empty()) {
			Util::logger.Log("The extension of asset file path is incorrect, should be .casset", Util::Logger::Level::Error);
			return false;
		}

		// check the file if is exist
		if (std::filesystem::exists({ targetPath })) {
			Util::logger.Log("The asset file already exist in " + targetPath, Util::Logger::Level::Error);
			return false;
		}

		// create file
		std::ofstream asset(targetPath, std::ios::binary);
		if (!asset) {
			Util::logger.Log("Failed to create asset file " + targetPath, Util::Logger::Level::Error);
			return false;
		}

		// write data
		uint8_t dataType = (uint8_t)DataType::Model;
		asset.write(WRITE_VALUE(dataType, uint8_t));	// data type

		uint8_t objectType = (uint8_t)ObjectType::Obj2D;
		asset.write(WRITE_VALUE(objectType, uint8_t));	// object type: object 2D

		//vertex buffer
		uint32_t vertexBufferStride = sizeof(Vertex2D);
		asset.write(WRITE_VALUE(vertexBufferStride, uint32_t));
		Util::Array<Vertex2D> vertices({  // create vertices and indices buffer and write into file
			{ 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f }, 
			{ 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f },
			{ -1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f }, 
			{ -1.f, -1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f } 
		});
		uint32_t vertexBufferSize = vertices.GetSize() * sizeof(Vertex2D);
		asset.write(WRITE_VALUE(vertexBufferSize, uint32_t));	// vertex buffer size
		asset.write(WRITE_BUFFER(vertices.GetData(), vertexBufferSize));	// vertex buffer

		// index buffer
		IndexType indexType = IndexType::UINT16;
		asset.write(WRITE_VALUE(indexType, uint8_t));					// index type
		Util::Array<uint16_t> indices({ 0, 1, 3, 0, 3, 2 });
		uint32_t indexBufferSize = indices.GetSize() * sizeof(uint16_t);
		asset.write(WRITE_VALUE(indexBufferSize,uint32_t));				// index buffer size
		asset.write(WRITE_BUFFER(indices.GetData(), indexBufferSize));	// index buffer

		asset.close();
		Util::logger.Log("Create square successfully!", Util::Logger::Level::Info);
		return true;
	}

	bool AssetGenerator::CreateAssetFileForMaterial(const Util::Array<std::string>& texturesPath, const std::string& assetFilePath) {
		if (texturesPath.GetSize() != 4) {
			Util::logger.Log("texturesPath should have 4 paths", Util::Logger::Level::Error);
			return false;
		}
		
		std::string targetPath = FilePathTools::getAssetFileStorePath(assetFilePath);
		if (targetPath.empty()) {
			Util::logger.Log("The extension of asset file path is incorrect, should be .casset", Util::Logger::Level::Error);
			return false;
		}

		// check the file if is exist
		if (std::filesystem::exists({ targetPath })) {
			Util::logger.Log("The asset file already exist in " + targetPath, Util::Logger::Level::Error);
			return false;
		}

		// create file
		std::ofstream asset(targetPath, std::ios::binary);
		if (!asset) {
			Util::logger.Log("Failed to create asset file " + targetPath, Util::Logger::Level::Error);
			return false;
		}

		uint8_t dataType = (uint8_t)DataType::Material;
		asset.write(WRITE_VALUE(dataType, uint8_t));		// data type


		// write data into it
		for (const auto& path : texturesPath) {
			uint32_t length = path.length();
			asset.write(WRITE_VALUE(length, uint32_t));

			if (length == 0) continue;
			asset.write(path.c_str(), length);
		}

		asset.close();
		Util::logger.Log("Create material successfully");
		return true;
	}

	bool AssetGenerator::CreateAssetFileForObject(const std::string& assetFilePath) {
		// TODO: need to implement
		return false;
	}

	PointsBuffer ContentLoader::LoadModel2D(std::ifstream& fileHandle) {
		// vertex buffer
		uint32_t vertexBufferStride;
		fileHandle.read(READ_VALUE(vertexBufferStride, uint32_t));	// read vertex stride
		uint32_t vertexBufferLength;
		fileHandle.read(READ_VALUE(vertexBufferLength, uint32_t));				// read length of vertex buffer
		char* vertexBuffer = new char[vertexBufferLength];
		fileHandle.read(READ_BUFFER(vertexBuffer, vertexBufferLength));			// read vertex buffer

		// index buffer
		IndexType indexType;
		fileHandle.read(READ_VALUE(indexType, uint8_t));	// read index type
		uint32_t indexBufferLength;
		fileHandle.read(READ_VALUE(indexBufferLength, uint32_t));	// read index buffer length
		char* indexBuffer = new char[indexBufferLength];
		fileHandle.read(READ_BUFFER(indexBuffer, indexBufferLength));

		Util::Array<uint8_t> vertexBufferArr(reinterpret_cast<uint8_t*>(vertexBuffer), vertexBufferLength);
		Util::Array<uint8_t> indexBufferArr(reinterpret_cast<uint8_t*>(indexBuffer), indexBufferLength);

		delete[] vertexBuffer;
		delete[] indexBuffer;

		Util::logger.Log("Load model successful.");
		fileHandle.close();
		return PointsBuffer(vertexBufferArr, indexBufferArr, vertexBufferStride, indexType);
	}


	Texture ContentLoader::LoadTextureFromFile(const std::string& filePath) {
		std::string targetFilePath = FilePathTools::getAssetFileStorePath(filePath);
		std::ifstream file(targetFilePath, std::ios::binary);
		if (!file) {
			Util::logger.Log("Cannot open file: " + targetFilePath, Util::Logger::Level::Error);
			return {};
		}

		DataType dataType;
		file.read(READ_VALUE(dataType, uint8_t));
		if (dataType != DataType::Texture) {
			Util::logger.Log("File " + targetFilePath + " is not a texture.", Util::Logger::Level::Error);
			return {};
		}

		uint32_t width, height;
		file.read(READ_VALUE(width, uint32_t));	// read width from file
		file.read(READ_VALUE(height, uint32_t));	// read height from file
		
		TextureFormat textureFormat;
		file.read(READ_VALUE(textureFormat, uint8_t));	// read texture format from file

		uint32_t length;
		file.read(READ_VALUE(length, uint32_t));		// read the length of data

		char* buffer = new char[length];
		file.read(READ_BUFFER(buffer, length));

		Util::logger.Log("Read texture from file " + targetFilePath + " successfully");
		Util::Array<uint8_t> rawData(reinterpret_cast<uint8_t*>(buffer), length);
		delete[] buffer;
		file.close();
		return Texture(rawData, width, height, 32, 4, textureFormat);
	}

	PointsBuffer ContentLoader::LoadModelFromFile(const std::string& filePath) {
		std::string targetFilePath = FilePathTools::getAssetFileStorePath(filePath);
		std::ifstream file(targetFilePath, std::ios::binary);
		if (!file) {
			Util::logger.Log("Cannot open file: " + targetFilePath, Util::Logger::Level::Error);
			return { Util::Array<uint8_t>(), Util::Array<uint8_t>(), 0, IndexType::DEFAULT };
		}

		DataType dataType;
		file.read(READ_VALUE(dataType, uint8_t));		// read data type
		if (dataType != DataType::Model) {
			Util::logger.Log("File " + targetFilePath + " is not a model.", Util::Logger::Level::Error);
			return { Util::Array<uint8_t>(), Util::Array<uint8_t>(), 0, IndexType::DEFAULT };
		}

		ObjectType objectType;
		file.read(READ_VALUE(objectType, uint8_t));		// read object type
		if (objectType == ObjectType::Obj2D) {
			return LoadModel2D(file);
		} else {
			// TODO: implement LoadObject3D and call in here
		}

		// return emtpy points buffer 
		Util::logger.Log("The object type is not supported by Cookie Engine yet.", Util::Logger::Level::Error);
		file.close();
		return { Util::Array<uint8_t>(), Util::Array<uint8_t>(), 0, IndexType::DEFAULT };
	}

	Material ContentLoader::LoadMaterialFromFile(const std::string& filePath) {
		std::string targetFilePath = FilePathTools::getAssetFileStorePath(filePath);
		std::ifstream file(targetFilePath, std::ios::binary);
		if (!file) {
			Util::logger.Log("Cannot open file: " + targetFilePath, Util::Logger::Level::Error);
			return {};
		}

		DataType dataType;
		file.read(READ_VALUE(dataType, uint8_t));		// read data type
		if (dataType != DataType::Material) {
			Util::logger.Log("File " + targetFilePath + " is not a material.", Util::Logger::Level::Error);
			return {};
		}

		Util::Array<std::string> paths;
		for (uint32_t i = 0; i < 4; i++) {
			uint32_t length;
			file.read(READ_VALUE(length, uint32_t));
			if (length == 0)
				paths.PushBack("");
			char* str = new char[length];
			file.read(READ_BUFFER(str, length));
			paths.PushBack(str);
		}

		return Material(paths[0], paths[1], paths[2], paths[3]);
	}

	Object2D ContentLoader::LoadObject2DFromFile(const std::string& filePath) {
		// TODO: need to implement
		return {};
	}

}