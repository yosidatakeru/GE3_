//単体
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include<d3d12.h>
#include<wrl.h>
#include<DirectXMath.h>


using namespace DirectX;
//using namespace Microsoft::WRL;
class Sprite
{
private:
	struct Transform
	{
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT3 rotate;
		DirectX::XMFLOAT3 translate;
	};

	struct  VertexData
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 texcoord;
	};



public:
	void Initialize(SpriteCommon* spriteCommon);

	void Draw();

private:
	//頂点データの作成関数
	void CreateVertex();

	//マテリアルの作成関数
	void CreateMAterial();

	//行列情報の作成
	//マテリアルの作成関数
	void CreateWVP();

private:
	DirectXCommon* directXCommon_ = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	//マテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;

	//行列
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;

	//色のパラメータ
	DirectX::XMFLOAT4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	DirectX::XMMATRIX* wvpData = nullptr;


	Transform transform = { {1,1,1}, {0,0,0}, {0,0,0} };

	//カメラ
	Transform camreaTransform = { {1,1,1}, {0,0,0}, {0,0,-5} };

	//画像の保蔵先のアドレス
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	

};
