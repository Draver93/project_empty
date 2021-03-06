﻿#include "VerificationWork.hpp"

cMMTurnsGameNode::cMMTurnsGameNode(cocos2d::Size size, std::vector<float> inPlayerSkills, std::vector<int> inGameLevel, std::vector<int> inSubjects, cTeacherCard* teacherCard, float inGoal, int inTurnCount, int days)
{
	dayCost = days;
	playerSkills = inPlayerSkills;
	subjects = inSubjects;
	goal = inGoal;
	turnCount = inTurnCount;
	tCard = teacherCard;
	turns = 0;
	currentScore = 0;
	addtime = 0;
	elementCount = 25;

	auto userDefault = cUserDefault::getInstance();
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	auto gTtfConfig = userDefault->font;
	auto gTtfConfigHd = userDefault->fontHd;

	float indent = visibleSize.height / 70.0f;
	float halfindent = indent / 2.0f;

	//float btmBarH = 0;
	cocos2d::Size btmBarSize;

	if (size.width > size.height * 0.6f) btmBarSize = cocos2d::Size(size.height * 0.6f, size.height * 0.6f);
	else btmBarSize = cocos2d::Size(visibleSize.width - indent * 2.0f, size.width); 

	cWindow *btmBar = new cWindow(btmBarSize, W_MUTE, 0);
	cocos2d::Rect btmBarRect = cocos2d::utils::getCascadeBoundingBox(btmBar);
	float barInd = (visibleSize.width - btmBarRect.size.width) / 2.0f;
	btmBar->setPosition(btmBarRect.size.width / 2.0f + barInd, btmBarRect.size.height / 2.0f + indent);
	btmBar->setWColor(cocos2d::Color3B(0, 0, 0));
	btmBar->setOpacity(100);
	btmBar->setLocalZOrder(2);
	this->addChild(btmBar);

	field = new cGameField(visibleSize.width, cocos2d::Vec2(8, 7));
	cocos2d::Size fSize = field->getContentSize() * field->getScale();
	btmBar->addChild((cocos2d::Node*)field);
	float a = (visibleSize.width - indent * 4.0f);
	float b = btmBarSize.width - indent * 2.0f;
	field->setScale((btmBarSize.width - indent * 2.0f) / visibleSize.width);
	field->setPosition(-btmBarSize.width / 2.0f + indent, -btmBarSize.height / 2.0f + indent);
	field->setLocalZOrder(3);
	//countElements = 25;

	cocos2d::Size ifSize = { btmBar->getWSize().width, size.height - btmBarSize.height -indent };
	cWindow* infoField = new cWindow(ifSize, W_MUTE, 0);
	infoField->setPosition(btmBar->getPosition().x, btmBar->getPosition().y + btmBar->getWSize().height / 2.0f + indent + ifSize.height / 2.0f);
	infoField->setWColor(cocos2d::Color3B(0, 0, 0));
	infoField->setCascadeOpacityEnabled(false);
	auto ch = infoField->getChildren();
	for (auto it : ch) it->setOpacity(0);
	this->addChild(infoField);


	cocos2d::Size sbSize = { ifSize.height * 0.7f, ifSize.height * 0.7f};
	cocos2d::Sprite *scoreBar = cocos2d::Sprite::create("icons/element.png");
	scoreBar->setColor(cocos2d::Color3B(0, 0, 0));
	scoreBar->setOpacity(100);
	scoreBar->setContentSize(sbSize);

	feScore.node = new cBaseNode();
	infoField->addChild(feScore.node);
	feScore.node->addChild(scoreBar);
	feScore.originPos = { 0, 0 };



	tbSize = cocos2d::Size{ sbSize * 0.4f };
	cocos2d::Sprite *turnBar = cocos2d::Sprite::create("icons/element.png");
	turnBar->setColor(cocos2d::Color3B(0, 0, 0));
	turnBar->setOpacity(100);
	turnBar->setContentSize(tbSize);

	feTurn.node = new cBaseNode();
	infoField->addChild(feTurn.node);
	feTurn.node->addChild(turnBar);
	feTurn.originPos = { -sbSize.width /2.0f - tbSize.width /2.0f, sbSize.height / 2.0f - tbSize.height / 2.0f };

	{
		bgScoreSize = cocos2d::Size{ sbSize.width * 1.5f, sbSize.height * 0.4f };
		scoreLabel = cocos2d::Label::createWithTTF(*gTtfConfigHd, "000", cocos2d::TextHAlignment::CENTER);
		scoreLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.58f));
		setNodeInRect(scoreLabel, bgScoreSize * 0.8f, 0);
		scoreLabel->setColor(cocos2d::Color3B(230, 230, 230));
		scoreLabel->setPosition(sbSize / 2.0f);
		scoreBar->addChild(scoreLabel);

		turnLabel = cocos2d::Label::createWithTTF(*gTtfConfigHd, "25", cocos2d::TextHAlignment::CENTER);
		turnLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.58f));
		setNodeInRect(turnLabel, tbSize * 0.6f, 0);
		turnLabel->setColor(cocos2d::Color3B(230, 230, 230));
		turnLabel->setPosition(tbSize / 2.0f);
		turnBar->addChild(turnLabel);
	}

	{
		cocos2d::Label *scoreDesc = cocos2d::Label::createWithTTF(*gTtfConfigHd, u8"Игровой счет", cocos2d::TextHAlignment::CENTER);
		scoreDesc->setAnchorPoint(cocos2d::Vec2(0.5f, 1.0f));
		scoreDesc->setColor(cocos2d::Color3B(230, 230, 230));
		setNodeInRect(scoreDesc, cocos2d::Size(bgScoreSize.width * 2.2f, bgScoreSize.height * 0.3f), 0);
		scoreDesc->setPosition(sbSize.width / 2.0f, 0);
		scoreBar->addChild(scoreDesc);

		cocos2d::Label *turnDesc = cocos2d::Label::createWithTTF(*gTtfConfigHd, u8"Ходы", cocos2d::TextHAlignment::CENTER);
		turnDesc->setAnchorPoint(cocos2d::Vec2(0.5f, 1.0f));
		turnDesc->setColor(cocos2d::Color3B(230, 230, 230));
		setNodeInRect(turnDesc, cocos2d::Size(tbSize.width * 2.2f, tbSize.height * 0.3f), 0);
		turnDesc->setPosition(tbSize.width / 2.0f, 0);
		turnBar->addChild(turnDesc);
	}
	updateInfoBar();
}
cMMTurnsGameNode::~cMMTurnsGameNode()
{
}
void cMMTurnsGameNode::step(float dt)
{
	updateGameStats(dt);
	if (addtime > 0.2 && (int)field->getElements().size() < elementCount)
	{
		int randNum = rand() % 4;
		field->addElement(subjects.at(randNum));
		addtime = 0;
	}
	addtime += dt;

	/*if (!tableArray.empty() && tableArray.back()->getPosition().y < 0)
	{
		cocos2d::Size rtSize = cardField->getWSize();

		for (auto it : tableArray)
		{
			float porc = abs(it->getPosition().y) / (rtSize.height / 3.0f) * 100.0f;
			if (porc > 100.0f) porc = 100.0f;
			it->setOpacity(255.0f - (255.0f / 100.0f * porc));
			it->setPosition(0, it->getPosition().y + (rtSize.height / 2.0f * dt));
			//it->setScale(abs(1.0f - (0.5f / 100.0f * porc)));
		}
	}
	infoTable->setVisible(tableArray.empty());*/

	cUserDefault *userDefault = cUserDefault::getInstance();
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	//АПАСНА
	((cMMTurns*)userDefault->activeMode)->turns = turns;
	((cMMTurns*)userDefault->activeMode)->currentScore = currentScore.get();

	feTurn.update(dt, tbSize.width / 10.0f);
	feScore.update(dt, bgScoreSize.width / 20.0f);

}
void cMMTurnsGameNode::reset()
{
}

