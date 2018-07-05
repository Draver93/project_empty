#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include "vector"
#include "string"
#include "map"

#include "baseClasses/BaseScene.hpp"

enum class ManagerState
{
	IDLE,
	HIDE_SCENE,
	SHOW_SCENE,
	UPDATE_SCENE,
};
class cSceneManager : public cocos2d::Node
{
private:
	static cSceneManager *pInstance;
	cSceneManager();

	ManagerState state;

	std::vector<cBaseScene*> scenes;
	cBaseScene *activeScene;
	float fadeTime;
	cocos2d::Color3B fadePrev;
	cocos2d::Color3B fadeActual;
	void updateFade(float dt);
	std::string newScene;
public:
	void changeScene(std::string name);
	static cSceneManager *getInstance();
	~cSceneManager();
	void update(float dt) override;
	void addScene(cBaseScene *scene);
	//void setScene(std::string name);
	cBaseScene *getActiveScene() { return activeScene; }
};


#endif // !SCENE_MANAGER_HPP
