#include "GameScene.hpp"

cGameScene::cGameScene()
{
	name = "game";
}
cGameScene::~cGameScene()
{
	layer = nullptr;
}

cocos2d::Scene*  cGameScene::bs_create()
{
	cocos2d::Scene* pScene = cocos2d::Scene::create();
	cUserDefault *userDefault = cUserDefault::getInstance();
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	auto center = cocos2d::Point(visibleSize.width / 2.0f + origin.x, visibleSize.height / 2.0f + origin.y);
	int dpi = cocos2d::Device::getDPI();
	cocos2d::Size btnMiddle = { dpi / 2.0f, dpi / 2.0f };
	float indent = visibleSize.height / 70.0f;

	//// background
	background = cocos2d::Sprite::create("gameScreen/bg_1.jpg");
	setNodeSize(background, visibleSize.height, true, false);
	background->setPosition(center);
	pScene->addChild(background);
	////
	float topBarH = visibleSize.height / 20.0f;

	//btn back
	{
		btnBack = new cButton(nullptr, cocos2d::Size(topBarH, topBarH), 1.1f, 10);
		cocos2d::Rect btnExitRect = cocos2d::utils::getCascadeBoundingBox(btnBack);
		btnBack->setPosition(btnExitRect.size.width / 2.0f + indent, visibleSize.height - btnExitRect.size.height / 2.0f - indent);
		cocos2d::Rect btnRect = cocos2d::utils::getCascadeBoundingBox(btnBack);
		btnBack->wnd->setWColor(cocos2d::Color3B(247, 116, 98));
		btnBack->setLocalZOrder(2);
		pScene->addChild(btnBack);
		btnBack->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			cSceneManager *sceneManager = cSceneManager::getInstance();
			sceneManager->changeScene("menu");
			return true;
		};

		cocos2d::Sprite *icon_back = cocos2d::Sprite::create("icons/back.png");
		setNodeSize(icon_back, btnBack->wnd->getWSize().width * 0.7f, true, true);
		btnBack->addChild(icon_back);
	}

	//btn settings
	{
		btnSettings = new cButton(nullptr, cocos2d::Size(topBarH, topBarH), 1.1f, 10);
		cocos2d::Rect btnExitRect = cocos2d::utils::getCascadeBoundingBox(btnSettings);
		btnSettings->setPosition(visibleSize.width - btnExitRect.size.width / 2.0f - indent, visibleSize.height - btnExitRect.size.height / 2.0f - indent);
		cocos2d::Rect btnRect = cocos2d::utils::getCascadeBoundingBox(btnSettings);
		btnSettings->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
		btnSettings->setLocalZOrder(2);
		pScene->addChild(btnSettings);

		cocos2d::Sprite *icon_exit = cocos2d::Sprite::create("icons/settings.png");
		setNodeSize(icon_exit, btnSettings->wnd->getWSize().width * 0.7f, true, true);
		btnSettings->addChild(icon_exit);
	}

	cocos2d::Size tbSize = cocos2d::Size(visibleSize.width - indent * 4.0f - topBarH * 2.0f, topBarH);
	//top bar
	{
		topBar = new cWindow(tbSize, W_MUTE, 0);
		cocos2d::Rect topBarRect = cocos2d::utils::getCascadeBoundingBox(topBar);
		topBar->setPosition(topBarRect.size.width / 2.0f + indent * 2.0f + topBarH, visibleSize.height - (topBarRect.size.height / 2.0f) - indent);
		topBar->setWColor(cocos2d::Color3B(0, 0, 0));
		topBar->setCascadeOpacityEnabled(false);
		auto ch = topBar->getChildren();
		for (auto &it : ch)it->setOpacity(100);
		topBar->setLocalZOrder(2);
		pScene->addChild(topBar);
	}

	cocos2d::Label *gameDesc = cocos2d::Label::createWithTTF(*userDefault->fontHd, userDefault->activeMode->getModeName(), cocos2d::TextHAlignment::CENTER);
	gameDesc->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	gameDesc->setColor(cocos2d::Color3B(230, 230, 230));
	setNodeInRect(gameDesc, tbSize * 0.8f, 0);
	topBar->setOpacity(100);
	topBar->addChild(gameDesc);

	cocos2d::Size gameFieldSize = { visibleSize.width - indent * 2.0f, visibleSize.height - indent * 3.0f - topBarH };

	layer = userDefault->activeMode->getNode(1 ,gameFieldSize);
	layer->setLocalZOrder(5);
	pScene->addChild(layer);
	return pScene;
}


void cGameScene::update(float dt)
{
	layer->step(dt);
}
