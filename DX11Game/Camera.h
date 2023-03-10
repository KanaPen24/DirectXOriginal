/*=====ヘッダコメント=====
*@プログラム名	：Camera.h
*@説明・概要	：カメラについて
*@製作者	：吉田叶聖
*@更新内容	：コメント追記
*@更新履歴	：2022/06/24
*@更新者	：吉田叶聖
*/
#pragma once
#include "Main.h"

class CAssimpModel;
class CCamera
{
protected:
	DirectX::XMFLOAT3 m_vPos;		// 視点
	DirectX::XMFLOAT3 m_vTarget;	// 注視点
	DirectX::XMFLOAT3 m_vUp;		// 上方ベクトル

	DirectX::XMFLOAT3 m_vAngle;		// カメラの角度
	DirectX::XMFLOAT3 m_vVec;		// カメラとターゲットのベクトル
	float m_fLengthInterval;		// カメラの視点と注視点の距離

private:
	float m_fAspectRatio;			// 縦横比
	float m_fFovY;					// 視野角(Degree)
	float m_fNearZ;					// 前方クリップ距離
	float m_fFarZ;					// 後方クリップ距離

	DirectX::XMFLOAT4X4 m_mtxWorld;	// ワールド マトリックス
	DirectX::XMFLOAT4X4 m_mtxView;	// ビュー マトリックス
	DirectX::XMFLOAT4X4 m_mtxProj;	// プロジェクション マトリックス

	CAssimpModel* m_pSky;			// スカイ ドーム

	static CCamera* m_pCamera;		// 現在のカメラ

public:
	CCamera();

	virtual void Init();
	virtual void Update();
	virtual void Clear();

	void SetPos(DirectX::XMFLOAT3& vPos) { m_vPos = vPos; }
	DirectX::XMFLOAT3& GetPos() { return m_vPos; }
	void SetPos(float x, float y, float z) { m_vPos.x = x, m_vPos.y = y, m_vPos.z = z; }
	void SetTarget(DirectX::XMFLOAT3& vTarget) { m_vTarget = vTarget; }
	void SetTarget(float x, float y, float z) { m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z; }
	void SetUpVector(DirectX::XMFLOAT3& vUp) { m_vUp = vUp; }
	void SetUpVector(float x, float y, float z) { m_vUp.x = x, m_vUp.y = y, m_vUp.z = z; }
	void SetWorldMatrix(DirectX::XMFLOAT4X4& mtxWorld);
	DirectX::XMFLOAT4X4& GetWorldMatrix() { return m_mtxWorld; }
	DirectX::XMFLOAT4X4& CalcWorldMatrix();
	void SetFovY(float fFovY) { m_fFovY = fFovY; }
	void SetAspectRatio(float fAspect) { m_fAspectRatio = fAspect; }
	void SetRangeZ(float fNearZ, float fFarZ) { m_fNearZ = fNearZ, m_fFarZ = fFarZ; }
	void UpdateMatrix();
	DirectX::XMFLOAT4X4& GetViewMatrix() { return m_mtxView; }
	DirectX::XMFLOAT4X4& GetProjMatrix() { return m_mtxProj; }
	DirectX::XMFLOAT3& GetAngle() { return m_vAngle; }
	DirectX::XMFLOAT3& GetVec() { return m_vVec; }
	float GetLength() { return m_fLengthInterval; }
	void SetSky(CAssimpModel* pSky) { m_pSky = pSky; }
	CAssimpModel* GetSky() { return m_pSky; }
	float GetFarZ() { return m_fFarZ; }

	static CCamera* Get() { return m_pCamera; }
	static void Set(CCamera* pCamera = nullptr);
};
