#include "Player.h"
#include "tank.h"
#include "CDirectInput.h"
#include "Bullet_Manager.h"
#include "IScene.h"
#include "dx11mathutil.h"
#include "Manager_Camera.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() {
	status = LIVE;
	life = 5;
	tag = "Player";
}

Player::Player(IScene& _scene)
	:life(5)
	,status(LIVE)
{
	//基底クラスメンバ変数設定
	scene = &_scene;
	tag = "Player";
	render = DEFAULTSHADOW;
	scene->AddSceneObject(this);
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player() {
	delete tank;
	delete smoke;
}


/// <summary>
/// 初期化
/// </summary>
bool Player::Init() {

    tank = new Tank(Tank::TYPE::PLAYER);
	smoke = new Smoke_Billboard(GetPostion());
	bool sts = tank->Init();
	smoke->Init();
	return sts;
}


void Player::Update() {

	if (status == LIVE) {
		tank->Update();
	}

	smoke->Update();

	//位置と行列を更新
	this->mtx = tank->GetMatrix();
	pos.x = mtx._41;
	pos.y = mtx._42;
	pos.z = mtx._43;

	// 砲台を動かす
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_SPACE)) {
		tank->Fire();
		Bullet_Manager::GetInstance().Fire(mtx,Bullet_Object::TYPEOBJ::PLAYER);
	}

	//弾のNo（削除用）
	int bulno = 0;

	//弾との当たり判定
	for (auto b : Bullet_Manager::GetInstance().GetBulPosEnemy()) {

		float lng;

		DX11p2pLength(XMFLOAT3(b.x, b.y, b.z), tank->GetPostion(), lng);

		if (lng < tank->radius + Bullet_Manager::GetInstance().GetBulRadius()) {

			//life--;
			Bullet_Manager::GetInstance().DeleteList(bulno, Bullet_Object::TYPEOBJ::ENEMY);
		}

		bulno++;
	}

	if (life <= 0) {
		status = DEAD;
		tank->isDraw = false;
	}

	
}

void Player::Draw() {
	tank->Draw();
	smoke->Draw();

	for (auto& sm : m_smokeList)
		sm->Draw();

}

void Player::DrawModel() {

	tank->DrawModel();

}

void Player::DrawShadow() {

	tank->DrawShadow();
}


void Player::SmokeGenerate() {

	std::unique_ptr<Smoke_Billboard> sm;

	sm = std::make_unique<Smoke_Billboard>(pos);

	sm->Init();

	m_smokeList.emplace_back(move(sm));

}