void cMMTurnsGameNode::updateInfoBar()
{
	scoreLabel->setString(std::to_string(currentScore.get()).substr(0, std::to_string(currentScore.get()).find('.') + 2));
	setNodeInRect(scoreLabel, bgScoreSize * 0.8f, 0);
	turnLabel->setString(std::to_string(int(turnCount - turns > 0 ? turnCount - turns : 0)));
	setNodeInRect(turnLabel, tbSize * 0.6f, 0);
}
void cMMTurnsGameNode::updateGameStats(float dt)
{
	currentScore.update(dt);
	if (currentScore.isProgress())
	{
		updateInfoBar();
		checkGameFinish();
	}
	//Расчет результата
	if (field->getTurnCount() != turns)
	{
		auto elements = field->getLastLine();
		float result = 0;
		for (auto it : elements)
		{
			result += playerSkills.at(it - 1);
			for (auto it2 : sCards)
			{
				if (it2->subject == it - 1)
				{
					for(auto it3 : sCardNodes)	
						if(it3->getName() == it2->id)
							it3->setOpacity(255);

					//result += it2->stat;
				}
			}
		}
		///Расчеты

		if (incr.size() >= elements.size())result *= incr.at(elements.size() - 1);
		else result *= incr.back();

		//addInTable( std::to_string(result).substr(0, std::to_string(result).find('.') + 2));

		if(!currentScore.isProgress())
			currentScore.set(currentScore.get() + result, 3, true);
		else
		{
			float point = currentScore.get_nVal();
			currentScore.set(currentScore.get());
			currentScore.set(point + result, 3, true);
		}
		turns = field->getTurnCount();

	}
	//Подсветка карт
	for (auto it : sCards)
	{
		for (auto it2 : sCardNodes)	
			if (it2->getName() == it->id)
			{
				float op = it2->getOpacity();
				if (op > 150) it2->setOpacity(op - 1.0 * dt);
				else it2->setOpacity(150);
			}
	}
}
void cMMTurnsGameNode::checkGameFinish()
{
	if (turns >= turnCount || currentScore.get() >= goal)
	{
		currentScore.set(currentScore.get_nVal());
		cUserDefault *userDefault = cUserDefault::getInstance();
		((cMMTurns*)userDefault->activeMode)->currentScore = currentScore.get();
		((cMMTurns*)userDefault->activeMode)->turns = turns;
		cEndDialog *endDialog = new cEndDialog();
		this->addChild(endDialog);
	
	}
}



