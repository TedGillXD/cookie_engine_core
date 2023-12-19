#include "Input.h"

#ifdef _WIN64

LRESULT CALLBACK ProcessInputs(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
		
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

#endif