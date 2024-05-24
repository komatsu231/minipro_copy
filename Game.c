#include "Game.h"
#include "DxLib.h"
#include "SceneManager.h"
#include "Title.h"

#define SCREEN_WIDTH    700     // 画面の幅
#define SCREEN_HEIGHT   600     // 画面の高さ
#define BOARD_WIDTH     8       // オセロ盤の幅
#define BOARD_HEIGHT    8       // オセロ盤の高さ
#define BUTTON1_X1      500     //降参ボタンのx1
#define BUTTON1_X2      650     //降参ボタンのx2
#define BUTTON1_Y1      350     //降参ボタンのy1
#define BUTTON1_Y2      410     //降参ボタンのy2
#define BUTTON2_X1      500     //タイトルに戻るボタンのx1
#define BUTTON2_X2      650     //タイトルに戻るボタンのx2
#define BUTTON2_Y1      450     //タイトルに戻るボタンのy1
#define BUTTON2_Y2      510     //タイトルに戻るボタンのy2
#define TEXT_HEIGHT     510     // テキスト表示欄(高さ)

BOOL popupFlag;                 // ポップアップの状態
BOOL endFlag;                   // ゲーム中かどうかの判定
BOOL isRelease;                 // マウスが離されたかの判定

Turn turn;                      // 手番
Button button;                  // ボタンの状態
Result result;                  // 勝敗
int blackNum, whiteNum;         // 黒の石数、白の石数
int passJudge;                  // 連続でパスをした回数
int mouseX, mouseY;             // マウス入力座標
int indexX, indexY;             // x,y軸のインデックス
int myStone;                    // 自分の石
int oppStone;                   // 相手の石
unsigned int crStone;           // 表示する点の色
int mouseInput, oldMouseInput;  // 現在・前回のマウスの入力状態 

int stStone[BOARD_WIDTH][BOARD_HEIGHT] = {0};   // 盤面の石の状態

void GameInitialize(){

    ClearDrawScreen();      // 画面のリセット

    // 変数の初期化
    turn = Turn_BLACK;
    button = Button_NONE;
    popupFlag = FALSE;
    endFlag = FALSE;
    isRelease = TRUE;
    blackNum = 0;
    whiteNum = 0;
    passJudge = 0;

    // 盤面の初期化
    stStone[3][3] = 2;
    stStone[3][4] = 1;
    stStone[4][3] = 1;
    stStone[4][4] = 2;

    DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Cr2, TRUE);  // 背景の描画
    DrawBox(50, 50, 450, 450, Cr, TRUE);                    // 盤面を描画

    // 盤のマス目を描画
    for (int i = 0; i < 9; i++){
        DrawLine((50 + (50 * i)), 50, (50 + (50 * i)), 450, Cr3);
    }
    for (int i = 0; i < 9; i++){
        DrawLine(50, (50 + (50 * i)), 450, (50 + (50 * i)), Cr3);
    }
    DrawPixel(450, 450, Cr3);

    DrawStone();    // 石を描画

    // 石数表示欄に各色の石を描画
    DrawOvalAA(500, 200, 15, 15, 32, Cr3, TRUE);
    DrawOvalAA(500, 250, 15, 15, 32, Cr4, TRUE);

    // 降参ボタンとタイトルに戻るボタンを描画
    DrawBox(BUTTON1_X1, BUTTON1_Y1, BUTTON1_X2, BUTTON1_Y2, Cr5, TRUE);
    DrawBox(BUTTON2_X1, BUTTON2_Y1, BUTTON2_X2, BUTTON2_Y2, Cr6, TRUE);
    DrawString( 515 , 360  , "Give Up" , Cr3 );
    DrawString( 516 , 460  , "Go Title" , Cr3 );

    DrawString(130, TEXT_HEIGHT, "It's Black's turn.", Cr3);    // 手番の表示

    CountStone();       // 石数を表示
    SearchPlace();      // 置ける場所を探索
    DrawStone();        // 石を描画
}

