#pragma once
#include "Base_Billboard.h"

class Exp_Billboard :public Billboard {

public:
	void Init();
	void Update();
	void Draw();

	bool isDraw;

private:
	const float ANIMESPEED = 0.3f;		//�A�j���[�V�����X�s�[�h

	//UVid
	float idU;
	float idV;
};
