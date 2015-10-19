#pragma once



#include <windows.h>
#include <string>
#include <map>
#include <stdio.h>



const int FILE_NAME_LENGTH = 256;		//�t�@�C���̖��O��
const int FILE_LIST_LENGTH = 100000;	//�t�@�C���̈����̒���
const int COPY_DATA_SIZE = 1024;		//�R�s�[����Ƃ��̃o�b�t�@�̃T�C�Y
const int MAX_FILE_NUM = 10000;			//�g�[�N����������Ƃ��ɕK�v

const int OUTBUFSIZ = 1024;
const int INBUFSIZ = 1024;



//============================
//�t�@�C���̏��
//============================
struct FileInfo
{
	DWORD size;			//�t�@�C���̈��k�O�̑傫��
	DWORD compressSize;	//�t�@�C���̈��k��̑傫��
	DWORD startAdress;	//�t�@�C���̊J�n�A�h���X
};

//============================
//�t�@�C���̏��������N���X
//============================
class CompressData
{
public:
	CompressData();
	~CompressData();
private:
	std::map<std::string,FileInfo>	m_mapFile;	//�t�@�C���̏�񂪊i�[����Ă��܂��B
	DWORD m_dwFileNum;
	char m_fileName[FILE_NAME_LENGTH];	//�o�͂���t�@�C����

	HRESULT Compress(FILE *fin,FILE *fout,DWORD *readByte);
	HRESULT Decompress(FILE *fin,FILE *fout);
	HRESULT Decompress(FILE *fin,char *outbuf,DWORD size);

	void ReadHeader(FILE *fp);
	HRESULT GetFileInfo(char *fileName,FileInfo **fd);
	void DirectoryName(char *fileName);
public:

	void Clear();
	HRESULT Read(char *fileName);	//�t�@�C���w�b�_�[��ǂݍ��݂܂��B
	HRESULT Split(char *fileName);	//�w�肵���t�@�C���𕪊����܂��B
	HRESULT Joint(char *fileName);	//�w�肵���t�@�C������t�@�C���̃��X�g���擾���܂��B
	HRESULT Joint(char *inFileList,char *outFileName);	//�����Ƀt�@�C���̖��O���擾�����܂��BinFileName = "abc.bmp,hoge.wav,unko.dat"
	int GetFileSize(char *fileName);	//�t�@�C���̏����擾���܂��B
	HRESULT GetFileData(char *fileName,char *outData);	//�����Ƀt�@�C���X�g���[�����i�[���܂��B
};



#include <string>
#include <stack>

//=======================================
//Joint�����ł͑�ςȂ̂�
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
	HRESULT JointFromFile(char *fileName);	//�w�肵���t�@�C������t�@�C���̃��X�g���擾���܂��B

};



