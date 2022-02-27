#pragma once

#include "Object.h"
#include "Smoke_Billboard.h"
#include "tank.h"
#include <vector>

class Player : public Object {
public:

	enum STATUS {
		WAIT = 0,
		LIVE,
		DEAD,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();
	Player(class IScene&);
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	bool Init();

	/// <summary>
	/// プレイヤー更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	void DrawModel();
	void DrawShadow();

	/// <summary>
	/// 状態設定
	/// </summary>
	void SetStatus(STATUS _status) { status = _status; };

	/// <summary>
	/// 状態取得
	/// </summary>
	const STATUS GetStatus() const { return status; };

	/// <summary>
	/// ライフ取得
	/// </summary>
	const uint32_t GetLife()const { return life; };

	/// <summary>
	/// パイプ取得
	/// </summary>
	const XMFLOAT4X4 GetPlayerPipe()const { return tank->GetPipeMtx(); };

private:

	STATUS status;

	uint32_t life;

	Tank* tank;

	Smoke_Billboard* smoke;

	std::vector<std::unique_ptr<Smoke_Billboard>> m_smokeList;

	void SmokeGenerate();

};