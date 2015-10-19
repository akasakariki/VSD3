#include "paczip.h"


#include <assert.h>
//==========================================
//コンストラクタ
//==========================================
MyCompressData::MyCompressData()
{


}


//=========================================
//デストラクタ
//=========================================
MyCompressData::~MyCompressData()
{



}






//=================================
//fgetsだと改行が入ってしまって変になるので除去
//=================================
void MyCompressData::RejectLastNL(char *ioBuffer)
{
	int iLength = (int)strlen(ioBuffer);
	if(ioBuffer[iLength - 1] == '\n')
	{
		ioBuffer[iLength - 1] = '\0';
	}
}

//=============================================
//ワイルドカードかを取得
//=============================================
bool MyCompressData::IsWildCard(char *inFileName)
{
	for(int i = 0; inFileName[i] != '\0'; i++)
	{
		if(inFileName[i] == '?' || inFileName[i] == '*')
		{
			return 1;
		}
	}
	return 0;
}



void MyCompressData::AnalysisWildCard(const char *inWildCard,char *outFileList,std::stack<std::string>& ioStack)
{
	bool theStartFlag = 1;
	if(outFileList[0] != '\0')
	{
		theStartFlag = 0;
	}
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	//================================
	//ワイルドカード使用時
	//================================
	hFind = FindFirstFile(inWildCard, &fd);		
		
	//検索失敗
	if(hFind == INVALID_HANDLE_VALUE) 
	{
		return;
	}
			
	//===================================
	//ディレクトリの名前取得
	//===================================
	char theDirName[256];
	strcpy(theDirName,inWildCard);
	for(int i = (int)strlen(theDirName); i >= 0; i--)
	{
		if(theDirName[i] == '/')
		{
			theDirName[i + 1] = '\0';
			break;
		}
	}

	
	char theBuffer[256];
	do 
	{
		if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			if(theStartFlag == 0)
			{
				strcat(outFileList,",");
			}
			strcat(outFileList,theDirName);
			strcat(outFileList,fd.cFileName);
			theStartFlag = 0;
		}
		else if(fd.cFileName[0] != '.')
		{
			strcpy(theBuffer,theDirName);
			strcat(theBuffer,fd.cFileName);
			strcat(theBuffer,"/");
			strcat(theBuffer,"*");
			ioStack.push(std::string(theBuffer));
		}
	} while(FindNextFile(hFind, &fd));

    FindClose(hFind);



}




//========================================
//出力するファイルリストを作成
//========================================
void MyCompressData::MakeFileList(char *inFileName,char *outFileList)
{
	bool theStartFlag = 1;
	if(outFileList[0] != '\0')
	{
		theStartFlag = 0;
	}

	if(IsWildCard(inFileName))
	{
		std::stack<std::string>theDirStack;

		theDirStack.push(std::string(inFileName));

		while(theDirStack.size() != 0)
		{
			std::string theWildCard = theDirStack.top();
			theDirStack.pop();
			AnalysisWildCard(theWildCard.c_str(),outFileList,theDirStack);
		}
	}
	else
	{
		//================================
		//ワイルドカード未使用時
		//================================
		if(theStartFlag == 0)
		{
			strcat(outFileList,",");
		}
		strcat(outFileList,inFileName);
		theStartFlag = 0;
	}
}








//====================================
//引数のファイルからパックファイルを作成します。
//====================================
HRESULT MyCompressData::JointFromFile(char *inFileName)
{
	FILE *fin = fopen(inFileName,"r");


	
	if(fin == NULL)
	{
		assert("そんなファイルないですよ");
		return E_FAIL;
	}

	
	char outFileName[FILE_NAME_LENGTH];
	if(NULL == fgets(outFileName,FILE_NAME_LENGTH,fin))
	{
		assert("出力するファイルを書きましょう。");
		return E_FAIL;
	}
	RejectLastNL(outFileName);
	

	char fileList[FILE_LIST_LENGTH] = "";
	char buffer[FILE_NAME_LENGTH];
	
	
	while(NULL != fgets(buffer,FILE_NAME_LENGTH,fin))
	{
		RejectLastNL(buffer);
		MakeFileList(buffer,fileList);
	}
	fclose(fin);
	return Joint(fileList,outFileName);
}

