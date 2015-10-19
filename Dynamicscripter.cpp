//--------------------------------------------------------
//動的スクリプト処理(スクリプト名判断、処理)
//処理優先順位　システム関係→その他。
//--------------------------------------------------------
#include "DynamicFunction.h"
#include "DynamicFunctionList.h"

#include <string.h>
#include <stdlib.h>

int CDynamicFuction::InitMakeFuction(Ctexture *ptexture,Csound *psound,CADV *padv,CVariable *ptmpvariable,CVariable *psysvariable,CFileReader	*pfr)
{
	DebugText("Dynamic Script System Initialize...");

	texture=ptexture;
	sound=psound;
	adv=padv;
	tmpvariable=ptmpvariable;
	sysvariable=psysvariable;
	fr=pfr;
	//一時変数作成
	//tmpvariable->Add_Variable();
	DebugText("OK!\n");
	
	return 0;
}
int CDynamicFuction::DynamicScripter_Process()
{

	return 0;

}
//以下、スクリプト処理関係ルール
//0は正常に命令文発見し処理も完了した
//-1エラーは命令スクリプトが見つかっていない。
//-2エラーは命令文を発見はしたが、処理に問題が発生した。

//イメージレイヤー関係--------------------------------------------------------------------------------------
int CDynamicFuction::DynamicScripter_Image(_tagDynamic tagdynamic)	
{
	//レイヤシステム関係(ls)
	//描写処理開始地点（S→描写処理<dw>→Eとすることで、1フレームごとの一斉表示）
	if(!strcmp(	tagdynamic.impname,			LAYER_SYS_S	)) 
	{
		texture->Drawing_start();
	}
	//描写処理終了地点
	else if(!strcmp(tagdynamic.impname,		LAYER_SYS_E)) 
	{
		texture->Drawing_end();
	}
	//色属性モードを変更（Bright）
	else if(!strcmp(tagdynamic.impname,		LAYER_SYS_G)) 
	{
	

		int G_a,G_r,G_g,G_b;
		G_a=G_r=G_g=G_b=255;
		G_a=atoi(tagdynamic.argudata[0]);//アルファ値
		//値チェック
		if(G_a>255&&G_a<0)
		{
			return -2;//エラー 値が不透明
		}
		if(tagdynamic.argudata[1][0]!='\0')
		{
			int G_r=atoi(tagdynamic.argudata[1]);//Ｒ値
			int G_g=atoi(tagdynamic.argudata[2]);//Ｇ値
			int G_b=atoi(tagdynamic.argudata[3]);//Ｂ値
			//値チェック
			if((G_r>255&&G_r<0)&&(G_g>255&&G_g<0)&&(G_b>255&&G_b<0))
			{
				return -2;//エラー 値が不透明
			}

		}

		texture->SetBright(G_a,G_r,G_g,G_b);
	}
	//ブレンドモードを指定・変更
	else if(!strcmp(tagdynamic.impname,		LAYER_SYS_BM)) 
	{
		//アルファモード	tagdynamic.argudata[0]=0　tagdynamic.argudata[2]=アルファ値
		int mode,num;
		mode=atoi(tagdynamic.argudata[0]);//モード
		num=atoi(tagdynamic.argudata[1]);//値
		if(num>255&&num<0)
		{
			return -2;//エラー 値が不透明
		}
		if(mode==0)
		{
			texture->SetBlendMode(MODE_ALPHA,num);
		}
		else
		{
			return -2;//エラー存在しないブレンドモード
		}
	}
	//ブレンドモードを解除
	else if(!strcmp(tagdynamic.impname,		LAYER_SYS_BMC)) 
	{
		texture->NoBlend();
	}
	//カラーキーを設定（読み込み前に指定）
	else if(!strcmp(tagdynamic.impname,		LAYER_SYS_CKEY)) 
	{
		int G_r,G_g,G_b;
		G_r=G_g=G_b=255;
		G_r=atoi(tagdynamic.argudata[0]);//Ｒ値
		G_g=atoi(tagdynamic.argudata[1]);//Ｇ値
		G_b=atoi(tagdynamic.argudata[2]);//Ｂ値
		//値チェック
		if((G_r>255&&G_r<0)&&(G_g>255&&G_g<0)&&(G_b>255&&G_b<0))
		{
			return -2;//エラー 値が不透明
		}

		texture->SetLoadImageColorKey(G_r, G_g,G_b);
	}
	//ロード関係(ll)
	//画像をレイヤーに通常読み込み
	else if(!strcmp(tagdynamic.impname,		LAYER_LOAD_A)) 
	{
		//ファイル名
		//tagdynamic.argudata[0]
		//変数に代入。
		//tagdynamic.argudata[1]
	
		int temp_varialnum=texture->LoadImage(tagdynamic.argudata[0],fr);
		
		//指定変数の管理番号を取得
		temp_variable=tmpvariable->SearchVariable(tagdynamic.argudata[1]);
		if(temp_variable.type==1)
		{
			//変数タイプint
			tmpvariable->ChangeVariable_int(&temp_varialnum,temp_variable.num);
		}
		else
		{
			//エラー 指定された変数は不透明（intではない
			return -1;
		}

		
	}
	//画像をマスク処理
	else if(!strcmp(tagdynamic.impname,		LAYER_LOAD_M)) 
	{
			
		int temp_varialnum=texture->LoadImage_Mask(tagdynamic.argudata[0],fr);

		
		//指定変数の管理番号を取得
		temp_variable=tmpvariable->SearchVariable(tagdynamic.argudata[1]);
		if(temp_variable.type==1)
		{
			//変数タイプint
			tmpvariable->ChangeVariable_int(&temp_varialnum,temp_variable.num);
		}
		else
		{
			//エラー 指定された変数は不透明（intではない
			return -1;
		}
	}
	//削除関係(ld)
	//指定レイヤー削除
	else if(!strcmp(tagdynamic.impname,		LAYER_DELETE_AT)) 
	{
		//0:削除レイヤｰ番号。
		texture->DeleteImage_At(atoi(tagdynamic.argudata[0]));
	}
	//レイヤ全削除
	else if(!strcmp(tagdynamic.impname,		LAYER_DELETE_ALL)) 
	{
		texture->DeleteImage_All();
	}
	//描写関係(lw)
	//画像、通常描写
	else if(!strcmp(tagdynamic.impname,		LAYER_DRAW_A)) 
	{
		texture->DrawImage(atoi(tagdynamic.argudata[0]),atof(tagdynamic.argudata[1]),atof(tagdynamic.argudata[2]));
	}
	//拡大描写
	else if(!strcmp(tagdynamic.impname,		LAYER_DRAWR_S)) 
	{
		texture->DrawImage_Stretch(atoi(tagdynamic.argudata[0]),
										atof(tagdynamic.argudata[1]),
										atof(tagdynamic.argudata[2]),
										atof(tagdynamic.argudata[3]),
										atof(tagdynamic.argudata[4]));
	}
	//回転描写
	else if(!strcmp(tagdynamic.impname,		LAYER_DRAW_R)) 
	{
		texture->DrawImage_Rotation(atoi(tagdynamic.argudata[0]),
										atof(tagdynamic.argudata[1]),
										atof(tagdynamic.argudata[2]),
										atof(tagdynamic.argudata[3]),
										atof(tagdynamic.argudata[4])
									,	atof(tagdynamic.argudata[5]));
	}
	//拡大回転描写
	else if(!strcmp(tagdynamic.impname,		LAYER_DRAW_SR)) 
	{
		texture->DrawImage_StretchRotation(atoi(tagdynamic.argudata[0]),
										atof(tagdynamic.argudata[1]),
										atof(tagdynamic.argudata[2]),
										atof(tagdynamic.argudata[3]),
										atof(tagdynamic.argudata[4]),
										atof(tagdynamic.argudata[5])
										,atof(tagdynamic.argudata[6])
										,atof(tagdynamic.argudata[7]));
	}
//ブレンドモード
	else if(!strcmp(tagdynamic.impname,		LAYER_DRAW_B)) 
	{
		//エラー実装されていません。（何を入れようとしてたのここ・・・？
	}
	else
	{
		return -1;
	}
	return 0;
}




