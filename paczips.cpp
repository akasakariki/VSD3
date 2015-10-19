#include "paczip.h"
#include <ctype.h>
#include <assert.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include "zlib/zlib.h"              
#include <assert.h>

#pragma comment(lib,"zlib/zlib.lib")



//====================================
//コンストラクタ
//====================================
CompressData::CompressData()
{
	m_dwFileNum = 0;
}

//====================================
//デストラクタ
//====================================
CompressData::~CompressData()
{
	Clear();
}



//====================================
//圧縮します。
//====================================
HRESULT CompressData::Compress(FILE *fin,FILE *fout,DWORD *readByte)
{
	char outbuf[OUTBUFSIZ];
	char inbuf[INBUFSIZ];
	
	//======================
	//Zストリームの初期化
	//======================
	z_stream z;
    z.avail_in = 0;            
    z.next_out = (Bytef *)outbuf;
    z.avail_out = OUTBUFSIZ;
	z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;
    if (deflateInit(&z, Z_DEFAULT_COMPRESSION) != Z_OK) 
	{
		assert(!"圧縮の初期化を失敗しました。");
        return E_FAIL;
    }
  
    int flush = Z_NO_FLUSH;
	*readByte = 0;
	int status = Z_OK;
	int count;
    while (status != Z_STREAM_END) 
	{
		//=========================
		//入力バッファを再確保
		//=========================
        if (z.avail_in == 0)
		{  
            z.next_in = (Bytef *)inbuf;
            z.avail_in = (int)fread(inbuf, 1, INBUFSIZ, fin);

			//======================================
			//入力ストリームが切れたら
			//======================================
            if (z.avail_in < INBUFSIZ) 
			{
				flush = Z_FINISH;
			}
		}

		//==========================
		//圧縮
		//==========================
        status = deflate(&z, flush); 
        if (status == Z_STREAM_END)
		{
			break; 
		}
        if (status != Z_OK) 
		{   
			assert(!"圧縮に失敗しました");
			return E_FAIL;
        }

		//=========================
		//出力バッファを再確保
		//=========================
        if (z.avail_out == 0) 
		{ 
			count = (int)fwrite(outbuf, 1, OUTBUFSIZ, fout);
            if (count != OUTBUFSIZ) 
			{
				assert(!"書き込みバッファがおかしいですよ");
				return E_FAIL;
            }
			*readByte += count;
            z.next_out = (Bytef *)outbuf;
            z.avail_out = OUTBUFSIZ;
        }
    }

	//==================================
	//バッファの残りを書き込み
	//==================================
	count = OUTBUFSIZ - z.avail_out;
    if (count != 0) 
	{
        if (fwrite(outbuf, 1, count, fout) != count) 
		{
			assert(!"書き込み失敗");
			return E_FAIL;
        }
		*readByte += count;
    }

	//===========================
	//開放
	//===========================
    if (deflateEnd(&z) != Z_OK) 
	{
        assert(!"圧縮の開放も失敗しました");
        return E_FAIL;
    }

	return S_OK;
}

