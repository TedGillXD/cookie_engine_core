#include "Direct3DClient.h"

namespace Cookie::Graphsic {


#ifdef _WIN64
	Direct3DClient::Direct3DClient() {
		_window = new Cookie::Platform::Window();
	}

	Direct3DClient::Direct3DClient(uint8_t width, uint8_t height) {
		_window = new Cookie::Platform::Window(width, height);
	}

	Direct3DClient::~Direct3DClient() {
		Shutdown();
	}

	bool Direct3DClient::CreateCommandObjects() {
		//create command queue
		D3D12_COMMAND_QUEUE_DESC queueDesc{};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.NodeMask = 0;	//always 0 if the system only have 1 GPU, and this project will only support 1 GPU system
		queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// this type of queue can basically do anything.
		if (FAILED(_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue)))) {
			return false;
		}

		//create command allocator 
		if (FAILED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc)))) {
			return false;
		}

		//create command list
		if (FAILED(_device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&_cmdList)))) {
			return false;
		}

		return true;
	}

	bool Direct3DClient::CreateSwapChain() {
		// 1. create swap chain object
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc{};
		swapChainDesc.Width = _window->GetWidth();
		swapChainDesc.Height = _window->GetHeight();
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
		swapChainDesc.BufferCount = BufferCount;
		swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.Stereo = FALSE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		fullscreenDesc.Windowed = TRUE;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> temp;
		_dxgiFactory->CreateSwapChainForHwnd(_cmdQueue.Get(), _window->GetHandle(), &swapChainDesc, &fullscreenDesc, nullptr, &temp);
		if (FAILED(temp->QueryInterface(IID_PPV_ARGS(&_swapChain)))) {
			return false;
		}

		// 2. create descriptor heap to store render target view
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
		heapDesc.NodeMask = 0;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NumDescriptors = BufferCount;
		if (FAILED(_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_rtvHeap)))) {
			return false;
		}

		// 3. create heap handle
		auto firstHandle = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
		for (uint32_t i = 0; i < BufferCount; i++) {
			rtvHandles[i] = firstHandle;
			rtvHandles[i].ptr += i * rtvIncrement;
		}

		// 4. create buffer
		Resize();

		return true;
	}

	bool Direct3DClient::CreateRenderTargetBuffer() {
		for (uint32_t i = 0; i < BufferCount; i++) {
			if (FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargetBuffer[i])))) {
				return false;
			}

			//create new buffer
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
			rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = 0;
			rtvDesc.Texture2D.PlaneSlice = 0;
			_device->CreateRenderTargetView(_renderTargetBuffer[i].Get(), &rtvDesc, rtvHandles[i]);
		}

		return true;
	}

	void Direct3DClient::ReleaseRenderTargetBuffer() {
		for (auto& buffer : _renderTargetBuffer) {
			if(buffer) buffer->Release();
		}
	}

	bool Direct3DClient::Resize() {
		for (int i = 0; i < BufferCount; i++) {
			FlushCommandQueue();
		}
		ReleaseRenderTargetBuffer();
		
		if (!_window->UpdateWidthAndHeight()) {
			return false;
		}

		HRESULT result = _swapChain->ResizeBuffers(
			BufferCount,
			_window->GetWidth(),
			_window->GetHeight(),
			DXGI_FORMAT_UNKNOWN,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING
		);
		if (result == S_FALSE) {
			return false;
		}

		_window->NoNeedResize();
		CreateRenderTargetBuffer();

		return true;
	}

	void Direct3DClient::FlushCommandQueue() {
		_currentFence++;
		if (FAILED(_cmdQueue->Signal(_fence.Get(), _currentFence))) {
			// TODO: throw an exception
		}

		if (_fence->GetCompletedValue() < _currentFence) {
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

			if (FAILED(_fence->SetEventOnCompletion(_currentFence, eventHandle))) {
				// TODO: throw an exception
			}

			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}


	bool Direct3DClient::Init() {
		if (!_window)
			return false;
		_window->NewWindow("D3D12 Client");
		if(!_window->IsSuccess())
			return false;

		_window->Display();

		//enable d3d12 debug layer
		#ifdef _DEBUG
		if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&_debug)))) {
			return false;
		}
		_debug->EnableDebugLayer();
		if (FAILED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&_dxgiDebug)))) {
			return false;
		}
		#endif	// _DEBUG

		//1. create factory
		if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&_dxgiFactory)))) {
			return false;
		}

		//2. create device
		if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device)))) {
			return false;
		}
		rtvIncrement = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		srvUavCbvIncrement = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//3. create fence
		if (FAILED(_device->CreateFence(_currentFence, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)))) {
			return false;
		}

		//4. create command object: cmdQueue, cmdList
		if (!CreateCommandObjects()) {
			return false;
		}

		//5. create swap chain
		if (!CreateSwapChain()) {
			return false;
		}

		return true;
	}

	void Direct3DClient::Update() {
		//1. process window message
		MSG message;
		while (PeekMessageW(&message, _window->GetHandle(), 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}

		//2. Process resize event if needed.
		if (_window->NeedResize()) {
			Resize();
		}

		//TODO: 3. update render items

	}

	bool Direct3DClient::ShouldClose() {
		return _window->ShouldClose();
	}

	void Direct3DClient::BeginFrame() {
		// init command list first
		_cmdAlloc->Reset();
		_cmdList->Reset(_cmdAlloc.Get(), nullptr);

		// TODO: change the state of back buffer from present to render target
	}

	void Direct3DClient::Predraw() {
		
	}

	void Direct3DClient::Draw() {
		
	}

	void Direct3DClient::EndFrame() {
		// TODO: change the state of back buffer from render target to present

		//close commandlist and execute it.
		if (SUCCEEDED(_cmdList->Close())) {
			ID3D12CommandList* lists[] = { _cmdList.Get() };
			_cmdQueue->ExecuteCommandLists(1, lists);
			FlushCommandQueue();
		}
	}

	void Direct3DClient::Present() {
		_swapChain->Present(0, 0);
	}

	void Direct3DClient::Shutdown() {

		_fence->Release();

		for (auto& buffer : _renderTargetBuffer) {
			buffer->Release();
		}
		_swapChain->Release();

		_cmdList->Release();
		_cmdAlloc->Release();
		_cmdQueue->Release();

	#ifdef _DEBUG
		if (_dxgiDebug) {
			OutputDebugStringW(L"DXGI Reports living device objects:\n");
			_dxgiDebug->ReportLiveObjects( // the function that will report all the living com objects. Base on this, we can check whether we still have some com objects not been released.
				DXGI_DEBUG_ALL,
				DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)
			);
		}

		_dxgiDebug->Release();
		_debug->Release();
	#endif

		if (_window) {
			delete _window;
			_window = nullptr;
		}
	}

#endif
}