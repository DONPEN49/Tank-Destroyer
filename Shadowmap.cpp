#include "Shadowmap.h"
#include "DX11util.h"
#include "DX11Settransform.h"
#include "shaderhashmap.h"
#include "CModel.h"
#include "ModelMgr.h"

// 頂点シェーダー
const char* vsshadowfilename[] = {
	"shader/vsshadowmap.hlsl",
	"shader/vsshadowmapsoft.hlsl",
};

// ピクセルシェーダー
const char* psshadowfilename[] = {
	"shader/psshadowmap.hlsl",
	"shader/psshadowsoft.hlsl",
};


/// <summary>
/// シャドウマップ初期化
/// </summary>
/// <returns></returns>
bool ShadowMap::Init() {

	uvmatrix = XMMATRIX(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f
	);

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// ピクセルシェーダー生成
	for (int i = 0; i < 2; i++) {
		bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psshadowfilename[i]);
		if (!sts) {
			std::string f(psshadowfilename[i]);
			std::string errormessage;
			errormessage = f + "SetPixelShader error";
			MessageBox(nullptr, errormessage.c_str(), "error", MB_OK);
			return false;
		}
	}

	// 頂点シェーダー生成
	for (int i = 0; i < 2; i++) {
		bool sts = ShaderHashmap::GetInstance()->SetVertexShader(
			vsshadowfilename[i],			// 頂点シェーダーファイル名
			layout,					// 頂点レイアウト
			numElements);			// エレメント数
		if (!sts) {
			std::string f(vsshadowfilename[i]);
			std::string errormessage;
			errormessage = f + " " + "SetVertexShader error";
			MessageBox(nullptr, errormessage.c_str(), "error", MB_OK);
			return false;
		}
	}

	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	//for (int i = 0; i < SHADOWMAP_NUM; i++) {

	//	//深度マップテクスチャーを作成

	//	D3D11_TEXTURE2D_DESC desc;
	//	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	//	desc.Width = DEPTHTEX[i];
	//	desc.Height = DEPTHTEX[i];
	//	desc.MipLevels = 1;
	//	desc.ArraySize = 1;
	//	desc.Format = DXGI_FORMAT_R32_FLOAT;
	//	desc.CPUAccessFlags = 0;	// CPUからはアクセスしない
	//	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	//	desc.MiscFlags = 0;			// その他の設定なし
	//	desc.SampleDesc.Count = 1;
	//	desc.SampleDesc.Quality = 0;
	//	desc.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法

	//	// シェーダ リソース ビューの作成
	//	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	//	srDesc.Format = DXGI_FORMAT_R32_FLOAT;					// フォーマット
	//	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;		// 2Dテクスチャ
	//	srDesc.Texture2D.MostDetailedMip = 0;					// 最初のミップマップ レベル
	//	srDesc.Texture2D.MipLevels = -1;						// すべてのミップマップ レベル

	//	// 2Dテクスチャを生成
	//	HRESULT hr = device->CreateTexture2D(
	//		&desc,							// 作成する2Dテクスチャの設定
	//		nullptr,						// 
	//		&shadowState[i].DepthMapTex);					// 作成したテクスチャを受け取る変数
	//	if (FAILED(hr))
	//		MessageBox(nullptr, "CreateTexture error", "Error", MB_OK);

	//	// シェーダ リソース ビューの作成
	//	hr = device->CreateShaderResourceView(
	//		shadowState[i].DepthMapTex.Get(),			// アクセスするテクスチャ リソース
	//		&srDesc,						// シェーダ リソース ビューの設定
	//		&shadowState[i].DepthMapSRV);				// 受け取る変数
	//	if (FAILED(hr))
	//		MessageBox(nullptr, "SRV error", "Error", MB_OK);

	//	// レンダーターゲットビュー生成
	//	hr = device->CreateRenderTargetView(shadowState[i].DepthMapTex.Get(),
	//		nullptr,
	//		&shadowState[i].DepthMapRTV);
	//	if (FAILED(hr))
	//		MessageBox(nullptr, "RTV error", "Error", MB_OK);
	//}

	//深度マップテクスチャーを作成
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = DEPTHTEX_WIDTH;
		desc.Height = DEPTHTEX_HEIGHT;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.CPUAccessFlags = 0;	// CPUからはアクセスしない
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = 0;			// その他の設定なし
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法

		// シェーダ リソース ビューの作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
		srDesc.Format = DXGI_FORMAT_R32_FLOAT; // フォーマット

		srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;		// 2Dテクスチャ
		srDesc.Texture2D.MostDetailedMip = 0;					// 最初のミップマップ レベル
		srDesc.Texture2D.MipLevels = -1;						// すべてのミップマップ レベル

		// 2Dテクスチャを生成
		HRESULT hr = device->CreateTexture2D(
			&desc,							// 作成する2Dテクスチャの設定
			nullptr,						// 
			&DepthMapTex);				// 作成したテクスチャを受け取る変数
		if (FAILED(hr))
			MessageBox(nullptr, "CreateTexture error", "Error", MB_OK);

		// シェーダ リソース ビューの作成
		hr = device->CreateShaderResourceView(
			DepthMapTex.Get(),			// アクセスするテクスチャ リソース
			&srDesc,						// シェーダ リソース ビューの設定
			&DepthMapSRV);				// 受け取る変数
		if (FAILED(hr))
			MessageBox(nullptr, "SRV error", "Error", MB_OK);

		// レンダーターゲットビュー生成
		hr = device->CreateRenderTargetView(DepthMapTex.Get(),
			nullptr,
			&DepthMapRTV);
		if (FAILED(hr))
			MessageBox(nullptr, "RTV error", "Error", MB_OK);


		//デプスステンシルビュー用のテクスチャーを作成
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = DEPTHTEX_WIDTH;
		descDepth.Height = DEPTHTEX_HEIGHT;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;

		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		hr = device->CreateTexture2D(&descDepth, nullptr, &DSTex);
		if (FAILED(hr)) {
			MessageBox(nullptr, "CreateTexture2D error", "error", MB_OK);
			return false;
		}

		//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
		hr = device->CreateDepthStencilView(DSTex.Get(), nullptr, &DSTexDSV);
		if (FAILED(hr)) {
			MessageBox(nullptr, "CreateDepthStencilView error", "error", MB_OK);
			return false;
		}
	}

	// 定数バッファ生成
	bool sts = CreateConstantBufferWrite(
		device,
		sizeof(ConstantBufferShaderShadowmap),
		&ConstantBufferShadowmap);			// シャドウマップ用定数バッファ
}

