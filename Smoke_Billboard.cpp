#include "Smoke_Billboard.h"
#include "Manager_Billboard.h"
#include "CCamera.h"

/// <summary>
/// コンストラクタ
/// </summary>
Smoke_Billboard::Smoke_Billboard()
	:speed(0.05f)
	, isDraw(true)
	, idU(0)
{

}

Smoke_Billboard::Smoke_Billboard(XMFLOAT3 _pos)
	:speed(0.05f)
	, pos(_pos)
	, isDraw(true)
	, idU(0)
{

}


/// <summary>
/// 初期化
/// </summary>
void Smoke_Billboard::Init() {

	SetBillboard(Manager_Billboard::GetInstance().GetBillboardPtr("assets/Texture/tanksmoke.png"));
	SetSize(5.0f, 5.0f);
	SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

}


/// <summary>
/// 更新
/// </summary>
void Smoke_Billboard::Update() {

	float move;

	move = speed;

	pos.y += speed;

	SetPos(pos);

}

/// <summary>
/// 描画
/// </summary>
void Smoke_Billboard::Draw() {

	idU += ANIMESPEED;
	if (idU > 10) {
		idV++;
		idU = 0;
		isDraw = false;
	}


	if (isDraw) {
		float u[4] = { 0.1f * (int)idU, 0.1f * (int)idU, 0.1f * ((int)idU + 1), 0.1f * ((int)idU + 1) };
		float v[4] = { 1.0f, 0.0f,1.0f, 0.0f };
		SetUV(u, v);
		bil->DrawBillBoardAdd(CCamera::GetInstance()->GetCameraMatrix());
	}
}