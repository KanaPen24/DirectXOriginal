/*=====ヘッダコメント=====
*@プログラム名	：Enemy.cpp
*@説明・概要	：敵の挙動について
*@製作者	：吉田叶聖
*@更新内容	：2022/06/24
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#include "Enemy.h"
#include "Player.h"
#include "Game.h"
#include "Debugproc.h"
#include "Fade.h"
#include "Sound.h"
#include "Input.h"

const float VALUE_MOVE_ENEMY = 1.0f;		// 移動量
const float RATE_ROTATE_ENEMY = 0.20f;		// 回転慣性係数
const float VALUE_ROTATE_ENEMY = 7.0f;		// 回転速度
const float SCALE = 10.0f;		// 大きさ
const float MAX_VOLUME = 1.0f;	// 最大音量

//グローバル変数
bool g_bAlflg = true;

// コンストラクタ
CEnemy::CEnemy(CScene* pScene) : CModel(pScene)
{
	m_id = GOT_ENEMY;
	m_pPlayer = nullptr;
	
}

// デストラクタ
CEnemy::~CEnemy()
{
}

// 初期化
HRESULT CEnemy::Init()
{
	HRESULT hr = CModel::Init();
	SetModel(MODEL_ENEMY);
	SetScale(XMFLOAT3(SCALE, SCALE, SCALE));
	SetPos(XMFLOAT3(-400.0f + (float)(rand() % 400), 0.0f, (float)(rand() % 900)));
	
	return hr;
}

// 終了処理
void CEnemy::Fin()
{
	CModel::Fin();
}

// 更新
void CEnemy::Update()
{
	//デバック用透明オンオフ
	if (CInput::GetKeyTrigger(VK_1))
		g_bAlflg = true;
	if (CInput::GetKeyTrigger(VK_2))
		g_bAlflg = false;

	float fChan = 0.0f;	//サウンドのLR調整用

	XMFLOAT3 pPos = m_pPlayer->GetPos();		//プレイヤーの座標取得
	// 現在位置取得
	XMFLOAT3 vPos = GetPos();
	// 回転
	XMFLOAT3 angle = GetAngle();

	// 位置を抽出、0クリア
	m_vPos.x = m_mWorld._41;
	m_vPos.y = m_mWorld._42;
	m_vPos.z = m_mWorld._43;
	m_mWorld._41 = 0.0f;
	m_mWorld._42 = 0.0f;
	m_mWorld._43 = 0.0f;
	// ローカルX軸、Z軸を抽出
	XMFLOAT3 vX(m_mWorld._11, m_mWorld._12, m_mWorld._13);
	XMFLOAT3 vZ(m_mWorld._31, m_mWorld._32, m_mWorld._33);
	// 回転
	XMMATRIX mRotate = XMMatrixIdentity();
	bool bRotate = false;
	// 目的の角度までの差分
	//float fDiffRotY = g_EnemyRotDest.y - g_EnemyRot.y;

	// プレイヤーを追尾する
	if (pPos.x  > vPos.x + 1.0f) {
		vPos.x += VALUE_MOVE_ENEMY;
		fChan = -1.0f;
	}
	else if (pPos.x  < vPos.x - 1.0f)
	{
		vPos.x += -VALUE_MOVE_ENEMY;
		fChan = 1.0f;
	}
	else
	{
		//x軸が重なるときにぶるぶるするのを防ぐため
		fChan = 0.0f;
		vPos.x = vPos.x;
	}
	if (pPos.z > vPos.z)
	{
		vPos.z += VALUE_MOVE_ENEMY;
	}
	else
	{
		vPos.z += -VALUE_MOVE_ENEMY;
	}

	// 目的の角度まで慣性をかける
	// 角度更新
	SetAngle(angle);

	// 移動
	m_vPos.x = vPos.x;
	m_vPos.y = vPos.y;
	m_vPos.z = vPos.z;
	//位置を反映
	m_mWorld._41 = m_vPos.x;
	m_mWorld._42 = m_vPos.y;
	m_mWorld._43 = m_vPos.z;

	SetPos(m_vPos);

	//距離のよってサウンド調整をする
	float x, z, volume;
	x = m_vPos.x - pPos.x;
	z = m_vPos.z - pPos.z;
	volume = x * x + z * z;
	sqrt(volume);
	volume = volume / 50000;
	volume = MAX_VOLUME - volume;
	volume = std::fmax(volume, 0.15f);
	volume = std::fmin(volume, 1.0f);
	CSound::SetVolume(BGM_GAME, volume, fChan);

	// 衝突判定
	if (CollisionBSphere(m_pPlayer)) {
		//CFade::CutOut(SCENE_GAMEOVER);
		//CSound::SetVolume(SE_DAMAGE, 0.2f);
		//CSound::Play(SE_DAMAGE);
	}

#ifdef _DEBUG
	CDebugProc::Print("[ﾃｷ ｲﾁ : (%f, %f, %f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	CDebugProc::Print("[Vol : (%f)\n", volume);
#endif
}

//描画
void CEnemy::Draw()
{
	if (g_bAlflg) {
		ID3D11Device* pDevice = GetDevice();
		CAssimpModel::InitShader(pDevice, 0);
		CModel::Draw();
	}
}
