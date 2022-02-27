#include <cinttypes>
#include "dx11util.h"
#include "CModel.h"
#include "CCamera.h"
#include "DX11Settransform.h"
#include "dx11mathutil.h"
#include "Application.h"
#include "CDirectInput.h"
#include "ModelMgr.h"
#include "quad2d.h"
#include "2dsystem.h"
#include "blendstate.h"
#include "SceneManager.h"
#include "Title_Scene.h"
#include "Scene_Game.h"
#include "Shadowmap.h"
#include "Texture2D_Manager.h"
#include "Manager_Billboard.h"
#include "BoundingSphere_Manager.h"

SceneManager sceneManager;

void GameInit() {
	// DX11初期化
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);
	
	Init2D();

	InitBlendState();

	// カメラが必要
	DirectX::XMFLOAT3 eye(0, 30, 75);		// カメラの位置
	DirectX::XMFLOAT3 lookat(0, 0, 0);	// 注視点
	DirectX::XMFLOAT3 up(0, 1, 0);			// カメラの上向きベクトル

	CCamera::GetInstance()->Init(
		10.0f,								// ニアクリップ
		1000.0f,							// ファークリップ
		XM_PI / 4.0f,						// 視野角
		Application::CLIENT_WIDTH,			// スクリーン幅
		Application::CLIENT_HEIGHT,			// スクリーンの高さ
		eye, lookat, up);					// カメラのデータ

	// 平行光源をセット
	DX11LightInit(
		DirectX::XMFLOAT4(1, 1, -1, 0));

	// アルファブレンド有効化
	TurnOnAlphablend();

	// DirectT INPUT 初期化
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT);

	// モデルの読み込み
	ModelMgr::GetInstance().AllModelLoad();

	//テクスチャ読み込み
	Texture2D_Manager::GetInstance().AllTexture2DLoad();

	//バウンディングスフィア生成
	BoundingSphere_Manager::GetInstance().AllBSCreate();

	//ビルボード読み込み
	Manager_Billboard::GetInstance().LoadAllBillboard();

	ShadowMap::GetInstance().Init();

	sceneManager.add<Title_Scene>("Title");
	sceneManager.add<Game_Scene>("Game");
	sceneManager.setCurrentScene("Title");

}

void GameInput(uint64_t dt) {

	CDirectInput::GetInstance().GetKeyBuffer();
	
	//シーン遷移
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
		if (sceneManager.GetCurrentSceneKey() == "Title") {
			sceneManager.SetFadeColor(XMFLOAT4(0, 0, 0, 1));
			sceneManager.changeScene<Game_Scene>("GameMain", 1000, false);
		}
	}

}

void GameUpdate(uint64_t dt) {

	sceneManager.Update();
}

void GameRender(uint64_t dt) {

	float col[4] = { 1,0,0,1 };

	// 描画前処理
	DX11BeforeRender(col);

	XMFLOAT4X4 mtx;

	// プロジェクション変換行列取得
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	// ビュー変換行列を取得
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	sceneManager.Draw();

	// 描画後処理
	DX11AfterRender();
}

void GameDispose() {

	

	DX11Uninit();
}

