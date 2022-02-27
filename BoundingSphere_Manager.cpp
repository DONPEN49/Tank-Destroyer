#include "BoundingSphere_Manager.h"


void BoundingSphere_Manager::AllBSCreate() {
	CreateBoundingSphere("assets/shot.fbx");

	CreateBoundingSphere("assets/tank/tank10_base.x");
	CreateBoundingSphere("assets/dice/Wall.x");
}