/*ゲーム終了処理*/
void GameFinalize(){

    ClearDrawScreen();

    // 変数のリセット
    turn = Turn_BLACK;
    button = Button_NONE;
    popupFlag = FALSE;
    endFlag = FALSE;
    isRelease = TRUE;
    blackNum = 0;
    whiteNum = 0;
    passJudge = 0;

    // 盤面のリセット
    for (int i = 0; i < BOARD_WIDTH; i++){
        for (int j = 0; j < BOARD_HEIGHT; j++){
            stStone[i][j] = 0;
        }
    }
    stStone[3][3] = 2;
    stStone[3][4] = 1;
    stStone[4][3] = 1;
    stStone[4][4] = 2;
}

/*ゲーム更新*/
void GameUpdate(){
    //マウスの入力座標を取得
    mouseInput = GetMouseInput(); 
    if (popupFlag == FALSE && endFlag == FALSE){
        //パスした場合
        if(passJudge >= 1){
            if(SearchPlace() == 0){
                DrawString(490, 110, "GameOver", Cr3);
                // テキスト表示欄のリセット
                DrawBox(15, 485, 499, 599, Cr2, TRUE);
                // 勝敗判定
                if(blackNum > whiteNum){
                    result = Result_BLACK;
                    DrawString(100, TEXT_HEIGHT, "Black Player Win!", Cr3);
                } 
                else if(whiteNum > blackNum){
                    result = Result_WHITE;
                    DrawString(100, TEXT_HEIGHT, "White Player Win!", Cr3);   
                }
                else if(whiteNum == blackNum){
                    result = Result_DRAW;
                    DrawString(100, TEXT_HEIGHT, "Draw!", Cr3);
                }
                endFlag = TRUE;
            }
            //置ける場所がある場合
            else{
                passJudge = 0;
                DrawStone();
            }
        }
        
        // マウスの左ボタンが押された場合
        if (mouseInput & MOUSE_INPUT_LEFT){
            GetMousePoint(&mouseX, &mouseY);

            // マウスの座標を盤面のインデックスに変換
            indexX = (mouseX / 50) - 1;
            indexY = (mouseY / 50) - 1;

            // 盤面の範囲内であれば更新
            if ((indexX >= 0 && indexX <= 7 && indexY >= 0 && indexY <= 7) && isRelease == TRUE){
                // 関数呼び出し
                if(AddStone()){
                    ChangeStone();

                    // ターンの切り替え
                    if (turn == Turn_BLACK){
                        turn = Turn_WHITE;
                        DrawString(130, TEXT_HEIGHT, "It's White's turn.", Cr3);
                    }
                    else if (turn == Turn_WHITE){
                        turn = Turn_BLACK;
                        DrawString(130, TEXT_HEIGHT, "It's Black's turn.", Cr3);
                    }
                    
                    //置ける場所がない場合
                    if(SearchPlace() == 0){
                        passJudge++;
                        if (turn == Turn_BLACK){
                            turn = Turn_WHITE;
                            // テキスト表示欄のリセット
                            DrawBox(30, 485, 330, 600, Cr2, TRUE);
                            DrawString(130, TEXT_HEIGHT, "It's White's turn.", Cr3);
                        }
                        else if (turn == Turn_WHITE){
                            turn = Turn_BLACK;
                            // テキスト表示欄のリセット
                            DrawBox(30, 485, 330, 600, Cr2, TRUE);
                            DrawString(130, TEXT_HEIGHT, "It's Black's turn.", Cr3);
                        }
                    }
                                        
                }
                
                DrawStone();                
                CountStone();                
            }
            //降参ボタンを押した場合
            if ((mouseX >= 500 && mouseX <= 650 && mouseY >= 350 && mouseY <= 410) && isRelease == TRUE){
                // 確認ボタンを描画
                DrawBox(BUTTON1_X1, BUTTON1_Y1, BUTTON1_X2, BUTTON1_Y2, Cr5, TRUE);
                DrawBox(BUTTON2_X1, BUTTON2_Y1, BUTTON2_X2, BUTTON2_Y2, Cr6, TRUE);
                // 確認内容を描画
                DrawBox(50, 460, 490, 599, GetColor(217, 217, 217), TRUE);
                DrawString(60, TEXT_HEIGHT, "Do you want to Give Up?", Cr5);
                DrawString(550, 360, "OK", Cr3);
                DrawString(550, 460, "NO", Cr3);

                button = Button_GIVEUP;
                popupFlag = TRUE;
            }
            //タイトルに戻るボタンを押した場合
            else if ((mouseX >= 500 && mouseX <= 650 && mouseY >= 450 && mouseY <= 510) && isRelease == TRUE){
                // 確認ボタンを描画
                DrawBox(BUTTON1_X1, BUTTON1_Y1, BUTTON1_X2, BUTTON1_Y2, Cr5, TRUE);
                DrawBox(BUTTON2_X1, BUTTON2_Y1, BUTTON2_X2, BUTTON2_Y2, Cr6, TRUE);
                // 確認内容を描画
                DrawBox(50, 460, 450, 599, Cr2, TRUE);
                DrawBox(50, 510, 699, 599, Cr2, TRUE);
                DrawString(45, TEXT_HEIGHT, "Do you want to Return to Title?", Cr5);
                DrawString(550, 360, "OK", Cr3);
                DrawString(550, 460, "NO", Cr3);

                button = Button_TITLE;
                popupFlag = TRUE;
            }
            //押下されている状態
            isRelease = FALSE;
        }
        
    }
    else if ((popupFlag == TRUE) && (endFlag == FALSE)){
        // マウスの左ボタンが押された場合
        if (mouseInput & MOUSE_INPUT_LEFT){
            GetMousePoint(&mouseX, &mouseY);

            // OKを押す場合
            if ((mouseX >= 500 && mouseX <= 650 && mouseY >= 350 && mouseY <= 410) && isRelease == TRUE){
                //降参ボタンを押した場合
                if (button == Button_GIVEUP){
                    result = (Result)turn;
                    GiveUp();
                }
                //タイトルに戻るボタンを押した場合
                else if (button == Button_TITLE){
                    ReturnTitle();
                }
            }
            // NOを押す場合
            else if ((mouseX >= 500 && mouseX <= 650 && mouseY >= 450 && mouseY <= 510) && isRelease == TRUE){
                // テキスト表示欄のリセット
                DrawBox(15, 485, 699, 599, Cr2, TRUE);

                if (turn == Turn_BLACK){
                    DrawString(130, TEXT_HEIGHT, "It's Black's turn.", GetColor(0, 0, 0));
                }
                else
                {
                    DrawString(130, TEXT_HEIGHT, "It's White's turn.", GetColor(0, 0, 0));
                }

                //何も押していない状態にして確認メッセージを消す
                DrawBox(BUTTON1_X1, BUTTON1_Y1, BUTTON1_X2, BUTTON1_Y2, Cr5, TRUE);
                DrawBox(BUTTON2_X1, BUTTON2_Y1, BUTTON2_X2, BUTTON2_Y2, Cr6, TRUE);
                DrawString(515, 360, "Give Up", Cr3);
                DrawString(516, 460, "Go Title", Cr3);
                button = Button_NONE;
                popupFlag = FALSE;
            }
            //押下されている状態
            isRelease = FALSE;
        }
    }
    //ゲーム終了後
    else if(endFlag == TRUE){
        //確認メッセージが出ていない場合
        if(popupFlag == FALSE){
            //降参ボタンの文字を消す
            DrawBox(BUTTON1_X1, BUTTON1_Y1, BUTTON1_X2, BUTTON1_Y2, Cr5, TRUE);

            // タイトルに戻るボタンが押された場合
            if (mouseInput & MOUSE_INPUT_LEFT){
                GetMousePoint(&mouseX, &mouseY);  
                if ((mouseX >= 500 && mouseX <= 650 && mouseY >= 450 && mouseY <= 510) && isRelease == TRUE){
                    // 確認ボタンを描画
                    DrawBox(BUTTON1_X1, BUTTON1_Y1, BUTTON1_X2, BUTTON1_Y2, Cr5, TRUE);
                    DrawBox(BUTTON2_X1, BUTTON2_Y1, BUTTON2_X2, BUTTON2_Y2, Cr6, TRUE);
                    // 確認内容を描画
                    DrawBox(50, 510, 699, 599, Cr2, TRUE);
                    DrawString(45, TEXT_HEIGHT, "Do you want to Return to Title?", Cr5);
                    DrawString(550, 360, "OK", Cr3);
                    DrawString(550, 460, "NO", Cr3);

                    button = Button_TITLE;
                    popupFlag = TRUE;
                    
                }
                isRelease = FALSE;
            }
        }
        //確認メッセージが出ている場合
        else{
            // マウスの左ボタンが押された場合
            if (mouseInput & MOUSE_INPUT_LEFT){
                GetMousePoint(&mouseX, &mouseY);
                //OKが押された場合
                if ((mouseX >= 500 && mouseX <= 650 && mouseY >= 350 && mouseY <= 410) && isRelease == TRUE){
                    ReturnTitle();
                }
                //NOが押された場合
                else if ((mouseX >= 500 && mouseX <= 650 && mouseY >= 450 && mouseY <= 510) && isRelease == TRUE){
                    // 押す前の状態に戻る
                    DrawBox(15, 485, 699, 599, Cr2, TRUE);
                    DrawBox(50, 460, 450, 599, Cr2, TRUE);
                    DrawBox(50, 510, 699, 599, Cr2, TRUE);

                    DrawBox(BUTTON1_X1, BUTTON1_Y1, BUTTON1_X2, BUTTON1_Y2, Cr5, TRUE);
                    DrawBox(BUTTON2_X1, BUTTON2_Y1, BUTTON2_X2, BUTTON2_Y2, Cr6, TRUE);
                    DrawString(516, 460, "Go Title", Cr3);
                    if (result == Result_BLACK){
                        DrawString(100, TEXT_HEIGHT, "Black Player Win!", Cr3);
                    }
                    else if (result == Result_WHITE){
                        DrawString(100, TEXT_HEIGHT, "White Player Win!", Cr3);
                    }
                    else
                    {
                        DrawString(300, TEXT_HEIGHT, "Draw!", Cr3);
                    }
                    
                    button = Button_NONE;
                    popupFlag = FALSE;
                    
                }
                isRelease = FALSE;
            }
        }
    }
    //マウスの左ボタンが離されたかを判定
    if ((oldMouseInput & MOUSE_INPUT_LEFT) != 0 && (mouseInput & MOUSE_INPUT_LEFT) == 0){
        isRelease = TRUE;
    }

    // 古いマウスの状態を更新
    oldMouseInput = mouseInput;
}


