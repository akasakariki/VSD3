#include "dinput.h"
#include "keyinput.h"
int check_key(char type[15])
{
	//テンキー
	if(strcmp(type,"TENKEY_PLUS"))return DIK_ADD;			
	if(strcmp(type,"TENKEY_DOT"))return DIK_DECIMAL;
	if(strcmp(type,"TENKEY_SLASH"))return DIK_DIVIDE;
	if(strcmp(type,"TENKEY_ASTERISK"))return DIK_MULTIPLY;
	if(strcmp(type,"TENKEY_ENTER"))return DIK_NUMPADENTER;
	if(strcmp(type,"TENKEY_MINUS"))return DIK_SUBTRACT;

	if(strcmp(type,"TENKEY_0"))return DIK_NUMPAD0;
	if(strcmp(type,"TENKEY_1"))return DIK_NUMPAD1;
	if(strcmp(type,"TENKEY_2"))return DIK_NUMPAD2;
	if(strcmp(type,"TENKEY_3"))return DIK_NUMPAD3;
	if(strcmp(type,"TENKEY_4"))return DIK_NUMPAD4;
	if(strcmp(type,"TENKEY_5"))return DIK_NUMPAD5;
	if(strcmp(type,"TENKEY_6"))return DIK_NUMPAD6;
	if(strcmp(type,"TENKEY_7"))return DIK_NUMPAD7;
	if(strcmp(type,"TENKEY_8"))return DIK_NUMPAD8;
	if(strcmp(type,"TENKEY_9"))return DIK_NUMPAD9;

	//数字
	if(strcmp(type,"KEY_0"))return DIK_0;
	if(strcmp(type,"KEY_1"))return DIK_1;
	if(strcmp(type,"KEY_2"))return DIK_2;
	if(strcmp(type,"KEY_3"))return DIK_3;
	if(strcmp(type,"KEY_4"))return DIK_4;
	if(strcmp(type,"KEY_5"))return DIK_5;
	if(strcmp(type,"KEY_6"))return DIK_6;
	if(strcmp(type,"KEY_7"))return DIK_7;
	if(strcmp(type,"KEY_8"))return DIK_8;
	if(strcmp(type,"KEY_9"))return DIK_9;
	//アルファベット
	if(strcmp(type,"KEY_A"))return DIK_A;
	if(strcmp(type,"KEY_B"))return DIK_B;
	if(strcmp(type,"KEY_C"))return DIK_C;
	if(strcmp(type,"KEY_D"))return DIK_D;
	if(strcmp(type,"KEY_E"))return DIK_E;
	if(strcmp(type,"KEY_F"))return DIK_F;
	if(strcmp(type,"KEY_G"))return DIK_G;
	if(strcmp(type,"KEY_H"))return DIK_H;
	if(strcmp(type,"KEY_I"))return DIK_I;
	if(strcmp(type,"KEY_J"))return DIK_J;
	if(strcmp(type,"KEY_K"))return DIK_K;
	if(strcmp(type,"KEY_L"))return DIK_L;
	if(strcmp(type,"KEY_N"))return DIK_N;
	if(strcmp(type,"KEY_M"))return DIK_M;
	if(strcmp(type,"KEY_O"))return DIK_O;
	if(strcmp(type,"KEY_P"))return DIK_P;
	if(strcmp(type,"KEY_Q"))return DIK_Q;
	if(strcmp(type,"KEY_R"))return DIK_R;
	if(strcmp(type,"KEY_S"))return DIK_S;
	if(strcmp(type,"KEY_T"))return DIK_T;
	if(strcmp(type,"KEY_U"))return DIK_U;
	if(strcmp(type,"KEY_V"))return DIK_V;
	if(strcmp(type,"KEY_W"))return DIK_W;
	if(strcmp(type,"KEY_X"))return DIK_X;
	if(strcmp(type,"KEY_Y"))return DIK_Y;
	if(strcmp(type,"KEY_Z"))return DIK_Z;
	//ファンクションキー
	if(strcmp(type,"KEY_F1"))return DIK_F1;
	if(strcmp(type,"KEY_F2"))return DIK_F2;
	if(strcmp(type,"KEY_F3"))return DIK_F3;
	if(strcmp(type,"KEY_F4"))return DIK_F4;
	if(strcmp(type,"KEY_F5"))return DIK_F5;
	if(strcmp(type,"KEY_F6"))return DIK_F6;
	if(strcmp(type,"KEY_F7"))return DIK_F7;
	if(strcmp(type,"KEY_F8"))return DIK_F8;
	if(strcmp(type,"KEY_F9"))return DIK_F9;
	if(strcmp(type,"KEY_F10"))return DIK_F10;
	if(strcmp(type,"KEY_F11"))return DIK_F11;
	if(strcmp(type,"KEY_F12"))return DIK_F12;
	//僕のキーボードだとF12までしかないんだけど、キーボードによってF15まであるものなんだろうか…。
	if(strcmp(type,"KEY_F13"))return DIK_F13;
	if(strcmp(type,"KEY_F14"))return DIK_F14;
	if(strcmp(type,"KEY_F15"))return DIK_F15;

	//その他
	if(strcmp(type,"KEY_APPS"))return DIK_APPS;
	if(strcmp(type,"KEY_BACKSP"))return DIK_BACK;
	if(strcmp(type,"KEY_CAPSLOCK"))return DIK_CAPITAL;
	if(strcmp(type,"KEY_COMMA"))return DIK_COMMA;
	if(strcmp(type,"KEY_DELETE"))return DIK_DELETE;
	if(strcmp(type,"KEY_END"))return DIK_END;
	if(strcmp(type,"KEY_EQUALS"))return DIK_EQUALS;
	if(strcmp(type,"KEY_ESCAPE"))return DIK_ESCAPE;
	if(strcmp(type,"KEY_GRAVE"))return DIK_GRAVE;
	if(strcmp(type,"KEY_SEMICOLON"))return DIK_SEMICOLON;
	if(strcmp(type,"KEY_SLASH"))return DIK_SLASH;
		if(strcmp(type,"KEY_YEN"))return DIK_YEN;
	if(strcmp(type,"KEY_HOME"))return DIK_HOME;
	if(strcmp(type,"KEY_INSERT"))return DIK_INSERT;
	if(strcmp(type,"KEY_LBRACKET"))return DIK_LBRACKET;
	if(strcmp(type,"KEY_LCONTROL"))return DIK_LCONTROL;
	if(strcmp(type,"KEY_LALT"))return DIK_LMENU;
	if(strcmp(type,"KEY_LSHIFT"))return DIK_LSHIFT;
	if(strcmp(type,"KEY_LWIN"))return DIK_LWIN;
	if(strcmp(type,"KEY_PGDOWN"))return DIK_NEXT;
	if(strcmp(type,"KEY_NUMLOCK"))return DIK_NUMLOCK;
	if(strcmp(type,"KEY_RBRACKET"))return DIK_RBRACKET;
	if(strcmp(type,"KEY_PAUSE"))return DIK_PAUSE;
	if(strcmp(type,"KEY_PERIOD"))return DIK_PERIOD;
	if(strcmp(type,"KEY_PGUP"))return DIK_PRIOR;
	if(strcmp(type,"KEY_RCONTROL"))return DIK_RCONTROL;
	if(strcmp(type,"KEY_RALT"))return DIK_RMENU;
	if(strcmp(type,"KEY_RSHIFT"))return DIK_RSHIFT;
	if(strcmp(type,"KEY_RWIN"))return DIK_RWIN;
	if(strcmp(type,"KEY_SCROLL"))return DIK_SCROLL;
	if(strcmp(type,"KEY_SPACE"))return DIK_SPACE;
	if(strcmp(type,"KEY_RETURN"))return DIK_RETURN;
	if(strcmp(type,"KEY_SYSRQ"))return DIK_SYSRQ;
	if(strcmp(type,"KEY_TAB"))return DIK_TAB;
	if(strcmp(type,"KEY_UP"))return DIK_UP;
	if(strcmp(type,"KEY_DOWN"))return DIK_DOWN;
	if(strcmp(type,"KEY_LEFT"))return DIK_LEFT;
		if(strcmp(type,"KEY_RIGHT"))return DIK_RIGHT;
		return -1;
}