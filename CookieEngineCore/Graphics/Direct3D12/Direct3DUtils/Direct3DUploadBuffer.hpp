#pragma once

#include "Common/Common.h"
#include "Common/MacroDefinition.h"
#include "Utils/Logger/Logger.h"

namespace Cookie::Graphsic {

	template<typename ResourceType>
	class UploadBuffer {
	public:
		DISABLE_MOVE_AND_COPY(UploadBuffer);

		UploadBuffer(Microsoft::WRL::ComPtr<ID3D12Device10>& device, uint32_t elementCount, bool isConstantBuffer = false) {
			_resourceSize = sizeof(ResourceType);
			_elementCount = elementCount;

			if (isConstantBuffer) {
				// resource size in constant buffer should be the multiply of 256 bytes
				_resourceSize = (_resourceSize + 255) & ~255;
			}

			D3D12_HEAP_PROPERTIES uploadBufferProps{};
			uploadBufferProps.VisibleNodeMask = 0;
			uploadBufferProps.Type = D3D12_HEAP_TYPE_UPLOAD;
			uploadBufferProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			uploadBufferProps.CreationNodeMask = 0;
			uploadBufferProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;

			D3D12_RESOURCE_DESC uploadResourceDesc{};
			uploadResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;	//the basic array of data.
			uploadResourceDesc.Alignment = 0;
			uploadResourceDesc.Width = _resourceSize * _elementCount;
			uploadResourceDesc.Height = 1;
			uploadResourceDesc.DepthOrArraySize = 1;
			uploadResourceDesc.MipLevels = 1;
			uploadResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
			uploadResourceDesc.SampleDesc.Count = 1;
			uploadResourceDesc.SampleDesc.Quality = 0;
			uploadResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR; //means in-order layout, other choices would propably reorder our data
			uploadResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;


			HRESULT hr = device->CreateCommittedResource(
				&uploadBufferProps,
				D3D12_HEAP_FLAG_NONE,
				&uploadResourceDesc,
				// common state will be automatically transit to proper state when it used in different senario,
				// it will be better if we can manually transit it to proper state so that we can obtain some kind of optimization
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(&_uploadResource)
			);

			if (FAILED(hr)) {
				_bIsValid = false;
				Util::logger.Log("Create committed resource failed.", Util::Logger::Level::Error)
			}
		}

		~UploadBuffer() {
			if (_uploadResource != nullptr) {
				_uploadResource->Unmap(0, nullptr);
			}

			_mappedData = nullptr;
		}

		[[nodiscard]] inline bool IsValid() {
			return _bIsValid;
		}

		[[nodiscard]] inline Microsoft::WRL::ComPtr<ID3D12Resource2>& GetResource() {
			return _uploadResource;
		}

		void CopyData(uint32_t elementIndex, const ResourceType& data) {
			memcpy(&_mappedData[elementIndex * _resourceSize], &data, sizeof(ResourceType));
		}

	private:
		bool _bIsValid = true;

		Microsoft::WRL::ComPtr<ID3D12Resource2> _uploadResource;
		uint8_t* _mappedData = nullptr;

		uint32_t _elementCount = 0;
		uint32_t _resourceSize = 0;

		bool _bIsConstantBuffer = false;
	};
}
