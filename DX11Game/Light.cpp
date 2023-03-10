/*=====ヘッダコメント=====
*@プログラム名	：Light.cpp
*@説明・概要	：光源やライトについて
*@製作者	：吉田叶聖
*@更新内容	：2022/06/04
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#include "Light.h"
#include "Input.h"
#include "DebugProc.h"

using namespace DirectX;

// グローバル変数
namespace {
	const XMFLOAT3 LIGHT0_DIRECTION = XMFLOAT3(0.0f, -1.0f, 0.0f);			//方向
	const XMFLOAT4 LIGHT0_DIFFUSE = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);	//拡散
	const XMFLOAT4 LIGHT0_AMBIENT = XMFLOAT4(0.01f, 0.01f, 0.01f, 1.0f);	//周囲
	const XMFLOAT4 LIGHT0_SPECULAR = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);		//鏡面反射
	const float ROTATE = 0.00024f;		// 回転
	CLight g_light;
};

// 静的メンバ変数
CLight* CLight::m_pLight = &g_light;

// コンストラクタ
CLight::CLight()
{
	Init();
}

// 初期化
void CLight::Init()
{
	XMFLOAT3 vDir = LIGHT0_DIRECTION;
	XMStoreFloat3(&m_direction, XMVector3Normalize(XMLoadFloat3(&vDir)));
	m_diffuse = LIGHT0_DIFFUSE;
	m_ambient = LIGHT0_AMBIENT;
	m_specular = LIGHT0_SPECULAR;
	m_bEnable = true;
	m_fAngle = 0.0f;
}

// 光源方向取得
XMFLOAT3& CLight::GetDir()
{
	//シーンで挙動を変える
	if (CInput::GetKeyPress(VK_J))
		m_fAngle += ROTATE;
	if (CInput::GetKeyPress(VK_L))
		m_fAngle -= ROTATE;
		
		m_direction = XMFLOAT3(sin(m_fAngle), -1.0f, cos(m_fAngle));
	
	if (m_bEnable) return m_direction;
	static XMFLOAT3 off(0.0f, 0.0f, 0.0f);
	return off;

}

// 光源インスタンス取得
CLight* CLight::Get()
{
	return m_pLight;
}

// 光源インスタンス設定
void CLight::Set(CLight* pLight)
{
	m_pLight = (pLight) ? pLight : &g_light;
}
