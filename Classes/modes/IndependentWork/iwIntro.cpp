#include "iwIntro.hpp"

cIWIntro::cIWIntro(cocos2d::Size size, cBaseMode * thisMode)
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
	cocos2d::Sprite *logo_3 = cocos2d::Sprite::create("gameTypes/stats.png");
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

		cocos2d::Sprite *logoImage = cocos2d::Sprite::create("gameTypes/stats.png");
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

		cocos2d::Label* exam = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Самостоятельная работа", cocos2d::TextHAlignment::CENTER);
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

		/*std::string text = u8"Собрать:" + std::to_string((int)mode->goal) + "." + std::to_string(int((mode->goal - (int)mode->goal) * 100.0f)) + u8"оч. за: " + std::to_string(mode->turnCount) + u8"ход.";
		cocos2d::Label* goalText = cocos2d::Label::createWithTTF(*gTtfConfig, text, cocos2d::TextHAlignment::CENTER);
		goalText->setColor(cocos2d::Color3B(200, 200, 200));
		setNodeInRect(goalText, szGoalWnd, szGoalWnd.width * 0.05f);
		goalWnd->addChild(goalText);

		cocos2d::Size sizeTitle = { szGoalWnd.width / 2.5f, indent * 2.0f };
		cWindow* infoTitle = new cWindow(sizeTitle, W_MUTE, 10);
		infoTitle->setWColor(cocos2d::Color3B(200, 200, 200));
		infoTitle->setPosition(-szGoalWnd.width / 2.0f + indent + sizeTitle.width / 2.0f, szGoalWnd.height / 2.0f);
		goalWnd->addChild(infoTitle);

		cocos2d::Label* goalLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Цель", cocos2d::TextHAlignment::CENTER);
		goalLabel->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(goalLabel, sizeTitle, sizeTitle.width * 0.05f);
		infoTitle->addChild(goalLabel);*/
	}

	cWindow* subjectWnd = nullptr;
	cocos2d::Size szSubjectWnd = { szIBG.width - indent * 2.0f, freeSpace / 4.5f };
	{
		subjectWnd = new cWindow(szSubjectWnd, W_MUTE, 10);
		subjectWnd->setPosition(0, goalWnd->getPosition().y - szGoalWnd.height / 2.0f - indent * 2.0f - szSubjectWnd.height / 2.0f);
		subjectWnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(subjectWnd);

		/*cocos2d::Size sizeTitle = { szSubjectWnd.width / 2.5f, indent * 2.0f };
		cWindow* infoTitle = new cWindow(sizeTitle, W_MUTE, 10);
		infoTitle->setWColor(cocos2d::Color3B(200, 200, 200));
		infoTitle->setPosition(-szSubjectWnd.width / 2.0f + indent + sizeTitle.width / 2.0f, szSubjectWnd.height / 2.0f);
		subjectWnd->addChild(infoTitle);

		cocos2d::Label* subLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Предметы", cocos2d::TextHAlignment::CENTER);
		subLabel->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(subLabel, sizeTitle, sizeTitle.width * 0.05f);
		infoTitle->addChild(subLabel);


		for (unsigned int i = 0; i < 4; i++)
		{
		float locIndent = indent * 1.5f;
		float width = (szSubjectWnd.width - locIndent * 5.0f) / 4.0f;
		if (width > szSubjectWnd.height - indent * 2.0f)width = szSubjectWnd.height - indent * 2.0f;
		cocos2d::Size sizeWnd = { width, width };
		cWindow* wnd = new cWindow(sizeWnd, W_MUTE, 10);
		wnd->setWColor(cocos2d::Color3B(200, 200, 200));
		wnd->setPosition((-szSubjectWnd.width / 2.0f + locIndent + width / 2.0f) + (width + locIndent) * i, -locIndent / 3.0f);
		subjectWnd->addChild(wnd);

		int id = ((cMMTurns*)thisMode)->subjects.at(i); // 1-8
		cocos2d::Sprite *subjSprite = cocos2d::Sprite::create("gameScreen/" + std::to_string(id) + ".png");
		setNodeInRect(subjSprite, sizeWnd, sizeWnd.width * 0.1f);
		wnd->addChild(subjSprite);
		}*/
	}

	cWindow* teacherWnd = nullptr;
	cocos2d::Size szTeacherWnd = { szIBG.width - indent * 2.0f, freeSpace / 3.0f };
	{
		teacherWnd = new cWindow(szTeacherWnd, W_MUTE, 10);
		teacherWnd->setPosition(0, subjectWnd->getPosition().y - szSubjectWnd.height / 2.0f - indent * 2.0f - szTeacherWnd.height / 2.0f);
		teacherWnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(teacherWnd);

		/*cocos2d::Size sizeTitle = { szTeacherWnd.width / 2.5f, indent * 2.0f };
		cWindow* infoTitle = new cWindow(sizeTitle, W_MUTE, 10);
		infoTitle->setWColor(cocos2d::Color3B(200, 200, 200));
		infoTitle->setPosition(-szTeacherWnd.width / 2.0f + indent + sizeTitle.width / 2.0f, szTeacherWnd.height / 2.0f);
		teacherWnd->addChild(infoTitle);

		cocos2d::Label* teacherLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Преподаватель", cocos2d::TextHAlignment::CENTER);
		teacherLabel->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(teacherLabel, sizeTitle, sizeTitle.width * 0.05f);
		infoTitle->addChild(teacherLabel);

		cocos2d::Sprite *teacherSprite = cocos2d::Sprite::create(mode->teacherCard->filePath);
		cocos2d::Size spriteSize = { (szTeacherWnd.width - indent * 3.0f) / 2.0f, szTeacherWnd.height - indent * 2.0f };
		setNodeInRect(teacherSprite, spriteSize, 0);
		teacherSprite->setPosition(-szTeacherWnd.width / 2.0f + indent + spriteSize.width / 2.0f, -szTeacherWnd.height / 2.0f + spriteSize.height / 2.0f);
		teacherWnd->addChild(teacherSprite);



		cocos2d::Label* nameLabel = cocos2d::Label::createWithTTF(*gTtfConfig, mode->teacherCard->name, cocos2d::TextHAlignment::CENTER);
		nameLabel->setColor(cocos2d::Color3B(200, 200, 200));
		setNodeInRect(nameLabel, spriteSize, 0);
		cocos2d::Size nlSize = nameLabel->getContentSize() * nameLabel->getScale();
		nameLabel->setPosition(spriteSize.width / 2.0f, szTeacherWnd.height / 4.0f);
		teacherWnd->addChild(nameLabel);

		cocos2d::Size tSubject = { spriteSize.height / 2.0f, spriteSize.height / 2.0f };
		cocos2d::Sprite *teacherSubject = cocos2d::Sprite::create("gameScreen/" + std::to_string(mode->teacherCard->subject) + ".png");
		setNodeInRect(teacherSubject, tSubject, 0);
		teacherSubject->setPosition(spriteSize.width / 2.0f - tSubject.width / 2.0f, -szTeacherWnd.height / 4.5f);
		teacherWnd->addChild(teacherSubject);

		cocos2d::Label* skillLabel = cocos2d::Label::createWithTTF(*gTtfConfig, std::to_string(int(-(1.0f - mode->teacherCard->stat) * 100.0f)) + "%", cocos2d::TextHAlignment::CENTER);
		skillLabel->setColor(cocos2d::Color3B(247, 116, 98));
		setNodeInRect(skillLabel, tSubject, tSubject.width * 0.01f);
		cocos2d::Size slSize = skillLabel->getContentSize() * skillLabel->getScale();
		skillLabel->setPosition(teacherSubject->getPosition().x + tSubject.width + halfindent, teacherSubject->getPosition().y);
		teacherWnd->addChild(skillLabel);*/
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

cIWIntro::~cIWIntro()
{
}

void cIWIntro::step(float dt)
{
	cocos2d::Vec2 sz = this->getParent()->convertToWorldSpace(this->getPosition());
	layer->setPosition(-sz.x, -sz.y);
}

void cIWIntro::reset()
{
}
