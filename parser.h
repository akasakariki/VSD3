#pragma once
#include "Renderer.h"
#include "Scene.h"
//�ȉ��X�N���v�g�L���K��
#define START_TAG			'<'			//�^�O�̊J�n�_			
#define END_TAG				'>'			//�^�O�̏I���_
#define IMP_NAME_DELIMI		' '			//���ߕ��I��
#define ARGU_DELIMI			' '			//�ʈ����I��
#define ARGU_DELIVERY		'='			//�����f�[�^�[�`�F�b�J�[
#define STRI_SIGN			'"'			//������J�n�E�I���_

//�ȉ��}�l�[�W�����g
#define TAG_DATA_MAX 10


struct _tagbody{
	int status;
	int data_max;
	char impname[15];//���ߖ��@15�����ȓ�
	char arguname[10][15];//������ 15�����ȓ�,10�f�[�^�[�܂�
	char argudata[10][255];//255�����܂� 10�f�[�^�[�܂�
};
//��
struct _Tag
{

	int tag_max;
	int tag_num;
	_tagbody	*tag_body;
};

class CTag
{
private:
	//_Tag tag;


	int Add_Tag(_Tag *tag);
	int ResetTag(_Tag *tag);
	int free_Tag(_Tag *tag);
	int trim_skip(char *script,int *i);
public:
	int Init_Tag(_Tag *tag);
	int push_parser(char script[],_Tag *tag);//�^�O���L�^
	_tagbody pop_parser(_Tag *tag);//�^�O���L�^
	int debugtest_pop_parser(_Tag *tag);//�f�o�b�O�e�X�g�p�A�S�Ẵ^�O���o�́B//printf�o��
		
};


extern double BorlandAnser(char *str);