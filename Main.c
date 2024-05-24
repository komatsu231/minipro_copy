// DXライブラリを使用したオセロゲームサンプル
// ライブラリ関数を使用している箇所にはコメントをつけていますが、詳細なリファレンスについては
// DXライブラリのフォルダの中にある『help』フォルダ内の『DxFunc.html』を参照して下さい。

#include "string.h"
#include "DxLib.h"
#include "SceneManager.h"
#include "Game.h"
#include "Title.h"

/* 描画サイズ等（画像サイズ等に合わせて変更してください） */
#define BOARD_SIZE_X 700
#define BOARD_SIZE_Y 600

/* DXライブラリ使用時のメイン関数 */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    //画面モードの変更（画面の解像度(X,Y). カラービット数）
    SetGraphMode( BOARD_SIZE_X, BOARD_SIZE_Y, 32 , 11);
    //ウィンドウモードで起動(フルスクリーンかウィンドウで一部関数の動作が変わるため注意)
    ChangeWindowMode( TRUE );
    //DXライブラリの初期化(DXライブラリを使用する際は必須になります。)
    DxLib_Init();

    // シーンマネージャーの初期化
    SceneMgrInitialize();

    /* メッセージイベント処理(ウィンドウが閉じられたりした際にループを抜ける) */
    while( ProcessMessage() == 0 )
    {
        // シーンマネージャーの更新
        SceneMgrUpdate();
    }

    // シーンマネージャーの終了処理
    SceneMgrFinalize();

    /* DXライブラリ使用の終了関数(DXライブラリを使用する際は必須になります。) */
    DxLib_End();

    return 0;
}