#include "InterSystemDef.h"
#define		DEBUG_BGPIX		"image\\program_sys\\log_bg.jpg"
void CInterSystemDef::inter_systeminit(Ctexture		copysystemTexture,CFileReader		copysystemfr,CADV copyadv)
{
	//初期化
	systemTexture=copysystemTexture;
	systemfr=copysystemfr;

	systemadv=copyadv;

	systemadv.Set_textwindow(30,800,565);
	systemadv.format_text(&systemText);

	fontnum=systemText.SetTextFont(25,0,3,false,false,false,"メイリオ");	//ファイル読み

	//ファイル画像管理変数　int systempicturenum[MAX]
	//0=デバッグログ背景
	systempicturenum[0]=systemTexture.LoadImageA(DEBUG_BGPIX,&copysystemfr);

}

void CInterSystemDef::inter_systemdraw()
{

}