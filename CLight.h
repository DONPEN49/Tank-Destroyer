#pragma once
#include	<DirectXMath.h>
#include	"memory.h"
#include	"Shader.h"
#include    "dx11mathutil.h"

class CLight {
	ALIGN16 struct ConstantBufferLight {
		DirectX::XMFLOAT4 LightDirection;
		DirectX::XMFLOAT4 EyePos;
		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 invLightDirection;
		DirectX::XMFLOAT4 invEyePos;
	};

	enum class LightType {
		DIRECTIONAL,
		POINT,
		SPOT
	};
	LightType			m_type;
	DirectX::XMFLOAT3	m_eyepos;
	DirectX::XMFLOAT4	m_lightpos;		// w=0の時は方向 w=1の時は位置
	ID3D11Buffer*       m_pConstantBufferLight = nullptr;
	DirectX::XMFLOAT4	m_ambient;
	DirectX::XMFLOAT4X4 m_invworldmtx;		// ワールド変換行列の逆行列
public:
	bool Init(DirectX::XMFLOAT3 eyepos, DirectX::XMFLOAT4 lightpos);
		

	// ワールド変換行列を指定して
	void SetMtxWorld(const DirectX::XMFLOAT4X4& worldmtx) {

		// ワールド変換行列の逆行列を求める
		DX11MtxInverse(m_invworldmtx, worldmtx);

		Update();
	}

	void Update();

	void Uninit() {
		if (m_pConstantBufferLight) {
			m_pConstantBufferLight->Release();
			m_pConstantBufferLight = nullptr;
		}
	}

	void SetEyePos(DirectX::XMFLOAT3 eyepos) {
		m_eyepos = eyepos;
	}

	void SetLightPos(DirectX::XMFLOAT4 lightpos) {
		m_lightpos = lightpos;
	}

	void SetAmbient(DirectX::XMFLOAT4 amb) {
		m_ambient = amb;
	}
};