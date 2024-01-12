#include "ImGuiManager.h"
#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
 

 void ImGuiManager::Initialize(HWND hwnd, DirectXCommon* directXcommon_)
 {
	//Imgui‚Ì‰Šú‰»
	 IMGUI_CHECKVERSION();
	 ImGui::CreateContext();
	 ImGui::StyleColorsDark();
	 ImGui_ImplWin32_Init(hwnd);
	 ImGui_ImplDX12_Init(
		 directXcommon_->GetDevice(),
		 directXcommon_->GeSwapChainDesc().BufferCount,
		 directXcommon_->GetRtvDesc().Format,
		 directXcommon_->GetSrvDescriptorHeap(),
		 directXcommon_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		 directXcommon_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart()
	 );
 }

 void ImGuiManager::NewFrame()
 {
	 ImGui_ImplDX12_NewFrame();
	 ImGui_ImplWin32_NewFrame();
	 ImGui::NewFrame();
 }

 void ImGuiManager::CreateCommand()
 {
	 ImGui::Render();
 }

 void ImGuiManager::CommandExcute(ID3D12GraphicsCommandList* commandList)
 {
	 ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

 }

 





 ImGuiManager* ImGuiManager::Create()
 {
	 ImGuiManager* instance = new ImGuiManager();
	 return instance;
 }

 ImGuiManager::~ImGuiManager()
 {
	 ImGui_ImplDX12_Shutdown();
	 ImGui_ImplWin32_Shutdown();
	 ImGui::DestroyContext();
 }

 void ImGuiManager::ShowDemo()
 {
	 //ImguiŠm”F—p
	 ImGui::ShowDemoWindow();
 }

