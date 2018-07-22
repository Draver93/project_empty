#include "vwGame.hpp"

cVWGame::cVWGame(cocos2d::Size size, cBaseMode *thisMode)
{

	addtime = 0;
	turns = 0;
	elementCount = 23;
	auto userDefault = cUserDefault::getInstance();
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	subjects = ((cVerificationWork*)userDefault->activeMode)->subjects;

	auto gTtfConfig = userDefault->font;
	auto gTtfConfigHd = userDefault->fontHd;

	float indent = visibleSize.height / 70.0f;

	cocos2d::Size btmBarSize, ifSize;
	if (size.width > size.height * 0.6f) btmBarSize = cocos2d::Size(size.height * 0.6f, size.height * 0.6f);
	else btmBarSize = cocos2d::Size(visibleSize.width - indent * 2.0f, size.width);

	ifSize = cocos2d::Size{ btmBarSize.width, size.height - btmBarSize.height - indent };
	scoreSize = cocos2d::Size{ ifSize.height * 0.7f, ifSize.height * 0.7f };

	cWindow* infoField, *btmBar;
	//bars
	{
		btmBar = new cWindow(btmBarSize, W_MUTE, 0);
		cocos2d::Rect btmBarRect = cocos2d::utils::getCascadeBoundingBox(btmBar);
		float barInd = (visibleSize.width - btmBarRect.size.width) / 2.0f;
		btmBar->setPosition(btmBarRect.size.width / 2.0f + barInd, btmBarRect.size.height / 2.0f + indent);
		btmBar->setWColor(cocos2d::Color3B(0, 0, 0));
		btmBar->setOpacity(100);
		btmBar->setLocalZOrder(2);
		this->addChild(btmBar);

		infoField = new cWindow(ifSize, W_MUTE, 0);
		infoField->setPosition(btmBar->getPosition().x, btmBar->getPosition().y + btmBar->getWSize().height / 2.0f + indent + ifSize.height / 2.0f);
		infoField->setWColor(cocos2d::Color3B(0, 0, 0));
		infoField->setCascadeOpacityEnabled(false);
		auto ch = infoField->getChildren();
		for (auto it : ch) it->setOpacity(0);
		this->addChild(infoField);

		field = new cGameField(visibleSize.width, cocos2d::Vec2(8, 7));
		btmBar->addChild((cocos2d::Node*)field);
		float a = (visibleSize.width - indent * 4.0f);
		float b = btmBarSize.width - indent * 2.0f;
		field->setScale((btmBarSize.width - indent * 2.0f) / visibleSize.width);
		field->setPosition(-btmBarSize.width / 2.0f + indent, -btmBarSize.height / 2.0f + indent);
		field->setLocalZOrder(3);
	}

	cocos2d::Sprite *scoreBar = NULL;
	cocos2d::Sprite *turnBar = NULL;
	//circles 2
	{
		scoreBar = cocos2d::Sprite::create("icons/element.png");
		scoreBar->setColor(cocos2d::Color3B(0, 0, 0));
		scoreBar->setOpacity(100);
		scoreBar->setContentSize(scoreSize);

		floatElement *feScore = new floatElement();
		feScore->name = "Score";
		feScore->node = new cBaseNode();
		feScore->node->addChild(scoreBar);
		feScore->originPos = { 0, 0 };
		infoField->addChild(feScore->node);
		elements.push_back(feScore);

		textScoreSize = cocos2d::Size{ scoreSize.width * 1.5f, scoreSize.height * 0.4f };
		scoreLabel = cocos2d::Label::createWithTTF(*gTtfConfigHd, "000", cocos2d::TextHAlignment::CENTER);
		scoreLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.58f));
		setNodeInRect(scoreLabel, textScoreSize * 0.8f, 0);
		scoreLabel->setColor(cocos2d::Color3B(230, 230, 230));
		scoreLabel->setPosition(scoreSize / 2.0f);
		scoreBar->addChild(scoreLabel);

		///////////

		turnSize = cocos2d::Size{ scoreSize * 0.4f };
		turnBar = cocos2d::Sprite::create("icons/element.png");
		turnBar->setColor(cocos2d::Color3B(0, 0, 0));
		turnBar->setOpacity(100);
		turnBar->setContentSize(turnSize);

		float rad = -60 * 3.14f / 180.0f;
		floatElement *feTurn = new floatElement();
		feTurn->name = "Turn";
		feTurn->node = new cBaseNode();
		feTurn->node->addChild(turnBar);
		feTurn->originPos = { textScoreSize.width * 0.5f * sin(rad), textScoreSize.width * 0.5f * cos(rad) };
		infoField->addChild(feTurn->node);
		elements.push_back(feTurn);

		testTurnSize = turnSize * 0.8f;
		turnLabel = cocos2d::Label::createWithTTF(*gTtfConfigHd, "25", cocos2d::TextHAlignment::CENTER);
		turnLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.58f));
		setNodeInRect(turnLabel, testTurnSize, 0);
		turnLabel->setColor(cocos2d::Color3B(230, 230, 230));
		turnLabel->setPosition(turnSize / 2.0f);
		turnBar->addChild(turnLabel);
	}

	//text
	{
		cocos2d::Label *scoreDesc = cocos2d::Label::createWithTTF(*gTtfConfigHd, u8"Игровой счет", cocos2d::TextHAlignment::CENTER);
		scoreDesc->setAnchorPoint(cocos2d::Vec2(0.5f, 1.0f));
		scoreDesc->setColor(cocos2d::Color3B(230, 230, 230));
		setNodeInRect(scoreDesc, cocos2d::Size(textScoreSize.width * 2.2f, textScoreSize.height * 0.3f), 0);
		scoreDesc->setPosition(scoreSize.width / 2.0f, 0);
		scoreBar->addChild(scoreDesc);

		cocos2d::Label *turnDesc = cocos2d::Label::createWithTTF(*gTtfConfigHd, u8"Ходы", cocos2d::TextHAlignment::CENTER);
		turnDesc->setAnchorPoint(cocos2d::Vec2(0.5f, 1.0f));
		turnDesc->setColor(cocos2d::Color3B(230, 230, 230));
		setNodeInRect(turnDesc, cocos2d::Size(testTurnSize.width * 2.2f, testTurnSize.height * 0.3f), 0);
		turnDesc->setPosition(turnSize.width / 2.0f, 0);
		turnBar->addChild(turnDesc);


		cardDesc = cocos2d::Label::createWithTTF(*gTtfConfigHd, u8"Карты", cocos2d::TextHAlignment::CENTER);
		turnDesc->setAnchorPoint(cocos2d::Vec2(0.5f, 1.0f));
		cardDesc->setColor(cocos2d::Color3B(230, 230, 230));
		setNodeInRect(cardDesc, cocos2d::Size(testTurnSize.width * 1.8f, testTurnSize.height * 0.2f), 0);
		infoField->addChild(cardDesc);
	}

	//circles 4
	{
		cardSize = scoreSize * 0.25f;
		trCardSize = turnSize;

		cocos2d::Sprite *back = cocos2d::Sprite::create("cards/btm.png");
		float hightRatio = back->getContentSize().height / back->getContentSize().width;

		float angle = 20.0f, shift = 60.0f;
		textScoreSize.width;
		auto cardIds = userDefault->profile["aActiveCards"];
		auto pos = scoreBar->getPosition();

		std::vector<cCard*> activeCards;
		activeCards.resize(4);

		for (int i = 0; i < cardIds.size(); i++)
		{
			auto obj = cardIds.at(i).begin();
			for(auto &it : userDefault->cards)
				if (it->id == obj.key())
					activeCards.at(obj.value() - 1) = it;
		}

		for (int i = 0; i < 4; i++)
		{
			floatElement *elem = new floatElement();
			elem->name = "el_" + std::to_string(i);

			float rad = (shift + angle * i) * 3.14f/ 180.0f;
			cocos2d::Sprite *el = cocos2d::Sprite::create("icons/element.png");
			el->setTag(i);
			el->setContentSize(cardSize);
			el->setColor(cocos2d::Color3B(0, 0, 0));
			el->setOpacity(100);
			el->setRotation(20);

			elem->node = new cBaseNode();
			elem->node->addChild(el);
			elem->originPos = { textScoreSize.width * 0.5f * sin(rad), textScoreSize.width * 0.5f * cos(rad) };
			infoField->addChild(elem->node);
			elements.push_back(elem);
			if (activeCards.at(i) != nullptr)
			{
				cocos2d::Node *card = activeCards.at(i)->getCard(gTtfConfig, 1, cardSize.width / hightRatio / 1.5f);
				card->setPosition(cardSize / 2.0f);
				card->setCascadeOpacityEnabled(true);
				card->setOpacity(200);
				el->addChild(card);
			}
		}

		//tr
		{
			floatElement *elem = new floatElement();
			elem->name = "tr";

			float rad = -120 * 3.14f / 180.0f;
			cocos2d::Sprite *el = cocos2d::Sprite::create("icons/element.png");
			
			el->setTag(5);
			el->setContentSize(trCardSize);
			el->setColor(cocos2d::Color3B(0, 0, 0));
			el->setOpacity(100);
			el->setRotation(20);

			elem->node = new cBaseNode();
			elem->node->addChild(el);
			elem->originPos = { textScoreSize.width * 0.5f * sin(rad), textScoreSize.width * 0.5f * cos(rad) };
			infoField->addChild(elem->node);
			elements.push_back(elem);
			//test
			cocos2d::Node *card = activeCards.at(0)->getCard(gTtfConfig, 1, trCardSize.width / hightRatio / 1.5f);
			card->setPosition(trCardSize / 2.0f);
			card->setCascadeOpacityEnabled(true);
			card->setOpacity(200);
			el->addChild(card);

			cocos2d::Label *trDesc = cocos2d::Label::createWithTTF(*gTtfConfigHd, u8"Учитель", cocos2d::TextHAlignment::CENTER);
			trDesc->setAnchorPoint(cocos2d::Vec2(0.5f, 1.0f));
			trDesc->setColor(cocos2d::Color3B(230, 230, 230));
			setNodeInRect(trDesc, cocos2d::Size(testTurnSize.width * 2.2f, testTurnSize.height * 0.3f), 0);
			trDesc->setPosition(0, -turnSize.height / 2.0f);
			elem->node->addChild(trDesc);
		}
	}
}
cVWGame::~cVWGame()
{
}

