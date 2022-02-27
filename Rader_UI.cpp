#include "Rader_UI.h"
#include "Texture2D_Manager.h"
#include "IScene.h"
#include "Application.h"

Rader_UI::Rader_UI(IScene* _manager) {
	scene = _manager;
	scene->AddSceneUI(this);
}

bool Rader_UI::Init() {

	//テクスチャ設定
	SetQuad2D(Texture2D_Manager::GetInstance().GetTexture2DPtr("assets/texture/reda.png"));
	SetPos(XMFLOAT3(Application::CLIENT_WIDTH / 2 - 530, Application::CLIENT_HEIGHT / 2 - 250, 0));
	SetScale(15, 15);

	return true;
}

void Rader_UI::Draw() {

	TurnOffZbuffer();

	UpdateQuad2D();

	quad2d->Draw();

	TurnOnZbuffer();
}