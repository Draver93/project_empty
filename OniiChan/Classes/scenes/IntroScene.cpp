#include "IntroScene.hpp"

cIntroScene::cIntroScene()
{
	name = "info";
}

cIntroScene::~cIntroScene()
{
	
}

cocos2d::Scene* cIntroScene::bs_create()
{
	cocos2d::Scene* pScene = cocos2d::Scene::create();

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	auto center = cocos2d::Point(visibleSize.width / 2.0f + origin.x, visibleSize.height / 2.0f + origin.y);

    cUserDefault *userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;
    
	//fade->setColor(cocos2d::Color3B(0, 0, 0));

	cocos2d::Label *introText = cocos2d::Label::createWithTTF(*gTtfConfig, "Created by: \n Finoshkin Alexey", cocos2d::TextHAlignment::RIGHT);
	setNodeSize(introText, visibleSize.width / 4.0f, true, true);
	cocos2d::Size sizeText = introText->getContentSize() * introText->getScale();
	introText->setPosition(visibleSize.width - sizeText.width / 2.0f - (sizeText.width / 10.0f), sizeText.height / 2.0f + (sizeText.width / 10.0f));
	pScene->addChild(introText);



	auto logo = cocos2d::Sprite::create("introScreen/logo3.png");
	logo->setPosition(visibleSize.width / 4.0f, center.y);
	logo->setContentSize(cocos2d::Size(visibleSize.width / 4.5f, visibleSize.width / 4.5f));
	pScene->addChild(logo);

	cocos2d::Label *logoLabel = cocos2d::Label::createWithTTF(*gTtfConfig, "OniiChan", cocos2d::TextHAlignment::RIGHT);
	setNodeSize(logoLabel, visibleSize.width / 2.5f, true, true);
	cocos2d::Size sizeTextLabel = logoLabel->getContentSize() * logoLabel->getScale();
	logoLabel->setPosition(visibleSize.width / 1.7f, center.y);
	pScene->addChild(logoLabel);

	cocos2d::Label *logoSubLabel = cocos2d::Label::createWithTTF(*gTtfConfig, "in school", cocos2d::TextHAlignment::RIGHT);
	setNodeSize(logoSubLabel, visibleSize.width / 4.0f, true, true);
	cocos2d::Size sizeTextSubLabel = logoSubLabel->getContentSize() * logoLabel->getScale();
	logoSubLabel->setPosition(visibleSize.width / 1.7f + sizeTextLabel.width / 4.0f, center.y - sizeTextSubLabel.height / 2.0f - sizeTextLabel.height /2.0f);
	pScene->addChild(logoSubLabel);

	steps = {1};
	activeStep = 0;
	activeStepTime = 0;
	return pScene;
}


void cIntroScene::update(float dt)
{
	if (activeStep < (int)steps.size())
	{
		if (steps.at(activeStep) > activeStepTime)
		{
			activeStepTime += dt;
		}
		else
		{
			activeStepTime = 0;
			activeStep++;
		}
	}
	else if(getState() != SceneState::CLOSE)
	{
		cSceneManager *sceneManager = cSceneManager::getInstance();
		sceneManager->changeScene("menu");
	}
}
