#pragma once



#include <windows.h>
#include <string>
#include <map>
#include <stdio.h>



const int FILE_NAME_LENGTH = 256;		//ファイルの名前長
const int FILE_LIST_LENGTH = 100000;	//ファイルの引数の長さ
const int COPY_DATA_SIZE = 1024;		//コピーするときのバッファのサイズ
const int MAX_FILE_NUM = 10000;			//トークン分解するときに必要

const int OUTBUFSIZ = 1024;
const int INBUFSIZ = 1024;



//============================
//ファイルの情報
//============================
struct FileInfo
{
	DWORD size;			//ファイルの圧縮前の大きさ
	DWORD compressSize;	//ファイルの圧縮後の大きさ
	DWORD startAdress;	//ファイルの開始アドレス
};

//============================
//ファイルの情報を扱うクラス
//============================
class CompressData
{
public:
	CompressData();
	~CompressData();
private:
	std::map<std::string,FileInfo>	m_mapFile;	//ファイルの情報が格納されています。
	DWORD m_dwFileNum;
	char m_fileName[FILE_NAME_LENGTH];	//出力するファイル名

	HRESULT Compress(FILE *fin,FILE *fout,DWORD *readByte);
	HRESULT Decompress(FILE *fin,FILE *fout);
	HRESULT Decompress(FILE *fin,char *outbuf,DWORD size);

	void ReadHeader(FILE *fp);
	HRESULT GetFileInfo(char *fileName,FileInfo **fd);
	void DirectoryName(char *fileName);
public:

	void Clear();
	HRESULT Read(char *fileName);	//ファイルヘッダーを読み込みます。
	HRESULT Split(char *fileName);	//指定したファイルを分割します。
	HRESULT Joint(char *fileName);	//指定したファイルからファイルのリストを取得します。
	HRESULT Joint(char *inFileList,char *outFileName);	//引数にファイルの名前を取得させます。inFileName = "abc.bmp,hoge.wav,unko.dat"
	int GetFileSize(char *fileName);	//ファイルの情報を取得します。
	HRESULT GetFileData(char *fileName,char *outData);	//引数にファイルストリームを格納します。
};



#include <string>
#include <stack>

//=======================================
//Jointだけでは大変なので
//=======================================
class MyCompressData:public CompressData
{
public:
	MyCompressData();
	~MyCompressData();
private:

	void AnalysisWildCard(const char *inWildCard,char *outFileList,std::stack<std::string>& ioStack);
	void MakeFileList(char *inFileName,char *outFileList);
	bool IsWildCard(char *inFileName);
	void RejectLastNL(char *ioBuffer);
public:
	HRESULT JointFromFile(char *fileName);	//指定したファイルからファイルのリストを取得します。

};



