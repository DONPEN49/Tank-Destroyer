#include "bullet.h"
#include "Game_Manager.h"
#include "CModel.h"
#include "Sphere.h"
#include "DX11util.h"
#include "DX11Settransform.h"
#include "BoundingSphere_Manager.h"

using namespace DirectX;

constexpr float SPEED = 10.0f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="_mtx"> ���˂����I�u�W�F�N�g�̍s�� </param>
/// <param name="_type"> �N���甭�˂��ꂽ�� </param>
Bullet_Object::Bullet_Object(XMFLOAT4X4 _mtx, TYPEOBJ _type)
	: sts(Bullet_Object::STATUS::LIVE)
	, speed(SPEED)
	, life(100)
	, type(_type)
	, weight(0)
	, radius(5.0f)
{
	//���˂����I�u�W�F�N�g�̍s����Z�b�g
	SetMatrix(_mtx);
	//�ʒu�ݒ�
	SetPos(XMFLOAT3(mtx._41, mtx._42, mtx._43));
	//���a
	radius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/shot.fbx");
}

Bullet_Object::Bullet_Object()
	: sts(Bullet_Object::STATUS::LIVE)
	, speed(SPEED)
	, life(100)
	, type(TYPEOBJ::PLAYER)
	, weight(0)
	, radius(5.0f)
{
	//���˂����I�u�W�F�N�g�̍s����Z�b�g
	//�ʒu�ݒ�
	SetPos(XMFLOAT3(mtx._41, mtx._42, mtx._43));
	//���a
	radius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/shot.fbx");
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Bullet_Object::~Bullet_Object() {
	/*Finalize();*/
}

bool Bullet_Object::Init() {
	

	return sts;
}

/// <summary>
/// �`��
/// </summary>
void Bullet_Object::Draw() {

	if (sts == STATUS::LIVE) {
		model->DrawModel(mtx);
		
	}
}

/// <summary>
/// �X�V
/// </summary>
void Bullet_Object::Update() {

	this->life--;

	if (this->life > 0) {
		XMFLOAT4 axisZ;

		// Z�������o��
		axisZ.x = this->mtx._31;
		axisZ.y = this->mtx._32;
		axisZ.z = this->mtx._33;
		axisZ.w = 0.0f;

		this->pos.x += axisZ.x*speed;
		this->pos.y += axisZ.y*speed;
		this->pos.z += axisZ.z*speed;

		this->mtx._41 = this->pos.x;
		this->mtx._42 = this->pos.y;
		this->mtx._43 = this->pos.z;
	}
	else {

 		this->sts = STATUS::DEAD;
	}

}

/// <summary>
/// �㏈��
/// </summary>
void Bullet_Object::Finalize() {
	this->model->Uninit();
	
}