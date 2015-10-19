//--------------------------------------------------------
//���I�X�N���v�g����(�X�N���v�g�����f�A����)
//�����D�揇�ʁ@�V�X�e���֌W�����̑��B
//--------------------------------------------------------
#include "DynamicFunction.h"
#include "DynamicFunctionList.h"

#include <string.h>
#include <stdlib.h>

int CDynamicFuction::InitMakeFuction(Ctexture *ptexture,Csound *psound,CADV *padv,CVariable *ptmpvariable,CVariable *psysvariable,CFileReader	*pfr)
{
	DebugText("Dynamic Script System Initialize...");

	texture=ptexture;
	sound=psound;
	adv=padv;
	tmpvariable=ptmpvariable;
	sysvariable=psysvariable;
	fr=pfr;
	//�ꎞ�ϐ��쐬
	//tmpvariable->Add_Variable();
	DebugText("OK!\n");
	
	return 0;
}
int CDynamicFuction::DynamicScripter_Process()
{

	return 0;

}
//�ȉ��A�X�N���v�g�����֌W���[��
//0�͐���ɖ��ߕ���������������������
//-1�G���[�͖��߃X�N���v�g���������Ă��Ȃ��B
//-2�G���[�͖��ߕ��𔭌��͂������A�����ɖ�肪���������B

