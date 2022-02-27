#pragma once
#include <vector>
#include "IScene.h"
#include "Base_UI.h"
#include "Object.h"

class Select_Scene : public IScene {
private:
	DirectX::XMFLOAT4X4 mtx;					// ï`âÊópçsóÒ
	class Tank* tank;

public:
	Select_Scene() {
		Dispose();
	}
	void Init();
	void Update();
	void Draw();

	void Dispose();
};