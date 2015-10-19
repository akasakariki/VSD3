#pragma once
#include "dx9sdk\Include\d3d9.h"
#include "dx9sdk\Include\d3dx9.h"
#include "Scene.h"
#include "FileReader.h"


#define ADD_MAX_TEXTURE 99
enum BLEND_MODE{
	MODE_ALPHA
};
struct Snflag
{
	float cxv;
	float cyv;
	float cx;
	float cy;
	float cy_mask;
};


class Ctexture
{
private:
	LPDIRECT3DDEVICE9		m_pD3DDevice;//�e�N�X�`���[�p
	LPDIRECT3D9				m_pD3D;	

	LPD3DXSPRITE       g_pSprite;
	LPDIRECT3DVERTEXBUFFER9 buffers;
	D3DCOLOR Color;//���P�x�ݒ�
	D3DCOLOR Color2;//�����F
	D3DLOCKED_RECT            D_Rect,S_Rect,B_Rect;
	LPDIRECT3DTEXTURE9 g_pBlendTexture;
	int Max;
	
	float sc;        // �ʓx���ߌW��
	float sgn;      // �؂�ւ�����
	float scale[4];      // �ʓx
	int test_flag;
	//2��n��ł͂Ȃ��ꍇ�̏������̔��f����я����p

	Snflag *nflag;
	LPDIRECT3DTEXTURE9 *g_pTexture;//�e�N�X�`���{��
	D3DXIMAGE_INFO	*Texture_Info;//�e�N�X�`�����*
	int *ManagementNum;//�Ǘ��ԍ�
	int totalmax;
	HWND					*hwnd;

public:
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
	void SettingTexture(LPDIRECT3DDEVICE9 m_pD3DDevice_dist,LPDIRECT3D9 m_pD3D_dist);
	int SetLoadImageColorKey(int r,int g,int b);
	int LoadImage(char FileName[],CFileReader *fr);
	int LoadImage_Mask(char FileName[],CFileReader *fr);
	int DrawImage(int num,float x,float y);
	int DrawImage_Stretch(int num,float x,float y,float w,float h);
	int DrawImage_StretchRotation(int num,float x,float y,float w,float h,float centerx,float centery,float Rotation);
	int DrawImage_Rotation(int num,float x,float y,float centerx,float centery,float Rotation);
    int DrawImage_Blend(int num,int blend_num,float x,float y,int Rate);
	void Drawing_start(void);
	void Drawing_end(void);
	void SetBright(int a,int r,int g,int b);
	void Allrelease(void);
	int DeleteImage_At(int num);
	int DeleteImage_All(void);
	void SetBlendMode(BLEND_MODE mode,int num);
	void NoBlend(void);

};


