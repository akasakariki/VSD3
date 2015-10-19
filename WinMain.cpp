//-------------------------------------------------------------------------------
//Visual Sound Drama V3�iVSDV3�j
//���C���\�[�X�R�[�h
//Japan Relational Digital Service Network �v���O�����J����
//�S���ҁF���J�l�G�iinfo@jrds.net�j
//DirectXvar=DirectX 9.0b SDK (2002)
//-------------------------------------------------------------------------------


#include <windows.h>
#include <mmsystem.h> //for timeGetTime()
#pragma comment( lib, "winmm.lib" )
#include "Renderer.h"
#include "Scene.h"
#include "main.h"
#include "GM_Debugmsystem.h"
const DWORD FPS = 60; // FPS�ݒ�
bool DebugMode=DEFDebug;
void Wait(DWORD); // �E�F�C�g�iFPS�Œ�p�j

Cdebugmode debugmode;
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//-------------------------------------------------------------
// �A�v���P�[�V�����̃G���g���|�C���g
// ����
//		hInstance     : ���݂̃C���X�^���X�̃n���h��
//		hPrevInstance : �ȑO�̃C���X�^���X�̃n���h��
//		lpCmdLine	  : �R�}���h���C���p�����[�^
//		nCmdShow	  : �E�B���h�E�̕\�����
// �߂�l
//		����������0�ȊO�̒l
//-------------------------------------------------------------

bool Window_Activ=true;//0=��A�N�e�B�u�@1=�A�N�e�B�u



