#pragma once
#include "vorbis-1.2.0\include\vorbis\vorbisfile.h"
#include "vorbis-1.2.0\include\vorbis\codec.h"
#include "dx9sdk\Include\dmusici.h"

//�����܂ŁA�T�E���h�֌W��DirectSound�����x�[�X�ɂ������̂ŁA
//������Vorbis�v���O������DirectSound�ł��g����悤�ɕϊ�����悤�ɂ��܂��B
#include <windows.h>
//PCM for Wave�t�H�[�}�b�g�\����
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

//���߁i�P�P�D�R�O�j�Í����http://marupeke296.com/OGG_No5_PlayOggInMemory.html���Q�l��
class Cvorbis
{
private:
	OggVorbis_File vf;
	vorbis_info *vi;//���i�[
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


