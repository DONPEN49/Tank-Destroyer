#include	"ModelMgr.h"

void ModelMgr::AllModelLoad() {

	//íÔ
	bool sts = LoadModel(
		"assets/tank/tank10_cat.x",
		"shader/vsoutline.hlsl",
		"shader/psoutline.hlsl",
		"assets/tank/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "tank10_cat.x");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}

	sts = LoadModel(
		"assets/tank/tank10_base.x",
		"shader/vsoutline.hlsl",
		"shader/psoutline.hlsl",
		"assets/tank/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "tank10_base.x");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}

	sts = LoadModel(
		"assets/tank/tank10_pipe.x",
		"shader/vsoutline.hlsl",
		"shader/psoutline.hlsl",
		"assets/tank/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "tank10_pipe.x");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}

	sts = LoadModel(
		"assets/tank/tank10_top.x",
		"shader/vsoutline.hlsl",
		"shader/psoutline.hlsl",
		"assets/tank/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "tank10_top.x");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}
	//////////

	//“GíÔ
	sts = LoadModel(
		"assets/tank/tank10_cat_enemy.x",
		"shader/vs.hlsl",
		"shader/psenemy.hlsl",
		"assets/tank/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "tank10_cat_enmey.x");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}

	sts = LoadModel(
		"assets/tank/tank10_base_enemy.x",
		"shader/vs.hlsl",
		"shader/psenemy.hlsl",
		"assets/tank/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "tank10_base_enmey.x");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}

	sts = LoadModel(
		"assets/tank/tank10_pipe_enemy.x",
		"shader/vs.hlsl",
		"shader/psenemy.hlsl",
		"assets/tank/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "tank10_pipe_enmey.x");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}

	sts = LoadModel(
		"assets/tank/tank10_top_enemy.x",
		"shader/vs.hlsl",
		"shader/psenemy.hlsl",
		"assets/tank/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "tank10_top_enmey.x");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}

	/// ////////


	sts = LoadModel(
		"assets/stage/land.x",
		"shader/vs.hlsl",
		"shader/pstexcol.hlsl",
		"assets/stage/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "stage1.x");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}

	sts = LoadModel(
		"assets/skydome/Field.x",
		"shader/vs.hlsl",
		"shader/pstexcol.hlsl",
		"assets/skydome/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "assets/skydome/sky.x");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}

	//’e
	sts = LoadModel(
		"assets/shot.fbx",
		"shader/vs.hlsl",
		"shader/ps.hlsl",
		"assets/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "assets/shot.fbx");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}

	sts = LoadModel(
		"assets/dice/Wall.x",
		"shader/vs.hlsl",
		"shader/ps.hlsl",
		"assets/dice/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "assets/dice/Wall.x");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}

	sts = LoadModel(
		"assets/land/stage_plane_plane_1.x",
		"shader/vs.hlsl",
		"shader/ps.hlsl",
		"assets/land/");
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "assets/land/landscape.x");
		MessageBox(nullptr, str, "load error", MB_OK);
		assert(false);
	}
}