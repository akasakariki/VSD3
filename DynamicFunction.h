#include "Renderer.h"
#include "Scene.h"
#include "Texture.h"
#include "FileReader.h"
struct _tagDynamic
{
	char impname[4];
	char argudata[10][255];//255文字まで 10データーまで
};


struct _fuct
{
	char *name;
	int *adminnum;
};

class CDynamicFuction
{
public:
	int InitMakeFuction(Ctexture *ptexture,Csound *sound,CADV *adv,CVariable *tmpvariable,CVariable *sysvariable,CFileReader	*fr);
	int AddFuction(char);
	int DynamicScripter_Process(void);
private:
	_fuct *fuctions;
	_ReturnVariabledata temp_variable;
	//パーサー
	int DynamicScripter_Image(_tagDynamic tagdynamic);
	int DynamicScripter_Sound(_tagDynamic tagdynamic);
	int DynamicScripter_Text(_tagDynamic tagdynamic);
	int DynamicScripter_Variable(_tagDynamic tagdynamic);	
	int DynamicScripter_System(_tagDynamic tagdynamic);
	//保持アドレス
	Ctexture *texture;//イメージ関係
	Csound *sound;//サウンド関係
	CADV *adv;//テキスト関係
	CVariable *tmpvariable;//一時変数関係
	CVariable *sysvariable;//システム変数関係
	CFileReader	*fr;

};