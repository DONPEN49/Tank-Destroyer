#pragma once
#include <vector>
#include "Base_UI.h"
#include "RaderEne_UI.h"

class Lader_UI : public UI {

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Lader_UI() {};
	Lader_UI(IScene&);

	bool Init();
	void Update();
	void Draw();

private:

	std::vector<std::unique_ptr<LaderEne_UI>> enelist;
	void Generate();
};