#include "Sound.h"
#include <stdlib.h>    // rand, srand使用
#include "Common.h"
#include "Vorbis.h"
int Csound::DX_SoundStart(void)
{
	DebugText("StartupSound...");
	m_pDefAudioPath=NULL;
	m_pLoader=NULL;
	Max=0;
	if(FAILED(CoInitialize(NULL))){
		return -1;	//　エラー戻り
	}
	hr = CoCreateInstance(
		CLSID_DirectMusicLoader,
		NULL,
		CLSCTX_INPROC,
		IID_IDirectMusicLoader8,
		(void**)&m_pLoader);
	if(FAILED(hr)){
		DebugText("NG!\n");
		return -5; }		// エラー戻り 
	DebugText("OK!\n");

	return 0;
}

int Csound::LoadSound(char LoadFileName[])
{
	DebugText("LoadSound(name=>%s)...",LoadFileName);
	if(Max>=ADD_MAX_SOUND)
	{
		//最大数を超えた
		return -1;
	}
	char ext[5];
	GetExt(LoadFileName,ext);
	if(strcmp(ext,"ogg")==0)//ogg
	{
		LoadOggFile(LoadFileName);
	}
	else
	{
			hr = CoCreateInstance(
			CLSID_DirectMusicPerformance,
			NULL,
			CLSCTX_INPROC,
			IID_IDirectMusicPerformance8,
			(void**)&m_pPerformance[Max]);
			
		if(FAILED(hr)){DebugText("NG!(ERROR2)\n"); return -2 ; }		// エラー戻り 

		hr = m_pPerformance[Max]->InitAudio(
			NULL,
			NULL,
			NULL,	
			DMUS_APATH_SHARED_STEREOPLUSREVERB,
			64,
			DMUS_AUDIOF_ALL,
			NULL);
		if(FAILED(hr)){DebugText("NG!(ERROR3)\n"); return -3; }		// エラー戻り

		hr = m_pPerformance[Max]->GetDefaultAudioPath(&m_pDefAudioPath);
		if(FAILED(hr)){DebugText("NG!(ERROR4)\n"); return -4; }		// エラー戻り 
		WCHAR wstrFileName[ MAX_PATH ];
		MultiByteToWideChar( CP_ACP, 0, LoadFileName, -1,wstrFileName, MAX_PATH );

		hr = m_pLoader->LoadObjectFromFile(
				CLSID_DirectMusicSegment,
				IID_IDirectMusicSegment8,
				wstrFileName,
				(void**)&g_pSegment[Max]);

		if(FAILED(hr))
		{
			DebugText("NG!  Not Found Sound Data\n");
			//エラー（ファイルが見付からない）
			 MessageBox(NULL,"Media not found","DirectMusic",MB_OK);
		   m_pPerformance[Max]->CloseDown();
		   m_pPerformance[Max]->Release();
			CoUninitialize();
			return -5;

		}
	}
	
	//管理番号返し
	ManagementNum[Max]=rand();
	Max++;

	return ManagementNum[Max-1];
}

int Csound::LoadOggFile(char FileName[])
{
	Cvorbis vorbis;
	DMUS_OBJECTDESC desc;
	vorbis.Load_ogg(FileName);
	desc=vorbis.Return_Desc();
	m_pLoader->GetObject( &desc, IID_IDirectMusicSegment8, (void**)&g_pSegment[Max] );
	return 0;
}

int Csound::PlaySound(int num,int kai)
{
	DebugText("PlaySound(num=>%s kai=>%d)...",num,kai);
	int count=0;

	for(count=0;count<Max;count++)
	{
		if(ManagementNum[count]==num)
		{
			g_pSegment[count]->Download( m_pPerformance[count] );

			if(kai==0){
				g_pSegment[count]->SetRepeats( DMUS_SEG_REPEAT_INFINITE  );
			}else
			{
				g_pSegment[count]->SetRepeats( kai-1 );
			}
			m_pPerformance[count]->PlaySegmentEx(g_pSegment[count], NULL, NULL, 0, 0, NULL, NULL, NULL );
			DebugText("OK!\n");
			return 0;
		}
	}
	//エラー管理番号なし
	DebugText("NG!  Not Found Sound Layer\n");
	return -1;
}
int Csound::DX_SoundRelease(void)
{
	// 演奏停止
	
	for(int count=0;count<Max;count++)
	{
		m_pPerformance[count]->Stop( NULL, NULL, 0, 0 );
		g_pSegment[count]->Unload( m_pPerformance[count] );

		SAFE_RELEASE(g_pSegment[count] );
		m_pPerformance[count]->CloseDown();
		SAFE_RELEASE(m_pPerformance[count]);
		if(count==(Max-1))
		{
			SAFE_RELEASE( m_pLoader);
		}
	}
	
	
	
	CoUninitialize();//COM解放
	return 0;

}

