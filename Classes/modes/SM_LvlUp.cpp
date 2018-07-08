#include "SM_LvlUp.hpp"

cSMLvlUp::cSMLvlUp(std::string modeName)
{
	std::string data = generateMode(modeName);
	std::vector<std::string> strVec = Split(data, '|');

	turnCount = std::stoi(strVec.at(0));

	cUserDefault *userDefault = cUserDefault::getInstance();
	auto skills = userDefault->profile["aStats"];
	for (unsigned int i = 0; i < 8; i++) playerSkills.push_back(skills[i]);
}
cSMLvlUp::~cSMLvlUp()
{
}

cDinamicLayer* cSMLvlUp::getModeIntro(cocos2d::Size size)
{
	cSMLvlUpIntro *layer = new cSMLvlUpIntro(size, this);
	return layer;
}
cDinamicLayer* cSMLvlUp::getGameNode(cocos2d::Size size)
{
	cSMLvlUpGame *node = new cSMLvlUpGame(size, turnCount, dayCost, subjects, playerSkills);
	return node;
}
cDinamicLayer* cSMLvlUp::getModeOutro(cocos2d::Size size)
{
	return nullptr;
}
std::string cSMLvlUp::generateMode(std::string modeName)
{
	std::string result;

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


	//steps
	int MIN_STEPS = 10;
	int MAX_STEPS = 35;

	int dStep = MAX_STEPS - MIN_STEPS;
	int rndSize = dStep / 5;

	int stepCount = MIN_STEPS + rand() % rndSize * 5;
	result = std::to_string(stepCount) + "|";

	//day cost
	int days_gen = -1;
	result += std::to_string(days_gen);

	userDefault->generatedModes[gameName] = result; 
	userDefault->saveModes();

	return result;
}

