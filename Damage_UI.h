#pragma once
#include "Base_UI.h"

class Damage_UI : public UI {

public:
	Damage_UI();
	Damage_UI(class IScene*);

	bool Init()override;
	void Update()override;
	void Draw()override;
	void Finalize() {};

private:
	
	static constexpr float RADIUS = 5.0f;

	uint16_t m_life;

};