#include "SM_Card.hpp"

cSMCard::cSMCard(std::string modeName)
{
	std::string data = generateMode(modeName);
	std::vector<std::string> strVec = Split(data, '|');

	turnCount = std::stoi(strVec.at(0));
	cardRareOpen = std::stoi(strVec.at(1));
	dayCost = 2; //std::stoi(strVec.at(2));
	cardDropInfo = {3, 6, 12, 15};
	cardDropInfo.resize(cardRareOpen);
}
cSMCard::~cSMCard()
{
}
cDinamicLayer* cSMCard::getModeIntro(cocos2d::Size size)
{
	cSMCardIntro *layer = new cSMCardIntro(size, this);
	return layer;
}
cDinamicLayer* cSMCard::getGameNode(cocos2d::Size size)
{
	cSMCardGame *node = new cSMCardGame(size, turnCount, dayCost, cardDropInfo);
	return node;
}
cDinamicLayer* cSMCard::getModeOutro(cocos2d::Size size)
{
	cSMCardOutro *node = new cSMCardOutro(turns, turnCount, dayCost, cardRares);
	return node;
}
std::string cSMCard::generateMode(std::string modeName)
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
	int MIN_STEPS = 5;
	int MAX_STEPS = 25;

	int dStep = MAX_STEPS - MIN_STEPS;
	int rndSize = dStep / 5;

	int stepCount = MIN_STEPS + rand() % rndSize * 5;
	result = std::to_string(stepCount) + "|";

	//Active cards
	int openCardRare = 2;
	if (userDefault->schools.at(sh).classes.at(cl).semesters.at(sem).days.size() / 2.0f - day < 0)openCardRare = 3;
	if(openCardRare == 3 && userDefault->schools.at(sh).classes.size() / 2.0f < cl)openCardRare = 4;
	result += std::to_string(openCardRare) + "|";

	//day cost
	int days_gen = -1;
	result += std::to_string(days_gen);

	userDefault->generatedModes[gameName] = result;
	userDefault->saveModes();

	return result;
}

