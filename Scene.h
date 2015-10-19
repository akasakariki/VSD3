//=============================================================
// Scene.h
// �V�[���N���X�̒�`
//=============================================================

#ifndef _Scene_h_
#define _Scene_h_

#include "dx9sdk\Include\d3d9.h"
#include "dx9sdk\Include\d3dx9.h"
#include "Common.h"

//=============================================================
// �ˑ�����N���X
//=============================================================

//=============================================================
// Scene
// �V�[���N���X
//=============================================================
class Scene
{
public:
	//---------------------------------------------------------
	// �f�t�H���g�R���X�g���N�^
	//---------------------------------------------------------
	Scene();

	//---------------------------------------------------------
	// �f�X�g���N�^
	//---------------------------------------------------------
	~Scene();

public:
	//---------------------------------------------------------
	// �V�[���𐶐�
	// ����
	//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
	// �߂�l
	//		����������S_OK
	//---------------------------------------------------------
	HRESULT Create(LPDIRECT3DDEVICE9 pD3DDevice);

	//---------------------------------------------------------
	// ���W�ϊ����s��
	// ����
	//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
	//---------------------------------------------------------
	void Transform(LPDIRECT3DDEVICE9 pD3DDevice);


	//---------------------------------------------------------
	// �V�[���̔j��
	//---------------------------------------------------------
	void Destroy();
	void Draw(LPDIRECT3DDEVICE9 pD3DDevice);
	
private:
	int LoadFlag;

	 LPD3DXEFFECT       ppEffect;
// �e�u�e�̐ݒ�
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
	float x,y,z;// ���_���W
	DWORD color;// ���_�J���[
	float u,v;// UV���W
	}D3DTEXVERTEX;
#endif // _Scene_h_