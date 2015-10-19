#pragma warning( disable : 4786 )
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector> 
#include <stack>
#include "parser.h"

//������̃X�y�[�X�����ׂĎ�菜��
void trim(char*str){
	if(' '==*str) strcpy(str,str+1);
	if(*str++) trim(str);
}
//��������g�[�N�����Ƃɐ؂�o��
void token(char*str,std::vector<std::string>*data){
	char pos;
	std::string strpos="";
	while(pos=*str++){
		switch (pos){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			strpos=strpos+pos;
			break;
		case '+':
			if(strpos!="") data->push_back(strpos);
			strpos="";
			data->push_back("+");
			break;
		case '-':
			if(strpos!="") data->push_back(strpos);
			strpos="";
			data->push_back("-");
			break;
		case '*':
			if(strpos!="") data->push_back(strpos);
			strpos="";
			data->push_back("*");
			break;
		case '/':
			if(strpos!="") data->push_back(strpos);
			strpos="";
			data->push_back("/");
			break;
		case '(':
			if(strpos!="") data->push_back(strpos);
			strpos="";
			data->push_back("(");
			break;
		case ')':
			if(strpos!="") data->push_back(strpos);
			strpos="";
			data->push_back(")");
			break;
		default:
			break;
		}
	}
	if(strpos!="") data->push_back(strpos);
}

//�g�[�N���ł��邩�m�F
bool isToken(std::string str){
	if(str=="+" || str=="-" || str=="*" || str=="/") return true;
	return false;
}

//�g�[�N���̐��`����
void tokenAna(std::vector<std::string>*in,std::vector<std::string>*out){
	if(isToken((*in)[0])){
		(*in)[1]=(*in)[0]+(*in)[1];
	}else{
		out->push_back((*in)[0]);
	}

	for(int i=1;i<in->size();i++) {
		if(isToken((*in)[i])){
			if(isToken((*in)[i-1])){
				(*in)[i+1]=(*in)[i]+(*in)[i+1];
			}else{
				if((*in)[i-1]=="("){
					(*in)[i+1]=(*in)[i]+(*in)[i+1];
				}else{
					out->push_back((*in)[i]);
				}
			}
		}else{
			out->push_back((*in)[i]);
		}
	}
}



//�t�|�[�����h�@�ɕϊ����邽�߂̉��Z�q�̗D�揇�ʂ�����
int Priority(const char*str){
	if(0==strcmp(str,"+")) return 10;
	if(0==strcmp(str,"-")) return 10;
	if(0==strcmp(str,"*")) return 20;
	if(0==strcmp(str,"/")) return 25;
	return 0;
}

//�������t�|�[�����h�@�ɕϊ�
void Analyzer(std::vector<std::string>*data,std::vector<std::string>*out,int*i){
	std::stack<const char*> stack;

	for(;*i<data->size();(*i)++){
		if((*data)[*i]=="+" || (*data)[*i]=="-" || (*data)[*i]=="*" || (*data)[*i]=="/"){
			if(stack.empty()==false){//false : �X�^�b�N�ɒl������
				if(Priority((*data)[*i].c_str())==Priority(stack.top())){
					stack.push((*data)[*i].c_str());//�l�̒ǉ�
				}
				if(Priority((*data)[*i].c_str())<Priority(stack.top())){
					while(stack.empty()==false){//false : �X�^�b�N�ɒl������
						const char*str=stack.top();//�Ō���̒l�̎擾
						stack.pop();//�Ō���̒l�̍폜
						out->push_back(str);
					}
					stack.push((*data)[*i].c_str());//�l�̒ǉ�
				}
				if(Priority((*data)[*i].c_str())>Priority(stack.top())){
					stack.push((*data)[*i].c_str());//�l�̒ǉ�
				}
			}else{
				stack.push((*data)[*i].c_str());//�l�̒ǉ�
			}
		}else{
			if((*data)[*i]=="("){
				*i=*i+1;
				Analyzer(data,out,i);
			}else{
				if((*data)[*i]==")"){
					goto end;
				}else{
					out->push_back((*data)[*i]);
				}
			}
		}
	}
end:
	while(stack.empty()==false){//false : �X�^�b�N�ɒl������
		const char*str=stack.top();//�Ō���̒l�̎擾
		stack.pop();//�Ō���̒l�̍폜
		out->push_back(str);
	}
}

//�t�|�[�����h�@�\�L�̌v�Z�����v�Z
double calc(std::vector<std::string>*out){
	int c=out->size();
	std::stack<double> stack;
	double j,k;
	for(int i=0;i<c;i++){
			if((*out)[i]== "+"){
				j=stack.top();
				stack.pop();
				k=stack.top();
				stack.pop();
				stack.push(k+j);
			}else if((*out)[i]=="-"){
				j=stack.top();
				stack.pop();
				k=stack.top();
				stack.pop();
				stack.push(k-j);
			}else if((*out)[i]=="*"){
				j=stack.top();
				stack.pop();
				k=stack.top();
				stack.pop();
				stack.push(k*j);
			}else if((*out)[i]=="/"){
				j=stack.top();
				stack.pop();
				k=stack.top();
				stack.pop();
				stack.push(k/j);
			}else{
				stack.push(atof((*out)[i].c_str()));
			}
		}
	return stack.top();
}

double BorlandAnser(char *str)
{
	std::vector<std::string> data;
	std::vector<std::string> out;
	//trim(str);
	token(str,&data);
	std::vector<std::string> data2;
	tokenAna(&data,&data2);
	int c=0;
	Analyzer(&data2,&out,&c);
	return calc(&out);
}

/*
int main(){
	printf("%s\n","((-10+-20)*((30+40))+1)*2 [Enter] �̗l�ɓ��͂��Ă�������");
	printf("--------------------------------------\n");
	while(1){
		std::vector<std::string> data;
		std::vector<std::string> out;

		//������Ƃ��ē���
		char str[1024];
		scanf("%s",str);
		trim(str);
		token(str,&data);
		
		std::vector<std::string> data2;
		tokenAna(&data,&data2);

		{printf("�g�[�N���؂�o������ : ");for(int a=0;a<data2.size();a++) printf("%s ",data2[a].c_str());}
		
		int c=0;
		Analyzer(&data2,&out,&c);

		{printf("\n�t�|�[�����h�@�ɕϊ� : ");for(int a=0;a<out.size();a++) printf("%s ",out[a].c_str());}
		printf("\n�v�Z���� : ");printf("%g\n",calc(&out));
		printf("--------------------------------------\n");
	}
	getchar();
	return 0;
}
*/

