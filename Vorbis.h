#pragma once
#include "vorbis-1.2.0\include\vorbis\vorbisfile.h"
#include "vorbis-1.2.0\include\vorbis\codec.h"
#include "dx9sdk\Include\dmusici.h"

//あくまで、サウンド関係はDirectSoundををベースにしたいので、
//ここのVorbisプログラムはDirectSoundでも使えるように変換するようにします。
#include <windows.h>
//PCM for Waveフォーマット構造体
typedef struct{
  char         cRIFF[4];
  int          iSizeRIFF;
  char         cType[4];
  char         cFmt[4];
  int          iSizeFmt;
  WAVEFORMATEX WaveFmt;
  char         cData[4];
  int          iSizeData;
} WAVEFILEHEADER;

//注釈（１１．３０）暗号解読http://marupeke296.com/OGG_No5_PlayOggInMemory.htmlを参考に
class Cvorbis
{
private:
	OggVorbis_File vf;
	vorbis_info *vi;//情報格納
	WAVEFILEHEADER wh;
	DMUS_OBJECTDESC desc;
	LONG lReadSize;
	LONG lWriteSize;
	LONG lWHSize;
	CHAR* lpWaveData;
public:
	int Load_ogg(char FileName[]);
	DMUS_OBJECTDESC Return_Desc(void);


};


