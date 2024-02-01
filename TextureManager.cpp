#include "TextureManager.h"
#include "Bufftr.h"
TextureManager* TextureManager::instance = nullptr;
uint32_t TextureManager::kSRVIndexTop = 1;


TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new TextureManager();
	}
	return instance;
}

void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void TextureManager::Initialize(DirectXCommon* directXCommon)
{
	directXCommon_ = directXCommon;
	textureDatas.reserve(DirectXCommon::kmixSRVcount);
}

void TextureManager::LoadTexture(const std::wstring& filePath)
{
	//�ǂݍ��ݍς݂��m�F���鏈��
	auto it = std::find_if
	(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TexTureData& textureData) {return textureData.filePath == filePath; }
	);
	if (it != textureDatas.end())
	{
		return;
	}
	//�摜���ő�l�ɒ����Ă��Ȃ����̊m�F
	assert(textureDatas.size() + kSRVIndexTop < DirectXCommon::kmixSRVcount);

	
	//�e�N�X�`���t�@�C����ǂ�Ńv���O�����ň�����悤�ɂ���
	DirectX::ScratchImage image{};

	 DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	

	//�~�b�v�}�b�v�̍쐬
	//�~�b�v�}�b�v...���摜��菬���ȃe�N�X�`���Q
	DirectX::ScratchImage mipImages{};
	 DirectX::GenerateMipMaps(
		image.GetImages(), image.GetImageCount(), image.GetMetadata(),
		DirectX::TEX_FILTER_SRGB, 0, mipImages);
	

	//�e�N�X�`���f�[�^�̒ǉ�
	textureDatas.resize(textureDatas.size() + 1);
	//�ǉ������e�N�X�`���f�[�^�̎Q�Ƃ��擾����
	TexTureData& textureData = textureDatas.back();
	textureData.filePath = filePath; 
	textureData.metaData = mipImages.GetMetadata();
	textureData.resource = CreateTextureResource(directXCommon_->GetDevice(),textureData.metaData);
	UploadTewtureData(textureData.resource.Get(), mipImages);



	//��m�̕ۑ�����郁����
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = directXCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = directXCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	handleCPU.ptr += directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * srvIndex ;
	handleGPU.ptr += directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * srvIndex;

	textureData.srvHandleCPU = handleCPU;
	textureData.srvHandleGPU = handleGPU;




	////SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureData.metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
    srvDesc.Texture2D.MipLevels = UINT(textureData.metaData.mipLevels);
	////SRV�̐���
    directXCommon_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);

}



uint32_t TextureManager::GetTextureIndexFilePath(const std::wstring& filePath)
{
	auto it = std::find_if
	(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TexTureData& textureData) {return textureData.filePath == filePath; }
	);
	if (it != textureDatas.end())
	{
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
		return textureIndex;
	}
	//�Ώۂ̉摜�������������~����
	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetStvHandleGPU(uint32_t textureIndex)
{
	//�Ώۂ̗v�f�ԍ����͈͊O���ǂ����𒲂ׂ�
	assert(textureIndex < DirectXCommon::kmixSRVcount);
	
	//�v�f�ԍ��̃e�N�X�`���f�[�^�̂��󂯎��
	TexTureData& textureData = textureDatas[textureIndex];

	return textureData.srvHandleGPU;
}





void TextureManager::UploadTewtureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{

	//Meta�����擾
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();

	//�SMipMap�ɂ���
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel)
	{
		//MipMapLevel���w�肵�ĊeImage���擾
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		//Texture�ɓ]��
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,				//�S�̈�
			img->pixels,			//���f�[�^�A�h���X
			UINT(img->rowPitch),	//1���C���T�C�Y
			UINT(img->slicePitch));	//1���T�C�Y

		assert(SUCCEEDED(hr));
	}

}
