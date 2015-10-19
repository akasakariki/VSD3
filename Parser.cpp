#include <stdio.h>
#include <string>
#include <mbctype.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "Common.h"
int SP,CP;
int CTag::free_Tag(_Tag *tag)
{
	free(tag->tag_body);
	return 0;
}

int CTag::Init_Tag(_Tag *tag)
{
	DebugText("TagSystem Initialize...");
	tag->tag_num=0;
	tag->tag_max=TAG_DATA_MAX;
	tag->tag_body=(_tagbody*)malloc(sizeof(_tagbody) * tag->tag_max);
	DebugText("OK!\n");
	return 0;
}

int CTag::Add_Tag(_Tag *tag)
{
	if(tag->tag_num>=(tag->tag_max-1))
	{
		tag->tag_max+=TAG_DATA_MAX;
		tag->tag_body=(_tagbody*)malloc(sizeof(_tagbody) * tag->tag_max);

	}
	tag->tag_num++;

	return 0;
}

int CTag::ResetTag(_Tag *tag)
{
	if(tag->tag_max>TAG_DATA_MAX)//�W����Ԃ�葽���Ɣ��f
	{
		tag->tag_max=0;
		free_Tag(tag);
		Add_Tag(tag);
		tag->tag_num=0;
	}
	tag->tag_num=0;
	return 0;
}

int CTag::trim_skip(char *script,int *i)//�����I�g�����ł͂Ȃ��A�X�L�b�v���Ă��������B
{
			if(script[*i]==' ')//���p�󔒂������Ă���ꍇ�̓T�[�`���Ă݂�B
			{
				for(;script[*i]==' ';*i+=1)
				{
					if(script[*i]='\0')
					{
						return -1;
					}
				}

			}
			return 0;
}

//�P�s���̃^�O���v�b�V��
int CTag::push_parser(char script[],_Tag *tag)
{
	DebugText("PushTag...");

	bool stri_f=false;
	int i=0;
	if(script[i]==START_TAG)//�^�O�̊J�n�_�͑��݂��邩���Ȃ��̂ł���΃T�[�`�B������������Ȃ��̂ł���΃G���[
	{
		for(;script[i]!=START_TAG;i++)
		{
			if(script[i]=='\0')
			{
				//�G���[�B�^�O�����������B
				return -1;
			}
		}
	}
	i++;
	char *p;//�|�C���^
	int k=0;
	p=tag->tag_body[tag->tag_num].impname;//�l�[���擾�J�n
	for(int k=0;k<=15;k++)
	{
		if(script[i]==IMP_NAME_DELIMI)break;//��؂�𔭌��B
		if(script[i]!=' ')*p++=script[i];//�󔒕������͂����Ă���Δr���B
		i++;
	};
	if(k>=15||isalpha(tag->tag_body[tag->tag_num].impname[0])==0)
	{
		//�G���[�A���ߕ������������B
				return -1;
	}
	*p++='\0';

	//�����܂łŁA���ߕ������ȉ��A�����o�^�B
	int countdata=0;

	DebugText("...");
	do {

		p=tag->tag_body[tag->tag_num].arguname[countdata];
		//������
		if(isalpha(script[i]))//���������p�ꂩ��n�܂��Ă��邩�`�F�b�N�B
		{
			*p++=script[i];
			i++;
		}
		else
		{
			//�G���[�B�擪���������p�p��ł͂Ȃ��B���l�������͕�����L���̉\��������
			trim_skip(script,&i);//�O�̂��߃T�[�`
			if(isalpha(script[i])==0)
			{
				
				return -1;
			}
		}
		for(k=0;k<=15;k++)
		{
			if(script[i]==ARGU_DELIVERY)break;//��؂�𔭌��B
			if(script[i]!=' ')*p++=script[i];//�󔒕������͂����Ă���Δr���B
			i++;
		};
		*p++='\0';
		i++;
		if(k>=15||isalpha(tag->tag_body[tag->tag_num].impname[0])==0)
		{
		//�G���[�A���ԃ`�F�b�N�����������B
					
					return -1;
		}

		p=tag->tag_body[tag->tag_num].argudata[countdata];
		//�f�[�^�[�o�^
		if(script[i]==STRI_SIGN)//������錾
		{
			stri_f=true;
			*p++=script[i];//������擪�L�������̂܂܋L��
			i++;
		}
		for(int k=0;k<=15;k++)
		{
			if((stri_f==true)&&(script[i]==STRI_SIGN)){i++;break;}
			if(script[i]==ARGU_DELIMI)break;//��؂�𔭌��B
			if(script[i]==END_TAG)break;//��؂�𔭌��B
			if(script[i]!=' ')*p++=script[i];//�󔒕������͂����Ă���Δr���B
			i++;
		};
		*p++='\0';


		trim_skip(script,&i);//�O�̂��߃g����
		//�����f�[�^�[����
		countdata++;

		stri_f=false;
		if(countdata>10)
		{
			DebugText("Error:�������������܂��B\n");
			//�G���[�B�������������܂��B
		}
	
		if(script[i]==END_TAG)break;
		
	} while ((script[i]!='\0')||(script[i]!=END_TAG));	//�X�N���v�g�I�[�܂ŌJ��Ԃ��B

	
	countdata--;
	tag->tag_body[tag->tag_num].data_max=countdata;
	//�o�^����
	Add_Tag(tag);
	DebugText("%s\n",tag->tag_body[tag->tag_num].impname);

	return 0;


}




