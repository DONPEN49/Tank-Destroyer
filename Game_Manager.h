#pragma once

#include <string>
#include <vector>
#include "Object.h"

class Player;
class Skydome;

class Game_Manager
{
public:
	Game_Manager();
	bool Init();
	void Input();
	void Update();
	void DrawObject();
	void Exit();

	void LoadDate();

	void AddObject(Object* _obj) {
		objlist.emplace_back(_obj);
	}

private:
	
	void UnloadData();
	

	std::vector<Object*> objlist;

	Skydome* skydome;
};