//�{�v���O����

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	AllocConsole();	//�R���\�[���N��
	freopen("CONOUT$", "w", stdout); //�W���o�͐���w��
	freopen("CONIN$", "r", stdin); //������
	DebugText("Visual Sound Drama Game Engine V3\nProductBy.RikiAkasaka(Planetarium)\n");
	DebugText("------------------------------------------------\n");
	DebugText("DebugConsoleSystemV1\n");
	DebugText("------------------------------------------------\n");
	DebugText("CreateWindow...");
	// �t���X�N���[���ɂ��邩�ǂ����̔���
	// �R�}���h���C����/f��/F���ݒ肳��Ă�����t���X�N���[���ɂ���
	BOOL isFullScreen = Screenmode;
    for(int i = 0; i < nCmdShow; i++) {
		if(_stricmp((char*)&lpCmdLine[i], "/f") == 0) {	// �R�}���h���C����/f�𔭌�
			isFullScreen = TRUE;	 // �t���O��TRUE�ɐݒ�
			break;
		}
    }

	char clsName[]		= "vsdv3";	// �E�B���h�E�N���X��

	HWND		hWnd;

	MSG			msg;
	
	// �E�B���h�E�N���X�̏�����
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),				// ���̍\���̂̃T�C�Y
		NULL,							// �E�C���h�E�̃X�^�C��(default)
		WindowProc,						// ���b�Z�[�W�����֐��̓o�^
		0,								// �ʏ�͎g��Ȃ��̂ŏ��0
		0,								// �ʏ�͎g��Ȃ��̂ŏ��0
		hInstance,						// �C���X�^���X�ւ̃n���h��
		NULL,							// �A�C�R���i�Ȃ��j
		LoadCursor(NULL, IDC_ARROW),	// �J�[�\���̌`
		NULL, NULL,						// �w�i�Ȃ��A���j���[�Ȃ�
		clsName,						// �N���X���̎w��
		NULL							// ���A�C�R���i�Ȃ��j
	};


	// �E�B���h�E�N���X�̓o�^
	if(RegisterClassEx(&wcex) == 0){
		return 0;	// �o�^���s
	}
	
	// �E�B���h�E�̍쐬
	if(isFullScreen) { // �t���X�N���[��
		int sw;
		int sh;
		// ��ʑS�̂̕��ƍ������擾
		sw = GetSystemMetrics(SM_CXSCREEN);
		sh = GetSystemMetrics(SM_CYSCREEN);

		hWnd = CreateWindow( 
					clsName, 				// �o�^����Ă���N���X��
					WINDOW_NAME, 			// �E�C���h�E��
					WS_POPUP,				// �E�C���h�E�X�^�C���i�|�b�v�A�b�v�E�C���h�E���쐬�j
					0, 						// �E�C���h�E�̉������̈ʒu
					0, 						// �E�C���h�E�̏c�����̈ʒu
					CLIENT_WIDTH, 			// �E�C���h�E�̕�
					CLIENT_HEIGHT,			// �E�C���h�E�̍���
					NULL,					// �e�E�C���h�E�̃n���h���i�ȗ��j
					NULL,					// ���j���[��q�E�C���h�E�̃n���h��
					hInstance, 				// �A�v���P�[�V�����C���X�^���X�̃n���h��
					NULL					// �E�C���h�E�̍쐬�f�[�^
				);
	}
	else {
		hWnd = CreateWindow(clsName, 
							WINDOW_NAME, 
							WS_OVERLAPPEDWINDOW &~WS_THICKFRAME &~WS_MAXIMIZEBOX,//�������̕ύX���o���Ȃ��悤�ɂ���i�����Ă�����j
							CW_USEDEFAULT, CW_USEDEFAULT, 
							CW_USEDEFAULT, CW_USEDEFAULT,
							NULL, NULL, hInstance, NULL);

		// �E�B���h�E�T�C�Y���Đݒ肷��
		RECT rect;
		int ww, wh;
		int cw, ch;
		// �N���C�A���g�̈�̊O�̕����v�Z
		GetClientRect(hWnd, &rect);		// �N���C�A���g�����̃T�C�Y�̎擾
		cw = rect.right - rect.left;	// �N���C�A���g�̈�O�̉������v�Z
		ch = rect.bottom - rect.top;	// �N���C�A���g�̈�O�̏c�����v�Z

		// �E�C���h�E�S�̂̉����̕����v�Z
		GetWindowRect(hWnd, &rect);		// �E�C���h�E�S�̂̃T�C�Y�擾
		ww = rect.right - rect.left;	// �E�C���h�E�S�̂̕��̉������v�Z
		wh = rect.bottom - rect.top;	// �E�C���h�E�S�̂̕��̏c�����v�Z
		ww = ww - cw;					// �N���C�A���g�̈�ȊO�ɕK�v�ȕ�
		wh = wh - ch;					// �N���C�A���g�̈�ȊO�ɕK�v�ȍ���

		// �E�B���h�E�T�C�Y�̍Čv�Z
		ww = CLIENT_WIDTH + ww;			// �K�v�ȃE�C���h�E�̕�
		wh = CLIENT_HEIGHT + wh;		// �K�v�ȃE�C���h�E�̍���

		// �E�C���h�E�T�C�Y�̍Đݒ�
		SetWindowPos(hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE);

	}
	



	// �E�B���h�E�̕\��
    ShowWindow(hWnd, nCmdShow);
	DebugText("OK!\n");
	// WM_PAINT���Ă΂�Ȃ��悤�ɂ���
	ValidateRect(hWnd, 0);
	DebugText("DirectX Initialize...");
	// �����_���[�I�u�W�F�N�g
	Renderer renderer;	

	// �����_���[�̏�����
	HRESULT hr;
	hr = renderer.Initialize(hWnd, isFullScreen, CLIENT_WIDTH, CLIENT_HEIGHT);
	
	if(FAILED(hr)) {
		return 0;	// ���������s
	}

	// �V�[���I�u�W�F�N�g
	Scene	 scene;

	// �V�[���̐���
	if(FAILED(scene.Create(renderer.GetDevice()))) {
		return 0;
	}

	DWORD StartTime, EndTime, PassTime;
	//������
	// ���b�Z�[�W��������ѕ`�惋�[�v�i���C���j
	int FotmatF=0;
	DebugText("OK!\nStart...\n");
	while(TRUE) 
	{
		StartTime = timeGetTime();//�`�ʑO�̃V�X�e�������^�C�����


        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if(msg.message == WM_QUIT) {	// PostQuitMessage()���Ă΂ꂽ
				break;	//���[�v�̏I��
			}
			else {
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else {	// �������郁�b�Z�[�W�������Ƃ��͕`����s��

			// �E�B���h�E�������Ă��鎞�����`�悷�邽�߂̏���
			WINDOWPLACEMENT wndpl;
			GetWindowPlacement(hWnd, &wndpl);	// �E�C���h�E�̏�Ԃ��擾
			if(FotmatF==0)
			{
				if(renderer.AddressHWND(&hWnd)!=0)//�]��
				{
					//�]���G���[
				}
				FotmatF=1;
			}
			if((wndpl.showCmd != SW_HIDE) && 
				(wndpl.showCmd != SW_MINIMIZE) &&
				(wndpl.showCmd != SW_SHOWMINIMIZED) &&
				(wndpl.showCmd != SW_SHOWMINNOACTIVE)) {
				
				// �`�揈���̎��s
			
				renderer.RenderScene(&scene);//���[�v���s
				

			}
		}
		if(Window_Activ==false)//��A�N�e�B�u�E�B���h�E
		{

			
		EndTime = timeGetTime();//�`�ʌ�̃^�C�����
        PassTime = EndTime - StartTime; // �o�ߎ��Ԃ̌v�Z
        (1000 / FPS > PassTime)? Wait(1000 / FPS - PassTime) :Wait(0); // �҂B
		}
		else
		{
					//FPS

		EndTime = timeGetTime();//�`�ʌ�̃^�C�����
        PassTime = EndTime - StartTime; // �o�ߎ��Ԃ̌v�Z
        (1000 / FPS > PassTime)? Wait(1000 / FPS - PassTime) :Wait(0); // �҂B
		}

	}


	return (int) msg.wParam;
}


