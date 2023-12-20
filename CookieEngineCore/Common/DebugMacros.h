#pragma once
#include <string>

static uint64_t global_id = 0;

#define PRINT_OUT 0

#ifdef _DEBUG

#if PRINT_OUT

#define OUTPUT_ADDRESS(_data, funcName) std::cout << "allocate memory at: " << _data << " from object: " << _name << " in function: " << funcName << std::endl
#define FREE(_data, funcName) std::cout << "from object: " << _name << ", free address: " << _data << ", free from function: " << funcName << std::endl; free(_data)
#define PRINT(_typeName)	std::cout << "object " << _typeName << " create: " << _name << std::endl

#else	// #if PRINT_OUT

#define OUTPUT_ADDRESS(_data, funcName)
#define FREE(_data, funcName) free(_data)
#define PRINT(_typeName)

#endif	// PRINT_OUT

#define OBJECT_CREATE(_typeName) _name = std::to_string(global_id++); PRINT(_typeName)
#define DEBUG_INFO()	\
private:				\
	std::string _name;

#else	// #ifdef _DEBUG

#define OUTPUT_ADDRESS(_data, funcName) 
#define FREE(_data, funcName) free(_data)
#define OBJECT_CREATE(_typeName)
#define DEBUG_INFO()

#endif	// _DEBUG

#ifndef ThrowIfFailed
#define ThrowIfFailed(x) 
#endif