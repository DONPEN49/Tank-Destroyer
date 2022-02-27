#pragma once
#include "Object.h"


class CModel;
class BoundingSphere;

class Bullet_Object : public Object {
public:

	/// <summary>
	/// 重力
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
	/// 弾の状態
	/// </summary>
	STATUS sts;

	/// <summary>
	/// 誰が打った弾か
	/// </summary>
	TYPEOBJ type;

	/// <summary>
	/// ライフ
	/// </summary>
	uint32_t life;

	/// <summary>
	/// 速度
	/// </summary>
	float speed;

	/// <summary>
	/// モデル
	/// </summary>
	CModel* model;
	BoundingSphere* sphere;

	/// <summary>
	/// 角度
	/// </summary>
	float angle;

	/// <summary>
	/// 質量
	/// </summary>
	float weight;

	/// <summary>
	/// サイズ
	/// </summary>
	float radius;
};