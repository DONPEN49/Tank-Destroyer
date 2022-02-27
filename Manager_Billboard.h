#pragma once
#include	<memory>
#include	<wrl/client.h>
#include	<unordered_map>
#include    "CBillBoard.h"

using Microsoft::WRL::ComPtr;



class Manager_Billboard {
private:
	Manager_Billboard() {};

	std::unordered_map<
		std::string,						// キー
		std::unique_ptr<CBillBoard>			// データ
	> m_billboardhashmap;
public:
	Manager_Billboard(const Manager_Billboard&) = delete;
	Manager_Billboard& operator=(const Manager_Billboard&) = delete;
	Manager_Billboard(Manager_Billboard&&) = delete;
	Manager_Billboard& operator=(Manager_Billboard&&) = delete;

	static Manager_Billboard& GetInstance() {
		static Manager_Billboard Instance;
		return Instance;
	}

	virtual ~Manager_Billboard() {
		Finalize();
	}

	void LoadAllBillboard();

	//ビルボードの後始末
	void Finalize() {
		for (auto itr = m_billboardhashmap.begin(); itr != m_billboardhashmap.end(); ++itr) {
			(itr->second)->~CBillBoard();
		}
	}

	// ビルボードをロードする
	bool LoadBillboard(std::string texturefilename) {

		std::unique_ptr<CBillBoard>	b;

		b = std::make_unique<CBillBoard>();

		float u[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
		float v[4] = { 1.0f, 0.0f, 1.0f, 0.0f };
		
		//とりあえず初期化しとく
		bool sts = b->Init(0, 0, 0, 0, 0, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		b->SetUV(u, v);
		if (!sts) {
			return false;
		}

		sts = b->LoadTexTure(texturefilename.c_str());
		if (!sts) {
			char str[128];
			sprintf_s<128>(str, "%s", texturefilename.c_str());
			MessageBox(nullptr, str, "Load Billboard Texture error", MB_OK);
			assert(false);
			return false;
		}

		// unordered_mapコンテナに格納
		m_billboardhashmap[texturefilename].swap(b);

		return true;
	}

	//ビルボードのポインタを取得する
	CBillBoard* GetBillboardPtr(std::string key) {
		// 存在するかを確かめる
		auto it = m_billboardhashmap.find(key);
		if (it == m_billboardhashmap.end()) {
			char str[128];
			sprintf_s<128>(str, "%s", key.c_str());
			MessageBox(nullptr, str, "Get Billboard Data error", MB_OK);
			assert(false);
			return nullptr;
		}
		return m_billboardhashmap[key].get();
	}
};