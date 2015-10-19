#pragma once
#include "dx9sdk\Include\dmusici.h"

#define ADD_MAX_SOUND 99


class Csound
{
protected:
	HRESULT	hr;//エラー情報

	IDirectMusicPerformance8	*m_pPerformance[ADD_MAX_SOUND];
	IDirectMusicAudioPath		*m_pDefAudioPath;
	IDirectMusicLoader8			*m_pLoader;
	IDirectMusicSegment8		*g_pSegment[ADD_MAX_SOUND];		// 音オブジェクト
	int ManagementNum[ADD_MAX_SOUND];//管理番号
	int Max;
public:
	int DX_SoundStart(void);
	int DX_SoundRelease(void);
	int LoadSound(char LoadFileName[]);
	int PlaySound(int num,int kai);
	int StopSound_At(int num);
	int StopSound_All(void);
	int DeleteSound_At(int num);
	int DeleteSound_All(void);
	void SetSoundVol(int num,long vol);//+10 ～ -100db=10db=1000
	long GetSoundVol(int num); 
	int GetCheckPlaying(int num);

	//ogg用ロード関数
	int LoadOggFile(char FileName[]);
};