#include "Direct3DClient.h"

namespace Cookie::Graphsic {


#ifdef _WIN64


	bool Direct3DDebug::Init() {
		//enable d3d12 debug layer
#ifdef _DEBUG
		if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&_debug)))) {
			return false;
		}
		_debug->EnableDebugLayer();
		if (FAILED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&_dxgiDebug)))) {
			return false;
		}

		return true;
#endif	// _DEBUG
	}

	void Direct3DDebug::Shutdown() {
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
	}

#pragma warning(disable : 26495)

	Direct3DClient::Direct3DClient() {
		_window = new Cookie::Platform::Window();
	}

	Direct3DClient::Direct3DClient(uint32_t width, uint32_t height, std::string windowTitle) {
		_windowTitle = windowTitle;
		_window = new Cookie::Platform::Window(width, height);
	}

	Direct3DClient::~Direct3DClient() {
		//Shutdown();
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

		if (!_frameBeginResource.Init(_device)) {
			return false;
		}

		if (!_frameEndResource.Init(_device)) {
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
			buffer.Reset();
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

		// get new viewport and rect when window is reiszed
		D3D12_VIEWPORT tempViewport{};
		tempViewport.Width = static_cast<float>(_window->GetWidth());
		tempViewport.Height = static_cast<float>(_window->GetHeight());
		tempViewport.TopLeftX = tempViewport.TopLeftY = 0;
		tempViewport.MinDepth = 0.f;
		tempViewport.MaxDepth = 1.f;
		_viewport = tempViewport;
		RECT tempRect{};
		tempRect = { 0, 0, (long)_window->GetWidth(), (long)_window->GetHeight() };
		_rect = tempRect;

		return true;
	}

	bool Direct3DClient::CloseAllCommandList() {
		if (FAILED(_frameBeginResource._cmdList->Close())) {
			return false;
		}

		//TODO: close all the commandlists

		if (FAILED(_frameEndResource._cmdList->Close())) {
			return false;
		}

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

		gameTimer.Start();
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

		//TODO: 3. update per frame data

		//4. Timer Tick
		gameTimer.Tick();
		float deltaTime = gameTimer.DeltaTime();

		//TODO: 5. call Tick function in each script

		//TODO: 6. update per object data
	}

	bool Direct3DClient::ShouldClose() {
		return _window->ShouldClose();
	}

	void Direct3DClient::BeginFrame() {
		// init command list first
		_frameBeginResource.Reset();
		_frameEndResource.Reset();

		// change the state of back buffer from present to render target
		_currentBackBufferIndex = _swapChain->GetCurrentBackBufferIndex();
		D3D12_RESOURCE_BARRIER barr{};
		barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barr.Transition.pResource = _renderTargetBuffer[_currentBackBufferIndex].Get();
		barr.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barr.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barr.Transition.Subresource = 0;
		barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		_frameBeginResource._cmdList->ResourceBarrier(1, &barr);

		// set render target
		float clearValue[4] = { 0.f, 0.f, 0.f, 1.f };
		_frameBeginResource._cmdList->ClearRenderTargetView(rtvHandles[_currentBackBufferIndex], clearValue, 0, nullptr);
		_frameBeginResource._cmdList->OMSetRenderTargets(1, &rtvHandles[_currentBackBufferIndex], false, nullptr);
	}

	void Direct3DClient::Predraw() {
		// Set for RS
		_frameBeginResource._cmdList->RSSetViewports(1, &_viewport);
		_frameBeginResource._cmdList->RSSetScissorRects(1, &_rect);

		// TODO: Setting for IA: vertex buffer view, index buffer view
		
	}

	void Direct3DClient::Draw() {
		// this function is for single thread rendering, in this case, I won't implement this function
	}

	void Direct3DClient::EndFrame() {
		// change the state of back buffer from render target to present
		D3D12_RESOURCE_BARRIER barr{};
		barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barr.Transition.pResource = _renderTargetBuffer[_currentBackBufferIndex].Get();
		barr.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barr.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		barr.Transition.Subresource = 0;
		barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		_frameEndResource._cmdList->ResourceBarrier(1, &barr);

		//close commandlist and execute it.
		if (CloseAllCommandList()) {
			ID3D12CommandList* lists[] = { 
				_frameBeginResource._cmdList.Get(),
				// TODO: cmdlist from other thread that records all the draw commands for each object
				_frameEndResource._cmdList.Get()
			};
			_cmdQueue->ExecuteCommandLists(_countof(lists), lists);
			FlushCommandQueue();
		}
	}

	void Direct3DClient::Present() {
		_swapChain->Present(0, 0);
	}

	void Direct3DClient::Shutdown() {
		for (int i = 0; i < BufferCount; i++) {
			FlushCommandQueue();
		}

		for (auto& buffer : _renderTargetBuffer) {
			buffer->Release();
		}
		_rtvHeap->Release();
		_swapChain->Release();

		_frameBeginResource.Release();
		_frameEndResource.Release();

		_fence->Release();

		_cmdQueue->Release();
		_device->Release();

		if (_window) {
			delete _window;
			_window = nullptr;
		}
	}

	void Direct3DClient::CloseClient() {
		_window->CloseWindow();
	}

#pragma warning(default : 26495)
#endif

}