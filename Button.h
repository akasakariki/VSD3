
struct _button
{
	int x;
	int y;
	int w;
	int h;
	bool drawf;
	char variable[30];
	
}
class CButton
{
private:
	_button *button;
	CVariable *VariableAddress;
public:
	int SettingVariable(CVariable *Variable);
	int AddButton(int x,int y,int w,int h,char *variables);
	int UpdateButton_xy(int x,int y);
	int UpdateButton_wh(int w,int h);
	int UpdateButton_Variable(char *variables);
}