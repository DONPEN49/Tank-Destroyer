#pragma once
#include "Object.h"


class CModel;
class BoundingSphere;

class Bullet_Object : public Object {
public:

	/// <summary>
	/// �d��
	/// </summary>
	static constexpr float GRAVITY = 9.8f;

	enum STATUS {
		LIVE = 0,
		DEAD
	};

	enum TYPEOBJ {
		PLAYER,
		ENEMY
	};

	Bullet_Object(XMFLOAT4X4 _mtx, TYPEOBJ _type);
	Bullet_Object();
	~Bullet_Object();

	bool Init();

	void Update();

	void Draw();
	void DrawModel() {};
	void DrawShadow() {};

	void Finalize();

	void SetModel(CModel* _model) { model = _model; }
	const STATUS GetStatus() const { return sts; }
	void SetStatus(const STATUS& _sts) { sts = _sts; }
	const TYPEOBJ GetType() const { return type; };


private:

	/// <summary>
	/// �e�̏��
	/// </summary>
	STATUS sts;

	/// <summary>
	/// �N���ł����e��
	/// </summary>
	TYPEOBJ type;

	/// <summary>
	/// ���C�t
	/// </summary>
	uint32_t life;

	/// <summary>
	/// ���x
	/// </summary>
	float speed;

	/// <summary>
	/// ���f��
	/// </summary>
	CModel* model;
	BoundingSphere* sphere;

	/// <summary>
	/// �p�x
	/// </summary>
	float angle;

	/// <summary>
	/// ����
	/// </summary>
	float weight;

	/// <summary>
	/// �T�C�Y
	/// </summary>
	float radius;
};