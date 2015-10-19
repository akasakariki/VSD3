//ファイルパックリーダー
#include "FileReader.h"
#include "Common.h"
// メモリマップドファイル実装部

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

// ファイルオープン
bool CFileReader::Open( char* filename, DWORD rwflag, DWORD openflag)
{
	DebugText("Open File(FilePath:%s)...",filename);
   // ファイルオープン
   m_hFile = CreateFile( filename, rwflag, 0, 0, openflag, 0, 0 );
   if( m_hFile == INVALID_HANDLE_VALUE )
      return false;

   // ファイルマッピングオブジェクトを作成
   DWORD mapflag = PAGE_READWRITE;
   if( rwflag == GENERIC_READ )
   mapflag = PAGE_READONLY; // 読み込み専用に設定
   m_hMap = CreateFileMapping( m_hFile, 0, mapflag, 0, 0, filename );
   if( m_hMap <= 0 ){
      CloseHandle( m_hFile );
      m_hFile = INVALID_HANDLE_VALUE;
      return false;
   }

   // ポインタを取得
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

   // ファイルサイズを取得
   DWORD high;
   m_dwFileSize = ::GetFileSize( m_hFile, &high );
	DebugText("OK!\n");

   return true;
}


// ファイルポインタ取得
bool CFileReader::GetPtr( void** ptr, char* subfilename, DWORD *pfilesize )
{
   *ptr = m_pPointer;
   if(pfilesize != NULL)
      *pfilesize = m_dwFileSize; // 全ファイルサイズを返す
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
					//エラー拡張子が長すぎます。
					DebugText("Error:拡張子が正常に取得できませんでした。(%s)\n",filepath);

				}
			}
			exp[loop2]='\0';
			return exp;
			
	}
	else
	{
		//エラー拡張子がない。
		DebugText("Error:拡張子が指定されていません。(%s)\n",filepath);

	}
	return 0;
}

//ファイル読み込み
//通常（音楽ファイルなど）
//ファイルシーク→ファイル発見→ファイルをメモリに複合化しつつ読み込み開始
//

