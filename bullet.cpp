#include "bullet.h"
#include "Game_Manager.h"
#include "CModel.h"
#include "Sphere.h"
#include "DX11util.h"
#include "DX11Settransform.h"
#include "BoundingSphere_Manager.h"

using namespace DirectX;

constexpr float SPEED = 10.0f;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_mtx"> 発射したオブジェクトの行列 </param>
/// <param name="_type"> 誰から発射されたか </param>
Bullet_Object::Bullet_Object(XMFLOAT4X4 _mtx, TYPEOBJ _type)
	: sts(Bullet_Object::STATUS::LIVE)
	, speed(SPEED)
	, life(100)
	, type(_type)
	, weight(0)
	, radius(5.0f)
{
	//発射したオブジェクトの行列をセット
	SetMatrix(_mtx);
	//位置設定
	SetPos(XMFLOAT3(mtx._41, mtx._42, mtx._43));
	//半径
	radius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/shot.fbx");
}

Bullet_Object::Bullet_Object()
	: sts(Bullet_Object::STATUS::LIVE)
	, speed(SPEED)
	, life(100)
	, type(TYPEOBJ::PLAYER)
	, weight(0)
	, radius(5.0f)
{
	//発射したオブジェクトの行列をセット
	//位置設定
	SetPos(XMFLOAT3(mtx._41, mtx._42, mtx._43));
	//半径
	radius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/shot.fbx");
}

/// <summary>
/// デストラクタ
/// </summary>
Bullet_Object::~Bullet_Object() {
	/*Finalize();*/
}

bool Bullet_Object::Init() {
	

	return sts;
}

/// <summary>
/// 描画
/// </summary>
void Bullet_Object::Draw() {

	if (sts == STATUS::LIVE) {
		model->DrawModel(mtx);
		
	}
}

/// <summary>
/// 更新
/// </summary>
void Bullet_Object::Update() {

	this->life--;

	if (this->life > 0) {
		XMFLOAT4 axisZ;

		// Z軸を取り出す
		axisZ.x = this->mtx._31;
		axisZ.y = this->mtx._32;
		axisZ.z = this->mtx._33;
		axisZ.w = 0.0f;

		this->pos.x += axisZ.x*speed;
		this->pos.y += axisZ.y*speed;
		this->pos.z += axisZ.z*speed;

		this->mtx._41 = this->pos.x;
		this->mtx._42 = this->pos.y;
		this->mtx._43 = this->pos.z;
	}
	else {

 		this->sts = STATUS::DEAD;
	}

}

/// <summary>
/// 後処理
/// </summary>
void Bullet_Object::Finalize() {
	this->model->Uninit();
	
}