#include "rule_AI.h"
#include "tank.h"
#include "Stage_Manager.h"

using namespace DirectX;


ruleinfo ruleselector(Enemy* e, Player* p) {

	std::vector<ruleinfo> ignitionlist;

	int index = 0;
	// ルールの条件チェック
	for (auto rule : rulelist) {
		bool sts = rule.rulefunc(e, p);
		if (sts) {
			// 発火したルールのインデックス番号を保存
			ignitionlist.emplace_back(rule);
		}
		index++;
	}

	// 発火したルールを優先順位（昇順）でソート
	std::sort(ignitionlist.begin(), ignitionlist.end());

	// 先頭のルールを返却
	return ignitionlist[0];

}

/// <summary>
/// 範囲外か
/// </summary>
bool Checkrule1(Enemy* e, Player* p) {

	//XMFLOAT3 pos = e->GetPostion();

	////float lng;

	////DX11Vec3Length(pos, lng);

	//int no = Stage_Manager::GetInstance().GetMapChipNo(pos.x, pos.z);

	////範囲外ならtrue
	///*if (lng > Enemy::RANGE_STAGE) {
	//	return true;
	//}*/

	//if (no == 9 || no == 4) {
	//	return true;
	//}
	return false;

}


/// <summary>
/// 反対を向く
/// </summary>
void rule1action(Enemy* e, Player* p) {

	e->SetState(Enemy::STATE::CYCLE);

}

/// <summary>
/// 範囲内
/// </summary>
bool Checkrule2(Enemy* e, Player* p) {

	//XMFLOAT3 pos = e->GetPostion();

	////float lng;

	////DX11Vec3Length(pos, lng);

	//int no = Stage_Manager::GetInstance().GetMapChipNo(pos.x, pos.z);

	//////範囲外ならtrue
	////if (lng < Enemy::RANGE_STAGE) {
	////	return true;
	////}

	//if (no != 9) {
	//	return true;
	//}
	return false;
}


/// <summary>
/// フリー
/// </summary>
void rule2action(Enemy* e, Player* p) {

	e->SetState(Enemy::STATE::FREE);

}

bool Checkrule3(Enemy* e, Player* p) {

	//XMFLOAT4X4 enemtx = e->GetMatrix();
	//XMFLOAT3 enepos = e->GetPostion();

	//XMFLOAT3 playerpos = p->GetPostion();

	//XMFLOAT3 axisz = { -enemtx._31,-enemtx._32 ,-enemtx._33 };

	//float eneradius = e->radius;

	//float lng;

	////球型のレイを飛ばす
	////近すぎると判定しない
	//for (int i = Enemy::RAYJUDGE; i < Enemy::RAYLENGTH; i++)
	//{
	//	//レイの位置を計算
	//	XMFLOAT3 raypos = { enepos.x + axisz.x * Enemy::RAYSIZE * i, enepos.y + axisz.y * Enemy::RAYSIZE * i , enepos.z + axisz.z * Enemy::RAYSIZE * i };

	//	DX11p2pLength(playerpos, raypos, lng);

	//	//プレイヤー発見
	//	if (lng < Enemy::RAYSIZE + eneradius) {
	//		return true;
	//	}
	//}

	return false;
}

void rule3action(Enemy* e, Player* p) {
	e->SetState(Enemy::STATE::CHASE);
}
