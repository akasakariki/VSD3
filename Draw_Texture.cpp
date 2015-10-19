#include "main.h"
#include "Scene.h"
#include "Renderer.h"
#include "Texture.h"
#include "Common.h"
#include <stdlib.h>    // rand, srand使用
#define	EXT_PNG	"PNG"
#define	EXT_PNG_S	"png"

void Ctexture::SettingTexture(LPDIRECT3DDEVICE9 m_pD3DDevice_dist,LPDIRECT3D9 m_pD3D_dist)
{
	DebugText("Texture Initialize...");

	m_pD3DDevice=m_pD3DDevice_dist;//ステージ情報の引き渡し
	m_pD3D=m_pD3D_dist;

	g_pSprite    = NULL;
	D3DXCreateSprite( m_pD3DDevice , &g_pSprite );
	Max=-1;
	Color=D3DCOLOR_XRGB(255,255,255);

	totalmax=ADD_MAX_TEXTURE;
	nflag=(Snflag*)malloc(sizeof(Snflag) * totalmax);
	g_pTexture=(LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9) * totalmax);
	Texture_Info=(D3DXIMAGE_INFO*)malloc(sizeof(D3DXIMAGE_INFO) * totalmax);
	ManagementNum=(int*)malloc(sizeof(int) * totalmax);

	m_pD3DDevice->Clear(0, 0, 
          D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
          D3DCOLOR_XRGB(0,0,0),
          1.0f,
          0          // ステンシルバッファを0にクリア
          );


	test_flag=0;
	DebugText("OK!\n");

}


int Ctexture::LoadImage_Mask(char FileName[],CFileReader *fr)
{
	DebugText("Load Mask Texture(Path:%s)...",FileName);

	if(!fr->Open(FileName)){
	//エラーファイルがみつかりません
      return -1;
	}
	Max++;
	if(Max>=totalmax)
	{
		//最大数を超えた
		totalmax+=ADD_MAX_TEXTURE;
		nflag=(Snflag*)realloc(nflag,sizeof(Snflag) * totalmax);
		g_pTexture=(LPDIRECT3DTEXTURE9*)realloc(g_pTexture,sizeof(LPDIRECT3DTEXTURE9) * totalmax);
		Texture_Info=(D3DXIMAGE_INFO*)realloc(Texture_Info,sizeof(D3DXIMAGE_INFO) * totalmax);
		ManagementNum=(int*)realloc(ManagementNum,sizeof(int) * totalmax);

	}

	//LPCVOID pSrcData
	char *ptr;
	DWORD size;
	fr->GetPtr((void**)&ptr, NULL, &size );
	//D3DXCreateTextureFromFile(m_pD3DDevice , FileName , &g_pTexture[Max] );
		if(D3DXCreateTextureFromFileInMemoryEx( 
			m_pD3DDevice, 
			ptr, 
			size, 
			D3DX_DEFAULT ,
			D3DX_DEFAULT ,
			0,
			0,
			D3DFMT_X8R8G8B8, //A→X
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,//ギザギザをぼかして多少綺麗に表示
			D3DX_DEFAULT,
			0,
			&Texture_Info[Max],
			NULL,
			&g_pTexture[Max]
			)!=D3D_OK)
		{
			//エラー
			return -1;
		}
	DebugText("...");
			
	D3DLOCKED_RECT  D_Rect;//画像サイズ
    int             xp,yp;

	//SetRect(&Rect,0,0,CLIENT_WIDTH,CLIENT_HEIGHT);


	int CheckNum=2;
	for(;CheckNum<(int)Texture_Info[Max].Width;)
	{
		//横幅サイズのチェック
		CheckNum*=2;//2乗チェックしていく
	}
	if(CheckNum!=Texture_Info[Max].Width)
	{
		//2乗ではない
		nflag[Max].cxv=(float)Texture_Info[Max].Width/CheckNum;
		nflag[Max].cx=(float)CheckNum;

	}
	else
	{

		nflag[Max].cxv=0;

	}

	CheckNum=2;
	for(;CheckNum<(int)Texture_Info[Max].Height;)
	{
		//縦幅サイズのチェック
		CheckNum*=2;//2乗チェックしていく
	}
	if(CheckNum!=Texture_Info[Max].Height)
	{
		//2乗ではない
		nflag[Max].cyv=(float)Texture_Info[Max].Height/CheckNum;
		nflag[Max].cy=(float)CheckNum;
	}
	else
	{
		nflag[Max].cyv=0;
	}
	nflag[Max].cy_mask=CheckNum;

		if (g_pTexture[Max]->LockRect(0,&D_Rect,NULL,0)!=D3D_OK)
	{
		DebugText("Error:LockRect\n");

		MessageBox(NULL,"LockRect Error","Surface Error",MB_OK);
		
		 return -1;
	}

	for(yp=0; yp<nflag[Max].cy_mask; yp++)//縦分
	{
		for(xp=0; xp<D_Rect.Pitch; xp+=4)//横分
		{
			//グレースケール変換無し、そのまま計算（軽量化のため）
			//α



			*((BYTE*)D_Rect.pBits+yp*D_Rect.Pitch+xp+3)=
				1-((*((BYTE*)D_Rect.pBits+yp*D_Rect.Pitch+xp)+
				*((BYTE*)D_Rect.pBits+yp*D_Rect.Pitch+xp+1)+
				*((BYTE*)D_Rect.pBits+yp*D_Rect.Pitch+xp+2))
				/3);
				


		}
	}
	g_pTexture[Max]->UnlockRect(0);
	int temp=0;
	_layerid idtemp;
	temp=IssueLayerID(Max,&idtemp);
	ManagementNum[Max]=idtemp.checker;
	DebugText("OK!\n");

	return temp;
}

