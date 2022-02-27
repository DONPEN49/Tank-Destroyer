#pragma once
#include <vector>
#include "Singleton.h"
#include "Base_UI.h"

class UI_Manager : public Singleton<UI_Manager> {

private:

	friend class Singleton<UI_Manager>;

	/// <summary>
	/// UIŠi”[ƒŠƒXƒg
	/// </summary>
	std::vector<UI*> uilist;

public:

	bool Init();
	void Update();
	void Draw();
	void Exit();
	void AddUI(UI*);

};
