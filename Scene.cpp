//
// Scene.cpp
// シーンの実装
//

#include <windows.h>
#include <mmsystem.h> //for timeGetTime()
#include "main.h"
#include "Scene.h"

#ifdef _DEBUG
#include <stdio.h> //for sprintf
#endif

//=============================================================
// 実装
//=============================================================
//-------------------------------------------------------------
// デフォルトコンストラクタ
//-------------------------------------------------------------
Scene::Scene()
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
Scene::~Scene()
{

	Destroy();
}

//-------------------------------------------------------------
// シーンを生成
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
// 戻り値
//		成功したらS_OK
//-------------------------------------------------------------
HRESULT Scene::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
	Destroy();





	return S_OK;
}

//-------------------------------------------------------------
// 座標変換を行う
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
//-------------------------------------------------------------
void Scene::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if(pD3DDevice == 0) {
		return;
	}

	// ビューポートの取得
	D3DVIEWPORT9    vp;
	if(FAILED(pD3DDevice->GetViewport(&vp))) {
		return;
	}

	// アスペクト比の計算
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

}

//-------------------------------------------------------------
// オブジェクト等の描画
// 引数
//		pD3DDevice : IDirect3DDevice9 インターフェイスへのポインタ
//-------------------------------------------------------------
void Scene::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if(pD3DDevice == 0) {
		return;
	}
	if(LoadFlag!=559)
	{
				D3DTEXVERTEX v[]=
		{
		  { -1.0f, +1.5f, 0.0f, 0xffffffff, 1.0f, 0.0f},
		  { -1.0f, -1.5f, 0.0f, 0xffffffff, 1.0f, 1.0f}, 
		  { +1.0f, +1.5f, 0.0f, 0xffffffff, 0.0f, 0.0f}, 
		  { +1.0f, -1.5f, 0.0f, 0xffffffff, 0.0f, 1.0f}
		};

	LoadFlag=559;
	}
	
	// レンダリングステートパラメータの設定
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	// 座標変換（カメラの設定など）
	Transform(pD3DDevice);

  // 頂点バッファのフォーマットの設定
    pD3DDevice->SetFVF(D3DFVF_TEXVERTEX);
  // 頂点バッファの描画

//    pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2 );


}

//-------------------------------------------------------------
// シーンの破棄
//-------------------------------------------------------------
void Scene::Destroy()
{
//	SAFE_RELEASE(m_pTexture);
	//SAFE_RELEASE(m_pMyVB);
}
