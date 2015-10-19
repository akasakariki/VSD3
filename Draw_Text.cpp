#include "main.h"
#include "Text.h"
#include "Renderer.h"
#include <stdlib.h>    // rand, srand使用

#include "Common.h"
//-----------------------------------------------------------
//フォント準備（作成＆アドレス引き渡し）
//-----------------------------------------------------------
int Ctext::SettingText(LPDIRECT3DDEVICE9 *pD3DDevice_dist)
{
	pD3DDevice=pD3DDevice_dist;
	//メモリ確保
	Totalmax=ADD_MAX_FONT;
	Max=-1;
	ManagementNum=(int*)malloc(sizeof(int) *Totalmax);
	lpFont=(LPD3DXFONT*)malloc(sizeof(LPD3DXFONT) *Totalmax);
	size=(int*)malloc(sizeof(int) *Totalmax);
	return 0;
}
//-------------------------------------------------------------
//フォントオブジェクト設定
//-------------------------------------------------------------
int Ctext::SetTextFont(int Size,int Angle,int Thickness,bool Oblique,bool Under,bool Cancellation,char FontNAME[] )
{
	DebugText("Set TextFont(fontname:%s)...",FontNAME);
	Max++;
	if(Max>=Totalmax)
	{
		//最大数を超えた
		Totalmax+=ADD_MAX_FONT;
		ManagementNum=(int*)realloc(ManagementNum,sizeof(int) *Totalmax);
		lpFont=(LPD3DXFONT*)realloc(lpFont,sizeof(LPD3DXFONT) *Totalmax);
		size=(int*)realloc(size,sizeof(int) *Totalmax);
	}
	HFONT hFont;
	int error;
	hFont=CreateFont(
		Size,              // 高さ
		0,               // 幅
		Angle,          // 文字送り方向の角度
		0,         // ベースラインの角度
		Thickness,             // 太さ
		Oblique,          // 斜体かどうか
		Under,       // 下線を引くかどうか
		Cancellation,       // 取り消し線を引くかどうか
		DEFAULT_CHARSET,         // 文字セット
		OUT_DEFAULT_PRECIS, // 出力精度
		CLIP_DEFAULT_PRECIS,   // クリッピング精度
		PROOF_QUALITY,         // 出力品質
		FIXED_PITCH | FF_MODERN,  // ピッチとファミリ
		FontNAME         // フォント名
	);
	size[Max]=Size;
   error=D3DXCreateFont(*pD3DDevice,hFont,&lpFont[Max]) ;
    
	if(error==D3DERR_INVALIDCALL)
	{
		//パラメーターミス
	}
	else if(error==E_OUTOFMEMORY)
	{
		//メモリの確保失敗
	}
	DeleteObject(hFont);
	ManagementNum[Max]=~Max;
	DebugText("OK!\n");
	return ManagementNum[Max];
}


//-------------------------------------------------------------
// フォントオブジェクト描写
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
//      LPD3DXFONT &lpFontフォントフェイスへのアドレス引用
//-------------------------------------------------------------
int Ctext::DrawText(int num,int x,int y,int alpha,int r,int g,int b,char String[])
{
	int count=~num;

	// 三角形の描画処理
	// 頂点バッファの設定
	RECT rect={x,y,CLIENT_WIDTH,CLIENT_HEIGHT};
	lpFont[count]->DrawText(String, strlen(String), &rect, DT_LEFT, D3DCOLOR_ARGB(alpha, r, g, b));
	return 0;


}

void Ctext::reset_font(void)
{
	Max=-1;
}
int Ctext::DeleteTextFont_At(int num)
{
	DebugText("Delete TextFont...");
		int count=~num;

		lpFont[count]->Release();
		for(int i=count;i<Max;i++)
		{
			lpFont[i]=lpFont[i+1];
		}
		Max--;

		if(Max<(Totalmax-ADD_MAX_FONT))
		{
			//最大数を超えた
			Totalmax-=ADD_MAX_FONT;
			ManagementNum=(int*)realloc(ManagementNum,sizeof(int) *Totalmax);
			lpFont=(LPD3DXFONT*)realloc(lpFont,sizeof(LPD3DXFONT) *Totalmax);
			size=(int*)realloc(size,sizeof(int) *Totalmax);
		}
		DebugText("OK!\n");
		return 0;

}

int Ctext::DeleteTextFont_All(void)
{
	DebugText("AllDelete TextFont...");
for(int count=0;count<Max;count++)
	{

		lpFont[count]->Release();
		
	}

	Totalmax=ADD_MAX_FONT;
	ManagementNum=(int*)realloc(ManagementNum,sizeof(int) *Totalmax);
	lpFont=(LPD3DXFONT*)realloc(lpFont,sizeof(LPD3DXFONT) *Totalmax);
	size=(int*)realloc(size,sizeof(int) *Totalmax);
			
	Max=-1;
	DebugText("OK!\n");
	return 0;
}
int Ctext::AllRelease()
{
	for(int count=0;count<Max;count++)
	{

		lpFont[count]->Release();
		
	}
	free(ManagementNum);
	free(lpFont);
	free(size);
	return 0;
}
int Ctext::return_size(int num)
{

			return size[~num];

}