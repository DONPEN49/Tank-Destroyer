#pragma once
#include <unordered_map>
#include "BoundingSphere.h"
#include "Singleton.h"
#include "ModelMgr.h"



class BoundingSphere_Manager : public Singleton<BoundingSphere_Manager> {

private:

	friend class Singleton<BoundingSphere_Manager>;

	std::unordered_map<
		std::string,							// キー
		std::unique_ptr<BoundingSphere>			// データ
	> bshashmap;

public:

	/// <summary>
	/// BS一斉生成
	/// </summary>
	void AllBSCreate();

	/// <summary>
	/// BSを生成
	/// </summary>
	/// <param name="_texturefilename">ファイル名</param>
	bool CreateBoundingSphere(std::string _filename) {

		std::unique_ptr<BoundingSphere> bs;

		bs = std::make_unique<BoundingSphere>();

		//モデルの頂点を抜き出す
		CModel* bsmodel;
		bsmodel = ModelMgr::GetInstance().GetModelPtr(_filename);
		if (bsmodel == nullptr) {
			char str[128];
			sprintf_s<128>(str, "%s", _filename.c_str());
			MessageBox(nullptr, str, "bs create error", MB_OK);
			assert(false);
			return false;
		}

		const ModelData& md = bsmodel->GetModelData();
		const std::vector<Mesh>& meshes = md.GetMeshes();

		std::vector<XMFLOAT3> vertices;

		for (auto m : meshes) {
			for (auto v : m.m_vertices) {
				vertices.emplace_back(v.m_Pos);
			}
		}

		//BS初期化
		bs->Init(vertices, XMFLOAT4(1, 1, 1, 0.3f));

		//コンテナ格納
		bshashmap[_filename].swap(bs);

		return true;
	}

	/// <summary>
	/// BS取得
	/// </summary>
	/// <param name="_key">ファイル名</param>
	BoundingSphere* GetBoundingSphere(std::string _key) {
		// 存在するかを確かめる
		auto it = bshashmap.find(_key);
		if (it == bshashmap.end()) {

			char str[128];
			sprintf_s<128>(str, "%s", _key.c_str());
			MessageBox(nullptr, str, "Get BS error", MB_OK);
			assert(false);
			return nullptr;

		}
		return bshashmap[_key].get();
	}

	/// <summary>
	/// 半径取得
	/// </summary>
	/// <param name="_key">ファイル名</param>
	/// <returns>半径</returns>
	float GetBSRadius(std::string _key) {
		auto it = bshashmap.find(_key);
		if (it == bshashmap.end()) {
			return 0;
		}
		return bshashmap[_key].get()->GetR();
	}

};
