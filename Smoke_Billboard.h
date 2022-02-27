#pragma once
#include "Base_Billboard.h"

class Smoke_Billboard : public Billboard {

public:

	Smoke_Billboard();
	Smoke_Billboard(XMFLOAT3);
	~Smoke_Billboard() {};

	void Init();
	void Update();
	void Draw();

	bool		isDraw;
private:

	static constexpr float ANIMESPEED = 0.1f;
	float		speed;
	XMFLOAT3	pos;
	float idU;
	float idV;
};