//====================================
//解凍します。
//====================================
HRESULT CompressData::Decompress(FILE *fin,FILE *fout)
{
	char outbuf[OUTBUFSIZ];
	char inbuf[INBUFSIZ];

	//========================
	//Zストリームの初期化
	//========================
	z_stream z; 
    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;
    z.next_in = Z_NULL;
    z.avail_in = 0;
    if (inflateInit(&z) != Z_OK) 
	{
		assert(!"解凍の初期化");
        return E_FAIL;
    }

    z.next_out = (Bytef *)outbuf;
    z.avail_out = OUTBUFSIZ;
    int status = Z_OK;
    while (status != Z_STREAM_END) 
	{
		//===============================
		//データの入力バッファを再確保
		//===============================
        if (z.avail_in == 0) 
		{  
            z.next_in =  (Bytef *)inbuf;
            z.avail_in = (int)fread(inbuf, 1, INBUFSIZ, fin); 
        }

		//============================
		//ここで解凍
		//============================
        status = inflate(&z,Z_NO_FLUSH);
        if (status == Z_STREAM_END)
		{
			break;
		}
        if (status != Z_OK) 
		{
			assert(!"解凍を失敗しました");
			return E_FAIL;
        }

		//===============================
		//データの出力バッファを再確保
		//===============================
        if (z.avail_out == 0) 
		{
            if (fwrite(outbuf, 1, OUTBUFSIZ, fout) != OUTBUFSIZ)
			{
				assert(!"書き込み失敗しました");
				return E_FAIL;
            }
            z.next_out =  (Bytef *)outbuf;
            z.avail_out = OUTBUFSIZ;
        }
    }

	//===============================
	//残りのデータを書き込み(アップデート用)
	//===============================
	/*int count = OUTBUFSIZ - z.avail_out;
    if (count != 0) //EOFまで実行
	{
        if (fwrite(outbuf, 1, count, fout) != count) 
		{
			assert(!"データの書き込みを失敗しました");
			return E_FAIL;
        }
    }
	*/

    //===============================
	//解凍の開放
	//===============================
    if (inflateEnd(&z) != Z_OK) 
	{
		assert(!"解凍の開放を失敗しました。");
		return E_FAIL;
    }


	
	return S_OK;
}

//===============================================
//ファイル単位で取得するときに使う
//===============================================
HRESULT CompressData::Decompress(FILE *fin,char *outbuf,DWORD size)
{
	char inbuf[INBUFSIZ];
	

	//===============================
	//Zストリームの初期化
	//===============================
	z_stream z; 
    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;
    z.next_in = Z_NULL;
    z.avail_in = 0;
	if (inflateInit(&z) != Z_OK) 
	{
		assert(!"解凍の初期化に失敗しました。");
		return E_FAIL;
	}
    z.next_out = (Bytef *)outbuf;
    z.avail_out = size;

    int status = Z_OK;
    while (status != Z_STREAM_END) 
	{
		//=========================
		//入力バッファが尽きたら
		//=========================
        if (z.avail_in == 0) 
		{ 
            z.next_in =  (Bytef *)inbuf;
            z.avail_in = (int)fread(inbuf, 1, INBUFSIZ, fin);
		}
        status = inflate(&z, Z_PARTIAL_FLUSH);
        if (status == Z_STREAM_END)
		{
			break;
		}
        if (status != Z_OK) 
		{
			assert(!"解凍を失敗しました。");
			return E_FAIL;
        }

		//=========================
		//出力バッファが尽きたら
		//=========================
        if (z.avail_out == 0) 
		{ 
			assert(!"ファイル出力バッファがつきました。");
			return E_FAIL;
        }
    }
	
	//===============================
	//開放
	//===============================
	if (inflateEnd(&z) != Z_OK) 
	{
		  assert(!"解凍の開放を失敗しました。");
		  return E_FAIL;
	}

	return S_OK;
}