/////
cMMTurnsIntroNode::cMMTurnsIntroNode(cocos2d::Size size, cBaseMode *thisMode)
{
	listenNode = new cBaseNode();
	cMMTurns* mode = (cMMTurns*)thisMode;

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    cUserDefault *userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;

	float indent = visibleSize.height / 70.0f;
	float halfindent = indent / 2.0f;

	examGame = new cButton(nullptr, size, 1.01f, -2);
	this->addChild(examGame);
	auto ch = examGame->getChildren();
	for (auto it : ch) it->setOpacity(200);

	examGame->setOpacity(200);

	cocos2d::Size szLogo = { size.width - indent * 2.0f, size.height - indent * 2.0f };

	cWindow* logo = new cWindow(szLogo, W_MUTE, 10);
	logo->setWColor(cocos2d::Color3B(96, 139, 171));
	examGame->addChild(logo);
	cocos2d::Sprite *logo_3 = cocos2d::Sprite::create("gameTypes/exam.png");
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

		cocos2d::Sprite *logoImage = cocos2d::Sprite::create("gameTypes/exam.png");
		setNodeInRect(logoImage, szLogoSmall, szLogoSmall.width * 0.2f);
		logoImage->setOpacity(150);
		logoSmall->addChild(logoImage);
	}

	cWindow* logoTitle = nullptr;
	cocos2d::Size szLogoTitle = { szIBG.width - szLogoSmall.width - indent * 3.0f, szLogoSmall.height};
	{
		logoTitle = new cWindow(szLogoTitle, W_MUTE, 10);
		logoTitle->setPosition(szIBG.width / 2.0f - indent - szLogoTitle.width / 2.0f, szIBG.height / 2.0f - indent - szLogoTitle.height / 2.0f);
		logoTitle->setWColor(cocos2d::Color3B(200, 200, 200));
		infoBg->addChild(logoTitle);

		cocos2d::Label* exam = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Экзамен", cocos2d::TextHAlignment::CENTER);
		exam->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(exam, szLogoTitle, szLogoTitle.width * 0.05f);
		logoTitle->addChild(exam);
	}

	float freeSpace = szIBG.height - szLogoSmall.height - indent * 10.0f;
	cWindow* goalWnd = nullptr;
	cocos2d::Size szGoalWnd = { szIBG.width - indent * 2.0f, freeSpace / 6.0f};
	{
		goalWnd = new cWindow(szGoalWnd, W_MUTE, 10);
		goalWnd->setPosition(0, logoTitle->getPosition().y - szLogoTitle.height / 2.0f - indent * 3.0f - szGoalWnd.height / 2.0f);
		goalWnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(goalWnd);
		
		std::string text = u8"Собрать:" + std::to_string((int)mode->goal) + "." + std::to_string(int((mode->goal - (int)mode->goal) * 100.0f)) + u8"оч. за: " + std::to_string(mode->turnCount) + u8"ход.";
		cocos2d::Label* goalText = cocos2d::Label::createWithTTF(*gTtfConfig, text, cocos2d::TextHAlignment::CENTER);
		goalText->setColor(cocos2d::Color3B(200, 200, 200));
		setNodeInRect(goalText, szGoalWnd, szGoalWnd.width * 0.05f);
		goalWnd->addChild(goalText);
		
		cocos2d::Size sizeTitle = { szGoalWnd.width / 2.5f, indent * 2.0f };
		cWindow* infoTitle = new cWindow(sizeTitle, W_MUTE, 10);
		infoTitle->setWColor(cocos2d::Color3B(200, 200, 200));
		infoTitle->setPosition(-szGoalWnd.width / 2.0f + indent + sizeTitle.width /2.0f, szGoalWnd.height / 2.0f);
		goalWnd->addChild(infoTitle);

		cocos2d::Label* goalLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Цель", cocos2d::TextHAlignment::CENTER);
		goalLabel->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(goalLabel, sizeTitle, sizeTitle.width * 0.05f);
		infoTitle->addChild(goalLabel);
	}

	cWindow* subjectWnd = nullptr;
	cocos2d::Size szSubjectWnd = { szIBG.width - indent * 2.0f, freeSpace / 4.5f };
	{
		subjectWnd = new cWindow(szSubjectWnd, W_MUTE, 10);
		subjectWnd->setPosition(0, goalWnd->getPosition().y - szGoalWnd.height / 2.0f - indent * 2.0f - szSubjectWnd.height / 2.0f);
		subjectWnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(subjectWnd);

		cocos2d::Size sizeTitle = { szSubjectWnd.width / 2.5f, indent * 2.0f };
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
		}
	}

	cWindow* teacherWnd = nullptr;
	cocos2d::Size szTeacherWnd = { szIBG.width - indent * 2.0f, freeSpace / 3.0f };
	{
		teacherWnd = new cWindow(szTeacherWnd, W_MUTE, 10);
		teacherWnd->setPosition(0, subjectWnd->getPosition().y - szSubjectWnd.height / 2.0f - indent * 2.0f - szTeacherWnd.height / 2.0f);
		teacherWnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(teacherWnd);

		cocos2d::Size sizeTitle = { szTeacherWnd.width / 2.5f, indent * 2.0f };
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
		teacherSubject->setPosition(spriteSize.width / 2.0f - tSubject.width /2.0f, -szTeacherWnd.height / 4.5f);
		teacherWnd->addChild(teacherSubject);

		cocos2d::Label* skillLabel = cocos2d::Label::createWithTTF(*gTtfConfig,std::to_string(int(-(1.0f - mode->teacherCard->stat) * 100.0f)) + "%", cocos2d::TextHAlignment::CENTER);
		skillLabel->setColor(cocos2d::Color3B(247, 116, 98));
		setNodeInRect(skillLabel, tSubject, tSubject.width * 0.01f);
		cocos2d::Size slSize = skillLabel->getContentSize() * skillLabel->getScale();
		skillLabel->setPosition(teacherSubject->getPosition().x + tSubject.width + halfindent, teacherSubject->getPosition().y);
		teacherWnd->addChild(skillLabel);
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

	examGame->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
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
cMMTurnsIntroNode::~cMMTurnsIntroNode()
{
}
void cMMTurnsIntroNode::step(float dt)
{
	cocos2d::Vec2 sz = this->getParent()->convertToWorldSpace(this->getPosition());
	layer->setPosition(-sz.x, -sz.y);
}
void cMMTurnsIntroNode::reset()
{
}

/////
cMMTurnsOutroNode::cMMTurnsOutroNode(cocos2d::Size size, int turns, int turnCount, float currentScore, float goal, int dayC)
{
	barStepTime = 0;
	dayCost = dayC;

	cScore = currentScore;
	cGoal = goal;
	cTurn = turns;
	cTurnCount = turnCount;

	cUserDefault *userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;
	auto gTtfConfigHd = userDefault->fontHd;

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	float indent = visibleSize.height / 70.0f;

	float topInterface = visibleSize.height / 20.0f,
		  btmInterface = visibleSize.width / 2.5f / 3.0f,
		  mdlInterface = visibleSize.height * 0.7f;


	float proc = currentScore / (goal / 100.0f);
	std::string resultMark = u8"Неуд.";
	mark = 2;

	if (proc >= 100.0f)
	{
		resultMark = u8"Отл.";
		mark = 5;
	}
	else if (proc >= 75.0f)
	{
		resultMark = u8"Хор.";
		mark = 4;
	}
	else if (proc >= 50.0f)
	{
		resultMark = u8"Удв.";
		mark = 3;
	}

	cocos2d::Size szResultWnd = { mdlInterface * 0.7f, mdlInterface };
	cWindow* resultWnd = new cWindow(szResultWnd, W_MUTE, 10);
	//Победа порожение 
	{

		resultWnd->setCascadeOpacityEnabled(false);
		resultWnd->setWColor(cocos2d::Color3B(255, 255, 255));
		auto ch = resultWnd->getChildren();
		for (auto it : ch) it->setOpacity(150);
		this->addChild(resultWnd);
		
		cWindow* scoreWnd = nullptr;
		cocos2d::Size szScoreWnd = { szResultWnd.width - indent * 2.0f, szResultWnd.height * 0.15f };
		{
			scoreWnd = new cWindow(szScoreWnd, W_MUTE, 10);
			scoreWnd->setPosition(0, szResultWnd.height / 2.0f - indent * 2.0 - szScoreWnd.height / 2.0f);
			scoreWnd->setWColor(cocos2d::Color3B(96, 139, 171));
			resultWnd->addChild(scoreWnd);

			cocos2d::Size sizeTitle = { szScoreWnd.width / 2.5f, indent * 2.0f };
			cWindow* infoTitle = new cWindow(sizeTitle, W_MUTE, 10);
			infoTitle->setWColor(cocos2d::Color3B(200, 200, 200));
			infoTitle->setPosition(-szScoreWnd.width / 2.0f + indent + sizeTitle.width / 2.0f, szScoreWnd.height / 2.0f);
			scoreWnd->addChild(infoTitle);

			cocos2d::Label* titleLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Очки", cocos2d::TextHAlignment::CENTER);
			titleLabel->setColor(cocos2d::Color3B(96, 139, 171));
			setNodeInRect(titleLabel, sizeTitle, sizeTitle.width * 0.05f);
			infoTitle->addChild(titleLabel);

			aBarSize = cocos2d::Size{ szScoreWnd.width - indent * 2.0f, szScoreWnd.height - indent * 4.0f };
			cWindow *bar = new cWindow(aBarSize, W_MUTE, 0);
			bar->setPosition(0, aBarSize.height / 2.0f - szScoreWnd.height / 2.0f + indent);
			bar->setWColor(cocos2d::Color3B(0, 0, 0));
			bar->setCascadeOpacityEnabled(false);
			auto ch = bar->getChildren();
			for (auto it : ch) it->setOpacity(100);
			scoreWnd->addChild(bar);

			scoreBar = new cWindow(aBarSize, W_MUTE, 0);
			scoreBar->setWColor(cocos2d::Color3B(70, 180, 70));
			bar->addChild(scoreBar);
			//Arrows
			{
				cocos2d::Sprite *minMark = cocos2d::Sprite::create("icons/back.png");
				minMark->setRotation(-90);
				minMark->setColor(cocos2d::Color3B(200, 200, 200));

				cocos2d::Size szArrow = cocos2d::Size(indent, indent);
				setNodeInRect(minMark, szArrow, 0);
				bar->addChild(minMark);

				cocos2d::Sprite *goalMark = cocos2d::Sprite::create("icons/back.png");
				goalMark->setRotation(-90);
				goalMark->setColor(cocos2d::Color3B(200, 200, 200));
				setNodeInRect(goalMark, szArrow, 0);
				bar->addChild(goalMark);
				float min = 0, posGoal = 0;
				if (goal < currentScore)
				{
					posBar = 100.0f;
					posGoal = goal / (currentScore / 100.0f);
					min = posGoal / 2.0f;
				}
				else
				{
					posBar =  currentScore / (goal / 100.0f);
					posGoal = 100.0f;
					min = posGoal / 2.0f;
				}
				minMark->setPosition(-aBarSize.width / 2.0f + aBarSize.width / 100.0f * min, aBarSize.height / 2.0f + aBarSize.height / 8.0f);
				goalMark->setPosition(-aBarSize.width / 2.0f + aBarSize.width / 100.0f * posGoal, aBarSize.height / 2.0f + aBarSize.height / 8.0f);


				cocos2d::Label* lMin = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Мин.", cocos2d::TextHAlignment::CENTER);
				lMin->setColor(cocos2d::Color3B(200, 200, 200));
				setNodeSize(lMin, szArrow.height,true, false);
				lMin->setPosition(minMark->getPosition().x, minMark->getPosition().y + szArrow.height);
				bar->addChild(lMin);

				cocos2d::Label* lMax= cocos2d::Label::createWithTTF(*gTtfConfig, u8"Макс.", cocos2d::TextHAlignment::CENTER);
				lMax->setColor(cocos2d::Color3B(200, 200, 200));
				setNodeSize(lMax, szArrow.height, true, false);
				lMax->setPosition(goalMark->getPosition().x, goalMark->getPosition().y + szArrow.height);
				bar->addChild(lMax);
			}

			sbLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"0", cocos2d::TextHAlignment::CENTER);
			sbLabel->setColor(cocos2d::Color3B(200, 200, 200));
			setNodeInRect(sbLabel, aBarSize, aBarSize.height * 0.1f);
			scoreBar->addChild(sbLabel);
		}

		cWindow* markWnd = nullptr;
		cocos2d::Size szMarkWnd = { szScoreWnd.width, szResultWnd.height * 0.5f };
		{
			markWnd = new cWindow(szMarkWnd, W_MUTE, 10);
			markWnd->setPosition(0, scoreWnd->getPosition().y - szScoreWnd.height / 2.0f - indent * 2.0f - szMarkWnd.height / 2.0f);
			markWnd->setWColor(cocos2d::Color3B(96, 139, 171));
			resultWnd->addChild(markWnd);

			cocos2d::Size sizeTitle = { szMarkWnd.width / 2.5f, indent * 2.0f };
			cWindow* infoTitle = new cWindow(sizeTitle, W_MUTE, 10);
			infoTitle->setWColor(cocos2d::Color3B(200, 200, 200));
			infoTitle->setPosition(-szMarkWnd.width / 2.0f + indent + sizeTitle.width / 2.0f, szMarkWnd.height / 2.0f);
			markWnd->addChild(infoTitle);

			cocos2d::Label* titleLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Подробно", cocos2d::TextHAlignment::CENTER);
			titleLabel->setColor(cocos2d::Color3B(96, 139, 171));
			setNodeInRect(titleLabel, sizeTitle, sizeTitle.width * 0.05f);
			infoTitle->addChild(titleLabel);

			auto cScore = std::to_string((int)currentScore) + "." + std::to_string(int((currentScore - (int)currentScore) * 100.0f));
			auto cGoal = std::to_string((int)goal) + "." + std::to_string(int((goal - (int)goal) * 100.0f));

			std::vector<std::string> names = {u8"Счет", u8"Оценка", u8"Ходы", u8"Ср.б.", u8"Затр. дни" };
			std::vector<std::string> val = { cScore + " / " + cGoal, resultMark,
												std::to_string(turns) + " / " + std::to_string(turnCount), 
												u8"None", 
												std::to_string(dayC) };

			cocos2d::Size nWndSize = { (szMarkWnd.width - indent * 3.0f) * 0.3f, (szMarkWnd.height - indent * 7.0f) / 5.0f };
			cocos2d::Size vWndSize = { (szMarkWnd.width - indent * 3.0f) * 0.7f, nWndSize.height };

			for (int i = 0; i < 5; i++)
			{
				cWindow* nameWnd = new cWindow(nWndSize, W_MUTE, 10);
				nameWnd->setPosition(-szMarkWnd.width / 2.0f + indent + nWndSize.width / 2.0f, (szMarkWnd.height / 2.0f - indent * 2.0f - nWndSize.height / 2.0f) - (nWndSize.height + indent) * i);
				nameWnd->setWColor(cocos2d::Color3B(0, 0, 0));
				ch = nameWnd->getChildren();
				for (auto it : ch) it->setOpacity(100);
				markWnd->addChild(nameWnd);

				cocos2d::Label* nLabel = cocos2d::Label::createWithTTF(*gTtfConfig, names.at(i), cocos2d::TextHAlignment::CENTER);
				setNodeInRect(nLabel, nWndSize * 0.8f, 0);
				nLabel->setColor(cocos2d::Color3B(200, 200, 200));
				nameWnd->addChild(nLabel);

				cWindow* valWnd = new cWindow(vWndSize, W_MUTE, 10);
				valWnd->setPosition(nWndSize.width / 2.0f + vWndSize.width /2.0f + indent, 0);
				valWnd->setWColor(cocos2d::Color3B(0, 0, 0));
				ch = valWnd->getChildren();
				for (auto it : ch) it->setOpacity(100);
				nameWnd->addChild(valWnd);

				cocos2d::Label* vLabel = cocos2d::Label::createWithTTF(*gTtfConfig, val.at(i), cocos2d::TextHAlignment::CENTER);
				setNodeInRect(vLabel, vWndSize * 0.8f, 0);
				vLabel->setColor(cocos2d::Color3B(200, 200, 200));
				valWnd->addChild(vLabel);

			}
		}
	}

	//bottom btns
	cButton *btnOk = nullptr, *btnCancel = nullptr;
	cocos2d::Size szBtn = { szResultWnd.width / 3.0f, szResultWnd.width / 3.0f / 3.0f };
	{
		btnOk = new cButton(nullptr, szBtn, 1.03f, -11);
		btnOk->setPosition((mdlInterface * 0.7f) / 2.0 - indent - szBtn.width / 2.0f, -mdlInterface  / 2.0 + indent + szBtn.height / 2.0f);
		btnOk->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
		resultWnd->addChild(btnOk);

		cocos2d::Label* OkLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Завершить", cocos2d::TextHAlignment::CENTER);
		OkLabel->setColor(cocos2d::Color3B(200, 200, 200));
		setNodeInRect(OkLabel, szBtn, szBtn.height * 0.6f);
		btnOk->addChild(OkLabel);

		btnOk->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			if (turns >= turnCount || currentScore >= goal)
			{
			}
			cUserDefault *userDefault = cUserDefault::getInstance();
			auto &level = userDefault->profile["aLevel"];

			int sh = level[0] - 1;
			int cl = level[1] - 1;
			int sem = level[2] - 1;
			int day = level[3] - 1;

			std::string key = std::to_string(level[0].get<int>()) + "|" + std::to_string(level[1].get<int>()) + "|" + std::to_string(level[2].get<int>()) + "|" + std::to_string(level[3].get<int>());
			userDefault->addHistoryElement(key, u8"Экзамен: ходы", mark);
			while (dayCost > 0)
			{
				if (day < int(userDefault->schools.at(sh).classes.at(cl).semesters.at(sem).days.size() - 1)) day++;
				else
				{
					if (sem < int(userDefault->schools.at(sh).classes.at(cl).semesters.size() - 1)) sem++;
					else
					{
						if (cl < int(userDefault->schools.at(sh).classes.size() - 1)) cl++;
						else
						{
							if (sh < int(userDefault->schools.size() - 1)) sh++;
							else
							{
								//Fucking END
							}
							cl = 0;
						}
						sem = 0;
					}
					day = 0;
				}
				dayCost--;
			}

			level[0] = sh + 1;
			level[1] = cl + 1;
			level[2] = sem + 1;
			level[3] = day + 1;
			userDefault->saveProfile();
			userDefault->saveHistory();

			cSceneManager *sceneManager = cSceneManager::getInstance();
			sceneManager->changeScene("menu");
			return true;
		};

		btnCancel = new cButton(nullptr, cocos2d::Size(szBtn.height, szBtn.height), 1.03f, -11);
		btnCancel->setPosition(btnOk->getPosition().x - indent - szBtn.width / 2.0f - szBtn.height / 2.0f, btnOk->getPosition().y);
		btnCancel->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
		resultWnd->addChild(btnCancel);

		cocos2d::Sprite *icon_res = cocos2d::Sprite::create("icons/res.png");
		setNodeInRect(icon_res, cocos2d::Size(szBtn.height, szBtn.height) * 0.8f, 0);
		icon_res->setOpacity(150);
		icon_res->setLocalZOrder(4);
		btnCancel->addChild(icon_res);

		btnCancel->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			cSceneManager *sceneManager = cSceneManager::getInstance();
			sceneManager->changeScene("game");
			return true;
		};

	}

}
cMMTurnsOutroNode::~cMMTurnsOutroNode()
{
}
void cMMTurnsOutroNode::step(float dt)
{
	float resultProcScore = cScore / (cGoal / 100.0f);
	barStepTime += dt;
	float dX = 10 - barStepTime;
	if (dX > 0)
	{
		float dY = 100 - (dX * dX * dX * dX) / 100.0f;
		dY = posBar / 100.0f * dY;
		auto newSize = cocos2d::Size(aBarSize.width / 100.0f * dY, aBarSize.height);
		scoreBar->setWSize(newSize);
		scoreBar->setPosition(-aBarSize.width /2.0f + (aBarSize.width / 100.0f * dY) / 2.0f, 0);

		if(resultProcScore > 100.0f)
			dY = resultProcScore / 100.0f * dY;

		sbLabel->setString(std::to_string((int)dY) + "." + std::to_string(int((dY - (int)dY) * 10.0f)) + "%");
		setNodeInRect(sbLabel, newSize, newSize.height * 0.5f);
	}
}
void cMMTurnsOutroNode::reset()
{
}