/*降参ボタン押下*/
void GiveUp(){
    DrawBox(50, 460, 450, 599, Cr2, TRUE);
    DrawBox(50, 510, 699, 599, Cr2, TRUE);

    DrawBox(BUTTON1_X1, BUTTON1_Y1, BUTTON1_X2, BUTTON1_Y2, Cr5, TRUE);
    DrawBox(BUTTON2_X1, BUTTON2_Y1, BUTTON2_X2, BUTTON2_Y2, Cr6, TRUE);

    //DrawString(515, 365, "Give Up", Cr);
    DrawString(516, 460, "Go Title", Cr3);
    if (turn == Turn_BLACK){
        result = Result_WHITE;
        DrawString(100, TEXT_HEIGHT, "White Player Win!", Cr3);
    }
    else
    {
        result = Result_BLACK;
        DrawString(100, TEXT_HEIGHT, "Black Player Win!", Cr3);        
    }
    popupFlag = FALSE;
    endFlag = TRUE;
}

/*タイトルに戻るボタン押下*/
void ReturnTitle(){
    GameFinalize();
    ChangeScene(Scene_TITLE);
}

/*置ける場所探索用*/
int SearchPlace(){    
    int count = 0;      // 置ける場所の数を保持

    // 全てのマスを探索
    for(int i = 0; i < BOARD_WIDTH; i++){
        for(int j = 0; j < BOARD_HEIGHT; j++){
            // 何も置いていないマスの場合
            if(stStone[i][j] == 0){
                // 探索開始
                // 左上
                if(SearchDirection(-1, -1, i, j)){
                    stStone[i][j] = 3;
                    count++;
                    continue;
                }
                //上
                if(SearchDirection(-1, 0, i, j)){
                    stStone[i][j] = 3;
                    count++;
                    continue;
                }
                // 右上
                if(SearchDirection(-1, 1, i, j)){
                    stStone[i][j] = 3;
                    count++;
                    continue;
                }
                // 右
                if(SearchDirection(0, 1, i, j)){
                    stStone[i][j] = 3;
                    count++;
                    continue;
                }
                // 右下
                if(SearchDirection(1, 1, i, j)){
                    stStone[i][j] = 3;
                    count++;
                    continue;
                }
                // 下
                if(SearchDirection(1, 0, i, j)){
                    stStone[i][j] = 3;
                    count++;
                    continue;
                }
                // 左下
                if(SearchDirection(1, -1, i, j)){
                    stStone[i][j] = 3;
                    count++;
                    continue;
                }
                // 左
                if(SearchDirection(0, -1, i, j)){
                    stStone[i][j] = 3;
                    count++;
                    continue;
                }
            }
        }
    }
    return count;
}

