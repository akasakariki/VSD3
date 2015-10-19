#include "Renderer.h"
#include "Scene.h"
#include "Texture.h"
#include "FileReader.h"
struct _tagDynamic
{
	char impname[4];
	char argudata[10][255];//255�����܂� 10�f�[�^�[�܂�
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
	//�p�[�T�[
	int DynamicScripter_Image(_tagDynamic tagdynamic);
	int DynamicScripter_Sound(_tagDynamic tagdynamic);
	int DynamicScripter_Text(_tagDynamic tagdynamic);
	int DynamicScripter_Variable(_tagDynamic tagdynamic);	
	int DynamicScripter_System(_tagDynamic tagdynamic);
	//�ێ��A�h���X
	Ctexture *texture;//�C���[�W�֌W
	Csound *sound;//�T�E���h�֌W
	CADV *adv;//�e�L�X�g�֌W
	CVariable *tmpvariable;//�ꎞ�ϐ��֌W
	CVariable *sysvariable;//�V�X�e���ϐ��֌W
	CFileReader	*fr;

};