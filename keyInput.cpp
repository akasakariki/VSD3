#include "keyinput.h"
#include "Common.h"
//------------------------------------------------
//キー入力インターフェイス
//------------------------------------------------

int CKeyinput::init_Key(HINSTANCE *hinst,HWND *hwnd)
{
	DebugText("KeyInputSystem Initialize...");

	switch_key=true;
	switch_pointer=true;
	g_pDInput=NULL;
	g_pDIDevice=NULL;
	copyhwnd=hwnd;
    DirectInput8Create(*hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, NULL);



    //IDirectInputDevice8インターフェイスの取得（キーボード）

    g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevice, NULL);

	g_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	g_pDIDevice->SetCooperativeLevel(*hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);


	//スタート
	 g_pDIDevice->Acquire();
	 DebugText("OK!\n");
	 return 0;
}

void CKeyinput::Reload_keystatus(void)
{
	
	//キーボード取得
	if(switch_key==true)
	g_pDIDevice->GetDeviceState( 256, byKeyState );
	//マウスポインター取得用
	if(switch_pointer==true)
	{
		GetCursorPos(&point);
		ScreenToClient(*copyhwnd, &point);
	}
}
//スクリプタ取得用（コンバーター通して）
bool CKeyinput::Get_keystutus(char types[15])
{
	DebugText("GetInputKey(KEY:%s)...",types);
	if(byKeyState[check_key(types)]&0x80)
	{
		DebugText("true\n");
		return true;
	}
	DebugText("false\n");
	return false;
}

//システム取得用（直接）
bool CKeyinput::Sys_Get_keystutus(int types)
{
	if(byKeyState[types]&0x80)
	{
		return true;
	}
	return false;
}

