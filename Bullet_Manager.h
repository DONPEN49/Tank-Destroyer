#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Singleton.h"
#include "bullet.h"

using namespace DirectX;

class Bullet_Manager : public  Singleton<Bullet_Manager> {

private:
	std::vector<std::unique_ptr<Bullet_Object>>	playerlist;
	std::vector<std::unique_ptr<Bullet_Object>>	enemylist;

	friend class Singleton<Bullet_Manager>;

	float bulletradius;

public:

	/// <summary>
	/// 弾発射
	/// </summary>
	/// <param name="mtx">発射したオブジェクトの行列</param>
	/// <param name="_type">発射したオブジェクトの種類</param>
	void Fire(XMFLOAT4X4 mtx, Bullet_Object::TYPEOBJ _type);

	void Init();

	void Update();

	void Draw();

	void Exit();

	/// <summary>
	/// 弾の位置取得
	/// </summary>
	/// <returns></returns>
	std::vector<XMFLOAT3> GetBulPosPlayer();
	std::vector<XMFLOAT3> GetBulPosEnemy();

	/// <summary>
	/// 弾の半径取得
	/// </summary>
	const float GetBulRadius() const { return bulletradius; };

	/// <summary>
	/// 弾リストから削除
	/// </summary>
	/// <param name="_no">削除する弾のNO</param>
	void DeleteList(int _no, Bullet_Object::TYPEOBJ _type);

};