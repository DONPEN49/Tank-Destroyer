#pragma once
#include "Object.h"

class CModel;
class IScene;

class LandScape : public Object {

public:

	LandScape() {};
	LandScape(IScene&);
	~LandScape();

	bool Init();

	void Update() {};

	void Draw() {};

	void DrawModel();

	void DrawShadow();


	void SetModel(CModel* _model) { model = _model; }

private:
	CModel* model;
};
