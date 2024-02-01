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
	//読み込み済みか確認する処理
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
	//画像が最大値に超えていないかの確認
	assert(textureDatas.size() + kSRVIndexTop < DirectXCommon::kmixSRVcount);

	
	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};

	 DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	

	//ミップマップの作成
	//ミップマップ...元画像より小さなテクスチャ群
	DirectX::ScratchImage mipImages{};
	 DirectX::GenerateMipMaps(
		image.GetImages(), image.GetImageCount(), image.GetMetadata(),
		DirectX::TEX_FILTER_SRGB, 0, mipImages);
	

	//テクスチャデータの追加
	textureDatas.resize(textureDatas.size() + 1);
	//追加したテクスチャデータの参照を取得する
	TexTureData& textureData = textureDatas.back();
	textureData.filePath = filePath; 
	textureData.metaData = mipImages.GetMetadata();
	textureData.resource = CreateTextureResource(directXCommon_->GetDevice(),textureData.metaData);
	UploadTewtureData(textureData.resource.Get(), mipImages);



	//画僧の保存されるメモリ
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
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
    srvDesc.Texture2D.MipLevels = UINT(textureData.metaData.mipLevels);
	////SRVの生成
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
	//対象の画像が無かったら停止する
	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetStvHandleGPU(uint32_t textureIndex)
{
	//対象の要素番号が範囲外かどうかを調べる
	assert(textureIndex < DirectXCommon::kmixSRVcount);
	
	//要素番号のテクスチャデータのを受け取る
	TexTureData& textureData = textureDatas[textureIndex];

	return textureData.srvHandleGPU;
}





void TextureManager::UploadTewtureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{

	//Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();

	//全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel)
	{
		//MipMapLevelを指定して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		//Textureに転送
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,				//全領域
			img->pixels,			//元データアドレス
			UINT(img->rowPitch),	//1ラインサイズ
			UINT(img->slicePitch));	//1枚サイズ

		assert(SUCCEEDED(hr));
	}

}
