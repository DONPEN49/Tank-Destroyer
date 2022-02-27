#include "Base_UI.h"
#include "IScene.h"

UI::UI()
	:tag("no name")
{
}

UI::UI(IScene& _manager)
	:scene(&_manager)
	,tag("no name")
{
	scene->AddSceneUI(this);
}
