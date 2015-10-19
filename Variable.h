#include <string>
#define ADD_MAX_VARIABLE	99

struct _Variable
{
	char name[30];
	int type;
	//0=error‚¨‚æ‚Ñnodata
	//1=int
	//2=char(‚½‚¾‚µ•¶Žš—ñ)
	//3=float
	//4=bool
	int *int_data;
	char *char_data;
	float *float_data;
	bool *bool_data;
};

struct _ReturnVariabledata
{

	int num;
	//int type		ƒ^ƒCƒv
	//1=int
	//2=char
	//3=float 
	//4=bool
	int type;
};
class CVariable
{
private:
	int Totalmax;
	int Max;
	_Variable *variable;
	_ReturnVariabledata temp;
public:
	int SettingVariable(void);
	_ReturnVariabledata SearchVariable(char *name);
	void Add_Variable(void);
	int DeleteVariable(int num);
	int DeleteVariable_all(void);
	int AllRelease(void);

	int AddVariable_int(int *body,char *name);
	int AddVariable_char(char *body,int char_len,char *name);
	int AddVariable_float(float *body,char *name);
	int AddVariable_bool(bool *body,char *name);

	int ChangeVariable_int(int* body,int num);
	int ChangeVariable_char(char* body,int num);
	int ChangeVariable_float(float *body,int num);
	int ChangeVariable_bool(bool *body,int num);


	void Return_int(int* body,int num);
	void Return_char(char* body,int num);
	void Return_float(float *body,int num);
	void Return_bool(bool *body,int num);


};

