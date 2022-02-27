#pragma once
#include "Base_UI.h"

class HP_UI : public UI {

public:
	HP_UI() {};
	HP_UI(class IScene&);

	bool Init();
	void Update();
};
