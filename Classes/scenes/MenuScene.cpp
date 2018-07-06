#include "MenuScene.hpp"

cMenuScene::cMenuScene()
{
	name = "menu";
}
cMenuScene::~cMenuScene()
{
}

cocos2d::Scene* cMenuScene::bs_create()
{
	int f = cocos2d::Director::getInstance()->getEventDispatcher()->getReferenceCount();
	prof = nullptr;
	cocos2d::Scene *pScene = cocos2d::Scene::create();

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	auto center = cocos2d::Point(visibleSize.width / 2.0f + origin.x, visibleSize.height / 2.0f + origin.y);
	auto glview = cocos2d::Director::getInstance()->getOpenGLView();

	int dpi = cocos2d::Device::getDPI();
	cUserDefault *userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;

	// background
	{
		background = cocos2d::Sprite::create("mainScreen/bg_2.jpg");
		setNodeSize(background, visibleSize.height, true, false);
		background->setPosition((background->getContentSize().width * background->getScale()) / 2.0f, center.y);
		pScene->addChild(background);
	}
	
	int s = 5;
	cocos2d::Size btnMiddle = { dpi / 2.0f, dpi / 2.0f };
	float indent = visibleSize.height / 70.0f;
	float btmBarH = (visibleSize.width - indent * 2.0f) / s;
	float topBarH = visibleSize.height / 20.0f;
	float mdlBarH = visibleSize.height - (indent * 4.0f + btmBarH + topBarH);

	playDialog = new cPlayDialog();
	playDialog->setVisible(false);
	pScene->addChild(playDialog);
	//btn exit
	{
		cButton *btnExit = new cButton(nullptr, cocos2d::Size(topBarH, topBarH), 1.1f, 10);
		cocos2d::Rect btnExitRect = cocos2d::utils::getCascadeBoundingBox(btnExit);
		btnExit->setPosition(btnExitRect.size.width / 2.0f + indent, visibleSize.height - btnExitRect.size.height / 2.0f - indent);
		cocos2d::Rect btnRect = cocos2d::utils::getCascadeBoundingBox(btnExit);
		btnExit->wnd->setWColor(cocos2d::Color3B(247, 116, 98));
		btnExit->setLocalZOrder(3);
		pScene->addChild(btnExit);
		btnExit->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			cocos2d::Director::getInstance()->end();
			//exit(0);
			return true;
		};

		cocos2d::Sprite *icon_exit = cocos2d::Sprite::create("icons/exit.png");
		setNodeSize(icon_exit, btnExit->wnd->getWSize().width * 0.7f, true, true);
		icon_exit->setLocalZOrder(4);
		btnExit->addChild(icon_exit);
	}
	
	//btn settings
	{
		cButton *btnExit = new cButton(nullptr, cocos2d::Size(topBarH, topBarH), 1.1f, 10);
		cocos2d::Rect btnExitRect = cocos2d::utils::getCascadeBoundingBox(btnExit);
		btnExit->setPosition(visibleSize.width - btnExitRect.size.width / 2.0f - indent, visibleSize.height - btnExitRect.size.height / 2.0f - indent);
		cocos2d::Rect btnRect = cocos2d::utils::getCascadeBoundingBox(btnExit);
		btnExit->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
		btnExit->setLocalZOrder(3);
		pScene->addChild(btnExit);

		cocos2d::Sprite *icon_exit = cocos2d::Sprite::create("icons/settings.png");
		setNodeSize(icon_exit, btnExit->wnd->getWSize().width * 0.7f, true, true);
		icon_exit->setLocalZOrder(4);
		btnExit->addChild(icon_exit);
	}

	//top bar
	{
		cWindow *topBar = new cWindow(cocos2d::Size(visibleSize.width - indent * 4.0f - topBarH * 2.0f, topBarH), W_MUTE, 0);
		cocos2d::Rect topBarRect = cocos2d::utils::getCascadeBoundingBox(topBar);
		topBar->setPosition(topBarRect.size.width / 2.0f + indent * 2.0f + topBarH, visibleSize.height - (topBarRect.size.height / 2.0f) - indent);
		topBar->setWColor(cocos2d::Color3B(0, 0, 0));
		topBar->setOpacity(100);
		topBar->setLocalZOrder(2);
		pScene->addChild(topBar);

        cUserDefault *userDefault = cUserDefault::getInstance();
	    auto gTtfConfig = userDefault->font;
        
		wndName = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Календарь", cocos2d::TextHAlignment::CENTER);
		setNodeSize(wndName, topBar->getWSize().height * 0.5f, true, false);
		wndName->setPosition(topBar->getPosition());
		wndName->setLocalZOrder(3);

		pScene->addChild(wndName);
	}
	
	
	//middle bar
	{
		bgWnd = new cWindow(cocos2d::Size(visibleSize.width - indent * 2.0f, mdlBarH), W_MUTE, 0);
		bgWnd->setWColor(cocos2d::Color3B(0, 0, 0));
		bgWnd->setOpacity(100);
		bgWnd->setPosition(center.x, indent * 2.0f + btmBarH + mdlBarH / 2.0f);
		bgWnd->setLocalZOrder(1);
		mdlWndRect = cocos2d::utils::getCascadeBoundingBox(bgWnd);
		pScene->addChild(bgWnd);

		if (userDefault->first_start == true)
		{		
			prof = new cCreateProfileDialog();
			pScene->addChild(prof);
		}
		else
		{
			cProfile *pr = new cProfile(mdlWndRect.size);
			pr->setLocalZOrder(55);
			bgWnd->addChild(pr);
			subMenu["profile"] = pr;
		}
		cCalendar *cal = new cCalendar(mdlWndRect.size);
		cal->setLocalZOrder(55);
		bgWnd->addChild(cal);
		cal->setVisible(false);
		subMenu["calendar"] = cal;

		//cStudentCard* card = new cStudentCard("wow", userDefault->generateCard((rand() % 3)));
		//userDefault->cards.push_back(card);
		//userDefault->saveCards();
		cCardInventory *cardInv = new cCardInventory(mdlWndRect.size);
		cardInv->setLocalZOrder(55);
		bgWnd->addChild(cardInv);
		cardInv->setVisible(false);
		subMenu["cards"] = cardInv;

		wndName->setString(u8"Профиль");
	}
	
	//btm bar
	cocos2d::Rect btmBarRect;
	{
		cWindow *btmBar = new cWindow(cocos2d::Size(visibleSize.width - indent * 2.0f, btmBarH), W_MUTE, 0);
		btmBarRect = cocos2d::utils::getCascadeBoundingBox(btmBar);
		btmBar->setPosition(btmBarRect.size.width / 2.0f + indent, btmBarRect.size.height / 2.0f + indent);
		btmBar->setWColor(cocos2d::Color3B(0, 0, 0));
		btmBar->setOpacity(100);
		btmBar->setLocalZOrder(2);
		pScene->addChild(btmBar);
	}

	//buttons on btm bar
	{
		for (int i = -(s / 2); i < s / 2 + 1; i++)
		{
			cButton *btn = new cButton(nullptr, cocos2d::Size(btmBarRect.size.height - indent * 2.0f, btmBarRect.size.height - indent * 2.0f), 1.1f, 10);
			cocos2d::Rect btnRect = cocos2d::utils::getCascadeBoundingBox(btn);
			btn->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
			btn->setPosition(visibleSize.width / 2.0f - ((btmBarRect.size.width) / s) * i, btnRect.size.height / 2.0f + indent * 2.0f);
			btn->setLocalZOrder(3);
			btn->setTag(i + s / 2);
			pScene->addChild(btn);
		}

		((cButton*)pScene->getChildByTag(0))->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			playDialog->show(3);
			return true;
		};
		((cButton*)pScene->getChildByTag(1))->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{

			return true;
		};
		((cButton*)pScene->getChildByTag(2))->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			for (auto it : subMenu) it.second->hide();
			subMenu.at("cards")->show();
			wndName->setString(u8"Инвентарь");

			return true;
		};
		((cButton*)pScene->getChildByTag(3))->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			for (auto it : subMenu) it.second->hide();
			subMenu.at("calendar")->show();
			wndName->setString(u8"Календарь");

			return true;
		};
		((cButton*)pScene->getChildByTag(4))->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			for (auto it : subMenu)  it.second->hide();
			if (subMenu.at("profile") != nullptr)
			{
				subMenu.at("profile")->show();
				wndName->setString(u8"Профиль");
			}

			return true;
		};

		cocos2d::Sprite *icon_0 = cocos2d::Sprite::create("icons/play.png");
		setNodeSize(icon_0, ((cButton*)pScene->getChildByTag(0))->wnd->getWSize().width * 0.7f, true, true);
		icon_0->setLocalZOrder(4);
		((cButton*)pScene->getChildByTag(0))->addChild(icon_0);

		cocos2d::Sprite *icon_1 = cocos2d::Sprite::create("icons/ad.png");
		setNodeSize(icon_1, ((cButton*)pScene->getChildByTag(1))->wnd->getWSize().width * 0.85f, true, true);
		icon_1->setLocalZOrder(4);
		((cButton*)pScene->getChildByTag(1))->addChild(icon_1);
		((cButton*)pScene->getChildByTag(1))->wnd->setWColor(cocos2d::Color3B(150, 150, 150));
		((cButton*)pScene->getChildByTag(1))->listener->setEnabled(false);
		cocos2d::Sprite *icon_2 = cocos2d::Sprite::create("icons/cards.png");
		setNodeSize(icon_2, ((cButton*)pScene->getChildByTag(2))->wnd->getWSize().width * 0.7f, true, true);
		icon_2->setLocalZOrder(4);
		((cButton*)pScene->getChildByTag(2))->addChild(icon_2);

		cocos2d::Sprite *icon_3 = cocos2d::Sprite::create("icons/calend.png");
		setNodeSize(icon_3, ((cButton*)pScene->getChildByTag(3))->wnd->getWSize().width * 0.7f, true, true);
		icon_3->setLocalZOrder(4);
		((cButton*)pScene->getChildByTag(3))->addChild(icon_3);

		cocos2d::Sprite *icon_4 = cocos2d::Sprite::create("icons/stat.png");
		setNodeSize(icon_4, ((cButton*)pScene->getChildByTag(4))->wnd->getWSize().width * 0.9f, true, true);
		icon_4->setLocalZOrder(4);
		((cButton*)pScene->getChildByTag(4))->addChild(icon_4);
	}
	
	return pScene;
}



void cMenuScene::update(float dt)
{
	if(prof != nullptr)
		if (subMenu["profile"] == nullptr && prof->complete == true)
		{
			cProfile *pr = new cProfile(mdlWndRect.size);
			pr->setLocalZOrder(55);
			bgWnd->addChild(pr);
			subMenu["profile"] = pr;
			prof->removeFromParent();
		}
	//cUserDefault *userDef = cUserDefault::getInstance();
	//userDef->updateCurrentModes();
	//userDef->activeMode = userDef->modes.at(0);

	//cSceneManager *sceneManager = cSceneManager::getInstance();
	//sceneManager->changeScene("game");

	//subMenu["cards"]->update(dt);
}