///
cSMCardIntro::cSMCardIntro(cocos2d::Size size, cBaseMode *thisMode)
{
	timeChange = 0;
	prevColor = 0, nextColor = 1;
	gradeColors = {dGrade, cGrade, bGrade, aGrade};

	listenNode = new cBaseNode();
	cSMCard* mode = (cSMCard*)thisMode;
	turnCount = mode->turnCount;
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
	cocos2d::Sprite *logo_3 = cocos2d::Sprite::create("gameTypes/cards.png");
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

		cocos2d::Sprite *logoImage = cocos2d::Sprite::create("gameTypes/cards.png");
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

		cocos2d::Label* exam = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Карты", cocos2d::TextHAlignment::CENTER);
		exam->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(exam, szLogoTitle, szLogoTitle.width * 0.05f);
		logoTitle->addChild(exam);
	}

	float freeSpace = szIBG.height - szLogoSmall.height - indent * 10.0f;
	cWindow* goalWnd = nullptr;
	cocos2d::Size szGoalWnd = { szIBG.width - indent * 2.0f, freeSpace * 0.25f };
	{
		goalWnd = new cWindow(szGoalWnd, W_MUTE, 10);
		goalWnd->setPosition(0, logoTitle->getPosition().y - szLogoTitle.height / 2.0f - indent * 3.0f - szGoalWnd.height / 2.0f);
		goalWnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(goalWnd);

		std::string text = u8"Собрать за " + std::to_string(turnCount) + u8"ход. цепочи\n элементов максимальной длинны.";
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
		infoTitle->addChild(goalLabel);
	}

	cWindow* infoWnd = nullptr;
	cocos2d::Size szInfoWnd = { szIBG.width - indent * 2.0f, freeSpace * 0.4f };
	{
		infoWnd = new cWindow(szInfoWnd, W_MUTE, 10);
		infoWnd->setPosition(0, goalWnd->getPosition().y - szGoalWnd.height / 2.0f - indent * 2.0f - szInfoWnd.height / 2.0f);
		infoWnd->setWColor(cocos2d::Color3B(96, 139, 171));
		infoBg->addChild(infoWnd);

		cocos2d::Size sizeTitle = { szInfoWnd.width / 2.5f, indent * 2.0f };
		cWindow* infoTitle = new cWindow(sizeTitle, W_MUTE, 10);
		infoTitle->setWColor(cocos2d::Color3B(200, 200, 200));
		infoTitle->setPosition(-szInfoWnd.width / 2.0f + indent + sizeTitle.width / 2.0f, szInfoWnd.height / 2.0f);
		infoWnd->addChild(infoTitle);

		cocos2d::Label* priceLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Памятка", cocos2d::TextHAlignment::CENTER);
		priceLabel->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(priceLabel, sizeTitle, sizeTitle.width * 0.05f);
		infoTitle->addChild(priceLabel);


		float hightCard = (szInfoWnd.height - indent * 4.0f) / 2.0f;
		float widthCard = hightCard / (88.8f / 63.5f);
		cocos2d::Size cardSize = { widthCard, hightCard };

		cocos2d::Size textSize = { szInfoWnd.width /2.0f - widthCard - indent, hightCard / 3.0f };
		std::vector<cWindow*> cardsVec;
		for (unsigned int i = 0; i < 4; i++)
		{
			cWindow *card = new cWindow(cardSize * 2.0f, W_MUTE, 10);
			card->setScale(0.5f);
			card->setRotation(20);
			infoWnd->addChild(card);
			cocos2d::Sprite *cardDQuestion = cocos2d::Sprite::create("icons/question.png");
			cardDQuestion->setColor(cocos2d::Color3B(96, 139, 171));
			setNodeInRect(cardDQuestion, cardSize * 2.0f, cardSize.width * 2.0f * 0.2f);
			card->addChild(cardDQuestion);

			cardsVec.push_back(card);
		}

		cocos2d::Label* labelD;
		cardsVec.at(0)->setWColor(dGrade);
		cardsVec.at(0)->setPosition(-szInfoWnd.width / 2.0f + indent + cardSize.width / 2.0f, szInfoWnd.height / 2.0f - indent * 2.0f - cardSize.height / 2.0f);
		labelD = cocos2d::Label::createWithTTF(*gTtfConfig, u8"- 3 эл.", cocos2d::TextHAlignment::CENTER);
		setNodeInRect(labelD, textSize, 0);
		cocos2d::Size dLSize = labelD->getContentSize() * labelD->getScale();
		labelD->setPosition(cardsVec.at(0)->getPosition().x + cardSize.width / 1.5f + dLSize.width / 2.0f, cardsVec.at(0)->getPosition().y);
		labelD->setColor(cocos2d::Color3B(200, 200, 200));
		infoWnd->addChild(labelD);

		cocos2d::Label* labelC;
		cardsVec.at(1)->setWColor(cGrade);
		cardsVec.at(1)->setPosition(-szInfoWnd.width / 2.0f + indent + cardSize.width / 2.0f, -szInfoWnd.height / 2.0f + indent + cardSize.height / 2.0f);	
		labelC = cocos2d::Label::createWithTTF(*gTtfConfig, u8"- 4-7 эл.", cocos2d::TextHAlignment::CENTER);
		setNodeInRect(labelC, textSize, 0);
		cocos2d::Size cLSize = labelC->getContentSize() * labelC->getScale();
		labelC->setPosition(cardsVec.at(1)->getPosition().x + cardSize.width / 1.5f + cLSize.width / 2.0f, cardsVec.at(1)->getPosition().y);
		labelC->setColor(cocos2d::Color3B(200, 200, 200));
		infoWnd->addChild(labelC);

		cocos2d::Label* labelB;
		cardsVec.at(2)->setWColor(bGrade);
		cardsVec.at(2)->setPosition(cardSize.width / 2.0f, szInfoWnd.height / 2.0f - indent * 2.0f - cardSize.height / 2.0f);
		if (mode->cardRareOpen > 2)
		{
			labelB = cocos2d::Label::createWithTTF(*gTtfConfig, u8"- 8-12 эл.", cocos2d::TextHAlignment::CENTER);
			labelB->setColor(cocos2d::Color3B(200, 200, 200));
		}
		else
		{
			labelB = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Закрыто.", cocos2d::TextHAlignment::CENTER);
			labelB->setColor(cocos2d::Color3B(247, 116, 98));
		}
		setNodeInRect(labelB, textSize, 0);
		cocos2d::Size bLSize = labelB->getContentSize() * labelB->getScale();
		labelB->setPosition(cardsVec.at(2)->getPosition().x + cardSize.width / 1.5f + bLSize.width / 2.0f, cardsVec.at(2)->getPosition().y);
		infoWnd->addChild(labelB);

		cocos2d::Label* labelA;
		cardsVec.at(3)->setWColor(aGrade);
		cardsVec.at(3)->setPosition(cardSize.width / 2.0f, -szInfoWnd.height / 2.0f + indent + cardSize.height / 2.0f);
		if (mode->cardRareOpen > 3)
		{
			labelA = cocos2d::Label::createWithTTF(*gTtfConfig, u8"- 13-15 эл.", cocos2d::TextHAlignment::CENTER);
			labelA->setColor(cocos2d::Color3B(200, 200, 200));
		}
		else
		{
			labelA = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Закрыто.", cocos2d::TextHAlignment::CENTER);
			labelA->setColor(cocos2d::Color3B(247, 116, 98));
		}
		setNodeInRect(labelA, textSize, 0);
		cocos2d::Size aLSize = labelA->getContentSize() * labelA->getScale();
		labelA->setPosition(cardsVec.at(3)->getPosition().x + cardSize.width / 1.5f + aLSize.width / 2.0f, cardsVec.at(3)->getPosition().y);
		infoWnd->addChild(labelA);
	}

	cWindow* priceWnd = nullptr;
	cocos2d::Size szPriceWnd = { szIBG.width - indent * 2.0f, freeSpace * 0.2f };
	{
		priceWnd = new cWindow(szPriceWnd, W_MUTE, 10);
		priceWnd->setPosition(0, infoWnd->getPosition().y - szInfoWnd.height / 2.0f - indent * 2.0f - szPriceWnd.height / 2.0f);
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

		float hightCard = szPriceWnd.height - indent * 3.0f;
		float widthCard = hightCard / (88.8f / 63.5f);
		cocos2d::Size cardSize = { widthCard, hightCard };

		dinamicCard = new cWindow(cardSize * 2.0f, W_MUTE, 10);
		dinamicCard->setWColor(dGrade);
		dinamicCard->setScale(0.5f);
		dinamicCard->setPosition(-szPriceWnd.width / 2.0f + indent + cardSize.width, -szPriceWnd.height / 2.0f + indent + cardSize.height / 2.0f);
		dinamicCard->setRotation(20);
		priceWnd->addChild(dinamicCard);
		cocos2d::Sprite *cardCQuestion = cocos2d::Sprite::create("icons/question.png");
		cardCQuestion->setColor(cocos2d::Color3B(96, 139, 171));
		setNodeInRect(cardCQuestion, cardSize * 2.0f, cardSize.width * 2.0f * 0.2f);
		dinamicCard->addChild(cardCQuestion);


		cocos2d::Size textSize = { szPriceWnd.width - cardSize.width * 1.5f - indent * 3.0f, szPriceWnd.height};
		cocos2d::Label* labelCard = cocos2d::Label::createWithTTF(*gTtfConfig, u8"- Случайная карта из собраных.", cocos2d::TextHAlignment::CENTER);
		setNodeInRect(labelCard, textSize, 0);
		cocos2d::Size aLSize = labelCard->getContentSize() * labelCard->getScale();
		labelCard->setPosition(dinamicCard->getPosition().x + cardSize.width / 1.5f + aLSize.width / 2.0f, dinamicCard->getPosition().y);
		labelCard->setColor(cocos2d::Color3B(200, 200, 200));
		priceWnd->addChild(labelCard);
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
	listenNode->listener = cocos2d::EventListenerTouchOneByOne::create();
	listenNode->listener->setSwallowTouches(true);
	listenNode->listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		return true;
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listenNode->listener, -10);
	layer->addChild(listenNode);
}
cSMCardIntro::~cSMCardIntro()
{
}
void cSMCardIntro::step(float dt)
{
	float fullTime = 0.2f;
	cocos2d::Vec2 sz = this->getParent()->convertToWorldSpace(this->getPosition());
	layer->setPosition(-sz.x, -sz.y);

	timeChange += dt;

	float dlocTime = fullTime - timeChange;
	if (fullTime - timeChange > 0)
	{
		cocos2d::Color3B dColor = cocos2d::Color3B((gradeColors.at(nextColor).r - gradeColors.at(prevColor).r) / fullTime * timeChange,
			(gradeColors.at(nextColor).g - gradeColors.at(prevColor).g) / fullTime * timeChange,
			(gradeColors.at(nextColor).b - gradeColors.at(prevColor).b) / fullTime * timeChange);

		cocos2d::Color3B resultColor = cocos2d::Color3B(gradeColors.at(prevColor).r + dColor.r, gradeColors.at(prevColor).g + dColor.g, gradeColors.at(prevColor).b + dColor.b);
		dinamicCard->setWColor(resultColor);
	}
	else
	{
		prevColor = nextColor;
		if (nextColor == 3)nextColor = 0;
		else nextColor++;
		timeChange = 0;
	}

}
void cSMCardIntro::reset()
{
}