//====================================
//引数のからパックファイルを作成します。
//====================================
HRESULT CompressData::Joint(char *inFileList,char *outFileName)
{
	int iLength = (int)strlen(inFileList);

	if(iLength == 0)
	{
		m_dwFileNum = 0;
		assert(!"ファイルのリストがおかしいです。");
		return E_FAIL;
	}

	//================================
	//トークン分解でファイルの数を取得
	//================================
	char *token[MAX_FILE_NUM];
	m_dwFileNum = 0;
	token[m_dwFileNum] = strtok(inFileList, ",\0" );
    while (token[m_dwFileNum] != NULL ) 
	{
		m_dwFileNum++;
		token[m_dwFileNum] = strtok( NULL,",\0" ); 
	}	
	



	FILE *fout = fopen(outFileName,"wb");
	if(fout == NULL)
	{
		assert(!"なんかおかしいよ");
		return E_FAIL;
	}
	

	
	DWORD headerSize = //ファイルヘッダーの終了位置を計算する 
				sizeof(DWORD) +						//ファイル数
				m_dwFileNum *						//ファイル数
				(
				sizeof(char) * FILE_NAME_LENGTH +	//各ファイルの名前
				sizeof(DWORD) +						//各ファイルの圧縮前の大きさ
				sizeof(DWORD) +						//各ファイルの圧縮後の大きさ
				sizeof(DWORD)						//各ファイルの先頭番地
				);
	//========================================
	//ヘッダーを読み飛ばす
	//本当は先にヘッダーを読みたいけど、圧縮後の
	//データさいずがわからないから・・・；
	//========================================
	fseek(fout,headerSize,SEEK_SET);
	
	
	DWORD compressSize[MAX_FILE_NUM];//圧縮後のデータサイズ

	//================================================
	//ファイルデータを圧縮後コピーします。
	//================================================
	for(DWORD i = 0; i < m_dwFileNum;i++)
	{
		//=================================
		//各ファイルの名前を取得する
		//=================================
		FILE *fin = fopen(token[i],"rb");
		if(Compress(fin,fout,&compressSize[i]) != S_OK)
		{
			return E_FAIL;
		}
		fclose(fin);

		printf("%sの書き込み成功\n",token[i]);
	}
	
	
	fseek(fout,0,SEEK_SET);	//ファイルヘッダーの作成のために戻す

	//===============================
	//ファイルヘッダーの作成
	//===============================
	fwrite(&m_dwFileNum,sizeof(DWORD),1,fout);	//ファイルの数を書き込み

	DWORD startAddress = headerSize;
	for(DWORD i = 0; i < m_dwFileNum;i++)
	{
		//=================================
		////各ファイルの名前を書き込み
		//=================================
		fwrite(token[i],sizeof(char),FILE_NAME_LENGTH,fout);	
		
		
		//=================================
		//各ファイルの圧縮前の大きさを書き込み
		//=================================
		FILE *fin = fopen(token[i],"rb"); 
		if(fin == NULL)
		{
			assert(!"ファイルリストのファイル名がおかしいですよ");
			return E_FAIL;
		}
		DWORD dwFileSize;
		fseek(fin,0,SEEK_END);
		dwFileSize = ftell(fin);
		fclose(fin);
		fwrite(&dwFileSize,sizeof(DWORD),1,fout);
		
		//==================================
		//各ファイルの圧縮後の大きさを書き込む
		//==================================
		fwrite(&compressSize[i],sizeof(DWORD),1,fout);

		//=================================
		//各ファイルの先頭番地を書き込む
		//=================================
		fwrite(&startAddress,sizeof(DWORD),1,fout);

		//=================================
		//先頭番地を移動
		//=================================
		startAddress += compressSize[i];
	}

	fclose(fout);
	puts("書き込み成功");
	return S_OK;
}

//====================================
//ファイルの情報を取得します。
//====================================
HRESULT CompressData::GetFileInfo(char *fileName,FileInfo **fd)
{
	std::map<std::string,FileInfo>::iterator p = m_mapFile.find(std::string(fileName));

	if(p == m_mapFile.end())
	{
		assert(!"そんなファイル格納されていませんよ");
		return E_FAIL;
	}
	*fd = (FileInfo *)&(p->second);
	return S_OK;
}



//====================================
//ディレクトリを作成します。
//====================================
void CompressData::DirectoryName(char *fileName)
{
	//======================
	//./の読み飛ばし
	//======================
	int iThisPoint = 2;
	int iLength = (int)strlen(fileName);
	for(int i = 0; i < iLength; i++)
	{
		if(fileName[i] == '/')
		{
			char buffer[FILE_NAME_LENGTH];
			strcpy(buffer,fileName);
			buffer[i + 1] = '\0';
			mkdir(buffer);
		}
	}
}

