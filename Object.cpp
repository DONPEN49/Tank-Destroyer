#include "Object.h"
#include "IScene.h"
#include "dx11mathutil.h"

Object::Object()
	: isDraw(true)
	, tag("no name")
{
	DX11MtxIdentity(mtx);
}

Object::Object(IScene*& _manager)
	: isDraw(true)
	, scene(_manager)
	, tag("no name")
{
	DX11MtxIdentity(mtx);
	scene->AddSceneObject(this);
}