/* 1方向探索用 */
BOOL SearchDirection(int inc_i, int inc_j, int i, int j){
    myStone = turn + 1;             // 自分の石
    oppStone = turn ? 1 : 2;        // 相手の石
    int i_num, j_num;               // 探索用

    // 配置可能場所を表す点の色を変更
    if(turn == Turn_BLACK){
        crStone = Cr3;
    }
    else{
        crStone = Cr4;
    }

    // 探索先が盤面の範囲内である場合
    if (i + inc_i >= 0 && i + inc_i < BOARD_WIDTH && j + inc_j >= 0 && j + inc_j < BOARD_HEIGHT){
        i_num = i + inc_i;
        j_num = j + inc_j;

        // 探索先のマスが相手の石の場合
        if (stStone[i_num][j_num] == oppStone){
            // 盤面の端や何も置いていないマスではない場合、探索を続ける
            while (stStone[i_num][j_num] == oppStone && 
                (i_num >= 0 && i_num < BOARD_WIDTH && j_num >= 0 && j_num < BOARD_HEIGHT)){
                i_num += inc_i;
                j_num += inc_j;
            }
            // 自分の石を発見した場合(置けるマス)
            if (stStone[i_num][j_num] == myStone && 
                (i_num >= 0 && i_num < BOARD_WIDTH && j_num >= 0 && j_num < BOARD_HEIGHT)){
                return TRUE;
            }
        }
    }
    return FALSE;
}

