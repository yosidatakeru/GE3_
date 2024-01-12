#pragma once
#include<d3d12.h>
#include <cassert>
#include<DirectXTex.h>


ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInByte)
{
	ID3D12Resource* resource = nullptr;
	////VertexResource�𐶐�
	//���_���\�[�X�p�̃q�[�v��ݒ�
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���_���\�[�X�̐ݒ�
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	//�o�b�t�@���\�[�X�B�e�N�X�`���̏ꍇ�͂܂��ʂ̐ݒ������
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInByte;
	//�o�b�t�@�̏ꍇ�͂�����1�ɂ��錈�܂�
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	//�o�b�t�@�̏ꍇ�͂���ɂ��錈�܂�
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//���ۂɒ��_���\�[�X�����

HRESULT	hr = device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));


	return  resource;
}
//�e�N�X�`�����\�[�X���쐬����֐�
//ID3D12Resource* CreateTextureResource(ID3D12Resource* device, DirectX::TexMetadata& metaData)
//{
//	//1.metadata�����Resource�̐ݒ�
//	D3D12_RESOURCE_DESC resourceDesc{};
//	//Texture�̕�
//	resourceDesc.Width = UINT(metaData.width);
//	//Texture�̍���
//	resourceDesc.Height = UINT(metaData.height);
//	//mipmap�̐�
//	resourceDesc.MipLevels = UINT16(metaData.mipLevels);
//	//���s�� or �z��Texture�̔z��
//	resourceDesc.DepthOrArraySize = UINT16(metaData.arraySize);
//	//Texture��Format
//	resourceDesc.Format = metaData.format;
//	//�T���v�����O�J�E���g
//	resourceDesc.SampleDesc.Count = 1;
//	//Texture�̎������B���i�g���Ă���̂�2����
//	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metaData.dimension);
//
//	//2.���p����Heap�̐ݒ�
//	//���p����Heap�̐ݒ�B���ɓ���ȉ^�p�B02_04ex�ň�ʓI�ȃP�[�X�ł�����
//	D3D12_HEAP_PROPERTIES heapProperties{};
//	//�ׂ����ݒ���s��
//	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
//	//WriteBack�|���V�[��CPU�A�N�Z�X�\
//	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	//�v���Z�b�T�̋߂��ɔz�u
//	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//
//
//	
//	//3.Resource�𐶐�����
//	ID3D12Resource* resource = nullptr;
//	HRESULT	hr = device->CreateCommittedResource(
//		&heapProperties,					//Heap�̐ݒ�
//		D3D12_HEAP_FLAG_NONE,				//Heap�̓���Ȑݒ�
//		&resourceDesc,						//Resource�̐ݒ�
//		D3D12_RESOURCE_STATE_GENERIC_READ,	//�����ResourceState�BTexture�͊�{�ǂނ���
//		nullptr,							//Clear�œK�l�B�g��Ȃ��̂�nullptr
//		IID_PPV_ARGS(&resource));			//�쐬����Resource�|�C���^�ւ̃|�C���^
//	assert(SUCCEEDED(hr));
//
//	return resource;
//
//};