#include "Object.h"

namespace Cookie::Content {

	PointsBuffer::PointsBuffer(const std::vector<uint8_t>& vertexBuffer, const std::vector<uint8_t>& indexBuffer, uint32_t vertexSizeInByte, IndexType indexFormat) {
		_vertexBuffer = vertexBuffer;
		_indexBuffer = indexBuffer;
		
		_vertexBufferStride = vertexSizeInByte;
		_indexFormat = indexFormat;

		_refCount = 0;
	}

	Texture::Texture() 
		: _width(0),
		_height(0), 
		_bitPerPixel(0),
		_channelCount(0),
		_format(TextureFormat::R8G8B8A8_UNORM) { _refCount = 0; }

	Texture::Texture(std::vector<uint8_t> RawTexture, uint32_t width, uint32_t height, uint32_t bitPerPixel, uint32_t channelCount, TextureFormat format) 
		: _rawTexture(RawTexture), 
		_width(width), 
		_height(height), 
		_bitPerPixel(bitPerPixel), 
		_channelCount(channelCount), 
		_format(format)  { _refCount = 0; }

	Material::Material()  { _refCount = 0; }

	Material::Material(std::string diffuseAlbedo, std::string fresnelMap, std::string normal, std::string texture)
		: _diffuseAlbedo(diffuseAlbedo),
		_fresnelMap(fresnelMap),
		_normal(normal),
		_baseColor(texture), 
		_bIsValid(false)  { _refCount = 0; }

	void Material::Init(uint32_t diffuseIndex, uint32_t fresnelIndex, uint32_t normalMapIndex, uint32_t baseColorIndex) {
		_diffuseIndex = diffuseIndex;
		_fresnelInedx = fresnelIndex;
		_normalMapIndex = normalMapIndex;
		_baseColorIndex = baseColorIndex;

		_bIsValid = true;
	}	

	Object2D::Object2D(std::string pointsBufferPath, std::string materialPath) 
		: _pointsBuffer(pointsBufferPath), _material(materialPath) { _refCount = 0; }

	void Object2D::Init(uint32_t materialIndex, uint32_t pointsBufferIndex) {
		_materialIndex = materialIndex;
		_pointsBufferIndex = pointsBufferIndex;
	}

}