//�t�@�C���p�b�N���[�_�[
#include "FileReader.h"
#include "Common.h"
// �������}�b�v�h�t�@�C��������

CFileReader::CFileReader()
{
   m_hFile = INVALID_HANDLE_VALUE;
   m_hMap = 0;
   m_pPointer = NULL;
   m_dwFileSize = 0;
}


CFileReader::~CFileReader()
{
   UnmapViewOfFile( m_pPointer );
   if( m_hMap != 0 )
   CloseHandle( m_hMap );
   if( m_hFile != INVALID_HANDLE_VALUE )
   CloseHandle( m_hFile );
}

// �t�@�C���I�[�v��
bool CFileReader::Open( char* filename, DWORD rwflag, DWORD openflag)
{
	DebugText("Open File(FilePath:%s)...",filename);
   // �t�@�C���I�[�v��
   m_hFile = CreateFile( filename, rwflag, 0, 0, openflag, 0, 0 );
   if( m_hFile == INVALID_HANDLE_VALUE )
      return false;

   // �t�@�C���}�b�s���O�I�u�W�F�N�g���쐬
   DWORD mapflag = PAGE_READWRITE;
   if( rwflag == GENERIC_READ )
   mapflag = PAGE_READONLY; // �ǂݍ��ݐ�p�ɐݒ�
   m_hMap = CreateFileMapping( m_hFile, 0, mapflag, 0, 0, filename );
   if( m_hMap <= 0 ){
      CloseHandle( m_hFile );
      m_hFile = INVALID_HANDLE_VALUE;
      return false;
   }

   // �|�C���^���擾
   DWORD mapviewflag = FILE_MAP_WRITE;
   if( mapflag == PAGE_READONLY )
      mapviewflag = FILE_MAP_READ;
   m_pPointer = (char*)MapViewOfFile( m_hMap, mapviewflag, 0, 0, 0);
   if( m_pPointer == NULL){
      CloseHandle( m_hMap );
      CloseHandle( m_hFile );
      m_hMap = 0;
      m_hFile = INVALID_HANDLE_VALUE;
   return false;
   }

   // �t�@�C���T�C�Y���擾
   DWORD high;
   m_dwFileSize = ::GetFileSize( m_hFile, &high );
	DebugText("OK!\n");

   return true;
}


// �t�@�C���|�C���^�擾
bool CFileReader::GetPtr( void** ptr, char* subfilename, DWORD *pfilesize )
{
   *ptr = m_pPointer;
   if(pfilesize != NULL)
      *pfilesize = m_dwFileSize; // �S�t�@�C���T�C�Y��Ԃ�
   return true;
}

char *CFileReader::GetExtname(char filepath[255])
{
	int loopi=0;
	int loop2=0;


	for(;filepath[loopi]!='.';loopi++)
	{
		if(loopi>=255)break;
			
	}
	loopi++;
	if(loopi<255)
	{
			for(;filepath[loopi]!='\0';loopi++)
			{
				exp[loop2]=filepath[loopi];
				loop2++;
				if(loop2>=6)
				{
					//�G���[�g���q���������܂��B
					DebugText("Error:�g���q������Ɏ擾�ł��܂���ł����B(%s)\n",filepath);

				}
			}
			exp[loop2]='\0';
			return exp;
			
	}
	else
	{
		//�G���[�g���q���Ȃ��B
		DebugText("Error:�g���q���w�肳��Ă��܂���B(%s)\n",filepath);

	}
	return 0;
}

//�t�@�C���ǂݍ���
//�ʏ�i���y�t�@�C���Ȃǁj
//�t�@�C���V�[�N���t�@�C���������t�@�C�����������ɕ��������ǂݍ��݊J�n
//

