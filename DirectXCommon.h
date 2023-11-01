#pragma once
#include<wrl.h>
#include<dxgi1_6.h>
#include<d3d12.h>
#include"WinApp.h"
#include <vector>
class DirectXCommon
{
public:
	void Initialize(WinApp*winApp);

	void Update();

	void PreDraw();

	void PosDeaw();

	ID3D12Device* GetDevice() const { return device.Get(); }

	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }


private:
	//�f�o�C�X�̏�����
	void DeviceInitialize();
	//�R�}���h�̏�����
	void InitializeCommand();

	//�X���b�v�`�F�[��
	void InitializeSwapchain();

	//�����_�[�^�[�Q�b�g
	void InitializeRenderTargetView();

	//�[�x�o�b�t�@
	void InitializeDepthBuffer();

	//�t�F���X
	void InitializeFence();

private:
	WinApp* winApp = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Device> device;

	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;

	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;

	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	D3D12_RESOURCE_BARRIER barrierDesc{};

};
	
	