///
cSMCardGame::cSMCardGame(cocos2d::Size size, int inTurnCount, int days, std::vector<int> cDropInfo)
{
	dayCost = days;
	turnCount = inTurnCount;
	turns = 0;
	elementCount = 25;
	addtime = 0;
	cardDropInfo = cDropInfo;
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	auto userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;

	float indent = visibleSize.height / 70.0f;
	float halfindent = indent / 2.0f;

	if (size.width > size.height * 0.6f) fieldSize = cocos2d::Size(size.height * 0.6f, size.height * 0.6f);
	else fieldSize = cocos2d::Size(visibleSize.width - indent * 2.0f, size.width);

	//base elements
	{	
		btmBar = new cWindow(fieldSize, W_MUTE, 0);
		ifSize = cocos2d::Size(btmBar->getWSize().width, size.height - fieldSize.height - indent);
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
		float b = fieldSize.width - indent * 2.0f;
		field->setScale((fieldSize.width - indent * 2.0f) / visibleSize.width);
		field->setPosition(-fieldSize.width / 2.0f + indent, -fieldSize.height / 2.0f + indent);
		field->setLocalZOrder(3);

		infoField = new cWindow(ifSize, W_MUTE, 0);
		infoField->setPosition(btmBar->getPosition().x, btmBar->getPosition().y + btmBar->getWSize().height / 2.0f + indent + ifSize.height / 2.0f);
		infoField->setWColor(cocos2d::Color3B(0, 0, 0));
		infoField->setCascadeOpacityEnabled(false);
		auto ch = infoField->getChildren();
		for (auto it : ch) it->setOpacity(100);
		this->addChild(infoField);
	}

	//cards
	{
		cfSize = cocos2d::Size(ifSize.width - indent * 2.0f, ifSize.height * 0.6f);
		cardField = new cWindow(cfSize, W_MUTE, 0);
		cardField->setPosition(0, -ifSize.height /2.0f + cfSize.height / 2.0f + indent);
		cardField->setWColor(cocos2d::Color3B(0, 0, 0));
		cardField->setCascadeColorEnabled(false);
		auto ch = cardField->getChildren();
		for (auto it : ch) it->setOpacity(100);
		infoField->addChild(cardField);

		cocos2d::Sprite *logoCard = cocos2d::Sprite::create("icons/cards.png");
		setNodeInRect(logoCard, cfSize, cfSize.height * 0.3f);
		logoCard->setColor(cocos2d::Color3B(0, 0, 0));
		logoCard->setOpacity(80);
		cardField->addChild(logoCard);
	}

	//turnBar
	{
		cocos2d::Size lnSize = { (ifSize.width - indent * 2.0f - halfindent) * 0.25f, ifSize.height / 11.0f };
		lbSize = cocos2d::Size{ (ifSize.width - indent * 2.0f - halfindent) * 0.75f, lnSize.height };

		cWindow* progressLabelBg = new cWindow(lnSize, W_MUTE, 0);
		progressLabelBg->setPosition(0, ifSize.height / 2.0f - indent - lnSize.height / 2.0f);
		progressLabelBg->setWColor(cocos2d::Color3B(0, 0, 0));
		progressLabelBg->setCascadeOpacityEnabled(false);
		auto ch = progressLabelBg->getChildren();
		for (auto it : ch) it->setOpacity(100);

		infoField->addChild(progressLabelBg);

		cWindow* progressBarBg = new cWindow(lbSize, W_MUTE, 0);
		progressBarBg->setPosition(progressLabelBg->getPosition().x, progressLabelBg->getPosition().y - halfindent - lbSize.height / 2.0f - lnSize.height / 2.0f);
		progressBarBg->setWColor(cocos2d::Color3B(0, 0, 0));
		progressBarBg->setOpacity(100);
		infoField->addChild(progressBarBg);

		progressBarFill = new cWindow(lbSize, W_MUTE, 0);
		progressBarFill->setWColor(cocos2d::Color3B(70, 200, 70));
		progressBarFill->setCascadeOpacityEnabled(false);
		ch = progressBarFill->getChildren();
		for (auto it : ch) it->setOpacity(200);
		progressBarBg->addChild(progressBarFill);

		cocos2d::Label *scoreLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Ходы", cocos2d::TextHAlignment::CENTER);
		setNodeSize(scoreLabel, lnSize.height * 0.6f, true, false);
		progressLabelBg->addChild(scoreLabel);

		lCurrentScore = cocos2d::Label::createWithTTF(*gTtfConfig, std::to_string(turnCount), cocos2d::TextHAlignment::CENTER);
		setNodeSize(lCurrentScore, lbSize.height * 0.6f, true, false);
		cocos2d::Size scCrrLabelSize = lCurrentScore->getContentSize() * lCurrentScore->getScale();
		lCurrentScore->setPosition(0, 0);
		progressBarFill->addChild(lCurrentScore);
	}
	updateInfoBar();
}
cSMCardGame::~cSMCardGame()
{
}
void cSMCardGame::step(float dt)
{
	updateGameStats(dt);
	if (addtime > 0.2 && (int)field->getElements().size() < elementCount)
	{
		int randNum = rand() % 4;
		field->addElement(randNum + 1);
		addtime = 0;
	}
	addtime += dt;


	updateCardPos();
}
void cSMCardGame::reset()
{
}
void cSMCardGame::updateInfoBar()
{
	std::string progressVal = std::to_string(turnCount - turns);
	lCurrentScore->setString(progressVal);
	setNodeSize(lCurrentScore, lbSize.height * 0.6f, true, false);

	float proc = 100 - turns / (turnCount / 100.0f);
	progressBarFill->setWSize(cocos2d::Size(lbSize.width * (proc / 100.0f), lbSize.height));
}
void cSMCardGame::updateGameStats(float dt)
{
	//Расчет результата
	if (field->getTurnCount() != turns)
	{
		auto elements = field->getLastLine();
		
		for(unsigned int i = 0; i < cardDropInfo.size(); i++)
			if (elements.size() <= cardDropInfo.at(i) || i == cardDropInfo.size() -1)
			{
				addCard(i); break;
			}		
		turns = field->getTurnCount();


		updateInfoBar();
		checkGameFinish();
	}
}
void cSMCardGame::checkGameFinish()
{
	if (turns >= turnCount)
	{
		cUserDefault *userDefault = cUserDefault::getInstance();
		((cSMCard*)userDefault->activeMode)->turns = turns;
		((cSMCard*)userDefault->activeMode)->cardRares = cardRares;
		cEndDialog *endDialog = new cEndDialog();
		this->addChild(endDialog);
	}
}
void cSMCardGame::updateCardPos()
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	float indent = visibleSize.height / 70.0f;
	float hightCard = cfSize.height - indent * 4.0f;
	float widthCard = hightCard / (88.8f / 63.5f);

	float widthField = cfSize.width - widthCard * 2.0f;

	for (unsigned int i = 0; i < sCardNodes.size(); i++)
	{
		cocos2d::Size newPos = { -cfSize.width / 2.0f + widthCard + (widthField / (sCardNodes.size() + 1)) * (i + 1), 0 };
		sCardNodes.at(i)->setPosition(newPos);
	}
}
void cSMCardGame::addCard(int rare)
{
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	float indent = visibleSize.height / 70.0f;

	float hightCard = cfSize.height - indent * 4.0f;
	float widthCard = hightCard / (88.8f / 63.5f);
	cocos2d::Size cardSize = { widthCard, hightCard };

	cWindow *card = new cWindow(cardSize * 2.0f, W_MUTE, 10);
	card->setScale(0.5f);
	int randAngle = rand() % 10;
	card->setRotation(20);
	card->setOpacity(240);
	cocos2d::Color3B clr;
	switch (rare)
	{
	case 0:
		clr = dGrade;
		break;
	case 1:
		clr = cGrade;
		break;
	case 2:
		clr = bGrade;
		break;
	case 3:
		clr = aGrade;
		break;
	}
	int rndColor = rand() % 10;
	clr.r += rndColor;
	clr.g += rndColor;
	clr.b += rndColor;
	card->setWColor(clr);
	cardField->addChild(card);

	cocos2d::Sprite *cardDQuestion = cocos2d::Sprite::create("icons/question.png");
	cardDQuestion->setColor(cocos2d::Color3B(0, 0, 0));
	cardDQuestion->setOpacity(100);
	setNodeInRect(cardDQuestion, cardSize * 2.0f, cardSize.width * 2.0f * 0.2f);
	card->addChild(cardDQuestion);

	sCardNodes.push_back(card);
	cardRares.push_back(rare);
}