//-------------------------------------------------------------
// ���b�Z�[�W�����p�R�[���o�b�N�֐�
// ����
//		hWnd	: �E�B���h�E�n���h��
//		msg		: ���b�Z�[�W
//		wParam	: ���b�Z�[�W�̍ŏ��̃p�����[�^
//		lParam	: ���b�Z�[�W��2�Ԗڂ̃p�����[�^
// �߂�l
//		���b�Z�[�W��������
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
	case WM_CLOSE:				// �E�C���h�E������ꂽ
		DebugText("CloseProgram...");
		PostQuitMessage(0);		// �A�v���P�[�V�������I������
		break;
	case WM_KEYDOWN:				// �L�[�������ꂽ
		if (wParam == VK_ESCAPE) {	// �����ꂽ�̂�ESC�L�[��
			DebugText("PushEscKey.\nCloseProgram...");
			PostQuitMessage(0);		// �A�v���P�[�V�������I������
		}

		break;

	case WM_ACTIVATEAPP://�E�B���h�E�A�N�e�B�u����
		if (wParam == TRUE)	// �A�N�e�B�u��ԂɂȂ���
		{
			Window_Activ=true;
		}
		else // ��A�N�e�B�u��ԂɂȂ���
		{
			Window_Activ=false;
		}
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
//----------------------------------------------------------------------------
//void wait�֐�
//FPS�����ɕۂ��߂̊֐��Bsleep�ɂ��CPU�g�p�����ŏ����ɉ������܂��B
//----------------------------------------------------------------------------
void Wait(DWORD wait_time){
    MSG msg;
    DWORD start_time = timeGetTime();

    do{
        // ���b�Z�[�W����
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
		}

        if(wait_time > 0) Sleep(1); // ������Ƌx�e�iCPU�̐�L���������邽�߁j
	}while(timeGetTime() < wait_time + start_time); // wait_time �������
}