//�C���[�W���C���[�֌W--------------------------------------------------------------------------------------
int CDynamicFuction::DynamicScripter_Image(_tagDynamic tagdynamic)	
{
	//���C���V�X�e���֌W(ls)
	//�`�ʏ����J�n�n�_�iS���`�ʏ���<dw>��E�Ƃ��邱�ƂŁA1�t���[�����Ƃ̈�ĕ\���j
	if(!strcmp(	tagdynamic.impname,			LAYER_SYS_S	)) 
	{
		texture->Drawing_start();
	}
	//�`�ʏ����I���n�_
	else if(!strcmp(tagdynamic.impname,		LAYER_SYS_E)) 
	{
		texture->Drawing_end();
	}
	//�F�������[�h��ύX�iBright�j
	else if(!strcmp(tagdynamic.impname,		LAYER_SYS_G)) 
	{
	

		int G_a,G_r,G_g,G_b;
		G_a=G_r=G_g=G_b=255;
		G_a=atoi(tagdynamic.argudata[0]);//�A���t�@�l
		//�l�`�F�b�N
		if(G_a>255&&G_a<0)
		{
			return -2;//�G���[ �l���s����
		}
		if(tagdynamic.argudata[1][0]!='\0')
		{
			int G_r=atoi(tagdynamic.argudata[1]);//�q�l
			int G_g=atoi(tagdynamic.argudata[2]);//�f�l
			int G_b=atoi(tagdynamic.argudata[3]);//�a�l
			//�l�`�F�b�N
			if((G_r>255&&G_r<0)&&(G_g>255&&G_g<0)&&(G_b>255&&G_b<0))
			{
				return -2;//�G���[ �l���s����
			}

		}

		texture->SetBright(G_a,G_r,G_g,G_b);
	}
	//�u�����h���[�h���w��E�ύX
	else if(!strcmp(tagdynamic.impname,		LAYER_SYS_BM)) 
	{
		//�A���t�@���[�h	tagdynamic.argudata[0]=0�@tagdynamic.argudata[2]=�A���t�@�l
		int mode,num;
		mode=atoi(tagdynamic.argudata[0]);//���[�h
		num=atoi(tagdynamic.argudata[1]);//�l
		if(num>255&&num<0)
		{
			return -2;//�G���[ �l���s����
		}
		if(mode==0)
		{
			texture->SetBlendMode(MODE_ALPHA,num);
		}
		else
		{
			return -2;//�G���[���݂��Ȃ��u�����h���[�h
		}
	}
	//�u�����h���[�h������
	else if(!strcmp(tagdynamic.impname,		LAYER_SYS_BMC)) 
	{
		texture->NoBlend();
	}
	//�J���[�L�[��ݒ�i�ǂݍ��ݑO�Ɏw��j
	else if(!strcmp(tagdynamic.impname,		LAYER_SYS_CKEY)) 
	{
		int G_r,G_g,G_b;
		G_r=G_g=G_b=255;
		G_r=atoi(tagdynamic.argudata[0]);//�q�l
		G_g=atoi(tagdynamic.argudata[1]);//�f�l
		G_b=atoi(tagdynamic.argudata[2]);//�a�l
		//�l�`�F�b�N
		if((G_r>255&&G_r<0)&&(G_g>255&&G_g<0)&&(G_b>255&&G_b<0))
		{
			return -2;//�G���[ �l���s����
		}

		texture->SetLoadImageColorKey(G_r, G_g,G_b);
	}
	//���[�h�֌W(ll)
	//�摜�����C���[�ɒʏ�ǂݍ���
	else if(!strcmp(tagdynamic.impname,		LAYER_LOAD_A)) 
	{
		//�t�@�C����
		//tagdynamic.argudata[0]
		//�ϐ��ɑ���B
		//tagdynamic.argudata[1]
	
		int temp_varialnum=texture->LoadImage(tagdynamic.argudata[0],fr);
		
		//�w��ϐ��̊Ǘ��ԍ����擾
		temp_variable=tmpvariable->SearchVariable(tagdynamic.argudata[1]);
		if(temp_variable.type==1)
		{
			//�ϐ��^�C�vint
			tmpvariable->ChangeVariable_int(&temp_varialnum,temp_variable.num);
		}
		else
		{
			//�G���[ �w�肳�ꂽ�ϐ��͕s�����iint�ł͂Ȃ�
			return -1;
		}

		
	}
	//�摜���}�X�N����
	else if(!strcmp(tagdynamic.impname,		LAYER_LOAD_M)) 
	{
			
		int temp_varialnum=texture->LoadImage_Mask(tagdynamic.argudata[0],fr);

		
		//�w��ϐ��̊Ǘ��ԍ����擾
		temp_variable=tmpvariable->SearchVariable(tagdynamic.argudata[1]);
		if(temp_variable.type==1)
		{
			//�ϐ��^�C�vint
			tmpvariable->ChangeVariable_int(&temp_varialnum,temp_variable.num);
		}
		else
		{
			//�G���[ �w�肳�ꂽ�ϐ��͕s�����iint�ł͂Ȃ�
			return -1;
		}
	}
	//�폜�֌W(ld)
	//�w�背�C���[�폜
	else if(!strcmp(tagdynamic.impname,		LAYER_DELETE_AT)) 
	{
		//0:�폜���C����ԍ��B
		texture->DeleteImage_At(atoi(tagdynamic.argudata[0]));
	}
	//���C���S�폜
	else if(!strcmp(tagdynamic.impname,		LAYER_DELETE_ALL)) 
	{
		texture->DeleteImage_All();
	}
	//�`�ʊ֌W(lw)
	//�摜�A�ʏ�`��
	else if(!strcmp(tagdynamic.impname,		LAYER_DRAW_A)) 
	{
		texture->DrawImage(atoi(tagdynamic.argudata[0]),atof(tagdynamic.argudata[1]),atof(tagdynamic.argudata[2]));
	}
	//�g��`��
	else if(!strcmp(tagdynamic.impname,		LAYER_DRAWR_S)) 
	{
		texture->DrawImage_Stretch(atoi(tagdynamic.argudata[0]),
										atof(tagdynamic.argudata[1]),
										atof(tagdynamic.argudata[2]),
										atof(tagdynamic.argudata[3]),
										atof(tagdynamic.argudata[4]));
	}
	//��]�`��
	else if(!strcmp(tagdynamic.impname,		LAYER_DRAW_R)) 
	{
		texture->DrawImage_Rotation(atoi(tagdynamic.argudata[0]),
										atof(tagdynamic.argudata[1]),
										atof(tagdynamic.argudata[2]),
										atof(tagdynamic.argudata[3]),
										atof(tagdynamic.argudata[4])
									,	atof(tagdynamic.argudata[5]));
	}
	//�g���]�`��
	else if(!strcmp(tagdynamic.impname,		LAYER_DRAW_SR)) 
	{
		texture->DrawImage_StretchRotation(atoi(tagdynamic.argudata[0]),
										atof(tagdynamic.argudata[1]),
										atof(tagdynamic.argudata[2]),
										atof(tagdynamic.argudata[3]),
										atof(tagdynamic.argudata[4]),
										atof(tagdynamic.argudata[5])
										,atof(tagdynamic.argudata[6])
										,atof(tagdynamic.argudata[7]));
	}
//�u�����h���[�h
	else if(!strcmp(tagdynamic.impname,		LAYER_DRAW_B)) 
	{
		//�G���[��������Ă��܂���B�i�������悤�Ƃ��Ă��̂����E�E�E�H
	}
	else
	{
		return -1;
	}
	return 0;
}




