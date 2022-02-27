#include "Bullet_Manager.h"
#include "bullet.h"
#include "ModelMgr.h"
#include "BoundingSphere_Manager.h"

void Bullet_Manager::Init() {

	//半径設定
	bulletradius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/shot.fbx");

}

/// <summary>
/// 弾発射
/// </summary>
/// <param name="_mtx">発射したオブジェクトの行列</param>
/// <param name="_type">どのオブジェが発射したか</param> 
void Bullet_Manager::Fire(XMFLOAT4X4 _mtx, Bullet_Object::TYPEOBJ _type) {

	std::unique_ptr<Bullet_Object> b;

	b = std::make_unique<Bullet_Object>(_mtx, _type);
	b->Init();
	b->SetModel(
		ModelMgr::GetInstance().GetModelPtr("assets/shot.fbx")
	);

	if (_type == Bullet_Object::TYPEOBJ::PLAYER) {
		playerlist.emplace_back(std::move(b));
	}
	else {
		enemylist.emplace_back(std::move(b));
	}

}

void Bullet_Manager::Update() {

	for (auto& bul : playerlist)
		bul->Update();

	for (auto& bul : enemylist)
		bul->Update();

	// 死んでる弾削除
	auto it = playerlist.begin();
	while (it != playerlist.end()) {

		if ((*it)->GetStatus() == Bullet_Object::STATUS::DEAD) {
			it = playerlist.erase(it);

		}
		else ++it;
	}

	auto itr = enemylist.begin();
	while (itr != enemylist.end()) {

		if ((*itr)->GetStatus() == Bullet_Object::STATUS::DEAD) {
			itr = enemylist.erase(itr);

		}
		else ++itr;
	}
}

void Bullet_Manager::Draw() {

	/*for (auto& bul : playerlist)
		bul->Draw();*/

	/*for (auto& bul : enemylist)
		bul->Draw();*/
}

void Bullet_Manager::Exit() {
	auto it = playerlist.begin();
	while (it != playerlist.end()) {
		it = playerlist.erase(it);
	}

	auto itr = enemylist.begin();
	while (itr != enemylist.end()) {
		itr = enemylist.erase(itr);
	}
}

/// <summary>
/// 弾の位置を返す
/// </summary>
std::vector<XMFLOAT3> Bullet_Manager::GetBulPosPlayer() {

	//仮のリスト
	std::vector<XMFLOAT3> templist;

	for (auto& bul : playerlist) {
		templist.emplace_back(bul->GetPostion());
	}

	return templist;
}


/// <summary>
/// 弾の位置を返す
/// </summary>
std::vector<XMFLOAT3> Bullet_Manager::GetBulPosEnemy() {

	//仮のリスト
	std::vector<XMFLOAT3> templist;

	for (auto& bul : enemylist) {
		templist.emplace_back(bul->GetPostion());
	}

	return templist;
}

/// <summary>
/// 弾削除
/// </summary>
/// <param name="_no">NO</param>
/// <param name="_type">種類</param>
void Bullet_Manager::DeleteList(int _no, Bullet_Object::TYPEOBJ _type) {

	if (_type == Bullet_Object::TYPEOBJ::PLAYER) {
		playerlist.erase(playerlist.begin() + _no);
	}
	else {
		enemylist.erase(enemylist.begin() + _no);
	}

}

