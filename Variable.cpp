#include "Variable.h"
#include "Common.h"
#pragma warning (disable:4996)
int CVariable::SettingVariable()
{
	DebugText("SetVariable...");
	Totalmax=ADD_MAX_VARIABLE;
	Max=-1;
	variable=(_Variable*)malloc(sizeof(_Variable) *Totalmax);
	DebugText("OK!\n");
	return 0;
}

_ReturnVariabledata CVariable::SearchVariable(char *name)
{
	DebugText("SearchVariable(Name=%s)...",name);
	_ReturnVariabledata temp_a;
	for(int i=0;i<=Max;i++)
	{
		if(strcmp(variable[i].name,name)==0)
		{
			temp_a.num=i;
			temp_a.type=variable[i].type;
			return temp_a;
		}
	}
	temp_a.num=-1;
	DebugText("OK!\n");

	return temp_a;
}

int CVariable::DeleteVariable(int num)
{
	DebugText("DeleteVariable(num=%d)\n",num);

	switch(variable[num].type)
	{
	case 0:
		//error
		return -1;
		break;
	case 1:
		//int
		free(variable[num].int_data);
		break;
	case 2:
		free(variable[num].char_data);
		break;
	case 3:
		free(variable[num].float_data);
		break;
	case 4:
		free(variable[num].bool_data);
		break;
	}
	Max--;
	if(Max<(Totalmax-ADD_MAX_VARIABLE))
	{
		//Å‘å”‚ð’´‚¦‚½
		Totalmax-=ADD_MAX_VARIABLE;
		variable=(_Variable*)realloc(variable,sizeof(_Variable)*Totalmax);
	}
	return 0;
}

void CVariable::Return_int(int* body,int num)
{
	body=variable[num].int_data;
}


void CVariable::Return_char(char* body,int num)
{
	strcpy(body,variable[num].char_data);
}


void CVariable::Return_float(float* body,int num)
{
	body=variable[num].float_data;
}

void CVariable::Return_bool(bool* body,int num)
{
	body=variable[num].bool_data;
}

void CVariable::Add_Variable(void)
{
	Max++;
	if(Max>=Totalmax)
	{
		Totalmax+=ADD_MAX_VARIABLE;
		variable=(_Variable*)realloc(variable,sizeof(_Variable)*Totalmax);
	}
}

int CVariable::AddVariable_int(int *body, char *name)
{
	DebugText("ResistVariable(name=>%s,body=>%d)...",name,body);

	Add_Variable();
	strcpy(variable[Max].name,name);
	variable[Max].type=1;
	variable[Max].int_data=new int;
	variable[Max].int_data=body;
	DebugText("OK!\n");
	return 0;
}

int CVariable::AddVariable_char(char *body,int char_len,char *name)
{
	DebugText("ResistVariable(name=>%s,body=>%s)...",name,body);
	Add_Variable();
	strcpy(variable[Max].name,name);
	variable[Max].type=2;
	variable[Max].char_data=new char[char_len];
	strcpy(variable[Max].char_data,body);
	DebugText("OK!\n");
	return 0;
}

int CVariable::AddVariable_float(float *body, char *name)
{
	DebugText("ResistVariable(name=>%s,body=>%lf)...",name,body);
	Add_Variable();
	strcpy(variable[Max].name,name);
	variable[Max].type=3;
	variable[Max].float_data=new float;
	variable[Max].float_data=body;
	DebugText("OK!\n");
	return 0;
}

int CVariable::AddVariable_bool(bool *body, char *name)
{
	DebugText("ResistVariable(name=>%s,body=>%d)...",name,body);
	Add_Variable();
	strcpy(variable[Max].name,name);
	variable[Max].type=4;
	variable[Max].bool_data=new bool;
	variable[Max].bool_data=body;
	DebugText("OK!\n");
	return 0;
}

int CVariable::ChangeVariable_int(int* body,int num)
{
	variable[num].int_data=body;
	return 0;
}

int CVariable::ChangeVariable_char(char* body,int num)
{
	variable[num].char_data=body;
	return 0;
}

int CVariable::ChangeVariable_float(float *body,int num)
{
	variable[num].float_data=body;
	return 0;
}

int CVariable::ChangeVariable_bool(bool *body,int num)
{
	variable[num].bool_data=body;
	return 0;
}



int CVariable::DeleteVariable_all()
{
	DebugText("AllDeleteVariable...");
	for(int i=0;i<=Max;i++)
	{
		DeleteVariable(i);
	}
	Max=-1;
	DebugText("OK!\n");
	return 0;
}


int CVariable::AllRelease()
{
	DeleteVariable_all();
	free(variable);
	return 0;
}
