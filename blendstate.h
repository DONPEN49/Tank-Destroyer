#pragma once

enum class BLENDSTATE {
	DEFAULT = 0,
	SRCALPHA,
	ADD,
	SUB
};

// ブレンドステート初期化
void InitBlendState();

// ソースアルファブレンドステート生成
void CreateBlendStateSrcAlpha();
// 加算合成ブレンドステート生成
void CreateBlendStateOne();
// デフォルトブレンドステート生成
void CreateBlendStateDefault();
// 減算合成ブレンドステート生成
void CreateBlendStateSubtract();
void SetBlendState(BLENDSTATE bs);
