#include "CardRoulette.hpp"

cCardRoulette::cCardRoulette(std::vector<int> cards, cocos2d::Size size)
{
	enable = false;
	inTime = moveTime = outTime = -1;
	speed = 0;
	activeNode = nullptr;
	wndSize = size;
	vecCards = cards;
	this->autorelease();
	this->scheduleUpdate();
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	float indent = visibleSize.height / 70.0f;

	float hightCard = size.height - indent * 3.0f;
	float widthCard = hightCard / (88.8f / 63.5f);
	cardSize = cocos2d::Size{ widthCard, hightCard };

	for (unsigned int i = 0; i < cards.size(); i++)
	{
		cWindow *card = new cWindow(cardSize, W_MUTE, 10);
		card->setTag(cards.at(i));
		card->setPosition((widthCard + indent) * i, -indent / 2.0f);
		cocos2d::Color3B clr;
		switch (cards.at(i))
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
		card->setWColor(clr);
		this->addChild(card);

		cocos2d::Sprite *cardDQuestion = cocos2d::Sprite::create("icons/question.png");
		cardDQuestion->setColor(cocos2d::Color3B(0, 0, 0));
		cardDQuestion->setOpacity(100);
		setNodeInRect(cardDQuestion, cardSize, cardSize.width * 0.2f);
		card->addChild(cardDQuestion);

		sCardNodes.push_back(card);
	}

	start(2.16f, 7.58f, 1.35f, 400);
}
cCardRoulette::~cCardRoulette()
{
}

void cCardRoulette::update(float dt)
{
	if (enable)
	{
		if (time < (inTime + moveTime + outTime))
		{
			float currentSpeed = 0;

			if (time <= inTime)
			{
				float proc = time / (inTime / 100.0f);
				currentSpeed = speed / 100.0f * proc;
			}
			else if (time <= inTime + moveTime)
			{
				currentSpeed = speed;
			}
			else if (time <= inTime + moveTime + outTime)
			{
				float proc = 100.0f - (time - (inTime + moveTime)) / (outTime / 100.0f);
				currentSpeed = speed / 100.0f * proc;
			}

			auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
			float indent = visibleSize.height / 70.0f;

			float hightCard = wndSize.height - indent * 3.0f;
			float widthCard = hightCard / (88.8f / 63.5f);
			int indentCard = abs(sCardNodes.at(0)->getPosition().x);
			for (unsigned int i = 0; i < sCardNodes.size(); i++)
			{
				sCardNodes.at(i)->setPosition(sCardNodes.at(i)->getPosition().x + currentSpeed * dt, sCardNodes.at(i)->getPosition().y);

				if (indentCard > abs(sCardNodes.at(i)->getPosition().x))
				{
					indentCard = abs(sCardNodes.at(i)->getPosition().x);
					activeNode = sCardNodes.at(i);
				}

				if (sCardNodes.at(i)->getPosition().x > wndSize.width / 2.0 - widthCard / 2.0f - indent * 2.0f)sCardNodes.at(i)->setVisible(false);
				else if (sCardNodes.at(i)->getPosition().x < -wndSize.width / 2.0 + widthCard / 2.0f + indent * 2.0f)sCardNodes.at(i)->setVisible(false);
				else
				{
					sCardNodes.at(i)->setVisible(true);

					float alpha = 100.0f - abs(sCardNodes.at(i)->getPosition().x) / ((wndSize.width / 2.0 - widthCard / 2.0f - indent * 2.0f) / 100.0f);
					sCardNodes.at(i)->setOpacity(255.0f / 100.0f * alpha);
					sCardNodes.at(i)->setScale(0.5f + (0.5f / 100.0f * alpha));
				}
			}

			int widthCardLine = ((sCardNodes.size() * widthCard) + (sCardNodes.size() - 1 * indent)) / 2.0f + widthCard / 4.0f;
			while (widthCardLine < sCardNodes.back()->getPosition().x)
			{
				sCardNodes.back()->setPosition(sCardNodes.front()->getPosition().x - (widthCard + indent), sCardNodes.front()->getPosition().y);
				sCardNodes.insert(sCardNodes.begin(), sCardNodes.back());

				sCardNodes.pop_back();
			}

			time += dt;
		}
		else
		{
			enable = false;
		}
	}
	else
	{
		if (activeNode->getPosition().x != 0)
		{
			float widthIndent = 0;
			float speedIndent = speed / 2.0f;

			if (speedIndent * dt < activeNode->getPosition().x)
				widthIndent = -(speedIndent * dt);
			else 
				widthIndent = -activeNode->getPosition().x;

			if (activeNode->getPosition().x < 0) widthIndent *= -1;

			auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
			float indent = visibleSize.height / 70.0f;

			float hightCard = wndSize.height - indent * 3.0f;
			float widthCard = hightCard / (88.8f / 63.5f);

			for (unsigned int i = 0; i < sCardNodes.size(); i++)
			{
				sCardNodes.at(i)->setPosition(sCardNodes.at(i)->getPosition().x + widthIndent, sCardNodes.at(i)->getPosition().y);

				if (sCardNodes.at(i)->getPosition().x > wndSize.width / 2.0 - widthCard / 2.0f - indent * 2.0f)sCardNodes.at(i)->setVisible(false);
				else if (sCardNodes.at(i)->getPosition().x < -wndSize.width / 2.0 + widthCard / 2.0f + indent * 2.0f)sCardNodes.at(i)->setVisible(false);
				else
				{
					sCardNodes.at(i)->setVisible(true);

					float alpha = 100.0f - abs(sCardNodes.at(i)->getPosition().x) / ((wndSize.width / 2.0 - widthCard / 2.0f - indent * 2.0f) / 100.0f);
					sCardNodes.at(i)->setOpacity(255.0f / 100.0f * alpha);
					sCardNodes.at(i)->setScale(0.5f + (0.5f / 100.0f * alpha));
				}
			}
		}
		else
		{
			reset();
		}
	}
}
void cCardRoulette::start(float in, float move, float out, float spd)
{
	time = 0;
	inTime = in;
	moveTime = move;
	outTime = out;
	speed = spd;

	enable = true;
}
void cCardRoulette::reset()
{
	time = 0;
	inTime = moveTime = outTime = -1;
	speed = 0;

	enable = false;
}
