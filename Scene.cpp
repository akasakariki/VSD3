//
// Scene.cpp
// �V�[���̎���
//

#include <windows.h>
#include <mmsystem.h> //for timeGetTime()
#include "main.h"
#include "Scene.h"

#ifdef _DEBUG
#include <stdio.h> //for sprintf
#endif

//=============================================================
// ����
//=============================================================
//-------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//-------------------------------------------------------------
Scene::Scene()
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
Scene::~Scene()
{

	Destroy();
}

//-------------------------------------------------------------
// �V�[���𐶐�
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
// �߂�l
//		����������S_OK
//-------------------------------------------------------------
HRESULT Scene::Create(LPDIRECT3DDEVICE9 pD3DDevice)
{
	Destroy();





	return S_OK;
}

//-------------------------------------------------------------
// ���W�ϊ����s��
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
//-------------------------------------------------------------
void Scene::Transform(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if(pD3DDevice == 0) {
		return;
	}

	// �r���[�|�[�g�̎擾
	D3DVIEWPORT9    vp;
	if(FAILED(pD3DDevice->GetViewport(&vp))) {
		return;
	}

	// �A�X�y�N�g��̌v�Z
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

}

//-------------------------------------------------------------
// �I�u�W�F�N�g���̕`��
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
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
	
	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	// ���W�ϊ��i�J�����̐ݒ�Ȃǁj
	Transform(pD3DDevice);

  // ���_�o�b�t�@�̃t�H�[�}�b�g�̐ݒ�
    pD3DDevice->SetFVF(D3DFVF_TEXVERTEX);
  // ���_�o�b�t�@�̕`��

//    pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2 );


}

//-------------------------------------------------------------
// �V�[���̔j��
//-------------------------------------------------------------
void Scene::Destroy()
{
//	SAFE_RELEASE(m_pTexture);
	//SAFE_RELEASE(m_pMyVB);
}
