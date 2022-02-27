#pragma once
#include	<directxmath.h>

// 頂点
struct Vertex {
	DirectX::XMFLOAT3 m_Pos;		// 位置
	DirectX::XMFLOAT3 m_Normal;		// 法線
	DirectX::XMFLOAT2 m_Tex;		// UV座標
};

// マテリアル
struct Material {
	float m_Alpha;
	DirectX::XMFLOAT3 m_Diffuse;
	DirectX::XMFLOAT3 m_Specular;
	float	m_SpecularPower;
	DirectX::XMFLOAT3 m_Ambient;
	DirectX::XMFLOAT3 m_Emissive;

	Material()
		: m_Alpha(0),
		m_Diffuse(DirectX::XMFLOAT3(0, 0, 0)),
		m_Specular(DirectX::XMFLOAT3(0, 0, 0)),
		m_SpecularPower(0),
		m_Ambient(DirectX::XMFLOAT3(0, 0, 0)),
		m_Emissive(DirectX::XMFLOAT3(0, 0, 0))
	{
	}
};

// 定数バッファ定義（マテリアル）
struct ConstantBufferMaterial {
	DirectX::XMFLOAT4	AmbientMaterial;		// 環境光の反射率
	DirectX::XMFLOAT4	DiffuseMaterial;		// ディフューズ光の反射率
	DirectX::XMFLOAT4	SpecularMaterial;		// スペキュラ光の反射率
};