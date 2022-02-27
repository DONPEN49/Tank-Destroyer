#pragma once
#include "Base_UI.h"
#include "Application.h"

class LaderEne_UI : public UI {

public:
	LaderEne_UI() {};
	~LaderEne_UI() {};

	bool Init()override;
	void Update() {};
	void Update(XMFLOAT4X4 _playermtx);
	void Draw()override;
	void Finalize() {};

	/// <summary>
	/// 敵の位置セット
	/// </summary>
	/// <param name="_enepos"></param>
	void SetEnePos(const XMFLOAT3 _enepos) {
		enepos = _enepos;
	}

private:

	bool drawflg;

	//レーダーの真ん中
	XMFLOAT2 centerpos = { Application::CLIENT_WIDTH / 2 - 530, Application::CLIENT_HEIGHT / 2 - 250 };
	XMFLOAT3 enepos;

	static constexpr float SERCHRANGE = 200.0f;
	float radius = 6.0f;
};