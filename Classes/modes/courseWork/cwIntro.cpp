#include "cwIntro.hpp"

cCWIntro::cCWIntro(cocos2d::Size size, cBaseMode * thisMode)
{
	listenNode = new cBaseNode();
	cTestWork* mode = (cTestWork*)thisMode;

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	cUserDefault *userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;

	float indent = visibleSize.height / 70.0f;
	float halfindent = indent / 2.0f;

	introBtn = new cButton(nullptr, size, 1.01f, -2);
	this->addChild(introBtn);
	auto ch = introBtn->getChildren();
	for (auto it : ch) it->setOpacity(200);

	introBtn->setOpacity(200);

	cocos2d::Size szLogo = { size.width - indent * 2.0f, size.height - indent * 2.0f };

	cWindow* logo = new cWindow(szLogo, W_MUTE, 10);
	logo->setWColor(cocos2d::Color3B(96, 139, 171));
	introBtn->addChild(logo);
	cocos2d::Sprite *logo_3 = cocos2d::Sprite::create("gameTypes/circ.png");
	setNodeInRect(logo_3, szLogo, szLogo.width * 0.2f);
	logo->addChild(logo_3);

	layer = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 200));
	this->addChild(layer);
	layer->setVisible(false);


	//width / 6
	cWindow* infoBg = nullptr;
	cocos2d::Size szIBG = { (visibleSize.height * 0.7f) * 0.7f,  visibleSize.height * 0.7f };
	{
		infoBg = new cWindow(szIBG, W_MUTE, 10);
		infoBg->setPosition(visibleSize / 2.0f);
		infoBg->setWColor(cocos2d::Color3B(255, 255, 255));
		layer->addChild(infoBg);

		infoBg->setCascadeOpacityEnabled(false);
		auto ch = infoBg->getChildren();
		for (auto it : ch) it->setOpacity(150);
	}

	cWindow* logoSmall = nullptr;
	cocos2d::Size szLogoSmall = { visibleSize.width / 9.0f, visibleSize.width / 9.0f };
	{
		logoSmall = new cWindow(szLogoSmall, W_MUTE, 10);
		logoSmall->setPosition(-szIBG.width / 2.0f + indent + szLogoSmall.width / 2.0f, szIBG.height / 2.0f - indent - szLogoSmall.height / 2.0f);
		logoSmall->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(logoSmall);

		cocos2d::Sprite *logoImage = cocos2d::Sprite::create("gameTypes/circ.png");
		setNodeInRect(logoImage, szLogoSmall, szLogoSmall.width * 0.2f);
		logoImage->setOpacity(150);
		logoSmall->addChild(logoImage);
	}

	cWindow* logoTitle = nullptr;
	cocos2d::Size szLogoTitle = { szIBG.width - szLogoSmall.width - indent * 3.0f, szLogoSmall.height };
	{
		logoTitle = new cWindow(szLogoTitle, W_MUTE, 10);
		logoTitle->setPosition(szIBG.width / 2.0f - indent - szLogoTitle.width / 2.0f, szIBG.height / 2.0f - indent - szLogoTitle.height / 2.0f);
		logoTitle->setWColor(cocos2d::Color3B(200, 200, 200));
		infoBg->addChild(logoTitle);

		cocos2d::Label* exam = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Курсовая работа", cocos2d::TextHAlignment::CENTER);
		exam->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(exam, szLogoTitle, szLogoTitle.width * 0.05f);
		logoTitle->addChild(exam);
	}

	float freeSpace = szIBG.height - szLogoSmall.height - indent * 10.0f;
	cWindow* goalWnd = nullptr;
	cocos2d::Size szGoalWnd = { szIBG.width - indent * 2.0f, freeSpace / 6.0f };
	{
		goalWnd = new cWindow(szGoalWnd, W_MUTE, 10);
		goalWnd->setPosition(0, logoTitle->getPosition().y - szLogoTitle.height / 2.0f - indent * 3.0f - szGoalWnd.height / 2.0f);
		goalWnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(goalWnd);
	}

	cWindow* subjectWnd = nullptr;
	cocos2d::Size szSubjectWnd = { szIBG.width - indent * 2.0f, freeSpace / 4.5f };
	{
		subjectWnd = new cWindow(szSubjectWnd, W_MUTE, 10);
		subjectWnd->setPosition(0, goalWnd->getPosition().y - szGoalWnd.height / 2.0f - indent * 2.0f - szSubjectWnd.height / 2.0f);
		subjectWnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(subjectWnd);
	}

	cWindow* teacherWnd = nullptr;
	cocos2d::Size szTeacherWnd = { szIBG.width - indent * 2.0f, freeSpace / 3.0f };
	{
		teacherWnd = new cWindow(szTeacherWnd, W_MUTE, 10);
		teacherWnd->setPosition(0, subjectWnd->getPosition().y - szSubjectWnd.height / 2.0f - indent * 2.0f - szTeacherWnd.height / 2.0f);
		teacherWnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(teacherWnd);
	}

	cButton *btnOk = nullptr, *btnCancel = nullptr;
	cocos2d::Size szBtn = { (szIBG.width - indent * 4.0f) / 2.5f, freeSpace / 9.0f };
	{
		btnOk = new cButton(nullptr, szBtn, 1.03f, -11);
		btnOk->setPosition(szIBG.width / 2.0 - indent - szBtn.width / 2.0f, -szIBG.height / 2.0 + indent + szBtn.height / 2.0f);
		btnOk->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(btnOk);

		cocos2d::Label* OkLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Начать", cocos2d::TextHAlignment::CENTER);
		OkLabel->setColor(cocos2d::Color3B(200, 200, 200));
		setNodeInRect(OkLabel, szBtn, szBtn.height * 0.6f);
		btnOk->addChild(OkLabel);

		btnOk->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			cSceneManager *sceneManager = cSceneManager::getInstance();
			sceneManager->changeScene("game");
			return true;
		};

		btnCancel = new cButton(nullptr, szBtn, 1.03f, -11);
		btnCancel->setPosition(btnOk->getPosition().x - indent - szBtn.width, btnOk->getPosition().y);
		btnCancel->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(btnCancel);

		cocos2d::Label* BackLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Назад", cocos2d::TextHAlignment::CENTER);
		BackLabel->setColor(cocos2d::Color3B(200, 200, 200));
		setNodeInRect(BackLabel, szBtn, szBtn.height * 0.6f);
		btnCancel->addChild(BackLabel);

		btnCancel->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{

			userDefault->activeMode = nullptr;
			this->setLocalZOrder(1);
			layer->setVisible(false);
			return true;
		};

	}

	introBtn->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
	{
		cUserDefault *userDefault = cUserDefault::getInstance();

		if (userDefault->activeMode == nullptr)
		{
			userDefault->activeMode = thisMode;
			this->setLocalZOrder(2);
			layer->setVisible(true);
		}
		else if (userDefault->activeMode != thisMode)
		{
			userDefault->activeMode = thisMode;
			this->setLocalZOrder(2);
			layer->setVisible(true);
		}
		else if (userDefault->activeMode == thisMode)
		{
			userDefault->activeMode = nullptr;
			this->setLocalZOrder(1);
			layer->setVisible(false);
		}
		return true;
	};

	layer->addChild(listenNode);

	listenNode->listener = cocos2d::EventListenerTouchOneByOne::create();
	listenNode->listener->setSwallowTouches(true);
	listenNode->listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		return true;
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listenNode->listener, -10);
}

cCWIntro::~cCWIntro()
{
}

void cCWIntro::step(float dt)
{
	cocos2d::Vec2 sz = this->getParent()->convertToWorldSpace(this->getPosition());
	layer->setPosition(-sz.x, -sz.y);
}

void cCWIntro::reset()
{
}
