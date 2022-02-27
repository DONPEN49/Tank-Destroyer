#pragma once
#include <DirectXMath.h>
#include <string>

using namespace DirectX;
class IScene;

class Object
{
public:

	enum RENDER {
		DEFAULT,		
		DEFAULTSHADOW,	//影だけつける
		NOSHADOW,		//影をつけない
		WITHSHADOW		//影をつける（モデルにも反映）
	};

	Object();
	Object(IScene*&);
	virtual ~Object() {};

	virtual bool Init() = 0;
	
	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void DrawModel() = 0;

	virtual void DrawShadow() = 0;

	//getter,setter
	const XMFLOAT3& GetPostion() const { return pos; }
	void SetPos(const XMFLOAT3& _pos) { pos = _pos; }
	const XMFLOAT4X4& GetMatrix() const { return mtx; }
	void SetMatrix(const XMFLOAT4X4& _mtx) { mtx = _mtx; }
	
	//描画するか
	bool isDraw;

	RENDER render;

	//タグ
	std::string tag;

	//マネージャー
	IScene* scene;

protected:

	/// <summary>
	/// ワールド行列
	/// </summary>
	XMFLOAT4X4 mtx;

	/// <summary>
	/// 位置
	/// </summary>
	XMFLOAT3 pos;
};



