#include "CLight.h"
#include "DX11util.h"

bool CLight::Init(DirectX::XMFLOAT3 eyepos, DirectX::XMFLOAT4 lightpos) {
	m_lightpos = lightpos;
	m_eyepos = eyepos;
	m_type = LightType::DIRECTIONAL;

	// コンスタントバッファ作成
	bool sts = CreateConstantBuffer(
		GetDX11Device(),				// デバイス
		sizeof(ConstantBufferLight),		// サイズ
		&m_pConstantBufferLight);			// コンスタントバッファ４
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(constant buffer Light) error", "Error", MB_OK);
		return false;
	}

	Update();

	return true;
}

void CLight::Update() {
	ConstantBufferLight		cb;

	cb.EyePos.w = 1.0;
	cb.EyePos.x = m_eyepos.x;
	cb.EyePos.y = m_eyepos.y;
	cb.EyePos.z = m_eyepos.z;

	cb.LightDirection.x = m_lightpos.x;
	cb.LightDirection.y = m_lightpos.y;
	cb.LightDirection.z = m_lightpos.z;
	cb.LightDirection.w = m_lightpos.w;

	cb.Ambient = m_ambient;

	// 視点位置に逆行列を掛けて変換する
	DirectX::XMFLOAT3 inveye;
	DX11Vec3MulMatrix(inveye, m_eyepos, m_invworldmtx);

	// 光の方向に逆行列を掛けて変換する
	DirectX::XMFLOAT3 invlight;
	DirectX::XMFLOAT3 lightdir;
	lightdir.x = m_lightpos.x;
	lightdir.y = m_lightpos.y;
	lightdir.z = m_lightpos.z;
	DX11Vec3MulMatrix(invlight, lightdir, m_invworldmtx);

	cb.invEyePos.x = inveye.x;
	cb.invEyePos.y = inveye.y;
	cb.invEyePos.z = inveye.z;
	cb.invEyePos.w = 1.0;

	cb.invLightDirection.x = invlight.x;
	cb.invLightDirection.y = invlight.y;
	cb.invLightDirection.z = invlight.z;
	cb.invLightDirection.w = 0.0f;

	GetDX11DeviceContext()->UpdateSubresource(m_pConstantBufferLight,
		0,
		nullptr,
		&cb,
		0, 0);

	// コンスタントバッファ4をｂ3レジスタへセット（頂点シェーダー用）
	GetDX11DeviceContext()->VSSetConstantBuffers(4, 1, &m_pConstantBufferLight);
	// コンスタントバッファ4をｂ3レジスタへセット(ピクセルシェーダー用)
	GetDX11DeviceContext()->PSSetConstantBuffers(4, 1, &m_pConstantBufferLight);

}