int Ctexture::LoadImage(char FileName[],CFileReader *fr)
{
	DebugText("Load Texture...");

	if(!fr->Open(FileName)){
	//エラーファイルがみつかりません
      return -1;
	}
	Max++;
	if(Max>=totalmax)
	{
		//最大数を超えた
		totalmax+=ADD_MAX_TEXTURE;
		nflag=(Snflag*)realloc(nflag,sizeof(Snflag) * totalmax);
		g_pTexture=(LPDIRECT3DTEXTURE9*)realloc(g_pTexture,sizeof(LPDIRECT3DTEXTURE9) * totalmax);
		Texture_Info=(D3DXIMAGE_INFO*)realloc(Texture_Info,sizeof(D3DXIMAGE_INFO) * totalmax);
		ManagementNum=(int*)realloc(ManagementNum,sizeof(int) * totalmax);

	}
	//LPCVOID pSrcData
	char *ptr;
	DWORD size;
	char fileextname[5];//拡張子名
	int ext_status=0;

	strcpy( fileextname, fr->GetExtname(FileName));//拡張子名を代入。
	if(strcmp(fileextname,EXT_PNG)==0)ext_status=1;//検査１(PNGファイル)
	if(strcmp(fileextname,EXT_PNG_S)==0)ext_status=1;//検査１(PNGファイル)

	fr->GetPtr((void**)&ptr, NULL, &size );
	
	if(ext_status==1)//PNG読み込み
	{

		if(D3DXCreateTextureFromFileInMemoryEx( 
					m_pD3DDevice, 
					ptr, 
					size, 
					D3DX_DEFAULT ,
					D3DX_DEFAULT ,
					0,
					0,
					D3DFMT_UNKNOWN, //デフォルトにしアルファ値をそのまま処理
					D3DPOOL_MANAGED,
					D3DX_DEFAULT,//ギザギザをぼかして多少綺麗に表示
					D3DX_DEFAULT,
					0,
					&Texture_Info[Max],
					NULL,
					&g_pTexture[Max]
					)!=D3D_OK)
				{
					//エラー
					return -1;
				}

	}
	else

	{


		//D3DXCreateTextureFromFile(m_pD3DDevice , FileName , &g_pTexture[Max] );
			if(D3DXCreateTextureFromFileInMemoryEx( 
				m_pD3DDevice, 
				ptr, 
				size, 
				D3DX_DEFAULT ,
				D3DX_DEFAULT ,
				0,
				0,
				D3DFMT_X8R8G8B8, //A→X
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,//ギザギザをぼかして多少綺麗に表示
				D3DX_DEFAULT,
				0,
				&Texture_Info[Max],
				NULL,
				&g_pTexture[Max]
				)!=D3D_OK)
			{
				//エラー
				return -1;
			}
	}


	DebugText("...");


//	D3DXCreateTextureFromFile(m_pD3DDevice , FileName , &g_pTexture[Max] );
	int CheckNum=2;
	for(;CheckNum<(int)Texture_Info[Max].Width;)
	{
		//横幅サイズのチェック
		CheckNum*=2;//2乗チェックしていく
	}
	if(CheckNum!=Texture_Info[Max].Width)
	{
		//2乗ではない
		nflag[Max].cxv=(float)Texture_Info[Max].Width/CheckNum;
		nflag[Max].cx=(float)CheckNum;
	}
	else
	{
		nflag[Max].cxv=0;

	}
	CheckNum=2;
	for(;CheckNum<(int)Texture_Info[Max].Height;)
	{
		//縦幅サイズのチェック
		CheckNum*=2;//2乗チェックしていく
	}
	if(CheckNum!=Texture_Info[Max].Height)
	{
		//2乗ではない
		nflag[Max].cyv=(float)Texture_Info[Max].Height/CheckNum;
		nflag[Max].cy=(float)CheckNum;
	}
	else
	{
		nflag[Max].cyv=0;
	}
	nflag[Max].cy_mask=CheckNum;
	int temp=0;
	_layerid idtemp;
	temp=IssueLayerID(Max,&idtemp);
	ManagementNum[Max]=idtemp.checker;
	DebugText("OK!\n");

	return temp;
}
//一般描写
int Ctexture::DrawImage(int num,float x,float y)
{
	D3DXVECTOR2 position,scale;
	position.x=x;
	position.y=y;
	int count=0;
	
	_layerid tempid;
	tempid=ReadLayerID(num);
	count=tempid.layernum;
	if(tempid.checker!=ManagementNum[count])
	{
		//エラー、指定されたレイヤは削除されている可能性が高い。
		return -1;
	}

	if(nflag[count].cxv!=0)
	{
		scale.x=nflag[count].cxv;
	}
	else
	{
		scale.x=1;
	}
	if(nflag[count].cyv!=0)
	{
		scale.y=nflag[count].cyv;
	}
	else
	{
		scale.y=1;
	}
	
	g_pSprite->Draw(g_pTexture[count],NULL,&scale,NULL,NULL,&position,Color);
	return 0;



}
//サイズ指定可能
int Ctexture::DrawImage_Stretch(int num,float x,float y,float w,float h)
{
	D3DXVECTOR2 position,scale;
	position.x=x;
	position.y=y;
	int count=0;

	_layerid tempid;
	tempid=ReadLayerID(num);
	count=tempid.layernum;
	if(tempid.checker!=ManagementNum[count])
	{
		//エラー、指定されたレイヤは削除されている可能性が高い。
		return -1;
	}

	if(nflag[count].cxv!=0)
	{
		scale.x=(w/nflag[count].cx);
	}
	else
	{
		scale.x=(w/Texture_Info[count].Width);
	}
	if(nflag[count].cyv!=0)
	{
		scale.y=(h/nflag[count].cy);
	}
	else
	{
		scale.y=(h/Texture_Info[count].Height);
	}


	g_pSprite->Draw(g_pTexture[count],NULL,&scale,NULL,NULL,&position,Color);
	return 0;


}
//回転描写
int Ctexture::DrawImage_Rotation(int num,float x,float y,float centerx,float centery,float Rotation)
{
	D3DXVECTOR2 position,scale,Rotationcenter;
	Rotationcenter.x=centerx;
	Rotationcenter.y=centery;
	position.x=x;
	position.y=y;
	int count=0;

	_layerid tempid;
	tempid=ReadLayerID(num);
	count=tempid.layernum;
	if(tempid.checker!=ManagementNum[count])
	{
		//エラー、指定されたレイヤは削除されている可能性が高い。
		return -1;
	}

	if(nflag[count].cxv!=0)
	{
		scale.x=nflag[count].cxv;
	}
	else
	{
		scale.x=1;
	}
	if(nflag[count].cyv!=0)
	{
		scale.y=nflag[count].cyv;
	}
	else
	{
		scale.y=1;
	}
	g_pSprite->Draw(g_pTexture[count],NULL,&scale,&Rotationcenter,Rotation,&position,Color);
	return 0;


}
//拡大回転
int Ctexture::DrawImage_StretchRotation(int num,float x,float y,float w,float h,float centerx,float centery,float Rotation)
{
	D3DXVECTOR2 position,scale,Rotationcenter;
	Rotationcenter.x=centerx;
	Rotationcenter.y=centery;
	position.x=x;
	position.y=y;
	int count=0;

	_layerid tempid;
	tempid=ReadLayerID(num);
	count=tempid.layernum;
	if(tempid.checker!=ManagementNum[count])
	{
		//エラー、指定されたレイヤは削除されている可能性が高い。
		return -1;
	}
	if(nflag[count].cxv!=0)
	{
		scale.x=(w/nflag[count].cx);
	}
	else
	{
		scale.x=(w/Texture_Info[count].Width);
	}
	if(nflag[count].cyv!=0)
	{
		scale.y=(h/nflag[count].cy);
	}
	else
	{
		scale.y=(h/Texture_Info[count].Height);
	}
	g_pSprite->Draw(g_pTexture[count],NULL,&scale,&Rotationcenter,Rotation,&position,Color);

	return 0;


}
int Ctexture::DrawImage_Blend(int num, int blend_num, float x, float y,int Rate)
{
	int num_a,num_b;

	num_a=num_b=0;

	_layerid tempid;
	tempid=ReadLayerID(num);
	num_a=tempid.layernum;
	if(tempid.checker!=ManagementNum[num_a])
	{
		//エラー、指定されたレイヤは削除されている可能性が高い。
		return -1;
	}

	tempid=ReadLayerID(blend_num);
	num_b=tempid.layernum;
	if(tempid.checker!=ManagementNum[num_b])
	{
		//エラー、指定されたレイヤは削除されている可能性が高い。
		return -1;
	}

	

	int             xp,yp;
	if (g_pTexture[num_b]->LockRect(0,&B_Rect,NULL,0)!=D3D_OK)
	{
		MessageBox(NULL,"LockRect Error","Surface Error",MB_OK);
		g_pTexture[num_b]->UnlockRect(0);
		 return -1;
	}
	g_pTexture[num_b]->UnlockRect(0);//ただ、アドレスデーターを得たかっただけです。

	//用意する画像サイズはマスク画像のサイズを標準にする(確保)
	m_pD3DDevice->CreateTexture(B_Rect.Pitch/4,
	nflag[num_b].cy_mask,
    0,
    0,
	D3DFMT_A8R8G8B8, 
	D3DPOOL_MANAGED,
	&g_pBlendTexture,
    0);
	//ロック
	if (g_pBlendTexture->LockRect(0,&D_Rect,NULL,0)!=D3D_OK)
	{
		MessageBox(NULL,"LockRect Error","Surface Error",MB_OK);
		g_pBlendTexture->UnlockRect(0);
		 return -1;
	}
			if (g_pTexture[num_a]->LockRect(0,&S_Rect,NULL,0)!=D3D_OK)
	{
		MessageBox(NULL,"LockRect Error","Surface Error",MB_OK);
		g_pTexture[num_a]->UnlockRect(0);
		 return -1;
	}
	g_pTexture[num_a]->UnlockRect(0);


	int Rate_Body;
	float Rateing;
	for(yp=0; yp<nflag[num_a].cy_mask; yp++)//縦分
	{
		for(xp=0; xp<D_Rect.Pitch; xp+=4)//横分
		{
			
			//α
			Rate_Body=(*((BYTE*)B_Rect.pBits+yp*B_Rect.Pitch+xp+3));
				//ブレンド率
			
			if(Rate>128)
			{
				//+
				Rateing=((Rate-128)*2)+Rate_Body;
			}
			else if(Rate<=128)
			{
				//-
				Rateing=((Rate*2)-255)+Rate_Body;

			}
			if(Rateing<0)Rateing=0;
			else if(Rateing>255)Rateing=255;
			


			//RGBの転送
			*((BYTE*)D_Rect.pBits+((yp*D_Rect.Pitch)+xp))=*((BYTE*)S_Rect.pBits+(yp*S_Rect.Pitch+xp));
			*((BYTE*)D_Rect.pBits+((yp*D_Rect.Pitch)+xp+1))=*((BYTE*)S_Rect.pBits+(yp*S_Rect.Pitch+xp+1));
			*((BYTE*)D_Rect.pBits+((yp*D_Rect.Pitch)+xp+2))=*((BYTE*)S_Rect.pBits+(yp*S_Rect.Pitch+xp+2));
			*((BYTE*)D_Rect.pBits+((yp*D_Rect.Pitch)+xp+3))=(BYTE)Rateing;

			


		}
	}
	
	
	D3DXVECTOR2 position,scale; 
	position.x=x;
	position.y=y;
	g_pBlendTexture->UnlockRect(0);


	if(nflag[num_a].cxv!=0)
	{
		scale.x=nflag[num_a].cxv;
	}
	else
	{
		scale.x=1;
	}
	if(nflag[num_a].cyv!=0)
	{
		scale.y=nflag[num_a].cyv;
	}
	else
	{
		scale.y=1;
	}
		g_pSprite->Draw(g_pBlendTexture,NULL,&scale,NULL,NULL,&position,Color);
	
	

	g_pBlendTexture->Release();

	return 0;
}
int Ctexture::DeleteImage_At(int num)
{
	int count;
	DebugText("Delete Texture Layer(num:%d)...",num);
	_layerid tempid;
	tempid=ReadLayerID(num);
	count=tempid.layernum;
	if(tempid.checker!=ManagementNum[count])
	{
		//エラー、指定されたレイヤは削除されている可能性が高い。
		return -1;
	}


	g_pTexture[count]->Release();
	for(int i=count;i<Max;i++)
	{
		g_pTexture[i]=g_pTexture[i+1];
		Texture_Info[i]=Texture_Info[i+1];
		nflag[i]=nflag[i+1];
	}
	Max--;
	if(Max<(totalmax-ADD_MAX_TEXTURE))
	{
	
		//最大数を超えた
		totalmax-=ADD_MAX_TEXTURE;
		nflag=(Snflag*)realloc(nflag,sizeof(Snflag) * totalmax);
		g_pTexture=(LPDIRECT3DTEXTURE9*)realloc(g_pTexture,sizeof(LPDIRECT3DTEXTURE9) * totalmax);
		Texture_Info=(D3DXIMAGE_INFO*)realloc(Texture_Info,sizeof(D3DXIMAGE_INFO) * totalmax);
		ManagementNum=(int*)realloc(ManagementNum,sizeof(int) * totalmax);

	}
	DebugText("OK!\n");
	return 0;

}
int Ctexture::DeleteImage_All(void)
{
	DebugText("All Delete Texture Layer...");
	for(int i=0;i<Max;i++)
	{
		g_pTexture[i]->Release();
	}
	Max=-1;

	
	totalmax=ADD_MAX_TEXTURE;
	nflag=(Snflag*)realloc(nflag,sizeof(Snflag) * totalmax);
	g_pTexture=(LPDIRECT3DTEXTURE9*)realloc(g_pTexture,sizeof(LPDIRECT3DTEXTURE9) * totalmax);
	Texture_Info=(D3DXIMAGE_INFO*)realloc(Texture_Info,sizeof(D3DXIMAGE_INFO) * totalmax);
	ManagementNum=(int*)realloc(ManagementNum,sizeof(int) * totalmax);
	DebugText("OK!\n");
	return 0;
}
void Ctexture::Drawing_start(void)
{
	g_pSprite->Begin();
}
void Ctexture::Drawing_end(void)
{
	g_pSprite->End();

}

