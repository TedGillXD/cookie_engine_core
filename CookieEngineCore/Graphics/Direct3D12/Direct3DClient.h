#pragma once
#include "Platform/Client.h"
#include "Platform/Window.h"
#include "Common/Common.h"


namespace Cookie::Graphsic {
#ifdef _WIN64

	class Direct3DClient : public Cookie::Platform::ClientBase {
	public:
		Direct3DClient();
		Direct3DClient(uint8_t width, uint8_t height);
		~Direct3DClient() override;

	private:
		bool CreateSwapChain();
		bool CreateRenderTargetBuffer();
		void ReleaseRenderTargetBuffer();

	public:
		bool Init() override;
		void Update() override;
		bool ShouldClose() override;

		void BeginFrame() override;
		void Predraw() override;
		void Draw() override;
		void EndFrame() override;
		void Present() override;

		void Shutdown() override;

	private:
		static constexpr uint32_t BufferCount = 2;
		Cookie::Platform::Window* _window;

		//device
		Microsoft::WRL::ComPtr<ID3D12Device10> _device;
		uint32_t rtvIncrement;
		uint32_t srvUavCbvIncrement;

		// queue, allocator, list
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> _cmdQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _cmdAlloc;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> _cmdList;

		// swapchain
		Microsoft::WRL::ComPtr<IDXGIFactory7> _dxgiFactory;
		Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapChain;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _rtvHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource2> _renderTargetBuffer[BufferCount];
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[BufferCount];

	#ifdef _DEBUG
		Microsoft::WRL::ComPtr<ID3D12Debug6> _debug;
		Microsoft::WRL::ComPtr<IDXGIDebug1> _dxgiDebug;
	#endif	// _DEBUG
	};

#endif	// _WIN64
}