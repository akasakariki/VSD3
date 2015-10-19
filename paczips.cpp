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
//�R���X�g���N�^
//====================================
CompressData::CompressData()
{
	m_dwFileNum = 0;
}

//====================================
//�f�X�g���N�^
//====================================
CompressData::~CompressData()
{
	Clear();
}



//====================================
//���k���܂��B
//====================================
HRESULT CompressData::Compress(FILE *fin,FILE *fout,DWORD *readByte)
{
	char outbuf[OUTBUFSIZ];
	char inbuf[INBUFSIZ];
	
	//======================
	//Z�X�g���[���̏�����
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
		assert(!"���k�̏����������s���܂����B");
        return E_FAIL;
    }
  
    int flush = Z_NO_FLUSH;
	*readByte = 0;
	int status = Z_OK;
	int count;
    while (status != Z_STREAM_END) 
	{
		//=========================
		//���̓o�b�t�@���Ċm��
		//=========================
        if (z.avail_in == 0)
		{  
            z.next_in = (Bytef *)inbuf;
            z.avail_in = (int)fread(inbuf, 1, INBUFSIZ, fin);

			//======================================
			//���̓X�g���[�����؂ꂽ��
			//======================================
            if (z.avail_in < INBUFSIZ) 
			{
				flush = Z_FINISH;
			}
		}

		//==========================
		//���k
		//==========================
        status = deflate(&z, flush); 
        if (status == Z_STREAM_END)
		{
			break; 
		}
        if (status != Z_OK) 
		{   
			assert(!"���k�Ɏ��s���܂���");
			return E_FAIL;
        }

		//=========================
		//�o�̓o�b�t�@���Ċm��
		//=========================
        if (z.avail_out == 0) 
		{ 
			count = (int)fwrite(outbuf, 1, OUTBUFSIZ, fout);
            if (count != OUTBUFSIZ) 
			{
				assert(!"�������݃o�b�t�@�����������ł���");
				return E_FAIL;
            }
			*readByte += count;
            z.next_out = (Bytef *)outbuf;
            z.avail_out = OUTBUFSIZ;
        }
    }

	//==================================
	//�o�b�t�@�̎c�����������
	//==================================
	count = OUTBUFSIZ - z.avail_out;
    if (count != 0) 
	{
        if (fwrite(outbuf, 1, count, fout) != count) 
		{
			assert(!"�������ݎ��s");
			return E_FAIL;
        }
		*readByte += count;
    }

	//===========================
	//�J��
	//===========================
    if (deflateEnd(&z) != Z_OK) 
	{
        assert(!"���k�̊J�������s���܂���");
        return E_FAIL;
    }

	return S_OK;
}

//====================================
//�𓀂��܂��B
//====================================
HRESULT CompressData::Decompress(FILE *fin,FILE *fout)
{
	char outbuf[OUTBUFSIZ];
	char inbuf[INBUFSIZ];

	//========================
	//Z�X�g���[���̏�����
	//========================
	z_stream z; 
    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;
    z.next_in = Z_NULL;
    z.avail_in = 0;
    if (inflateInit(&z) != Z_OK) 
	{
		assert(!"�𓀂̏�����");
        return E_FAIL;
    }

    z.next_out = (Bytef *)outbuf;
    z.avail_out = OUTBUFSIZ;
    int status = Z_OK;
    while (status != Z_STREAM_END) 
	{
		//===============================
		//�f�[�^�̓��̓o�b�t�@���Ċm��
		//===============================
        if (z.avail_in == 0) 
		{  
            z.next_in =  (Bytef *)inbuf;
            z.avail_in = (int)fread(inbuf, 1, INBUFSIZ, fin); 
        }

		//============================
		//�����ŉ�
		//============================
        status = inflate(&z,Z_NO_FLUSH);
        if (status == Z_STREAM_END)
		{
			break;
		}
        if (status != Z_OK) 
		{
			assert(!"�𓀂����s���܂���");
			return E_FAIL;
        }

		//===============================
		//�f�[�^�̏o�̓o�b�t�@���Ċm��
		//===============================
        if (z.avail_out == 0) 
		{
            if (fwrite(outbuf, 1, OUTBUFSIZ, fout) != OUTBUFSIZ)
			{
				assert(!"�������ݎ��s���܂���");
				return E_FAIL;
            }
            z.next_out =  (Bytef *)outbuf;
            z.avail_out = OUTBUFSIZ;
        }
    }

	//===============================
	//�c��̃f�[�^����������(�A�b�v�f�[�g�p)
	//===============================
	/*int count = OUTBUFSIZ - z.avail_out;
    if (count != 0) //EOF�܂Ŏ��s
	{
        if (fwrite(outbuf, 1, count, fout) != count) 
		{
			assert(!"�f�[�^�̏������݂����s���܂���");
			return E_FAIL;
        }
    }
	*/

    //===============================
	//�𓀂̊J��
	//===============================
    if (inflateEnd(&z) != Z_OK) 
	{
		assert(!"�𓀂̊J�������s���܂����B");
		return E_FAIL;
    }


	
	return S_OK;
}

