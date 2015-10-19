#include <stdio.h>
#include <string>
#include <mbctype.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "Common.h"
int SP,CP;
int CTag::free_Tag(_Tag *tag)
{
	free(tag->tag_body);
	return 0;
}

int CTag::Init_Tag(_Tag *tag)
{
	DebugText("TagSystem Initialize...");
	tag->tag_num=0;
	tag->tag_max=TAG_DATA_MAX;
	tag->tag_body=(_tagbody*)malloc(sizeof(_tagbody) * tag->tag_max);
	DebugText("OK!\n");
	return 0;
}

int CTag::Add_Tag(_Tag *tag)
{
	if(tag->tag_num>=(tag->tag_max-1))
	{
		tag->tag_max+=TAG_DATA_MAX;
		tag->tag_body=(_tagbody*)malloc(sizeof(_tagbody) * tag->tag_max);

	}
	tag->tag_num++;

	return 0;
}

int CTag::ResetTag(_Tag *tag)
{
	if(tag->tag_max>TAG_DATA_MAX)//標準状態より多いと判断
	{
		tag->tag_max=0;
		free_Tag(tag);
		Add_Tag(tag);
		tag->tag_num=0;
	}
	tag->tag_num=0;
	return 0;
}

int CTag::trim_skip(char *script,int *i)//実質的トリムではなく、スキップしていくだけ。
{
			if(script[*i]==' ')//半角空白が入っている場合はサーチしてみる。
			{
				for(;script[*i]==' ';*i+=1)
				{
					if(script[*i]='\0')
					{
						return -1;
					}
				}

			}
			return 0;
}

//１行文のタグをプッシュ
int CTag::push_parser(char script[],_Tag *tag)
{
	DebugText("PushTag...");

	bool stri_f=false;
	int i=0;
	if(script[i]==START_TAG)//タグの開始点は存在するかしないのであればサーチ。もしも見つからないのであればエラー
	{
		for(;script[i]!=START_TAG;i++)
		{
			if(script[i]=='\0')
			{
				//エラー。タグがおかしい。
				return -1;
			}
		}
	}
	i++;
	char *p;//ポインタ
	int k=0;
	p=tag->tag_body[tag->tag_num].impname;//ネーム取得開始
	for(int k=0;k<=15;k++)
	{
		if(script[i]==IMP_NAME_DELIMI)break;//区切りを発見。
		if(script[i]!=' ')*p++=script[i];//空白文字がはいっていれば排除。
		i++;
	};
	if(k>=15||isalpha(tag->tag_body[tag->tag_num].impname[0])==0)
	{
		//エラー、命令文がおかしい。
				return -1;
	}
	*p++='\0';

	//ここまでで、命令文完了以下、引数登録。
	int countdata=0;

	DebugText("...");
	do {

		p=tag->tag_body[tag->tag_num].arguname[countdata];
		//引数名
		if(isalpha(script[i]))//引数名が英語から始まっているかチェック。
		{
			*p++=script[i];
			i++;
		}
		else
		{
			//エラー。先頭文字が半角英語ではない。数値もしくは文字列記号の可能性が高い
			trim_skip(script,&i);//念のためサーチ
			if(isalpha(script[i])==0)
			{
				
				return -1;
			}
		}
		for(k=0;k<=15;k++)
		{
			if(script[i]==ARGU_DELIVERY)break;//区切りを発見。
			if(script[i]!=' ')*p++=script[i];//空白文字がはいっていれば排除。
			i++;
		};
		*p++='\0';
		i++;
		if(k>=15||isalpha(tag->tag_body[tag->tag_num].impname[0])==0)
		{
		//エラー、中間チェックがおかしい。
					
					return -1;
		}

		p=tag->tag_body[tag->tag_num].argudata[countdata];
		//データー登録
		if(script[i]==STRI_SIGN)//文字列宣言
		{
			stri_f=true;
			*p++=script[i];//文字列先頭記号をそのまま記載
			i++;
		}
		for(int k=0;k<=15;k++)
		{
			if((stri_f==true)&&(script[i]==STRI_SIGN)){i++;break;}
			if(script[i]==ARGU_DELIMI)break;//区切りを発見。
			if(script[i]==END_TAG)break;//区切りを発見。
			if(script[i]!=' ')*p++=script[i];//空白文字がはいっていれば排除。
			i++;
		};
		*p++='\0';


		trim_skip(script,&i);//念のためトリム
		//引数データー次へ
		countdata++;

		stri_f=false;
		if(countdata>10)
		{
			DebugText("Error:引数が多すぎます。\n");
			//エラー。引数が多すぎます。
		}
	
		if(script[i]==END_TAG)break;
		
	} while ((script[i]!='\0')||(script[i]!=END_TAG));	//スクリプト終端まで繰り返す。

	
	countdata--;
	tag->tag_body[tag->tag_num].data_max=countdata;
	//登録完了
	Add_Tag(tag);
	DebugText("%s\n",tag->tag_body[tag->tag_num].impname);

	return 0;


}




