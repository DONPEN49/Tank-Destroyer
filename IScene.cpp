#include	"IScene.h"
#include	"SceneManager.h"

Quad2D		g_quadfadein;
Quad2D		g_quadfadeout;

IScene::IScene() {
}

void IScene::updateFadeIn(double t) {
	std::cout << "updateFadeIn:" << t << std::endl;
	Update();
}

void IScene::updateFadeOut(double t) {
	std::cout << "updateFadeOut:" << t << std::endl;
	Update();
}

void IScene::drawFadeIn(double t)
{
	static bool first = true;
	// �����Ő^�������瓧���փA���t�@�l��ω������Ȃ����ʃT�C�Y�̋�`��`�悷��
	DirectX::XMFLOAT4 fadecolor = { 0,0,0,1 };

	DirectX::XMFLOAT2 uv[4] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f}
	};

	if (first) {
		g_quadfadein.Init(Application::CLIENT_WIDTH,
			Application::CLIENT_HEIGHT,
			DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w), uv);
			g_quadfadein.LoadTexture("assets/white.bmp");
			first = false;
	}

	std::cout << "DrawFadeIn:" << t << std::endl;

	Draw();

	fadecolor = manager->GetFadeColor();

	fadecolor.w = fadecolor.w*(1.0 - t);

	TurnOffZbuffer();

	g_quadfadein.SetPosition(
		Application::CLIENT_WIDTH / 2,
		Application::CLIENT_HEIGHT / 2, 0);
	
	g_quadfadein.updateVertex(
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w), uv);

	g_quadfadein.updateVbuffer();
		g_quadfadein.Draw();
	
	TurnOnZbuffer();
}

void IScene::drawFadeOut(double t)
{
	static bool first = true;
	// �����Ő^�������瓧���փA���t�@�l��ω������Ȃ����ʃT�C�Y�̋�`��`�悷��
	DirectX::XMFLOAT4 fadecolor = { 0,0,0,1 };
	DirectX::XMFLOAT2 uv[4] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f}
	};

	if (first) {
		g_quadfadeout.Init(Application::CLIENT_WIDTH,
			Application::CLIENT_HEIGHT,
			DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w), uv);
			g_quadfadeout.LoadTexture("assets/white.bmp");
			first = false;
	}
	std::cout << "DrawFadeOut:" << t << std::endl;

	TurnOnZbuffer();
	Draw();
	TurnOffZbuffer();

	// �����œ�������^�����փA���t�@�l��ω������Ȃ����ʃT�C�Y�̋�`��`�悷��
	fadecolor = manager->GetFadeColor();

	fadecolor.w = t;

	g_quadfadeout.updateVertex(
			Application::CLIENT_WIDTH,
			Application::CLIENT_HEIGHT,
			DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w), uv);

	g_quadfadeout.updateVbuffer();

	g_quadfadeout.SetPosition(
			Application::CLIENT_WIDTH / 2,
			Application::CLIENT_HEIGHT / 2, 0);

	g_quadfadeout.Draw();

	TurnOnZbuffer();
}