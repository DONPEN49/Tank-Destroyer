#pragma once

#include <vector>
#include "Singleton.h"

class Manager : public Singleton<Manager> {
public:

	friend class Singleton<Manager>;

	Manager(class Game_Manager* _game);
	virtual ~Manager() = 0;

	virtual bool Init() = 0;
	//virtual void Input() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;

private:
	class Game_Manager* game;
};
