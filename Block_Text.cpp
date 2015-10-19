#include "Text.h"


void CBlockText::SettingBlockText(Ctext *desttext)
{
	DebugText("TextBlockInitialize...");
	text=desttext;
	Totalmax=ADD_MAX_BT;
	blocktext=(_bt*)malloc(sizeof(_bt) *Totalmax);
	ManagementNum=(int*)malloc(sizeof(int)*Totalmax);
	Max=-1;
DebugText("OK!\n");
}

int  CBlockText::SetBlockText(int fontnums,int x,int y,int r,int g,int b,int alpha,char bodysw[])
{
	DebugText("SetBlockText(%s)...",bodysw);
	Max++;
	if(Max>=Totalmax)
	{
		Totalmax+=ADD_MAX_BT;
		blocktext=(_bt*)realloc(blocktext,sizeof(_bt) *Totalmax);
		ManagementNum=(int*)realloc(ManagementNum,sizeof(int)*Totalmax);
	}
	blocktext[Max].x=x;
	blocktext[Max].y=y;
	blocktext[Max].r=r;
	blocktext[Max].g=g;
	blocktext[Max].b=b;
	blocktext[Max],alpha=alpha;
	blocktext[Max].fontnum=fontnums;
	wsprintf(blocktext[Max].body,"%s",bodysw);
	ManagementNum[Max]=rand();
	DebugText("OK!\n");
	return ManagementNum[Max];
}

void CBlockText::DrawBlockText(void)
{
	for(int i=0;i<=Max;i++)
	{
		text->DrawTextA(blocktext[i].fontnum,blocktext[i].x,blocktext[i].y,blocktext[i].alpha,blocktext[i].r,blocktext[i].g,blocktext[i].b,blocktext[i].body);
	}
}

int CBlockText::DeleteBlockText_At(int num)
{
	DebugText("DeleteBlockText...");
	for(int count=0;count<Max;count++)
	{
		if(ManagementNum[count]==num)
		{
			Max--;

			if(Max<(Totalmax-ADD_MAX_FONT))
			{
				//Å‘å”‚ð’´‚¦‚½
				Totalmax-=ADD_MAX_FONT;
				ManagementNum=(int*)realloc(ManagementNum,sizeof(int) *Totalmax);
				blocktext=(_bt*)realloc(blocktext,sizeof(_bt) *Totalmax);
			}
			return 0;
		}
	}
	DebugText("OK!\n");
	return -1;//ƒGƒ‰[ŠÇ—”Ô†‚ªŒ©•t‚©‚ç‚È‚¢
}

int CBlockText::DeleteBlockText_All(void)
{
	DebugText("AllDeleteBlockText...");
	text->DeleteTextFont_All();
	Totalmax=ADD_MAX_FONT;
	blocktext=(_bt*)realloc(blocktext,sizeof(_bt) *Totalmax);
	ManagementNum=(int*)realloc(ManagementNum,sizeof(int)*Totalmax);
	Max=-1;
	DebugText("OK!\n");
	return 0;
}

int CBlockText::AllRelease()
{
	free(blocktext);
	free(ManagementNum);
	return 0;
}