#include	"CBillBoard.h"
#include	"Shader.h"
#include	"dx11mathutil.h"
#include	"DX11Settransform.h"
#include	"blendstate.h"

// ビルボードの頂点座標を計算
void CBillBoard::CalcVertex() {
	m_Vertex[0].x = -m_XSize / 2;
	m_Vertex[0].y = -m_YSize / 2;
	m_Vertex[0].z = 0.0f;
	m_Vertex[0].color = m_Color;

	m_Vertex[1].x = -m_XSize / 2;
	m_Vertex[1].y = m_YSize / 2;
	m_Vertex[1].z = 0.0f;
	m_Vertex[1].color = m_Color;

	m_Vertex[2].x = m_XSize / 2;
	m_Vertex[2].y = -m_YSize / 2;
	m_Vertex[2].z = 0;
	m_Vertex[2].color = m_Color;

	m_Vertex[3].x = m_XSize / 2;
	m_Vertex[3].y = m_YSize / 2;
	m_Vertex[3].z = 0.0f;
	m_Vertex[3].color = m_Color;

	if (m_vbuffer == nullptr) {
		// 頂点バッファ作成（後で変更可能な）
		bool sts = CreateVertexBufferWrite(m_dev, sizeof(MyVertex), 4, m_Vertex, &m_vbuffer);
		if (!sts) {
			MessageBox(nullptr, "create vertex buffer erro(CBillBoard)", "error", MB_OK);
		}
	}
	else {
		// 作成済みなら 内容を書き換える 
		D3D11_MAPPED_SUBRESOURCE pData;

		HRESULT hr = m_devcontext->Map(m_vbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			memcpy_s(pData.pData, pData.RowPitch, (void*)(m_Vertex), sizeof(MyVertex) * 4);
			m_devcontext->Unmap(m_vbuffer, 0);
		}
	}
}

// サイズをセット
void CBillBoard::SetSize(float x, float y) {
	m_XSize = x;
	m_YSize = y;
	CalcVertex();
}

// カラー値をセット
void CBillBoard::SetColor(XMFLOAT4  col) {
	m_Color = col;
	CalcVertex();
}

// 位置をセット
void CBillBoard::SetPosition(float x, float y, float z) {
	m_x = x;
	m_y = y;
	m_z = z;
}

// ビルボード用の行列を生成
void CBillBoard::CalcBillBoardMatrix(const DirectX::XMFLOAT4X4& cameramat) {

	m_mat._11 = cameramat._11;
	m_mat._12 = cameramat._21;
	m_mat._13 = cameramat._31;

	m_mat._21 = cameramat._12;
	m_mat._22 = cameramat._22;
	m_mat._23 = cameramat._32;

	m_mat._31 = cameramat._13;
	m_mat._32 = cameramat._23;
	m_mat._33 = cameramat._33;

	m_mat._41 = m_x;
	m_mat._42 = m_y;
	m_mat._43 = m_z;

	m_mat._14 = 0;
	m_mat._24 = 0;
	m_mat._34 = 0;
	m_mat._44 = 1;
}

// 描画
void CBillBoard::Draw() {
	// Ｚバッファ無効化
	TurnOffZbuffer();

	// シェーダーリソースビューをピクセルシェーダーへセット
	m_devcontext->PSSetShaderResources(0, 1, &m_srv);

	// ワールド変換行列
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_mat);

	// 頂点バッファをセット
	unsigned int stride = sizeof(MyVertex);
	unsigned  offset = 0;

	// 頂点バッファをセット
	m_devcontext->IASetVertexBuffers(0, 1, &m_vbuffer, &stride, &offset);

	// トポロジーをセット
	m_devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点フォーマットをセット
	m_devcontext->IASetInputLayout(m_pVertexLayout);

	// 4角形描画
	m_devcontext->VSSetShader(m_pVertexShader, nullptr, 0);		// 頂点シェーダーをセット
	m_devcontext->PSSetShader(m_pPixelShader, nullptr, 0);		// ピクセルシェーダーをセット
	m_devcontext->Draw(4, 0);									// 4頂点描画(頂点バッファのみで描画)

	// Ｚバッファ有効化
	TurnOnZbuffer();
}

// ビルボードを描画
void CBillBoard::DrawBillBoard(const XMFLOAT4X4& cameramat) {

	// カメラ行列から　ビルボード用のマトリックスを作成
	CalcBillBoardMatrix(cameramat);

	SetBlendState(BLENDSTATE::SRCALPHA);

	// 描画
	Draw();

	SetBlendState(BLENDSTATE::SRCALPHA);
}

// ビルボードを描画
void CBillBoard::DrawBillBoardAdd(const XMFLOAT4X4& cameramat) {

	// カメラ行列から　ビルボード用のマトリックスを作成
	CalcBillBoardMatrix(cameramat);

	//アルファブレンディングをセットする
	SetBlendState(BLENDSTATE::ADD);

	// 描画
	Draw();

	SetBlendState(BLENDSTATE::SRCALPHA);

	//アルファブレンディングで半透明合成をセットする
}

// ビルボードを描画(Ｚ軸で回転)
void CBillBoard::DrawRotateBillBoard(const DirectX::XMFLOAT4X4 &cameramat, float angle) {

	// カメラ行列から　ビルボード用のマトリックスを作成
	CalcBillBoardMatrix(cameramat);

	// Ｚ軸回転行列を作成
	DirectX::XMFLOAT4X4 matRotZ;
	DirectX::XMFLOAT3 axisZ = { cameramat._13, cameramat._23, cameramat._33 };

	// 指定軸回転の行列を作成する
	DX11MtxRotationAxis(axisZ, angle, matRotZ);

	// 行列を合成し、位置をセット
	DX11MtxMultiply(m_mat, m_mat, matRotZ);
	m_mat._41 = m_x;
	m_mat._42 = m_y;
	m_mat._43 = m_z;

	//アルファブレンディングをセットする
//	SetBlendStateSrcAlpha();

	// 描画
	Draw();

	//アルファブレンディングをセットする
//	SetBlendStateSrcAlpha();

}

//	テクスチャ読み込み
bool CBillBoard::LoadTexTure(const char* filename) {

	// すでに読み込まれているテクスチャがあれば解放する
	if (m_srv != nullptr) {
		m_srv->Release();
		m_srv = nullptr;
	}

	// SRVを読み込む
	bool sts = CreateSRVfromFile(filename,
		m_dev, m_devcontext, &m_res, &m_srv);
	if (sts == false) {
		MessageBox(nullptr, "textureload error(CBillBoard LoadTexture)", "error", MB_OK);
		return false;
	}

	return true;
}