_tagbody CTag::pop_parser(_Tag *tag)
{
	DebugText("PopTag...");

	tag->tag_num--;//�|�b�v�̂��ߌ��炷

	_tagbody tags;
	
	tags=tag->tag_body[tag->tag_num];



	if(tag->tag_num<(tag->tag_max-TAG_DATA_MAX))
	{
		tag->tag_max-=TAG_DATA_MAX;
		tag->tag_body=(_tagbody*)malloc(sizeof(_tagbody) * tag->tag_max);//�̈���k��

	}
	DebugText("%s\n",tags.impname);

	return tags;
}






int CTag::debugtest_pop_parser(_Tag *tag)
{
	printf("�X�N���v�g�����v���O����V3\n�����́{�����\n\n");
	_tagbody tags;

	for(;tag->tag_num>=0;)
	{
		//�|�b�v
		tags=pop_parser(tag);
		printf("-----------------------------------\n");	
		printf("���ߕ��F%s\n",tags.impname);


		for(int i=0;i<=tags.data_max;i++)
		{
			printf("������%d�F%s\n",i,tags.arguname[i]);
			printf("�������%d�F%s\n",i,tags.argudata[i]);
		}

	}

	return 0;
}
//-----------------------------------------------
//DynamicScript�����p 
//-----------------------------------------------
//�P�s���̃^�O���v�b�V��
/*int CTag::push_parser_dynamic(char script[],_Tag *tag)
{

	bool stri_f=false;
	int i=0;
	if(script[i]==START_TAG)//�^�O�̊J�n�_�͑��݂��邩���Ȃ��̂ł���΃T�[�`�B������������Ȃ��̂ł���΃G���[
	{
		for(;script[i]!=START_TAG;i++)
		{
			if(script[i]=='\0')
			{
				//�G���[�B�^�O�����������B
				return -1;
			}
		}
	}
	i++;
	char *p;//�|�C���^
	int k=0;
	p=tag->tag_body[tag->tag_num].impname;//�l�[���擾�J�n
	for(int k=0;k<=15;k++)
	{
		if(script[i]==IMP_NAME_DELIMI)break;//��؂�𔭌��B
		if(script[i]!=' ')*p++=script[i];//�󔒕������͂����Ă���Δr���B
		i++;
	};
	if(k>=15||isalpha(tag->tag_body[tag->tag_num].impname[0])==0)
	{
		//�G���[�A���ߕ������������B
				return -1;
	}
	*p++='\0';

	//�����܂łŁA���ߕ������ȉ��A�����o�^�B
	int countdata=0;


	do {

		p=tag->tag_body[tag->tag_num].arguname[countdata];
		//������
		if(isalpha(script[i]))//���������p�ꂩ��n�܂��Ă��邩�`�F�b�N�B
		{
			*p++=script[i];
			i++;
		}
		else
		{
			//�G���[�B�擪���������p�p��ł͂Ȃ��B���l�������͕�����L���̉\��������
			trim_skip(script,&i);//�O�̂��߃T�[�`
			if(isalpha(script[i])==0)
			{
				
				return -1;
			}
		}
		for(k=0;k<=15;k++)
		{
			if(script[i]==ARGU_DELIVERY)break;//��؂�𔭌��B
			if(script[i]!=' ')*p++=script[i];//�󔒕������͂����Ă���Δr���B
			i++;
		};
		*p++='\0';
		i++;
		if(k>=15||isalpha(tag->tag_body[tag->tag_num].impname[0])==0)
		{
		//�G���[�A���ԃ`�F�b�N�����������B
					
					return -1;
		}

		p=tag->tag_body[tag->tag_num].argudata[countdata];
		//�f�[�^�[�o�^
		if(script[i]==STRI_SIGN)//������錾
		{
			stri_f=true;
			*p++=script[i];//������擪�L�������̂܂܋L��
			i++;
		}
		for(int k=0;k<=15;k++)
		{
			if((stri_f==true)&&(script[i]==STRI_SIGN)){i++;break;}
			if(script[i]==ARGU_DELIMI)break;//��؂�𔭌��B
			if(script[i]==END_TAG)break;//��؂�𔭌��B
			if(script[i]!=' ')*p++=script[i];//�󔒕������͂����Ă���Δr���B
			i++;
		};
		*p++='\0';


		trim_skip(script,&i);//�O�̂��߃g����
		//�����f�[�^�[����
		countdata++;

		stri_f=false;
		if(countdata>10)
		{
			//�G���[�B�������������܂��B
		}
	
		if(script[i]==END_TAG)break;
		
	} while ((script[i]!='\0')||(script[i]!=END_TAG));	//�X�N���v�g�I�[�܂ŌJ��Ԃ��B

	
	countdata--;
	tag->tag_body[tag->tag_num].data_max=countdata;
	//�o�^����
	Add_Tag(tag);
	return 0;


}
*/