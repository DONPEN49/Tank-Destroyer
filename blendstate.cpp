#include	<wrl/client.h>
#include	<d3d11.h>
#include	"DX11util.h"
#include	"blendstate.h"

using Microsoft::WRL::ComPtr;

// ブレンドステート
static ComPtr<ID3D11BlendState>	g_pBlendStateSrcAlpha;
static ComPtr<ID3D11BlendState>	g_pBlendStateOne;
static ComPtr<ID3D11BlendState>	g_pBlendStateDefault;
static ComPtr<ID3D11BlendState>	g_pBlendStateSubtract;

void InitBlendState() {
	CreateBlendStateSrcAlpha();
	// 加算合成ブレンドステート生成
	CreateBlendStateOne();
	// デフォルトブレンドステート生成
	CreateBlendStateDefault();
	// 減算合成ブレンドステート生成
	CreateBlendStateSubtract();
}

// ソースアルファブレンドステート生成
void CreateBlendStateSrcAlpha() {

	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// 配列の0番のみ使用

	BlendDesc.RenderTarget[0].BlendEnable = TRUE;

	// RGBカラー
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	// アルファ値
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11Device* dev;
	dev = GetDX11Device();

	dev->CreateBlendState(&BlendDesc, &g_pBlendStateSrcAlpha);
}

// 加算合成ブレンドステート生成
void CreateBlendStateOne() {

	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// 配列の0番のみ使用

	BlendDesc.RenderTarget[0].BlendEnable = TRUE;

	// RGB計算
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	// アルファ
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11Device* dev;
	dev = GetDX11Device();

	dev->CreateBlendState(&BlendDesc, &g_pBlendStateOne);
}

// デフォルトブレンドステート生成
void CreateBlendStateDefault() {

	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// 配列の0番のみ使用

	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11Device* dev;
	dev = GetDX11Device();

	dev->CreateBlendState(&BlendDesc, &g_pBlendStateDefault);
}

// 減算合成ブレンドステート生成
void CreateBlendStateSubtract() {

	D3D11_BLEND_DESC BlendDesc;

	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;		// 配列の0番のみ使用

	BlendDesc.RenderTarget[0].BlendEnable = TRUE;

	// RGBカラー
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;

	// アルファ値
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11Device* dev;
	dev = GetDX11Device();

	dev->CreateBlendState(&BlendDesc, &g_pBlendStateSubtract);
}

void SetBlendState(BLENDSTATE bs) {
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ID3D11DeviceContext* devcontext;
	devcontext = GetDX11DeviceContext();

	switch (bs) {
		case BLENDSTATE::DEFAULT:
			devcontext->OMSetBlendState(g_pBlendStateDefault.Get(), blendFactor, 0xffffffff);
			break;
		case BLENDSTATE::ADD:
			devcontext->OMSetBlendState(g_pBlendStateOne.Get(), blendFactor, 0xffffffff);
			break;
		case BLENDSTATE::SRCALPHA:
			devcontext->OMSetBlendState(g_pBlendStateSrcAlpha.Get(), blendFactor, 0xffffffff);
			break;
		case BLENDSTATE::SUB:
			devcontext->OMSetBlendState(g_pBlendStateSubtract.Get(), blendFactor, 0xffffffff);
			break;
	}
}

