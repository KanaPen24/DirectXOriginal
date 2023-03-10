/*=====ヘッダコメント=====
*@プログラム名	：Rader.h
*@説明・概要	：レーダーについて
*@製作者	：吉田叶聖
*@更新内容	：2023/03/06
*@更新履歴	：コメント追記
*@更新者	：吉田叶聖
*/
#pragma once
#include "main.h"
#include "Game.h"

enum {
	RT_RADAR,		// レーダー
	RT_BRIGHTSPOT,	// 輝点

	MAX_RT
};

class CRadar
{
private:
	static ID3D11ShaderResourceView* m_pTexture[MAX_RT];

	bool m_bVisible;
	XMFLOAT3 m_vPos[MAX_OBJECT];
	XMFLOAT3 m_vPos2D[MAX_OBJECT];
	CGame* m_pScene;
	int m_nObject;
	int m_nCntObj;
	CPlayer* m_pPlayer;

public:
	void Init(CGame* pScene);
	void Update();
	void Draw();

	static HRESULT LoadTexture();
	static void ReleaseTexture();
};
