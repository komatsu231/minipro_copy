#pragma once
#ifndef DEF_GAME_H
#define DEF_GAME_H

#include "windows.h"

/*手番*/
typedef enum{
    Turn_BLACK = 0,
    Turn_WHITE = 1,
}Turn;

/*ボタンの状態*/
typedef enum{
    Button_GIVEUP = 0,
    Button_TITLE  = 1,
    Button_NONE   = 2
}Button;

/*勝敗*/
typedef enum{
    Result_BLACK = 0,
    Result_WHITE  = 1,
    Result_DRAW   = 2
}Result;

/*インデックス*/
extern int mouseX, mouseY;
/*マウス入力状態*/
extern int mouseInput;
/*マウスの前の入力状態*/
extern int oldMouseInput;
/*マウスを離したか*/
extern BOOL isRelease;

/*ゲーム初期化*/
void GameInitialize();
/*ゲーム終了処理*/
void GameFinalize();
/*ゲーム更新*/
void GameUpdate();
/*降参ボタン押下*/
void GiveUp();
/*タイトルに戻るボタン押下*/
void ReturnTitle();
/*置ける場所探索用*/
int SearchPlace();
/*石数を数える*/
void CountStone();
/*マウスクリックで石を置く*/
BOOL AddStone();
/*挟まれた石の色を変更する*/
BOOL ChangeStone();
//1方向探索用
BOOL SearchDirection(int a, int b, int i, int j);

/* 2次元配列に応じて石を描画 */
void DrawStone();

#endif


