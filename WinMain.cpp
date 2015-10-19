//-------------------------------------------------------------------------------
//Visual Sound Drama V3（VSDV3）
//メインソースコード
//Japan Relational Digital Service Network プログラム開発部
//担当者：時雨四季（info@jrds.net）
//DirectXvar=DirectX 9.0b SDK (2002)
//-------------------------------------------------------------------------------


#include <windows.h>
#include <mmsystem.h> //for timeGetTime()
#pragma comment( lib, "winmm.lib" )
#include "Renderer.h"
#include "Scene.h"
#include "main.h"
#include "GM_Debugmsystem.h"
const DWORD FPS = 60; // FPS設定
bool DebugMode=DEFDebug;
void Wait(DWORD); // ウェイト（FPS固定用）

Cdebugmode debugmode;
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//-------------------------------------------------------------
// アプリケーションのエントリポイント
// 引数
//		hInstance     : 現在のインスタンスのハンドル
//		hPrevInstance : 以前のインスタンスのハンドル
//		lpCmdLine	  : コマンドラインパラメータ
//		nCmdShow	  : ウィンドウの表示状態
// 戻り値
//		成功したら0以外の値
//-------------------------------------------------------------

bool Window_Activ=true;//0=非アクティブ　1=アクティブ



//本プログラム

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	AllocConsole();	//コンソール起動
	freopen("CONOUT$", "w", stdout); //標準出力先を指定
	freopen("CONIN$", "r", stdin); //同じく
	DebugText("Visual Sound Drama Game Engine V3\nProductBy.RikiAkasaka(Planetarium)\n");
	DebugText("------------------------------------------------\n");
	DebugText("DebugConsoleSystemV1\n");
	DebugText("------------------------------------------------\n");
	DebugText("CreateWindow...");
	// フルスクリーンにするかどうかの判定
	// コマンドラインに/fか/Fが設定されていたらフルスクリーンにする
	BOOL isFullScreen = Screenmode;
    for(int i = 0; i < nCmdShow; i++) {
		if(_stricmp((char*)&lpCmdLine[i], "/f") == 0) {	// コマンドラインに/fを発見
			isFullScreen = TRUE;	 // フラグをTRUEに設定
			break;
		}
    }

	char clsName[]		= "vsdv3";	// ウィンドウクラス名

	HWND		hWnd;

	MSG			msg;
	
	// ウィンドウクラスの初期化
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),				// この構造体のサイズ
		NULL,							// ウインドウのスタイル(default)
		WindowProc,						// メッセージ処理関数の登録
		0,								// 通常は使わないので常に0
		0,								// 通常は使わないので常に0
		hInstance,						// インスタンスへのハンドル
		NULL,							// アイコン（なし）
		LoadCursor(NULL, IDC_ARROW),	// カーソルの形
		NULL, NULL,						// 背景なし、メニューなし
		clsName,						// クラス名の指定
		NULL							// 小アイコン（なし）
	};


	// ウィンドウクラスの登録
	if(RegisterClassEx(&wcex) == 0){
		return 0;	// 登録失敗
	}
	
	// ウィンドウの作成
	if(isFullScreen) { // フルスクリーン
		int sw;
		int sh;
		// 画面全体の幅と高さを取得
		sw = GetSystemMetrics(SM_CXSCREEN);
		sh = GetSystemMetrics(SM_CYSCREEN);

		hWnd = CreateWindow( 
					clsName, 				// 登録されているクラス名
					WINDOW_NAME, 			// ウインドウ名
					WS_POPUP,				// ウインドウスタイル（ポップアップウインドウを作成）
					0, 						// ウインドウの横方向の位置
					0, 						// ウインドウの縦方向の位置
					CLIENT_WIDTH, 			// ウインドウの幅
					CLIENT_HEIGHT,			// ウインドウの高さ
					NULL,					// 親ウインドウのハンドル（省略）
					NULL,					// メニューや子ウインドウのハンドル
					hInstance, 				// アプリケーションインスタンスのハンドル
					NULL					// ウインドウの作成データ
				);
	}
	else {
		hWnd = CreateWindow(clsName, 
							WINDOW_NAME, 
							WS_OVERLAPPEDWINDOW &~WS_THICKFRAME &~WS_MAXIMIZEBOX,//高さ等の変更を出来ないようにする（消してあげる）
							CW_USEDEFAULT, CW_USEDEFAULT, 
							CW_USEDEFAULT, CW_USEDEFAULT,
							NULL, NULL, hInstance, NULL);

		// ウィンドウサイズを再設定する
		RECT rect;
		int ww, wh;
		int cw, ch;
		// クライアント領域の外の幅を計算
		GetClientRect(hWnd, &rect);		// クライアント部分のサイズの取得
		cw = rect.right - rect.left;	// クライアント領域外の横幅を計算
		ch = rect.bottom - rect.top;	// クライアント領域外の縦幅を計算

		// ウインドウ全体の横幅の幅を計算
		GetWindowRect(hWnd, &rect);		// ウインドウ全体のサイズ取得
		ww = rect.right - rect.left;	// ウインドウ全体の幅の横幅を計算
		wh = rect.bottom - rect.top;	// ウインドウ全体の幅の縦幅を計算
		ww = ww - cw;					// クライアント領域以外に必要な幅
		wh = wh - ch;					// クライアント領域以外に必要な高さ

		// ウィンドウサイズの再計算
		ww = CLIENT_WIDTH + ww;			// 必要なウインドウの幅
		wh = CLIENT_HEIGHT + wh;		// 必要なウインドウの高さ

		// ウインドウサイズの再設定
		SetWindowPos(hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE);

	}
	



	// ウィンドウの表示
    ShowWindow(hWnd, nCmdShow);
	DebugText("OK!\n");
	// WM_PAINTが呼ばれないようにする
	ValidateRect(hWnd, 0);
	DebugText("DirectX Initialize...");
	// レンダラーオブジェクト
	Renderer renderer;	

	// レンダラーの初期化
	HRESULT hr;
	hr = renderer.Initialize(hWnd, isFullScreen, CLIENT_WIDTH, CLIENT_HEIGHT);
	
	if(FAILED(hr)) {
		return 0;	// 初期化失敗
	}

	// シーンオブジェクト
	Scene	 scene;

	// シーンの生成
	if(FAILED(scene.Create(renderer.GetDevice()))) {
		return 0;
	}

	DWORD StartTime, EndTime, PassTime;
	//初期化
	// メッセージ処理および描画ループ（メイン）
	int FotmatF=0;
	DebugText("OK!\nStart...\n");
	while(TRUE) 
	{
		StartTime = timeGetTime();//描写前のシステム内部タイムを会得


        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if(msg.message == WM_QUIT) {	// PostQuitMessage()が呼ばれた
				break;	//ループの終了
			}
			else {
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else {	// 処理するメッセージが無いときは描画を行う

			// ウィンドウが見えている時だけ描画するための処理
			WINDOWPLACEMENT wndpl;
			GetWindowPlacement(hWnd, &wndpl);	// ウインドウの状態を取得
			if(FotmatF==0)
			{
				if(renderer.AddressHWND(&hWnd)!=0)//転送
				{
					//転送エラー
				}
				FotmatF=1;
			}
			if((wndpl.showCmd != SW_HIDE) && 
				(wndpl.showCmd != SW_MINIMIZE) &&
				(wndpl.showCmd != SW_SHOWMINIMIZED) &&
				(wndpl.showCmd != SW_SHOWMINNOACTIVE)) {
				
				// 描画処理の実行
			
				renderer.RenderScene(&scene);//ループ実行
				

			}
		}
		if(Window_Activ==false)//非アクティブウィンドウ
		{

			
		EndTime = timeGetTime();//描写後のタイムを会得
        PassTime = EndTime - StartTime; // 経過時間の計算
        (1000 / FPS > PassTime)? Wait(1000 / FPS - PassTime) :Wait(0); // 待つ。
		}
		else
		{
					//FPS

		EndTime = timeGetTime();//描写後のタイムを会得
        PassTime = EndTime - StartTime; // 経過時間の計算
        (1000 / FPS > PassTime)? Wait(1000 / FPS - PassTime) :Wait(0); // 待つ。
		}

	}


	return (int) msg.wParam;
}


