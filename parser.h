#pragma once
#include "Renderer.h"
#include "Scene.h"
//以下スクリプト記号規定
#define START_TAG			'<'			//タグの開始点			
#define END_TAG				'>'			//タグの終了点
#define IMP_NAME_DELIMI		' '			//命令文終了
#define ARGU_DELIMI			' '			//個別引数終了
#define ARGU_DELIVERY		'='			//引数データーチェッカー
#define STRI_SIGN			'"'			//文字列開始・終了点

//以下マネージメント
#define TAG_DATA_MAX 10


struct _tagbody{
	int status;
	int data_max;
	char impname[15];//命令名　15文字以内
	char arguname[10][15];//引数名 15文字以内,10データーまで
	char argudata[10][255];//255文字まで 10データーまで
};
//↓
struct _Tag
{

	int tag_max;
	int tag_num;
	_tagbody	*tag_body;
};

class CTag
{
private:
	//_Tag tag;


	int Add_Tag(_Tag *tag);
	int ResetTag(_Tag *tag);
	int free_Tag(_Tag *tag);
	int trim_skip(char *script,int *i);
public:
	int Init_Tag(_Tag *tag);
	int push_parser(char script[],_Tag *tag);//タグを記録
	_tagbody pop_parser(_Tag *tag);//タグを記録
	int debugtest_pop_parser(_Tag *tag);//デバッグテスト用、全てのタグを出力。//printf出力
		
};


extern double BorlandAnser(char *str);