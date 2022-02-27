#include "Lader_UI.h"
#include "Texture2D_Manager.h"
#include "IScene.h"
#include "Enemy.h"
#include "Object.h"
#include "Player.h"
#include "RaderEne_UI.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_scene"></param>
Lader_UI::Lader_UI(IScene& _scene) {
	scene = &_scene;
	tag = "Lader";
	scene->AddSceneUI(this);
}

/// <summary>
/// 初期化
/// </summary>
/// <returns></returns>
bool Lader_UI::Init() {

	SetQuad2D(Texture2D_Manager::GetInstance().GetTexture2DPtr("assets/texture/reda.png"));
	SetPos(XMFLOAT3(Application::CLIENT_WIDTH / 2 - 530, Application::CLIENT_HEIGHT / 2 - 250, 0));
	SetScale(15, 15);
	blend = BLENDSTATE::DEFAULT;

	for (int i = 0; i < 5; i++) {
		Generate();
	}

	return true;
}

void Lader_UI::Update() {

	std::vector<XMFLOAT3> templist;
	auto eneobj = scene->GetSceneObjects<Enemy>("Enemy");

	auto playermtx = scene->GetSceneObject<Player>("Player")->GetMatrix();

	////敵の位置を入れる
	//for (auto ene : eneobj) {
	//	XMFLOAT3 b ;
	//	b = ene->GetPostion();
	//	templist.emplace_back(b);
	//}
	
	//万が一数が合わなかった場合減らす
	for (int i = eneobj.size(); i < enelist.size(); i++){
		enelist.erase(enelist.begin());
	}

	for (int i = 0; i < eneobj.size(); i++){
		enelist.at(i)->SetEnePos(eneobj.at(i)->GetPostion());
		enelist.at(i)->Update(playermtx);
	}

}

void Lader_UI::Draw() {

	TurnOffZbuffer();

	UpdateQuad2D();

	quad2d->Draw();

	for (auto& e : enelist) {
		e->Draw();
	}

	TurnOnZbuffer();
}

/// <summary>
/// 生成
/// </summary>
void Lader_UI::Generate() {
	
	std::unique_ptr<LaderEne_UI> e;

	e = std::make_unique<LaderEne_UI>();

	e->Init();

	enelist.emplace_back(move(e));

}