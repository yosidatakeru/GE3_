//単体
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include<d3d12.h>
#include<wrl.h>

//using namespace Microsoft::WRL;
class Sprite
{
public:
	void Initialize(DirectXCommon* directXCommon, SpriteCommon* spriteCommon);

	void Draw();
private:
	DirectXCommon* directXCommon_ = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
};
