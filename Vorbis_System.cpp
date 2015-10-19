#pragma comment ( lib, "lib/ogg_static.lib" )
#pragma comment ( lib, "lib/vorbis_static.lib" )
#pragma comment ( lib, "lib/vorbisfile_static.lib" )
#pragma warning (disable:4996)
#include "Vorbis.h"
#include <math.h>
#include "Common.h"

int Cvorbis::Load_ogg(char szName[])
{
	DebugText("Open the 'OGG' File(FILE=>NAME:%s)...",szName);
	// ̧�ق��J��
	FILE *lpFile = fopen( szName, "rb" );
	if( lpFile == NULL )  return -1;

	// OggVorbis�̃t�@�C�������擾
	if( ov_open( lpFile, &vf, NULL, 0 ) < 0 )
	{
	  fclose( lpFile );
	  return -1;
	}

	// OggVorbis�̏����擾
	vi = ov_info( &vf, -1 );
	if( vi == NULL )
	{
	  ov_clear( &vf );
	  return -1;
	}
	DebugText("...");
	//�T�C�Y�̌v�Z
	lWHSize = sizeof(wh.cRIFF)    + sizeof(wh.iSizeRIFF) +
				   sizeof(wh.cType)    + sizeof(wh.cFmt)      +
				   sizeof(wh.iSizeFmt) + sizeof(wh.WaveFmt)   +
				   sizeof(wh.cData)    + sizeof(wh.iSizeData);


	INT iWord = 2;  // �ʎq���o�C�g��(1 or 2)

	LONG  lDataSize = (LONG)ceil( 
	  vi->channels * vi->rate * ov_time_total( &vf,-1 ) * iWord 
	);

	lpWaveData = new CHAR[ lDataSize + lWHSize ];
	if( lpWaveData == NULL )
	{
	  ov_clear( &vf );
	  return -1;
	}
	INT  iCurrentSection;
	
	lReadSize = 0;
	while( 1 )
	{
	  // �޺���
	  lWriteSize = ov_read( &vf, lpWaveData + lReadSize + lWHSize,
							lDataSize - lReadSize, 0, iWord, 1,
							&iCurrentSection );
		DebugText("...");
	  // �޺��ޏI��
	  if( ! lWriteSize )
	  {
		break;
	  }
	 
	  // �װ
	  else if ( lWriteSize < 0 )
	  {
		/* #define SAFE_RELEASE(p) if(p){p->Release();p=NULL;} */
		SAFE_DELETE( lpWaveData );
		ov_clear( &vf );
		DebugText("FAILD\n");
		return FALSE;
	  }

	  // �S���޺��ނł��Ȃ�����
	  else
	  {
		lReadSize += lWriteSize;
	  }
	}

	
	// �w�b�_�̏�����
	memcpy( wh.cRIFF, "RIFF", 4 );
	wh.iSizeRIFF = lWHSize + lReadSize - 8;
	memcpy( wh.cType, "WAVE", 4 );
	memcpy( wh.cFmt, "fmt ", 4 );
	wh.iSizeFmt = sizeof(WAVEFORMATEX);
	wh.WaveFmt.cbSize          = sizeof(WAVEFORMATEX);
	wh.WaveFmt.wFormatTag      = WAVE_FORMAT_PCM;
	wh.WaveFmt.nChannels       = vi->channels;
	wh.WaveFmt.nSamplesPerSec  = vi->rate;
	wh.WaveFmt.nAvgBytesPerSec = vi->rate * vi->channels * iWord;
	wh.WaveFmt.nBlockAlign     = vi->channels * iWord;
	wh.WaveFmt.wBitsPerSample  = iWord * 8;
	memcpy( wh.cData, "data", 4 );
	wh.iSizeData = lReadSize;

	// ��������̃w�b�_�̏�������
	int iSize = 0;
	memcpy((char*)*lpWaveData,         &wh.cRIFF,     sizeof(wh.cRIFF));
	iSize += sizeof(wh.cRIFF);
	memcpy((char*)*lpWaveData + iSize, &wh.iSizeRIFF, sizeof(wh.iSizeRIFF));
	iSize += sizeof(wh.iSizeRIFF);
	memcpy((char*)*lpWaveData + iSize, &wh.cType,     sizeof(wh.cType));
	iSize += sizeof(wh.cType);
	memcpy((char*)*lpWaveData + iSize, &wh.cFmt,      sizeof(wh.cFmt));
	iSize += sizeof(wh.cFmt);
	memcpy((char*)*lpWaveData + iSize, &wh.iSizeFmt,  sizeof(wh.iSizeFmt));
	iSize += sizeof(wh.iSizeFmt);
	memcpy((char*)*lpWaveData + iSize, &wh.WaveFmt,   sizeof(wh.WaveFmt));
	iSize += sizeof(wh.WaveFmt);
	memcpy((char*)*lpWaveData + iSize, &wh.cData,     sizeof(wh.cData));
	iSize += sizeof(wh.cData);
	memcpy((char*)*lpWaveData + iSize, &wh.iSizeData, sizeof(wh.iSizeData));

   ov_clear( &vf );
	DebugText("OK!\n");
	return 0;
}



DMUS_OBJECTDESC Cvorbis::Return_Desc(void)
{
	// �ϊ�����Wave�̃T�C�Y
	LONG lWaveSize = lReadSize + lWHSize;

	// �p�����[�^��ݒ�
	DMUS_OBJECTDESC desc;
	ZeroMemory( &desc, sizeof(DMUS_OBJECTDESC) );
	desc.dwSize      = sizeof(DMUS_OBJECTDESC);
	desc.dwValidData = DMUS_OBJ_CLASS |     // guidClass�L��
					   DMUS_OBJ_MEMORY;     // ������������Z�O�����g�쐬
	desc.guidClass   = CLSID_DirectMusicSegment;
	desc.pbMemData   = (LPBYTE)lpWaveData;  // ���������̃f�[�^�ւ̃|�C���^
	desc.llMemLength = lWaveSize;           // ���������̃f�[�^�̃T�C�Y


	return desc;
}