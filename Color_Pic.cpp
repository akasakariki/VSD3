/*
カラーピック変換モジュール（16進数→10進数）
ADV専用改変バージョン
by.時雨四季（JRDSN)
*/
#include <stdio.h> 
#include <windows.h>
#include "Ex_ColorPic.h"
#include "Common.h"
int SetMode;
char ColorChord[8];
struct RGBChord
{
	int red[2];
	int green[2];
	int blue[2];
};
int Convert(const char s);
ColorPic ColorPIC(char *ColorChord_USER) 
{
	DebugText("カラーコードを解析、変換します。(%s)...",ColorChord_USER);
	RGBChord rgb;
	ColorPic returnpic;
			//wsprintf(ColorChord,"%S",ColorChord_USER);

			for(int is=0;is<=7;is++)ColorChord[is]=ColorChord_USER[is];
			for(int is=0;is<=1;is++)
			{
				rgb.red[is]=Convert(ColorChord[is+1]);
				rgb.green[is]=Convert(ColorChord[is+3]);
				rgb.blue[is]=Convert(ColorChord[is+5]);

			}
			returnpic.red=(rgb.red[0]*16)+(rgb.red[1]*1);
			returnpic.green=(rgb.green[0]*16)+(rgb.green[1]*1);
			returnpic.blue=(rgb.blue[0]*16)+(rgb.blue[1]*1);

			DebugText("OK!(%d:%d:%d)\n",rgb.red,rgb.green,rgb.blue);
		return returnpic;
}

int Convert(const char s)
{
	int returnint=0;
	  switch (s) //16進数を変換（文字列を変換します）
	  { 
			case'1':
			returnint=1;
			break;
			case'2':
			returnint=2;
			break;
			case'3':
			returnint=3;
			break;
			case'4':
			returnint=4;
			break;
			case'5':
			returnint=5;
			break;
			case'6':
			returnint=6;
			break;
			case'7':
			returnint=7;
			break;
			case'8':
			returnint=8;
			break;
			case'9':
			returnint=9;
			break;
			case'a':
			returnint=10;
			break;
			case'b':
			returnint=11;
			break;
			case'c':
			returnint=12;
			break;
			case'd':
			returnint=13;
			break;
			case 'e':
			returnint=14;
			break;
			case 'f':
			returnint=15;
			break;


			case'A':
			returnint=10;
			break;
			case'B':
			returnint=11;
			break;
			case'C':
			returnint=12;
			break;
			case'D':
			returnint=13;
			break;
			case 'E':
			returnint=14;
			break;
			case 'F':
			returnint=15;
			break;


	  }
	  return returnint;
}