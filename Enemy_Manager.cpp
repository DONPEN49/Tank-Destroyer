#include <random>
#include <stdio.h>
#include "Enemy_Manager.h"
#include "CCamera.h"
#include "Player.h"
#include "rule_AI.h"
#include "Bullet_Manager.h"
#include "obb.h"
#include "IScene.h"

//乱数用変数
std::random_device rnddev;
std::mt19937 rnd;
std::uniform_real_distribution<float> distx(-150, 150);
std::uniform_real_distribution<float> distz(-150, 150);

Enemy_Manager::Enemy_Manager() {

}

Enemy_Manager::Enemy_Manager(IScene* _scene)
	:m_scene(_scene)
{
	
}


/// <summary>
/// 初期化
/// </summary>
/// <returns>初期化できたか</returns>
bool Enemy_Manager::Init(IScene* _scene) {

	m_scene = _scene;

	//要素削除
	enemylist.clear();

	//敵生成
	for (int i = 0; i < ENEMY_MAX; i++) {
		bool sts = Generate();
		if (!sts) {
			char str[128];
			sprintf_s<128>(str, "%s", "enemy");
			MessageBox(nullptr, str, "Generate error", MB_OK);
			assert(false);
			return false;
		}
	}

	//半径設定
	EnemyRadius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/tank/tank10_base.x");

	return true;
}

/// <summary>
///　敵生成
/// </summary>
bool Enemy_Manager::Generate() {
	//シード値をセット
	rnd.seed(rnddev());

	Enemy* e;

	e = new Enemy(m_scene);
	bool sts = e->Init();
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "enemy");
		MessageBox(nullptr, str, "init error", MB_OK);
		assert(false);
	}

	e->SetRand(rnd);
	e->SetPos(DirectX::XMFLOAT3(distx(rnd), 0.0f, distz(rnd)));

	enemylist.emplace_back(e);

	return sts;
}

/// <summary>
/// 更新
/// </summary>
void Enemy_Manager::Update(Player* _player) {

	float lng;		//弾との距離

	//弾の半径
	float bulrad = Bullet_Manager::GetInstance().GetBulRadius();

	//弾の番号
	int bullet_no = 0;

	//フラスタムカリング用行列
	XMFLOAT4X4 view, proj, out;
	view = CCamera::GetInstance()->GetCameraMatrix();
	proj = CCamera::GetInstance()->GetProjectionMatrix();
	DX11MtxMultiply(out, view, proj);

	//一斉更新
	for (auto& e : enemylist) {
		
		//破壊演出中以外
		if (e->GetState() != Enemy::STATE::DESTROY) {

			//AI設定
			ruleinfo rule = ruleselector(e, _player);
			rule.ruleactionfunc(e, _player);

			//当たり判定
			for (auto b : Bullet_Manager::GetInstance().GetBulPosPlayer()) {

				DX11p2pLength(e->GetPostion(), XMFLOAT3(b.x, b.y, b.z), lng);

				//ヒット
				if (lng < bulrad + EnemyRadius) {

					//弾削除
					Bullet_Manager::GetInstance().DeleteList(bullet_no, Bullet_Object::TYPEOBJ::PLAYER);
					e->life--;
					if (e->life <= 0) {
						Explosion(e->GetPostion());
					}
				}
				bullet_no++;
			}
		}
		e->UpdateEnemy(_player->GetMatrix());

		if (e->GetState() != Enemy::STATE::DEAD) {

			
			e->isDraw = IsInFrustum(e->GetPostion(), out);
		}

		bullet_no = 0;
	}

	auto ene = enemylist.begin();
	//死んでいたら除外する
	while (ene != enemylist.end()) {
		if ((*ene)->GetState() == Enemy::STATE::DEAD) {
			ene = enemylist.erase(ene);
		}
		else ++ene;
	}

	auto exp = explist.begin();
	
	while (exp != explist.end()) {
		if (!(*exp)->isDraw) {
			exp = explist.erase(exp);
		}
		else ++exp;
	}

}

/// <summary>
/// 描画
/// </summary>
void Enemy_Manager::Draw() {

	for (auto& exp : explist)
		exp->Draw();

	for (auto& e : enemylist)
		e->Draw();

}

/// <summary>
/// 描画
/// </summary>
void Enemy_Manager::DrawModel() {

	for (auto& exp : explist)
		exp->Draw();

	for (auto& e : enemylist) {

		e->DrawModel();
	}


}

/// <summary>
/// 描画
/// </summary>
void Enemy_Manager::DrawShadow() {

	for (auto& exp : explist)
		exp->Draw();

	for (auto& e : enemylist) {

		e->DrawShadow();
	}
		

}

/// <summary>
/// 敵OBBリスト取得
/// </summary>
/// <returns>敵OBBリスト</returns>
std::vector<COBB::OBBinfo> Enemy_Manager::GetEnemyOBB() {

	std::vector<COBB::OBBinfo> obblist;

	/*for (auto& e : enemylist) {
		obblist.emplace_back(e->GetOBB());
	}*/

	return obblist;
}

/// <summary>
/// 敵の位置取得
/// </summary>
std::vector<DirectX::XMFLOAT3> Enemy_Manager::GetEnemyPoses() {

	std::vector<DirectX::XMFLOAT3> templist;

	for (auto& e : enemylist)
		templist.emplace_back(e->GetPostion());


	return templist;
}

void Enemy_Manager::Finalize() {
	auto it = enemylist.begin();
	while (it != enemylist.end()) {

		it = enemylist.erase(it);
	}

	enemylist.clear();
}

void Enemy_Manager::Explosion(XMFLOAT3 _enepos) {

	std::unique_ptr<Exp_Billboard> exp;

	exp = std::make_unique<Exp_Billboard>();

	exp->Init();
	exp->SetPos(_enepos);

	explist.emplace_back(move(exp));
}