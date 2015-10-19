//=============================================================
// Scene.h
// シーンクラスの定義
//=============================================================

#ifndef _Scene_h_
#define _Scene_h_

#include "dx9sdk\Include\d3d9.h"
#include "dx9sdk\Include\d3dx9.h"
#include "Common.h"

//=============================================================
// 依存するクラス
//=============================================================

//=============================================================
// Scene
// シーンクラス
//=============================================================
class Scene
{
public:
	//---------------------------------------------------------
	// デフォルトコンストラクタ
	//---------------------------------------------------------
	Scene();

	//---------------------------------------------------------
	// デストラクタ
	//---------------------------------------------------------
	~Scene();

public:
	//---------------------------------------------------------
	// シーンを生成
	// 引数
	//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
	// 戻り値
	//		成功したらS_OK
	//---------------------------------------------------------
	HRESULT Create(LPDIRECT3DDEVICE9 pD3DDevice);

	//---------------------------------------------------------
	// 座標変換を行う
	// 引数
	//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
	//---------------------------------------------------------
	void Transform(LPDIRECT3DDEVICE9 pD3DDevice);


	//---------------------------------------------------------
	// シーンの破棄
	//---------------------------------------------------------
	void Destroy();
	void Draw(LPDIRECT3DDEVICE9 pD3DDevice);
	
private:
	int LoadFlag;

	 LPD3DXEFFECT       ppEffect;
// ＦＶＦの設定
	//#define D3DFVF_TEXVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
	#define D3DFVF_TEXVERTEX  (D3DFVF_XYZ | D3DFVF_DIFFUSE  | D3DFVF_TEX1)
	HWND					*hwnd;
	HWND* AddressHWND(HWND *hwnd_dist)
	{
		if(hwnd!=0)
		{
			hwnd=hwnd_dist;
			return 0;
		}
		else
		{
			return hwnd;
		}
	}

};
typedef struct _D3DTEXVERTEX{
	float x,y,z;// 頂点座標
	DWORD color;// 頂点カラー
	float u,v;// UV座標
	}D3DTEXVERTEX;
#endif // _Scene_h_