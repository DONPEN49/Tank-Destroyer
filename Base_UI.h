#pragma once
#include <string>
#include "quad2d.h"

using namespace DirectX;

class UI {
protected:

	Quad2D* quad2d;

	/// <summary>
	/// 幅
	/// </summary>
	float width;

	/// <summary>
	/// 高さ
	/// </summary>
	float height;

	/// <summary>
	/// 位置
	/// </summary>
	XMFLOAT3 pos;

	class IScene* scene;

	/// <summary>
	/// 透明度
	/// </summary>
	float alpha = 1.0f;

public:

	static constexpr uint32_t FULLWIDTH = 36;
	static constexpr uint32_t FULLHEIGHT = 30;

	BLENDSTATE blend;

	/// <summary>
	/// タグ
	/// </summary>
	std::string tag;


	/// <summary>
	/// コンストラクタ
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
	/// 位置設定
	/// </summary>
	/// <param name="_pos">位置</param>
	void SetPos(const XMFLOAT3 _pos) {
		quad2d->SetPosition(_pos.x, _pos.y, _pos.z);
		pos = _pos;
	}

	/// <summary>
	/// サイズ設定
	/// </summary>
	/// <param name="_width">幅</param>
	/// <param name="_height">高さ</param>
	void SetScale(const float _width, const float _height) {
		quad2d->SetScale(_width, _height, 1.0f);
		width = _width;
		height = _height;
	}

	/// <summary>
	/// データ設定
	/// </summary>
	void SetQuad2D(Quad2D* _quad2d) {
		quad2d = _quad2d;
	}

	/// <summary>
	/// 頂点データ更新
	/// </summary>
	/// <remarks> 特に変更がない場合のみ呼び出す </remarks>
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
	/// アルファ値セット
	/// </summary>
	void SetAlpha(float _alphe) { alpha = _alphe; };

};