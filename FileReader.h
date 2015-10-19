#pragma once

#include <windows.h>

class CFileReader
{
   protected:
   HANDLE m_hFile;
   HANDLE m_hMap;
   void* m_pPointer;
   DWORD m_dwFileSize;
	char exp[5];
   public:
   CFileReader();
   virtual ~CFileReader();


   // �t�@�C���I�[�v��
   virtual bool Open( char* filename, DWORD rwflag=GENERIC_READ | GENERIC_WRITE, DWORD openflag=OPEN_EXISTING);

   // �t�@�C���|�C���^�擾
   virtual bool GetPtr( void** ptr , char* subfilename=NULL, DWORD *pfilesize=NULL );
   virtual char *GetExtname(char filepath[255]);//�t�@�C���g���q����
};

