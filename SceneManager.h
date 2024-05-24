#ifndef DEF_SCENEMANAGER_H
#define DEF_SCENEMANAGER_H

/* 画面の状態 */
typedef enum {
    Scene_TITLE,    //タイトル画面
    Scene_GAME,     //ゲーム画面
    Scene_NONE,     //無し
} Scene;

/* シーンマネージャー 初期化 */
void SceneMgrInitialize(); 
/* シーンマネージャー 終了処理 */
void SceneMgrFinalize();   
/* シーンマネージャー 更新 */
void SceneMgrUpdate();     

/* 次シーンの変更 */
void ChangeScene(Scene next_scene);

#endif