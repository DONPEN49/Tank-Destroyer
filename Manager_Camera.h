#pragma once
#include <DirectXMath.h>
#include <array>
#include "Singleton.h"

using namespace DirectX;

class Manager_Camera : public Singleton<Manager_Camera>
{
private:

	void TPSCamera(XMFLOAT4X4 _playermtx);
	void FPSCamera(XMFLOAT4X4 _playermtx);
	void DelayCamera(XMFLOAT4X4 _playermtx);

	static int CameraFlg;

	//目標を基準とした位置
	XMFLOAT3 localPos;

	//DelayCamera用変数
	int svidx = 0;
	bool firstflag = false;
	std::array<XMFLOAT3, 100> axisz;
	std::array<XMFLOAT3, 100> axisy;
	std::array<XMFLOAT3, 100> pos;

	//ShakeCamera用
	int frame = 0;
	static constexpr int MAXFRAME = 10;


public:
	//カメラ切り替え
	void UpdateCamera(XMFLOAT4X4 _playermtx);

	void SetLocalPos(const XMFLOAT3& _pos) { localPos = _pos; }

	void ShakeCamera(XMFLOAT4X4 _playermtx);

	void UpdateSphereCamera(float _radius, float _elevation, float _azimuth, const XMFLOAT3& _lookat);

	bool shakeflag = false;

	//デフォルトの位置
	static constexpr float DEFPOSX = 0.0f;
	static constexpr float DEFPOSY = 10.0f;
	static constexpr float DEFPOSZ = 35.0f;
};

