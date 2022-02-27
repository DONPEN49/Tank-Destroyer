#pragma once

#include "DX11util.h"
#include "Singleton.h"

class ShadowMap : public Singleton<ShadowMap>{

private:

	friend class Singleton<ShadowMap>;

	static constexpr int DEPTHTEX_WIDTH = 4096 * 4;
	static constexpr int DEPTHTEX_HEIGHT = 4096 * 4;

	static constexpr int SHADOWMAP_NUM = 3;

	static constexpr int DEPTHTEX[SHADOWMAP_NUM] = { 4096 * 4, 2048, 1024 };

	// シャドウマップ用定数バッファ
	struct ConstantBufferShaderShadowmap {
		DirectX::XMFLOAT4X4 ViewFromLight;				// 光源の位置カメラ
		DirectX::XMFLOAT4X4 ProjectionFromLight;		// 光源の位置カメラに対応したプロジェクション変換行列
		DirectX::XMFLOAT4X4 ScreenToUVCoord;			// スクリーン座標をテクスチャ座標空間に変換
	};

	struct ShadowState {
		ComPtr<ID3D11Texture2D>				DepthMapTex;					// テクスチャ
		ComPtr<ID3D11ShaderResourceView>	DepthMapSRV;					// シェーダーリソースビュー
		ComPtr<ID3D11RenderTargetView>		DepthMapRTV;					// レンダーターゲットビュー
		ComPtr<ID3D11Texture2D>*			DSTex;							// テクスチャ（DepthStencil）
		ComPtr<ID3D11DepthStencilView>*		DSTexDSV;						// ＤＳＶ
		ComPtr<ID3D11Buffer>*				ConstantBufferShadowmap;		// シャドウマップ用定数バッファ

	};

	// 頂点フォーマット定義
	ComPtr<ID3D11InputLayout>			pVertexLayout;

	ComPtr<ID3D11Texture2D>				DepthMapTex;		// テクスチャ
	ComPtr<ID3D11ShaderResourceView>	DepthMapSRV;		// シェーダーリソースビュー
	ComPtr<ID3D11RenderTargetView>		DepthMapRTV;		// レンダーターゲットビュー

	ComPtr<ID3D11Texture2D>				DSTex;				// テクスチャ（DepthStencil）
	ComPtr<ID3D11DepthStencilView>		DSTexDSV;			// ＤＳＶ

	ComPtr<ID3D11Buffer>				ConstantBufferShadowmap;		// シャドウマップ用定数バッファ

	//スクリーン座標をUV座標に変換する行列
	ALIGN16 XMMATRIX uvmatrix;

	/*float cascadeAreaTbl[SHADOWMAP_NUM];*/

	ShadowState shadowState[SHADOWMAP_NUM];

public:

	enum SHADERTYPE {
		SHADOW,
		MODEL,

	};

	bool Init();

	void RenderModel();

	void RenderDepthMap();

	void SetShadowShader(ID3D11DeviceContext*&);

	void Release();
};