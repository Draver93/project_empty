#ifndef INTRO_SCENE_HPP
#define INTRO_SCENE_HPP

#include "baseClasses/Utils.hpp"
#include "baseClasses/BaseScene.hpp"
#include "SceneManager.hpp"
#include "baseClasses/ui/UI.hpp"
#include "baseClasses/UserDefault.hpp"


class cIntroScene : public cBaseScene
{
private:
	std::vector<float> steps;
	int activeStep;
	float activeStepTime;
public:
	cIntroScene();
	~cIntroScene();
	cocos2d::Scene* bs_create();
	void update(float dt);
};

#endif // !INTRO_SCENE_HPP