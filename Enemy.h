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
	/// �R���X�g���N�^
	/// </summary>
	Enemy();
	Enemy(IScene*);

	bool Init();
	void Update();
	void Draw();
	void DrawModel();
	void DrawShadow();

	/// <summary>
	/// �G�X�V
	/// </summary>
	void UpdateEnemy(XMFLOAT4X4 _playermtx);

	//���getter,setter
	void SetState(STATE _state) { state = _state; };
	const STATE GetState() const { return state; };

	void SetRand(std::mt19937 _mt) { mt2 = _mt; };

	//���C�t�擾
	const uint16_t& GetLife() const { return life; };
	uint16_t		life;			//���C�t
private:

	//�A�N�V������
	enum class Action {
		ACT_LEFT = 0,
		ACT_RIGHT,
		ACT_STRAIGHT,
		MAX_ACTION
	};

	/// <summary>
	/// �GAI
	/// </summary>
	void AutoPirot();

	/// <summary>
	/// �G�U��
	/// </summary>
	void AutoAttack();

	std::mt19937	mt2;

	uint32_t		keepTime;

	Action			action;			
	STATE			state;			//���
	uint32_t		atkInterval;	//�U���Ԋu		
	
	//��Ԗ{��
	Tank* tank;

	Exp_Billboard* ep;
};
