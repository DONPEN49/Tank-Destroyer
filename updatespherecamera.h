#pragma once
#include	<DirectXMath.h>

void UpdateSphereCamera(
	float radius,					// 半径(入力)
	float elevation,				// 仰角（入力）
	float azimuth,					// 方位角（入力）
	const DirectX::XMFLOAT3& lookat);		// 注視点（入力）
