#pragma once
#include <string>
#include "quad2d.h"

using namespace DirectX;

class UI {
protected:

	Quad2D* quad2d;

	/// <summary>
	/// ��
	/// </summary>
	float width;

	/// <summary>
	/// ����
	/// </summary>
	float height;

	/// <summary>
	/// �ʒu
	/// </summary>
	XMFLOAT3 pos;

	class IScene* scene;

	/// <summary>
	/// �����x
	/// </summary>
	float alpha = 1.0f;

public:

	static constexpr uint32_t FULLWIDTH = 36;
	static constexpr uint32_t FULLHEIGHT = 30;

	BLENDSTATE blend;

	/// <summary>
	/// �^�O
	/// </summary>
	std::string tag;


	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UI();
	UI(IScene&);

	virtual bool Init() { return true; };
	virtual void Update() {};
	virtual void Draw() {
		TurnOffZbuffer();

		UpdateQuad2D();

		quad2d->Draw();

		TurnOnZbuffer();
	};

	virtual void DrawAdd() {
		TurnOffZbuffer();

		UpdateQuad2D();

		quad2d->DrawAdd();

		TurnOnZbuffer();
	};

	virtual void Finalize() {};

	/// <summary>
	/// �ʒu�ݒ�
	/// </summary>
	/// <param name="_pos">�ʒu</param>
	void SetPos(const XMFLOAT3 _pos) {
		quad2d->SetPosition(_pos.x, _pos.y, _pos.z);
		pos = _pos;
	}

	/// <summary>
	/// �T�C�Y�ݒ�
	/// </summary>
	/// <param name="_width">��</param>
	/// <param name="_height">����</param>
	void SetScale(const float _width, const float _height) {
		quad2d->SetScale(_width, _height, 1.0f);
		width = _width;
		height = _height;
	}

	/// <summary>
	/// �f�[�^�ݒ�
	/// </summary>
	void SetQuad2D(Quad2D* _quad2d) {
		quad2d = _quad2d;
	}

	/// <summary>
	/// ���_�f�[�^�X�V
	/// </summary>
	/// <remarks> ���ɕύX���Ȃ��ꍇ�̂݌Ăяo�� </remarks>
	void UpdateQuad2D() {

		XMFLOAT2 uv[4] = {
			{0.0f,0.0f},
			{1.0f,0.0f},
			{0.0f,1.0f},
			{1.0f,1.0f}
		};
		quad2d->SetPosition(pos.x, pos.y, pos.z);
		quad2d->updateVertex(static_cast<uint32_t>(width), static_cast<uint32_t>(height), XMFLOAT4(1.0f, 1.0f, 1.0f, alpha), uv);
		quad2d->updateVbuffer();
	}
	
	/// <summary>
	/// �A���t�@�l�Z�b�g
	/// </summary>
	void SetAlpha(float _alphe) { alpha = _alphe; };

};