///
cSMCardOutro::cSMCardOutro(int turns, int turnCounts, int dayCost, std::vector<int> cardsRares)
{
	cUserDefault *userDefault = cUserDefault::getInstance();
	auto gTtfConfig = userDefault->font;
	auto gTtfConfigHd = userDefault->fontHd;

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	float indent = visibleSize.height / 70.0f;

	float topInterface = visibleSize.height / 20.0f,
		btmInterface = visibleSize.width / 2.5f / 3.0f,
		mdlInterface = (visibleSize.height - topInterface - btmInterface - (4 * indent)) * 0.6f;

	studentCard = nullptr;

	//Победа порожение 
	{
		cocos2d::Size szResultWnd = { visibleSize.width - indent * 4.0f, mdlInterface };

		resultWnd = new cWindow(szResultWnd, W_MUTE, 10);
		resultWnd->setCascadeOpacityEnabled(false);
		auto ch = resultWnd->getChildren();
		for (auto it : ch) it->setOpacity(100);
		this->addChild(resultWnd);

		szScoreWnd = cocos2d::Size{ szResultWnd.width - indent * 4.0f, szResultWnd.height / 3.0f };
		{
			scoreWnd = new cWindow(szScoreWnd, W_MUTE, 10);
			scoreWnd->setPosition(0, szResultWnd.height / 2.0f - indent * 2.0 - szScoreWnd.height / 2.0f);
			scoreWnd->setWColor(cocos2d::Color3B(96, 139, 171));
			resultWnd->addChild(scoreWnd);

			cocos2d::Size sizeTitle = { szScoreWnd.width / 2.5f, indent * 2.0f };
			cWindow* infoTitle = new cWindow(sizeTitle, W_MUTE, 10);
			infoTitle->setWColor(cocos2d::Color3B(200, 200, 200));
			infoTitle->setPosition(0, szScoreWnd.height / 2.0f);
			scoreWnd->addChild(infoTitle);

			cocos2d::Label* titleLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Карты", cocos2d::TextHAlignment::CENTER);
			titleLabel->setColor(cocos2d::Color3B(96, 139, 171));
			setNodeInRect(titleLabel, sizeTitle, sizeTitle.width * 0.05f);
			infoTitle->addChild(titleLabel);

			roullet = new cCardRoulette(cardsRares, szScoreWnd);
			scoreWnd->addChild(roullet);
		}

		szMarkWnd = cocos2d::Size{ szScoreWnd.width, szResultWnd.height - szScoreWnd.height - indent * 6.0f };
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

		}
	}

	//bottom btns
	{
		cocos2d::Size btnSize = cocos2d::Size(visibleSize.width / 2.5f, visibleSize.width / 2.5f / 3.0f);
		cButton *exitBtn = new cButton(nullptr, btnSize, 1.05f, -2);
		this->addChild(exitBtn);
		exitBtn->wnd->setWColor(cocos2d::Color3B(96, 139, 171));

		cocos2d::Label* okLabel = cocos2d::Label::createWithTTF(*gTtfConfig, u8"Завершить", cocos2d::TextHAlignment::CENTER);
		setNodeInRect(okLabel, btnSize, btnSize.height * 0.6f);
		okLabel->setColor(cocos2d::Color3B(200, 200, 200));
		exitBtn->addChild(okLabel);
		exitBtn->btnUp = [&](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			if (studentCard == nullptr) return true;

			cUserDefault *userDefault = cUserDefault::getInstance();
			auto students = userDefault->generatedCards["studentCards"];

			auto level = userDefault->profile["aLevel"];
			std::vector<std::string> vec = Split(studentCard->rData, '/');
			students[vec.at(0)] = vec.at(1);
			userDefault->saveCards();

			cSceneManager *sceneManager = cSceneManager::getInstance();
			sceneManager->changeScene("menu");
			return true;
		};


		cocos2d::Size resBtnSize = { btnSize.height, btnSize.height };
		cButton *restartBtn = new cButton(nullptr, resBtnSize, 1.05f, -2);
		this->addChild(restartBtn);
		restartBtn->wnd->setWColor(cocos2d::Color3B(96, 139, 171));
		cocos2d::Sprite *icon_res = cocos2d::Sprite::create("icons/res.png");
		setNodeInRect(icon_res, resBtnSize, btnSize.height * 0.2f);
		icon_res->setOpacity(150);
		icon_res->setLocalZOrder(4);
		restartBtn->addChild(icon_res);
		restartBtn->btnUp = [=](cocos2d::Touch* touch, cocos2d::Node* node)
		{
			cSceneManager *sceneManager = cSceneManager::getInstance();
			sceneManager->changeScene("game");
			return true;
		};

		float btnLineSize = btnSize.width + resBtnSize.width + indent;
		exitBtn->setPosition(btnLineSize / 2.0f - btnSize.width / 2.0f, -(visibleSize.height / 2.0f) + indent * 2.0f + btnSize.height / 2.0f);
		restartBtn->setPosition(-btnLineSize / 2.0f + resBtnSize.width / 2.0f, -(visibleSize.height / 2.0f) + indent * 2.0f + btnSize.height / 2.0f);

	}

}
cSMCardOutro::~cSMCardOutro()
{
}
void cSMCardOutro::step(float dt)
{
	if (!roullet->isActive() && studentCard == nullptr)
	{
		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		float indent = visibleSize.height / 70.0f;

		cBaseNode* card = roullet->getActiveCard();
		cUserDefault *userDefault = cUserDefault::getInstance();
		std::string data = userDefault->generateCard(card->getTag());

		std::vector<std::string> vec = Split(data, '/');
		studentCard = new cStudentCard(vec.at(0), vec.at(1));
	

		sCard = studentCard->getCard(userDefault->font, 1, roullet->cardSize.width / 2.0f);

		sCard->setPosition(card->getPosition());
		sCard->setOpacity(0);
		scoreWnd->addChild(sCard);
		timeFade = 0;
		
		{
			infoWnd = new cBaseNode();
			cocos2d::Node *infoCard = studentCard->getCard(userDefault->font, 1, (szMarkWnd.height - indent * 2.5f) / 2.0f);
			float widthCard = szMarkWnd.height / (88.8f / 63.5f);

			infoCard->setPosition(-szMarkWnd.width / 2.0f + widthCard / 2.0f, -indent  / 2.0f);
			infoWnd->addChild(infoCard);

			cocos2d::Size textSize = { szMarkWnd.width - widthCard - indent, szMarkWnd.height - indent * 2.0f};
			cocos2d::Label* nameLabel = cocos2d::Label::createWithTTF(*userDefault->font, studentCard->name, cocos2d::TextHAlignment::CENTER);
			nameLabel->setColor(cocos2d::Color3B(200, 200, 200));
			setNodeInRect(nameLabel, textSize, 0);
			cocos2d::Size nlSize = nameLabel->getContentSize() * nameLabel->getScale();
			nameLabel->setPosition(-szMarkWnd.width / 2.0f + widthCard + nlSize.width / 2.0f, nlSize.height / 2.0f + indent * 2.0f);
			infoWnd->addChild(nameLabel);

			cocos2d::Size tSubject = { textSize.height / 3.0f, textSize.height / 3.0f };
			cocos2d::Sprite *teacherSubject = cocos2d::Sprite::create("gameScreen/" + std::to_string(studentCard->subject + 1) + ".png");
			setNodeInRect(teacherSubject, tSubject, 0);
			teacherSubject->setPosition(nameLabel->getPosition().x - tSubject.width / 2.0f, -indent -tSubject.height / 2.0f);
			infoWnd->addChild(teacherSubject);

			cocos2d::Label* skillLabel = cocos2d::Label::createWithTTF(*userDefault->font,"+" + std::to_string(studentCard->stat).substr(0, std::to_string(studentCard->stat).find('.') + 2), cocos2d::TextHAlignment::CENTER);
			skillLabel->setColor(cocos2d::Color3B(70, 200, 70));
			setNodeInRect(skillLabel, tSubject, tSubject.width * 0.01f);
			cocos2d::Size slSize = skillLabel->getContentSize() * skillLabel->getScale();
			skillLabel->setPosition(teacherSubject->getPosition().x + tSubject.width + indent / 2.0f, teacherSubject->getPosition().y);
			infoWnd->addChild(skillLabel);
			infoWnd->setCascadeOpacityEnabled(true);
			infoWnd->setOpacity(0);
			markWnd->addChild(infoWnd);
		}
	}
	if (studentCard != nullptr)
	{
		int TIME_FADE = 3.0f;
		if (timeFade < TIME_FADE)
		{
			float proc = timeFade / (TIME_FADE / 100.0f);

			cBaseNode* card = roullet->getActiveCard();
			card->setOpacity(255.0f - (255.0f / 100.0f * proc));
			sCard->setOpacity(255.0f / 100.0f * proc);
			infoWnd->setOpacity(255.0f / 100.0f * proc);
			timeFade += dt;
		}
		else if(timeFade != 10.0f)
		{
			cBaseNode* card = roullet->getActiveCard();
			card->setOpacity(0);
			sCard->setOpacity(255.0f);
			infoWnd->setOpacity(255.0f);

			timeFade = 10.0f;
		}
	}
}
void cSMCardOutro::reset()
{
}
