#pragma once
#include "Base_UI.h"

class Crosshair_UI : public UI {

public:
	Crosshair_UI() {};
	Crosshair_UI(class IScene&);

	bool Init() override;
	void Update() {};
	void Draw()override;
	void Finalize() {};
};