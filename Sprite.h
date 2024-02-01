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
	void Initialize(SpriteCommon* spriteCommon, std::wstring textureFilePath);

	void Update();

	void Draw();

	//ゲッタセッター
	DirectX::XMFLOAT2 GetPosition() { return position; }
	void SetPosintion(DirectX::XMFLOAT2 pos) { position = pos; }

	//回転
	float GetRotation() { return rotation; }
	void SetRotaion(float rot) { rotation = rot; }
	//色 
	DirectX::XMFLOAT4 GetColor() { return color_; }
	void SetColor(DirectX::XMFLOAT4 color) { color_ = color; }

	//サイズ
	DirectX::XMFLOAT2 GetSize() { return size; }
	void SetSize(DirectX::XMFLOAT2 size) { this->size = size; }

	//アンカーポイント
	DirectX::XMFLOAT2 GetAnchorpoint() { return anchorPoint; }
	void SetAnchorpoint(DirectX::XMFLOAT2 anchor) { anchorPoint = anchor; }

	//左右反転
	bool GetFlipX() { return isFlipX; }
	//上下反転
	bool GetFlipY() { return isFlipY; }

	//切り抜き関連
	DirectX::XMFLOAT2 GetTextureLeftTop() { return textureLeftTop; }
	DirectX::XMFLOAT2 GetTextureSize() { return textureSize; }


	

	//左右反転
	void SetFlipX(bool isFlag) { isFlag = isFlag; }

	//上下反転
	void SetFlipY(bool isFlag) { isFlag = isFlag; }


	void SetTexture(std::wstring textureFilePath);

	//切り抜き関連
	void SetTextureLeftTop(DirectX::XMFLOAT2 value) { textureLeftTop = value; }
	void SetTextureSize(DirectX::XMFLOAT2 size) { textureSize = size; }


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
	
	//本来テクスチャサイズを合わせる
	void AdujustTextueSize();

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
	DirectX::XMFLOAT2 size = { 512,512 };
	
	//アンカーポイント
	DirectX::XMFLOAT2 anchorPoint = { 0,0 };

	//左右反転
	bool isFlipX = false;
	//上下反転」
	bool isFlipY = false;

	//切り抜き
	//切り抜きたい画像内の座標
	DirectX::XMFLOAT2 textureLeftTop = { 0,0 };
	//切り抜きたい画像内のサイズ
	DirectX::XMFLOAT2 textureSize = { 100,100 };


	//画像の保存場所
	uint32_t textureIndex_ = 0;
	//カメラ
	Transform camreaTransform = { {1,1,1}, {0,0,0}, {0,0,-5} };

	

	
	

};
