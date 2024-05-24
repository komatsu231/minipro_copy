#include "SceneManager.h"
#include "DxLib.h"
#include "Title.h"
#include "Game.h"

#include <string.h>

unsigned int Cr, Cr2, Cr3, Cr4, Cr5, Cr6;
LPCSTR fontPath;
int nowTime, startTime;

/* タイトル画面の初期化 */
void TitleInitialize(){
    Cr  = GetColor(10, 249, 105);   //盤面の色
    Cr2 = GetColor(217, 217, 217);  //背景の色
    Cr3 = GetColor(0, 0, 0);        //黒色
    Cr4 = GetColor(255, 255, 255);  //白色
    Cr5 = GetColor(255, 8, 8);      //赤色
    Cr6 = GetColor(8, 137, 255);    //青色

    SetFontSize( 32 );      // 文字のサイズ
    // フォントのロード
    //LPCSTR fontPath = "Raleway-ExtraBold.ttf"; // 読み込むフォントファイルのパス
    fontPath = "Raleway-SemiBoldItalic.ttf"; // 読み込むフォントファイルのパス
    if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) > 0){
        // フォント読み込み成功
    }
    else
    {
        // フォント読込エラー処理
        MessageBox(NULL, "フォント読込失敗", "", MB_OK);
    }
    ChangeFont("Raleway SemiBold Italic", DX_CHARSET_DEFAULT);

    DrawBox(0, 0, 700, 600, Cr2, TRUE);
    DrawBox(100, 400, 300, 500, Cr5, TRUE);
    DrawBox(400, 400, 600, 500, Cr6, TRUE);

    DrawString(149, 431, "START", Cr4);
    DrawString(466, 431, "END", Cr4);
    SetFontSize(96);
    DrawString(180, 200, "Othello", Cr3);

    //開始時刻を取得
    startTime = GetNowCount();
}     

/* タイトル画面の終了処理 */
void TitleFinalize(){
    //描画をクリア
    ClearDrawScreen();

    SetFontSize(32);
}       

/* タイトル画面の更新処理 */
void TitleUpdate(){
    //マウスの入力を取得
    mouseInput = GetMouseInput();
    //マウスの左ボタンが押された場合
    if(mouseInput & MOUSE_INPUT_LEFT){
        GetMousePoint(&mouseX, &mouseY);
        //現在の時間を取得
        nowTime = GetNowCount();
        //開始から１秒経過している場合
        if(nowTime - startTime >= 1000){
            // スタートボタンを押下
            if((mouseX >= 100 && mouseX <= 300) && 
                (mouseY >= 400 && mouseY <= 500) &&
                isRelease == TRUE){

                ChangeScene(Scene_GAME);
            }

            // 終了ボタンを押下
            if(
                (mouseX >= 400 && mouseX <= 600) && 
                (mouseY >= 400 && mouseY <= 500) &&
                isRelease == TRUE){
                
                //終了関数を呼び出す
                TitleFinalize();

                // フォントのアンロード
                if (RemoveFontResourceEx(fontPath, FR_PRIVATE, NULL))
                {
                    // アンロード成功
                }
                else
                {
                    MessageBox(NULL, "Remove failure", "", MB_OK);
                }

                //アプリケーションの終了
                DxLib_End();
            }
        }
        //マウスの左ボタンが押されている
        isRelease = FALSE;
    }
    //マウスの左ボタンが離されたかを判定
    if ((oldMouseInput & MOUSE_INPUT_LEFT) != 0 && (mouseInput & MOUSE_INPUT_LEFT) == 0){
        isRelease = TRUE;
    }

    // 古いマウスの状態を更新
    oldMouseInput = mouseInput;
}         