#include "DxLib.h"
#include "SceneManager.h"
#include "Title.h"
#include "Game.h"

static Scene scene = Scene_TITLE;       // 現在のシーン
static Scene nextScene = Scene_NONE;    // 次のシーン

static void SceneMgrInitializeModule(Scene scene_name);    // 指定モジュールの初期化 
static void SceneMgrFinalizeModule(Scene scene_name);      // 指定モジュールの終了処理

// 初期化
void SceneMgrInitialize(){
    SceneMgrInitializeModule(scene);
}

// 終了処理
void SceneMgrFinalize(){
    SceneMgrFinalizeModule(scene);
}

// 更新
void SceneMgrUpdate(){
    // 次のシーンがセットされている場合
    if(nextScene != Scene_NONE){
        SceneMgrFinalizeModule(scene);      // 現在のシーンの終了処理
        scene = nextScene;                  // 次のシーンを現在のシーンにセット
        nextScene = Scene_NONE;             // 次のシーンの情報をクリア
        SceneMgrInitializeModule(scene);    // 現在のシーンを初期化
    }

    // 現在のシーンによって処理を分岐
    switch (scene){
    case Scene_TITLE:
        TitleUpdate();
        break;
    case Scene_GAME:
        GameUpdate();
        break;
    default:
        break;
    }
}

// 引数 next_scene にシーンを変更する
void ChangeScene(Scene next_scene){
    nextScene = next_scene;         // 次のシーンをセット
}

// 引数sceneモジュールを初期化する
static void SceneMgrInitializeModule(Scene scene_name) {
    //シーンによって処理を分岐
    switch (scene){
    case Scene_TITLE:       // 指定画面がタイトル画面
        TitleInitialize();
        break;
    case Scene_GAME:        // 指定画面がゲーム画面
        GameInitialize();
        break;
    default:
        break;
    }
}

// 引数sceneモジュールの終了処理を行う
static void SceneMgrFinalizeModule(Scene scene_name) {
    //シーンによって処理を分岐
    switch (scene) {         
    case Scene_TITLE:       //指定画面がタイトル画面
        TitleFinalize();   
        break;
    case Scene_GAME:        //指定画面がゲーム画面
        GameFinalize();
        break;
    default:
        break;
    }
}