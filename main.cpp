
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
//#include"SpriteCommon.h"
#include"Sprite.h"

#include"ImGuiManager.h"






// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    
    Input* input = nullptr;
    WinApp* winApp = nullptr;
    DirectXCommon* directXCommon = nullptr;
    SpriteCommon* spriteCommon = nullptr;
    Sprite* sprite = nullptr;
#pragma region WindowsAPI初期化処理
    winApp = new WinApp();
    winApp->Initialize();

  
#pragma endregion

#pragma region DirectX初期化処理
    // DirectX初期化処理　ここから
    directXCommon = new DirectXCommon();
    directXCommon->Initialize(winApp);


  
#pragma endregion



    input = new Input();
    input->Initialize(winApp);


    ImGuiManager* imgui=  ImGuiManager::Create();
    ImGuiManager::Initialize(winApp->GetHwnd(),directXCommon);
    
    spriteCommon = new SpriteCommon();
    spriteCommon->Initialize(directXCommon);

    sprite = new Sprite();
    sprite->Initialize(spriteCommon);

    // ゲームループ
    while (true) {
        if (winApp->Update() == true)
        {
            break;
        }
        ImGuiManager::NewFrame();
        imgui->ShowDemo();
        input->Update();
       
        // 数字の0キーが押されていたら
        if (input->TriggerKey(DIK_0))
        {
            OutputDebugStringA("Hit 0\n");  // 出力ウィンドウに「Hit 0」と表示
        }

        ImGuiManager::CreateCommand();
        directXCommon->PreDraw();
        spriteCommon->SpritePreDraw();
        sprite->Draw();

        ImGuiManager::CommandExcute(directXCommon->GetCommandList());

        directXCommon->PosDeaw();

        

    }
    delete imgui;
    winApp->Finalize();
    delete input;
    delete winApp;
    delete directXCommon;
    delete sprite;
   

   
    return 0;
}
