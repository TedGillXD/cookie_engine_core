#pragma once

#include <cstdint>
#include <cassert>
#include <cstring>
#include <memory>
#include <algorithm>
#include <string>

#ifndef DISABLE_COPY
#define DISABLE_COPY(Type)						\
		explicit Type(const Type&) = delete;	\
		Type& operator=(const Type&) = delete;
#endif

#ifndef DISABLE_MOVE
#define DISABLE_MOVE(Type)						\
		explicit Type(Type&&) = delete;			\
		Type& operator=(Type&&) = delete;
#endif

#ifndef DISABLE_MOVE_AND_COPY
#define DISABLE_MOVE_AND_COPY(Type)	DISABLE_COPY(Type) DISABLE_MOVE(Type)
#endif

// include vulkan header files


#ifdef _WIN64	// include d3d12 header files

#define NOMINMAX	// to disable the min max function that provided by microsoft

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>	// for ComPtr

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#ifdef _DEBUG		//to enable debug layer
#include <d3d12sdklayers.h>		//these headers will provide accessing to debugging infrastructure
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif	// _DEBUG

#endif