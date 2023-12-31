#pragma once
#include "Utils/Utils.h"
#ifdef _WIN64
#include <Windows.h>
#endif

// 26 alphabets
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyADelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyBDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyCDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyDDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyEDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyFDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyGDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyHDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyIDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyJDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyKDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyLDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyMDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyNDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyODelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyPDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyQDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyRDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeySDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyTDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyUDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyVDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyWDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyXDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyYDelegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyZDelegate);

// 0-9 numbers
DECLARE_MULTICAST_FUNCTION_DELEGATE(Key0Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(Key1Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(Key2Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(Key3Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(Key4Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(Key5Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(Key6Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(Key7Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(Key8Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(Key9Delegate);

// F1-F12 fucntional keys
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyF1Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyF2Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyF3Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyF4Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyF5Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyF6Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyF7Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyF8Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyF9Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyF10Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyF11Delegate);
DECLARE_MULTICAST_FUNCTION_DELEGATE(KeyF12Delegate);


class Input {
public:
	//26 alphabets
	KeyADelegate KeyA;
	KeyBDelegate KeyB;
	KeyCDelegate KeyC;
	KeyDDelegate KeyD;
	KeyEDelegate KeyE;
	KeyFDelegate KeyF;
	KeyGDelegate KeyG;
	KeyHDelegate KeyH;
	KeyIDelegate KeyI;
	KeyJDelegate KeyJ;
	KeyKDelegate KeyK;
	KeyLDelegate KeyL;
	KeyMDelegate KeyM;
	KeyNDelegate KeyN;
	KeyODelegate KeyO;
	KeyPDelegate KeyP;
	KeyQDelegate KeyQ;
	KeyRDelegate KeyR;
	KeySDelegate KeyS;
	KeyTDelegate KeyT;
	KeyUDelegate KeyU;
	KeyVDelegate KeyV;
	KeyWDelegate KeyW;
	KeyXDelegate KeyX;
	KeyYDelegate KeyY;
	KeyZDelegate KeyZ;

	//0-9 numbers
	Key0Delegate Key0;
	Key1Delegate Key1;
	Key2Delegate Key2;
	Key3Delegate Key3;
	Key4Delegate Key4;
	Key5Delegate Key5;
	Key6Delegate Key6;
	Key7Delegate Key7;
	Key8Delegate Key8;
	Key9Delegate Key9;

	// F1-F12
	KeyF1Delegate F1;
	KeyF2Delegate F2;
	KeyF3Delegate F3;
	KeyF4Delegate F4;
	KeyF5Delegate F5;
	KeyF6Delegate F6;
	KeyF7Delegate F7;
	KeyF8Delegate F8;
	KeyF9Delegate F9;
	KeyF10Delegate F10;
	KeyF11Delegate F11;
	KeyF12Delegate F12;
};

#ifdef _WIN64

LRESULT CALLBACK ProcessInputs(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif