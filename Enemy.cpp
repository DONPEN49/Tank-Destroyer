#include "Enemy.h"

#include <random>
#include "tank.h"
#include "Game_Manager.h"
#include "Bullet_Manager.h"
#include "Stage_Manager.h"
#include "IScene.h"
#include "Player.h"
#include "Exp_Billboard.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Enemy::Enemy()
{
	life = 3;
	tank->type = Tank::TYPE::ENEMY;
	tank->speed = -0.4f;
	keepTime = 10;
	state = STATE::FREE;
	atkInterval = 10;
	render = RENDER::DEFAULTSHADOW;
}

Enemy::Enemy(IScene* _scene)
	:life(3)
	,state(STATE::FREE)
	,atkInterval(10)
	,keepTime(10)
{
	scene = _scene;
	tag = "Enemy";
	scene->AddSceneObject(this);
	render = RENDER::DEFAULTSHADOW;
}

/// <summary>
/// ������
/// </summary>
/// <returns></returns>
bool Enemy::Init() {

	tank = new Tank(Tank::TYPE::ENEMY);
	tank->Init();
	tank->speed = -0.4f;
	return true;
}

/// <summary>
/// �X�V
/// </summary>
void Enemy::Update() {

	auto player = scene->GetSceneObject<Player>("Player");
	UpdateEnemy(player->GetMatrix());
}

/// <summary>
/// �`��
/// </summary>
void Enemy::Draw() {
	tank->Draw();
}

void Enemy::DrawShadow() {
	tank->DrawShadow();
}

void Enemy::DrawModel() {
	tank->DrawModel();
}

/// <summary>
/// �GAI
/// </summary>
void Enemy::AutoPirot() {

	//��������
	std::uniform_int_distribution<int32_t> rand4(0, static_cast<int>(Action::MAX_ACTION) - 1);
	std::uniform_int_distribution<uint32_t> rand10(30, 50);


	tank->angle.x = tank->angle.y = tank->angle.z = 0.0f;
	//speed = 1.0f;

	
	//if (lng > RANGE_STAGE) {
	//	speed = 0.0f;
	//}

	//int no = Stage_Manager::GetInstance().GetMapChipNo(pos.x, pos.z);

	/*if (no == 9) {
		speed = 0.0f;
	}*/

	switch (action)
	{
	case Enemy::Action::ACT_LEFT:
		tank->angle.y -= tank->ROTATIONRATE;
		break;
	case Enemy::Action::ACT_RIGHT:
		tank->angle.y += tank->ROTATIONRATE;
		break;
	case Enemy::Action::ACT_STRAIGHT:

		break;
	default:
		break;
	}

	if (--keepTime <= 0 && state != Enemy::STATE::DESTROY) {
		action = static_cast<Action>(rand4(mt2));
		keepTime = rand10(mt2);
	}
}

/// <summary>
/// �U��
/// </summary>
void Enemy::AutoAttack() {

	std::uniform_int_distribution<uint32_t> rand10(10, 50);

	//�e����
	if (--atkInterval <= 0) {
		
		XMFLOAT4X4 tempmtx = tank->GetTankBarrelMtx();

		//��Ԃ̎p�����t�̂��ߔ��΂ɂ���
		tempmtx._31 = -tempmtx._31;
		tempmtx._32 = -tempmtx._32;
		tempmtx._33 = -tempmtx._33;
		tempmtx._34 = -tempmtx._34;

	
		Bullet_Manager::GetInstance().Fire(tempmtx, Bullet_Object::TYPEOBJ::ENEMY);


		atkInterval = rand10(mt2);
	}

}

/// <summary>
/// �G�X�V
/// </summary>
/// <param name="_playermtx">���@�s��</param>
void Enemy::UpdateEnemy(XMFLOAT4X4 _playermtx) {

	if (life <= 0 && state != Enemy::STATE::DESTROY) {
		SetState(Enemy::STATE::DESTROY);
		tank->PrepareDestroy();
	}

	float lng;
	pos = tank->GetPostion();
	/*DX11Vec3Length(pos, lng);*/

	uint16_t bullet_no = 0;

	//�����蔻��
	for (auto b : Bullet_Manager::GetInstance().GetBulPosPlayer()) {

		DX11p2pLength(pos, XMFLOAT3(b.x, b.y, b.z), lng);

		//�q�b�g
		if (lng < 100) {

			//�e�폜
			Bullet_Manager::GetInstance().DeleteList(bullet_no, Bullet_Object::TYPEOBJ::PLAYER);
			life--;
			if (life <= 0) {
				
			}
		}
		bullet_no++;
	}
	switch (state)
	{
		//�ǐՂƍU��
	case Enemy::STATE::CHASE:
		tank->UpdateChase(_playermtx);
		AutoAttack();
		break;
		//���G
	case Enemy::STATE::FREE:
		AutoPirot();
		tank->UpdateFree();
		break;
	case Enemy::STATE::CYCLE:
		tank->UpdateCycle();
		break;
	case Enemy::STATE::DEAD:
		isDraw = false;
		break;
	case Enemy::STATE::DESTROY:
		bool sts = tank->DestroyEffect();

		//�j��G�t�F�N�g���I����������
		if (sts) {
			SetState(Enemy::STATE::DEAD);
		}
		break;
	}

	
}