//====================================
//ファイルのサイズを取得します。
//====================================
int CompressData::GetFileSize(char *fileName)
{	
	std::map<std::string,FileInfo>::iterator p = m_mapFile.find(std::string(fileName));

	if(p == m_mapFile.end())
	{
		assert(!"そんなファイル格納されていませんよ");
		return E_FAIL;
	}
	return p->second.size;
}
//==========================================
//ヘッダーファイルの読み込み
//==========================================
void CompressData::ReadHeader(FILE *fp)
{
	fread(&m_dwFileNum,sizeof(DWORD),1,fp);					//ファイルの個数を読み込む

	char fileName[FILE_NAME_LENGTH];
	FileInfo fd;
	for(DWORD i = 0; i < m_dwFileNum; i++)
	{
		fread(fileName,sizeof(char),FILE_NAME_LENGTH,fp);	//ファイルの名前を読み込む
		fread(&fd.size,sizeof(DWORD),1,fp);					//ファイルのサイズを読み込む
		fread(&fd.compressSize,sizeof(DWORD),1,fp);			//ファイルの圧縮後のサイズを読み込む
		fread(&fd.startAdress,sizeof(DWORD),1,fp);			//ファイルの開始アドレスを読み込む
		m_mapFile.insert(std::pair<std::string,FileInfo>(std::string(fileName),fd));
	}
}

//====================================
//読み込むだけですよ。
//====================================
HRESULT CompressData::Read(char *fileName)
{
	Clear();
	strcpy(m_fileName,fileName);
	FILE *fp = fopen(m_fileName,"rb");
	if(fp == NULL)
	{
		return E_FAIL;
	}
	ReadHeader(fp);
	fclose(fp);
	return S_OK;
}

//=================================
//ファイルを分割します。
//=================================
HRESULT CompressData::Split(char *inFileName)
{
	Clear();
	strcpy(m_fileName,inFileName);
	FILE *fin = fopen(m_fileName,"rb");
	if(fin == NULL)
	{
		assert(!"分割する元のファイルがないですよ");
		return E_FAIL;
	}

	//============================
	//ファイルヘッダーの読み込み
	//============================
	ReadHeader(fin);	
	puts("結合ファイルの読み込み成功");
	
	std::map<std::string,FileInfo>::iterator p = m_mapFile.begin();
	for(DWORD i = 0; i < m_dwFileNum; i++)
	{
		FileInfo &fd = p->second;
		char *fileName = (char *)p->first.c_str();
		DirectoryName(fileName);	//ファイルを作成する前にディレクトリの作成
		FILE *fout = fopen(fileName,"wb");
		if(fout == NULL)
		{
			assert(!"引数の設定がおかしいです");
			return E_FAIL;
		}
		fseek(fin,fd.startAdress,SEEK_SET);
		
		
		Decompress(fin,fout);
		
		fclose(fout);

		p++;
		printf("%sの書き込み成功\n",fileName);
	}

	puts("書き込み成功");
	fclose(fin);

	return S_OK;
}











//=====================================
//クラスの情報をクリアします。
//=====================================
void CompressData::Clear()
{
	m_mapFile.clear();
	m_dwFileNum = 0;
	m_fileName[0] = '\0';
}
//================================================================================
//ファイルデータを取得します。ただしoutDataはメモリが確保されてなくてはなりません
//================================================================================
HRESULT CompressData::GetFileData(char *fileName,char *outData)
{	
	FileInfo *fi;
	if(GetFileInfo(fileName,&fi) == E_FAIL)
	{
		assert(!"ファイルがないですよ");
		return E_FAIL;
	}
	

	FILE *fp = fopen(m_fileName,"rb");
	fseek(fp,fi->startAdress,SEEK_SET);
	Decompress(fp,outData,fi->size);

	fclose(fp);
	
	return S_OK;
}






