#include "RaderEne_UI.h"
#include "Texture2D_Manager.h"
#include "DX11util.h"


bool LaderEne_UI::Init() {

	SetQuad2D(Texture2D_Manager::GetInstance().GetTexture2DPtr("assets/texture/EPoint.png"));
	SetScale(3, 3);

	return true;
}

void LaderEne_UI::Update(XMFLOAT4X4 _playermtx) {

	drawflg = false;
	radius = 70.0f;
	float angle;					//�G�Ǝ��@�̊p�x
	float anglediff;

	XMFLOAT3 axis;		//��
	XMFLOAT2 raderpos;

	//���@�̕����x�N�g��
	DirectX::XMFLOAT3 vel = { _playermtx._31 * 2.0f, 0.0f, _playermtx._33 * 2.0f };

	//���@�ƓG�Ƃ̃x�N�g��
	DirectX::XMFLOAT3 dir = { enepos.x - _playermtx._41, 0.0f , enepos.z - _playermtx._43 };

	DirectX::XMFLOAT2 vel2d = { vel.x, vel.z };
	DirectX::XMFLOAT2 dir2d = { dir.x, dir.z };

	float lng = sqrtf(dir2d.x*dir2d.x + dir2d.y * dir2d.y);

	//���K��
	DX11Vec3Normalize(vel, vel);
	DX11Vec3Normalize(dir, dir);

	DX11Vec2Normalize(vel2d, vel2d);
	DX11Vec2Normalize(dir2d, dir2d);

	//�O��
	DX11Vec3Cross(axis, vel, dir);

	//���ς���cos�Ƃ��v�Z
	angle = vel2d.x*dir2d.x + vel2d.y*dir2d.y;

	//�A�[�Ncos���g���ă��W�A�����v�Z
	anglediff = acosf(angle);

	//���ɂ���Ċp�x���}�C�i�X�ɂ���
	if (axis.y > 0.0f) {
		anglediff = -anglediff;
	}

	if (lng < SERCHRANGE) {
		drawflg = true;

		//�����ɂ���Ĕ��a�ݒ�
		radius *= lng / SERCHRANGE;
	}

	//�O�x��y���ɂ��邽��cos��sin���t�ɂ���
	raderpos.x = centerpos.x + radius * sinf(anglediff);
	raderpos.y = centerpos.y + radius * cosf(anglediff);

	pos.x = raderpos.x;
	pos.y = raderpos.y;
	pos.z = 0;
}

void LaderEne_UI::Draw() {

	if (drawflg) {

		TurnOffZbuffer();

		XMFLOAT2 uv[4] = {
				{0.0f,0.0f},
				{1.0f,0.0f},
				{0.0f,1.0f},
				{1.0f,1.0f}
		};

		quad2d->SetPosition(pos.x, pos.y, pos.z);
		quad2d->updateVertex(width, height, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), uv);
		quad2d->updateVbuffer();

		quad2d->Draw();

		TurnOnZbuffer();
	}


}