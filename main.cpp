



#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"









// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    
    Input* input = nullptr;
    WinApp* winApp = nullptr;
    DirectXCommon* directXCommon = nullptr;
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







    // ゲームループ
    while (true) {
        if (winApp->Update() == true)
        {
            break;
        }

        input->Update();
        
        // 数字の0キーが押されていたら
        if (input->TriggerKey(DIK_0))
        {
            OutputDebugStringA("Hit 0\n");  // 出力ウィンドウに「Hit 0」と表示
        }

      
        directXCommon->PreDraw();
       

        directXCommon->PosDeaw();

        

    }

    winApp->Finalize();
    delete input;
    delete winApp;
    delete directXCommon;


   
    return 0;
}
