#include"DirectXCommon.h"
#include<string>

#include <DirectXTex.h>

class TextureManager
{
private:
	//�摜�`�悷�邽�߂ɕK�v�̃f�[�^
	struct TexTureData
	{
		std::wstring filePath;
		DirectX::TexMetadata metaData;
		Microsoft::WRL::ComPtr<ID3D12Resource>resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};


public:
	static TextureManager* GetInstance();
	void Finalize();

	void Initialize(DirectXCommon* directXCommon);
	//�摜�̓ǂݍ���
	void  LoadTexture(const std::wstring& filePath);

	//�w�肵���摜���z��̉��Ԗڂɂ��邩�m�F
	uint32_t GetTextureIndexFilePath(const std::wstring& filePath);

	//�w�肵�����v�f�ԍ��n���h�����󂯎��
	D3D12_GPU_DESCRIPTOR_HANDLE GetStvHandleGPU(uint32_t textureIndex);

	//���^�f�[�^�擾
	const DirectX::TexMetadata& GetMetaData(uint32_t textureIndex);
private:
	//�V�F�[�_�[�ɑ���
	void UploadTewtureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);
private:
	static TextureManager* instance;
	static uint32_t kSRVIndexTop;


	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;
private:
	std::vector<TexTureData> textureDatas;
	DirectXCommon* directXCommon_ = nullptr;
};
 