int Csound::StopSound_At(int num)
{
	DebugText("StopSound(num=>%d)...",num);
	for(int count=0;count<Max;count++)
	{
		if(ManagementNum[count]==num)
		{
			m_pPerformance[count]->Stop( NULL, NULL, 0, 0 );
			DebugText("OK!\n");
			return 0;
		}

	}
	DebugText("NG!\n");
	return -1;
}
int Csound::StopSound_All(void)
{
	DebugText("AllStopSound...");
	for(int count=0;count<Max;count++)
	{

		m_pPerformance[count]->Stop( NULL, NULL, 0, 0 );
		

	}
	DebugText("OK!\n");
	return 0;
}


int Csound::DeleteSound_At(int num)
{
	DebugText("DeleteSound(num=>%d)...",num);
	for(int count=0;count<Max;count++)
	{
		if(ManagementNum[count]==num)
		{
			m_pPerformance[count]->Stop( NULL, NULL, 0, 0 );
			g_pSegment[count]->Unload( m_pPerformance[count] );
			m_pPerformance[count]->CloseDown();
			SAFE_RELEASE(g_pSegment[count] );
			SAFE_RELEASE(m_pPerformance[count] );
			for(int i=count;i<Max;i++)
			{
				m_pPerformance[i]=m_pPerformance[i+1];
				g_pSegment[i]=g_pSegment[i+1];

			}
		}

	}
	Max--;
	DebugText("OK!\n");
	return 0;
}

int Csound::DeleteSound_All(void)
{
	DebugText("AllDeleteSound...");
	for(int count=0;count<Max;count++)
	{
		m_pPerformance[count]->Stop( NULL, NULL, 0, 0 );
		g_pSegment[count]->Unload( m_pPerformance[count] );
		m_pPerformance[count]->CloseDown();
		SAFE_RELEASE(g_pSegment[count] );
		SAFE_RELEASE(m_pPerformance[count] );

	}
	Max=0;
	DebugText("OK!\n");
	return 0;
}

void Csound::SetSoundVol(int num,long vol)
{
	DebugText("SetSoundVol(num=>%d vol=>%d)...",num,vol);
	for(int count=0;count<Max;count++)
	{
		if(ManagementNum[count]==num)
		{
			m_pPerformance[count]->SetGlobalParam( GUID_PerfMasterVolume, &vol, sizeof(long) );

		}

	}
	DebugText("OK!\n");
}
long Csound::GetSoundVol(int num)
{
	DebugText("GetSoundVol(num=>%d)...",num);
	long vol;
	for(int count=0;count<Max;count++)
	{
		if(ManagementNum[count]==num)
		{
			m_pPerformance[count]->GetGlobalParam( GUID_PerfMasterVolume, &vol, sizeof(long) );
			
		}
	}
	DebugText("OK! Vol=>%d",vol);
	return vol;
}

int Csound::GetCheckPlaying(int num)
{
	DebugText("CheckPlaySound(num=>%d)...",num);
	int check;
	for(int count=0;count<Max;count++)
	{
		if(ManagementNum[count]==num)
		{
			check=m_pPerformance[count]->IsPlaying(g_pSegment[count],0);
			if(check==S_OK)
			{
				DebugText("OK!  Played.\n");
				return 1;
			}
			else if(check==S_FALSE)
			{
				DebugText("OK! not Play.\n");
				return 0;
			}
			else
			{
				DebugText("NG!\n");
				return -1;
			}
		}
	}
	return -1;
	//エラー管理番号が見付からない
	DebugText("NG! not found Layer.\n");
}