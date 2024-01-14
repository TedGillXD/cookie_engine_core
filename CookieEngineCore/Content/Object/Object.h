#pragma once
#include "Common/Common.h"
#include "Utils/Utils.h"

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

	class ObjectBase {
	public:
		inline void AddRef() { _refCount++; }
		inline void Dereference() {
			assert(_refCount > 0);
			if (_refCount > 0) {
				_refCount--;
			}
		}
		[[nodiscard]] inline uint32_t GetRef() { return _refCount; }

	protected:
		uint32_t _refCount;
	};

	// the object class is a abstract representation of a render item
	class PointsBuffer : public ObjectBase {
	public:
		/**
		 * vertexBuffer: the buffer that stores all vertices in bytes
		 * indexBuffer: the buffer that stores all indices in bytes
		 * vertexSizeInByte: the size of single vertex
		 * indexFormat: the type of single index
		 */
		PointsBuffer() : _vertexBufferStride(0) { _refCount = 0; }
		PointsBuffer(const std::vector<uint8_t>& vertexBuffer, const std::vector<uint8_t>& indexBuffer, uint32_t vertexSizeInByte, IndexType indexFormat);
		~PointsBuffer() = default;

	public:

		[[nodiscard]] inline const uint8_t* GetRawVertexBuffer() { return _vertexBuffer.data(); }
		[[nodiscard]] inline const uint8_t* GetRawIndexBuffer() { return _indexBuffer.data(); }
		[[nodiscard]] inline uint32_t GetVertexStride() { return _vertexBufferStride; }
		[[nodiscard]] inline uint8_t GetIndexStride() { return (uint8_t)_indexFormat; }

		[[nodiscard]] inline bool IsReadSuccessfully() {
			return IsValid();
		}

		[[nodiscard]] inline bool IsValid() {
			if (_vertexBufferStride == 0 || (_vertexBuffer.empty() && _indexBuffer.empty())) {
				return false;
			}
			return true;
		}

	private:
		std::vector<uint8_t>	_vertexBuffer;
		std::vector<uint8_t>	_indexBuffer;

		uint32_t				_vertexBufferStride = 0;	// the size of vertex struct
		IndexType				_indexFormat;				// the format of single index in index buffer
	};

	// Texture is a class that contains the raw data of a texture loaded from file
	class Texture : public ObjectBase {
	public:
		Texture();	// the default constructor is for create an empty texture
		Texture(std::vector<uint8_t> RawTexture, uint32_t width, uint32_t height, uint32_t bitPerPixel, uint32_t channelCount, TextureFormat format);
		~Texture() = default;

	public:		
		[[nodiscard]] inline const uint32_t Width() { return _width; }
		[[nodiscard]] inline const uint32_t Height() { return _height; }
		[[nodiscard]] inline const uint32_t BitPerPixel() { return _bitPerPixel; }
		[[nodiscard]] inline const uint32_t ChannelCount() { return _channelCount; }
		[[nodiscard]] inline const uint8_t* const GetRawTexture() { return _rawTexture.data(); }
		[[nodiscard]] inline const TextureFormat Format() { return _format; }

		[[nodiscard]] inline bool IsReadSuccessfully() {
			return IsValid();
		}

		[[nodiscard]] inline bool IsValid() {
			if (_width == 0 || _height == 0 || _rawTexture.empty() || _channelCount == 0) {
				return false;
			}
			return true;
		}

	private:
		uint32_t				_width = 0;
		uint32_t				_height = 0;
		uint32_t				_bitPerPixel = 0;
		uint32_t				_channelCount = 0;
		TextureFormat			_format;
		std::vector<uint8_t>	_rawTexture;
	};

	// Material is a class that contains Textures and other infos such as transformation, color...
	class alignas(64) Material : public ObjectBase {
	public:
		Material();
		Material(std::string diffuseAlbedo, std::string fresnelMap, std::string normal, std::string baseColor);
		~Material() = default;

		void Init(uint32_t diffuseIndex, uint32_t fresnelIndex, uint32_t normalMapIndex, uint32_t baseColorIndex);

		[[nodiscard]] inline const std::string& GetDiffuseAlbedoPath() { return _diffuseAlbedo; }
		[[nodiscard]] inline const std::string& GetFresnelMapPath() { return _fresnelMap; }
		[[nodiscard]] inline const std::string& GetNormalMapPath() { return _normal; }
		[[nodiscard]] inline const std::string& GetBaseColorPath() { return _baseColor; }
		[[nodiscard]] inline uint32_t GetDiffuseIndex() { return _diffuseIndex; }
		[[nodiscard]] inline uint32_t GetFresnelIndex() { return _fresnelInedx; }
		[[nodiscard]] inline uint32_t GetNormalMapIndex() { return _normalMapIndex; }
		[[nodiscard]] inline uint32_t GetBaseColorIndex() { return _baseColorIndex; }
		
		[[nodiscard]] inline bool IsReadSuccessfully() {
			if (_baseColor.empty()) {
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool IsValid() {
			return _bIsValid;
		}

	private:
		bool _bIsValid = false;

		//when string is empty, means the texture is not existed.
		std::string _diffuseAlbedo;	// the key of diffuse map
		std::string _fresnelMap;	// the key of fresnelR0
		std::string _normal;	// the key of normal map
		std::string _baseColor;	// the key of texture that provide base color info

		// index of each texture, when index == 0, means not valid
		uint32_t	_diffuseIndex = 0;
		uint32_t	_fresnelInedx = 0;
		uint32_t	_normalMapIndex = 0;
		uint32_t	_baseColorIndex = 0;
	};

	// 2D object raw data
	class Object2D : public ObjectBase {
	public:
		// constructors and destructor
		Object2D() {  }
		Object2D(std::string pointsBufferPath, std::string materialPath);
		Object2D(const Object2D& obj) = default;
		Object2D& operator=(const Object2D& obj) = default;
		Object2D(Object2D&& obj) = default;
		Object2D& operator=(Object2D&& obj) = default;
		~Object2D() = default;

		void Init(uint32_t materialIndex, uint32_t pointsBufferIndex);
		[[nodiscard]] inline const std::string& GetPointsBufferPath() { return _pointsBuffer; }
		[[nodiscard]] inline const std::string& GetMaterialPath() { return _material; }
		[[nodiscard]] inline uint32_t GetPointsBufferIndex() { return _pointsBufferIndex; }
		[[nodiscard]] inline uint32_t GetMaterialIndex() { return _materialIndex; }

		[[nodiscard]] inline bool IsReadSuccessfully() {
			if (_pointsBuffer.empty() || _material.empty()) {
				return false;
			}
			return true;
		}

		[[nodiscard]] inline bool IsValid() {
			if (_pointsBufferIndex == 0 || _materialIndex == 0) {
				return false;
			}
			return true;
		}

	private:
		std::string		_pointsBuffer;
		std::string		_material;
		uint32_t		_pointsBufferIndex = 0;
		uint32_t		_materialIndex = 0;
	};

	// 3D object raw data, will be developed in the future
	class Object3D : public ObjectBase {
	public:

	private:

	};
}