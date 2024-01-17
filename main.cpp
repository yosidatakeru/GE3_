
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
//#include"SpriteCommon.h"
#include"Sprite.h"

#include"ImGuiManager.h"
#include<Vector>





// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    
    Input* input = nullptr;
    WinApp* winApp = nullptr;
    DirectXCommon* directXCommon = nullptr;
    SpriteCommon* spriteCommon = nullptr;
   // Sprite* sprite = nullptr;
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

    //複数化
    std::vector<Sprite*> sprite;
    for (int i = 0; i < 5; i++)
    {
        Sprite* temp = new Sprite();
        temp->Initialize(spriteCommon);
        temp->SetPosintion({ (float)i * 1,0 });
        sprite.push_back(temp);
    }

    
    // ゲームループ
    while (true) {
        if (winApp->Update() == true)
        {
            break;
        }
        ImGuiManager::NewFrame();
        imgui->ShowDemo();

        input->Update();

        
        //DirectX::XMFLOAT2 pos = sprite->GetPosition();
        //    //移動処理
        //pos.x += 0.005;
        //sprite->SetPosintion(pos);
        //
        ////回転の処理
        //float rot = sprite->GetRotation();
        //rot += 0.005;
        //sprite->SetRotaion(rot);
        //
        ////色の処理
        //DirectX::XMFLOAT4 color = sprite->GetColor();
        //color.x -= 0.1f;
        //    if (color.x < 0)
        //    {
        //        color.x = 1.0f;
        //    }
        //    sprite->SetColor(color);

        //    //サイズ
        //    DirectX::XMFLOAT2 size = sprite->GetSize();
        //    size.y += 0.01;
        //    sprite->SetSize(size);


        for (int i = 0; i <5 ; i++)
        {
            sprite[i]->Update();
        }
       
       
        // 数字の0キーが押されていたら
        if (input->TriggerKey(DIK_0))
        {
            OutputDebugStringA("Hit 0\n");  // 出力ウィンドウに「Hit 0」と表示
        }

        ImGuiManager::CreateCommand();
        directXCommon->PreDraw();
        spriteCommon->SpritePreDraw();
        for (int i = 0; i < 5; i++)
        {
            sprite[i]->Draw();
        }
        ImGuiManager::CommandExcute(directXCommon->GetCommandList());

        directXCommon->PosDeaw();

        

    }
    delete imgui;
    winApp->Finalize();
    delete input;
    delete winApp;
    delete directXCommon;
    for (int i = 0; i < 5; i++)
    {
        delete sprite[i];
    }

   
    return 0;
}