//-------------------------------------------------------------
// メッセージ処理用コールバック関数
// 引数
//		hWnd	: ウィンドウハンドル
//		msg		: メッセージ
//		wParam	: メッセージの最初のパラメータ
//		lParam	: メッセージの2番目のパラメータ
// 戻り値
//		メッセージ処理結果
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
	case WM_CLOSE:				// ウインドウが閉じられた
		DebugText("CloseProgram...");
		PostQuitMessage(0);		// アプリケーションを終了する
		break;
	case WM_KEYDOWN:				// キーが押された
		if (wParam == VK_ESCAPE) {	// 押されたのはESCキーだ
			DebugText("PushEscKey.\nCloseProgram...");
			PostQuitMessage(0);		// アプリケーションを終了する
		}

		break;

	case WM_ACTIVATEAPP://ウィンドウアクティブ調査
		if (wParam == TRUE)	// アクティブ状態になった
		{
			Window_Activ=true;
		}
		else // 非アクティブ状態になった
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
//void wait関数
//FPSを一定に保つための関数。sleepによりCPU使用率を最小減に押さえます。
//----------------------------------------------------------------------------
void Wait(DWORD wait_time){
    MSG msg;
    DWORD start_time = timeGetTime();

    do{
        // メッセージ処理
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
		}

        if(wait_time > 0) Sleep(1); // ちょっと休憩（CPUの占有率を下げるため）
	}while(timeGetTime() < wait_time + start_time); // wait_time だけ回る
}

