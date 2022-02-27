#include	"SceneManager.h"

// ひとつの遷移方法で更新
void SceneManager::updateSingle()
{
	// 経過時間を取得
	double elapsed = m_stopwatch.msF();

	// フェードアウト遷移　and 経過時間が遷移時間をオーバーしたとき
	if (m_transitionState == TransitionState::FadeOut
		&& elapsed >= m_transitionTimeMillisec)
	{
		// 次のシーンの更新処理を実行
		m_scenefactories[m_nextscenekey]->Update();

		// カレントシーンを次のシーンにする
		m_currentscenekey = m_nextscenekey;

		// 遷移ステートをフェードインにする
		m_transitionState = TransitionState::FadeIn;

		// ストップウォッチを再スタート
		m_stopwatch.restart();

		// 経過時間を０にもどす
		elapsed = 0.0;
	}

	// フェードイン遷移　and 経過時間が遷移時間をオーバーしたとき
	if (m_transitionState == TransitionState::FadeIn &&
		elapsed >= m_transitionTimeMillisec)
	{
		// ストップウォッチをリセット
		m_stopwatch.reset();

		// 遷移時間をＡＣＴＩＶＥにする
		m_transitionState = TransitionState::Active;
	}

	switch (m_transitionState)
	{
		case TransitionState::FadeIn:
			assert(m_transitionTimeMillisec);
			m_scenefactories[m_currentscenekey]->updateFadeIn(elapsed / m_transitionTimeMillisec);
			return;
		case TransitionState::Active:
			m_scenefactories[m_currentscenekey]->Update();
			return;
		case TransitionState::FadeOut:
			assert(m_transitionTimeMillisec);
			m_scenefactories[m_currentscenekey]->updateFadeOut(elapsed / m_transitionTimeMillisec);
			return;
		default:
			return;
	}
}

void SceneManager::updateCross()
{
	// 経過時間を取得
	const double elapsed = m_stopwatch.msF();

	// 遷移方法がクロスフェードの場合
	if (m_transitionState == TransitionState::FadeInOut)
	{
		if (elapsed >= m_transitionTimeMillisec)
		{
			m_currentscenekey = m_nextscenekey;

			m_stopwatch.reset();

			m_transitionState = TransitionState::Active;
		}
	}

	if (m_transitionState == TransitionState::Active)
	{
		m_scenefactories[m_currentscenekey]->Update();

		return;
	}
	else
	{
		const double t = elapsed / m_transitionTimeMillisec;

		m_scenefactories[m_currentscenekey]->updateFadeOut(t);

		m_scenefactories[m_nextscenekey]->updateFadeIn(t);

		return;
	}
}


// カレントシーンをセットする
void SceneManager::setCurrentScene(std::string key) {

	m_currentscenekey = key;
}

void SceneManager::Update() {

	if (m_crossFade)
	{
		return updateCross();
	}
	else
	{
		return updateSingle();
	}
}

void SceneManager::Draw() {

	drawScene();

	// カレントシーン描画
//		m_scenefactories[m_currentscenekey]->Draw();
}



void SceneManager::drawScene()
{
	// カレントシーンキーが空なら何もしない
	if (m_currentscenekey.empty())
	{
		return;
	}

	// アクティブかつ遷移時間が０なら現在シーンを描画
	if (m_transitionState == TransitionState::Active ||
		!m_transitionTimeMillisec)
	{
		m_scenefactories[m_currentscenekey]->Draw();
	}

	// 経過時間を取得
	const double elapsed = m_stopwatch.msF();
		// 遷移フェードインなら
	if (m_transitionState == TransitionState::FadeIn)
	{
		// フェードイン描画

//		TurnOffZbuffer();
		m_scenefactories[m_currentscenekey]->drawFadeIn(elapsed / m_transitionTimeMillisec);
//		TurnOnZbuffer();
	}
	// 遷移フェードアウトなら
	else if (m_transitionState == TransitionState::FadeOut)
	{
		// フェードアウト描画
//		TurnOffZbuffer();
		m_scenefactories[m_currentscenekey]->drawFadeOut(elapsed / m_transitionTimeMillisec);
//		TurnOnZbuffer();
	}
	// 遷移クロスフェードなら
	else if (m_transitionState == TransitionState::FadeInOut)
	{
		// フェードアウト描画
//		TurnOffZbuffer();
		m_scenefactories[m_currentscenekey]->drawFadeOut(elapsed / m_transitionTimeMillisec);
			// 次のシーンが設定されていれば
		if (!m_nextscenekey.empty())
		{
			// フェードイン描画
			m_scenefactories[m_nextscenekey]->drawFadeIn(elapsed / m_transitionTimeMillisec);
		}
//		TurnOnZbuffer();
	}
}

void SceneManager::SetTransitionState(SceneManager::TransitionState ts) {
	m_transitionState = ts;
}


std::string SceneManager::GetCurrentSceneKey() {
	return m_currentscenekey;
}

DirectX::XMFLOAT4 SceneManager::GetFadeColor() {
	return m_fadeColor;
}

void SceneManager::SetFadeColor(DirectX::XMFLOAT4 color) {
	m_fadeColor = color;
}