void cVWGame::updateInfoBar()
{
	scoreLabel->setString(std::to_string(currentScore.get()).substr(0, std::to_string(currentScore.get()).find('.') + 2));
	setNodeInRect(scoreLabel, scoreSize * 0.8f, 0);
	turnLabel->setString(std::to_string(int(turnCount - turns > 0 ? turnCount - turns : 0)));
	setNodeInRect(turnLabel, turnSize * 0.6f, 0);
}
void cVWGame::updateGameStats(float dt)
{
	currentScore.update(dt);
	if (currentScore.isProgress())
	{
		updateInfoBar();
		checkGameFinish();

		float height = 0;
		float width = 0;
		for (auto &it : elements)
		{
			it->update(dt, testTurnSize.width / 20.0f);
			if (it->name.substr(0, 3) == "el_")
				width += it->node->getPosition().x;
			if (it->name == "el_3")
			{
				height = it->node->getPosition().y;
			}
		}
		width /= 4.0f;
		cardDesc->setPosition(width, height - cardSize.height / 1.5f);
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
					result += it2->stat;
				}
			}
		}
		///Расчеты

		if (incr.size() >= elements.size())result *= incr.at(elements.size() - 1);
		else result *= incr.back();

		if (!currentScore.isProgress())
			currentScore.set(currentScore.get() + result, 3, true);
		else
		{
			float point = currentScore.get_nVal();
			currentScore.set(currentScore.get());
			currentScore.set(point + result, 3, true);
		}
		turns = field->getTurnCount();
	}
}
void cVWGame::checkGameFinish()
{
	if (turns >= turnCount || currentScore.get() >= goal)
	{
		currentScore.set(currentScore.get_nVal());
		cUserDefault *userDefault = cUserDefault::getInstance();
		((cVerificationWork*)userDefault->activeMode)->currentScore = currentScore.get();
		((cVerificationWork*)userDefault->activeMode)->turns = turns;
		cEndDialog *endDialog = new cEndDialog();
		this->addChild(endDialog);
	}
}

void cVWGame::step(float dt)
{
	updateGameStats(dt);
	if (addtime > 0.2 && (int)field->getElements().size() < elementCount)
	{
		int randNum = rand() % 4;
		field->addElement(subjects.at(randNum));
		addtime = 0;
	}
	addtime += dt;
}
void cVWGame::reset()
{
}
