#include <string>
#include <dxgidebug.h>
#include<dxcapi.h>
#include <wrl.h> 
#include"DirectXCommon.h"
#include <d3d12.h>

using namespace Microsoft::WRL;

class SpriteCommon
{
public:
	
	void Initialize(DirectXCommon* directXCommon);
	
	////CompileShader関数
	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

	ID3D12RootSignature* GetRootSignature() const { return rootSignature.Get(); }
	ID3D12PipelineState* GetGraphicsPipelineState() const { return  graphicsPipelineState.Get(); }
private:
	DirectXCommon* directXCommon_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;
	
};