/// <summary>
/// 描画処理
/// </summary>
void ShadowMap::RenderModel() {

	// ターゲットバッファクリア	
	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	//red,green,blue,alpha

	// デバイスコンテキスト取得
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// レンダリングターゲットビュー、デプスステンシルビューを設定
	ID3D11RenderTargetView* rtv[] = { CDirectXGraphics::GetInstance()->GetRenderTargetView() };
	devcontext->OMSetRenderTargets(1, rtv, CDirectXGraphics::GetInstance()->GetDepthStencilView());

	// ビューポートを設定
	D3D11_VIEWPORT vp;

	vp.Width = static_cast<float>(CDirectXGraphics::GetInstance()->GetViewPortWidth());
	vp.Height = static_cast<float>(CDirectXGraphics::GetInstance()->GetViewPortHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	devcontext->RSSetViewports(1, &vp);

	// ターゲットバッファクリア
	devcontext->ClearRenderTargetView(CDirectXGraphics::GetInstance()->GetRenderTargetView(), ClearColor);
	// Zバッファクリア
	devcontext->ClearDepthStencilView(CDirectXGraphics::GetInstance()->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 定数バッファを６番スロットにセット
	devcontext->PSSetConstantBuffers(6, 1, ConstantBufferShadowmap.GetAddressOf());
	devcontext->VSSetConstantBuffers(6, 1, ConstantBufferShadowmap.GetAddressOf());

	// depthmapをセット
	devcontext->PSSetShaderResources(1, 1, DepthMapSRV.GetAddressOf());

	ID3D11InputLayout* layout;
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsshadowfilename[MODEL]);
	ID3D11VertexShader* vsh;
	vsh = ShaderHashmap::GetInstance()->GetVertexShader(vsshadowfilename[MODEL]);
	ID3D11PixelShader* psh;
	psh = ShaderHashmap::GetInstance()->GetPixelShader(psshadowfilename[MODEL]);

	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(layout);
	// 頂点シェーダーをセット
	devcontext->VSSetShader(vsh, nullptr, 0);
	// ピクセルシェーダーをセット
	devcontext->PSSetShader(psh, nullptr, 0);

}

/// <summary>
/// 深度バッファを描画
/// </summary>
void ShadowMap::RenderDepthMap() {
	// ターゲットバッファクリア	
	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha

	// デバイスコンテキスト取得
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// レンダリングターゲットビュー、デプスステンシルビューを設定
	ID3D11RenderTargetView* rtv[] = { DepthMapRTV.Get() };
	devcontext->OMSetRenderTargets(1, rtv, DSTexDSV.Get());

	// ビューポートを設定
	D3D11_VIEWPORT vp;
	vp.Width = DEPTHTEX_WIDTH;
	vp.Height = DEPTHTEX_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	devcontext->RSSetViewports(1, &vp);

	// ターゲットバッファクリア
	devcontext->ClearRenderTargetView(DepthMapRTV.Get(), ClearColor);
	// Zバッファクリア
	devcontext->ClearDepthStencilView(DSTexDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 深度バッファを生成する為のシェーダー情報を取得
	ID3D11InputLayout* layout;
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsshadowfilename[SHADOW]);
	ID3D11VertexShader* vsh;
	vsh = ShaderHashmap::GetInstance()->GetVertexShader(vsshadowfilename[SHADOW]);
	ID3D11PixelShader* psh;
	psh = ShaderHashmap::GetInstance()->GetPixelShader(psshadowfilename[SHADOW]);

	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(layout);
	// 頂点シェーダーをセット
	devcontext->VSSetShader(vsh, nullptr, 0);
	// ピクセルシェーダーをセット
	devcontext->PSSetShader(psh, nullptr, 0);

	// カメラの位置
	XMFLOAT3 LightPos;
	/*static float angle = 0;*/
	LightPos.x = 400/* * cos(angle)*/;
	LightPos.z = 100/* * sin(angle)*/;
	LightPos.y = 500;

	XMFLOAT4 l;
	l.x = LightPos.x;
	l.y = LightPos.y;
	l.z = LightPos.z;
	l.w = 1.0f;
	GetLight().SetLightPos(l);
	GetLight().Update();

	/*angle += XM_PI / 180.0f;*/

	ALIGN16 XMVECTOR Eye = XMVectorSet(LightPos.x, LightPos.y, LightPos.z, 0.0f);
	ALIGN16 XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	ALIGN16 XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	ALIGN16 XMMATRIX camera;
	camera = XMMatrixTranspose(XMMatrixLookAtLH(Eye, At, Up));

	// ビュー変換行列をセット（光源位置からのカメラ）
	ConstantBufferShaderShadowmap cb;
	XMStoreFloat4x4(&cb.ViewFromLight, camera);

	// 光源カメラ用のプロジェクション変換行列
	float	nearclip = 1.0f;
	float	farclip = 1000.0f;	// ファークリップの値がおかしくてバグった
	float	Aspect = 1.0f;
	float	Fov = XM_PI / 3;

	ALIGN16 XMMATRIX ProjectionFromLight;

	ProjectionFromLight = XMMatrixTranspose(XMMatrixPerspectiveFovLH(Fov, Aspect, nearclip, farclip));
	XMStoreFloat4x4(&cb.ProjectionFromLight, ProjectionFromLight);		// プロジェクション変換行列をセット

	// スクリーン座標をＵＶ座標に変換する行列
	XMStoreFloat4x4(&cb.ScreenToUVCoord, XMMatrixTranspose(uvmatrix));

	// 定数バッファに反映させる
	D3D11_MAPPED_SUBRESOURCE pData;

	HRESULT hr = devcontext->Map(ConstantBufferShadowmap.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(ConstantBufferShaderShadowmap));
		devcontext->Unmap(ConstantBufferShadowmap.Get(), 0);
	}

	// 定数バッファを６番スロットにセット
	devcontext->PSSetConstantBuffers(6, 1, ConstantBufferShadowmap.GetAddressOf());
	devcontext->VSSetConstantBuffers(6, 1, ConstantBufferShadowmap.GetAddressOf());
}

void ShadowMap::SetShadowShader(ID3D11DeviceContext*& _device) {

	ID3D11InputLayout* layout;
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsshadowfilename[MODEL]);
	ID3D11VertexShader* vsh;
	vsh = ShaderHashmap::GetInstance()->GetVertexShader(vsshadowfilename[MODEL]);
	ID3D11PixelShader* psh;
	psh = ShaderHashmap::GetInstance()->GetPixelShader(psshadowfilename[MODEL]);

	// 頂点フォーマットをセット
	_device->IASetInputLayout(layout);
	// 頂点シェーダーをセット
	_device->VSSetShader(vsh, nullptr, 0);
	// ピクセルシェーダーをセット
	_device->PSSetShader(psh, nullptr, 0);
}

void ShadowMap::Release() {
	
}