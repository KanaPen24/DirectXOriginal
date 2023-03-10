/*=====ヘッダコメント=====
*@プログラム名	：TitleSelect.cpp
*@説明・概要	：タイトルセレクトついて
*@製作者	：吉田叶聖
*@更新者	：吉田叶聖
*@更新内容	：初期制作
*@更新履歴	：2021/11/28
*/
//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "titleSelect.h"
#include "Input.h"
#include "Fade.h"
#include "Polygon.h"
#include "Texture.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
const int NUM_SELECT_MENU = 2;					// セレクトメニュー数
const float SELECT_MENU_WIDTH = 500.0f;			// セレクトメニュー幅
const float	SELECT_MENU_HEIGHT = 100.0f;		// セレクトメニュー高さ
const float SELECT_MENU_POS_X = 25.0f;			// セレクトメニュー位置(X座標)
const float SELECT_MENU_POS_Y = -150.0f;		// セレクトメニュー位置(Y座標)
const float SELECT_MENU_INTERVAL = 150.0f;		// セレクトメニュー間隔
const int GAMEPAD_LEFT_STICK_DEADZONE = 7800;	//左スティックのデッドゾーン
const int MAX_TEXTURE = 2;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTextures[MAX_TEXTURE] = { nullptr };	// テクスチャへのポインタ

static TITLE_SELECT g_nSelectTMenu = GAME;		// 選択中のメニューNo.
static float m_fCurve = 0.0f;
static float m_fCol = 0.0f;
static int g_texture = 0;
static bool m_bJoyStick;

static LPCWSTR c_aFileNameTitleMenu[MAX_TEXTURE] =
{
	L"data/texture/start.png",	// game
	L"data/texture/end.png",	// end
};

//=============================================================================
// ステージセレクト初期化処理
//=============================================================================
HRESULT CTSelect::Init(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	for (int nCntPauseMenu = 0; nCntPauseMenu < MAX_TEXTURE; ++nCntPauseMenu) {
		// テクスチャの読み込み
		hr = CreateTextureFromFile(pDevice,			// デバイスへのポインタ
			c_aFileNameTitleMenu[nCntPauseMenu],	// ファイルの名前
			&g_pTextures[nCntPauseMenu]);			// 読み込むメモリー
	}

	g_nSelectTMenu = GAME;
	m_fCurve = 0.0f;
	m_bJoyStick = false;

	// 効果音初期化
	//g_pSE_SHIZUKU = CreateSound(SE_SHIZUKU_PATH);
	//一時停止初期化
	return hr;
}

//=============================================================================
// ステージセレクト終了処理
//=============================================================================
void CTSelect::Uninit(void)
{
	// テクスチャの開放
	for (int nCntPauseMenu = 0; nCntPauseMenu < NUM_SELECT_MENU; ++nCntPauseMenu) {
		SAFE_RELEASE(g_pTextures[nCntPauseMenu]);
	}
}

//=============================================================================
// ステージセレクト更新処理
//=============================================================================
void CTSelect::Update(void)
{
	DWORD JoyCount = CInput::GetJoyCount();
	LONG JoyY = CInput::GetJoyY(JOYSTICKID1);
	//デッドゾーン以下ならすべて０にする
	if ((JoyY <  GAMEPAD_LEFT_STICK_DEADZONE &&
		JoyY > -GAMEPAD_LEFT_STICK_DEADZONE))
	{
		JoyY = 0;
	}
	if (JoyCount >= 1) {
		//ジョイスティックがうえ
		if (JoyY <= -GAMEPAD_LEFT_STICK_DEADZONE && m_bJoyStick == false){
				CSound::Play(SE_SHIZUKU);
			g_nSelectTMenu = (TITLE_SELECT)((g_nSelectTMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
			SetTitleSelect();
			m_bJoyStick = true;
		}
		//ジョイスティックが下
		if (JoyY >= GAMEPAD_LEFT_STICK_DEADZONE && m_bJoyStick == false){
				CSound::Play(SE_SHIZUKU);
			g_nSelectTMenu = (TITLE_SELECT)((g_nSelectTMenu + 1) % NUM_SELECT_MENU);
			SetTitleSelect();
			m_bJoyStick = true;
		}
		if (JoyY == 0l)
		{
			m_bJoyStick = false;
		}
	}
	if (CInput::GetKeyRepeat(VK_UP))
	{
		g_nSelectTMenu = (TITLE_SELECT)((g_nSelectTMenu + NUM_SELECT_MENU - 1) % NUM_SELECT_MENU);
		SetTitleSelect();
	}
	if (CInput::GetKeyRepeat(VK_DOWN))
	{
		g_nSelectTMenu = (TITLE_SELECT)((g_nSelectTMenu + 1) % NUM_SELECT_MENU);
		SetTitleSelect();
	}

	m_fCurve += XM_PI * 0.01f;
	if (m_fCurve > XM_PI) {
		m_fCurve -= XM_2PI;
	}

	// 反射光の設定
	m_fCol = cosf(m_fCurve) * 0.2f + 0.8f;

}

//=============================================================================
// ステージセレクト描画処理
//=============================================================================
void CTSelect::Draw(void)
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	CPolygon::SetTexture(nullptr);
	CPolygon::SetSize(SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT);
	for (int nCntTitleMenu = 0; nCntTitleMenu < NUM_SELECT_MENU; ++nCntTitleMenu) {
		CPolygon::SetPos(SELECT_MENU_POS_X, SELECT_MENU_POS_Y - nCntTitleMenu * SELECT_MENU_INTERVAL);

		if (nCntTitleMenu == g_nSelectTMenu) {
			//選んでいるメニューの方
			CPolygon::SetColor(1.0f, 0.0f, 0.0f, 1.0f);
			CPolygon::SetSize(SELECT_MENU_WIDTH + 30.f, SELECT_MENU_HEIGHT + 30.f);
		}
		else {
			//選ばれてないほう
			CPolygon::SetColor(0.3f, 0.0f, 0.0f, 0.5f);
			CPolygon::SetSize(SELECT_MENU_WIDTH, SELECT_MENU_HEIGHT);
		}
		// テクスチャの設定
		CPolygon::SetTexture(g_pTextures[nCntTitleMenu]);

		// ポリゴンの描画
		CPolygon::Draw(pDeviceContext);
	}
	CPolygon::SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// ステージセレクトメニューの設定
//=============================================================================
void CTSelect::SetTitleSelect(void)
{
	m_fCurve = 0.0f;
}

//=============================================================================
// ステージセレクトメニューの取得
//=============================================================================
TITLE_SELECT CTSelect::GetTitleSelect(void)
{
	return g_nSelectTMenu;
}

//=============================================================================
// ステージセレクトメニューのリセット
//=============================================================================
void CTSelect::ResetTitleSelect(void)
{
	g_nSelectTMenu = GAME;
	CSound::Play(SE_SHIZUKU);
	SetTitleSelect();
}
