#pragma once
#include    <vector>
#include    <random>
#include	"CModel.h"
#include    "Object.h"
#include    "obb.h"
#include    "Smoke_Billboard.h"

using namespace DirectX;

class BoundingSphere;
class Fire_Billboard;
class IScene;

class Tank : public Object
{
public:
	// オブジェクト名
	enum TANKPARTS {
		TANK_CATERPILLAR,			// キャタピラ
		TANK_BASE,					// 本体
		TANK_TOP,					// 砲塔
		TANK_BARREL,				// 砲身
		NONE,						// なし
		END,						// 終了
		PARTSMAX					// パーツの最大数
	};

	// 構造体型タグ
	struct	TankInitData {
		Tank::TANKPARTS		ParentObjectNo;		// 親オブジェクトＮｏ
		Tank::TANKPARTS		ModelNo;			// モデル番号
		XMFLOAT3			FirstPosition;		// 最初の位置
		XMFLOAT3			FirstAngle;			// 最初の角度
	};

	// タンクのモデル名リスト
	struct	TankModelData {
		Tank::TANKPARTS		ModelNo;					// モデル番号
		const char*			XfileName;					// Ｘファイル名
	};

	// 初期配置データ
	static TankInitData	ObjectInitData[];

	// モデルファイル名リスト
	static TankModelData			XFileData[];

	enum TYPE {
		PLAYER = 0,
		ENEMY
	};

	float				radius;		//半径
	XMFLOAT3			angle;							// 回転角
	bool				keyInput;						//キー入力したか
	TYPE				type;							//タイプ
	float				speed;							// スピード
	static constexpr float ROTATIONRATE = 0.5f;				//回転角度
	std::mt19937	mt;										//破壊時用乱数

public:

	static constexpr float RANGE_STAGE = 450.0f;		//行動範囲

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Tank();
	Tank(TYPE);
	Tank(IScene*);

	/// <summary>
	/// モデルをセット
	/// </summary>
	/// <param name="p">モデル </param>
	/// <param name="idx">パーツの番号　</param>
	void SetModel(CModel* p, int idx) {
		models[idx] = p;
	}

	/// <summary>
	/// バレルの行列取得
	/// </summary>
	/// <returns>行列</returns>
	const XMFLOAT4X4& GetTankBarrelMtx() const { return mtxParentChild[TANKPARTS::TANK_BARREL]; }

	/// <summary>
	/// 戦車のOBB取得
	/// </summary>
	/// <returns></returns>
	const COBB::OBBinfo& GetOBB() const { return obb->GetOBB(); }

	void SetPos(const XMFLOAT3& _pos) {
		pos = _pos;
		mtxlocalpose[TANKPARTS::TANK_CATERPILLAR]._41 = pos.x;
		mtxlocalpose[TANKPARTS::TANK_CATERPILLAR]._42 = pos.y;
		mtxlocalpose[TANKPARTS::TANK_CATERPILLAR]._43 = pos.z;
	}

	/// <summary>
	/// ランダム値セット
	/// </summary>
	/// <param name="_rnd"></param>
	void SetRand(const std::mt19937& _rnd) {
		mt = _rnd;
	}

	bool Init();
	void Draw();
	void DrawModel();
	void DrawShadow();
	void Update();

	void Finalize();

	//敵用
	void UpdateChase(XMFLOAT4X4 _playermtx);
	void UpdateFree();
	void UpdateCycle();

	//爆発時の値セット
	void PrepareDestroy();

	//爆発演出
	bool DestroyEffect();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	//砲身の行列取得
	const XMFLOAT4X4 GetPipeMtx() const;

	void UpdateSelect();

private:

	enum IKPOS {
		RIGHTUP,
		RIGHTDOWN,
		LEFTUP,
		LEFTDOWN,
	};

	/// <summary>
	/// ローカルポーズを更新する
	/// </summary>
	void UpdateLocalpose();	

	void UpdateEnemyLocalpose(XMFLOAT4X4 _playermtx);
	void UpdateEneLocalpose();

	/// <summary>
	/// 親子関係を考慮した行列を作成
	/// </summary>
	void CaliculateParentChildMtx();					

	/// <summary>
	/// 煙
	/// </summary>
	void GenerateSmoke();

	CModel*				models[TANKPARTS::PARTSMAX];
	XMFLOAT4X4			mtxlocalpose[PARTSMAX];				// 自分のことだけを考えた行列
	XMFLOAT4X4			mtxParentChild[PARTSMAX];			// 親子関係を考慮した行列
	
	XMFLOAT3            ikpos[4];							//IK用支点位置

	COBB*               obb;								//OBB
	
	static constexpr float ACCEL = 0.01f;					//加速度							
	static constexpr float FIRSTSP = 0.1f;					//初速度
	static constexpr float MAXSPEED = 0.3f;					//最高速度
	static constexpr float PIPELENGTH = 20.0f;				//パイプの長さ
	
	float move = 0;											//移動量

	float destroyRotx = 0;									//破壊時回転角
	float destroyRoty = 0;
	float destroyRotz = 0;
	float destroySpeed = 0;
	uint16_t destroyCnt = 0;

	std::vector<std::unique_ptr<Fire_Billboard>> m_fireList;
	std::vector<std::unique_ptr<Smoke_Billboard>> m_smokeList;

	uint16_t m_fireInterval = 6;
	uint16_t m_fireCnt;
	bool m_fireFlg;
	float m_fireReactionAngle = 1.0f;

	XMFLOAT4X4 m_fireMtx;

	//Lerp用
	XMFLOAT3 lerpstart;
	XMFLOAT3 lerpend;
};