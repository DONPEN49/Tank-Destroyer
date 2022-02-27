#include "Crosshair_UI.h"
#include "Texture2D_Manager.h"
#include "DX11util.h"
#include "IScene.h"

Crosshair_UI::Crosshair_UI(IScene& _manager) 
{
	//リストに追加
	scene = &_manager;
	scene->AddSceneUI(this);
}


bool Crosshair_UI::Init() {

	//セット
	SetQuad2D(Texture2D_Manager::GetInstance().GetTexture2DPtr("assets/texture/Closs.png"));
	SetPos(XMFLOAT3(Application::CLIENT_WIDTH / 2, Application::CLIENT_HEIGHT / 2.5f, 0));
	SetScale(12, 12);

	return true;
}

void Crosshair_UI::Draw() {
	TurnOffZbuffer();

	UpdateQuad2D();

	quad2d->Draw();

	TurnOnZbuffer();
}