/*石数を数える*/
void CountStone(){
    // 石数の初期化
    blackNum = 0;
    whiteNum = 0;

    // 配列から数を数える
    for(int i = 0; i < BOARD_WIDTH; i++){
        for(int j = 0; j < BOARD_HEIGHT; j++){
            if(stStone[i][j] == 1){
                blackNum += 1;
                printf("%d", blackNum);
            }
            else if(stStone[i][j] == 2){
                whiteNum += 1;
            }
        }
    }

    DrawBox(520, 155, 730, 300, Cr2, TRUE);         // 石数表示欄のリセット

    // 石数の表示
    DrawFormatString(530, 177, Cr3, "%d", blackNum);
    DrawFormatString(530, 228, Cr3, "%d", whiteNum);
}

/*マウスクリックで石を置く*/
BOOL AddStone(){
    myStone = turn + 1;         // 自分の石

    // クリックした場所に石を置けない場合
    if(stStone[indexY][indexX] != 3){
        return FALSE;
    }
    else{
        DrawBox(30, 485, 330, 600, Cr2, TRUE);      // テキスト表示欄のリセット
        stStone[indexY][indexX] = myStone;          // クリックしたマスに自分の石をセット
        
        // 配置可能位置の要素をリセット
        for(int i = 0; i < BOARD_WIDTH; i++){
            for(int j = 0; j < BOARD_HEIGHT; j++){
                if(stStone[i][j] == 3){
                    stStone[i][j] = 0;
                }
            }
        }
        return TRUE;
    }    
}   

