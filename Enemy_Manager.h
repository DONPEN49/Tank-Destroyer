#pragma once


#include <vector>
#include "Enemy.h"
#include "Exp_Billboard.h"
#include "obb.h"
#include "Singleton.h"

class IScene;

class Enemy_Manager : public Singleton<Enemy_Manager> {

public:

	static constexpr uint16_t ENEMY_MAX = 3;

	Enemy_Manager();
	Enemy_Manager(IScene*);

	bool Init(IScene*);
	void Update(class Player*);
	void Draw();
	void DrawModel();
	void DrawShadow();
	void Finalize();

	/// <summary>
	/// OBBリスト取得
	/// </summary>
	std::vector<COBB::OBBinfo> GetEnemyOBB();

	/// <summary>
	/// 敵の位置取得
	/// </summary>
	std::vector<DirectX::XMFLOAT3> GetEnemyPoses();

private:

	friend class Singleton<Enemy_Manager>;

	/// <summary>
	/// 敵生成
	/// </summary>
	bool Generate();

	/// <summary>
	/// 爆破エフェクト生成
	/// </summary>
	void Explosion(DirectX::XMFLOAT3);

	std::vector<Enemy*> enemylist;		//敵のリスト
	std::vector<std::unique_ptr<Exp_Billboard>> explist;	//爆破テクスチャリスト

	float EnemyRadius;

	IScene* m_scene;

};