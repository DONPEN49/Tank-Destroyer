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
		DEFAULTSHADOW,	//�e��������
		NOSHADOW,		//�e�����Ȃ�
		WITHSHADOW		//�e������i���f���ɂ����f�j
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
	
	//�`�悷�邩
	bool isDraw;

	RENDER render;

	//�^�O
	std::string tag;

	//�}�l�[�W���[
	IScene* scene;

protected:

	/// <summary>
	/// ���[���h�s��
	/// </summary>
	XMFLOAT4X4 mtx;

	/// <summary>
	/// �ʒu
	/// </summary>
	XMFLOAT3 pos;
};



