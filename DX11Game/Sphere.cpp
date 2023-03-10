/*=====ヘッダコメント=====
*@プログラム名	：Sphere.cpp
*@説明・概要	：球体について
*@製作者	：吉田叶聖
*@更新内容	：コメント追記
*@更新履歴	：2023/01/17
*@更新者	：吉田叶聖
*/
#include "Sphere.h"

// コンストラクタ
CSphere::CSphere() : CMesh()
{
}

// デストラクタ
CSphere::~CSphere()
{
}

// 初期化
HRESULT CSphere::Init(int nSlice, int nStack, float fRadius)
{
	HRESULT hr = S_OK;
	int nVertex = (nSlice + 1) * (nStack + 1);
	int nFace = nSlice * nStack * 2;
	VERTEX_3D* pVertexWk = new VERTEX_3D[nVertex];
	int* pIndexWk = new int[nFace * 3];
	// 頂点、法線ベクトル、テクスチャ座標の位置
	VERTEX_3D* pVtx = pVertexWk;
	for (int j = 0; j <= nStack; ++j) {
		float v = (float)j / (float)nStack;
		float ph = XM_PI * v;
		float y = cosf(ph);
		float r = sinf(ph);
		for (int i = 0; i <= nSlice; ++i) {
			float u = (float)i / (float)nSlice;
			float th = XM_2PI * u;
			float x = r * cosf(th);
			float z = r * sinf(th);
			pVtx->vtx.x = x * fRadius;
			pVtx->vtx.y = y * fRadius;
			pVtx->vtx.z = z * fRadius;
			pVtx->nor.x = x;
			pVtx->nor.y = y;
			pVtx->nor.z = z;
			pVtx->tex.x = u;
			pVtx->tex.y = v;
			pVtx->diffuse = XMFLOAT4(1, 1, 1, 1);
			++pVtx;
		}
	}
	// 面のインデックス
	int* pIdx = pIndexWk;
	for (int j = 0; j < nStack; ++j) {
		for (int i = 0; i < nSlice; ++i) {
			int nIndex = (nSlice + 1) * j + i;
			// 左下半分
			*pIdx++ = nIndex + (nSlice + 1);
			*pIdx++ = nIndex;
			*pIdx++ = nIndex + (nSlice + 1) + 1;
			// 右上半分
			*pIdx++ = nIndex + 1;
			*pIdx++ = nIndex + (nSlice + 1) + 1;
			*pIdx++ = nIndex;
		}
	}
	hr = CMesh::Init(pVertexWk, nVertex, pIndexWk, nFace * 3);
	delete[] pIndexWk;
	delete[] pVertexWk;
	return hr;
}
