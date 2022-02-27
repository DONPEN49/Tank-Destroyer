#pragma once
#include "Base_UI.h"

class Rader_UI : public UI {

public:
	Rader_UI(class IScene*);
	Rader_UI() {};

	bool Init()override;
	void Update() {};
	void Draw()override;
	void Finalize() {};

};