//===============================================
//�t�@�C���P�ʂŎ擾����Ƃ��Ɏg��
//===============================================
HRESULT CompressData::Decompress(FILE *fin,char *outbuf,DWORD size)
{
	char inbuf[INBUFSIZ];
	

	//===============================
	//Z�X�g���[���̏�����
	//===============================
	z_stream z; 
    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;
    z.next_in = Z_NULL;
    z.avail_in = 0;
	if (inflateInit(&z) != Z_OK) 
	{
		assert(!"�𓀂̏������Ɏ��s���܂����B");
		return E_FAIL;
	}
    z.next_out = (Bytef *)outbuf;
    z.avail_out = size;

    int status = Z_OK;
    while (status != Z_STREAM_END) 
	{
		//=========================
		//���̓o�b�t�@���s������
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
			assert(!"�𓀂����s���܂����B");
			return E_FAIL;
        }

		//=========================
		//�o�̓o�b�t�@���s������
		//=========================
        if (z.avail_out == 0) 
		{ 
			assert(!"�t�@�C���o�̓o�b�t�@�����܂����B");
			return E_FAIL;
        }
    }
	
	//===============================
	//�J��
	//===============================
	if (inflateEnd(&z) != Z_OK) 
	{
		  assert(!"�𓀂̊J�������s���܂����B");
		  return E_FAIL;
	}

	return S_OK;
}



//====================================
//�����̂���p�b�N�t�@�C�����쐬���܂��B
//====================================
HRESULT CompressData::Joint(char *inFileList,char *outFileName)
{
	int iLength = (int)strlen(inFileList);

	if(iLength == 0)
	{
		m_dwFileNum = 0;
		assert(!"�t�@�C���̃��X�g�����������ł��B");
		return E_FAIL;
	}

	//================================
	//�g�[�N�������Ńt�@�C���̐����擾
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
		assert(!"�Ȃ񂩂���������");
		return E_FAIL;
	}
	

	
	DWORD headerSize = //�t�@�C���w�b�_�[�̏I���ʒu���v�Z���� 
				sizeof(DWORD) +						//�t�@�C����
				m_dwFileNum *						//�t�@�C����
				(
				sizeof(char) * FILE_NAME_LENGTH +	//�e�t�@�C���̖��O
				sizeof(DWORD) +						//�e�t�@�C���̈��k�O�̑傫��
				sizeof(DWORD) +						//�e�t�@�C���̈��k��̑傫��
				sizeof(DWORD)						//�e�t�@�C���̐擪�Ԓn
				);
	//========================================
	//�w�b�_�[��ǂݔ�΂�
	//�{���͐�Ƀw�b�_�[��ǂ݂������ǁA���k���
	//�f�[�^���������킩��Ȃ�����E�E�E�G
	//========================================
	fseek(fout,headerSize,SEEK_SET);
	
	
	DWORD compressSize[MAX_FILE_NUM];//���k��̃f�[�^�T�C�Y

	//================================================
	//�t�@�C���f�[�^�����k��R�s�[���܂��B
	//================================================
	for(DWORD i = 0; i < m_dwFileNum;i++)
	{
		//=================================
		//�e�t�@�C���̖��O���擾����
		//=================================
		FILE *fin = fopen(token[i],"rb");
		if(Compress(fin,fout,&compressSize[i]) != S_OK)
		{
			return E_FAIL;
		}
		fclose(fin);

		printf("%s�̏������ݐ���\n",token[i]);
	}
	
	
	fseek(fout,0,SEEK_SET);	//�t�@�C���w�b�_�[�̍쐬�̂��߂ɖ߂�

	//===============================
	//�t�@�C���w�b�_�[�̍쐬
	//===============================
	fwrite(&m_dwFileNum,sizeof(DWORD),1,fout);	//�t�@�C���̐�����������

	DWORD startAddress = headerSize;
	for(DWORD i = 0; i < m_dwFileNum;i++)
	{
		//=================================
		////�e�t�@�C���̖��O����������
		//=================================
		fwrite(token[i],sizeof(char),FILE_NAME_LENGTH,fout);	
		
		
		//=================================
		//�e�t�@�C���̈��k�O�̑傫������������
		//=================================
		FILE *fin = fopen(token[i],"rb"); 
		if(fin == NULL)
		{
			assert(!"�t�@�C�����X�g�̃t�@�C���������������ł���");
			return E_FAIL;
		}
		DWORD dwFileSize;
		fseek(fin,0,SEEK_END);
		dwFileSize = ftell(fin);
		fclose(fin);
		fwrite(&dwFileSize,sizeof(DWORD),1,fout);
		
		//==================================
		//�e�t�@�C���̈��k��̑傫������������
		//==================================
		fwrite(&compressSize[i],sizeof(DWORD),1,fout);

		//=================================
		//�e�t�@�C���̐擪�Ԓn����������
		//=================================
		fwrite(&startAddress,sizeof(DWORD),1,fout);

		//=================================
		//�擪�Ԓn���ړ�
		//=================================
		startAddress += compressSize[i];
	}

	fclose(fout);
	puts("�������ݐ���");
	return S_OK;
}

