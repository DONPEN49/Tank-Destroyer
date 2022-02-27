#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"Shader.h"

class CQuad {
private:
	ID3D11Buffer*		m_pConstantBuffer;	// 定数バッファ
	ID3D11Buffer*		m_pVertexBuffer;	// 頂点バッファ
	ID3D11Buffer*		m_pIndexBuffer;		// インデックスバッファ
	ID3D11VertexShader* m_pVertexShader;	// 頂点シェーダー
	ID3D11PixelShader*  m_pPixelShader;		// ピクセルシェーダー
	ID3D11InputLayout*  m_pVertexLayout;	// 頂点レイアウト

	struct Vertex{
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT2	TexCoord;
	};

	Vertex				m_Vertex[4];		// 頂点座標＊４

	void SetVertexData(float x, float y, float width, float height) {
		// 頂点座標セット
		m_Vertex[0].Pos.x = x;
		m_Vertex[0].Pos.y = y;
		m_Vertex[0].Pos.z = 0.0f;
		m_Vertex[0].TexCoord.x = 0.0f;
		m_Vertex[0].TexCoord.y = 0.0f;

		m_Vertex[1].Pos.x = x + width;
		m_Vertex[1].Pos.y = y;
		m_Vertex[1].Pos.z = 0.0f;
		m_Vertex[1].TexCoord.x = 1.0f;
		m_Vertex[1].TexCoord.y = 0.0f;

		m_Vertex[2].Pos.x = x;
		m_Vertex[2].Pos.y = y - height;
		m_Vertex[2].Pos.z = 0.0f;
		m_Vertex[2].TexCoord.x = 0.0f;
		m_Vertex[2].TexCoord.y = 1.0f;

		m_Vertex[3].Pos.x = x + width;
		m_Vertex[3].Pos.y = y - height;
		m_Vertex[3].Pos.z = 0.0f;
		m_Vertex[3].TexCoord.x = 1.0f;
		m_Vertex[3].TexCoord.y = 1.0f;
	}

public:
	void ChangePosSize(float x, float y, float width, float height, ID3D11DeviceContext* deviceContext){
		D3D11_MAPPED_SUBRESOURCE pData;

		HRESULT hr = deviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			SetVertexData(x, y, width, height);
			memcpy_s(pData.pData, pData.RowPitch, (void*)(m_Vertex), sizeof(Vertex)*4);
			deviceContext->Unmap(m_pVertexBuffer, 0);
		}
	}
	void Draw(ID3D11DeviceContext* device) {

		// 頂点バッファをセットする
		unsigned int stride = sizeof(Vertex);
		unsigned  offset = 0;
		device->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		device->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);			// インデックスバッファをセット
		device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);		// トポロジーをセット（旧プリミティブタイプ）

		// 頂点レイアウトセット
		device->IASetInputLayout(m_pVertexLayout);

		device->VSSetShader(m_pVertexShader, nullptr, 0);		// 頂点シェーダーをセット
		device->PSSetShader(m_pPixelShader, nullptr, 0);		// ピクセルシェーダーをセット

		device->DrawIndexed(4,									// 描画するインデックス数(4)
								0,								// 最初のインデックスバッファの位置
								0);								// 頂点バッファの最初から使う
	}

	bool Init(float x,float y,float width ,float height,ID3D11Device* device) {

		bool sts;

		// 頂点座標セット	
		SetVertexData(x, y, width, height);

		// 頂点バッファ作成
		sts = CreateVertexBufferWrite(
			device,
			sizeof(Vertex),			// １頂点当たりバイト数
			4,						// 頂点数
			m_Vertex,				// 頂点データ格納メモリ先頭アドレス
			&m_pVertexBuffer);		// 頂点バッファ
		if (!sts) {
			MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
			return false;
		}

		unsigned int idx[4] = {0,1,2,3};

		// インデックスバッファ作成
		sts = CreateIndexBuffer(
			device,
			4,				// インデックス数
			idx,			// インデックスデータ先頭アドレス
			&m_pIndexBuffer); // インデックスバッファ
		if (!sts) {
			MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
			return false;
		}

		// 頂点データの定義
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		unsigned int numElements = ARRAYSIZE(layout);

		// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
		sts = CreateVertexShader(device,
			"shader/quad.fx",
			"main",
			"vs_5_0",
			layout,
			numElements,
			&m_pVertexShader,
			&m_pVertexLayout);
		if (!sts) {
			MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
			return false;
		}

		// ピクセルシェーダーを生成
		sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
			device,							// デバイスオブジェクト
			"shader/quad.fx",
			"PS",
			"ps_5_0",
			&m_pPixelShader);
		if (!sts) {
			MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
			return false;
		}

		return true;
	}

	void Exit() {

		// ピクセルシェーダー解放
		if (m_pPixelShader) {
			m_pPixelShader->Release();
			m_pPixelShader = nullptr;
		}

		// 頂点シェーダー解放
		if (m_pVertexShader) {
			m_pVertexShader->Release();
			m_pVertexShader = nullptr;
		}

		if (m_pVertexLayout) {
			m_pVertexLayout->Release();
			m_pVertexLayout = nullptr;
		}

		if (m_pIndexBuffer) {
			m_pIndexBuffer->Release();
			m_pIndexBuffer = nullptr;
		}

		if (m_pVertexBuffer) {
			m_pVertexBuffer->Release();
			m_pVertexBuffer = nullptr;
		}

	}
};