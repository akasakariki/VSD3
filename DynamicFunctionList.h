//-----------------------------------------------
//Function List (Dynamic)
//ここでは名詞_動詞とルールを変更
//-----------------------------------------------

//イメージレイヤー関係--------------------------------------------------------------------------------------
//レイヤシステム関係(ls)
#define		LAYER_SYS_S					"lss"						//描写処理開始地点（S→描写処理<dw>→Eとすることで、1フレームごとの一斉表示）
#define		LAYER_SYS_E					"lse"						//描写処理終了地点
#define		LAYER_SYS_G					"lsg"						//色属性モードを変更（Bright）
#define		LAYER_SYS_BM				"lsb"						//ブレンドモードを指定・変更
#define		LAYER_SYS_BMC				"lsbc"						//ブレンドモードを解除
#define		LAYER_SYS_CKEY				"lsk"						//カラーキーを設定（読み込み前に指定）

//ロード関係(ll)
#define		LAYER_LOAD_A				"lla"						//画像をレイヤーに通常読み込み
#define		LAYER_LOAD_M				"llm"						//画像をマスク処理
//削除関係(ld)
#define		LAYER_DELETE_AT				"ldt"						//指定レイヤー削除
#define		LAYER_DELETE_ALL			"lda"						//レイヤ全削除
//描写関係(lw)
#define		LAYER_DRAW_A				"lwa"						//画像、通常描写
#define		LAYER_DRAWR_S				"lws"						//拡大描写
#define		LAYER_DRAW_R				"lwr"						//回転描写
#define		LAYER_DRAW_SR				"lwsr"						//拡大回転描写
#define		LAYER_DRAW_B				"lwb"						//ブレンド描写


//サウンド関係(s)--------------------------------------------------------------------------------------
#define		SOUND_LOAD					"sl"						//サウンド読み込み
#define		SOUND_PLAY					"sp"						//サウンド再生
#define		SOUND_STOP_AT				"stt"						//個別サウンド一時停止
#define		SOUND_STOP_ALL				"sta"						//全サウンド一時停止
#define		SOUND_DELETE_AT				"sdt"						//個別サウンド削除
#define		SOUND_DELETE_ALL			"sda"						//全サウンド削除
#define		SOUND_VOLUME_AT				"sv"						//個別サウンドボリューム変更
#define		SOUND_GETVOLUME_AT			"svg"						//個別サウンドボリュームを取得
#define		SOUND_GETPLAYCHECK			"spg"						//個別サウンドの再生状態を取得

//テキスト関係(t)--------------------------------------------------------------------------------------
#define		TEXT_SETFONT				"tsf"						//フォント設定
#define		TEXT_DRAWTEXT				"twt"						//テキスト描写
#define		TEXT_DELETEFONT_AT			"tdft"						//フォント削除	
#define		TEXT_DELETEFONT_ALL			"tdfa"						//フォント全削除

//変数関係(v)--------------------------------------------------------------------------------------
#define		VARIABLE_MAKE				"vm"						//変数作成
#define		VARIABLE_CALILATION			"vc"						//変数を含めた計算