//�T�E���h�֌W(s)--------------------------------------------------------------------------------------
int CDynamicFuction::DynamicScripter_Sound(_tagDynamic tagdynamic)	
{
	//�T�E���h�ǂݍ���
	if(!strcmp(	tagdynamic.impname,			SOUND_LOAD)) 
	{
		
	}
	//�T�E���h�Đ�
	else if(!strcmp(tagdynamic.impname,		SOUND_PLAY)) 
	{
	}
	//�ʃT�E���h�ꎞ��~
	else if(!strcmp(tagdynamic.impname,		SOUND_STOP_AT)) 
	{
	}
	//�S�T�E���h�ꎞ��~
	else if(!strcmp(tagdynamic.impname,		SOUND_STOP_ALL)) 
	{
	}
	//�ʃT�E���h�폜
	else if(!strcmp(tagdynamic.impname,		SOUND_DELETE_AT	)) 
	{
	}
	//�S�T�E���h�폜
	else if(!strcmp(tagdynamic.impname,		SOUND_DELETE_ALL)) 
	{
	}
	//�ʃT�E���h�{�����[���ύX
	else if(!strcmp(tagdynamic.impname,		SOUND_VOLUME_AT	)) 
	{
	}
	//�ʃT�E���h�{�����[�����擾
	else if(!strcmp(tagdynamic.impname,		SOUND_GETVOLUME_AT)) 
	{
	}
	//�ʃT�E���h�̍Đ���Ԃ��擾
	else if(!strcmp(tagdynamic.impname,		SOUND_GETPLAYCHECK)) 
	{
	}
	else
	{
		return -1;
	}
	return 0;
}





//�e�L�X�g�֌W(t)--------------------------------------------------------------------------------------
int CDynamicFuction::DynamicScripter_Text(_tagDynamic tagdynamic)	
{
	//�t�H���g�ݒ�
	if(!strcmp(	tagdynamic.impname,			TEXT_SETFONT)) 
	{
		
	}
	//�e�L�X�g�`��
	else if(!strcmp(tagdynamic.impname,		TEXT_DRAWTEXT)) 
	{
	}
	//�t�H���g�폜	
	else if(!strcmp(tagdynamic.impname,		TEXT_DELETEFONT_AT)) 
	{
	}
	//�t�H���g�S�폜
	else if(!strcmp(tagdynamic.impname,		TEXT_DELETEFONT_ALL)) 
	{
	}
	else
	{
		return -1;
	}
	return 0;
}

//�ϐ��֌W(v)--------------------------------------------------------------------------------------
int CDynamicFuction::DynamicScripter_Variable(_tagDynamic tagdynamic)	
{
	//�ϐ��쐬
	if(!strcmp(	tagdynamic.impname,			VARIABLE_MAKE)) 
	{
		
	}
	//�ϐ����܂߂��v�Z
	else if(!strcmp(tagdynamic.impname,		VARIABLE_CALILATION	)) 
	{
	}
	else
	{
		return -1;
	}
	return 0;
}



//�V�X�e���֌W(s)--------------------------------------------------------------------------------------
int CDynamicFuction::DynamicScripter_System(_tagDynamic tagdynamic)	
{

	if(!strcmp(	tagdynamic.impname,			VARIABLE_MAKE)) 
	{
		
	}

	else if(!strcmp(tagdynamic.impname,		VARIABLE_CALILATION	)) 
	{
	}
	else
	{
		return -1;
	}
	return 0;
}
