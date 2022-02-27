#include "Title_Scene.h"
#include "Texture2D_Manager.h"
#include "Skydome.h"
#include "tank.h"
#include "CModel.h"
#include "ModelMgr.h"
#include "Shadowmap.h"
#include "landscape.h"
#include "Manager_Camera.h"

/// <summary>
/// 初期化
/// </summary>
void Title_Scene::Init() {

	this->tag = "Title";

	// 単位行列化
	DX11MtxIdentity(mtx);

	//PLAY
	{
		UI* play = new UI(*this);
		play->tag = "play";
		play->SetQuad2D(Texture2D_Manager::GetInstance().GetTexture2DPtr("assets/texture/play.png"));
		play->SetPos(XMFLOAT3(Application::CLIENT_WIDTH / 4, Application::CLIENT_HEIGHT / 4 * 3.5f, 0));
		play->blend = BLENDSTATE::DEFAULT;
		play->SetScale(18, 12);
	}

	//EXIT
	{
		UI* exit = new UI(*this);
		exit->tag = "exit";
		exit->SetQuad2D(Texture2D_Manager::GetInstance().GetTexture2DPtr("assets/texture/exit.png"));
		exit->SetPos(XMFLOAT3(Application::CLIENT_WIDTH / 4 * 3, Application::CLIENT_HEIGHT / 4 * 3.5f, 0));
		exit->blend = BLENDSTATE::DEFAULT;
		exit->SetScale(18, 12);
	}

	//TITLE
	{
		UI* title = new UI(*this);
		title->tag = "title";
		title->SetQuad2D(Texture2D_Manager::GetInstance().GetTexture2DPtr("assets/texture/title.png"));
		title->SetPos(XMFLOAT3(Application::CLIENT_WIDTH / 4 * 2, Application::CLIENT_HEIGHT / 4 * 1.25f, 0));
		title->blend = BLENDSTATE::DEFAULT;
		title->SetScale(28, 22);
	}


	//オブジェクト追加
	Skydome* skydome = new Skydome(*this);

	LandScape* land = new LandScape(*this);

	tank = new Tank(this);
	//tank->SetPos(XMFLOAT3(-15.4419365, 0.0f, -168.9f));

	//初期化
	bool sts;
	for (auto obj : listObject) {
		sts = obj->Init();
		if (!sts) {
			char str[128];
			sprintf_s<128>(str, "%s", obj->tag.c_str());
			MessageBox(nullptr, str, "Init error", MB_OK);
			assert(false);
		}
	}

	for (auto ui : listUI) {
		sts = ui->Init();
		if (!sts) {
			char str[128];
			sprintf_s<128>(str, "%s", ui->tag.c_str());
			MessageBox(nullptr, str, "Init error", MB_OK);
			assert(false);
		}
	}

	return;
}

/// <summary>
/// 更新
/// </summary>
void Title_Scene::Update() {

	/*for (auto obj : listObject)
		obj->Update();

	for (auto ui : listUI)
		ui->Update();*/

	//auto tank = GetSceneObject<Tank>("Tank");
	//tank->UpdateSelect();

	static float azimuth = 0.0f;

	azimuth += 0.3f;

	Manager_Camera::GetInstance().UpdateSphereCamera(30.0f, -60.0f, azimuth, XMFLOAT3(0.0f, 0.0f, 0.0f));
		
}

/// <summary>
/// 描画
/// </summary>
void Title_Scene::Draw() {

	static float alpha;

	alpha += 0.05f;

	auto play = GetSceneUI<UI>("play");

	play->SetAlpha(sinf(alpha));

	ShadowMap::GetInstance().RenderDepthMap();

	for (auto obj : listObject) {
		if (obj->render == Object::RENDER::WITHSHADOW || obj->render == Object::RENDER::DEFAULTSHADOW) {
			if (obj->isDraw) {
				obj->DrawShadow();
			}

		}
	}

	ShadowMap::GetInstance().RenderModel();

	for (auto obj : listObject) {
		if (obj->render == Object::RENDER::DEFAULT || obj->render == Object::RENDER::DEFAULTSHADOW) {
			if (obj->isDraw) {
				obj->Draw();
			}
		}
		else {
			if (obj->isDraw) {
				obj->DrawModel();
			}
		}
	}

	for (auto ui : listUI) {

		if (ui->blend == BLENDSTATE::ADD) {
			ui->DrawAdd();
		}
		else {
			ui->Draw();
		}
	}

}

/// <summary>
/// 終了
/// </summary>
void Title_Scene::Dispose() {

	auto obj = listObject.begin();
	while (obj != listObject.end()) {
		obj = listObject.erase(obj);
	}
	listObject.clear();

	auto ui = listUI.begin();
	while (ui != listUI.end()) {
		ui = listUI.erase(ui);
	}
	listUI.clear();
}