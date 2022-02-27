#pragma once

template<class T>
class Singleton {
public:
	static inline T& GetInstance() {
		static T instance;
		return instance;
	}
protected:
	Singleton() {};
	//Singleton(class Game_Manager*);
	virtual ~Singleton() {};

private:
	void operator=(const Singleton& obj) {};
	Singleton(const Singleton &obj) {};
};
