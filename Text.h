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
	//テキスト用フォント設定
	//int Size			文字の大きさ
	//int Angle　		文字の角度
	//int Thickness		文字の太さ
	//bool Oblique		斜線
	//bool Under		下線
	//bool Cancellation	打ち消し線
	//char FontNAME[] 　フォント名（メイリオなど）
	//フォント管理番号をintで返します（0~99）
	//-------------------------------------------------
	int SetTextFont(int Size,int Angle,int Thickness,bool Oblique,bool Under,bool Cancellation,char FontNAME[] );
	
	//-------------------------------------------------
	//文字の表示
	//int num			フォント管理番号
	//int x				文字のX位置
	//int y				文字のY位置
	//int alpha			文字のアルファ値（薄さ）０～２５５
	//int r				R値（赤系）０～２５５
	//int g				G値（緑系）０～２５５
	//int b				B値（青系）０～２５５
	//char String[]		本文
	//正常なら０を返し値とします。
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
	LPD3DXFONT	*lpFont;//フォント*
	int *ManagementNum;//管理番号*
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

//ブロックテキスト
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

