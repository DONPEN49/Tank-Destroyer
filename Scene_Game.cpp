#include "Scene_Game.h"
#include "Shadowmap.h"
#include "Manager_Camera.h"
#include "ModelMgr.h"
#include "Player.h"
#include "Skydome.h"
#include "landscape.h"
#include "Enemy.h"
#include "HP_UI.h"
#include "Smoke_Billboard.h"
#include "Damage_UI.h"
#include "Crosshair_UI.h"
#include "Enemy_Manager.h"
#include "Bullet_Manager.h"
#include "Lader_UI.h"
#include "CCamera.h"

/// <summary>
/// ������
/// </summary>
void Game_Scene::Init() {

	this->tag = "Game";

	//�J�����̈ʒu�ݒ�
	Manager_Camera::GetInstance().SetLocalPos(XMFLOAT3(Manager_Camera::DEFPOSX, Manager_Camera::DEFPOSY, Manager_Camera::DEFPOSZ));

	//�I�u�W�F�N�g�ǉ�
	Skydome* skydome = new Skydome(*this);
	LandScape* land = new LandScape(*this);
	Player* player = new Player(*this);

	//UI�ǉ�
	HP_UI* hp = new HP_UI(*this);
	Crosshair_UI* cross = new Crosshair_UI(*this);
	Lader_UI* lader = new Lader_UI(*this);

	Enemy_Manager::GetInstance().Init(this);

	//������
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

}

/// <summary>
/// ����
/// </summary>
void Game_Scene::Input() {

}

/// <summary>
/// �X�V
/// </summary>
void Game_Scene::Update() {

	//�t���X�^���J�����O�p�s��
	XMFLOAT4X4 view, proj, out;
	view = CCamera::GetInstance()->GetCameraMatrix();
	proj = CCamera::GetInstance()->GetProjectionMatrix();
	DX11MtxMultiply(out, view, proj);

	//��čX�V
	for (auto obj : listObject)
		obj->Update();

	//�t���X�^���J�����O
	for (auto obj : listObject) {
		if (obj->tag != "LandScape") {
			obj->isDraw = IsInFrustum(obj->GetPostion(), out);
		}
		
	}

	//�J�����̈ʒu�X�V
	auto player = GetSceneObject<Player>("Player");

	for (auto ui : listUI)
		ui->Update();

	Manager_Camera::GetInstance().UpdateCamera(player->GetPlayerPipe());
}

/// <summary>
/// �`��
/// </summary>
void Game_Scene::Draw() {

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
/// �I��
/// </summary>
void Game_Scene::Dispose() {

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