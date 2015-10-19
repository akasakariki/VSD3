#include "main.h"
#include "Text.h"
#include "Renderer.h"
#include <stdlib.h>    // rand, srand�g�p

#include "Common.h"
//-----------------------------------------------------------
//�t�H���g�����i�쐬���A�h���X�����n���j
//-----------------------------------------------------------
int Ctext::SettingText(LPDIRECT3DDEVICE9 *pD3DDevice_dist)
{
	pD3DDevice=pD3DDevice_dist;
	//�������m��
	Totalmax=ADD_MAX_FONT;
	Max=-1;
	ManagementNum=(int*)malloc(sizeof(int) *Totalmax);
	lpFont=(LPD3DXFONT*)malloc(sizeof(LPD3DXFONT) *Totalmax);
	size=(int*)malloc(sizeof(int) *Totalmax);
	return 0;
}
//-------------------------------------------------------------
//�t�H���g�I�u�W�F�N�g�ݒ�
//-------------------------------------------------------------
int Ctext::SetTextFont(int Size,int Angle,int Thickness,bool Oblique,bool Under,bool Cancellation,char FontNAME[] )
{
	DebugText("Set TextFont(fontname:%s)...",FontNAME);
	Max++;
	if(Max>=Totalmax)
	{
		//�ő吔�𒴂���
		Totalmax+=ADD_MAX_FONT;
		ManagementNum=(int*)realloc(ManagementNum,sizeof(int) *Totalmax);
		lpFont=(LPD3DXFONT*)realloc(lpFont,sizeof(LPD3DXFONT) *Totalmax);
		size=(int*)realloc(size,sizeof(int) *Totalmax);
	}
	HFONT hFont;
	int error;
	hFont=CreateFont(
		Size,              // ����
		0,               // ��
		Angle,          // ������������̊p�x
		0,         // �x�[�X���C���̊p�x
		Thickness,             // ����
		Oblique,          // �Α̂��ǂ���
		Under,       // �������������ǂ���
		Cancellation,       // �����������������ǂ���
		DEFAULT_CHARSET,         // �����Z�b�g
		OUT_DEFAULT_PRECIS, // �o�͐��x
		CLIP_DEFAULT_PRECIS,   // �N���b�s���O���x
		PROOF_QUALITY,         // �o�͕i��
		FIXED_PITCH | FF_MODERN,  // �s�b�`�ƃt�@�~��
		FontNAME         // �t�H���g��
	);
	size[Max]=Size;
   error=D3DXCreateFont(*pD3DDevice,hFont,&lpFont[Max]) ;
    
	if(error==D3DERR_INVALIDCALL)
	{
		//�p�����[�^�[�~�X
	}
	else if(error==E_OUTOFMEMORY)
	{
		//�������̊m�ێ��s
	}
	DeleteObject(hFont);
	ManagementNum[Max]=~Max;
	DebugText("OK!\n");
	return ManagementNum[Max];
}


//-------------------------------------------------------------
// �t�H���g�I�u�W�F�N�g�`��
// ����
//		pD3DDevice : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
//      LPD3DXFONT &lpFont�t�H���g�t�F�C�X�ւ̃A�h���X���p
//-------------------------------------------------------------
int Ctext::DrawText(int num,int x,int y,int alpha,int r,int g,int b,char String[])
{
	int count=~num;

	// �O�p�`�̕`�揈��
	// ���_�o�b�t�@�̐ݒ�
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
			//�ő吔�𒴂���
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