#pragma once
#include	<string>
#include	<unordered_map>
#include	<functional>
#include	<iostream>
#include	<memory>
#include	<directxmath.h>
#include	<chrono>
#include	<thread>
#include	"Application.h"
#include	"uncopyable.h"
#include	"StopWatch.h"
#include	"quad2d.h"
#include	"IScene.h"

class IScene;

class SceneManager {
private:
	// 遷移方法定義列挙クラス
	enum class TransitionState
	{
		None,
		FadeIn,
		Active,
		FadeOut,
		FadeInOut,
	};

	// シーン保管
	std::unordered_map<std::string, std::unique_ptr<IScene>>	m_scenefactories;
	std::string m_currentscenekey;			// カレントシーンのキー
	std::string m_nextscenekey;				// 次シーン

	// 画面遷移法
	TransitionState m_transitionState = TransitionState::None;

	int32_t	m_transitionTimeMillisec = 1000;	// 遷移時間（デフォルト：１０００ミリ秒）

	Stopwatch m_stopwatch;						// ストップウオッチ

	DirectX::XMFLOAT4 m_fadeColor = DirectX::XMFLOAT4(0, 0, 0, 1);		// フェードカラー

	bool m_crossFade = false;

	void updateSingle();					// ひとつの遷移方法で更新
	void updateCross();						// クロスフェードで更新
	void drawScene();						// カレントシーン描画
	void SetTransitionState(SceneManager::TransitionState ts);	// 遷移方法セット
public:
	template <class sceneclass>
	void add(std::string key);
	// シーンを変更する1
	template <class sceneclass>
	void changeScene(std::string key);

	// シーンを変更する2
	template <class sceneclass>
	bool changeScene(std::string key, int32_t transitionTimeMillisec, bool crossFade);

	// カレントシーンをセットする
	void setCurrentScene(std::string key);
	void Update();
	void Draw();

	std::string GetCurrentSceneKey();
	DirectX::XMFLOAT4 GetFadeColor();
	void SetFadeColor(DirectX::XMFLOAT4 color);
};

template <class sceneclass>
void SceneManager::add(std::string key) {
	m_scenefactories[key] = std::move(std::make_unique<sceneclass>());
	m_scenefactories[key].get()->Init();
	m_scenefactories[key].get()->SetSceneManager(this);

	SetTransitionState(SceneManager::TransitionState::Active);
}

// シーンを変更する
template <class sceneclass>
void SceneManager::changeScene(std::string key) {

	// 指定されたキーが存在しているか
	auto it = m_scenefactories.find(key);
	if (it != m_scenefactories.end()) {
		// 存在していた場合
	}
	else {
		// 存在していなかった場合生成

		m_scenefactories[key] = std::move(std::make_unique<sceneclass>());
		m_scenefactories[key].get()->Init(this);
		m_scenefactories[key].get()->SetSceneManager(this);
	}

	// カレントシーンセット	
	setCurrentScene(key);
}

template <class sceneclass>
bool SceneManager::changeScene(std::string key, int32_t transitionTimeMillisec, bool crossFade) {
	// 現在のシーンと変更後のシーンが同じであればクロスフェード行わない
	if (key == m_currentscenekey)
	{
		crossFade = false;
	}
	// 指定されたキーが存在しているか
	auto it = m_scenefactories.find(key);
	if (it != m_scenefactories.end()) {
		// 存在していた場合
	}
	else {
		// 存在していなかった場合生成
		m_scenefactories[key] = std::move(std::make_unique<sceneclass>());
		// シーン初期化
		m_scenefactories[key].get()->Init();
		// シーンマネージャのポインタをセット
		m_scenefactories[key].get()->SetSceneManager(this);
	}
	m_nextscenekey = key;
	m_crossFade = crossFade;
	// クロスフェードを行う
	if (crossFade)
	{
		// 遷移時間をメンバ変数にセット
		m_transitionTimeMillisec = transitionTimeMillisec;
		m_transitionState = TransitionState::FadeInOut;
		m_stopwatch.restart();
	}
	else
	{
		//　フェードアウト後にフェードインなので遷移時間を2で割る
		m_transitionTimeMillisec = (transitionTimeMillisec / 2);
		m_transitionState = TransitionState::FadeOut;

		m_stopwatch.restart();
	}

	return true;
}