/*挟まれた石の色を変更する*/
BOOL ChangeStone(){
    myStone = turn + 1;         // 自分の石
    oppStone = turn ? 1 : 2;    // 相手の石
    int i_num, j_num;           // 探索用       

    // 各方向を探索、可能であれば間の石を自分の石に変更
    // 左上
    if (SearchDirection(-1, -1, indexY, indexX)){
        j_num = indexX - 1;
        i_num = indexY - 1;
        while(stStone[i_num][j_num] == oppStone){
            stStone[i_num][j_num] = myStone;
            j_num--;
            i_num--;
        }
    }
    // 上
    if (SearchDirection(-1, 0, indexY, indexX)){
        j_num = indexX;
        i_num = indexY - 1;
        while(stStone[i_num][j_num] == oppStone){
            stStone[i_num][j_num] = myStone;
            i_num--;
        }
    }
    // 右上
    if (SearchDirection(-1, 1, indexY, indexX)){
        j_num = indexX + 1;
        i_num = indexY - 1;
        while(stStone[i_num][j_num] == oppStone){
            stStone[i_num][j_num] = myStone;
            j_num++;
            i_num--;
        }
    }
    // 右
    if (SearchDirection(0, 1, indexY, indexX)){
        j_num = indexX + 1;
        i_num = indexY;
        while(stStone[i_num][j_num] == oppStone){
            stStone[i_num][j_num] = myStone;
            j_num++;
        }
    }
    // 右下
    if (SearchDirection(1, 1, indexY, indexX)){
        j_num = indexX + 1;
        i_num = indexY + 1;
        while(stStone[i_num][j_num] == oppStone){
            stStone[i_num][j_num] = myStone;
            j_num++;
            i_num++;
        }
    }
    // 下
    if (SearchDirection(1, 0, indexY, indexX)){
        j_num = indexX;
        i_num = indexY + 1;
        while(stStone[i_num][j_num] == oppStone){
            stStone[i_num][j_num] = myStone;
            i_num++;
        }
    }
    // 左下
    if (SearchDirection(1, -1, indexY, indexX)){
        j_num = indexX - 1;
        i_num = indexY + 1;
        while(stStone[i_num][j_num] == oppStone){
            stStone[i_num][j_num] = myStone;
            j_num--;
            i_num++;
        }
    }
    // 左
    if (SearchDirection(0, -1, indexY, indexX)){
        j_num = indexX - 1;
        i_num = indexY;
        while(stStone[i_num][j_num] == oppStone){
            stStone[i_num][j_num] = myStone;
            j_num--;
        }
    }
}

/* 2次元配列に応じて石を描画 */
void DrawStone(){
    // 配置可能場所を表す点の色を変更
    if(turn == Turn_BLACK){
        crStone = Cr3;
    }
    else if(turn = Turn_WHITE){
        crStone = Cr4;
    }

    // 全マスについて、値に応じた石を描画する
    for(int i = 0; i < BOARD_WIDTH; i++){
        for(int j = 0; j < BOARD_HEIGHT; j++){
            if(stStone[i][j] == 0){
                DrawBox(50 * (j + 1) + 1, 50 * (i + 1) + 1, 50 * (j + 2) - 1, 50 * (i + 2) - 1, Cr, TRUE); 
            }else if(stStone[i][j] == 1){
                DrawBox(50 * (j + 1) + 1, 50 * (i + 1) + 1, 50 * (j + 2) - 1, 50 * (i + 2) - 1, Cr, TRUE); 
                DrawOvalAA(50 * (j + 1) + 25, 50 * (i + 1) + 25, 15, 15, 32, Cr3, TRUE);
            }else if(stStone[i][j] == 2){
                DrawBox(50 * (j + 1) + 1, 50 * (i + 1) + 1, 50 * (j + 2) - 1, 50 * (i + 2) - 1, Cr, TRUE); 
                DrawOvalAA(50 * (j + 1) + 25, 50 * (i + 1) + 25, 15, 15, 32, Cr4, TRUE);
            }else if(stStone[i][j] == 3){
                DrawBox(50 * (j + 1) + 1, 50 * (i + 1) + 1, 50 * (j + 2) - 1, 50 * (i + 2) - 1, Cr, TRUE); 
                DrawOvalAA(50 * (j + 1) + 25, 50 * (i + 1) + 25, 3, 3, 32, crStone, TRUE);
            }
            DrawOvalAA(500, 200, 15, 15, 32, Cr3, TRUE);
        }
    }
}
