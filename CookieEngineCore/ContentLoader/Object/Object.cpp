#include "Object.h"

namespace Cookie::Content {

	PointsBuffer::PointsBuffer(const Util::Array<uint8_t>& vertexBuffer, const Util::Array<uint8_t>& indexBuffer, uint32_t vertexSizeInByte, IndexType indexFormat) {
		_vertexBuffer = vertexBuffer;
		_indexBuffer = indexBuffer;
		
		_vertexBufferStride = vertexSizeInByte;
		_indexFormat = indexFormat;
	}

	PointsBuffer::PointsBuffer(const PointsBuffer& obj) {
		this->_vertexBuffer = obj._vertexBuffer;
		this->_indexBuffer = obj._indexBuffer;
		this->_vertexBufferStride = obj._vertexBufferStride;
		this->_indexFormat = obj._indexFormat;
	}

	PointsBuffer::PointsBuffer(PointsBuffer&& obj) noexcept {
		this->_vertexBuffer = obj._vertexBuffer;
		this->_indexBuffer = obj._indexBuffer;
		this->_vertexBufferStride = obj._vertexBufferStride;
		this->_indexFormat = obj._indexFormat;

		obj._vertexBuffer = Util::Array<uint8_t>();
		obj._indexBuffer = Util::Array<uint8_t>();
		obj._indexFormat = IndexType::DEFAULT;
		obj._vertexBufferStride = 0;
	}

	PointsBuffer& PointsBuffer::operator=(const PointsBuffer& obj) {
		assert(this != std::addressof(obj));
		if (this != std::addressof(obj)) {
			this->_vertexBuffer = obj._vertexBuffer;
			this->_indexBuffer = obj._indexBuffer;
			this->_vertexBufferStride = obj._vertexBufferStride;
			this->_indexFormat = obj._indexFormat;
		}

		return *this;
	}

	PointsBuffer& PointsBuffer::operator=(PointsBuffer&& obj) noexcept {
		assert(this != std::addressof(obj));
		if (this != std::addressof(obj)) {
			this->_vertexBuffer = obj._vertexBuffer;
			this->_indexBuffer = obj._indexBuffer;
			this->_vertexBufferStride = obj._vertexBufferStride;
			this->_indexFormat = obj._indexFormat;

			obj._vertexBuffer = Util::Array<uint8_t>();
			obj._indexBuffer = Util::Array<uint8_t>();
			obj._indexFormat = IndexType::DEFAULT;
			obj._vertexBufferStride = 0;
		}

		return *this;
	}

	Texture::Texture() 
		: _width(0),
		_height(0), 
		_bitPerPixel(0),
		_channelCount(0),
		_format(TextureFormat::R8G8B8A8_UNORM) { }

	Texture::Texture(Util::Array<uint8_t> RawTexture, uint32_t width, uint32_t height, uint32_t bitPerPixel, uint32_t channelCount, TextureFormat format) 
		: _rawTexture(RawTexture), 
		_width(width), 
		_height(height), 
		_bitPerPixel(bitPerPixel), 
		_channelCount(channelCount), 
		_format(format){ }

	Material::Material() { }

	Material::Material(std::string diffuseAlbedo, std::string fresnelMap, std::string normal, std::string texture)
		: _diffuseAlbedo(diffuseAlbedo),
		_fresnelMap(fresnelMap),
		_normal(normal),
		_texture(texture) { }

}