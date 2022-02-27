#include "Exp_Billboard.h"
#include "Manager_Billboard.h"
#include "CCamera.h"

void Exp_Billboard::Init() {
	
	SetBillboard(Manager_Billboard::GetInstance().GetBillboardPtr("assets/Texture/Particle01.png"));
	SetSize(50.0f, 50.0f);
	SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	isDraw = true;
	idU = 0;
	idV = 0;
}

void Exp_Billboard::Update() {

}

void Exp_Billboard::Draw() {
	idU += ANIMESPEED;
	if (idU > 3) {
		idV++;
		idU = 0;
	}
	if (idV > 3) {
		idV = 0;
		isDraw = false;
	}

	float u[4] = { 0.25f * (int)idU, 0.25f * (int)idU, 0.25f * ((int)idU + 1), 0.25f * ((int)idU + 1) };
	float v[4] = { 0.25f * ((int)idV + 1), 0.25f * (int)idV,0.25f * ((int)idV + 1), 0.25f * (int)idV };
	SetUV(u, v);
	bil->DrawBillBoard(CCamera::GetInstance()->GetCameraMatrix());
}