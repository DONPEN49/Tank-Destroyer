#pragma once
#include <vector>
#include "CModel.h"
#include "Object.h"

class Stage_Object : public Object{

public:
	bool Init() { return false; };
	bool Init(std::string _modelname);
	void Update() {};
	void Draw();
	void DrawShadow();
	void DrawModel();
	void Finalize();

private:
	CModel* model;
	void SetModel(CModel* _model) { model = _model; }
};