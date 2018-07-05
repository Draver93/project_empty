#include "SceneManager.hpp"

cSceneManager* cSceneManager::pInstance = nullptr;

void cSceneManager::updateFade(float dt)
{
	/*float proc = (fadeTime - dt) / (activeScene->fadeTime / 100.0f);
	float dOpacity = 255 / 100.0f * proc;

	if (state == ManagerState::SHOW_SCENE)
	{
		cocos2d::Color3B dColor;
		dColor.r = fadePrev.r + (fadeActual.r - fadePrev.r) / 100.0f * (100.0f - proc);
		dColor.g = fadePrev.g + (fadeActual.g - fadePrev.g) / 100.0f * (100.0f - proc);
		dColor.b = fadePrev.b + (fadeActual.b - fadePrev.b) / 100.0f * (100.0f - proc);
		activeScene->fade->setColor(dColor);
		activeScene->fade->setOpacity(dOpacity);
	}
	else if (state == ManagerState::HIDE_SCENE)
	{
		activeScene->fade->setColor(fadeActual);
		activeScene->fade->setOpacity(255 - dOpacity);
	}

	fadeTime -= dt;*/
}

cSceneManager::cSceneManager()
{
	state = ManagerState::IDLE;

	this->init();
	this->scheduleUpdate();
	auto director = cocos2d::Director::getInstance();
	scenes.clear();
	activeScene = nullptr;
	newScene.clear();
	/*cIntroScene *intro = new cIntroScene();
	intro->bs_create();
	intro->fade->setOpacity(0);
	fadePrev = intro->fade->getColor();
	fadeActual = fadePrev;
	activeScene = intro;
	director->runWithScene((cocos2d::Scene*)intro);*/
}

cSceneManager * cSceneManager::getInstance()
{
	if (pInstance != nullptr)return pInstance;
	else
	{
		pInstance = new cSceneManager();
		return pInstance;
	}
	return nullptr;
}

cSceneManager::~cSceneManager()
{

}

void cSceneManager::update(float dt)
{
	/*auto director = cocos2d::Director::getInstance();
	if (activeScene == nullptr) return;
	if (activeScene->getState() == SceneState::CLOSE && state != ManagerState::HIDE_SCENE)
	{
		fadeTime = activeScene->fadeTime;
		state = ManagerState::HIDE_SCENE;
	}
	if (activeScene->getState() == SceneState::CLOSE && state == ManagerState::HIDE_SCENE)
	{
		updateFade(dt);
		fadeTime -= dt;
		if (fadeTime < 0)
		{
			fadeTime = 0;
			activeScene->fade->setOpacity(255);
			state = ManagerState::UPDATE_SCENE;
		}
	}
	if (activeScene->getState() != SceneState::CLOSE && state == ManagerState::SHOW_SCENE)
	{
		updateFade(dt);
		if (fadeTime < 0)
		{
			fadeTime = 0;
			activeScene->fade->setOpacity(0);
			state = ManagerState::IDLE;
		}
	}
	if (activeScene->getState() == SceneState::CLOSE && state == ManagerState::UPDATE_SCENE)
	{
		setScene(activeScene->getNextScene());
	}*/

	if (!newScene.empty())
	{
		auto director = cocos2d::Director::getInstance();
		fadePrev = fadeActual;

		for (auto scene : scenes)
		{
			if (scene->name == newScene)
			{
				if (activeScene != nullptr)
				{
					cocos2d::Scene* pScene = scene->bs_create();
					activeScene = scene;
					director->replaceScene(pScene);
				}
				else
				{
					cocos2d::Scene* pScene = scene->bs_create();
					activeScene = scene;
					director->runWithScene(pScene);
				}
				//fadeTime = activeScene->fadeTime;
				state = ManagerState::SHOW_SCENE;
			}
		}

		//fadeActual = activeScene->fade->getColor();
		//updateFade(0);
		newScene.clear();
	}
	else
	{
		activeScene->update(dt);
	}

}

void cSceneManager::addScene(cBaseScene * scene)
{
	scenes.push_back(scene);
}

void cSceneManager::changeScene(std::string name)
{
	newScene = name;
}

