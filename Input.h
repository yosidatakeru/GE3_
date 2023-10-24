#pragma once
#include<Windows.h>
#define DIRECTINPUT_VERSION     0x0800  // DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include<wrl.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include"WinApp.h"

//using namespace Microsoft::WRL;

class Input
{
public:
	
	
	
	void Initialize(WinApp* winApp);

	void Update();

	bool PushKey(BYTE keyNumber);

	bool TriggerKey(BYTE keyNumber);

private:
	WinApp* winApp_ = nullptr;

	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;

	BYTE key[256] = {};

	BYTE keyPre[256] = {};
};

