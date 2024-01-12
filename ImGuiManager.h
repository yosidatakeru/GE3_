#pragma once

#include "DirectXCommon.h"


class ImGuiManager
{
public:
	//������
	static void Initialize(HWND hwnd, DirectXCommon* directXcommon_);
	//�X�V�J�n
	static void NewFrame();
	//�X�V�I��
	static void CreateCommand();
	//�R�}���h��ς�
	static void CommandExcute(ID3D12GraphicsCommandList * commandList);

	static ImGuiManager* Create();
	void ShowDemo();
	~ImGuiManager();
private:

	//DirectXCommon* directXcommon = nullptr;
	
};

