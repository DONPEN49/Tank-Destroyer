#pragma once
#include "Object.h"

class CModel;
class IScene;

class Skydome : public Object {

public:
	
	Skydome();
	Skydome(IScene&);

	bool Init();
	void Update();
	void Draw();
	void DrawShadow();
	void DrawModel();
	
	void SetModel(CModel* _model) { model = _model; };

private:

	CModel* model;
};