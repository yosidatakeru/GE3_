#include "Sprite.h"
#include<d3d12.h>
#include<wrl.h>
#include"Bufftr.h"
#include"externals/imgui/imgui.h"


void Sprite::Initialize(SpriteCommon* spriteCommon)
{
	HRESULT hr{};
	directXCommon_ = spriteCommon->GetDirectXCommon();
	spriteCommon_ = spriteCommon;
	
	//画像読み込み
	DirectX::ScratchImage mipImages = spriteCommon->LoadTexture(L"Resources/uvChecker.png");
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	ID3D12Resource* textureResource = CreateTextureResource(directXCommon_->GetDevice(),metaData);
	spriteCommon_->UploadTewtureData(textureResource,mipImages);




	//SRV
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metaData.mipLevels);

	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU =
		directXCommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU =
		directXCommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();


	textureSrvHandleCPU.ptr += directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += directXCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);



	//SRVの生成
	directXCommon_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);




	CreateVertex();
	//インデクス
	CreateIndex();
	
	CreateMAterial();

	CreateWVP();
}

void Sprite::Update()
{
	ImGui::Begin("texture");
	ImGui::DragFloat3("UV_Pos", &uvTransform.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UV_Pos", &uvTransform.translate.z);
	ImGui::DragFloat3("UV_Pos", &uvTransform.translate.x, 0.01f, -10.0f, 10.0f);

	ImGui::End();
}

void Sprite::Draw()
{
	
	//回転処理(アップデート書くといいかも)
	//transform.rotate.y += 0.03;
	//ワールド
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotate));
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.translate));

	//回転行列とスケール行列
	XMMATRIX rotationAndScaleMatrix = XMMatrixMultiply(rotateMatrix, scaleMatrix);

	//最終的行列返還
	XMMATRIX worldMatrix = XMMatrixMultiply(rotationAndScaleMatrix, translationMatrix);
	*wvpData = worldMatrix;


	//カメラ
	XMMATRIX cameraScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&camreaTransform.scale));
	XMMATRIX cameraRotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&camreaTransform.rotate));
	XMMATRIX cameraTranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&camreaTransform.translate));
	//回転とスケールの掛け算
	//回転行列とスケール行列
	XMMATRIX cameraRotationAndScaleMatrix = XMMatrixMultiply(cameraRotateMatrix, cameraScaleMatrix);

	//最終的行列返還
	XMMATRIX cameraMatrix = XMMatrixMultiply(cameraRotationAndScaleMatrix, cameraTranslationMatrix);
	
	XMMATRIX view = XMMatrixInverse(nullptr, cameraMatrix);
	XMMATRIX proj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f), (float)WinApp::window_width / (float)WinApp::window_height,0.1f,100.0f);
	
	XMMATRIX worldViewProjecionMatrix = worldMatrix * (view * proj);
	*wvpData = worldViewProjecionMatrix;




	//UV座標
	XMMATRIX uvScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&uvTransform.scale));
	XMMATRIX uvRotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&uvTransform.rotate));
	XMMATRIX uvTranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&uvTransform.translate));

	//回転行列とスケール行列
	XMMATRIX uvRotationAndScaleMatrix = XMMatrixMultiply(uvRotateMatrix, uvScaleMatrix);

	//最終的行列返還
	XMMATRIX uvWorldMatrix = XMMatrixMultiply(uvRotationAndScaleMatrix, uvTranslationMatrix);
	materialData->uvTransform = uvWorldMatrix;





	////コマンドを積む
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	
	//頂点情報
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	
	//インデクス情報
	directXCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	//色情報
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress())  ;

	//行列
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());

	//画像
	directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2,textureSrvHandleGPU);

	
	directXCommon_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	//directXCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);

}



void Sprite::CreateVertex()
{
	////VertexBufferViewを作成
	//頂点バッファビューを作成する
	vertexResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * 4);

	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	//１頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	VertexData* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	
	//左下
	vertexData[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[0].texcoord = {0.0f,1.0f };
	//上
	vertexData[1].position = { -0.5f,0.5f,0.0f,1.0f };
	vertexData[1].texcoord = {0.0f,0.0f };
	//右下
	vertexData[2].position = { 0.5f,-0.5f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };

	//上
	vertexData[3].position = { 0.5f,0.5f,0.0f,1.0f };
	vertexData[3].texcoord = { 1.0f,0.0f };
	
	
}


void Sprite::CreateIndex()
{
	indexResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(uint32_t) * 6);

	//リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	//１頂点あたりのサイズ
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	//Resourceにデータを書き込む
	uint32_t* indexData = nullptr;
	//書き込むためのアドレスを取得
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	//三角形一枚作成
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;

	//三角形１枚作製
	indexData[3] = 1;
	indexData[4] = 3;
	indexData[5] = 2;
}



void Sprite::CreateMAterial()
{
	//マテリアルにデータを書き込む
	materialResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(MaterialData));
	
	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	//今回は赤を書き込む(ここで色を変えられる)
	materialData->color = color_;

	materialData->uvTransform = XMMatrixIdentity();
	

}

void Sprite::CreateWVP()
{
	
	//XMMATRIX=M4x4と一緒
	wvpResource = CreateBufferResource(directXCommon_->GetDevice(), sizeof(XMMATRIX));
	
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	
	*wvpData = XMMatrixIdentity();

}

 