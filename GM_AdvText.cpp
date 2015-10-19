#include "GM_AdvText.h"
#include "main.h"
#include <stdio.h>

int CADV::Set_textwindow(int size, int dist_w, int dist_h)
{
	litedrawf=LiteDrawing;

	DebugText("SetTextWindow...");
	int w,h;
	w=(dist_w/(size/2));
	h=(dist_h/(size/2));
	if(w>CLIENT_WIDTH||h>CLIENT_HEIGHT)
	{
		//エラー
		//画面内からはみ出る可能性があります
		return -1;
	}

	textbody=new char*[h];
	data=new int***[h];
	for(int i=0;i<h;i++)
	{
		textbody[i]=new char[w];
		data[i]=new int**[w];
		for(int k=0;k<w;k++)
		{
			data[i][k]=new int*[DateNum];
			for(int js=0;js<DateNum;js++)data[i][k][js]=new int[5];//ものすごい・・・多次元配列・・

		}
	}
	flag=1;
	//念のため初期化処理
	for(int i=0;i<h;i++)
	{
		for(int k=0;k<w;k++)
		{
			textbody[i][k]=0;
			for(int ii=0;ii<DateNum;ii++)
			{
				for(int js=0;js<5;js++)data[i][k][ii][js]=0;
			}
		}
	}
	char_w=w;
	char_h=h;
	char_size=size;
	SP=0;
	CP=0;

	//ふりがなシステムの準備
	furigana = (_furigana*)malloc(sizeof(_furigana) * furigana_max);//メモリ確保
	furigana_max=-1;
	furigana_totalmax=furigana_max;
	furigana_mode=0;
	DebugText("OK!\n");
	return 0;
}

int CADV::Release_textwindow()
{
	for(int i=0;i<char_h;i++)
	{
		delete	[] textbody[i];
		for(int k=0;k<char_w;k++)
		{
			delete [] data[i][k];
		}
		delete [] data[i];
	}
	delete [] textbody;
	delete [] data;
	return 0;	
}

int CADV::stack_text(char moji,int datas[DateNum])
{

	if(furigana_mode!=2)
	{
		if(char_w<=CP&&_ismbblead(textbody[SP][CP-1])==0)
		{
			//マルチバイト
			SP++;
			CP=0;
			if(SP>=char_h)
			{
				//フォーマット
				format_text(text);
				SP=0;
			}
		}
		textbody[SP][CP]=moji;
	}
	if(datas[0]!=0)
	{
		for(int i=0;i<DateNum;i++)
		{
			for(int k=0;k<DateNum;k++)
			data[SP][CP][i][k]=datas[i];
		}
	}
	CP++;
	if(furigana_mode==1)
	{
		furigana_moji++;

	}
	else if(furigana_mode==2)
	{

		furigana[furigana_max].furigana_string[ furigana_moji_CP]=moji;
		furigana_moji_CP++;
		CP--;//+-0
	}
	return 0;
}

int CADV::format_text(Ctext *text_dest)
{
	DebugText("ClearText...");
	adv_moji_blank_w=def_adv_moji_blank_w;
	adv_moji_blank_h=def_adv_moji_blank_h;
	CP=SP=0;
	text=text_dest;
	for(int i=0;i<char_h;i++)
	{
		for(int k=0;k<char_w;k++)
		{
			textbody[i][k]=0;
			for(int ii=0;ii<DateNum;ii++)data[i][k][ii]=0;
		}
	}
	DebugText("OK!\n");
	return 0;

}
void CADV::Draw_text(int num,int x,int y,int w,int h,int alpha)
{
	
	int r,g,b;
	r=g=b=255;
	char Moji[3];
	int k=0;
	int size=text->return_size(num);
	if(litedrawf!=true)
	{
		for(int s=0;s<(h/(size/2));s++)
		{
			for(int i=0;i<(w/(size/2));i++)
			{
				if( _ismbblead(textbody[s][i])!=0)
				{
					
					if(textbody[s][i+1]=='\0')
					{ 
						k=1;
						break;
					}
					Moji[0]=textbody[s][i];
					Moji[1]=textbody[s][i+1];
					Moji[2]='\0';
				} 
				else 
				{
					//半角英数字を取得するにはこちらに代入を記載
					Moji[0]=textbody[s][i];
					Moji[1]='\0';
					
				}
				text->DrawTextA(num,(int)x+(((size/2)+adv_moji_blank_w)*i),(int)y+(((size/2)+adv_moji_blank_h)*s),alpha,r,g,b,Moji);
				if( _ismbblead(textbody[s][i])!=0)i++;
				if(textbody[s][i]=='\0')
				{
					k=1;
					break;
				}
			}
			if(k==1)break;

		}
	}
	else
	{
		//簡易描写
		for(int i=0;i<textbody[i][0]!=0;i++)
		text->DrawTextA(num,(int)x,(int)y+(((size/2)+adv_moji_blank_h)*i),alpha,r,g,b,textbody[i]);
	}
	

}

void CADV::set_furigana_mode()
{


	furigana_max++;
	
	if(furigana_max>=furigana_totalmax)
	{
		_furigana * temp;
		//領域を拡大
		furigana_totalmax+=furigana_maxs;
		temp=(_furigana *)realloc(furigana , (sizeof(_furigana) * furigana_totalmax));
		if(temp!=NULL)
		{
			furigana=temp;
		}
		else
		{
			//エラー
		}
	}
	furigana_temp_x=(int)(((char_size/2)+adv_moji_blank_w)*CP);
	furigana_temp_y=(int)(((char_size/2)+adv_moji_blank_h)*SP);
	furigana_temp_y-=(furigana_fontsize);
	furigana[furigana_max].r=255;
	furigana[furigana_max].g=255;
	furigana[furigana_max].b=255;
	furigana_mode=1;
	furigana_moji=0;
	DebugText("Set 'Furigana' Mode.\n");
}
void CADV::set2_furigana_mode()
{
	

	furigana_moji_CP=0;
	furigana_mode=2;

}
void CADV::out_furigana_mode()
{
	
	furigana[furigana_max].furigana_string[ furigana_moji_CP]='\0';
	
	int furiganalensize=strlen(furigana[furigana_max].furigana_string);
	furiganalensize*=(furigana_fontsize/2);
	furigana_moji*=(char_size/2);
	int furigana_temp_xs=furigana_moji-furiganalensize;
	furigana_temp_xs/=2;
	furigana[furigana_max].x=furigana_temp_x+furigana_temp_xs;
	furigana[furigana_max].y=furigana_temp_y;
	furigana[furigana_max].r=255;
	furigana[furigana_max].g=255;
	furigana[furigana_max].b=255;
	furigana_mode=0;
	furigana_moji=0;
	DebugText("Out 'Furigana' Mode.\n");
}

void CADV::Draw_furigana(int x,int y,int alpha)
{
	for(int i=0;i<=furigana_max;i++)
	{
		if(furigana_max<0)break;
		text->DrawTextA(furigana_fontnum,(int)x+furigana[i].x,(int)y+furigana[i].y,alpha,furigana[i].r,furigana[i].g,furigana[i].b,furigana[i].furigana_string);
	}
}

int CADV::Set_furiganafont(int size,char fontname[])
{
	DebugText("Set 'Furigana' Font...");
	//text->DeleteTextFont_At(furigana_fontnum);
	furigana_fontnum=text->SetTextFont(size,0,3,false,false,false,fontname );
	furigana_fontsize=size;
	DebugText("OK!\n");
	return 0;
}