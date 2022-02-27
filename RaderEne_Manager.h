#pragma once

#include <vector>
#include <memory>
#include "RaderEne_UI.h"
#include "Singleton.h"

using namespace DirectX;


class RaderEne_Manager : public Singleton<RaderEne_Manager> {

public:
	void Init();
	void Update() {};
	void Update(DirectX::XMFLOAT4X4 _playermtx);
	void Draw();

private:

	friend class Singleton<RaderEne_Manager>;

	void Generate();

	std::vector<std::unique_ptr<LaderEne_UI>> enelist;
};
