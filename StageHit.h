#pragma once
#include	<DirectXMath.h>
#include	<vector>
#include	"CModel.h"
#include	"collision.h"

// 地形データの面の方程式情報
struct PLANEINFO{
	PLANE		plane;				//平面の方程式
	DirectX::XMFLOAT3	p0, p1, p2;		//頂点座標
};
void StageHitInit(CModel* pmodel);
void StageHitExit();
void CreateStagePlaneInfoAll(std::vector<XMFLOAT3>& v,std::vector<int>& idx);
bool SearchAllSurface(float x, float z, DirectX::XMFLOAT3& ans);
