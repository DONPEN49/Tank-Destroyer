#pragma once
#include	<string>
#include	<unordered_map>
#include	<functional>
#include	<iostream>
#include	<memory>
#include	<directxmath.h>
#include	<chrono>
#include	<thread>
#include    <vector>
#include	"SceneManager.h"
#include	"uncopyable.h"
#include	"StopWatch.h"
#include	"quad2d.h"
#include	"Application.h"
#include    "Object.h"
#include    "Base_UI.h"

class SceneManager;

class IScene : Uncopyable {
private:
	SceneManager*	manager;
protected:
	std::vector<Object*>	listObject;
	std::vector<UI*>		listUI;
	std::string tag;
public:

	const std::string GetSceneTag() const { return tag; };

	IScene();

	void SetSceneManager(SceneManager* sm) {
		manager = sm;
	}

	virtual ~IScene() {}
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Dispose() = 0;

	virtual void updateFadeIn(double t);
	virtual void updateFadeOut(double t);

	virtual void drawFadeIn(double t);
	virtual void drawFadeOut(double t);

	void AddSceneObject(Object* _obj) {
		listObject.emplace_back(_obj);
	}

	void AddSceneUI(UI* _ui) {
		listUI.emplace_back(_ui);
	}

	template<typename T>
	T* GetSceneObject(std::string _tag) {
		for (Object* obj : listObject) {
			if (obj->tag == _tag) {
				return (T*)obj;
			}
		}

		return nullptr;
	}

	template<typename T>
	std::vector<T*> GetSceneObjects(std::string _tag) {
		std::vector<T*> temp;
		for (Object* obj : listObject) {
			if (obj->tag == _tag) {
				temp.emplace_back((T*)obj);
			}
		}
		return temp;
	}

	template<typename T>
	T* GetSceneUI(std::string _tag) {
		for (UI* ui : listUI) {
			if (ui->tag == _tag) {
				return (T*)ui;
			}
		}

		return nullptr;
	}
};