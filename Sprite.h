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
	
	//頂点情報
	struct  VertexData
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 texcoord;
	};

   //マテリアル
	struct MaterialData
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX uvTransform;
	};


public:
	void Initialize(SpriteCommon* spriteCommon);

	void Update();

	void Draw();
	
//ゲッタセッター
	DirectX::XMFLOAT2 GetPosition()          { return position; }
	void SetPosintion(DirectX::XMFLOAT2 pos) { position = pos; }

	//回転
	float GetRotation()                      { return rotation; }
	void SetRotaion(float rot)               { rotation = rot; }
	//色 
	DirectX::XMFLOAT4 GetColor()             { return color_; }
	void SetColor(DirectX::XMFLOAT4 color)   { color_ = color; }

	//サイズ
	DirectX::XMFLOAT2 GetSize()              { return size; }
	void SetSize(DirectX::XMFLOAT2 size) { this->size = size; }
private:
	//頂点データの作成関数
	void CreateVertex();

	//インデックス情報作成
	void CreateIndex();
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
	//Resourceにデータを書き込む
	VertexData* vertexData = nullptr;


	//インデックス
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	//マテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	MaterialData* materialData = nullptr;

	//行列
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;

	//色のパラメータ
	DirectX::XMFLOAT4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	DirectX::XMMATRIX* wvpData = nullptr;

	//UV座標
	Transform uvTransform = { {1,1,1}, {0,0,0}, {0,0,0} };

	Transform transform = { {1,1,1}, {0,0,0}, {0,0,0} };
	DirectX::XMFLOAT2 position = { 0,0 };
	float rotation = 0;

	//サイズ管理
	DirectX::XMFLOAT2 size = { 1,1 };
	//カメラ
	Transform camreaTransform = { {1,1,1}, {0,0,0}, {0,0,-5} };

	//画像の保蔵先のアドレス
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	

};