/////
cMMTurns::cMMTurns(std::string modeName)
{
 	cUserDefault *userDefault = cUserDefault::getInstance();
	auto skills = userDefault->profile["aStats"];
	for (unsigned int i = 0; i < skills.size(); i++) playerSkills.push_back(skills[i]);

	auto level = userDefault->profile["aLevel"];
	int sh = level[0] - 1;
	int cl = level[1] - 1;
	int sem = level[2] - 1;
	int day = level[3] - 1;
	gameLevel = { sh , cl ,sem, day };

	turns = 0;
	currentScore = 0;


	std::string data = generateMode(modeName);
	std::vector<std::string> strVec = Split(data, '|');

	for (auto it : userDefault->cards)
		if (it->id == strVec.at(3))
		{
			teacherCard = (cTeacherCard*)it;
			break;
		}

	std::string subStr = strVec.at(0);
	subStr.pop_back();
	subStr.erase(subStr.begin());
	std::vector<std::string> strVecSub = Split(subStr, ',');
	subjects.clear();
	for (auto it : strVecSub) subjects.push_back(std::stoi(it));

	goal = std::stof(strVec.at(2));
	turnCount = std::stoi(strVec.at(1));

	dayCost = 1;
}
cMMTurns::~cMMTurns()
{
}
cDinamicLayer* cMMTurns::getModeIntro(cocos2d::Size size)
{
	cMMTurnsIntroNode *node = new cMMTurnsIntroNode(size, this);
	return node;
}
cDinamicLayer* cMMTurns::getGameNode(cocos2d::Size size)
{
	cMMTurnsGameNode *node = new cMMTurnsGameNode(size, playerSkills, gameLevel, subjects, teacherCard, goal, turnCount, dayCost);
	return node;
}
cDinamicLayer* cMMTurns::getModeOutro(cocos2d::Size size)
{
	cMMTurnsOutroNode *node = new cMMTurnsOutroNode(size, turns, turnCount, currentScore, goal, dayCost);
	return node;
}
std::string cMMTurns::generateMode(std::string modeName)
{
	std::string result;

	int MIN_TURNS = 20;
	int MAX_TURNS = 50;
	int STEP_TURNS = 5;
	int MIN_LINE = 3;
	int MAX_LINE = 15;



	cUserDefault *userDefault = cUserDefault::getInstance();
	auto level = userDefault->profile["aLevel"];
	int sh = level[0] - 1;
	int cl = level[1] - 1;
	int sem = level[2] - 1;
	int day = level[3] - 1;
	std::vector<int> groupLevel = { sh , cl ,sem, day };

	std::string gameName = std::to_string(sh) + "|" + std::to_string(cl) + "|" + std::to_string(sem) + "|" + std::to_string(day) + "|" + modeName;


	if (userDefault->generatedModes[gameName] != nullptr)
		return userDefault->generatedModes[gameName];

	///Dinamic skill wrong!!!!!!!!!!!!
	auto skills = userDefault->profile["aStats"];
	std::vector<float> playerSkills;
	float minPS = skills[0], maxPS = skills[0];
	for (unsigned int i = 0; i < skills.size(); i++)
	{
		float val = skills[i];
		if (val < minPS) minPS = val;
		if (val > maxPS) maxPS = val;
		playerSkills.push_back(val);
	}

	//Генерация предметов
	std::vector<int> var = { 1, 2, 3, 4, 5, 6, 7, 8 };
	std::string subjectGen = "[";
	std::vector<int> subjectVec;
	for (int i = 0; i < 4; i++)
	{
		int num = rand() % var.size();
		subjectVec.push_back(var.at(num));
		subjectGen += std::to_string(var.at(num));
		var.erase(var.begin() + num);
		if (i < 3)subjectGen += ",";
	}
	subjectGen += "]";
	result += subjectGen + "|";

	turnCount = MIN_TURNS + (rand() % (int((MAX_TURNS - MIN_TURNS) / STEP_TURNS))) * STEP_TURNS;
	result += std::to_string(turnCount) + "|";

	float minScore = MIN_LINE * turnCount * minPS;
	float maxScore = MAX_LINE * turnCount * incr.at(MAX_LINE-1) * maxPS;
	//maxScore = minScore + ( maxScore - minScore ) / 2.0f;

	//minScore = maxScore / 2.0f;
	maxScore = maxScore * 0.8f; //FOR MY SKILL
	result += std::to_string( int(minScore + (maxScore - minScore) / 6.0f) ); //middle


	std::string data = userDefault->generateCard(subjectVec);
	std::vector<std::string> vecStr = Split(data, '/');

	cTeacherCard *card = new cTeacherCard(vecStr.at(0), vecStr.at(1));
	userDefault->cards.push_back(card);

	result += "|" + vecStr.at(0) + "|";

	//day cost
	int days_gen = -1;
	result += std::to_string(days_gen);

	userDefault->generatedModes[gameName] = result;

	userDefault->saveModes();

	return result;
}
