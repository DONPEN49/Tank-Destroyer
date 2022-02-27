#include "Stage_Manager.h"
#include "ModelMgr.h"
#include "dx11mathutil.h"
#include "Texture2D_Manager.h"
#include "BoundingSphere_Manager.h"
#include "CCamera.h"
#include "StageHit.h"

int  Stage_Manager::map[Stage_Manager::MAPSIZEZ][Stage_Manager::MAPSIZEX] = {
	
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		4,9,9,9,9,9,9,9,9,9,9,9,9,9,4,
		4,9,0,0,0,0,0,0,0,0,0,0,0,9,4,
		4,9,0,9,9,9,0,0,0,9,9,9,0,9,4,
		4,9,0,9,4,9,0,0,0,9,4,9,0,9,4,
		4,9,0,9,9,9,0,0,0,9,9,9,0,9,4,
		4,9,0,0,0,0,0,0,0,0,0,0,0,9,4,
		4,9,0,0,0,0,0,0,0,0,0,0,0,9,4,
		4,9,0,0,0,0,0,0,0,0,0,0,0,9,4,
		4,9,0,9,9,9,0,0,0,9,9,9,0,9,4,
		4,9,0,9,4,9,0,0,0,9,4,9,0,9,4,
		4,9,0,9,9,9,0,0,0,9,9,9,0,9,4,
		4,9,0,0,0,0,0,0,0,0,0,0,0,9,4,
		4,9,9,9,9,9,9,9,9,9,9,9,9,9,4,
		4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
};

/// <summary>
/// ステージオブジェクト生成
/// </summary>
/// <returns></returns>
bool Stage_Manager::Generate(std::string _filename, XMFLOAT4X4 _mtx) {

	std::unique_ptr<Stage_Object> s;

	s = std::make_unique<Stage_Object>();

	bool sts = s->Init(_filename);
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", _filename.c_str());
		MessageBox(nullptr, str, "Generate error", MB_OK);
		assert(false);
		return false;
	}

	//ワールド行列をセット
	s->SetMatrix(_mtx);

	stagelist.emplace_back(move(s));

	return true;
}

/// <summary>
/// 初期化
/// </summary>
/// <returns>初期化できたか</returns>
bool Stage_Manager::Init() {

	bool sts = true;

	XMFLOAT4X4 worldmtx;
	XMFLOAT3 angle = { 0,0,0 };
	XMFLOAT3 trans = { 0,0,0 };

	DX11MtxIdentity(worldmtx);

	/*sts = Generate("assets/land/land.x", worldmtx);
	if (!sts) {
		char str[128];
		sprintf_s<128>(str, "%s", "Stage_Manager");
		MessageBox(nullptr, str, "Init error", MB_OK);
		assert(false);
		return false;
	}*/

	modelmapchip = ModelMgr::GetInstance().GetModelPtr("assets/dice/Wall.x");

	wallradius = BoundingSphere_Manager::GetInstance().GetBSRadius("assets/dice/Wall.x");

	StageHitInit(ModelMgr::GetInstance().GetModelPtr("assets/land/Stage_StandByRoom_ver10.x"));

	return sts;
}

/// <summary>
/// 更新
/// </summary>
void Stage_Manager::Update() {

	for (auto& stage : stagelist)
		stage->Update();
}

/// <summary>
/// 描画
/// </summary>
void Stage_Manager::Draw() {

	for (auto& stage : stagelist)
		stage->Draw();

	//マップチップオブジェクト描画
	for (int z = -MAPSIZEZ / 2; z <= MAPSIZEZ / 2; z++) {
		for (int x = -MAPSIZEX / 2; x <= MAPSIZEX / 2; x++) {

			Chip3DDraw(z, x);
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void Stage_Manager::DrawModel() {

	for (auto& stage : stagelist)
		stage->DrawModel();

	////マップチップオブジェクト描画
	//for (int z = -MAPSIZEZ / 2; z <= MAPSIZEZ / 2; z++) {
	//	for (int x = -MAPSIZEX / 2; x <= MAPSIZEX / 2; x++) {

	//		Chip3DDraw(z, x);
	//	}
	//}
}

/// <summary>
/// 描画
/// </summary>
void Stage_Manager::DrawShadow() {

	for (auto& stage : stagelist)
		stage->DrawShadow();

	////マップチップオブジェクト描画
	//for (int z = -MAPSIZEZ / 2; z <= MAPSIZEZ / 2; z++) {
	//	for (int x = -MAPSIZEX / 2; x <= MAPSIZEX / 2; x++) {

	//		Chip3DDraw(z, x);
	//	}
	//}
}


/// <summary>
/// 終了
/// </summary>
void Stage_Manager::Exit() {

	//リスト削除
	auto it = stagelist.begin();
	while (it != stagelist.end()) {
		it = stagelist.erase(it);
	}

	stagelist.clear();
}

void Stage_Manager::Chip3DDraw(int x, int z) {

	XMFLOAT4X4 mtx;
	DX11MtxIdentity(mtx);

	// 平行移動量
	XMFLOAT3 trans = { MAPCHIPWIDTH*x ,0.0f , MAPCHIPWIDTH*z };
	
	DX11MtxTranslation(trans, mtx);
	mtx._11 *= 2.0f;
	mtx._22 *= 2.0f;
	mtx._33 *= 2.0f;

	// マップチップフロア１のＩＤ番号を取得
	int modelno = GetMapChipNo(trans.x, trans.z);

	// マップチップ描画
	if (modelno == 4) {

		//フラスタムカリング
		XMFLOAT4X4 view, proj, out;
		view = CCamera::GetInstance()->GetCameraMatrix();
		proj = CCamera::GetInstance()->GetProjectionMatrix();
		DX11MtxMultiply(out, view, proj);

		if (IsInFrustum(XMFLOAT3(mtx._41, mtx._42, mtx._43), wallradius, out)) {
			modelmapchip->Draw(mtx);
		}


	}

}