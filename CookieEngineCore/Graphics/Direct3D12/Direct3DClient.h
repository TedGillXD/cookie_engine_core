#pragma once
#include "Platform/Client.h"
#include "Platform/Window.h"
#include "Common/Common.h"


namespace Cookie::Graphsic {
#ifdef _WIN64

	class Direct3DDebug {
	private:
#ifdef _DEBUG
		Microsoft::WRL::ComPtr<ID3D12Debug6> _debug;
		Microsoft::WRL::ComPtr<IDXGIDebug1> _dxgiDebug;
#endif	// _DEBUG

	public:
		bool Init();
		void Shutdown();
	};

	struct alignas(64) Direct3DCommandSet {
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _cmdAlloc;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> _cmdList;

		inline bool Init(Microsoft::WRL::ComPtr<ID3D12Device10>& device) {
			if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc)))) {
				return false;
			}
			if (FAILED(device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&_cmdList)))) {
				return false;
			}
			return true;
		}

		inline void Reset() {
			_cmdAlloc->Reset();
			_cmdList->Reset(_cmdAlloc.Get(), nullptr);
		}

		inline void Release() {
			_cmdAlloc->Release();
			_cmdList->Release();
		}
	};

	class Direct3DClient : public Cookie::Platform::ClientBase {
	public:
		Direct3DClient();
		Direct3DClient(uint32_t width, uint32_t height, std::string windowTitle);
		~Direct3DClient() override;

	private:
		bool CreateCommandObjects();
		bool CreateSwapChain();
		bool CreateRenderTargetBuffer();
		void ReleaseRenderTargetBuffer();
		bool Resize();
		bool CloseAllCommandList();

		void FlushCommandQueue();

	public:
		void Render() override;
		void HandleWindowMessage() override;

		bool Init() override;
		void Update() override;
		bool ShouldClose() override;

		void BeginFrame() override;
		void Predraw() override;
		void Draw() override;
		void EndFrame() override;
		void Present() override;

		void Shutdown() override;

		void CloseClient() override;

		[[nodiscard]] inline Cookie::Platform::Window* const GetWindow() { return _window; }

	private:
		static constexpr uint32_t BufferCount = 2;
		Cookie::Platform::Window* _window;
		std::string _windowTitle;

		// device
		Microsoft::WRL::ComPtr<ID3D12Device10> _device;
		uint32_t rtvIncrement;
		uint32_t srvUavCbvIncrement;

		// queue, allocator, list
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> _cmdQueue;
		Direct3DCommandSet _frameBeginResource;
		Direct3DCommandSet _frameEndResource;

		// swapchain
		Microsoft::WRL::ComPtr<IDXGIFactory7> _dxgiFactory;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapChain;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _rtvHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource2> _renderTargetBuffer[BufferCount];
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[BufferCount];
		uint32_t _currentBackBufferIndex;

		// fence
		uint64_t _currentFence;
		Microsoft::WRL::ComPtr<ID3D12Fence1> _fence;

		// rasterizer
		D3D12_VIEWPORT _viewport;
		RECT _rect;

	};

#endif	// _WIN64
}