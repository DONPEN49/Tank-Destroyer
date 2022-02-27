#pragma once
#include <random>
#include "Object.h"
#include "BoundingSphere_Manager.h"

class Tank;
class IScene;
class Exp_Billboard;

class Enemy : public Object{

public:

	static constexpr float RAYSIZE = 30.0f;
	static constexpr uint32_t RAYLENGTH = 20;
	static constexpr uint32_t RAYJUDGE = 2;

	enum class STATE {
		CHASE,
		FREE,
		CYCLE,
		DESTROY,
		DEAD,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy();
	Enemy(IScene*);

	bool Init();
	void Update();
	void Draw();
	void DrawModel();
	void DrawShadow();

	/// <summary>
	/// 敵更新
	/// </summary>
	void UpdateEnemy(XMFLOAT4X4 _playermtx);

	//状態getter,setter
	void SetState(STATE _state) { state = _state; };
	const STATE GetState() const { return state; };

	void SetRand(std::mt19937 _mt) { mt2 = _mt; };

	//ライフ取得
	const uint16_t& GetLife() const { return life; };
	uint16_t		life;			//ライフ
private:

	//アクション列挙
	enum class Action {
		ACT_LEFT = 0,
		ACT_RIGHT,
		ACT_STRAIGHT,
		MAX_ACTION
	};

	/// <summary>
	/// 敵AI
	/// </summary>
	void AutoPirot();

	/// <summary>
	/// 敵攻撃
	/// </summary>
	void AutoAttack();

	std::mt19937	mt2;

	uint32_t		keepTime;

	Action			action;			
	STATE			state;			//状態
	uint32_t		atkInterval;	//攻撃間隔		
	
	//戦車本体
	Tank* tank;

	Exp_Billboard* ep;
};
