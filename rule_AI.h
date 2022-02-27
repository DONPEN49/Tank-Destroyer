#pragma once
#include <vector>
#include <functional>
#include <algorithm>
#include "Enemy.h"
#include "Player.h"




struct ruleinfo {
	uint32_t	priority;									// 優先度
	std::function<bool(Enemy*, Player*)> rulefunc;			// ルール判定
	std::function<void(Enemy*, Player*)> ruleactionfunc;	// ルール実行

	bool operator<(const ruleinfo& another) const {
		//メンバ変数であるnum1で比較した結果を
		//この構造体の比較とする(昇順)
		return priority < another.priority;
	}
};

ruleinfo ruleselector(Enemy* e, Player* p);

/// <summary>
/// 範囲外
/// </summary>
bool Checkrule1(Enemy* e, Player* p);
void rule1action(Enemy*, Player*);

/// <summary>
/// 範囲内
/// </summary>
bool Checkrule2(Enemy* e, Player* p);
void rule2action(Enemy*, Player*);

/// <summary>
/// 追跡
/// </summary>
bool Checkrule3(Enemy* e, Player* p);
void rule3action(Enemy*, Player*);


static std::vector<ruleinfo> rulelist = {
	{1,Checkrule1,rule1action},
	{3,Checkrule2,rule2action},
	{2,Checkrule3,rule3action},
};




