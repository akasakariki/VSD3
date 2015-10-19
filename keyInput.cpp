#include "keyinput.h"
#include "Common.h"
//------------------------------------------------
//�L�[���̓C���^�[�t�F�C�X
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



    //IDirectInputDevice8�C���^�[�t�F�C�X�̎擾�i�L�[�{�[�h�j

    g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevice, NULL);

	g_pDIDevice->SetDataFormat(&c_dfDIKeyboard);
	g_pDIDevice->SetCooperativeLevel(*hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);


	//�X�^�[�g
	 g_pDIDevice->Acquire();
	 DebugText("OK!\n");
	 return 0;
}

void CKeyinput::Reload_keystatus(void)
{
	
	//�L�[�{�[�h�擾
	if(switch_key==true)
	g_pDIDevice->GetDeviceState( 256, byKeyState );
	//�}�E�X�|�C���^�[�擾�p
	if(switch_pointer==true)
	{
		GetCursorPos(&point);
		ScreenToClient(*copyhwnd, &point);
	}
}
//�X�N���v�^�擾�p�i�R���o�[�^�[�ʂ��āj
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

//�V�X�e���擾�p�i���ځj
bool CKeyinput::Sys_Get_keystutus(int types)
{
	if(byKeyState[types]&0x80)
	{
		return true;
	}
	return false;
}

