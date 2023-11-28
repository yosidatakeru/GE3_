#include "Sprite.h"
#include<d3d12.h>
#include<wrl.h>


#include<DirectXMath.h>


using namespace DirectX;

void Sprite::Initialize(DirectXCommon* directXCommon,SpriteCommon* spriteCommon)
{
	HRESULT hr{};
	directXCommon_ = directXCommon;
	spriteCommon_ = spriteCommon;
	////VertexResourceを生成
	//頂点リソース用のヒープを設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(XMFLOAT4) * 3;
	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に頂点リソースを作る
	
	hr = directXCommon_->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));


	////VertexBufferViewを作成
	//頂点バッファビューを作成する
	
	
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(DirectX::XMFLOAT4) * 3;
	//１頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(DirectX::XMFLOAT4);


	


}

void Sprite::Draw()
{
	
	//Resourceにデータを書き込む
	XMFLOAT4* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0] = { -0.5f,-0.5f,0.0f,1.0f };
	//上
	vertexData[1] = { 0.0f,0.5f,0.0f,1.0f };
	//右下
	vertexData[2] = { 0.5f,-0.5f,0.0f,1.0f };
	


	////コマンドを積む
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXCommon_->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->GetRootSignature());
	directXCommon_->GetCommandList()->SetPipelineState(spriteCommon_->GetGraphicsPipelineState());
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//描画(DrawCall)３兆点で１つのインスタンス。
	directXCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

}

 