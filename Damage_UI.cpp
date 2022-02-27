#include "Damage_UI.h"
#include "Texture2D_Manager.h"
#include "Application.h"
#include "IScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_manager">UIマネージャー</param>
Damage_UI::Damage_UI(IScene* _manager)
	: m_life(60)
{
	scene = _manager;
	scene->AddSceneUI(this);
}

Damage_UI::Damage_UI() 
	: m_life(60)
{

}

bool Damage_UI::Init() {
	//テクスチャ設定
	SetQuad2D(Texture2D_Manager::GetInstance().GetTexture2DPtr("assets/texture/damage.png"));
	SetPos(XMFLOAT3(Application::CLIENT_WIDTH / 2, Application::CLIENT_HEIGHT / 2, 0));
	SetScale(20.0f, 9.0f);
	
	return true;
}

void Damage_UI::Update() {

	//quad2d->SetRotation
}

void Damage_UI::Draw() {
	TurnOffZbuffer();

	UpdateQuad2D();

	quad2d->Draw();

	TurnOnZbuffer();
}
