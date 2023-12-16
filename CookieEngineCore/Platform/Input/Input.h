#pragma once
#ifdef _WIN64
#include <Windows.h>
#endif


#ifdef _WIN64

LRESULT CALLBACK OnWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif