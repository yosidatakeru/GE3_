#pragma once

#include "DirectXCommon.h"


class ImGuiManager
{
public:
	//初期化
	static void Initialize(HWND hwnd, DirectXCommon* directXcommon_);
	//更新開始
	static void NewFrame();
	//更新終了
	static void CreateCommand();
	//コマンドを積む
	static void CommandExcute(ID3D12GraphicsCommandList * commandList);

	static ImGuiManager* Create();
	void ShowDemo();
	~ImGuiManager();
private:

	//DirectXCommon* directXcommon = nullptr;
	
};

