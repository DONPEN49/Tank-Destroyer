#pragma once
#include <unordered_map>
#include "quad2d.h"
#include "Singleton.h"

using namespace DirectX;


class Texture2D_Manager : public Singleton<Texture2D_Manager> {
private:

	std::unordered_map<
		std::string,						// キー
		std::unique_ptr<Quad2D>			// データ
	> quad2dhashmap;

	friend class Singleton<Texture2D_Manager>;

public:

	/// <summary>
	/// テクスチャ一斉読み込み
	/// </summary>
	void AllTexture2DLoad();

	/// <summary>
	/// テクスチャロード
	/// </summary>
	/// <param name="_texturefilename">ファイル名</param>
	bool LoadTexture2D(std::string _texturefilename) {

		std::unique_ptr<Quad2D> q;

		q = std::make_unique<Quad2D>();

		XMFLOAT2 uv[4] = {
			{0.0f,0.0f},
			{1.0f,0.0f},
			{0.0f,1.0f},
			{1.0f,1.0f}
		};

		//とりあえず初期化しとく
		bool sts = q->Init(0, 0, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), uv);
		if (!sts) {
			return false;
		}

		q->LoadTexture(_texturefilename.c_str());

		// unordered_mapコンテナに格納
		quad2dhashmap[_texturefilename].swap(q);
	}

	/// <summary>
	/// 読み込んでいたテクスチャデータ取得
	/// </summary>
	/// <param name="_key">ファイル名</param>
	Quad2D* GetTexture2DPtr(std::string _key) {
		// 存在するかを確かめる
		auto it = quad2dhashmap.find(_key);
		if (it == quad2dhashmap.end()) {
			char str[128];
			sprintf_s<128>(str, "%s", _key.c_str());
			MessageBox(nullptr, str, "Get Texture error", MB_OK);
			assert(false);
			return nullptr;
		}
		return quad2dhashmap[_key].get();
	}

	/// <summary>
	/// 後処理
	/// </summary>
	void Exit() {
		for (auto itr = quad2dhashmap.begin(); itr != quad2dhashmap.end(); ++itr) {
			(itr->second)->UnInit();
		}
	}
};