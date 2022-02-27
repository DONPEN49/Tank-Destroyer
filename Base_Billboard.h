#pragma once
#include <DirectXMath.h>
#include "CBillBoard.h"

class Billboard {

public:

	CBillBoard* bil;

	Billboard() {};
	virtual ~Billboard() {};
	
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//ˆÊ’u‚ðÝ’è
	void SetPos(DirectX::XMFLOAT3 _pos) {
		bil->SetPosition(_pos.x, _pos.y, _pos.z);
	}

	void SetBillboard(CBillBoard* _bil) {
		bil = _bil;
	}

	void SetUV(float u[], float v[]) {
		bil->SetUV(u, v);
	}

	void SetSize(float _width, float _height) {
		bil->SetSize(_width, _height);
	}

	void SetColor(DirectX::XMFLOAT4 _color) {
		bil->SetColor(_color);
	}
};

