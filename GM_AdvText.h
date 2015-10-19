//GM_AdvText.h
//ADVゲームシステムプログラム
#pragma once
#include "Text.h"
#include <mbctype.h>
#define LiteDrawing	true
#define DateNum	5
#define adv_fontsize 25
#define adv_furiganafontsize 10 
#define def_adv_moji_blank_w 0
#define def_adv_moji_blank_h 5
#define furigana_maxs	10
#define furigana_stringmax	50
#define MAX_LEN		4
#define MAX_TEXT		255

struct _furigana
{
	int x;
	int y;
	int r;
	int g;
	int b;
	char furigana_string[furigana_maxs];
};


class CADV
{
public:
	//int w=横の文字数
	//int h=縦の行数
	int Set_textwindow(int size,int w,int h);//テキスト領域の宣言
	int Set_furiganafont(int size,char fontname[]);
	void Draw_furigana(int x,int y,int alpha);
	int Release_textwindow(void);
	int stack_text(char moji,int datas[DateNum]);//文字の収納
	int format_text(Ctext *text_dest);
	void Draw_text(int num,int x,int y,int w,int h,int alpha);
	//更新関数（ウィンサイズ似合わせて文字表示を調整する）
	void Reload_text(void);

	//フリガナモード
	void set_furigana_mode();
	void set2_furigana_mode();
	void out_furigana_mode();

	
private:
	_furigana *furigana;
	int furigana_temp_x;
	int furigana_temp_y;
	int furigana_max;
	int furigana_totalmax;
	int furigana_mode;
	int furigana_fontsize;
	int furigana_fontnum;
	int furigana_temp_stringlensize;
	int furigana_moji;
	int furigana_moji_CP;
	Ctext *text;
	int flag;
	char **textbody;
	int ****data;
	int char_w;
	int char_h;
	int char_size;
	int SP;
	int CP;
	int adv_moji_blank_w;
	int adv_moji_blank_h;

	
	

	bool litedrawf;
};