_tagbody CTag::pop_parser(_Tag *tag)
{
	DebugText("PopTag...");

	tag->tag_num--;//ポップのため減らす

	_tagbody tags;
	
	tags=tag->tag_body[tag->tag_num];



	if(tag->tag_num<(tag->tag_max-TAG_DATA_MAX))
	{
		tag->tag_max-=TAG_DATA_MAX;
		tag->tag_body=(_tagbody*)malloc(sizeof(_tagbody) * tag->tag_max);//領域を縮小

	}
	DebugText("%s\n",tags.impname);

	return tags;
}






int CTag::debugtest_pop_parser(_Tag *tag)
{
	printf("スクリプト分解プログラムV3\n字句解析＋語句解析\n\n");
	_tagbody tags;

	for(;tag->tag_num>=0;)
	{
		//ポップ
		tags=pop_parser(tag);
		printf("-----------------------------------\n");	
		printf("命令文：%s\n",tags.impname);


		for(int i=0;i<=tags.data_max;i++)
		{
			printf("引数名%d：%s\n",i,tags.arguname[i]);
			printf("引数情報%d：%s\n",i,tags.argudata[i]);
		}

	}

	return 0;
}
//-----------------------------------------------
//DynamicScript処理用 
//-----------------------------------------------
//１行文のタグをプッシュ
/*int CTag::push_parser_dynamic(char script[],_Tag *tag)
{

	bool stri_f=false;
	int i=0;
	if(script[i]==START_TAG)//タグの開始点は存在するかしないのであればサーチ。もしも見つからないのであればエラー
	{
		for(;script[i]!=START_TAG;i++)
		{
			if(script[i]=='\0')
			{
				//エラー。タグがおかしい。
				return -1;
			}
		}
	}
	i++;
	char *p;//ポインタ
	int k=0;
	p=tag->tag_body[tag->tag_num].impname;//ネーム取得開始
	for(int k=0;k<=15;k++)
	{
		if(script[i]==IMP_NAME_DELIMI)break;//区切りを発見。
		if(script[i]!=' ')*p++=script[i];//空白文字がはいっていれば排除。
		i++;
	};
	if(k>=15||isalpha(tag->tag_body[tag->tag_num].impname[0])==0)
	{
		//エラー、命令文がおかしい。
				return -1;
	}
	*p++='\0';

	//ここまでで、命令文完了以下、引数登録。
	int countdata=0;


	do {

		p=tag->tag_body[tag->tag_num].arguname[countdata];
		//引数名
		if(isalpha(script[i]))//引数名が英語から始まっているかチェック。
		{
			*p++=script[i];
			i++;
		}
		else
		{
			//エラー。先頭文字が半角英語ではない。数値もしくは文字列記号の可能性が高い
			trim_skip(script,&i);//念のためサーチ
			if(isalpha(script[i])==0)
			{
				
				return -1;
			}
		}
		for(k=0;k<=15;k++)
		{
			if(script[i]==ARGU_DELIVERY)break;//区切りを発見。
			if(script[i]!=' ')*p++=script[i];//空白文字がはいっていれば排除。
			i++;
		};
		*p++='\0';
		i++;
		if(k>=15||isalpha(tag->tag_body[tag->tag_num].impname[0])==0)
		{
		//エラー、中間チェックがおかしい。
					
					return -1;
		}

		p=tag->tag_body[tag->tag_num].argudata[countdata];
		//データー登録
		if(script[i]==STRI_SIGN)//文字列宣言
		{
			stri_f=true;
			*p++=script[i];//文字列先頭記号をそのまま記載
			i++;
		}
		for(int k=0;k<=15;k++)
		{
			if((stri_f==true)&&(script[i]==STRI_SIGN)){i++;break;}
			if(script[i]==ARGU_DELIMI)break;//区切りを発見。
			if(script[i]==END_TAG)break;//区切りを発見。
			if(script[i]!=' ')*p++=script[i];//空白文字がはいっていれば排除。
			i++;
		};
		*p++='\0';


		trim_skip(script,&i);//念のためトリム
		//引数データー次へ
		countdata++;

		stri_f=false;
		if(countdata>10)
		{
			//エラー。引数が多すぎます。
		}
	
		if(script[i]==END_TAG)break;
		
	} while ((script[i]!='\0')||(script[i]!=END_TAG));	//スクリプト終端まで繰り返す。

	
	countdata--;
	tag->tag_body[tag->tag_num].data_max=countdata;
	//登録完了
	Add_Tag(tag);
	return 0;


}
*/