//サウンド関係(s)--------------------------------------------------------------------------------------
int CDynamicFuction::DynamicScripter_Sound(_tagDynamic tagdynamic)	
{
	//サウンド読み込み
	if(!strcmp(	tagdynamic.impname,			SOUND_LOAD)) 
	{
		
	}
	//サウンド再生
	else if(!strcmp(tagdynamic.impname,		SOUND_PLAY)) 
	{
	}
	//個別サウンド一時停止
	else if(!strcmp(tagdynamic.impname,		SOUND_STOP_AT)) 
	{
	}
	//全サウンド一時停止
	else if(!strcmp(tagdynamic.impname,		SOUND_STOP_ALL)) 
	{
	}
	//個別サウンド削除
	else if(!strcmp(tagdynamic.impname,		SOUND_DELETE_AT	)) 
	{
	}
	//全サウンド削除
	else if(!strcmp(tagdynamic.impname,		SOUND_DELETE_ALL)) 
	{
	}
	//個別サウンドボリューム変更
	else if(!strcmp(tagdynamic.impname,		SOUND_VOLUME_AT	)) 
	{
	}
	//個別サウンドボリュームを取得
	else if(!strcmp(tagdynamic.impname,		SOUND_GETVOLUME_AT)) 
	{
	}
	//個別サウンドの再生状態を取得
	else if(!strcmp(tagdynamic.impname,		SOUND_GETPLAYCHECK)) 
	{
	}
	else
	{
		return -1;
	}
	return 0;
}





//テキスト関係(t)--------------------------------------------------------------------------------------
int CDynamicFuction::DynamicScripter_Text(_tagDynamic tagdynamic)	
{
	//フォント設定
	if(!strcmp(	tagdynamic.impname,			TEXT_SETFONT)) 
	{
		
	}
	//テキスト描写
	else if(!strcmp(tagdynamic.impname,		TEXT_DRAWTEXT)) 
	{
	}
	//フォント削除	
	else if(!strcmp(tagdynamic.impname,		TEXT_DELETEFONT_AT)) 
	{
	}
	//フォント全削除
	else if(!strcmp(tagdynamic.impname,		TEXT_DELETEFONT_ALL)) 
	{
	}
	else
	{
		return -1;
	}
	return 0;
}

//変数関係(v)--------------------------------------------------------------------------------------
int CDynamicFuction::DynamicScripter_Variable(_tagDynamic tagdynamic)	
{
	//変数作成
	if(!strcmp(	tagdynamic.impname,			VARIABLE_MAKE)) 
	{
		
	}
	//変数を含めた計算
	else if(!strcmp(tagdynamic.impname,		VARIABLE_CALILATION	)) 
	{
	}
	else
	{
		return -1;
	}
	return 0;
}



//システム関係(s)--------------------------------------------------------------------------------------
int CDynamicFuction::DynamicScripter_System(_tagDynamic tagdynamic)	
{

	if(!strcmp(	tagdynamic.impname,			VARIABLE_MAKE)) 
	{
		
	}

	else if(!strcmp(tagdynamic.impname,		VARIABLE_CALILATION	)) 
	{
	}
	else
	{
		return -1;
	}
	return 0;
}