void Ctexture::SetBright(int a,int r,int g,int b)
{
	
	Color=D3DCOLOR_ARGB(a,r,g,b);
}

void Ctexture::Allrelease(void)
{
	g_pSprite->Release();
	for(int i=0;i<=Max;i++)
	{
		g_pTexture[i]->Release();
	}

	
	free(nflag);
	free(g_pTexture);
	free(Texture_Info);
	free(ManagementNum);
}

void Ctexture::SetBlendMode(BLEND_MODE mode, int num)
{
	DebugText("Set BlendMode...");
	switch(mode)
	{
	case MODE_ALPHA:
			DebugText("AlphaMode...");
			m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			 if ( num == 255 ) Color = D3DCOLOR_XRGB(255,255,255);
			 else                Color = D3DCOLOR_RGBA(255,255,255,num);
			break;
	}
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DebugText("OK!\n");
}
void Ctexture::NoBlend(void)
{
	DebugText("Clear Blend Mode");
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);//アルファブレンド解除
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	Color = D3DCOLOR_RGBA(255,255,255,255);
	DebugText("OK!\n");

}

int Ctexture::SetLoadImageColorKey(int r, int g, int b)
{
	if(r<0||r>255)
	{
		Color2=0;
	}
	else if(g<0||g>255)
	{
		Color2=0;
	}
	else if(b<0||b>255)
	{
		Color2=0;
	}
	else
	{
		Color2 = D3DCOLOR_RGBA(r,g,b,255);
	}
	return 0;
}
