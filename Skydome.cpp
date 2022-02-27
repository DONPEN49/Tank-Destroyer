#include "Skydome.h"
#include "ModelMgr.h"
#include "DX11util.h"
#include "IScene.h"
#include "Player.h"
/// <summary>
/// コンストラクタ
/// </summary>
Skydome::Skydome() {
	tag = "Skydome";
	render = DEFAULT;
}
Skydome::Skydome(IScene& _scene) {

	//基底クラスメンバ変数初期化
	scene = &_scene;
	tag = "Skydome";
	render = DEFAULT;
	scene->AddSceneObject(this);
}

/// <summary>
/// 初期化
/// </summary>
bool Skydome::Init() {

	SetModel(ModelMgr::GetInstance().GetModelPtr("assets/skydome/Field.x"));

	if (model == nullptr) {
		return false;
	}
	return true;
}


void Skydome::Update() {

	//プレイヤー取得

	if (scene->GetSceneTag() == "Game") {
		auto player = scene->GetSceneObject<Player>("Player");

		SetPos(player->GetPostion());
	}
	
}

void Skydome::Draw() {

	mtx._41 = pos.x;
	mtx._42 = pos.y;
	mtx._43 = pos.z;

	model->Draw(mtx);

}

void Skydome::DrawShadow() {

	mtx._41 = pos.x;
	mtx._42 = pos.y;
	mtx._43 = pos.z;

	GetLight().SetMtxWorld(mtx);

	model->DrawShadow(mtx);

}

void Skydome::DrawModel() {

	mtx._41 = pos.x;
	mtx._42 = pos.y;
	mtx._43 = pos.z;

	GetLight().SetMtxWorld(mtx);

	model->DrawModel(mtx);

}