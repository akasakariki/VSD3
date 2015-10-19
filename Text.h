#pragma once
#include "dx9sdk\Include\d3d9.h"
#include "dx9sdk\Include\d3dx9.h"
#include "Scene.h"
#define ADD_MAX_FONT 99
class Ctext
{
public:
	int SettingText(LPDIRECT3DDEVICE9 *pD3DDevice_dist);
	//-------------------------------------------------
	//�e�L�X�g�p�t�H���g�ݒ�
	//int Size			�����̑傫��
	//int Angle�@		�����̊p�x
	//int Thickness		�����̑���
	//bool Oblique		�ΐ�
	//bool Under		����
	//bool Cancellation	�ł�������
	//char FontNAME[] �@�t�H���g���i���C���I�Ȃǁj
	//�t�H���g�Ǘ��ԍ���int�ŕԂ��܂��i0~99�j
	//-------------------------------------------------
	int SetTextFont(int Size,int Angle,int Thickness,bool Oblique,bool Under,bool Cancellation,char FontNAME[] );
	
	//-------------------------------------------------
	//�����̕\��
	//int num			�t�H���g�Ǘ��ԍ�
	//int x				������X�ʒu
	//int y				������Y�ʒu
	//int alpha			�����̃A���t�@�l�i�����j�O�`�Q�T�T
	//int r				R�l�i�Ԍn�j�O�`�Q�T�T
	//int g				G�l�i�Όn�j�O�`�Q�T�T
	//int b				B�l�i�n�j�O�`�Q�T�T
	//char String[]		�{��
	//����Ȃ�O��Ԃ��l�Ƃ��܂��B
	//-------------------------------------------------
	int DrawText(int num,int x,int y,int alpha,int r,int g,int b,char String[]);
	void reset_font(void);
	int DeleteTextFont_At(int num);
	int DeleteTextFont_All(void);
	int return_size(int num);
	int AllRelease(void);
private:
	int Max;
	int Totalmax;
	LPD3DXFONT	*lpFont;//�t�H���g*
	int *ManagementNum;//�Ǘ��ԍ�*
	LPDIRECT3DDEVICE9 *pD3DDevice;
	int *size;//*
};


#include <string>
#include <iostream>
#define ADD_MAX_BT	99
struct _bt
{
	int x;
	int y;
	int r;
	int g;
	int b;
	int alpha;
	int fontnum;
	char body[30];
	
};

//�u���b�N�e�L�X�g
class CBlockText
{
private:
	Ctext *text;
	_bt *blocktext;
	int Totalmax;
	int Max;
	int *ManagementNum;
public:
	void SettingBlockText(Ctext *text);
	int DeleteBlockText_At(int num);
	void DrawBlockText(void);
	int SetBlockText(int fontnums,int x,int y,int r,int g,int b,int alpha,char bodysw[]);
	int DeleteBlockText_All(void);
	int AllRelease(void);
};

