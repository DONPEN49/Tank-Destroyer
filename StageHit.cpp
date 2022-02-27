//*****************************************************************************
//!	@file	StageHit.cpp
//!	@brief	
//!	@note	ステージとの当たり判定
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<DirectXMath.h>
#include	"StageHit.h"
#include	"collision.h"
#include	"CModel.h"

using namespace DirectX;

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
PLANEINFO*		g_PlaneInfo=nullptr;				// 平面ポリゴン情報
int				g_NumFaces;							// 三角形ポリゴン数

//==============================================================================
//!	@fn		StageHitInit
//!	@brief　地形との当たり判定初期化処理
//!	@param  CModelポインタ
//!	@param	デバイスオブジェクト
//!	@retval	なし
//==============================================================================
void StageHitInit(CModel* pmodel){

	std::vector<XMFLOAT3> vertices;		// 頂点データ
	std::vector<int> indices;			// インデックスデータ

	const ModelData& md = pmodel->GetModelData();
	const std::vector<Mesh>& meshes = md.GetMeshes();

	// 頂点データを取り出す
	for (auto m : meshes) {
		for (auto v : m.m_vertices) {
			vertices.emplace_back(v.m_Pos);
		}
	}

	// インデックスデータを取り出す
	for (auto m : meshes) {
		for (auto idx : m.m_indices) {
			indices.emplace_back(idx);
		}
	}

	// 平面の方程式を作成
	CreateStagePlaneInfoAll(vertices,indices);
}

//==============================================================================
//!	@fn		StageHitExit
//!	@brief　地形との当たり判定終了処理
//!	@retval	なし
//==============================================================================
void StageHitExit(){
	if(g_PlaneInfo != nullptr){
		delete[] g_PlaneInfo;
	}
}

//==============================================================================
//!	@fn		CreateStagePlaneInfoAll
//!	@brief　指定されたメッシュのポリゴンについて当たり判定を行うための平面方程式情報を生成する
//!	@param	メッシュ
//!	@param	デバイスオブジェクト
//!	@retval	なし
//==============================================================================
/*
void CreateStagePlaneInfoAll(DatFileLoader*	datfiledata)
{
	X::Vertex*				pVertices;						// 頂点情報格納用

	unsigned int*			pIndices;						// インデックス番号
	int						idx0, idx1, idx2;				// ３角形ポリゴンの頂点インデックス格納用

	pIndices = datfiledata->m_Index;

	pVertices = datfiledata->m_Vertex;

	// ＸＹＺの座標だけが含まれるメッシュクローンを作成
	g_NumFaces = datfiledata->m_IndexSuu / 3;		// ３角形数取得

	g_PlaneInfo = new PLANEINFO[g_NumFaces];		// 面数分確保

	// ポリゴン数分ループ
	for (int i = 0; i<g_NumFaces; i++){
		idx0 = *pIndices++;							// ３角形ポリゴンの頂点インデックス取得（０番目）
		idx1 = *pIndices++;							// ３角形ポリゴンの頂点インデックス取得（１番目）
		idx2 = *pIndices++;							// ３角形ポリゴンの頂点インデックス取得（２番目）

		g_PlaneInfo[i].p0.x = (pVertices + idx0)->m_Pos.x;
		g_PlaneInfo[i].p0.y = (pVertices + idx0)->m_Pos.y;
		g_PlaneInfo[i].p0.z = (pVertices + idx0)->m_Pos.z;

		g_PlaneInfo[i].p1.x = (pVertices + idx1)->m_Pos.x;
		g_PlaneInfo[i].p1.y = (pVertices + idx1)->m_Pos.y;
		g_PlaneInfo[i].p1.z = (pVertices + idx1)->m_Pos.z;

		g_PlaneInfo[i].p2.x = (pVertices + idx2)->m_Pos.x;
		g_PlaneInfo[i].p2.y = (pVertices + idx2)->m_Pos.y;
		g_PlaneInfo[i].p2.z = (pVertices + idx2)->m_Pos.z;

		//平面の方程式を作成する
		CreatePlaneInfo(g_PlaneInfo[i].p0, g_PlaneInfo[i].p1, g_PlaneInfo[i].p2, g_PlaneInfo[i].plane);
	}
}
*/

//==============================================================================
//!	@fn		CreateStagePlaneInfoAll
//!	@brief　指定されたメッシュのポリゴンについて当たり判定を行うための平面方程式情報を生成する
//!	@param	頂点データ
//!	@param	インデックスデータ
//!	@retval	なし
//==============================================================================
void CreateStagePlaneInfoAll(std::vector<XMFLOAT3>& vertices,std::vector<int>& indices) {

	g_NumFaces = indices.size() / 3;				// ３角形数計算

	g_PlaneInfo = new PLANEINFO[g_NumFaces];		// 面数分確保

	// 面数分ループ
	for (int facecnt = 0; facecnt < g_NumFaces; facecnt++){

		g_PlaneInfo[facecnt].p0.x = vertices[indices[facecnt *3]].x;
		g_PlaneInfo[facecnt].p0.y = vertices[indices[facecnt *3]].y;
		g_PlaneInfo[facecnt].p0.z = vertices[indices[facecnt *3]].z;

		g_PlaneInfo[facecnt].p1.x = vertices[indices[facecnt *3+1]].x;
		g_PlaneInfo[facecnt].p1.y = vertices[indices[facecnt *3+1]].y;
		g_PlaneInfo[facecnt].p1.z = vertices[indices[facecnt *3+1]].z;

		g_PlaneInfo[facecnt].p2.x = vertices[indices[facecnt *3+2]].x;
		g_PlaneInfo[facecnt].p2.y = vertices[indices[facecnt *3+2]].y;
		g_PlaneInfo[facecnt].p2.z = vertices[indices[facecnt *3+2]].z;

		//平面の方程式を作成する
		CreatePlaneInfo(g_PlaneInfo[facecnt].p0, g_PlaneInfo[facecnt].p1, g_PlaneInfo[facecnt].p2, g_PlaneInfo[facecnt].plane);
	}
}

//==============================================================================
//!	@fn		SearchAllSurface
//!	@brief　指定されたメッシュすべての３角形ポリゴンについての交点を求める
//!	@param	Ｘ座標
//!	@param	Ｚ座標
//!	@param	交点
//!	@retval	true 交点あり false 交点なし
//==============================================================================
bool SearchAllSurface(float x, float z, XMFLOAT3& ans){

	XMFLOAT3		DownVector(0, 1, 0);				// 直線の方向ベクトル
	XMFLOAT3		CrossPoint;							// 交点
	XMFLOAT3		CurrentPosition(x, 0, z);			// 現在位置（高さ情報なし）

	bool FindFlag = false;								// ポリゴンが見つかったかどうか
	bool status;										

	// 全ポリゴン数分ループ
	for (int i = 0; i<g_NumFaces; i++){
		// 平面と直線の交点を求める
		status = LinetoPlaneCross(g_PlaneInfo[i].plane,	//平面の方程式
			CurrentPosition,							//直線の起点（球の現在位置）
			DownVector,									//直線の方向ベクトル
			CrossPoint);								//交点座標
		if (status){
			// 交点が３角形の内部にあるか？
			if (CheckInTriangle(g_PlaneInfo[i].p0, g_PlaneInfo[i].p1, g_PlaneInfo[i].p2, CrossPoint)){
				CurrentPosition.y = CrossPoint.y;
				FindFlag = true;
				break;
			}
		}
	}
	if (FindFlag){
		ans = CurrentPosition;
		return true;
	}
	else{
		return false;
	}
}

//******************************************************************************
//	End of file.
//******************************************************************************