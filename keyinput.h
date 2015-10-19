#include "dinput.h"

#pragma comment(lib, "dinput8.lib")
//------------------------------------------------
//キー入力インターフェイス
//------------------------------------------------


class CKeyinput{
private:
	POINT point;
	LPDIRECTINPUT8 g_pDInput;
	BYTE byKeyState[256];
	LPDIRECTINPUTDEVICE8 g_pDIDevice;
	HWND *copyhwnd;

	bool switch_key;
	bool switch_pointer;
public:
	 int init_Key(HINSTANCE *hinst,HWND *hwnd);
	 void Reload_keystatus(void);
	 bool Get_keystutus(char types[15]);
	 bool Sys_Get_keystutus(int typs);
};

//キーボードコード変換
extern  int check_key(char type[15]);