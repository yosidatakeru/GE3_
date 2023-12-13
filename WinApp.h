﻿#pragma once
#include <Windows.h>

class WinApp
{
public:
	void Initialize();

	bool Update();

	void Finalize();

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//Getter
	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetHInstanc() const { return w.hInstance; }

public:
	// ウィンドウサイズ
	static const int window_width = 1280;  // 横幅
	static const int window_height = 720;  // 縦幅


private:
	HWND hwnd;
	WNDCLASSEX w{};
	MSG msg{};  // メッセージ
};

