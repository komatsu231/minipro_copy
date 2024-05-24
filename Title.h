#ifndef TITLE_H
#define TITLE_H

/*盤面の色*/
extern unsigned int Cr;
/*背景の色*/
extern unsigned int Cr2;
/*黒色*/
extern unsigned int Cr3;
/*白色*/
extern unsigned int Cr4;
/*赤色*/
extern unsigned int Cr5;
/*青色*/
extern unsigned int Cr6;


/* タイトル画面の初期化 */
void TitleInitialize();    

/* タイトル画面の終了処理 */
void TitleFinalize();     

/* タイトル画面の更新処理 */
void TitleUpdate();         

#endif