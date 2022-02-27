#include "landscape.h"
#include "IScene.h"
#include "CModel.h"
#include "ModelMgr.h"
#include "DX11util.h"
#include "StageHit.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
LandScape::LandScape(IScene& _scene) {
	scene = &_scene;
	scene->AddSceneObject(this);
	tag = "LandScape";
	render = NOSHADOW;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
LandScape::~LandScape() {
	StageHitExit();
}

/// <summary>
/// ������
/// </summary>
/// <returns></returns>
bool LandScape::Init() {

	if (scene->GetSceneTag() == "Title") {
		SetModel(ModelMgr::GetInstance().GetModelPtr("assets/stage/land.x"));
		DX11MtxScale(0.01f, 0.01f, 0.01f, mtx);
	}
	else {
		//SetModel(ModelMgr::GetInstance().GetModelPtr("assets/stage/land.x"));
		SetModel(ModelMgr::GetInstance().GetModelPtr("assets/land/stage_plane_plane_1.x"));
		StageHitInit(model);
	}
	
	if (model == nullptr) {
		return false;
	}

	return true;

}

/// <summary>
/// ���f���`��
/// </summary>
void LandScape::DrawModel() {
	
	GetLight().SetMtxWorld(mtx);

	model->DrawModel(mtx);
}

void LandScape::DrawShadow() {

	GetLight().SetMtxWorld(mtx);

	model->DrawShadow(mtx);
}


