#pragma once
#include "Common/Common.h"
#include "Utils/Utils.h"

#ifndef INDEX_TYPE
#define INDEX_TYPE  std::variant<uint8_t, uint16_t, uint32_t, uint64_t>
#endif	// INDEX_TYPE

namespace Cookie::Content {
	
	enum class IndexType : uint8_t{
		UINT8 = 8,
		UINT16 = 16,
		UINT32 = 32,
		UINT64 = 64,
		DEFAULT = UINT16
	};

	enum class TextureFormat : uint8_t {
		R8G8B8A8_UNORM = 0,
		B8G8R8A8_UNORM = 1,
	};

	struct alignas(32) Vertex2D {
		float x, y;			// the position of vertex
		float color[4];		// rgba of this vertex
		float u, v;			// uv coordinate of the vertex
	};

	// the object class is a abstract representation of a render item
	class PointsBuffer {
	public:
		/**
		 * vertexBuffer: the buffer that stores all vertices in bytes
		 * indexBuffer: the buffer that stores all indices in bytes
		 * vertexSizeInByte: the size of single vertex
		 * indexFormat: the type of single index
		 */
		PointsBuffer(const Util::Array<uint8_t>& vertexBuffer, const Util::Array<uint8_t>& indexBuffer, uint32_t vertexSizeInByte, IndexType indexFormat);
		PointsBuffer(const PointsBuffer& obj);
		PointsBuffer& operator=(const PointsBuffer& obj);
		PointsBuffer(PointsBuffer&& obj) noexcept;
		PointsBuffer& operator=(PointsBuffer&& obj) noexcept;
		~PointsBuffer() = default;

	public:

		[[nodiscard]] inline const uint8_t* GetRowVertexBuffer() { return _vertexBuffer.GetData(); }
		[[nodiscard]] inline const uint8_t* GetRowIndexBuffer() { return _indexBuffer.GetData(); }
		[[nodiscard]] inline const uint32_t GetVertexStride() { return _vertexBufferStride; }
		[[nodiscard]] inline const uint8_t GetIndexStride() { return (uint8_t)_indexFormat; }

		[[nodiscard]] inline bool IsValid() {
			if (_vertexBufferStride == 0 || (_vertexBuffer.Empty() && _indexBuffer.Empty())) {
				return false;
			}
			return true;
		}

	private:
		Util::Array<uint8_t>	_vertexBuffer;
		Util::Array<uint8_t>	_indexBuffer;

		uint32_t				_vertexBufferStride;	// the size of vertex struct
		IndexType				_indexFormat;			// the format of single index in index buffer
	};

	// Texture is a class that contains the raw data of a texture loaded from file
	class Texture {
	public:
		Texture();	// the default constructor is for create an empty texture
		Texture(Util::Array<uint8_t> RawTexture, uint32_t width, uint32_t height, uint32_t bitPerPixel, uint32_t channelCount, TextureFormat format);
		~Texture() = default;

	public:		
		[[nodiscard]] inline const uint32_t Width() { return _width; }
		[[nodiscard]] inline const uint32_t Height() { return _height; }
		[[nodiscard]] inline const uint32_t BitPerPixel() { return _bitPerPixel; }
		[[nodiscard]] inline const uint32_t ChannelCount() { return _channelCount; }
		[[nodiscard]] inline const uint8_t* const GetRawTexture() { return _rawTexture.GetData(); }
		[[nodiscard]] inline const TextureFormat Format() { return _format; }

		[[nodiscard]] inline bool IsValid() {
			if (_width == 0 || _height == 0 || _rawTexture.Empty() || _channelCount == 0) {
				return false;
			}
			return true;
		}

	private:
		uint32_t				_width;
		uint32_t				_height;
		uint32_t				_bitPerPixel;
		uint32_t				_channelCount;
		TextureFormat			_format;
		Util::Array<uint8_t>	_rawTexture;
	};

	// Material is a class that contains Textures and other infos such as transformation, color...
	class Material {
	public:
		Material();
		Material(std::string diffuseAlbedo, std::string fresnelMap, std::string normal, std::string texture);
		~Material() = default;

	private:
		//when string is empty, means the texture is not existed.
		std::string _diffuseAlbedo;	// the key of diffuse map
		std::string _fresnelMap;	// the key of fresnelR0
		std::string _normal;	// the key of normal map
		std::string _texture;	// the key of texture that provide base color info
	};

	// 2D object raw data
	class Object2D {
	public:
		// TODO: constructors and destructor
		~Object2D() = default;

	private:
		std::string		_pointsBuffer;
		std::string		_material;
		uint8_t			_zIndex;
	};

	// 3D object raw data, will be developed in the future
	class Object3D {

	};
}