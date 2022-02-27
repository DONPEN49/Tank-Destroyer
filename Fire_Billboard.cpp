#include "Fire_Billboard.h"
#include "Manager_Billboard.h"
#include "CCamera.h"

Fire_Billboard::Fire_Billboard() {

}

Fire_Billboard::Fire_Billboard(DirectX::XMFLOAT3 _pos)
	: idU(0)
	, idV(0)
	, isDraw(true)
{
	SetBillboard(Manager_Billboard::GetInstance().GetBillboardPtr("assets/Texture/explosion2.png"));
	SetPos(_pos);
	SetSize(13.0f, 13.0f);
	SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Fire_Billboard::Init() {

}

void Fire_Billboard::Update() {

}

void Fire_Billboard::Draw() {
	idU += ANIMESPEED;
	if (idU > 7) {
		idV++;
		idU = 0;
	}
	if (idV > 7) {
		idV = 0;
		isDraw = false;
	}

	if (isDraw) {
		float u[4] = { 0.125f * (int)idU, 0.125f * (int)idU, 0.125f * ((int)idU + 1), 0.125f * ((int)idU + 1) };
		float v[4] = { 0.1667f * ((int)idV + 1), 0.1667f * (int)idV,0.1667f * ((int)idV + 1), 0.1667f * (int)idV };
		SetUV(u, v);
		bil->DrawBillBoard(CCamera::GetInstance()->GetCameraMatrix());
	}
}