cSMLvlUpIntro::cSMLvlUpIntro(cocos2d::Size size, cBaseMode *thisMode)
{
	listenNode = new cBaseNode();
	cSMLvlUp* mode = (cSMLvlUp*)thisMode;

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
	logo->setPosition(-size.width / 2.0f + indent + szLogo.width / 2.0f, size.height / 2.0f - indent - szLogo.height / 2.0f);
	logo->setWColor(cocos2d::Color3B(96, 139, 171));
	examGame->addChild(logo);
	cocos2d::Sprite *logo_3 = cocos2d::Sprite::create("gameTypes/stats.png");
	setNodeInRect(logo_3, szLogo, szLogo.width * 0.2f);
	logo->addChild(logo_3);

	layer = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 200));
	this->addChild(layer);
	layer->setVisible(false);


	//width / 6
	cWindow* infoBg = nullptr;
	cocos2d::Size szIBG = { visibleSize.width * 0.8f,  visibleSize.height * 0.7f };
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

		cocos2d::Label* exam = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Подготовка", cocos2d::TextHAlignment::CENTER);
		exam->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(exam, szLogoTitle, szLogoTitle.width * 0.05f);
		logoTitle->addChild(exam);
	}

	float freeSpace = szIBG.height - szLogoSmall.height - indent * 10.0f;
	cWindow* goalWnd = nullptr;
	cocos2d::Size szGoalWnd = { szIBG.width - indent * 2.0f, freeSpace * 0.2f };
	{
		goalWnd = new cWindow(szGoalWnd, W_MUTE, 10);
		goalWnd->setPosition(0, logoTitle->getPosition().y - szLogoTitle.height / 2.0f - indent * 3.0f - szGoalWnd.height / 2.0f);
		goalWnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(goalWnd);
		std::string text = u8"За " + std::to_string(mode->turnCount) + u8" ходов необходимо\n собрать опр. количество очков\n для любых выбранных предметов.";
		cocos2d::Label* goalText = cocos2d::Label::createWithTTF(*gTtfConfig, text, cocos2d::TextHAlignment::CENTER);
		goalText->setColor(cocos2d::Color3B(200, 200, 200));
		setNodeInRect(goalText, szGoalWnd, szGoalWnd.width * 0.05f);
		goalText->setPosition(-szGoalWnd.width / 2.0f + szGoalWnd.width / 2.0f, szGoalWnd.height / 2.0f - indent * 0.5f - szGoalWnd.height / 2.0f);
		goalWnd->addChild(goalText);

		cocos2d::Size sizeTitle = { szGoalWnd.width / 2.5f, indent * 2.0f };
		cWindow* infoTitle = new cWindow(sizeTitle, W_MUTE, 10);
		infoTitle->setWColor(cocos2d::Color3B(200, 200, 200));
		infoTitle->setPosition(-szGoalWnd.width / 2.0f + indent + sizeTitle.width / 2.0f, szGoalWnd.height / 2.0f);
		goalWnd->addChild(infoTitle);

		cocos2d::Label* goalLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Цель", cocos2d::TextHAlignment::CENTER);
		goalLabel->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(goalLabel, sizeTitle, sizeTitle.width * 0.05f);
		infoTitle->addChild(goalLabel);
	}

	cWindow* subjectWnd = nullptr;
	cocos2d::Size szSubjectWnd = { szIBG.width - indent * 2.0f, freeSpace * 0.4f };
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

		cocos2d::Label* subLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Выберите предметы", cocos2d::TextHAlignment::CENTER);
		subLabel->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(subLabel, sizeTitle, sizeTitle.width * 0.05f);
		infoTitle->addChild(subLabel);

		int xPos = 0, yPos = 0;
		for (unsigned int i = 0; i < 8; i++)
		{
			float locIndent = indent * 1.5f;
			float width = (szSubjectWnd.width - locIndent * 5.0f) / 4.0f;
			if (width > szSubjectWnd.height - indent * 2.0f)width = szSubjectWnd.height - indent * 2.0f;
			cocos2d::Size sizeWnd = { width, width };
			cButton* btn = new cButton(nullptr, sizeWnd, 1.03f, -11);
			btn->wnd->setWColor(cocos2d::Color3B(200, 200, 200));
			btn->setPosition(((-szSubjectWnd.width / 2.0f + locIndent + width / 2.0f) + (width + locIndent) * xPos), (szSubjectWnd.height / 2.0f - locIndent - sizeWnd.height / 2.0f) - (sizeWnd.height + halfindent) * yPos);
			subjectWnd->addChild(btn);
			vecSubject.push_back(btn);
			cocos2d::Sprite *subjSprite = cocos2d::Sprite::create("gameScreen/" + std::to_string(i + 1) + ".png");
			setNodeInRect(subjSprite, sizeWnd, sizeWnd.width * 0.1f);
			btn->addChild(subjSprite);

			btn->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
			{
				bool has = false;
				for (unsigned int j = 0; j < activeSubjects.size(); j++)
				{
					if (activeSubjects.at(j) == i)
					{
						has = true;
						activeSubjects.erase(activeSubjects.begin() + j);
					}
				}

				if (has == false)
				{
					if (activeSubjects.size() < 4)
					{
						activeSubjects.push_back(i);
					}
					else if (activeSubjects.size() == 4)
					{
						for (unsigned int j = 0; j < activeSubjects.size(); j++)
						{
							if (j != 0)
							{
								activeSubjects.at(j - 1) = activeSubjects.at(j);
							}
						}
						activeSubjects.back() = i;
					}
				}

				mode->subjects = activeSubjects;
				return true;
			};


			if (xPos > 2)
			{
				xPos = 0;
				yPos++;
			}
			else xPos++;
		}

	}

	cWindow* priceWnd = nullptr;
	cocos2d::Size szPriceWnd = { szIBG.width - indent * 2.0f, freeSpace * 0.2f };
	{
		priceWnd = new cWindow(szPriceWnd, W_MUTE, 10);
		priceWnd->setPosition(0, subjectWnd->getPosition().y - szSubjectWnd.height / 2.0f - indent * 2.0f - szPriceWnd.height / 2.0f);
		priceWnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(priceWnd);

		cocos2d::Size sizeTitle = { szPriceWnd.width / 2.5f, indent * 2.0f };
		cWindow* infoTitle = new cWindow(sizeTitle, W_MUTE, 10);
		infoTitle->setWColor(cocos2d::Color3B(200, 200, 200));
		infoTitle->setPosition(-szPriceWnd.width / 2.0f + indent + sizeTitle.width / 2.0f, szPriceWnd.height / 2.0f);
		priceWnd->addChild(infoTitle);

		cocos2d::Label* priceLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Награда", cocos2d::TextHAlignment::CENTER);
		priceLabel->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(priceLabel, sizeTitle, sizeTitle.width * 0.05f);
		infoTitle->addChild(priceLabel);

		//Price icon and text
		{
			cocos2d::Size iconSize = { szPriceWnd.height - indent * 3.0f,  szPriceWnd.height - indent * 3.0f };
			cocos2d::Sprite *elem = cocos2d::Sprite::create("icons/element.png");
			elem->setColor(cocos2d::Color3B(200, 200, 200));
			setNodeSize(elem, iconSize.width, false, true);
			elem->setPosition(-szPriceWnd.width / 2.0f + indent + iconSize.width / 2.0f, -indent / 2.0f);
			priceWnd->addChild(elem);

			cocos2d::Sprite *question = cocos2d::Sprite::create("icons/question.png");
			setNodeInRect(question, iconSize, iconSize.width * 0.3f);
			question->setColor(cocos2d::Color3B(96, 139, 171));
			question->setPosition(iconSize / 2.0f);
			elem->addChild(question);

			cocos2d::Sprite *upSign = cocos2d::Sprite::create("icons/upVal.png");
			setNodeInRect(upSign, iconSize / 2.0f, 0);
			upSign->setColor(cocos2d::Color3B(131, 204, 101));
			upSign->setPosition(iconSize.width * 0.9f, iconSize.height * 0.8f);
			elem->addChild(upSign);

			cocos2d::Label* textPrice = cocos2d::Label::createWithTTF(*gTtfConfig, u8"- Повышение навыков игрока.", cocos2d::TextHAlignment::CENTER);
			textPrice->setColor(cocos2d::Color3B(200, 200, 200));
			setNodeSize(textPrice, szPriceWnd.width - indent * 3 - iconSize.width, true, true);
			cocos2d::Size txtSize = textPrice->getContentSize() * textPrice->getScale();
			textPrice->setPosition(elem->getPosition().x + indent + txtSize.width / 2.0f + iconSize.width / 2.0f, elem->getPosition().y);
			priceWnd->addChild(textPrice);
		}
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
			if(activeSubjects.size() != 4) return true;
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
cSMLvlUpIntro::~cSMLvlUpIntro()
{
}
void cSMLvlUpIntro::step(float dt)
{
	cocos2d::Vec2 sz = this->getParent()->convertToWorldSpace(this->getPosition());
	layer->setPosition(-sz.x, -sz.y);

	for(unsigned int i = 0; i < vecSubject.size(); i++)
	{
		vecSubject.at(i)->setOpacity(100);
	}
	for (unsigned int i = 0; i < activeSubjects.size(); i++)
	{
		vecSubject.at(activeSubjects.at(i))->setOpacity(255);
	}

}
void cSMLvlUpIntro::reset()
{
}

cSMLvlUpGame::cSMLvlUpGame(cocos2d::Size size, int inTurnCount, int days, std::vector<int> inSubjects, std::vector<float> inPlayerSkills)
{
	dayCost = days;
	turnCount = inTurnCount;
	playerSkills = inPlayerSkills;
	turns = 0;
	elementCount = 25;
	addtime = 0;
	auto userDefault = cUserDefault::getInstance();
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	auto gTtfConfig = userDefault->font;

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

	cocos2d::Size ifSize = { btmBar->getWSize().width, size.height - btmBarSize.height - indent };
	cWindow* infoField = new cWindow(ifSize, W_MUTE, 0);
	infoField->setPosition(btmBar->getPosition().x, btmBar->getPosition().y + btmBar->getWSize().height / 2.0f + indent + ifSize.height / 2.0f);
	infoField->setWColor(cocos2d::Color3B(0, 0, 0));
	infoField->setCascadeOpacityEnabled(false);
	auto ch = infoField->getChildren();
	for (auto it : ch) it->setOpacity(100);
	this->addChild(infoField);

	cocos2d::Size lnSize = { (ifSize.width - indent * 2.0f - halfindent) * 0.25f, ifSize.height / 11.0f };
	turnBarSize = cocos2d::Size{ (ifSize.width - indent * 2.0f - halfindent) * 0.75f, lnSize.height };

	//turn bar
	{
		cWindow* turnLabelBg = new cWindow(lnSize, W_MUTE, 0);
		turnLabelBg->setPosition(-ifSize.width / 2.0f + lnSize.width / 2.0f + indent, ifSize.height / 2.0f - indent - lnSize.height / 2.0f);
		turnLabelBg->setWColor(cocos2d::Color3B(0, 0, 0));
		turnLabelBg->setCascadeOpacityEnabled(false);
		auto ch = turnLabelBg->getChildren();
		for (auto it : ch) it->setOpacity(100);
		infoField->addChild(turnLabelBg);

		cWindow* turnBarBg = new cWindow(turnBarSize, W_MUTE, 0);
		turnBarBg->setPosition(turnLabelBg->getPosition().x + halfindent + turnBarSize.width / 2.0f + lnSize.width / 2.0f, turnLabelBg->getPosition().y);
		turnBarBg->setWColor(cocos2d::Color3B(0, 0, 0));
		turnBarBg->setOpacity(100);
		infoField->addChild(turnBarBg);

		turnBarFill = new cWindow(turnBarSize, W_MUTE, 0);
		turnBarFill->setWColor(cocos2d::Color3B(70, 200, 70));
		turnBarFill->setCascadeOpacityEnabled(false);
		ch = turnBarFill->getChildren();
		for (auto it : ch) it->setOpacity(200);
		turnBarFill->setWSize(cocos2d::Size(turnBarFill->getWSize().width / 4.0f, turnBarFill->getWSize().height));
		turnBarBg->addChild(turnBarFill);

		cocos2d::Label *scoreLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Ходы", cocos2d::TextHAlignment::CENTER);
		setNodeSize(scoreLabel, lnSize.height * 0.6f, true, false);
		turnLabelBg->addChild(scoreLabel);

		cocos2d::Label *lMaxScore = cocos2d::Label::createWithTTF(*gTtfConfig, std::to_string(turnCount), cocos2d::TextHAlignment::CENTER);
		setNodeSize(lMaxScore, turnBarSize.height * 0.6f, true, false);
		cocos2d::Size scLabelSize = lMaxScore->getContentSize() * lMaxScore->getScale();
		lMaxScore->setPosition(turnBarSize.width / 2.0f - scLabelSize.width / 2.0f - turnBarSize.height * 0.3f, 0);
		turnBarBg->addChild(lMaxScore);

		cocos2d::Label *lCurrentScore = cocos2d::Label::createWithTTF(*gTtfConfig, std::to_string(0), cocos2d::TextHAlignment::CENTER);
		setNodeSize(lCurrentScore, turnBarSize.height * 0.6f, true, false);
		cocos2d::Size scCrrLabelSize = lCurrentScore->getContentSize() * lCurrentScore->getScale();
		lCurrentScore->setPosition(turnBarSize.width / 2.0f - scCrrLabelSize.width / 2.0f - turnBarSize.height * 0.3f, 0);
		turnBarFill->addChild(lCurrentScore);
	}

	//stats
	{
		cocos2d::Size swSize = { ifSize.width - indent * 2.0f, ifSize.height - indent * 3.0f - turnBarSize.height };
		cWindow* statWnd = new cWindow(swSize, W_MUTE, 0);
		statWnd->setPosition(0, ifSize.height / 2.0f - indent * 2.0f - turnBarSize.height - swSize.height / 2.0f);
		statWnd->setWColor(cocos2d::Color3B(0, 0, 0));
		statWnd->setCascadeOpacityEnabled(false);
		auto ch = statWnd->getChildren();
		for (auto it : ch) it->setOpacity(100);
		infoField->addChild(statWnd);

		cocos2d::Size subSize = { (swSize.width - indent * 2.0f), (swSize.height - indent * 5.0f) / 4.0f };
		cocos2d::Size logoSize = { subSize.height - indent, subSize.height - indent };
		bSize = cocos2d::Size{ subSize.width - logoSize.width - indent * 3.0f , turnBarSize.height };

		for (unsigned int i = 0; i < 4; i++)
		{
			sSubjectState sub;
			sub.type = inSubjects.at(i);
			cWindow* subjectBg = new cWindow(subSize, W_MUTE, 0);
			subjectBg->setWColor(cocos2d::Color3B(96, 139, 171));

			subjectBg->setPosition((-swSize.width / 2.0f + indent + subSize.width / 2.0),
									(swSize.height / 2.0f - indent - subSize.height / 2.0) - (indent + subSize.height) * i);
			statWnd->addChild(subjectBg);

			cocos2d::Sprite* subjectLogo = cocos2d::Sprite::create("gameScreen/" + std::to_string(sub.type + 1) + ".png");
			setNodeInRect(subjectLogo, logoSize, 0);
			subjectLogo->setPosition(-subSize.width / 2.0f + indent + logoSize.width / 2.0f, 0);
			subjectBg->addChild(subjectLogo);

			cWindow* bar = new cWindow(bSize, W_MUTE, 0);
			bar->setPosition(subjectLogo->getPosition().x + indent + logoSize.width / 2.0f + bSize.width / 2.0f, 0);
			bar->setWColor(cocos2d::Color3B(0, 0, 0));
			ch = bar->getChildren();
			for (auto it : ch) it->setOpacity(100);
			subjectBg->addChild(bar);

			cWindow* barFill = new cWindow(bSize, W_MUTE, 0);
			barFill->setWColor(cocos2d::Color3B(200, 200, 200));
			bar->addChild(barFill);
			sub.progressBar = barFill;
			subjects.push_back(sub);
		}
	}

	updateInfoBar();
}
cSMLvlUpGame::~cSMLvlUpGame()
{
}
void cSMLvlUpGame::step(float dt)
{
	updateGameStats(dt);
	if (addtime > 0.2 && (int)field->getElements().size() < elementCount)
	{
		int randNum = rand() % 4;
		field->addElement(subjects.at(randNum).type + 1);
		addtime = 0;
	}
	addtime += dt;
}
void cSMLvlUpGame::reset()
{
}
void cSMLvlUpGame::updateInfoBar()
{
	//STATS
	for (unsigned int i = 0; i < subjects.size(); i++)
	{
		cocos2d::Size sz = cocos2d::Size(bSize.width / 5.0f, bSize.height);
		subjects.at(i).progressBar->setWSize(sz);
		subjects.at(i).progressBar->setPosition(-bSize.width / 2.0f + subjects.at(i).progressBar->getWSize().width / 2.0f, 0);
	}

	//TURNS
	float proc = (turnCount - turns) / (turnCount / 100.0f);
	cocos2d::Size sz = cocos2d::Size((turnBarSize.width / 100.0f) * proc, turnBarSize.height);
	turnBarFill->setWSize(sz);
	turnBarFill->setPosition(-turnBarSize.width / 2.0f + turnBarFill->getWSize().width / 2.0f, 0);
}
void cSMLvlUpGame::updateGameStats(float dt)
{
	//Расчет результата
	if (field->getTurnCount() != turns)
	{
		auto elements = field->getLastLine();
		float result = 0;
		for (auto it : elements)
		{
			result += playerSkills.at(it - 1);
		}
		///Расчеты

		if (incr.size() >= elements.size())result *= incr.at(elements.size() - 1);
		else result *= incr.back();

		//currentScore += result;
		turns = field->getTurnCount();
		if (turns > turnCount)turns = turnCount;

		updateInfoBar();
		checkGameFinish();
	}
}
void cSMLvlUpGame::checkGameFinish()
{
}

cSMLvlUpOutro::cSMLvlUpOutro()
{
}
cSMLvlUpOutro::~cSMLvlUpOutro()
{
}
void cSMLvlUpOutro::step(float dt)
{

}
void cSMLvlUpOutro::reset()
{
}