//====================================
//�t�@�C���̏����擾���܂��B
//====================================
HRESULT CompressData::GetFileInfo(char *fileName,FileInfo **fd)
{
	std::map<std::string,FileInfo>::iterator p = m_mapFile.find(std::string(fileName));

	if(p == m_mapFile.end())
	{
		assert(!"����ȃt�@�C���i�[����Ă��܂����");
		return E_FAIL;
	}
	*fd = (FileInfo *)&(p->second);
	return S_OK;
}



//====================================
//�f�B���N�g�����쐬���܂��B
//====================================
void CompressData::DirectoryName(char *fileName)
{
	//======================
	//./�̓ǂݔ�΂�
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
//�t�@�C���̃T�C�Y���擾���܂��B
//====================================
int CompressData::GetFileSize(char *fileName)
{	
	std::map<std::string,FileInfo>::iterator p = m_mapFile.find(std::string(fileName));

	if(p == m_mapFile.end())
	{
		assert(!"����ȃt�@�C���i�[����Ă��܂����");
		return E_FAIL;
	}
	return p->second.size;
}
//==========================================
//�w�b�_�[�t�@�C���̓ǂݍ���
//==========================================
void CompressData::ReadHeader(FILE *fp)
{
	fread(&m_dwFileNum,sizeof(DWORD),1,fp);					//�t�@�C���̌���ǂݍ���

	char fileName[FILE_NAME_LENGTH];
	FileInfo fd;
	for(DWORD i = 0; i < m_dwFileNum; i++)
	{
		fread(fileName,sizeof(char),FILE_NAME_LENGTH,fp);	//�t�@�C���̖��O��ǂݍ���
		fread(&fd.size,sizeof(DWORD),1,fp);					//�t�@�C���̃T�C�Y��ǂݍ���
		fread(&fd.compressSize,sizeof(DWORD),1,fp);			//�t�@�C���̈��k��̃T�C�Y��ǂݍ���
		fread(&fd.startAdress,sizeof(DWORD),1,fp);			//�t�@�C���̊J�n�A�h���X��ǂݍ���
		m_mapFile.insert(std::pair<std::string,FileInfo>(std::string(fileName),fd));
	}
}

//====================================
//�ǂݍ��ނ����ł���B
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
//�t�@�C���𕪊����܂��B
//=================================
HRESULT CompressData::Split(char *inFileName)
{
	Clear();
	strcpy(m_fileName,inFileName);
	FILE *fin = fopen(m_fileName,"rb");
	if(fin == NULL)
	{
		assert(!"�������錳�̃t�@�C�����Ȃ��ł���");
		return E_FAIL;
	}

	//============================
	//�t�@�C���w�b�_�[�̓ǂݍ���
	//============================
	ReadHeader(fin);	
	puts("�����t�@�C���̓ǂݍ��ݐ���");
	
	std::map<std::string,FileInfo>::iterator p = m_mapFile.begin();
	for(DWORD i = 0; i < m_dwFileNum; i++)
	{
		FileInfo &fd = p->second;
		char *fileName = (char *)p->first.c_str();
		DirectoryName(fileName);	//�t�@�C�����쐬����O�Ƀf�B���N�g���̍쐬
		FILE *fout = fopen(fileName,"wb");
		if(fout == NULL)
		{
			assert(!"�����̐ݒ肪���������ł�");
			return E_FAIL;
		}
		fseek(fin,fd.startAdress,SEEK_SET);
		
		
		Decompress(fin,fout);
		
		fclose(fout);

		p++;
		printf("%s�̏������ݐ���\n",fileName);
	}

	puts("�������ݐ���");
	fclose(fin);

	return S_OK;
}











//=====================================
//�N���X�̏����N���A���܂��B
//=====================================
void CompressData::Clear()
{
	m_mapFile.clear();
	m_dwFileNum = 0;
	m_fileName[0] = '\0';
}
//================================================================================
//�t�@�C���f�[�^���擾���܂��B������outData�̓��������m�ۂ���ĂȂ��Ă͂Ȃ�܂���
//================================================================================
HRESULT CompressData::GetFileData(char *fileName,char *outData)
{	
	FileInfo *fi;
	if(GetFileInfo(fileName,&fi) == E_FAIL)
	{
		assert(!"�t�@�C�����Ȃ��ł���");
		return E_FAIL;
	}
	

	FILE *fp = fopen(m_fileName,"rb");
	fseek(fp,fi->startAdress,SEEK_SET);
	Decompress(fp,outData,fi->size);

	fclose(fp);
	
	return S_OK;
}






